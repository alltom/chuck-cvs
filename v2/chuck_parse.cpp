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
// file: chuck_parse.cpp
// desc: chuck parser interface
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Summer 2005
//-----------------------------------------------------------------------------
#include "chuck_parse.h"
#include "chuck_errmsg.h"
#include <string.h>
using namespace std;


// global
static char g_filename[1024] = "";


//-----------------------------------------------------------------------------
// name: open_cat_ck()
// desc: ...
//-----------------------------------------------------------------------------
FILE * open_cat_ck( c_str fname )
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
// name: chuck_parse()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL chuck_parse( c_constr fname, FILE * fd )
{
	t_CKBOOL clo = FALSE;

    strcpy( g_filename, fname );

    // test it
    if( !fd ) {
        fd = open_cat_ck( g_filename );
        if( !fd ) strcpy( g_filename, fname );
		else clo = TRUE;
    }

    // reset
    if( EM_reset( g_filename, fd ) == FALSE ) return FALSE;

    // TODO: clean g_program
    g_program = NULL;

    // parse
    if( !(yyparse( ) == 0) ) return FALSE;
	
	// done
	if( clo ) fclose( fd );

    return TRUE;
}




//------------------------------------------------------------------------------
// name: reset_parse()
// desc: ...
//------------------------------------------------------------------------------
void reset_parse( )
{
    // empty file name
    EM_change_file( NULL );
}




//-----------------------------------------------------------------------------
// name: parseLine()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL SyntaxQuery::parseLine( const std::string & line, SyntaxTokenList & tokens )
{
    // clear the token list
    tokens.howmany = 0;

    return TRUE;
}
