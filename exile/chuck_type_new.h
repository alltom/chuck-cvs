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
// file: chuck_type.h
// desc: chuck type-system / type-checker
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002 - original
//       Autumn 2004 - rewrite
//-----------------------------------------------------------------------------
#ifndef __CHUCK_TYPE_H__
#define __CHUCK_TYPE_H__

#include <stdlib.h>
#include <memory.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "chuck_absyn.h"




//-----------------------------------------------------------------------------
// name: enum te_Type
// desc: basic, default ChucK types
//-----------------------------------------------------------------------------
typedef enum {
    // general types
    te_int, te_uint, te_single, te_float, te_double, te_time, te_dur,
    te_string, te_thread, te_shred, te_class, te_function, te_object,
    te_null, te_ugen, te_event, te_void, te_stdout, te_stderr, te_user,
	te_adc, te_dac, te_bunghole, te_midiin, te_midiout
} te_Type;




//-----------------------------------------------------------------------------
// name: struct Chuck_Scope
// desc: scoping structure
//-----------------------------------------------------------------------------
template<class T>
struct Chuck_Scope
{
public:
    // constructor
    Chuck_Scope() { this->push(); }
    // desctructor
    ~Chuck_Scope() { this->pop(); }

    // push scope
    void push() { scope.push_back( new map<S_Symbol, T> ); }
    // pop scope
    void pop()
    { assert( scope.size() != 0 ); delete scope.back(); scope.pop_back(); }

    // add
    void add( const string & id, const T & value )
    { this->add( insert_symbol(id.c_str()), value ); }
    void add( S_Symbol id, const T & value )
    { assert( scope.size() != 0 ); (*scope.back())[id] = value; }

    // lookup id
	T operator []( const string & id )
	{ return this->lookup( id ); }
    T lookup( const string & id, t_CKBOOL local = FALSE )
    { return this->lookup( insert_symbol(id.c_str()), local ); }
    T lookup( S_Symbol id, t_CKBOOL local = FALSE )
    {
        T val; assert( scope.size() != 0 );

        if( local )
            return (*scope.back())[id];
        else
            for( t_CKUINT i = 0; i < scope.size(); i++ )
                if( val = (*scope.back())[id] ) return val;

        return 0;
    }

protected:
    vector<map<S_Symbol, T> *> scope;
};


// forward reference
struct Chuck_Type;
struct Chuck_Value;
struct Chuck_Func;
struct Chuck_DLL;
typedef struct Chuck_Type * t_CKTYPE;
class Chuck_VM;
class Chuck_VM_Code;


//-----------------------------------------------------------------------------
// name: struct Chuck_Namespace
// desc: Chuck Namespace containing semantic information
//-----------------------------------------------------------------------------
struct Chuck_Namespace
{
    // maps
    Chuck_Scope<Chuck_Type *> type;
    Chuck_Scope<Chuck_Value *> value;
    Chuck_Scope<Chuck_Func *> func;
    Chuck_Scope<Chuck_Namespace *> class_defs;
    Chuck_Scope<void *> addr;

    // name
    string name;
    // top-level code
    Chuck_VM_Code * code;
    // parent env
    Chuck_Namespace * parent;

    // constructor
	Chuck_Namespace() { parent = NULL; }
    // destructor
	~Chuck_Namespace() { }
    
    // look up type
    Chuck_Type * lookup_type( const string & name, t_CKBOOL climb = TRUE );
    // look up value
    Chuck_Value * lookup_value( const string & name, t_CKBOOL climb = TRUE );
    // look up func
    Chuck_Func * lookup_func( const string & name, t_CKBOOL climb = TRUE );
    // look up class
    Chuck_Namespace * lookup_class( const string & name, t_CKBOOL climb = TRUE );
    // look up addr
    void * lookup_addr( const string & name, t_CKBOOL climb = TRUE );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Context
// desc: runtime type information pertaining to a file
//-----------------------------------------------------------------------------
struct Chuck_Context
{
    // src_name
    string src;
	// parse tree
	a_Program parse_tree;
    // context namespace
    Chuck_Namespace nspc;
	// code
	Chuck_VM_Code * code;

