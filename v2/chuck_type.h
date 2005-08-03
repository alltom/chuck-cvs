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
#include "chuck_dl.h"
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
// name: enum te_HowMuch
// desc: how much to scan/type check
//-----------------------------------------------------------------------------
typedef enum { 
    te_do_all, te_do_classes_only, te_do_no_classes
} te_HowMuch;




//-----------------------------------------------------------------------------
// name: struct Chuck_Scope
// desc: scoping structure
//-----------------------------------------------------------------------------
template <class T>
struct Chuck_Scope
{
public:
    // constructor
    Chuck_Scope() { this->push(); }
    // desctructor
    ~Chuck_Scope() { this->pop(); }

    // push scope
    void push() { scope.push_back( new map<S_Symbol, Chuck_VM_Object *> ); }

    // pop scope
    void pop()
    {
        assert( scope.size() != 0 );
        // TODO: release contents of scope.back()
        delete scope.back(); scope.pop_back();
    }

    // reset the scope
    void reset()
    { scope.clear(); this->push(); }
    
    // atomic commit
    void commit()
    {
        assert( scope.size() != 0 );        
        std::map<S_Symbol, Chuck_VM_Object *>::iterator iter;

        // go through buffer    
        for( iter = commit_map.begin(); iter != commit_map.end(); iter++ )
        {
            // add to front/where
            (*scope.front())[(*iter).first] = (*iter).second;
        }

        // clear
        commit_map.clear();
    }

    // roll back since last commit or beginning
    void rollback()
    {
        assert( scope.size() != 0 );
        std::map<S_Symbol, Chuck_VM_Object *>::iterator iter;

        // go through buffer    
        for( iter = commit_map.begin(); iter != commit_map.end(); iter++ )
        {
            // release
            (*iter).second->release();
        }

        // clear
        commit_map.clear();
    }

    // add
    void add( const string & id, Chuck_VM_Object * value )
    { this->add( insert_symbol(id.c_str()), value ); }
    void add( S_Symbol id, Chuck_VM_Object * value )
    { 
        assert( scope.size() != 0 );
        // add if back is NOT front
        if( scope.back() != scope.front() ) (*scope.back())[id] = value; 
        // add for commit
        else commit_map[id] = value;
    }

    // lookup id
    T operator []( const string & id )
    { return (T)this->lookup( id ); }
    T lookup( const string & id, t_CKINT climb = 1 )
    { return (T)this->lookup( insert_symbol(id.c_str()), climb ); }
    // -1 base, 0 current, 1 climb
    T lookup( S_Symbol id, t_CKINT climb = 1 )
    {
        Chuck_VM_Object * val; assert( scope.size() != 0 );

        if( climb == 0 )
        {
            val = (*scope.back())[id];
            // look in commit buffer if the back is the front
            if( !val && scope.back() == scope.front() 
                && (commit_map.find(id) != commit_map.end()) ) 
                val = commit_map[id];
        }
        else if( climb > 0 )
        {
            for( t_CKUINT i = scope.size(); i > 0; i-- )
                if( val = (*scope[i-1])[id] ) break;
            // look in commit buffer
            if( !val && (commit_map.find(id) != commit_map.end()) )
                val = commit_map[id];
        }
        else
        {
            val = (*scope.front())[id];
            // look in commit buffer
            if( !val && (commit_map.find(id) != commit_map.end()) )
                val = commit_map[id];
        }

        return (T)val;
    }

protected:
    std::vector<map<S_Symbol, Chuck_VM_Object *> *> scope;
    std::map<S_Symbol, Chuck_VM_Object *> commit_map;
};


// forward reference
struct Chuck_Type;
struct Chuck_Value;
struct Chuck_Func;
struct Chuck_Multi;
struct Chuck_VM;
struct Chuck_VM_Code;

