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
// file: ulib_machine.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#include "ulib_machine.h"
#include "chuck_vm.h"




//-----------------------------------------------------------------------------
// name: machine_query()
// desc: query entry point
//-----------------------------------------------------------------------------
DLL_QUERY machine_query( Chuck_DL_Query * QUERY )
{
    QUERY->set_name( QUERY, "machine" );
    
    // add add
    QUERY->add_export( QUERY, "int", "add", machine_add_impl, TRUE );
    QUERY->add_param( QUERY, "string", "path" );
    
    // add spork
    QUERY->add_export( QUERY, "int", "spork", machine_add_impl, TRUE );
    QUERY->add_param( QUERY, "string", "path" );
    
    // add remove
    QUERY->add_export( QUERY, "int", "remove", machine_remove_impl, TRUE );
    QUERY->add_param( QUERY, "int", "id" );

    // add replace
    QUERY->add_export( QUERY, "int", "replace", machine_replace_impl, TRUE );
    QUERY->add_param( QUERY, "int", "id" );
    QUERY->add_param( QUERY, "string", "path" );

    // add status
    QUERY->add_export( QUERY, "int", "status", machine_status_impl, TRUE );

    return TRUE;
}



static Chuck_VM * the_vm = NULL;
static proc_msg_func the_func = NULL;
//-----------------------------------------------------------------------------
// name: machine_init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL machine_init( Chuck_VM * vm, proc_msg_func proc_msg )
{
    the_vm = vm;
    the_func = proc_msg;
    return TRUE;
}


// add
CK_DLL_FUNC( machine_add_impl )
{
    const char * v = *(const char **)ARGS;
    RETURN->v_int = (int)the_func( MSG_ADD, 0, v, TRUE );
}

// remove
CK_DLL_FUNC( machine_remove_impl )
{
    t_CKUINT v = *(t_CKUINT *)ARGS;
    RETURN->v_int = (int)the_func( MSG_REMOVE, v, NULL, TRUE );
}

// replace
CK_DLL_FUNC( machine_replace_impl )
{
    t_CKUINT v = *(t_CKUINT *)ARGS;
    const char * v2 = *((const char **)ARGS+1);
    RETURN->v_int = (int)the_func( MSG_REPLACE, v, v2, TRUE );
}

// status
CK_DLL_FUNC( machine_status_impl )
{
    RETURN->v_int = (int)the_func( MSG_STATUS, 0, NULL, TRUE );
}