	// constructor
	Chuck_Context() { parse_tree = NULL; code = NULL; }
	// destructor
	~Chuck_Context() { }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Env
// desc: chuck env with type info
//-----------------------------------------------------------------------------
struct Chuck_Env
{
	// global namespace
	Chuck_Namespace global;
	// namespace stack
	vector<Chuck_Namespace *> stack;
	// expression namespace
	Chuck_Namespace * curr;

    // scope table
    Chuck_Scope<t_CKUINT> scope;
    // control scope (for break, continue)
    vector<a_Stmt> loops;
    vector<a_Stmt> swich;
    // current function definition
    Chuck_Func * func;

	// current contexts in memory
	vector<Chuck_Context *> contexts;
    // current context
    Chuck_Context * context;

	// VM reference
	Chuck_VM * vm;
	// chuck dlls in memory
	map<Chuck_DLL *, t_CKUINT> dlls;

	// constructor
	Chuck_Env( )
	{ this->reset(); vm = NULL; context = NULL; func = NULL; }
	// destructor
	~Chuck_Env() { }

	// reset
	void reset( )
	{ stack.clear(); stack.push_back( &global ); curr = NULL; }

	// top
	Chuck_Namespace * top( )
	{ assert( stack.size() > 0 ); return stack.back(); }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Type
// desc: class containing information about a type
//-----------------------------------------------------------------------------
struct Chuck_Type
{
    // type id
    te_Type id;
    // type name
    string name;
    // type parent (could be NULL)
    Chuck_Type * parent;
    // size (in bytes)
    t_CKUINT size;
    // self size (size in memory)
    t_CKUINT self_size;
    // array size (equals 0 means not array, else dimension of array)
    t_CKUINT array_depth;
    // type info
    Chuck_Namespace * info;
	// owner of the type
	Chuck_Namespace * owner;

public:
    // copy
    Chuck_Type * copy()
    { Chuck_Type * n = new Chuck_Type; memcpy( n, this, sizeof(*this) ); }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Value
// desc: a variable in scope
//-----------------------------------------------------------------------------
struct Chuck_Value
{
    // type
    Chuck_Type * type;
    // name
    string name;
	// addr
	void * addr;

	// constructor
	Chuck_Value( Chuck_Type * t, const string & n, void * a = NULL )
	{ type = t; name = n; addr = a; }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Func
// desc: function definition
//-----------------------------------------------------------------------------
struct Chuck_Func
{
    // name
    string name;
    // func def from parser
    a_Func_Def def;
    // VM code
    Chuck_VM_Code * code;
};




//-----------------------------------------------------------------------------
// primary chuck type checker interface
//-----------------------------------------------------------------------------
// initialize the type engine
Chuck_Env * type_engine_init( Chuck_VM * vm );
// shutdown the type engine
void type_engine_shutdown( Chuck_Env * env );
// type check a program into the env
t_CKBOOL type_engine_check_prog( Chuck_Env * env, a_Program prog );
// type check a statement
t_CKBOOL type_engine_check_stmt( Chuck_Env * env, a_Stmt stmt );
// type check an expression
t_CKTYPE type_engine_check_exp( Chuck_Env * env, a_Exp exp );
// add an chuck dll into the env
t_CKBOOL type_engine_add_dll( Chuck_Env * env, Chuck_DLL * dll, const string & nspc );
// type equality
t_CKBOOL operator ==( const Chuck_Type & lhs, const Chuck_Type & rhs );
t_CKBOOL equals( Chuck_Type * lhs, Chuck_Type * rhs );
t_CKBOOL operator >>( const Chuck_Type & lhs, const Chuck_Type & rhs );
t_CKBOOL isa( Chuck_Type * lhs, Chuck_Type * rhs );




#endif
