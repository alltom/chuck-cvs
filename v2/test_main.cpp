//-----------------------------------------------------------------------------
// file: test_main.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fstream>
using namespace std;

extern "C" int yyparse( void );

#include "chuck_type.h"
#include "chuck_emit.h"
#include "chuck_vm.h"
#include "chuck_bbq.h"
#include "chuck_utils.h"
#include "chuck_errmsg.h"

// current version
#define CK_VERSION "1.2.0.0"


#ifdef __PLATFORM_WIN32__
//'C' specification necessary for windows to link properly
extern "C" a_Program g_program;
#else
extern a_Program g_program;
#endif




//-----------------------------------------------------------------------------
// name: test_parse()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL test_parse()
{
    return TRUE;
}



//-----------------------------------------------------------------------------
// name: test_type()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL test_type()
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
    return 0;
}