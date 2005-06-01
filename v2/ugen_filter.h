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
// file: ugen_filter.h
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Philip L. Davidson (philipd@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#ifndef __UGEN_FILTER_H__
#define __UGEN_FILTER_H__

#include "chuck_dl.h"


// query
DLL_QUERY filter_query( Chuck_DL_Query * query );

// filter
CK_DLL_CTOR( filter_ctor );
CK_DLL_DTOR( filter_dtor );
CK_DLL_TICK( filter_tick );
CK_DLL_CTRL( filter_ctrl_coefs );
CK_DLL_CGET( filter_cget_coefs );
CK_DLL_PMSG( filter_pmsg );

// biquad
CK_DLL_CTOR( biquad_ctor );
CK_DLL_DTOR( biquad_dtor );
CK_DLL_TICK( biquad_tick );

CK_DLL_CTRL( biquad_ctrl_pfreq );
CK_DLL_CGET( biquad_cget_pfreq );
CK_DLL_CTRL( biquad_ctrl_prad );
CK_DLL_CGET( biquad_cget_prad );
CK_DLL_CTRL( biquad_ctrl_zfreq );
CK_DLL_CGET( biquad_cget_zfreq );
CK_DLL_CTRL( biquad_ctrl_zrad );
CK_DLL_CGET( biquad_cget_zrad );
CK_DLL_CTRL( biquad_ctrl_norm );
CK_DLL_CGET( biquad_cget_norm );
CK_DLL_CTRL( biquad_ctrl_pregain );
CK_DLL_CGET( biquad_cget_pregain );
CK_DLL_CTRL( biquad_ctrl_eqzs );
CK_DLL_CGET( biquad_cget_eqzs );
CK_DLL_CTRL( biquad_ctrl_b0 );
CK_DLL_CGET( biquad_cget_b0 );
CK_DLL_CTRL( biquad_ctrl_b1 );
CK_DLL_CGET( biquad_cget_b1 );
CK_DLL_CTRL( biquad_ctrl_b2 );
CK_DLL_CGET( biquad_cget_b2 );
CK_DLL_CTRL( biquad_ctrl_a0 );
CK_DLL_CGET( biquad_cget_a0 );
CK_DLL_CTRL( biquad_ctrl_a1 );
CK_DLL_CGET( biquad_cget_a1 );
CK_DLL_CTRL( biquad_ctrl_a2 );
CK_DLL_CGET( biquad_cget_a2 );

// onepole
CK_DLL_CTOR( onepole_ctor );
CK_DLL_DTOR( onepole_dtor );
CK_DLL_TICK( onepole_tick );
CK_DLL_CTRL( onepole_ctrl_pole );

// onezero
CK_DLL_CTOR( onezero_ctor );
CK_DLL_DTOR( onezero_dtor );
CK_DLL_TICK( onezero_tick );
CK_DLL_CTRL( onezero_ctrl_zero );

// twopole
CK_DLL_CTOR( twopole_ctor );
CK_DLL_DTOR( twopole_dtor );
CK_DLL_TICK( twopole_tick );
CK_DLL_CTRL( twopole_ctrl_freq );
CK_DLL_CTRL( twopole_ctrl_rad );
CK_DLL_CTRL( twopole_ctrl_norm );

// twozero
CK_DLL_CTOR( twozero_ctor );
CK_DLL_DTOR( twozero_dtor );
CK_DLL_TICK( twozero_tick );
CK_DLL_CTRL( twozero_ctrl_freq );
CK_DLL_CTRL( twozero_ctrl_rad );

// gQ
CK_DLL_CTOR( gQ_ctor );
CK_DLL_DTOR( gQ_dtor );
CK_DLL_TICK( gQ_tick );
CK_DLL_CTRL( gQ_ctrl_freq );
CK_DLL_CTRL( gQ_ctrl_rad );

// allpass
CK_DLL_CTOR( allpass_ctor );
CK_DLL_DTOR( allpass_dtor );
CK_DLL_TICK( allpass_tick );
CK_DLL_PMSG( allpass_pmsg );

// delay
CK_DLL_CTOR( delay_ctor );
CK_DLL_DTOR( delay_dtor );
CK_DLL_TICK( delay_tick );
CK_DLL_CTRL( delay_ctrl_delay );
CK_DLL_CTRL( delay_ctrl_max );
CK_DLL_CTRL( delay_ctrl_tap );
CK_DLL_CTRL( delay_ctrl_energy );

// delayA
CK_DLL_CTOR( delayA_ctor );
CK_DLL_DTOR( delayA_dtor );
CK_DLL_TICK( delayA_tick );
CK_DLL_CTRL( delayA_ctrl_delay );
CK_DLL_CTRL( delayA_ctrl_max );

// delayL
CK_DLL_CTOR( delayL_ctor );
CK_DLL_DTOR( delayL_dtor );
CK_DLL_TICK( delayL_tick );
CK_DLL_CTRL( delayL_ctrl_delay );
CK_DLL_CTRL( delayL_ctrl_max );




#endif
