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
// file: ugen_osc.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Philip L. Davidson (philipd@alumni.princeton.edu)
// date: Summer 2004
//-----------------------------------------------------------------------------
#include "ugen_osc.h"
#include "chuck_bbq.h"
#include <math.h>
#include <stdio.h>


//-----------------------------------------------------------------------------
// name: osc_query()
// desc: ...
//-----------------------------------------------------------------------------
DLL_QUERY osc_query( Chuck_DL_Query * QUERY )
{
    // add sinosc
    QUERY->ugen_add( QUERY, "sinosc", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, sinosc_ctor, sinosc_dtor, sinosc_tick, sinosc_pmsg );
    // add ctrl
    QUERY->ugen_ctrl( QUERY, sinosc_ctrl_freq, sinosc_cget_freq, "float", "freq" );
    QUERY->ugen_ctrl( QUERY, sinosc_ctrl_sfreq, NULL, "float", "sfreq" );
    QUERY->ugen_ctrl( QUERY, sinosc_ctrl_phase, sinosc_cget_phase, "float", "phase" );

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: struct Osc_Data
// desc: ...
//-----------------------------------------------------------------------------
struct Osc_Data
{
    double t;
    double num;
    double freq;
    double phase;
    t_CKUINT srate;
    
    Osc_Data( )
    {
        t = 0.0;
        num = 0.0;
        freq = 220.0;
        phase = 0.0;
        srate = Digitalio::sampling_rate();
        sinosc_ctrl_freq( 0, this, &freq );
        sinosc_ctrl_phase( 0, this, &phase );
    }
};


//-----------------------------------------------------------------------------
// name: sinosc_ctor()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTOR sinosc_ctor( t_CKTIME now )
{
    // return data to be used later
    return new Osc_Data;
}




//-----------------------------------------------------------------------------
// name: sinosc_dtor()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_DTOR sinosc_dtor( t_CKTIME now, void * data )
{
    // delete
    delete (Osc_Data *)data;
}




//-----------------------------------------------------------------------------
// name: sinosc_tick()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_TICK sinosc_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    Osc_Data * d = (Osc_Data *)data;
    //phase offsets don't mean so much when oscillators are keeping 
    //track of their own ticks, unless they are created at the same time..
    *out = (SAMPLE)sin( d->phase + d->t * d->num );
    d->t += 1.0;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: sinosc_ctrl_freq()
// desc: set oscillator frequency
//-----------------------------------------------------------------------------
UGEN_CTRL sinosc_ctrl_freq( t_CKTIME now, void * data, void * value )
{
    Osc_Data * d = (Osc_Data *)data;
    d->freq = GET_CK_FLOAT(value);
    d->num = 2.0 * 3.14159265358979323846 * d->freq / d->srate;
}


//-----------------------------------------------------------------------------
// name: sinosc_ctrl_sfreq()
// desc: sets oscillator frequency, implicitly adjusts phase offset to match
//       avoid signal discontinuity. - pld
//-----------------------------------------------------------------------------
UGEN_CTRL sinosc_ctrl_sfreq( t_CKTIME now, void * data, void * value )
{
    //phase matching freq adjust
    Osc_Data * d = (Osc_Data *)data;
    double curnum = d->num;
    d->freq = (float)GET_CK_FLOAT(value);
    d->num = 2.0 * 3.14159265358979323846 * d->freq / d->srate;
    double nphase = d->phase + d->t * ( curnum - d->num );
    d->phase = nphase - (2.0 * 3.14159265358979323846) * floor ( nphase / (2.0 * 3.14159265358979323846) );
}


//-----------------------------------------------------------------------------
// name: sinosc_ctrl_phase()
// desc: explicitly set oscillator phase-offset   -pld
//-----------------------------------------------------------------------------
UGEN_CTRL sinosc_ctrl_phase( t_CKTIME now, void * data, void * value )
{
    Osc_Data * d = (Osc_Data *)data;
    d->phase = (float)GET_CK_FLOAT(value);
}


//-----------------------------------------------------------------------------
// name: sinosc_cget_freq()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CGET sinosc_cget_freq( t_CKTIME now, void * data, void * out )
{
    Osc_Data * d = (Osc_Data *)data;
    SET_NEXT_FLOAT( out, (t_CKFLOAT)d->freq );
}


//-----------------------------------------------------------------------------
// name: sinosc_cget_phase()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CGET sinosc_cget_phase( t_CKTIME now, void * data, void * out )
{
    Osc_Data * d = (Osc_Data *)data;
    SET_NEXT_FLOAT( out, (t_CKFLOAT)d->phase );
}


//-----------------------------------------------------------------------------
// name: sinosc_pmsg()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_PMSG sinosc_pmsg( t_CKTIME now, void * data, const char * msg, void * value )
{
    Osc_Data * d = (Osc_Data *)data;
    if( !strcmp( msg, "print" ) )
    {
        fprintf( stdout, "sinosc: (freq=%f)", d->freq );
        return TRUE;
    }
    
    // didn't handle
    return FALSE;
}
