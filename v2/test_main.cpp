//-----------------------------------------------------------------------------
// file: test_main.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>
using namespace std;

#include "chuck_type.h"
#include "chuck_emit.h"
#include "chuck_vm.h"
#include "chuck_bbq.h"
#include "chuck_utils.h"
#include "chuck_errmsg.h"


// current version
#define CK_VERSION "1.2.0.0"


// C specification necessary for windows to link properly
#ifdef __PLATFORM_WIN32__
  extern "C" a_Program g_program;
#else
  extern a_Program g_program;
#endif

// link with the parser
extern "C" int yyparse( void );

// global variables
char g_filename[1024] = "";




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
// name: test_parse()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL test_parse( c_str fname, FILE * fd = NULL )
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
// name: test_type()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL test_type( Chuck_Env * env, a_Program prog )
{
    if( !type_engine_check_prog( env, prog ) )
        return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: test_emit()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL test_emit()
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: main()
// desc: entry point
//-----------------------------------------------------------------------------
int main( int argc, char ** argv )
{
    t_CKBOOL ret = TRUE;
    Chuck_Env * env = NULL;
    Chuck_Emitter * emit = NULL;
    Chuck_VM_Code * code = NULL;

    // test the parsing
    if( !test_parse( "a.ck" ) )
    {
        fprintf( stderr, "parse failed...\n" );
        return 1;
    }

    // allocate the type checker
    env = type_engine_init( NULL );

    // test the type checker
    if( !test_type( env, g_program ) )
    {
        fprintf( stderr, "type check failed...\n" );
        return 1;
    }
    
    fprintf( stderr, "type check success!\n" );

    // allocate the emitter
    emit = emit_engine_init( env );

    // test the emitter
    code = emit_engine_emit_prog( emit, g_program );
    if( !code )
    {
        fprintf( stderr, "emitter failed...\n" );
        return 1;
    }

    fprintf( stderr, "emit success!\n" );

    return 0;
}
