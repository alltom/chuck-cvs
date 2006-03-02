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
//         Ananya Misra (amisra@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#ifndef __UGEN_XXX_H__
#define __UGEN_XXX_H__

#include "chuck_dl.h"


// query
DLL_QUERY xxx_query( Chuck_DL_Query * query );

// global
struct Chuck_Type;
extern Chuck_Type * g_t_dac;
extern Chuck_Type * g_t_adc;

// stereo
CK_DLL_CTOR( stereo_ctor );
CK_DLL_CTRL( stereo_ctrl_pan );
CK_DLL_CGET( stereo_cget_pan );

// multi
CK_DLL_CTOR( multi_ctor );
CK_DLL_CTRL( multi_ctrl_pan );
CK_DLL_CGET( multi_cget_pan );
CK_DLL_CGET( multi_cget_chan );

// bunghole
CK_DLL_TICK( bunghole_tick );

// pan2
CK_DLL_CTOR( pan2_ctor );
CK_DLL_DTOR( pan2_dtor );
CK_DLL_CTRL( pan2_ctrl_value );
CK_DLL_CGET( pan2_cget_value );

// MIX2
CK_DLL_CTOR( mix2_ctor );
CK_DLL_CTOR( mix2_dtor );
CK_DLL_CTRL( mix2_ctrl_value );
CK_DLL_CGET( mix2_cget_value );

// noise
CK_DLL_TICK( noise_tick );

// cnoise
CK_DLL_CTOR( cnoise_ctor );
CK_DLL_DTOR( cnoise_dtor );
CK_DLL_TICK( cnoise_tick );
CK_DLL_CTRL( cnoise_ctrl_mode );
CK_DLL_CTRL( cnoise_ctrl_fprob );

// impulse
CK_DLL_CTOR( impulse_ctor );
CK_DLL_DTOR( impulse_dtor );
CK_DLL_TICK( impulse_tick );
CK_DLL_CTRL( impulse_ctrl_next );
CK_DLL_CGET( impulse_cget_next );

// step
CK_DLL_CTOR( step_ctor );
CK_DLL_DTOR( step_dtor );
CK_DLL_TICK( step_tick );
CK_DLL_CTRL( step_ctrl_next );
CK_DLL_CGET( step_cget_next );

// gain
CK_DLL_CTOR( gain_ctor );
CK_DLL_DTOR( gain_dtor );
CK_DLL_TICK( gain_tick );
CK_DLL_CTRL( gain_ctrl_value );
CK_DLL_CGET( gain_cget_value );

// halfrect
CK_DLL_TICK( halfrect_tick );

// fullrect
CK_DLL_TICK( fullrect_tick );

// zerox
CK_DLL_CTOR( zerox_ctor );
CK_DLL_DTOR( zerox_dtor );
CK_DLL_TICK( zerox_tick );

// delayp
CK_DLL_CTOR( delayp_ctor );
CK_DLL_DTOR( delayp_dtor );
CK_DLL_PMSG( delayp_pmsg );
CK_DLL_TICK( delayp_tick );
CK_DLL_CTRL( delayp_ctrl_delay );
CK_DLL_CGET( delayp_cget_delay );
CK_DLL_CTRL( delayp_ctrl_window );
CK_DLL_CGET( delayp_cget_window );
CK_DLL_CTRL( delayp_ctrl_max );
CK_DLL_CGET( delayp_cget_max );

// sndbuf
CK_DLL_CTOR( sndbuf_ctor );
CK_DLL_DTOR( sndbuf_dtor );
CK_DLL_TICK( sndbuf_tick );
CK_DLL_CTRL( sndbuf_ctrl_read );
CK_DLL_CGET( sndbuf_cget_read );
CK_DLL_CTRL( sndbuf_ctrl_write );
CK_DLL_CGET( sndbuf_cget_write );
CK_DLL_CTRL( sndbuf_ctrl_pos );
CK_DLL_CGET( sndbuf_cget_pos );
CK_DLL_CTRL( sndbuf_ctrl_loop );
CK_DLL_CGET( sndbuf_cget_loop );
CK_DLL_CTRL( sndbuf_ctrl_interp );
CK_DLL_CGET( sndbuf_cget_interp );
CK_DLL_CTRL( sndbuf_ctrl_rate );
CK_DLL_CGET( sndbuf_cget_rate );
CK_DLL_CTRL( sndbuf_ctrl_play );
CK_DLL_CGET( sndbuf_cget_play );
CK_DLL_CTRL( sndbuf_ctrl_freq );
CK_DLL_CGET( sndbuf_cget_freq );
CK_DLL_CTRL( sndbuf_ctrl_phase );
CK_DLL_CGET( sndbuf_cget_phase );
CK_DLL_CTRL( sndbuf_ctrl_channel );
CK_DLL_CGET( sndbuf_cget_channel );
CK_DLL_CTRL( sndbuf_ctrl_chunks );
CK_DLL_CGET( sndbuf_cget_chunks );
CK_DLL_CTRL( sndbuf_ctrl_phase_offset );
CK_DLL_CGET( sndbuf_cget_samples );
CK_DLL_CGET( sndbuf_cget_length );
CK_DLL_CGET( sndbuf_cget_channels );
CK_DLL_CGET( sndbuf_cget_valueAt );




#endif
