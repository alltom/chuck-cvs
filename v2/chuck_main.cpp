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
// desc: chuck entry point
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Ananya Misra (amisra@cs.princeton.edu)
// date: version 1.1.x.x - Autumn 2002
//       version 1.2.x.x - Autumn 2004
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include "chuck_parse.h"
#include "chuck_type.h"
#include "chuck_emit.h"
#include "chuck_vm.h"
#include "chuck_bbq.h"
#include "chuck_utils.h"
#include "chuck_errmsg.h"
#include "chuck_lang.h"

#include "ugen_osc.h"
#include "ugen_xxx.h"
#include "ugen_filter.h"
#include "ugen_stk.h"
#include "ulib_machine.h"
#include "ulib_math.h"
#include "ulib_std.h"
#include "ulib_opsc.h"

//#include "ulib_net.h"

#include "util_thread.h"
#include "util_network.h"

#include <signal.h>
#ifndef __PLATFORM_WIN32__
  #define CHUCK_THREAD pthread_t
  #include <pthread.h>
  #include <unistd.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
#else 
  #define CHUCK_THREAD HANDLE
#endif


// current version
#define CK_VERSION "1.2.0.0"


// global variables
#if defined(__MACOSX_CORE__)
  t_CKINT g_priority = 95;
#elif defined(__PLATFORM_WIN32__)
  t_CKINT g_priority = 0;
#else
  t_CKINT g_priority = 0x7fffffff;
#endif
Chuck_VM * g_vm = NULL;
CHUCK_THREAD g_tid = 0;
char g_host[256] = "127.0.0.1";
int g_port = 8888;
// ck_socket g_sock;
Chuck_Env * g_env = NULL;
Chuck_Emitter * g_emitter = NULL;
t_CKBOOL g_error = FALSE;

ck_socket g_sock;




//-----------------------------------------------------------------------------
// name: signal_int()
// desc: ...
//-----------------------------------------------------------------------------
void signal_int( int sig_num )
{
    fprintf( stderr, "[chuck]: cleaning up...\n" );

    if( g_vm )
    {
        Chuck_VM * vm = g_vm;
        g_vm = NULL;
        vm->stop();
        stk_detach( 0, NULL );
        midirw_detach( );
#ifndef __PLATFORM_WIN32__
        // pthread_kill( g_tid, 2 );
        if( g_tid ) pthread_cancel( g_tid );
        // if( g_tid ) usleep( 50000 );
        delete( vm );
#else
        CloseHandle( g_tid );
#endif
//        ck_close( g_sock );
    }

#ifndef __PLATFORM_WIN32__
//    pthread_join( g_tid, NULL );
#endif
    
    exit(2);
}




t_CKUINT g_sigpipe_mode = 0;
//-----------------------------------------------------------------------------
// name: signal_pipe()
// desc: ...
//-----------------------------------------------------------------------------
void signal_pipe( int sig_num )
{
    fprintf( stderr, "[chuck]: sigpipe handled - broken pipe (no connection)...\n" );
    if( g_sigpipe_mode )
    {
        stk_detach( 0, NULL );
        exit( 1 );
    }
}




//-----------------------------------------------------------------------------
// name: parse()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL parse( const char * filename, FILE * fd )
{
    // parse
    return chuck_parse( filename, fd );
}




//-----------------------------------------------------------------------------
// name: type_check()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_check( Chuck_Env * env, const string & filename )
{
    // parse and type check it
    return parse_and_check_prog( env, filename );
}




//-----------------------------------------------------------------------------
// name: emit()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_VM_Code * emit( Chuck_Emitter * emit, a_Program prog )
{
    // emit to vm code
    return emit_engine_emit_prog( emit, prog );
}




