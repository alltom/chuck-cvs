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




//-----------------------------------------------------------------------------
// name: init_base_class()
// desc: initialize base class (such as object, string, etc...)
//-----------------------------------------------------------------------------
void init_base_class( Chuck_Env * env, Chuck_Type * type, 
                      Chuck_Namespace * nspc, t_CKUINT pre_ctor )
{
    Chuck_Value * value = NULL;
    Chuck_Type * type_type = NULL;

    // make sure there is not namesapce
    assert( type->info == NULL );

    // allocate namespace for type
    type->info = new Chuck_Namespace;
    type->info->add_ref();
    // name it
    type->info->name = type->name;
    // set the parent namespace
    type->info->parent = nspc;

    // if pre constructor
    if( pre_ctor != NULL )
    {
        // flag it
        type->has_constructor = TRUE;
        // allocate vm code for pre_ctor
        type->info->pre_ctor = new Chuck_VM_Code;
        // add pre_ctor
        type->info->pre_ctor->native_func = pre_ctor;
        // specify that we need this
        type->info->pre_ctor->need_this = TRUE;
        // no arguments to preconstructor other than self
        type->info->pre_ctor->stack_depth = sizeof(t_CKUINT);
    }

    // set the beginning of the data segment after parent
    if( type->parent ) type->info->offset = type->parent->obj_size;
    // duplicate parent's virtual table
    type->info->obj_v_table = type->info->obj_v_table;

    // set the owner namespace
    type->owner = nspc;
    // set the size, which is always the width of a pointer
    type->size = sizeof(t_CKUINT);
    // set the object size
    type->obj_size = 0; // TODO

    // flag as complete
    type->is_complete = TRUE;
    // make type
    type_type = t_class.copy( env );
    type_type->actual_type = type;
    // make value
    value = new Chuck_Value( type_type, type->name );
    value->add_ref();
    value->owner = nspc;
    value->is_const = TRUE;
    value->is_member = FALSE;

    // add to env
    nspc->value.add( value->name, value );
}




//-----------------------------------------------------------------------------
// name: init_class_object()
// desc: ...
//-----------------------------------------------------------------------------
void init_class_object( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
    init_base_class( env, type, env->global(), (t_CKUINT)object_ctor );
}




//-----------------------------------------------------------------------------
// name: init_class_event()
// desc: ...
//-----------------------------------------------------------------------------
void init_class_event( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
//    init_base_class( env, type, env->global(), (t_CKUINT)object_ctor );
}




//-----------------------------------------------------------------------------
// name: init_class_shred()
// desc: ...
//-----------------------------------------------------------------------------
void init_class_shred( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
//    init_base_class( env, type, env->global(), (t_CKUINT)object_ctor );
}




//-----------------------------------------------------------------------------
// name: init_class_string()
// desc: ...
//-----------------------------------------------------------------------------
void init_class_string( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
//    init_base_class( env, type, env->global(), (t_CKUINT)string_ctor );
}




//-----------------------------------------------------------------------------
// name: init_class_array()
// desc: ...
//-----------------------------------------------------------------------------
void init_class_array( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
//    init_base_array( env, type, env->global(), (t_CKUINT)string_ctor );
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
