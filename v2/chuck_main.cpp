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
#include <time.h>

#include "chuck_compile.h"
#include "chuck_vm.h"
#include "chuck_bbq.h"
#include "chuck_errmsg.h"
#include "chuck_lang.h"
#include "chuck_otf.h"
#include "chuck_shell.h"
#include "chuck_console.h"

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
#define CK_VERSION "1.2.0.2b (dracula)"


// global variables
#if defined(__MACOSX_CORE__)
  t_CKINT g_priority = 80;
  t_CKINT g_priority_low = 60;
#elif defined(__PLATFORM_WIN32__)
  t_CKINT g_priority = 0;
  t_CKINT g_priority_low = 0;
#else
  t_CKINT g_priority = 0x7fffffff;
  t_CKINT g_priority_low = 0x7fffffff;
#endif

// global virtual machine
Chuck_VM * g_vm = NULL;
// global compiler
Chuck_Compiler * g_compiler = NULL;
// the shell
Chuck_Shell * g_shell = NULL;

// thread id for otf thread
CHUCK_THREAD g_tid_otf = 0;
// thread id for shell
CHUCK_THREAD g_tid_shell = 0;

// default destination host name
char g_host[256] = "127.0.0.1";
// default destination host port
int g_port = 8888;
// default socket
ck_socket g_sock = NULL;




//-----------------------------------------------------------------------------
// name: all_detach()
// desc: called during cleanup to close all open file handles
//-----------------------------------------------------------------------------
extern "C" void all_detach()
{
    // close stk file handles
    stk_detach( 0, NULL );
    // close midi file handles
    midirw_detach();
}




//-----------------------------------------------------------------------------
// name: signal_int()
// desc: ...
//-----------------------------------------------------------------------------
extern "C" void signal_int( int sig_num )
{
    fprintf( stderr, "[chuck]: cleaning up...\n" );

    if( g_vm )
    {
        // get vm
        Chuck_VM * vm = g_vm;
        // flag the global one
        g_vm = NULL;
        // if not NULL
        if( vm )
        {
            // stop
            vm->stop();
            // detach
            all_detach();
        }

        // things don't work so good on windows...
#ifndef __PLATFORM_WIN32__
        // pthread_kill( g_tid_otf, 2 );
        if( g_tid_otf ) pthread_cancel( g_tid_otf );
        // if( g_tid_otf ) usleep( 50000 );
        SAFE_DELETE( vm );
#else
        // close handle
        if( g_tid_otf ) CloseHandle( g_tid_otf );
#endif
        // ck_close( g_sock );
    }

#ifndef __PLATFORM_WIN32__
    // pthread_join( g_tid_otf, NULL );
#endif
    
    exit(2);
}




