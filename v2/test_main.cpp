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

    if( !test_parse( "a.ck" ) )
    {
        fprintf( stderr, "parse failed...\n" );
        return 1;
    }

    fprintf( stderr, "sucess!\n" );

    return 0;
}