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
// file: chuck_oo.h
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#ifndef __CHUCK_OO_H__
#define __CHUCK_OO_H__

#include "chuck_def.h"
#include <string>
#include <vector>
#include <map>
#include <queue>


// forward reference
struct Chuck_Type;
struct Chuck_Value;
struct Chuck_Func;
struct Chuck_Namespace;
struct Chuck_Context;
struct Chuck_Env;
struct Chuck_VM_Code;
struct Chuck_VM_Shred;
struct Chuck_VM;




//-----------------------------------------------------------------------------
// name: struct Chuck_VM_Object
// desc: base vm object
//-----------------------------------------------------------------------------
struct Chuck_VM_Object
{
public:
    Chuck_VM_Object() { this->init_ref(); }
    virtual ~Chuck_VM_Object() { }

public:
    void add_ref();
    void release();

public:
    t_CKUINT m_ref_count;
    t_CKBOOL m_pooled;

public:
    // where
    std::vector<Chuck_VM_Object *> * m_v_ref;
    
private:
    void init_ref();
};




//-----------------------------------------------------------------------------
// name: struct Chuck_VM_Alloc
// desc: vm object manager
//-----------------------------------------------------------------------------
struct Chuck_VM_Alloc
{
public:
    static Chuck_VM_Alloc * instance();
    
public:
    void add_object( Chuck_VM_Object * obj );
    void free_object( Chuck_VM_Object * obj );

protected:
    static Chuck_VM_Alloc * our_instance;

protected:
    Chuck_VM_Alloc();
    ~Chuck_VM_Alloc();

protected: // data
    std::map<Chuck_VM_Object *, void *> m_objects;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_VTable
// desc: virtual table
//-----------------------------------------------------------------------------
struct Chuck_VTable
{
public:
    std::vector<Chuck_Func *> funcs;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Object
// dsec: base object
//-----------------------------------------------------------------------------
struct Chuck_Object : Chuck_VM_Object
{
public:
    Chuck_Object();
    virtual ~Chuck_Object();

public:
    // virtual table
    Chuck_VTable * vtable;
    // reference to type
    Chuck_Type * type_ref;
    // the size of the data region
    t_CKUINT size;
    // data for the object
    t_CKBYTE * data;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Array4
// desc: native ChucK arrays (for 4-byte)
//-----------------------------------------------------------------------------
struct Chuck_Array4 : Chuck_Object
{
public:
    Chuck_Array4( t_CKBOOL is_obj, t_CKINT capacity = 8 );
    ~Chuck_Array4();

public:
    t_CKUINT addr( t_CKINT i );
    t_CKUINT addr( const std::string & key );
    t_CKINT get( t_CKINT i, t_CKUINT * val );
    t_CKINT get( const std::string & key, t_CKUINT * val );
    t_CKINT set( t_CKINT i, t_CKUINT val );
    t_CKINT set( const std::string & key, t_CKUINT val );
    t_CKINT erase( const std::string & key );
    t_CKINT push_back( t_CKUINT val );
    t_CKINT pop_back( );
    t_CKINT back( t_CKUINT * val ) const;
    t_CKINT size( ) const { return m_size; }
    t_CKINT capacity( ) const { return m_capacity; }
    void clear( );

public:
    std::vector<t_CKUINT> m_vector;
    std::map<std::string, t_CKUINT> m_map;
    t_CKINT m_size;
    t_CKINT m_capacity;
    t_CKBOOL m_is_obj;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Array8
// desc: native ChucK arrays (for 8-byte)
//-----------------------------------------------------------------------------
struct Chuck_Array8 : Chuck_Object
{
public:
    Chuck_Array8( t_CKINT capacity = 8 );
    ~Chuck_Array8();

public:
    t_CKUINT addr( t_CKINT i );
    t_CKUINT addr( const std::string & key );
    t_CKINT get( t_CKINT i, t_CKFLOAT * val );
    t_CKINT get( const std::string & key, t_CKFLOAT * val );
    t_CKINT set( t_CKINT i, t_CKFLOAT val );
    t_CKINT set( const std::string & key, t_CKFLOAT val );
    t_CKINT erase( const std::string & key );
    t_CKINT push_back( t_CKFLOAT val );
    t_CKINT pop_back( );
    t_CKINT back( t_CKFLOAT * val ) const;
    t_CKINT size( ) const { return m_size; }
    t_CKINT capacity( ) const { return m_capacity; }
    void clear( );

public:
    std::vector<t_CKFLOAT> m_vector;
    std::map<std::string, t_CKFLOAT> m_map;
    t_CKINT m_size;
    t_CKINT m_capacity;
};




//-----------------------------------------------------------------------------
// name: Chuck_Event
// desc: base Chuck Event class
//-----------------------------------------------------------------------------
struct Chuck_Event : Chuck_Object
{
public:
    void signal();
    void broadcast();
    void wait( Chuck_VM_Shred * shred, Chuck_VM * vm );

	std::queue<Chuck_VM_Shred *> m_queue;
};




//-----------------------------------------------------------------------------
// name: Chuck_String
// desc: base Chuck string class
//-----------------------------------------------------------------------------
struct Chuck_String : Chuck_Object
{
public:
    Chuck_String( const std::string & s = "" ) { str = s; }
    ~Chuck_String() { }

public:
    std::string str;
};




#endif
