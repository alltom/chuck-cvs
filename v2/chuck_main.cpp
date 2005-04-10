//-----------------------------------------------------------------------------
// file: chuck_main.cpp
// desc: chuck entry point
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: version 1.1.x.x - Autumn 2002
//       version 1.2.x.x - Autumn 2004
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "chuck_type.h"
#include "chuck_emit.h"
#include "chuck_vm.h"
#include "chuck_bbq.h"
#include "chuck_utils.h"
#include "chuck_errmsg.h"

#include "ugen_osc.h"
#include "ugen_xxx.h"
#include "ugen_filter.h"
#include "ugen_stk.h"
#include "ulib_machine.h"
#include "ulib_math.h"
#include "ulib_net.h"
#include "ulib_std.h"

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
#else
  t_CKINT g_priority = 0x7fffffff;
#endif
Chuck_VM * g_vm = NULL;
CHUCK_THREAD g_tid = 0;
char g_host[256] = "127.0.0.1";
int g_port = 8888;
// ck_socket g_sock;
Chuck_Env * g_env = NULL;
t_CKBOOL g_error = FALSE;
char g_filename[1024] = "";


// 'C' specification necessary for windows to link properly
#ifdef __PLATFORM_WIN32__
  extern "C" a_Program g_program;
#else
  extern a_Program g_program;
#endif

// link with the parser
extern "C" int yyparse( void );




//-----------------------------------------------------------------------------
// name: open_cat()
// desc: ...
//-----------------------------------------------------------------------------
FILE * open_cat( c_str fname )
{
    FILE * fd = NULL;
    if( !(fd = fopen( fname, "rb" )) )
        if( !strstr( fname, ".ck" ) && !strstr( fname, ".CK" ) )
        {
            strcat( fname, ".ck" );
            fd = fopen( fname, "rb" );
        }
    return fd;
}




//-----------------------------------------------------------------------------
// name: parse()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL parse( c_constr fname, FILE * fd = NULL )
{
    strcpy( g_filename, fname );

    // test it
    if( !fd ) {
        fd = open_cat( g_filename );
        if( !fd ) strcpy( g_filename, fname );
    }

    // reset
    if( EM_reset( g_filename, fd ) == FALSE ) return FALSE;

    // TODO: clean g_program
    g_program = NULL;

    // parse
    if( !(yyparse( ) == 0) ) return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_check()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_check( Chuck_Env * env, a_Program prog )
{
    // type check it
    if( !type_engine_check_prog( env, prog ) )
        return FALSE;

    return TRUE;
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
// name: usage()
// desc: ...
//-----------------------------------------------------------------------------
void usage()
{
    fprintf( stderr, "usage: chuck --[options|commands] [+-=^] file1 file2 file3 ...\n" );
    fprintf( stderr, "   [options] = halt|loop|audio|silent|dump|nodump|about|\n" );
    fprintf( stderr, "               srate<N>|bufsize<N>|bufnum<N>|dac<N>|adc<N>|\n" );
    fprintf( stderr, "               remote<hostname>|port<N>\n" );
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
    t_CKBOOL ret = TRUE;
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
    t_CKBOOL dump = TRUE;

    t_CKUINT files = 0;
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
                g_priority = atoi( argv[i]+7 );
            else if( !strncmp(argv[i], "--remote", 8) )
                strcpy( g_host, argv[i]+8 );
            else if( !strncmp(argv[i], "@", 1) )
                strcpy( g_host, argv[i]+1 );
            else if( !strncmp(argv[i], "--port", 6) )
                g_port = atoi( argv[i]+6 );
            else if( !strncmp(argv[i], "-p", 2) )
                g_port = atoi( argv[i]+2 );
            else if( !strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")
                 || !strcmp(argv[i], "--about") )
            {
                usage();
                exit( 2 );
            }
            //else if( a = send_cmd( argc, argv, i ) )
            //    exit( 0 );
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

    // check buffer size
    buffer_size = next_power_2( buffer_size-1 );
    // set priority
    Chuck_VM::our_priority = g_priority;
    
    if ( !files && vm_halt )
    {
        fprintf( stderr, "[chuck]: no input files... (try --help)\n" );
        exit( 1 );
    }

    // allocate the vm
    Chuck_VM * vm = g_vm = new Chuck_VM;
    if( !vm->initialize( enable_audio, vm_halt, srate, buffer_size,
                         num_buffers, dac, adc, g_priority ) )
    {
        fprintf( stderr, "[chuck]: %s\n", vm->last_error() );
        exit( 1 );
    }

    // allocate the type checker
    env = g_env = type_engine_init( NULL );
    // allocate the emitter
    emitter = emit_engine_init( env );
    // enable dump
    emitter->dump = dump;

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
    
        // parse
        if( !parse( argv[i] ) )
            return 1;

        // type check
        if( !type_check( g_env, g_program ) )
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

    // run the vm
    vm->run();

    return 0;
}
