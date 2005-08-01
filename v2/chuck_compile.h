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
// file: chuck_compile.h
// desc: chuck compile system unifying parser, type checker, and emitter
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2005 - original
//-----------------------------------------------------------------------------
#ifndef __CHUCK_COMPILE_H__
#define __CHUCK_COMPILE_H__

#include "chuck_def.h"
#include "chuck_parse.h"
#include "chuck_type.h"
#include "chuck_emit.h"
#include "chuck_vm.h"




//-----------------------------------------------------------------------------
// name: struct Chuck_Compiler
// desc: the sum of the components in compilation
//-----------------------------------------------------------------------------
struct Chuck_Compiler
{
public: // data
    // type-checking environment
    Chuck_Env * env;
    // emitter
    Chuck_Emitter * emitter;
    // generated code
    Chuck_VM_Code * code;

    // auto-depend flag
    t_CKBOOL m_auto_depend;
    // recent map
    std::map<std::string, Chuck_Context *> m_recent;

public: // to all
    // contructor
    Chuck_Compiler();
    // destructor
    virtual ~Chuck_Compiler();

    // initialize
    t_CKBOOL initialize( Chuck_VM * vm );
    // shutdown
    void shutdown();

    // parse, type-check, and emit a program
    t_CKBOOL go( const string & filename, FILE * fd = NULL );
    // resolve a type automatically, if auto_depend is on
    t_CKBOOL resolve( const string & type );
    // get the code generated from the last go()
    Chuck_VM_Code * output( );

protected: // internal
    // do entire file
    t_CKBOOL do_entire_file( const string & path, FILE * fd = NULL );
    // do just class definitions
    t_CKBOOL do_only_classes( const string & path, FILE * fd = NULL );
    // do normal compile
    t_CKBOOL do_normal( const string & path, FILE * fd = NULL );
    // look up in recent
    Chuck_Context * find_recent_path( const string & path );
    // look up in recent
    Chuck_Context * find_recent_type( const string & type );
    // add to recent
    t_CKBOOL add_recent_path( const string & path, Chuck_Context * context );
};




#endif
