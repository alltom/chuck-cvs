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
// file: chuck_parse.h
// desc: chuck parser interface (using flex and bison)
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002 - original in chuck_main.cpp
//       Autumn 2005 - this file
//-----------------------------------------------------------------------------
#ifndef __CHUCK_PARSE_H__
#define __CHUCK_PARSE_H__

#include "chuck_def.h"
#include "chuck_absyn.h"

#include <stdio.h>


// 'C' specification necessary for windows to link properly
#ifdef __PLATFORM_WIN32__
  extern "C" a_Program g_program;
#else
  extern a_Program g_program;
#endif

// link with the parser
extern "C" int yyparse( void );


// open file with .ck append as appropriate
FILE * open_cat_ck( c_str filename );
// parse file
t_CKBOOL chuck_parse( c_constr fname, FILE * fd = NULL );
// reset the parser
void reset_parse( );


// syntax highlighting tools
#include <string>
#include <vector>


// syntax types (for highlighting)
enum SyntaxType
{
    COMMA = 0,
    SEMICOLON,
    DBLCOLON,
    PAREN,
    DOT,
    CHUCK_OP,
    OPERATOR,
    KEYWORD,
    DEBUG_PRINT,
    SPORK,
    INTEGER,
    FLOATING,
    STRING,
    COMMENT,
    OTHER,

    NUM_SYNTAX_TYPES
};


// token info
struct SyntaxToken
{
    // the token
    std::string token;
    // type
    t_CKUINT type;

    // from the beginning of line
    std::string::size_type begin;
    // from the beginning of line
    std::string::size_type end;
    
    // constructor
    SyntaxToken() : type(0) { }
    // copy constructor
    SyntaxToken( const SyntaxToken & rhs )
    {
        token = rhs.token;
        type = rhs.type;
        begin = rhs.begin;
        end = rhs.end;
    }
};


// token list
struct SyntaxTokenList
{
    std::vector<SyntaxToken> list;
    std::vector<SyntaxToken>::size_type howmany;
    
    // copy constructor
    SyntaxTokenList( const SyntaxTokenList & rhs )
    {
        // how big
        std::vector<SyntaxToken>::size_type size = ( rhs.howmany > 0 ? rhs.howmany :  2 );
        // allocate
        list.resize( size );
        // copy
        howmany = rhs.howmany;
    }
};


// token query
struct SyntaxQuery
{
public:
    t_CKBOOL parseLine( const std::string & line,
                        SyntaxTokenList & tokens );
};




#endif