struct Chuck_DLL;


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

    // virtual table
    Chuck_VTable obj_v_table;
    // static data segment
    t_CKBYTE * class_data;
    // static data segment size
    t_CKUINT class_data_size;

    // name
    string name;
    // top-level code
    Chuck_VM_Code * pre_ctor;
    // type that contains this
    Chuck_Namespace * parent;
    // address offset
    t_CKUINT offset;

    // constructor
    Chuck_Namespace() { pre_ctor = NULL; parent = NULL; offset = 0; 
                        class_data = NULL; class_data_size = 0; }
    // destructor
    ~Chuck_Namespace() { }

    // look up type
    Chuck_Type * lookup_type( const string & name, t_CKINT climb = 1 );
    Chuck_Type * lookup_type( S_Symbol name, t_CKINT climb = 1 );
    // look up value
    Chuck_Value * lookup_value( const string & name, t_CKINT climb = 1 );
    Chuck_Value * lookup_value( S_Symbol name, t_CKINT climb = 1 );
    // look up func
    Chuck_Func * lookup_func( const string & name, t_CKINT climb = 1 );
    Chuck_Func * lookup_func( S_Symbol name, t_CKINT climb = 1 );

    // commit the maps
    void commit() { type.commit(); value.commit(); func.commit(); }
    // rollback the maps
    void rollback() { type.rollback(); value.rollback(); func.rollback(); }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Context
// desc: runtime type information pertaining to a file
//-----------------------------------------------------------------------------
struct Chuck_Context : public Chuck_VM_Object  
{
    // src_name
    string filename;
    // parse tree
    a_Program parse_tree;
    // context namespace
    Chuck_Namespace * nspc;
    // public class def if any
    a_Class_Def public_class_def;
    // error - means to free nspc too
    t_CKBOOL has_error;
    // progress in scan / type check
    // t_CKUINT progress;

    // things to release with the context
    std::vector<Chuck_VM_Object *> new_types;
    std::vector<Chuck_VM_Object *> new_values;
    std::vector<Chuck_VM_Object *> new_funcs;
    std::vector<Chuck_VM_Object *> new_nspc;

    // commit map
    std::map<Chuck_Namespace *, Chuck_Namespace *> commit_map;
    // add for commit/rollback
    void add_commit_candidate( Chuck_Namespace * nspc );
    // commit
    void commit();
    // rollback
    void rollback();

    // constructor
    Chuck_Context() { parse_tree = NULL; nspc = new Chuck_Namespace; 
                      public_class_def = NULL; has_error = FALSE; }
    // destructor
    ~Chuck_Context();

    // special alloc
    Chuck_Type * new_Chuck_Type();
    Chuck_Value * new_Chuck_Value( Chuck_Type * t, const string & name );
    Chuck_Func * new_Chuck_Func();
    Chuck_Namespace * new_Chuck_Namespace();
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Env
// desc: chuck env with type info
//-----------------------------------------------------------------------------
struct Chuck_Env : public Chuck_VM_Object
{
public:
    static Chuck_Env * instance();

private:
    static Chuck_Env * our_instance;
    // constructor
    Chuck_Env( )
    { context = &global_context; 
      global_nspc = global_context.nspc;
      this->reset(); }

protected:
    // global namespace
    Chuck_Namespace * global_nspc;
    // global context
    Chuck_Context global_context;

public:
    // global namespace
    Chuck_Namespace * global() { return global_nspc; }
    // namespace stack
    vector<Chuck_Namespace *> nspc_stack;
    // expression namespace
    Chuck_Namespace * curr;
    // class stack
    vector<Chuck_Type *> class_stack;
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
    // Chuck_VM * vm;
    // chuck dlls in memory
    // map<Chuck_DLL *, t_CKUINT> dlls;

    // reserved words
    map<string, t_CKBOOL> key_words;
    map<string, t_CKBOOL> key_types;
    map<string, t_CKBOOL> key_values;

    // destructor
    ~Chuck_Env() { }

