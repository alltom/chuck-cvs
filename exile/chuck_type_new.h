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
struct Chuck_Scope
{
    vector<map<S_Symbol, t_CKUINT> *> scope;
    
    // constructor
    Chuck_Scope() { this->push(); }
    // desctructor
    ~Chuck_Scope() { this->pop(); }
    // push scope
    void push() { scope.push_back( new map<S_Symbol, t_CKUINT> ); }
    // pop scope
    void pop()
    { assert( scope.size() != 0 ); delete scope.back(); scope.pop_back(); }
    // add id
    void add( S_Symbol id, t_CKUINT value = TRUE )
    { assert( scope.size() != 0 ); (*scope.back())[id] = value; }
    // lookup id
    t_CKUINT lookup( S_Symbol id, t_CKBOOL local = TRUE )
    {
        t_CKUINT val; assert( scope.size() != 0 );
        if( local ) { return (*scope.back())[id]; }
        else {
            for( int i = 0; i < scope.size(); i++ )
                if( val = (*scope.back())[id] ) return val;
        }
        return 0;
    }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Env
// desc: environment containing semantic information
//-----------------------------------------------------------------------------
struct Chuck_Env
{
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Type
// desc: class containing information about a type
//-----------------------------------------------------------------------------
struct Chuck_Type
{
public:
    // type id
    te_Type id;
    // type name
    string name;
    // type parent (could be NULL)
    Chuck_Type parent;
    // size (in bytes)
    t_CKUINT size;
    // self size (not including inherited data)
    t_CKUINT self_size;
    // array size (equals 0 means not array, else dimension of array)
    t_CKUINT array_depth;
    // type environment
    Chuck_Env env;
};
