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
// name: chuck_lang.cpp
// desc: chuck class library base
//
// authors: Ge Wang (gewang@cs.princeton.edu)
//          Perry R. Cook (prc@cs.princeton.edu)
//    date: spring 2005
//-----------------------------------------------------------------------------
#include "chuck_lang.h"




//-----------------------------------------------------------------------------
// name: lang_query()
// desc: query entry point
//-----------------------------------------------------------------------------
DLL_QUERY lang_query( Chuck_DL_Query * QUERY )
{
    QUERY->setname( QUERY, "Lang" );

    /*! \nameinfo
    basic classes for ChucK class library
    */

    // class
    QUERY->begin_class( QUERY, "Object", "" );
    
    // add ctor
    QUERY->add_ctor( QUERY, object_ctor );

    // add dtor
    QUERY->add_dtor( QUERY, object_dtor );

    // add setTestID
    QUERY->add_mfun( QUERY, object_setTestID, "void", "setTestID" );
    QUERY->add_arg( QUERY, "int", "value" );

    // add getTestID
    QUERY->add_mfun( QUERY, object_getTestID, "int", "getTestID" );

    // add toString
    //! return string that represents the value of the object
    // QUERY->add_mfun( QUERY, object_toString, "string", "toString" );
    
    // end class
    QUERY->end_class( QUERY );

    return TRUE;
}


// Object ctor
CK_DLL_CTOR( object_ctor )
{
    fprintf( stderr, "object ctor\n" );
}


// Object dtor
CK_DLL_DTOR( object_dtor )
{
    fprintf( stderr, "object dtor\n" );
}


// setTestID
CK_DLL_MFUN( object_setTestID )
{
    t_CKINT v = GET_NEXT_INT(ARGS);
    fprintf( stderr, "setTestID( %i )\n", v );
}


// getTestID
CK_DLL_MFUN( object_getTestID )
{
    fprintf( stderr, "getTestID()\n" );
    RETURN->v_int = 2;
}