    // reset
    void reset( )
    { nspc_stack.clear(); nspc_stack.push_back( this->global() );
      class_stack.clear(); class_stack.push_back( NULL );
      assert( context == &global_context );
      // if( context ) { contexts.pop_back(); context->release(); } 
      curr = this->global(); class_def = NULL; func = NULL;
      class_scope = 0; }

    // top
    Chuck_Namespace * nspc_top( )
    { assert( nspc_stack.size() > 0 ); return nspc_stack.back(); }
    Chuck_Type * class_top( )
    { assert( class_stack.size() > 0 ); return class_stack.back(); }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_UGen_Info
// desc: ugen info stored with ugen types
//-----------------------------------------------------------------------------
struct Chuck_UGen_Info : public Chuck_VM_Object
{
    // tick function pointer
    f_tick tick;
    // pmsg function pointer
    f_pmsg pmsg;
    // number of incoming channels
    t_CKUINT num_ins;
    // number of outgoing channels
    t_CKUINT num_outs;

    // constructor
    Chuck_UGen_Info()
    { tick = NULL; pmsg = NULL; num_ins = 1; num_outs = 1; }
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
    // type info
    Chuck_Namespace * info;
    // func info
    Chuck_Func * func;
    // def
    a_Class_Def def;
    // ugen
    Chuck_UGen_Info * ugen_info;
    // copy
    t_CKBOOL is_copy;
    // defined
    t_CKBOOL is_complete;
    // has pre constructor
    t_CKBOOL has_constructor;

public:
    // constructor
    Chuck_Type( te_Type _id = te_null, const string & _n = "", 
                Chuck_Type * _p = NULL, t_CKUINT _s = 0 )
    { id = _id; name = _n; parent = _p; size = _s; owner = NULL; 
      array_type = NULL; array_depth = 0; obj_size = 0;
      info = NULL; func = NULL; def = NULL; is_copy = FALSE; 
      ugen_info = NULL; is_complete = TRUE; has_constructor = FALSE; }
    // destructor
    ~Chuck_Type() { reset(); }
    
    // reset
    void reset()
    { id = te_void; parent = NULL;
      size = array_depth = obj_size = 0;
      // fprintf( stderr, "type: %s %i\n", c_name(), (t_CKUINT)this );
      array_type = NULL; if( info ) info->release(); 
      owner = info = NULL; func = NULL; is_copy = FALSE;
      if( ugen_info ) ugen_info->release(); }
    
