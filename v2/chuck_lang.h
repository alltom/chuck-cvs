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
// name: chuck_lang.h
// desc: chuck class library base
//
// authors: Ge Wang (gewang@cs.princeton.edu)
//          Perry R. Cook (prc@cs.princeton.edu)
//    date: spring 2005
//-----------------------------------------------------------------------------
#ifndef __CHUCK_LANG_H__
#define __CHUCK_LANG_H__

#include "chuck_def.h"
#include "chuck_oo.h"
#include "chuck_dl.h"


// query
DLL_QUERY lang_query( Chuck_DL_Query * QUERY );

// base class initialization
t_CKBOOL init_class_object( Chuck_Env * env, Chuck_Type * type );
t_CKBOOL init_class_event( Chuck_Env * env, Chuck_Type * type );
t_CKBOOL init_class_shred( Chuck_Env * env, Chuck_Type * type );
t_CKBOOL init_class_string( Chuck_Env * env, Chuck_Type * type );
t_CKBOOL init_class_array( Chuck_Env * env, Chuck_Type * type );




//-----------------------------------------------------------------------------
// object API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( object_ctor );
CK_DLL_DTOR( object_dtor );
CK_DLL_MFUN( object_equals );
CK_DLL_MFUN( object_getType );
CK_DLL_MFUN( object_hashCode );
CK_DLL_MFUN( object_toString );
CK_DLL_MFUN( object_setTestID );
CK_DLL_MFUN( object_getTestID );
CK_DLL_SFUN( object_getTest );




//-----------------------------------------------------------------------------
// array API
//-----------------------------------------------------------------------------
CK_DLL_MFUN( array_capacity );
CK_DLL_MFUN( array_size );
CK_DLL_MFUN( array_push_back );
CK_DLL_MFUN( array_pop_back );
CK_DLL_MFUN( array_push_front );
CK_DLL_MFUN( array_pop_font );
CK_DLL_MFUN( array_clear );
CK_DLL_MFUN( array_set_capacity );
CK_DLL_MFUN( array_set_size );




//-----------------------------------------------------------------------------
// name: Data_String
// desc: data for base Chuck string class
//-----------------------------------------------------------------------------
struct Data_String
{
public:
    Data_String( const std::string & s = "" ) { str = s; }
    ~Data_String() { }

public:
    std::string str;
};
//-----------------------------------------------------------------------------
// string API
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// name: Data_Event
// desc: data for base Chuck Event class
//-----------------------------------------------------------------------------
struct Data_Event
{
public:
    Data_Event();
    ~Data_Event();

public:
    t_CKUINT signal();
    t_CKUINT broadcast();
    t_CKUINT wait();
};
//-----------------------------------------------------------------------------
// event API
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// name: Data_Exception
// desc: data for base Chuck Exception class
//-----------------------------------------------------------------------------
struct Data_Exception
{
public:
    Data_Exception();
    ~Data_Exception();

public:
};
//-----------------------------------------------------------------------------
// exception API
//-----------------------------------------------------------------------------




#endif
