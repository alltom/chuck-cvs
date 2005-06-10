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
// file: chuck_emit.h
// desc: chuck instruction emitter
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002 - first version
//       Autumn 2004 - redesign
//-----------------------------------------------------------------------------
#ifndef __CHUCK_EMIT_H__
#define __CHUCK_EMIT_H__

#include "chuck_def.h"
#include "chuck_oo.h"
#include "chuck_type.h"
#include "chuck_frame.h"


// forward references
struct Chuck_Instr;
struct Chuck_Instr_Goto;
struct Chuck_VM_Code;
struct Chuck_VM_Shred;



//-----------------------------------------------------------------------------
// name: struct Chuck_Code
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Code
{
public:
    // name
    string name;
    // stack depth
    t_CKUINT stack_depth;
    // need this
    t_CKBOOL need_this;
    // frame
    Chuck_Frame * frame;
    // code
    vector<Chuck_Instr *> code;

    // continue stack
    vector<Chuck_Instr_Goto *> stack_cont;
    // break stack
    vector<Chuck_Instr_Goto *> stack_break;
    // return stack
    vector<Chuck_Instr_Goto *> stack_return;
    
    // constructor
    Chuck_Code( )
    {
        stack_depth = 0;
        need_this = FALSE;
        frame = new Chuck_Frame;
    }

    // destructor
    ~Chuck_Code() { delete frame; frame = NULL; }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Emitter
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Emitter : public Chuck_VM_Object
{
    // reference to the type checker environment
    Chuck_Env * env;
    // reference to VM
    Chuck_VM * vm;

    // current code
    Chuck_Code * code;
    // current context
    Chuck_Context * context;
    // expression namespace
    Chuck_Namespace * nspc;
    // current function definition
    Chuck_Func * func;

    // code stack
    vector<Chuck_Code *> stack;
    // locals
    vector<Chuck_Local *> locals;

    // dump
    t_CKBOOL dump;

    // constructor
    Chuck_Emitter()
    { env = NULL; vm = NULL; code = NULL; context = NULL; 
      nspc = NULL; func = NULL; dump = FALSE; }

    // destructor
    ~Chuck_Emitter()
    { }

    // append instruction
    void append( Chuck_Instr * instr )
    { assert( code != NULL ); code->code.push_back( instr ); }
    // index
    t_CKUINT next_index()
    { assert( code != NULL ); return code->code.size(); }

    // push scope
    void push_scope( )
    { assert( code != NULL ); code->frame->push_scope(); }
    // alloc local
    Chuck_Local * alloc_local( t_CKUINT size, const string & name, t_CKBOOL is_ref )
    { assert( code != NULL ); return code->frame->alloc_local( size, name, is_ref ); }
    // pop scope
    void pop_scope( );

    // default durations
    t_CKBOOL find_dur( const string & name, t_CKDUR * out );
};




// function prototypes
Chuck_Emitter * emit_engine_init( Chuck_Env * env );
t_CKBOOL emit_engine_shutdown( Chuck_Emitter *& emit );
Chuck_VM_Code * emit_engine_emit_prog( Chuck_Emitter * emit, a_Program prog );
Chuck_VM_Code * emit_to_code( Chuck_Code * in, Chuck_VM_Code * out = NULL, t_CKBOOL dump = FALSE );
t_CKBOOL emit_engine_addr_map( Chuck_Emitter * emit, Chuck_VM_Shred * shred );
t_CKBOOL emit_engine_resolve( );




#endif
