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




//-----------------------------------------------------------------------------
// object API
//-----------------------------------------------------------------------------
CK_DLL_MFUN( object_setTestID );
CK_DLL_MFUN( object_getTestID );
CK_DLL_MFUN( object_toString );




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
// name: Chuck_String
// desc: base Chuck string class
//-----------------------------------------------------------------------------
struct Chuck_String
{
public:
    Chuck_String( const std::string & s = "" ) { str = s; }
    ~Chuck_String() { }

public:
    std::string str;
};
//-----------------------------------------------------------------------------
// string API
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// name: Chuck_Event
// desc: base Chuck Event class
//-----------------------------------------------------------------------------
struct Chuck_Event
{
public:
    Chuck_Event();
    ~Chuck_Event();

public:
    t_CKUINT signal();
    t_CKUINT broadcast();
    t_CKUINT wait();
};
//-----------------------------------------------------------------------------
// Event API
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// name: Chuck_Exception
// desc: base Chuck Exception class
//-----------------------------------------------------------------------------
struct Chuck_Exception
{
public:
    Chuck_Exception();
    ~Chuck_Exception();

public:
};
//-----------------------------------------------------------------------------
// Exception API
//-----------------------------------------------------------------------------




#endif
