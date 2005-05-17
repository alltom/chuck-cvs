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
#include "chuck_type.h"
#include "chuck_vm.h"


// offset for member variable
static t_CKUINT object_offset_m_testID = CK_INVALID_OFFSET;

// storage for static variable
static t_CKINT object_our_testID = 0;


//-----------------------------------------------------------------------------
// name: init_class_object()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_object( Chuck_Env * env, Chuck_Type * type )
{
    Chuck_DL_Func * func = NULL;

    // init as base class
    if( !type_engine_import_class_begin( env, type, env->global(), (t_CKUINT)object_ctor ) )
        return FALSE;

    // add setTestID()
    func = make_new_mfun( "void", "setTestID", object_setTestID );
    func->add_arg( "int", "id" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add getTestID()
    func = make_new_mfun( "int", "getTestID", object_getTestID );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add getTest()
    func = make_new_sfun( "int", "testStatic", object_testStatic );
    func->add_arg( "float", "i" );
    if( !type_engine_import_sfun( env, func ) ) goto error;

    // add member variable
    object_offset_m_testID = type_engine_import_mvar( env, "int", "m_testID", FALSE );
    if( object_offset_m_testID == CK_INVALID_OFFSET ) goto error;

    // add static variable
    if( !type_engine_import_svar( env, "int", "our_testID", FALSE, (t_CKUINT)&object_our_testID ) )
        goto error;

    // end the class import
    type_engine_import_class_end( env );
    
    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}




//-----------------------------------------------------------------------------
// name: init_class_event()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_event( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
//    init_base_class( env, type, env->global(), (t_CKUINT)object_ctor );
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: init_class_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_shred( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
//    init_base_class( env, type, env->global(), (t_CKUINT)object_ctor );
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: init_class_string()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_string( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
//    init_base_class( env, type, env->global(), (t_CKUINT)string_ctor );
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: init_class_array()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_array( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
//    init_base_array( env, type, env->global(), (t_CKUINT)string_ctor );
    return TRUE;
}




// Object ctor
CK_DLL_CTOR( object_ctor )
{
//    fprintf( stderr, "object ctor\n" );
}


// Object dtor
CK_DLL_DTOR( object_dtor )
{
//    fprintf( stderr, "object dtor\n" );
}


// setTestID
CK_DLL_MFUN( object_setTestID )
{
    t_CKINT v = GET_NEXT_INT(ARGS);
    *(t_CKINT *)(SELF->data + object_offset_m_testID ) = v;
}


// getTestID
CK_DLL_MFUN( object_getTestID )
{
    RETURN->v_int = *(t_CKINT *)(SELF->data + object_offset_m_testID );
}


// getTest
CK_DLL_SFUN( object_testStatic )
{
    t_CKFLOAT v = GET_NEXT_FLOAT(ARGS);
    fprintf( stderr, "testStatic %f\n", v );
}




/*
//-----------------------------------------------------------------------------
// name: lang_query()
// desc: query entry point
//-----------------------------------------------------------------------------
DLL_QUERY lang_query( Chuck_DL_Query * QUERY )
{
    QUERY->setname( QUERY, "Lang" );

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
*/