    // assignment - this does not touch the Chuck_VM_Object
    const Chuck_Type & operator =( const Chuck_Type & rhs )
    {
      // copy
      this->array_type = rhs.array_type;
      this->array_depth = rhs.array_depth;
      this->func = rhs.func;
      this->id = rhs.id;
      this->info = rhs.info;
      if( this->info ) this->info->add_ref(); // HACK:
      this->name = rhs.name;
      this->owner = rhs.owner;
      this->parent = rhs.parent;
      this->obj_size = rhs.obj_size;
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
    // static?
    t_CKBOOL is_static;  // do something
    // is context-global?
    t_CKBOOL is_context_global;
    // 0 = public, 1 = protected, 2 = private
    t_CKUINT access;
    // owner
    Chuck_Namespace * owner;
    // owner (class)
    Chuck_Type * owner_class;
    // remember function pointer - if this is a function
    Chuck_Func * func_ref;
    // overloads
    t_CKINT func_num_overloads;

    // constructor
    Chuck_Value( Chuck_Type * t, const string & n, void * a = NULL,
                 t_CKBOOL c = FALSE, t_CKBOOL acc = 0, Chuck_Namespace * o = NULL,
                 Chuck_Type * oc = NULL, t_CKUINT s = 0 )
    { type = t; name = n; offset = s; is_const = c; access = acc; 
      owner = o; owner_class = oc; addr = a; is_member = FALSE; is_static = FALSE;
      is_context_global = FALSE; func_ref = NULL; func_num_overloads = 0; }
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
    // imported code
    Chuck_DL_Func * dl_code;
    // member
    t_CKBOOL is_member;
    // virtual table index
    t_CKUINT vt_index;
    // rember value
    Chuck_Value * value_ref;
    // for overloading
    Chuck_Func * next;
    // for overriding
    Chuck_Value * up;

    // constructor
    Chuck_Func() { def = NULL; code = NULL; is_member = FALSE; vt_index = 0xffffffff; 
                   value_ref = NULL; dl_code = NULL; next = NULL; up = NULL; }
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
t_CKBOOL type_engine_check_prog( Chuck_Env * env, a_Program prog, 
                                 const string & filename );
// make a context
Chuck_Context * type_engine_make_context( a_Program prog,
                                          const string & filename );
// type check a context into the env
t_CKBOOL type_engine_check_context( Chuck_Env * env,
                                    Chuck_Context * context,
                                    te_HowMuch how_much = te_do_all );
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

// import
Chuck_Type * type_engine_import_class_begin( Chuck_Env * env, Chuck_Type * type, 
                                             Chuck_Namespace * where, f_ctor pre_ctor );
Chuck_Type * type_engine_import_class_begin( Chuck_Env * env, const char * name, const char * parent,
                                             Chuck_Namespace * where, f_ctor pre_ctor );
Chuck_Type * type_engine_import_ugen_begin( Chuck_Env * env, const char * name, const char * parent,
                                            Chuck_Namespace * where, f_ctor pre_ctor,
                                            f_tick tick, f_pmsg pmsg,
                                            t_CKUINT num_ins = 0xffffffff, t_CKUINT num_outs = 0xffffffff );
t_CKBOOL type_engine_import_mfun( Chuck_Env * env, Chuck_DL_Func * mfun );
t_CKBOOL type_engine_import_sfun( Chuck_Env * env, Chuck_DL_Func * sfun );
t_CKUINT type_engine_import_mvar( Chuck_Env * env, const char * type, 
                                  const char * name, t_CKUINT is_const );
t_CKBOOL type_engine_import_svar( Chuck_Env * env, const char * type,
                                  const char * name, t_CKUINT is_const,
                                  t_CKUINT addr );
t_CKBOOL type_engine_import_ugen_ctrl( Chuck_Env * env, const char * type, const char * name,
                                       f_ctrl ctrl, t_CKBOOL write, t_CKBOOL read );
t_CKBOOL type_engine_import_class_end( Chuck_Env * env );

// helpers
t_CKBOOL type_engine_check_reserved( Chuck_Env * env, const string & id, int pos );
t_CKBOOL type_engine_check_reserved( Chuck_Env * env, S_Symbol id, int pos );
t_CKBOOL type_engine_check_primitive( Chuck_Type * type );
t_CKBOOL type_engine_compat_func( a_Func_Def lhs, a_Func_Def rhs, int pos, string & err, t_CKBOOL print = TRUE );
Chuck_Type  * type_engine_find_common_anc( Chuck_Type * lhs, Chuck_Type * rhs );
Chuck_Type  * type_engine_find_type( Chuck_Env * env, a_Id_List path );
Chuck_Value * type_engine_find_value( Chuck_Type * type, const string & id );
Chuck_Value * type_engine_find_value( Chuck_Type * type, S_Symbol id );
Chuck_Namespace * type_engine_find_nspc( Chuck_Env * env, a_Id_List path );

// array verify
t_CKBOOL verify_array( a_Array_Sub array );
// conversion
const char * type_path( a_Id_List path );
a_Id_List str2list( const string & path );

// default types
extern Chuck_Type t_void;
extern Chuck_Type t_int;
extern Chuck_Type t_float;
extern Chuck_Type t_time;
extern Chuck_Type t_dur;
extern Chuck_Type t_object;
extern Chuck_Type t_null;
extern Chuck_Type t_string;
extern Chuck_Type t_array;
extern Chuck_Type t_shred;
extern Chuck_Type t_thread;
extern Chuck_Type t_function;
extern Chuck_Type t_class;
extern Chuck_Type t_event;
extern Chuck_Type t_ugen;




#endif
