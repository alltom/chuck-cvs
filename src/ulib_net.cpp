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
// file: ulib_net.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#include "ulib_net.h"




//-----------------------------------------------------------------------------
// name: net_query()
// desc: ...
//-----------------------------------------------------------------------------
DLL_QUERY net_query( Chuck_DL_Query * QUERY )
{
/*    // add udpout
    QUERY->ugen_add( QUERY, "udpout", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, udpout_ctor, udpout_dtor, udpout_tick, NULL );
    // ctrl funcs
    QUERY->ugen_ctrl( QUERY, udpout_ctrl_addr, udpout_cget_addr, "string", "addr" );
    QUERY->ugen_ctrl( QUERY, udpout_ctrl_port, udpout_cget_port, "int", "port" );
    QUERY->ugen_ctrl( QUERY, udpout_ctrl_size, udpout_cget_size, "dur", "size" );
    QUERY->ugen_ctrl( QUERY, udpout_ctrl_name, udpout_cget_name, "string", "name" );
    
    // add udpin
    QUERY->ugen_add( QUERY, "udpin", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, udpin_ctor, udpin_dtor, udpin_tick, NULL );
    // ctrl funcs
    QUERY->ugen_ctrl( QUERY, udpin_ctrl_port, udpin_cget_port, "int", "port" );
    QUERY->ugen_ctrl( QUERY, udpin_ctrl_name, udpin_cget_name, "string", "name" );
*/    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: udpout
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTOR udpout_ctor( t_CKTIME now );
UGEN_DTOR udpout_dtor( t_CKTIME now, void * data );
UGEN_TICK udpout_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );
UGEN_CTRL udpout_ctrl_addr( t_CKTIME now, void * data, void * value );
UGEN_CGET udpout_cget_addr( t_CKTIME now, void * data, void * out );
UGEN_CTRL udpout_ctrl_port( t_CKTIME now, void * data, void * value );
UGEN_CGET udpout_cget_port( t_CKTIME now, void * data, void * out );
UGEN_CTRL udpout_ctrl_size( t_CKTIME now, void * data, void * value );
UGEN_CGET udpout_cget_size( t_CKTIME now, void * data, void * out );
UGEN_CTRL udpout_ctrl_name( t_CKTIME now, void * data, void * value );
UGEN_CGET udpout_cget_name( t_CKTIME now, void * data, void * out );




//-----------------------------------------------------------------------------
// name: udpin
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTOR udpin_ctor( t_CKTIME now );
UGEN_DTOR udpin_dtor( t_CKTIME now, void * data );
UGEN_TICK udpin_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );
UGEN_CTRL udpin_ctrl_port( t_CKTIME now, void * data, void * value );
UGEN_CGET udpin_cget_port( t_CKTIME now, void * data, void * out );
UGEN_CTRL udpin_ctrl_name( t_CKTIME now, void * data, void * value );
UGEN_CGET udpin_cget_name( t_CKTIME now, void * data, void * out );
