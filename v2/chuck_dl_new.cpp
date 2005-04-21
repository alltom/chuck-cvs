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
// name: chuck_dl.cpp
// desc: chuck dynamic linking
//
// authors: Ge Wang (gewang@cs.princeton.edu)
//          Perry R. Cook (prc@cs.princeton.edu)
// mac os code based on apple's open source
//
// date: spring 2004 - 1.1
//       spring 2005 - 1.2
//-----------------------------------------------------------------------------
#include "chuck_dl_new.h"


//-----------------------------------------------------------------------------
// internal implementation of query functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// name: ck_setname()
// desc: set the name of the module
//-----------------------------------------------------------------------------
void CK_DLL_CALL ck_setname( Chuck_DL_Query * query, const char * name )
{
}


//-----------------------------------------------------------------------------
// name: ck_begin_class()
// desc: begin class/namespace, can be nexted
//-----------------------------------------------------------------------------
void CK_DLL_CALL ck_begin_class( Chuck_DL_Query * query, const char * name, const char * parent )
{
}


//-----------------------------------------------------------------------------
// name: ck_add_ctor()
// desc: add constructor, can be followed by add_arg
//-----------------------------------------------------------------------------
void CK_DLL_CALL ck_add_ctor( Chuck_DL_Query * query )
{
}


//-----------------------------------------------------------------------------
// name: ck_add_dtor()
// add destructor, no args allowed
//-----------------------------------------------------------------------------
void CK_DLL_CALL ck_add_dtor( Chuck_DL_Query * query )
{
}


//-----------------------------------------------------------------------------
// name: ck_add_mfun()
// desc: add member function, can be followed by add_arg
//-----------------------------------------------------------------------------
void CK_DLL_CALL ck_add_mfun( Chuck_DL_Query * query, f_mfun addr, 
                              const char * type, const char * name )
{
}


//-----------------------------------------------------------------------------
// name: ck_add_sfun()
// desc: add static function, can be followed by add_arg
//-----------------------------------------------------------------------------
void CK_DLL_CALL ck_add_sfun( Chuck_DL_Query * query, f_sfun addr,
                              const char * type, const char * name )
{
}


//-----------------------------------------------------------------------------
// name: ck_add_mvar()
// desc: add member var
//-----------------------------------------------------------------------------
void CK_DLL_CALL ck_add_mvar( Chuck_DL_Query * query, const char * type, const char * name,
                              t_CKBOOL is_const )
{
}


//-----------------------------------------------------------------------------
// name: ck_add_svar()
// desc: add static var
//-----------------------------------------------------------------------------
void CK_DLL_CALL ck_add_svar( Chuck_DL_Query * query, const char * type, const char * name,
                              t_CKBOOL is_const )
{
}


//-----------------------------------------------------------------------------
// name: ck_add_arg()
// desc: add argument to function
//-----------------------------------------------------------------------------
void CK_DLL_CALL add_arg( Chuck_DL_Query * query, const char * type, const char * name )
{
}


//-----------------------------------------------------------------------------
// name: ck_add_ugen_func()
// desc: (ugen only) add tick and pmsg functions
//-----------------------------------------------------------------------------
void CK_DLL_CALL ck_add_ugen_func( Chuck_DL_Query * query, f_tick ugen_tick, f_pmsg ugen_pmsg )
{
}


//-----------------------------------------------------------------------------
// name: ck_add_ugen_ctrl()
// desc: (ugen only) add ctrl parameters
//-----------------------------------------------------------------------------
void CK_DLL_CALL ck_add_ugen_ctrl( Chuck_DL_Query * query, f_ctrl ugen_ctrl, f_cget ugen_cget,
                                   const char * type, const char * name )
{
}


//-----------------------------------------------------------------------------
// name: ck_end_class()
// desc: end class/namespace, compile it
//-----------------------------------------------------------------------------
t_CKBOOL CK_DLL_CALL end_class( Chuck_DL_Query * query )
{
    return TRUE;
}
