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

#include "chuck_compile.h"
#include "chuck_vm.h"
#include "chuck_bbq.h"
#include "chuck_utils.h"
#include "chuck_errmsg.h"
#include "chuck_lang.h"
#include "chuck_otf.h"

#include "ugen_stk.h"
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
ck_socket g_sock;

// global compiler
Chuck_Compiler * g_compiler = NULL;




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
                if( !process_msg( g_vm, g_compiler, &msg, FALSE, client ) )
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
    Chuck_Compiler * compiler = NULL;
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

    // allocate the compiler
    compiler = g_compiler = new Chuck_Compiler;
    // initialize the compiler
    compiler->initialize( vm );
    // enable dump
    compiler->emitter->dump = dump;

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
                compiler->emitter->dump = TRUE;
            else if( !strcmp(argv[i], "--nodump") || !strcmp(argv[i], "-d" ) )
                compiler->emitter->dump = FALSE;
            
            continue;
        }

        // parse, type-check, and emit
        if( !compiler->go( argv[i], NULL ) )
            return 1;

        // get the code
        code = compiler->output();
        // name it
        code->name += string("'") + argv[i] + "'";

        // spork it
        shred = vm->spork( code, NULL );
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

    // close all file handles in stk
    stk_detach( 0, NULL );

    // free vm
    SAFE_DELETE( vm );
    // free the compiler
    SAFE_DELETE( compiler );

    return 0;
}
