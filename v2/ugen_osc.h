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
// file: ugen_osc.h
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Philip L. Davidson (philipd@alumni.princeton.edu)
// date: Summer 2004
//-----------------------------------------------------------------------------
#ifndef __UGEN_OSC_H__
#define __UGEN_OSC_H__

#include "chuck_dl.h"


// query
DLL_QUERY osc_query( Chuck_DL_Query * query );

// osc - base
CK_DLL_CTOR( osc_ctor );
CK_DLL_DTOR( osc_dtor );
CK_DLL_TICK( osc_tick );
CK_DLL_PMSG( osc_pmsg );
CK_DLL_CTRL( osc_ctrl_freq );
CK_DLL_CGET( osc_cget_freq );
CK_DLL_CTRL( osc_ctrl_period );
CK_DLL_CGET( osc_cget_period );
CK_DLL_CTRL( osc_ctrl_phase );
CK_DLL_CGET( osc_cget_phase );
CK_DLL_CTRL( osc_ctrl_width );
CK_DLL_CGET( osc_cget_width );
CK_DLL_CTRL( osc_ctrl_sync );
CK_DLL_CGET( osc_cget_sync );

// sinosc
CK_DLL_TICK( sinosc_tick );

// pulseosc
CK_DLL_TICK( pulseosc_tick );

// triosc
CK_DLL_TICK( triosc_tick );

// sawosc 
CK_DLL_CTOR( sawosc_ctor );
CK_DLL_CTRL( sawosc_ctrl_width );

// sqrosc 
CK_DLL_CTOR( sqrosc_ctor );
CK_DLL_CTRL( sqrosc_ctrl_width );




#endif
