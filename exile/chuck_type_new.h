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
    te_string, te_thread, te_shred, te_array, te_function, te_object,
    te_ugen, te_null, te_event, te_tuple, te_midiin, te_midiout,
    te_adc, te_dac, te_bunghole,

    // system types (internal - cannot instantiate directly)
    te_void,              // no type
    te_system_out,        // stdout
    te_system_err,        // stderr
    te_system_namespace,  // namespace
    te_system_class,      // class
    te_system_user        // user-defined type
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
    { this->add( insert_symbo(id.c_str()), value ); }
    void add( S_Symbol id, const T & value )
    { assert( scope.size() != 0 ); (*scope.back())[id] = value; }

    // lookup id
    T lookup( const string & id, t_CKBOOL local = FALSE )
    { return this->lookup( insert_symbol(id.c_str()), local ); }
    T lookup( S_Symbol id, t_CKBOOL local = FALSE )
    {
        T val; assert( scope.size() != 0 );

        if( local )
            return (*scope.back())[id];
        else
            for( int i = 0; i < scope.size(); i++ )
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


//-----------------------------------------------------------------------------
// name: struct Chuck_Env
// desc: environment containing semantic information
//-----------------------------------------------------------------------------
struct Chuck_Env
{
    // maps
    Chuck_Scope<Chuck_Type *> * type;
    Chuck_Scope<Chuck_Value *> * value;
    Chuck_Scope<Chuck_Func *> * func;
    Chuck_Scope<Chuck_Env *> * class_defs;
    Chuck_Scope<Chuck_Env *> * nspc_defs;
    Chuck_Scope<void *> * addr;

    // name
    string name;
    // parent env
    Chuck_Env * parent;

    // static scope table    
    static Chuck_Scope<t_CKUINT> scope;
    
    // constructor
    Chuck_Env();
    // destructor
    ~Chuck_Env();
    
    // look up type
    Chuck_Type * lookup_type( const string & name, t_CKBOOL climb = TRUE );
    // look up value
    Chuck_Value * lookup_value( const string & name, t_CKBOOL climb = TRUE );
    // look up func
    Chuck_Func * lookup_func( const string & name, t_CKBOOL climb = TRUE );
    // look up class
    Chuck_Env * lookup_class( const string & name, t_CKBOOL climb = TRUE );
    // look up namespace
    Chuck_Env * lookup_nspc( const string & name, t_CKBOOL climb = TRUE );
    // look up addr
    void * lookup_addr( const string & name, t_CKBOOL climb = TRUE );
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
    // self size (not including inherited data)
    t_CKUINT self_size;
    // array size (equals 0 means not array, else dimension of array)
    t_CKUINT array_depth;
    // type environment
    Chuck_Env * env;
    
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
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Func
// desc: function definition
//-----------------------------------------------------------------------------
struct Chuck_Func
{
    // func def from parser
    a_Func_Def def;
    // name
    string name;
};




#endif
