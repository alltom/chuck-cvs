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
// file: ugen_xxx.h
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#ifndef __UGEN_XXX_H__
#define __UGEN_XXX_H__

#include "chuck_dl.h"


// query
DLL_QUERY xxx_query( Chuck_DL_Query * query );

// noise
UGEN_TICK noise_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );

// impulse
UGEN_CTOR impulse_ctor( t_CKTIME now );
UGEN_DTOR impulse_dtor( t_CKTIME now, void * data );
UGEN_TICK impulse_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );
UGEN_CTRL impulse_ctrl_value( t_CKTIME now, void * data, void * value );

// step
UGEN_CTOR step_ctor( t_CKTIME now );
UGEN_DTOR step_dtor( t_CKTIME now, void * data );
UGEN_TICK step_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );
UGEN_CTRL step_ctrl_value( t_CKTIME now, void * data, void * value );

// gain
UGEN_CTOR gain_ctor( t_CKTIME now );
UGEN_DTOR gain_dtor( t_CKTIME now, void * data );
UGEN_TICK gain_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );
UGEN_CTRL gain_ctrl_value( t_CKTIME now, void * data, void * value );

// halfrect
UGEN_TICK halfrect_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );

// fullrect
UGEN_TICK fullrect_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );

// zerox
UGEN_CTOR zerox_ctor( t_CKTIME now );
UGEN_DTOR zerox_dtor( t_CKTIME now, void * data );
UGEN_TICK zerox_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );

// DAC and ADC
UGEN_TICK dac_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );

// sndbuf
UGEN_CTOR sndbuf_ctor( t_CKTIME now );
UGEN_DTOR sndbuf_dtor( t_CKTIME now, void * data );
UGEN_TICK sndbuf_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );
UGEN_CTRL sndbuf_ctrl_path( t_CKTIME now, void * data, void * value );
UGEN_CTRL sndbuf_ctrl_pos( t_CKTIME now, void * data, void * value );


#endif