//-----------------------------------------------------------------------------
// name: send_file()
// desc: ...
//-----------------------------------------------------------------------------
int send_file( const char * filename, Net_Msg & msg, const char * op )
{
    FILE * fd = NULL;
    struct stat fs;
    
    strcpy( msg.buffer, "" );
    //if( filename[0] != '/' )
    //{ 
    //    strcpy( msg.buffer, getenv("PWD") ? getenv("PWD") : "" );
    //    strcat( msg.buffer, getenv("PWD") ? "/" : "" );
    //}
    strcat( msg.buffer, filename );

    // test it
    fd = open_cat_ck( (char *)msg.buffer );
    if( !fd )
    {
        fprintf( stderr, "[chuck]: cannot open file '%s' for [%s]...\n", filename, op );
        return FALSE;
    }
            
    if( !chuck_parse( (char *)msg.buffer, fd ) )
    {
        fprintf( stderr, "[chuck]: skipping file '%s' for [%s]...\n", filename, op );
        fclose( fd );
        return FALSE;
    }
            
    // stat it
    stat( msg.buffer, &fs );
    fseek( fd, 0, SEEK_SET );

    //fprintf(stderr, "sending TCP file %s\n", msg.buffer );
    // send the first packet
    msg.param2 = (t_CKUINT)fs.st_size;
    msg.length = 0;
    otf_hton( &msg );
    ck_send( g_sock, (char *)&msg, sizeof(msg) );

    // send the whole thing
    t_CKUINT left = (t_CKUINT)fs.st_size;
    while( left )
    {
        //fprintf(stderr,"file %03d bytes left ... ", left);
        // amount to send
        msg.length = left > NET_BUFFER_SIZE ? NET_BUFFER_SIZE : left;
        // read
        msg.param3 = fread( msg.buffer, sizeof(char), msg.length, fd );
        // amount left
        left -= msg.param3 ? msg.param3 : 0;
        msg.param2 = left;
        //fprintf(stderr, "sending fread %03d length %03d...\n", msg.param3, msg.length );
        // send it
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    
    // close
    fclose( fd );
    //fprintf(stderr, "done.\n", msg.buffer );
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: recv_file()
// desc: ...
//-----------------------------------------------------------------------------
FILE * recv_file( const Net_Msg & msg, ck_socket sock )
{
    Net_Msg buf;
    
    // what is left
    // t_CKUINT left = msg.param2;
    // make a temp file
    FILE * fd = tmpfile();

    do {
        // msg
        if( !ck_recv( sock, (char *)&buf, sizeof(buf) ) )
            goto error;
        otf_ntoh( &buf );
        // write
        fwrite( buf.buffer, sizeof(char), buf.length, fd );
    }while( buf.param2 );
    
    return fd;

error:
    fclose( fd );
    fd = NULL;
    return NULL;
}




//-----------------------------------------------------------------------------
// name: send_connect()
// desc: ...
//-----------------------------------------------------------------------------
int send_connect()
{
    g_sock = ck_tcp_create( 0 );
    if( !g_sock )
    {
        fprintf( stderr, "[chuck]: cannot open socket to send command...\n" );
        return FALSE;
    }

    if( strcmp( g_host, "127.0.0.1" ) )
        fprintf( stderr, "[chuck]: connecting to %s on port %i via TCP...\n", g_host, g_port );
    
    if( !ck_connect( g_sock, g_host, g_port ) )
    {
        fprintf( stderr, "[chuck]: cannot open TCP socket on %s:%i...\n", g_host, g_port );
        return FALSE;
    }
    
    ck_send_timeout( g_sock, 0, 2000000 );

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: send_cmd()
// desc: ...
//-----------------------------------------------------------------------------
int send_cmd( int argc, char ** argv, t_CKINT & i )
{
    Net_Msg msg;
    g_sigpipe_mode = 1;
    int tasks_total = 0, tasks_done = 0;
    
    if( !strcmp( argv[i], "--add" ) || !strcmp( argv[i], "+" ) )
    {
        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [add]...\n" );
            goto error;
        }

        if( !send_connect() ) return 0;
        do {
            msg.type = MSG_ADD;
            msg.param = 1;
            tasks_done += send_file( argv[i], msg, "add" );
            tasks_total++;
        } while( ++i < argc );
        
        if( !tasks_done )
            goto error;
    }
    else if( !strcmp( argv[i], "--remove" ) || !strcmp( argv[i], "-" ) )
    {
        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [remove]...\n" );
            goto error;
        }

        if( !send_connect() ) return 0;
        do {
            msg.param = atoi( argv[i] );
            msg.type = MSG_REMOVE;
            otf_hton( &msg );
            ck_send( g_sock, (char *)&msg, sizeof(msg) );
        } while( ++i < argc );
    }
    else if( !strcmp( argv[i], "--" ) )
    {
        if( !send_connect() ) return 0;
        msg.param = 0xffffffff;
        msg.type = MSG_REMOVE;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--replace" ) || !strcmp( argv[i], "=" ) )
    {
        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [replace]...\n" );
            goto error;
        }

        if( i <= 0 )
            msg.param = 0xffffffff;
        else
            msg.param = atoi( argv[i] );

        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [replace]...\n" );
            goto error;
        }

        if( !send_connect() ) return 0;
        msg.type = MSG_REPLACE;
        if( !send_file( argv[i], msg, "replace" ) )
            goto error;
    }
    else if( !strcmp( argv[i], "--removeall" ) || !strcmp( argv[i], "--remall" ) )
    {
        if( !send_connect() ) return 0;
        msg.type = MSG_REMOVEALL;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--kill" ) )
    {
        if( !send_connect() ) return 0;
        msg.type = MSG_REMOVEALL;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
        msg.type = MSG_KILL;
        msg.param = (i+1)<argc ? atoi(argv[++i]) : 0;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--time" ) )
    {
        if( !send_connect() ) return 0;
        msg.type = MSG_TIME;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--status" ) || !strcmp( argv[i], "^" ) )
    {
        if( !send_connect() ) return 0;
        msg.type = MSG_STATUS;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else
        return 0;
        
    // send
    msg.type = MSG_DONE;
    otf_hton( &msg );
    ck_send( g_sock, (char *)&msg, sizeof(msg) );

    // set timeout
    ck_recv_timeout( g_sock, 0, 2000000 );
    // reply
    if( ck_recv( g_sock, (char *)&msg, sizeof(msg) ) )
    {
        otf_ntoh( &msg );
        fprintf( stderr, "[chuck(remote)]: operation %s\n", ( msg.param ? "successful" : "failed (sorry)" ) );
        if( !msg.param )
            fprintf( stderr, "(reason): %s\n", 
                ( strstr( (char *)msg.buffer, ":" ) ? strstr( (char *)msg.buffer, ":" ) + 1 : (char *)msg.buffer ) ) ;
    }
    else
    {
        fprintf( stderr, "[chuck]: remote operation timed out...\n" );
    }
    // close the sock
    ck_close( g_sock );
    
    // exit
    exit( msg.param );

    return 1;
    
error:
    msg.type = MSG_DONE;
    otf_hton( &msg );
    ck_send( g_sock, (char *)&msg, sizeof(msg) );
    ck_close( g_sock );
    
    exit( 1 );

    return 0;
}




//-----------------------------------------------------------------------------
// name: process_msg()
// desc: ...
//-----------------------------------------------------------------------------
extern "C" t_CKUINT process_msg( Net_Msg * msg, t_CKBOOL immediate, void * data )
{
    Chuck_Msg * cmd = new Chuck_Msg;
    Chuck_VM_Code * code = NULL;
    FILE * fd = NULL;
    t_CKBOOL error = FALSE;
    
    // fprintf( stderr, "UDP message recv...\n" );
    if( msg->type == MSG_REPLACE || msg->type == MSG_ADD )
    {
        // see if entire file is on the way
        if( msg->param2 )
        {
            fd = recv_file( *msg, (ck_socket)data );
            if( !fd )
            {
                fprintf( stderr, "[chuck]: incoming source transfer '%s' failed...\n",
                    mini(msg->buffer) );
                return 0;
            }
        }
        
        // parse
        if( !chuck_parse( msg->buffer, fd ) )
            return 1;

        // type check
        if( !type_check( g_env, msg->buffer ) )
            return 1;

        // emit
        if( !(code = emit( g_emitter, g_program )) )
        {
            error = TRUE;
            goto unload;
        }

        // name it
        code->name += string(msg->buffer);

unload:

        // unload the context from the type-checker
        if( !type_engine_unload_context( g_env ) )
        {
            EM_error2( 0, "internal error unloading context...\n" );
            return 1;
        }

        if( error )
            return 1;
        
        // set the flags for the command
        cmd->type = msg->type;
        cmd->code = code;
        if( msg->type == MSG_REPLACE )
            cmd->param = msg->param;
    }
    else if( msg->type == MSG_STATUS || msg->type == MSG_REMOVE || msg->type == MSG_REMOVEALL
             || msg->type == MSG_KILL || msg->type == MSG_TIME )
    {
        cmd->type = msg->type;
        cmd->param = msg->param;
    }
    else
    {
        fprintf( stderr, "[chuck]: unrecognized incoming command from network: '%i'\n", cmd->type );
        SAFE_DELETE(cmd);
        return 0;
    }
    
    // immediate
    if( immediate )
        return g_vm->process_msg( cmd );

    g_vm->queue_msg( cmd, 1 );

    return 1;
}




//-----------------------------------------------------------------------------
// name: cb()
// desc: ...
//-----------------------------------------------------------------------------
void * cb( void * p )
{
    Net_Msg msg;
    Net_Msg ret;
    ck_socket client;
    int n;

#ifndef __PLATFORM_WIN32__
    // catch SIGPIPE
    signal( SIGPIPE, signal_pipe );
#endif

    while( true )
    {
        client = ck_accept( g_sock );
        if( !client )
        {
            if( g_vm ) fprintf( stderr, "[chuck]: socket error during accept()...\n" );
            usleep( 40000 );
            ck_close( client );
            continue;
        }
        msg.clear();
        // set time out
        ck_recv_timeout( client, 0, 5000000 );
        n = ck_recv( client, (char *)&msg, sizeof(msg) );
        otf_ntoh( &msg );
        if( n != sizeof(msg) )
        {
            fprintf( stderr, "[chuck]: 0-length packet...\n", (int)client );
            usleep( 40000 );
            ck_close( client );
            continue;
        }
        
        if( msg.header != NET_HEADER )
        {
            fprintf( stderr, "[chuck]: header mismatch - possible endian lunacy...\n" );
            ck_close( client );
            continue;
        }

        while( msg.type != MSG_DONE )
        {
            if( g_vm )
            {
                if( !process_msg( &msg, FALSE, client ) )
                {
                    ret.param = FALSE;
                    strcpy( (char *)ret.buffer, EM_lasterror() );
                    while( msg.type != MSG_DONE && n )
                    {
                        n = ck_recv( client, (char *)&msg, sizeof(msg) );
                        otf_ntoh( &msg );
                    }
                    break;
                }
                else
                {
                    ret.param = TRUE;
                    strcpy( (char *)ret.buffer, "success" );
                    n = ck_recv( client, (char *)&msg, sizeof(msg) );
                    otf_ntoh( &msg );
                }
            }
        }
        
        otf_hton( &ret );
        ck_send( client, (char *)&ret, sizeof(ret) );
        ck_close( client );
    }
    
    return NULL;
}




//-----------------------------------------------------------------------------
// name: next_power_2()
// desc: ...
// thanks: to Niklas Werner / music-dsp
//-----------------------------------------------------------------------------
t_CKUINT next_power_2( t_CKUINT n )
{
    t_CKUINT nn = n;
    for( ; n &= n-1; nn = n );
    return nn * 2;
}




//-----------------------------------------------------------------------------
// name: load_module()
// desc: load a dll and add it
//-----------------------------------------------------------------------------
t_CKBOOL load_module( Chuck_Env * env, f_ck_query query, 
                      const char * name, const char * nspc )
{
    Chuck_DLL * dll = NULL;
    
    // load osc
    dll = new Chuck_DLL( name );
    dll->load( query );
    if( !dll->query() || !type_engine_add_dll( env, dll, nspc ) )
    {
        fprintf( stderr, 
                 "[chuck]: internal error loading module '%s.%s'...\n", 
                 nspc, name );
        if( !dll->query() )
            fprintf( stderr, "       %s\n", dll->last_error() );

        g_error = TRUE;
        return FALSE;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: load_internal_modules()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL load_internal_modules( Chuck_Env * env )
{
    // make context
    Chuck_Context * context = new Chuck_Context;
    // add ref
    context->add_ref();
    // load it
    type_engine_load_context( env, context );

    // load
    load_module( env, osc_query, "osc", "global" );
    load_module( env, xxx_query, "xxx", "global" );
    load_module( env, filter_query, "filter", "global" );
    load_module( env, stk_query, "stk", "global" );

    // load
    if( !load_module( env, machine_query, "Machine", "global" ) ) goto error;
    machine_init( g_vm, process_msg );
    if( !load_module( env, libstd_query, "Std", "global" ) ) goto error;
    if( !load_module( env, libmath_query, "Math", "global" ) ) goto error;
    if( !load_module( env, opensoundcontrol_query, "opsc", "global" ) ) goto error;
    // if( !load_module( env, net_query, "net", "global" ) ) goto error;

    if( !init_class_Midi( env ) ) goto error;
    if( !init_class_MidiRW( env ) ) goto error;

    // clear context
    type_engine_unload_context( env );

    // commit what is in the type checker at this point
    env->global()->commit();
    
    return TRUE;

error:

    // clear context
    type_engine_unload_context( env );

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: uh()
// desc: ...
//-----------------------------------------------------------------------------
void uh( )
{
    // TODO: play white noise and/or sound effects
    srand( time( NULL ) );
    while( true )
    {
        int n = (int)(rand() / (float)RAND_MAX * poop_size);
        printf( "%s\n", poop[n] );
        usleep( (unsigned long)(rand() / (float)RAND_MAX * 2000000) );
    }
}




//-----------------------------------------------------------------------------
// name: usage()
// desc: ...
//-----------------------------------------------------------------------------
void usage()
{
    fprintf( stderr, "usage: chuck --[options|commands] [+-=^] file1 file2 file3 ...\n" );
    fprintf( stderr, "   [options] = halt|loop|audio|silent|dump|nodump|about\n" );
    fprintf( stderr, "               srate<N>|bufsize<N>|bufnum<N>|dac<N>|adc<N>|\n" );
    fprintf( stderr, "               remote<hostname>|port<N>|probe\n" );
    fprintf( stderr, "   [commands] = add|remove|replace|status|time|kill\n" );
    fprintf( stderr, "   [+-=^] = shortcuts for add, remove, replace, status\n\n" );
    fprintf( stderr, "chuck version: %s\n", CK_VERSION );
    fprintf( stderr, "   http://chuck.cs.princeton.edu/\n\n" );
}




//-----------------------------------------------------------------------------
// name: main()
// desc: entry point
//-----------------------------------------------------------------------------
int main( int argc, char ** argv )
{
    Chuck_Env * env = NULL;
    Chuck_Emitter * emitter = NULL;
    Chuck_VM_Code * code = NULL;
    Chuck_VM_Shred * shred = NULL;
    
    t_CKBOOL enable_audio = TRUE;
    t_CKBOOL vm_halt = TRUE;
    t_CKUINT srate = SAMPLING_RATE_DEFAULT;
    t_CKUINT buffer_size = BUFFER_SIZE_DEFAULT;
    t_CKUINT num_buffers = NUM_BUFFERS_DEFAULT;
    t_CKUINT dac = 0;
    t_CKUINT adc = 0;
    t_CKBOOL dump = FALSE;
    t_CKBOOL probe = FALSE;
    t_CKBOOL set_priority = FALSE;

    t_CKUINT files = 0;
    t_CKINT i;
    t_CKINT a;

    // parse command line args
    for( i = 1; i < argc; i++ )
    {
        if( argv[i][0] == '-' || argv[i][0] == '+' ||
            argv[i][0] == '=' || argv[i][0] == '^' || argv[i][0] == '@' )
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
            else if( !strncmp(argv[i], "--srate", 7) )
                srate = atoi( argv[i]+7 ) > 0 ? atoi( argv[i]+7 ) : srate;
            else if( !strncmp(argv[i], "-r", 2) )
                srate = atoi( argv[i]+2 ) > 0 ? atoi( argv[i]+2 ) : srate;
            else if( !strncmp(argv[i], "--bufsize", 9) )
                buffer_size = atoi( argv[i]+9 ) > 0 ? atoi( argv[i]+9 ) : buffer_size;
            else if( !strncmp(argv[i], "-b", 2) )
                buffer_size = atoi( argv[i]+2 ) > 0 ? atoi( argv[i]+2 ) : buffer_size;
            else if( !strncmp(argv[i], "--bufnum", 8) )
                num_buffers = atoi( argv[i]+8 ) > 0 ? atoi( argv[i]+8 ) : num_buffers;
            else if( !strncmp(argv[i], "-n", 2) )
                num_buffers = atoi( argv[i]+2 ) > 0 ? atoi( argv[i]+2 ) : num_buffers;
            else if( !strncmp(argv[i], "--dac", 5) )
                dac = atoi( argv[i]+5 ) > 0 ? atoi( argv[i]+5 ) : 0;
            else if( !strncmp(argv[i], "--adc", 5) )
                adc = atoi( argv[i]+5 ) > 0 ? atoi( argv[i]+5 ) : 0;
            else if( !strncmp(argv[i], "--level", 7) )
            {   g_priority = atoi( argv[i]+7 ); set_priority = TRUE; }
            else if( !strncmp(argv[i], "--remote", 8) )
                strcpy( g_host, argv[i]+8 );
            else if( !strncmp(argv[i], "@", 1) )
                strcpy( g_host, argv[i]+1 );
            else if( !strncmp(argv[i], "--port", 6) )
                g_port = atoi( argv[i]+6 );
            else if( !strncmp(argv[i], "-p", 2) )
                g_port = atoi( argv[i]+2 );
            else if( !strcmp( argv[i], "--probe" ) )
                probe = TRUE;
            else if( !strcmp( argv[i], "--poop" ) )
                uh();
            else if( !strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")
                 || !strcmp(argv[i], "--about") )
            {
                usage();
                exit( 2 );
            }
            else if(( a = send_cmd( argc, argv, i ) ))
                exit( 0 );
            else
            {
                fprintf( stderr, "[chuck]: invalid flag '%s'\n", argv[i] );
                usage();
                exit( 1 );
            }
        }
        else
            files++;
    }
    
    // probe
    if( probe )
    {
        Digitalio::probe();

        EM_error2b( 0, "" );
        probeMidiIn();
        EM_error2b( 0, "" );
        probeMidiOut();
        EM_error2b( 0, "" );
    
        // exit
        exit( 0 );
    }
    
    // check buffer size
    buffer_size = next_power_2( buffer_size-1 );
    // audio, boost
    if( !set_priority && !enable_audio ) g_priority = 0x7fffffff;
    // set priority
    Chuck_VM::our_priority = g_priority;
    
    if ( !files && vm_halt )
    {
        fprintf( stderr, "[chuck]: no input files... (try --help)\n" );
        exit( 1 );
    }

    // allocate the vm - needs the type system
    Chuck_VM * vm = g_vm = new Chuck_VM;
    if( !vm->initialize( enable_audio, vm_halt, srate, buffer_size,
                         num_buffers, dac, adc, g_priority ) )
    {
        fprintf( stderr, "[chuck]: %s\n", vm->last_error() );
        exit( 1 );
    }

    // allocate the type checker
    env = g_env = type_engine_init( vm );
    // allocate the emitter
    emitter = g_emitter = emit_engine_init( env );
    // enable dump
    emitter->dump = dump;
    // load internal libs
    if( !load_internal_modules( env ) ) exit( 1 );

    // vm synthesis subsystem - needs the type system
    vm->initialize_synthesis( );

    // catch SIGINT
    signal( SIGINT, signal_int );
#ifndef __PLATFORM_WIN32__
    // catch SIGPIPE
    signal( SIGPIPE, signal_pipe );
#endif

    // loop through and process each file
    for( i = 1; i < argc; i++ )
    {
        // make sure
        if( argv[i][0] == '-' || argv[i][0] == '+' )
        {
            if( !strcmp(argv[i], "--dump") || !strcmp(argv[i], "+d" ) )
                emitter->dump = TRUE;
            else if( !strcmp(argv[i], "--nodump") || !strcmp(argv[i], "-d" ) )
                emitter->dump = FALSE;
            
            continue;
        }
    
        // parse and type check
        if( !type_check( g_env, argv[1] ) )
            return 1;

        // emit
        if( !(code = emit( emitter, g_program )) )
            return 1;

        // name it
        code->name += string("'") + argv[i] + "'";

        // spork it
        shred = vm->spork( code, NULL );

        // unload the context from the type-checker
        if( !type_engine_unload_context( env ) )
        {
            EM_error2( 0, "internal error unloading context...\n" );
            return 1;
        }
    }

    // start tcp server
    g_sock = ck_tcp_create( 1 );
    if( !g_sock || !ck_bind( g_sock, g_port ) || !ck_listen( g_sock, 10 ) )
    {
        fprintf( stderr, "[chuck]: cannot bind to tcp port %i...\n", g_port );
        ck_close( g_sock );
        g_sock = NULL;
    }
    else
    {
#ifndef __PLATFORM_WIN32__
        pthread_create( &g_tid, NULL, cb, NULL );
#else
        g_tid = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)cb, NULL, 0, 0);
#endif
    }

    // run the vm
    vm->run();

    // free vm
    SAFE_DELETE( vm );
    stk_detach( 0, NULL );
    // TODO: clean env
    // TODO: clean emitter

    return 0;
}