t_CKUINT g_sigpipe_mode = 0;
//-----------------------------------------------------------------------------
// name: signal_pipe()
// desc: ...
//-----------------------------------------------------------------------------
extern "C" void signal_pipe( int sig_num )
{
    fprintf( stderr, "[chuck]: sigpipe handled - broken pipe (no connection)...\n" );
    if( g_sigpipe_mode )
    {
        all_detach();
        exit( 2 );
    }
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
// name: get_count()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL get_count( const char * arg, t_CKUINT * out )
{
    // no comment
    if( !strncmp( arg, "--", 2 ) ) arg += 2;
    else if( !strncmp( arg, "-", 1 ) ) arg += 1;
    else return FALSE;

    // end of string
    if( *arg == '\0' ) return FALSE;
    // not digit
    if( *arg < '0' || *arg > '9' ) return FALSE;

    // number
    *out = (t_CKUINT)atoi( arg );

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: version()
// desc: ...
//-----------------------------------------------------------------------------
void version()
{
    fprintf( stderr, "\n" );
    fprintf( stderr, "chuck version: %s\n", CK_VERSION );
    fprintf( stderr, "   http://chuck.cs.princeton.edu/\n\n" );
}




//-----------------------------------------------------------------------------
// name: usage()
// desc: ...
//-----------------------------------------------------------------------------
void usage()
{
    fprintf( stderr, "usage: chuck --[options|commands] [+-=^] file1 file2 file3 ...\n" );
    fprintf( stderr, "   [options] = halt|loop|audio|silent|dump|nodump|about|\n" );
    fprintf( stderr, "               srate<N>|bufsize<N>|bufnum<N>|dac<N>|adc<N>|\n" );
    fprintf( stderr, "               remote<hostname>|port<N>|verbose<N>|probe|\n" );
    fprintf( stderr, "               blocking|callback\n" );
    fprintf( stderr, "   [commands] = add|remove|replace|status|time|kill\n" );
    fprintf( stderr, "   [+-=^] = shortcuts for add, remove, replace, status\n" );
    version();
}




//-----------------------------------------------------------------------------
// name: main()
// desc: entry point
//-----------------------------------------------------------------------------
int main( int argc, char ** argv )
{
    Chuck_Compiler * compiler = NULL;
    Chuck_VM * vm = NULL;
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
    t_CKBOOL auto_depend = FALSE;
    t_CKBOOL block = FALSE;
    t_CKBOOL enable_shell = FALSE;
    t_CKINT  log_level = CK_LOG_SYSTEM_ERROR;

    t_CKUINT files = 0;
    t_CKUINT count = 1;
    t_CKINT i;

    // parse command line args
    for( i = 1; i < argc; i++ )
    {
        if( argv[i][0] == '-' || argv[i][0] == '+' ||
            argv[i][0] == '=' || argv[i][0] == '^' || argv[i][0] == '@' )
        {
            if( !strcmp(argv[i], "--dump") || !strcmp(argv[i], "+d")
                || !strcmp(argv[i], "--nodump") || !strcmp(argv[i], "-d") )
                continue;
            else if( get_count( argv[i], &count ) )
                continue;
            else if( !strcmp(argv[i], "--audio") || !strcmp(argv[i], "-a") )
                enable_audio = TRUE;
            else if( !strcmp(argv[i], "--silent") || !strcmp(argv[i], "-s") )
                enable_audio = FALSE;
            else if( !strcmp(argv[i], "--halt") || !strcmp(argv[i], "-t") )
                vm_halt = TRUE;
            else if( !strcmp(argv[i], "--loop") || !strcmp(argv[i], "-l") )
                vm_halt = FALSE;
            else if( !strcmp(argv[i], "--callback") )
                block = FALSE;
            else if( !strcmp(argv[i], "--blocking") )
                block = TRUE;
            else if( !strcmp(argv[i], "--console") || !strcmp( argv[i], "-c" ) )
                enable_shell = TRUE;
            else if( !strcmp(argv[i], "--shell") )
                enable_shell = TRUE;
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
            else if( !strncmp(argv[i], "--auto", 6) )
                auto_depend = TRUE;
            else if( !strncmp(argv[i], "-u", 2) )
                auto_depend = TRUE;
            else if( !strncmp(argv[i], "--log", 5) )
                log_level = argv[i][5] ? atoi( argv[i]+5 ) : CK_LOG_INFO;
            else if( !strncmp(argv[i], "--verbose", 9) )
                log_level = argv[i][9] ? atoi( argv[i]+9 ) : CK_LOG_INFO;
            else if( !strncmp(argv[i], "-v", 2) )
                log_level = argv[i][2] ? atoi( argv[i]+2 ) : CK_LOG_INFO;
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
            else if( !strcmp( argv[i], "--version" ) )
            {
                version();
                exit( 2 );
            }
            else if( otf_send_cmd( argc, argv, i, g_host, g_port ) )
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
    if( !set_priority && !block ) g_priority = g_priority_low;
    if( !set_priority && !enable_audio ) g_priority = 0x7fffffff;
    // set priority
    Chuck_VM::our_priority = g_priority;

    if ( !files && vm_halt && !enable_shell)
    {
        fprintf( stderr, "[chuck]: no input files... (try --help)\n" );
        exit( 1 );
    }

    // set log level
    EM_setlog( log_level );

    // allocate the vm - needs the type system
    vm = g_vm = new Chuck_VM;
    if( !vm->initialize( enable_audio, vm_halt, srate, buffer_size,
                         num_buffers, dac, adc, block ) )
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
    // set auto depend
    compiler->set_auto_depend( auto_depend );

    // vm synthesis subsystem - needs the type system
    if( !vm->initialize_synthesis( ) )
    {
        fprintf( stderr, "[chuck]: %s\n", vm->last_error() );
        exit( 1 );
    }

    // catch SIGINT
    signal( SIGINT, signal_int );
#ifndef __PLATFORM_WIN32__
    // catch SIGPIPE
    signal( SIGPIPE, signal_pipe );
#endif

    // shell
    if( enable_shell )
    {
        Chuck_Shell_Mode * mode = NULL;
        Chuck_Shell_UI * ui = NULL;

        // instantiate
        g_shell = new Chuck_Shell;
        
        // instantiate mode

        // instantiate shell UI
        ui = new Chuck_Console();

        // initialize shell UI
        if( !ui->init() )
        {
            fprintf( stderr, "[chuck]: error starting shell UI...\n" );
            exit(1);
        }
        
        // initialize
        if( !g_shell->init( vm, compiler, ui ) )
        {
            fprintf( stderr, "[chuck]: error starting shell...\n" );
            exit( 1 );
        }
    }

    // reset count
    count = 1;

    // log
    EM_log( CK_LOG_SYSTEM, "starting compilation..." );
    // push indent
    EM_pushlog();

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
            else
                get_count( argv[i], &count );

            continue;
        }

        // log
        EM_log( CK_LOG_FINE, "compiling '%s'...", argv[i] );
        // push indent
        EM_pushlog();

        // parse, type-check, and emit
        if( !compiler->go( argv[i], NULL ) )
            return 1;

        // get the code
        code = compiler->output();
        // name it
        code->name += string(argv[i]);

        // log
        EM_log( CK_LOG_FINE, "sporking %d %s...", count,
                count == 1 ? "instance" : "instances" );

        // spork it
        while( count-- ) shred = vm->spork( code, NULL );

        // pop indent
        EM_poplog();

        // reset count
        count = 1;
    }

    // pop indent
    EM_poplog();

    // reset the parser
    reset_parse();

    // boost priority
    if( Chuck_VM::our_priority != 0x7fffffff )
    {
        // try
        if( !Chuck_VM::set_priority( Chuck_VM::our_priority, vm ) )
        {
            // error
            fprintf( stderr, "[chuck]: %s\n", vm->last_error() );
            exit( 1 );
        }
    }

    // log
    EM_log( CK_LOG_SYSTEM, "starting listener on port: %d...", g_port );

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
        pthread_create( &g_tid_otf, NULL, otf_cb, NULL );
#else
        g_tid_otf = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)otf_cb, NULL, 0, 0 );
#endif
    }

    // start shell on separate thread
    if( enable_shell )
    {
#ifndef __PLATFORM_WIN32__
        pthread_create( &g_tid_shell, NULL, shell_cb, g_shell );
#else
        g_tid_shell = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)shell_cb, g_shell, 0, 0 );
#endif
    }

    // run the vm
    vm->run();

    // detach
    all_detach();

    // free vm
    g_vm = NULL; SAFE_DELETE( vm );
    // free the compiler
    SAFE_DELETE( compiler );

    return 0;
}
