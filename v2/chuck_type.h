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

#include "chuck_def.h"
#include "chuck_absyn.h"
#include "chuck_oo.h"
using namespace std;



//-----------------------------------------------------------------------------
// name: enum te_Type
// desc: basic, default ChucK types
//-----------------------------------------------------------------------------
typedef enum {
    // general types
    te_int, te_uint, te_single, te_float, te_double, te_time, te_dur,
    te_string, te_thread, te_shred, te_class, te_function, te_object,
    te_user, te_array, te_null, te_ugen, te_event, te_void, te_stdout, 
    te_stderr, te_adc, te_dac, te_bunghole, te_midiin, te_midiout, 
    te_multi
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
    // reset the scope
    void reset()
    { scope.clear(); this->push(); }

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
            for( t_CKUINT i = scope.size(); i > 0; i-- )
                if( val = (*scope[i-1])[id] ) return val;

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
struct Chuck_Multi;
struct Chuck_UGen_Info;

class Chuck_VM;
class Chuck_VM_Code;


//-----------------------------------------------------------------------------
// name: struct Chuck_Namespace
// desc: Chuck Namespace containing semantic information
//-----------------------------------------------------------------------------
struct Chuck_Namespace : public Chuck_VM_Object
{
    // maps
    Chuck_Scope<Chuck_Type *> type;
    Chuck_Scope<Chuck_Value *> value;
    Chuck_Scope<Chuck_Func *> func;
    Chuck_Scope<Chuck_UGen_Info *> ugen;
    Chuck_Scope<void *> addr;    

    // virtual table
    Chuck_VTable obj_v_table;
    // static data segment
    t_CKBYTE * class_data;
    // static data segment size
    t_CKUINT class_data_size;

    // name
    string name;
    // top-level code
    Chuck_VM_Code * code;
    // type that contains this
    Chuck_Namespace * parent;
    // address offset
    t_CKUINT offset;

    // constructor
	Chuck_Namespace() { code = NULL; parent = NULL; offset = 0; 
                        class_data = NULL; class_data_size = 0; }
    // destructor
	~Chuck_Namespace() { }

    // look up type
    Chuck_Type * lookup_type( const string & name, t_CKBOOL climb = TRUE );
    Chuck_Type * lookup_type( S_Symbol name, t_CKBOOL climb = TRUE );
    // look up value
    Chuck_Value * lookup_value( const string & name, t_CKBOOL climb = TRUE );
    Chuck_Value * lookup_value( S_Symbol name, t_CKBOOL climb = TRUE );
    // look up func
    Chuck_Func * lookup_func( const string & name, t_CKBOOL climb = TRUE );
    Chuck_Func * lookup_func( S_Symbol name, t_CKBOOL climb = TRUE );
    // look up ugen
    Chuck_UGen_Info * lookup_ugen( const string & name, t_CKBOOL climb = TRUE );
    Chuck_UGen_Info * lookup_ugen( S_Symbol name, t_CKBOOL climb = TRUE );
    // look up addr
    void * lookup_addr( const string & name, t_CKBOOL climb = TRUE );
    void * lookup_addr( S_Symbol name, t_CKBOOL climb = TRUE );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Context
// desc: runtime type information pertaining to a file
//-----------------------------------------------------------------------------
struct Chuck_Context : public Chuck_VM_Object  
{
    // src_name
    string src;
	// parse tree
	a_Program parse_tree;
    // context namespace
    Chuck_Namespace nspc;

    // things to release with the context
    std::vector<Chuck_VM_Object *> new_types;
    std::vector<Chuck_VM_Object *> new_values;
    std::vector<Chuck_VM_Object *> new_funcs;
    std::vector<Chuck_VM_Object *> new_nspc;

    // constructor
	Chuck_Context() { parse_tree = NULL; }
	// destructor
	~Chuck_Context();

    // special alloc
    Chuck_Type * new_Chuck_Type();
    Chuck_Value * new_Chuck_Value( Chuck_Type * t, const string & name );
    Chuck_Func * new_Chuck_Func();
    Chuck_Namespace * new_Chuck_Namespace();
};



// where we are
enum ce_scope{ ce_global = 0, ce_class }; 

//-----------------------------------------------------------------------------
// name: struct Chuck_Env
// desc: chuck env with type info
//-----------------------------------------------------------------------------
struct Chuck_Env : public Chuck_VM_Object
{
	// global namespace
	Chuck_Namespace global;
	// namespace stack
	vector<Chuck_Namespace *> stack;
	// expression namespace
	Chuck_Namespace * curr;
    // current class definition
    Chuck_Type * class_def;
    // current function definition
    Chuck_Func * func;
    // how far nested in a class definition
    t_CKUINT class_scope;

	// current contexts in memory
	vector<Chuck_Context *> contexts;
    // current context
    Chuck_Context * context;

    // control scope (for break, continue)
    vector<a_Stmt> breaks;

	// VM reference
	Chuck_VM * vm;
	// chuck dlls in memory
	map<Chuck_DLL *, t_CKUINT> dlls;

    // reserved words
    map<string, t_CKBOOL> key_words;
    map<string, t_CKBOOL> key_types;
    map<string, t_CKBOOL> key_values;

	// constructor
	Chuck_Env( )
	{ vm = NULL; context = NULL; this->reset(); }
	// destructor
	~Chuck_Env() { }

	// reset
	void reset( )
	{ stack.clear(); stack.push_back( &global ); 
      if( context ) { contexts.pop_back(); context->release(); } 
      curr = &global; class_def = NULL; func = NULL;
      context = NULL; class_scope = 0; }

    // top
	Chuck_Namespace * top( )
	{ assert( stack.size() > 0 ); return stack.back(); }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Type
// desc: class containing information about a type
//-----------------------------------------------------------------------------
struct Chuck_Type : public Chuck_VM_Object
{
    // type id
    te_Type id;
    // type name
    string name;
    // type parent (could be NULL)
    Chuck_Type * parent;
    // size (in bytes)
    t_CKUINT size;
	// owner of the type
	Chuck_Namespace * owner;
    // array type
    union { Chuck_Type * array_type; Chuck_Type * actual_type; };
    // array size (equals 0 means not array, else dimension of array)
    t_CKUINT array_depth;
    // object size (size in memory)
    t_CKUINT obj_size;
    // self size (size in memory)
    t_CKUINT self_size;
    // type info
    Chuck_Namespace * info;
    // func info
    Chuck_Func * func;
    // def
    a_Class_Def def;
    // ugen
    Chuck_UGen_Info * ugen;
    // copy
    t_CKBOOL is_copy;

public:
    // constructor
    Chuck_Type( te_Type _id = te_null, const string & _n = "", 
                Chuck_Type * _p = NULL, t_CKUINT _s = 0 )
    { id = _id; name = _n; parent = _p; size = _s; owner = NULL; 
      array_type = NULL; array_depth = 0; obj_size = 0; self_size = 0;
      info = NULL; func = NULL; def = NULL; is_copy = FALSE; 
      ugen = NULL; }
    // destructor
    ~Chuck_Type() { reset(); }
    
    // reset
    void reset()
    { id = te_void; parent = NULL; ugen = NULL;
      size = array_depth = obj_size = self_size = 0;
      array_type = NULL; if( info ) info->release(); 
      owner = info = NULL; func = NULL; is_copy = FALSE; }
    
    // assignment - this does not touch the Chuck_VM_Object
    const Chuck_Type & operator =( const Chuck_Type & rhs )
    {
      // copy
      this->array_type = rhs.array_type;
      this->array_depth = rhs.array_depth;
      this->func = rhs.func;
      this->id = rhs.id;
      this->info = rhs.info;
      this->name = rhs.name;
      this->owner = rhs.owner;
      this->parent = rhs.parent;
      this->obj_size = rhs.obj_size;
      this->self_size = rhs.self_size;
      this->size = rhs.size;
      this->def = rhs.def;
      this->is_copy = TRUE;

      // TODO: fix this reference counting mess
      // add references
      // if( array_type ) rhs.array_type->add_ref();
      // if( info ) rhs.info->add_ref();
      
      return *this;
    }

    // copy
    Chuck_Type * copy( Chuck_Env * env ) const
    { Chuck_Type * n = env->context->new_Chuck_Type(); 
      *n = *this; return n; }
    
    // to string
    string ret;
    const string & str()
    { ret = name;
      for( t_CKUINT i = 0; i < array_depth; i++ ) ret += string("[]");
      return ret; }
    // to c
    const char * c_name()
    { return str().c_str(); }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Value
// desc: a variable in scope
//-----------------------------------------------------------------------------
struct Chuck_Value : public Chuck_VM_Object
{
    // type
    Chuck_Type * type;
    // name
    string name;
    // offset
    t_CKUINT offset;
    // addr
    void * addr;
    // const?
    t_CKBOOL is_const;
    // member?
    t_CKBOOL is_member;
    // is context-global?
    t_CKBOOL is_context_global;
    // 0 = public, 1 = protected, 2 = private
    t_CKUINT access;
    // owner
    Chuck_Namespace * owner;
    // owner (class)
    Chuck_Type * owner_class;

	// constructor
	Chuck_Value( Chuck_Type * t, const string & n, void * a = NULL,
                 t_CKBOOL c = FALSE, t_CKBOOL acc = 0, Chuck_Namespace * o = NULL,
                 Chuck_Type * oc = NULL, t_CKUINT s = 0 )
	{ type = t; name = n; offset = s; is_const = c; access = acc; 
      owner = o; owner_class = oc; addr = a; is_member = FALSE;
      is_context_global = FALSE; }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Func
// desc: function definition
//-----------------------------------------------------------------------------
struct Chuck_Func : public Chuck_VM_Object
{
    // name
    string name;
    // func def from parser
    a_Func_Def def;
    // code
    Chuck_VM_Code * code;
    // member
    t_CKBOOL is_member;
    // offset
    // t_CKUINT offset;
    // virtual table index
    t_CKINT vt_index;

    // constructor
    Chuck_Func() { def = NULL; code = NULL; is_member = FALSE; /* offset = 0; */ vt_index = -1; }
};




//-----------------------------------------------------------------------------
// primary chuck type checker interface
//-----------------------------------------------------------------------------
// initialize the type engine
Chuck_Env * type_engine_init( Chuck_VM * vm );
// shutdown the type engine
void type_engine_shutdown( Chuck_Env * env );
// load a context to be type-checked or emitted
t_CKBOOL type_engine_load_context( Chuck_Env * env, Chuck_Context * context );
// unload a context after being emitted
t_CKBOOL type_engine_unload_context( Chuck_Env * env );
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
t_CKBOOL operator !=( const Chuck_Type & lhs, const Chuck_Type & rhs );
t_CKBOOL equals( Chuck_Type * lhs, Chuck_Type * rhs );
t_CKBOOL operator <=( const Chuck_Type & lhs, const Chuck_Type & rhs );
t_CKBOOL isa( Chuck_Type * lhs, Chuck_Type * rhs );
t_CKBOOL isprim( Chuck_Type * type );
t_CKBOOL isobj( Chuck_Type * type );
t_CKBOOL isfunc( Chuck_Type * type );

// helpers
t_CKBOOL type_engine_check_reserved( Chuck_Env * env, const string & id, int pos );
t_CKBOOL type_engine_check_reserved( Chuck_Env * env, S_Symbol id, int pos );
t_CKBOOL type_engine_check_primitive( Chuck_Type * type );
t_CKBOOL type_engine_compat_func( a_Func_Def lhs, a_Func_Def rhs, int pos, string & err );
Chuck_Value * type_engine_find_value( Chuck_Type * type, const string & id );
Chuck_Value * type_engine_find_value( Chuck_Type * type, S_Symbol id );

// default types
extern Chuck_Type t_void;
extern Chuck_Type t_int;
extern Chuck_Type t_float;
extern Chuck_Type t_time;
extern Chuck_Type t_dur;
extern Chuck_Type t_object;
extern Chuck_Type t_null;
extern Chuck_Type t_string;
extern Chuck_Type t_shred;
extern Chuck_Type t_thread;
extern Chuck_Type t_function;
extern Chuck_Type t_class;
extern Chuck_Type t_event;
extern Chuck_Type t_ugen;




#endif
