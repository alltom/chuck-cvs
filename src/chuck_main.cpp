/*----------------------------------------------------------------------------
    ChucK Concurrent, On-the-fly Audio Programming Language
      Compiler and Virtual Machine

    Copyright (c) 2004 Ge Wang and Perry R. Cook.  All rights reserved.
      http://chuck.cs.princeton.edu/
      http://soundlab.cs.princeton.edu/

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
    U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// file: chuck_main.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
using namespace std;

extern "C"
{
#include "chuck_utils.h"
#include "chuck_errormsg.h"
#include "chuck_socket.h"

extern int yyparse( void );
}

#include "chuck_type.h"
#include "chuck_emit.h"
#include "chuck_instr.h"
#include "chuck_vm.h"
#include "chuck_bbq.h"

#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "ugen_osc.h"
#include "ugen_xxx.h"
#include "ugen_filter.h"
#include "ugen_stk.h"
#include "ulib_machine.h"
#include "ulib_math.h"
#include "ulib_std.h"

// current version
#define CK_VERSION "1.1.4.3"

extern a_Program g_program;
ck_socket g_sock;
Chuck_VM * g_vm = NULL;
t_Env g_env = NULL;
t_CKBOOL g_error = FALSE;
pthread_t g_tid = 0;
char g_host[256] = "127.0.0.1";
int g_port = 8888;
#if defined(__MACOSX_CORE__)
t_CKUINT g_priority = 90;
#else
t_CKUINT g_priority = 0xffffffff;
#endif




//-----------------------------------------------------------------------------
// name: signal_int()
// desc: ...
//-----------------------------------------------------------------------------
void signal_int( int sig_num )
{
    if( g_vm )
    {
        Chuck_VM * vm = g_vm;
        g_vm = NULL;
        fprintf( stderr, "[chuck]: cleaning up...\n" );
        vm->stop();
        vm->shutdown();
        pthread_kill( g_tid, 2 );
        usleep( 100000 );
        delete( vm );
    }
    
    exit(2);
}




char filename[1024] = "";
//-----------------------------------------------------------------------------
// name: parse()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL parse( c_str fname )
{
    t_CKBOOL ret = FALSE;
    strcpy( filename, fname );
    FILE * f = NULL;

    // test it
    if( !(f = fopen( filename, "r" )) )
        if( !strstr( filename, ".ck" ) && !strstr( filename, ".CK" ) )
        {
            strcat( filename, ".ck" );
            if( !(f = fopen( filename, "r" )) )
                strcpy( filename, fname );
        }

    // close it
    if( f ) fclose( f );

    // parse
    EM_reset( filename );
    ret = (yyparse( ) == 0);

    return ret;
}




//-----------------------------------------------------------------------------
// name: type_check()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_check( t_Env env, a_Program prog )
{
    t_CKBOOL ret = FALSE;

    ret = type_engine_check_prog( env, g_program );

    return ret;
}




//-----------------------------------------------------------------------------
// name: emit()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_code( Chuck_Emmission * emit, a_Program prog )
{
    t_CKBOOL ret = FALSE;

    ret = emit_engine_emit_prog( emit, g_program );

    return ret;
}




//-----------------------------------------------------------------------------
// name: dump_instr()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL dump_instr( Chuck_VM_Code * code )
{
    fprintf( stdout, "[chuck]: dumping src/shred '%s'...\n", code->name.c_str() );
    fprintf( stdout, "...\n" );

    for( int i = 0; i < code->num_instr; i++ )
        fprintf( stdout, "'%i' %s( %s )\n", i, 
            code->instr[i]->name(), code->instr[i]->params() );

    fprintf( stdout, "...\n\n" );

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: load_module()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL load_module( t_Env env, f_ck_query query, const char * name, const char * nspc )
{
    Chuck_DLL * dll = NULL;
    
    // load osc
    dll = new Chuck_DLL( name );
    dll->load( query );
    if( !dll->query() || !type_engine_add_dll( env, dll, nspc ) )
    {
        fprintf( stderr, 
                 "[chuck]: internal error loading internal module '%s.%s'...\n", 
                 nspc, name );
        if( !dll->query() )
            fprintf( stderr, "       %s\n", dll->last_error() );
        g_error = TRUE;
        exit(1);
    }    

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: usage()
// desc: ...
//-----------------------------------------------------------------------------
void usage()
{
    fprintf( stdout, "usage: chuck --[options|commands] [+-=^] file1 file2 file3 ...\n" );
    fprintf( stdout, "   [options] = halt|loop|audio|silent|dump|nodump|bufsize<N>|dac<N>|adc<N>|about\n" );
    fprintf( stdout, "   [commands] = add|remove|replace|status\n" );
    fprintf( stdout, "   [+-=^] = short-cuts for add, remove, replace, and status\n\n" );
    fprintf( stdout, "chuck version: %s\n", CK_VERSION );
    fprintf( stdout, "   http://chuck.cs.princeton.edu/\n\n" );
}




//-----------------------------------------------------------------------------
// name: struct Msg()
// desc: ...
//-----------------------------------------------------------------------------
struct Msg
{
    t_CKUINT type;
    t_CKUINT param;
    char buffer[512];
};




//-----------------------------------------------------------------------------
// name: process_msg()
// desc: ...
//-----------------------------------------------------------------------------
extern "C" t_CKUINT process_msg( t_CKUINT type, t_CKUINT param, const char * buffer,
                                 t_CKBOOL immediate )
{
    Msg msg;
    Chuck_Msg cmd;
    
    // copy in
    msg.type = type;
    msg.param = param;
    if( !buffer ) buffer = "";
    strcpy( msg.buffer, buffer );

    // fprintf( stderr, "UDP message recv...\n" );
    if( msg.type == MSG_REPLACE || msg.type == MSG_ADD )
    {
        if( !parse( msg.buffer ) )
            return 0;

        // type check
        if( !type_check( g_env, g_program ) )
            return 0;

        // emit
        Chuck_Emmission * emit = emit_engine_init( g_env );
        if( !emit_code( emit, g_program ) )
            return 0;

        // transform the code
        Chuck_VM_Code * code = emit_to_code( emit );
        code->name = msg.buffer;

        cmd.type = msg.type;
        cmd.code = code;

        if( msg.type == MSG_REPLACE )
            cmd.param = msg.param;
    }
    else if( msg.type == MSG_STATUS || msg.type == MSG_REMOVE || msg.type == MSG_REMOVEALL
             || msg.type == MSG_KILL || msg.type == MSG_TIME )
    {
        cmd.type = msg.type;
        cmd.param = msg.param;
    }
    else
    {
        fprintf( stderr, "[chuck]: unrecognized incoming command from network: '%i'\n", cmd.type );
        return 0;
    }
    
    // immediate
    if( immediate )
        return g_vm->process_msg( &cmd );

    g_vm->queue_msg( &cmd, 1 );

    return 0;
}




//-----------------------------------------------------------------------------
// name: load_internal_modules()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL load_internal_modules( t_Env env )
{
    // load
    load_module( env, osc_query, "osc", "global" );
    load_module( env, xxx_query, "xxx", "global" );
    load_module( env, filter_query, "filter", "global" );
    load_module( env, stk_query, "stk", "global" );
    stk_init( Digitalio::sampling_rate() );

    // load
    load_module( env, machine_query, "machine", "machine" );
    machine_init( g_vm, process_msg );
    load_module( env, libstd_query, "std", "std" );
    load_module( env, libmath_query, "math", "math" );
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: cb()
// desc: ...
//-----------------------------------------------------------------------------
void * cb( void * p )
{
    Msg msg;

    while( true )
    {
        memset( &msg, 0, sizeof(msg) );
        ck_recv( g_sock, (char *)&msg, sizeof(msg) );
        process_msg( msg.type, msg.param, msg.buffer, FALSE );
    }
    
    return NULL;
}




//-----------------------------------------------------------------------------
// name: send_cmd()
// desc: ...
//-----------------------------------------------------------------------------
int send_cmd( int argc, char ** argv, int  & i )
{
    Msg msg;
    memset( &msg, 0, sizeof(msg) );
    g_sock = ck_udp_create();

    if( !ck_connect( g_sock, g_host, g_port ) )
    {
        fprintf( stderr, "[chuck]: cannot open UDP socket on %s:%i\n", g_host, g_port );
        return 1;
    }

    if( !strcmp( argv[i], "--add" ) || !strcmp( argv[i], "+" ) )
    {
        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [add]\n" );
            return 1;
        }

        do {
            ifstream fin;
            fin.open( argv[i] );
            if( !fin.good() )
            {
                fprintf( stderr, "[chuck]: cannot open file '%s' for [add]\n", argv[i] );
                return 1;
            }
            msg.type = MSG_ADD;
            strcpy( msg.buffer, getenv("PWD") ? getenv("PWD") : "" );
            strcat( msg.buffer, getenv("PWD") ? "/" : "" );
            strcat( msg.buffer, argv[i] );
            ck_send( g_sock, (char *)&msg, sizeof(msg) );
        } while( ++i < argc );
    }
    else if( !strcmp( argv[i], "--remove" ) || !strcmp( argv[i], "-" ) )
    {
        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [remove]\n" );
            return 1;
        }

        do {
            msg.param = atoi( argv[i] );
            msg.type = MSG_REMOVE;
            ck_send( g_sock, (char *)&msg, sizeof(msg) );
        } while( ++i < argc );
    }
    else if( !strcmp( argv[i], "--" ) )
    {
        msg.param = 0xffffffff;
        msg.type = MSG_REMOVE;
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--replace" ) || !strcmp( argv[i], "=" ) )
    {
        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [replace]\n" );
            return 1;
        }
        
        if( i <= 0 )
            msg.param = 0xffffffff;
        else
            msg.param = atoi( argv[i] );

        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [replace]\n" );
            return 1;
        }

        ifstream fin;
        fin.open( argv[i] );
        if( !fin.good() )
        {
            fprintf( stderr, "[chuck]: cannot open file '%s' for [replace]\n", argv[i] );
            return 1;
        }
        fin.close();
        
        msg.type = MSG_REPLACE;
        strcpy( msg.buffer, argv[i] );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--removeall" ) || !strcmp( argv[i], "--remall" ) )
    {
        msg.type = MSG_REMOVEALL;
        msg.param = 0;
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--kill" ) )
    {
        msg.type = MSG_REMOVEALL;
        msg.param = 0;
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
        msg.type = MSG_KILL;
        msg.param = (i+1)<argc ? atoi(argv[++i]) : 0;
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--time" ) )
    {
        msg.type = MSG_TIME;
        msg.param = 0;
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--status" ) || !strcmp( argv[i], "^" ) )
    {
        msg.type = MSG_STATUS;
        msg.param = 0;
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else
        return 0;
    
    return -1;
}




//-----------------------------------------------------------------------------
// name: next_power_2()
// desc: ...
//-----------------------------------------------------------------------------
t_CKUINT next_power_2( t_CKUINT n )
{
    if( n == 0 ) return 512;
    
    t_CKUINT count = 0;
    t_CKUINT i = sizeof(n) * 8;
    t_CKUINT x = 0;
    t_CKUINT mask = 0x1 << (i-1);
    while( i-- )
    {
        if( mask & n ) count++;
        if( (mask & n) && !x ) x = mask;
        mask >>= 1;
    }
    
    return count > 1 ? x << 1 : n;
}




//-----------------------------------------------------------------------------
// name: main()
// desc: ...
//-----------------------------------------------------------------------------
int main( int argc, char ** argv )
{
    int i, a, files = 0;
    t_CKBOOL enable_audio = TRUE;
    t_CKBOOL vm_halt = TRUE;
    t_CKUINT buffer_size = 512;
    t_CKUINT num_buffers = 4;
    t_CKUINT dac = 0;
    t_CKUINT adc = 0;

    for( i = 1; i < argc; i++ )
    {
        if( argv[i][0] == '-' || argv[i][0] == '+' ||
            argv[i][0] == '=' || argv[i][0] == '^' )
        {
            if( !strcmp(argv[i], "--dump") || !strcmp(argv[i], "+d")
                || !strcmp(argv[i], "--nodump") || !strcmp(argv[i], "-d") )
                continue;
            else if( !strcmp(argv[i], "--audio") || !strcmp(argv[i], "-a") )
                enable_audio = TRUE;
            else if( !strcmp(argv[i], "--silent") || !strcmp(argv[i], "-s") )
                enable_audio = FALSE;
            else if( !strcmp(argv[i], "--halt") || !strcmp(argv[i], "-t") )
                vm_halt = TRUE;
            else if( !strcmp(argv[i], "--loop") || !strcmp(argv[i], "-l") )
                vm_halt = FALSE;
            else if( !strncmp(argv[i], "--bufsize", 8) )
                buffer_size = atoi( argv[i]+8 ) > 0 ? atoi( argv[i]+8 ) : 512;
            else if( !strncmp(argv[i], "-b", 2) )
                buffer_size = atoi( argv[i]+2 ) > 0 ? atoi( argv[i]+2 ) : 512;
            else if( !strncmp(argv[i], "--dac", 5) )
                dac = atoi( argv[i]+5 ) > 0 ? atoi( argv[i]+5 ) : 0;
            else if( !strncmp(argv[i], "--adc", 5) )
                adc = atoi( argv[i]+5 ) > 0 ? atoi( argv[i]+5 ) : 0;
            else if( !strncmp(argv[i], "--level", 7) )
                g_priority = atoi( argv[i]+7 ) > 0 ? atoi( argv[i]+7 ) : 0xffffffff;
            else if( !strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")
                 || !strcmp(argv[i], "--about") )
            {
                usage();
                exit( 2 );
            }
            else if( a = send_cmd( argc, argv, i ) )
                exit( !(a < 0) );
            else
            {
                fprintf( stdout, "[chuck]: invalid flag '%s'\n", argv[i] );
                usage();
                exit( 1 );
            }
        }
        else
            files++;
    }
    
    // check buffer size
    buffer_size = next_power_2( buffer_size );
    
    if ( !files && vm_halt )
    {
        fprintf( stdout, "[chuck]: no input files...\n" );
        exit( 1 );
    }

    // allocate the vm
    Chuck_VM * vm = g_vm = new Chuck_VM;
    if( !vm->initialize( enable_audio, vm_halt, buffer_size, num_buffers, 
                         dac, adc, g_priority ) )
    {
        fprintf( stderr, "[chuck]: %s\n", vm->last_error() );
        exit( 1 );
    }

#ifndef __WINDOWS_DS__
    // catch SIGINT
    signal( SIGINT, signal_int );
#endif

    // allocate the type system
    g_env = type_engine_init( vm );
    // set the env
    vm->set_env( g_env );
    // load modules
    if( !load_internal_modules( g_env ) || g_error )
        exit( 1 );
    
    Chuck_VM_Code * code = NULL;
    Chuck_VM_Shred * shred = NULL;
    t_CKBOOL dump = FALSE;
    for( i = 1; i < argc; i++ )
    {
        if( argv[i][0] == '-' || argv[i][0] == '+' )
        {
            if( !strcmp(argv[i], "--dump") || !strcmp(argv[i], "+d" ) )
                dump = TRUE;
            else if( !strcmp(argv[i], "--nodump") || !strcmp(argv[i], "-d" ) )
                dump = FALSE;
            
            continue;
        }
    
        // parse
        if( !parse( argv[i] ) )
            return 1;

        // type check
        if( !type_check( g_env, g_program ) )
            return 1;

        // emit
        Chuck_Emmission * emit = emit_engine_init( g_env );
        if( !emit_code( emit, g_program ) )
            return 1;

        // transform the code
        code = emit_to_code( emit, dump );
        code->name = argv[i];

        // dump
        if( dump ) dump_instr( code );

        // spork it
        shred = vm->spork( code );
        
        // mem map it
        emit_engine_addr_map( emit, shred );
    
        // cleanup the emitter
        emit_engine_shutdown( emit );
    }

    // link
    emit_engine_resolve();
    
    // start udp server
    g_sock = ck_udp_create();
    if( !ck_bind( g_sock, g_port ) )
        fprintf( stderr, "[chuck]: cannot bind to udp port %i...\n", g_port );
    else
    {
        pthread_create( &g_tid, NULL, cb, NULL );
    }

    // run the vm
    vm->run();
    
    return 0;
}
