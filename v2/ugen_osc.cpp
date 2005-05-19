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
#include "chuck_type.h"
#include <math.h>
#include <stdio.h>




static t_CKUINT g_srate = 0;
// for member data offset
static t_CKUINT osc_offset_data = 0;


//-----------------------------------------------------------------------------
// name: osc_query()
// desc: ...
//-----------------------------------------------------------------------------
DLL_QUERY osc_query( Chuck_DL_Query * QUERY )
{
    // srate
    g_srate = QUERY->srate;
    // get the env
    Chuck_Env * env = Chuck_Env::instance();

    Chuck_DL_Func * func = NULL;

    //---------------------------------------------------------------------
    // init as base class: osc
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "osc", "ugen", env->global(), 
                                        (t_CKUINT)osc_ctor, osc_tick, osc_pmsg ) )
        return FALSE;

    // add member variable
    osc_offset_data = type_engine_import_mvar( env, "int", "@osc_data", FALSE );
    if( osc_offset_data == CK_INVALID_OFFSET ) goto error;

    // add ctrl: freq
    func = make_new_mfun( "float", "freq", osc_ctrl_freq );
    func->add_arg( "float", "hz" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );


    //---------------------------------------------------------------------
    // sinosc
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "sinosc", "osc", env->global(), 
                                        NULL, sinosc_tick, NULL ) )
        return FALSE;

    // add ctrl: freq
    func = make_new_mfun( "float", "freq", osc_ctrl_freq );
    func->add_arg( "float", "hz" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );

    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
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
    double phase_offset;
    int    sync; 
    t_CKUINT srate;
    double width;
    
    double phase;
    
    Osc_Data()
    {
        t = 0.0;
        num = 0.0;
        freq = 220.0;
        sync = 0; //internal 
        width = 0.5;
        phase_offset = 0.0;
        srate = g_srate;
        
        phase = 0.0;
    }
};




//-----------------------------------------------------------------------------
// name: sinosc_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( osc_ctor )
{
    Osc_Data * d = new Osc_Data;
    Chuck_DL_Return r;
    // return data to be used later
    OBJ_MEMBER_UINT(SELF, osc_offset_data) = (t_CKUINT)d;
    osc_ctrl_freq( SELF, &(d->freq), &r );
    //osc_ctrl_phase_offset( obj, &phase_offset, NULL );
}




//-----------------------------------------------------------------------------
// name: osc_dtor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_DTOR( osc_dtor )
{
    // get the data
    Osc_Data * data = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // delete
    delete data;
    // set to NULL
    OBJ_MEMBER_UINT(SELF, osc_offset_data) = 0;
}




//-----------------------------------------------------------------------------
// name: osc_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( osc_tick )
{
    // get the data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // sync to now
    // if( d->sync == 1 ) d->phase = now * d->num;
    // compute
    *out = (SAMPLE) ( d->sync == 2 ) ? ::sin( TWO_PI * in ) : ::sin( d->phase );
    // move phase
    if( d->sync == 0 )
    {
        d->phase += d->num;
        // keep the phase between 0 and TWO_PI
        if( d->phase > TWO_PI ) d->phase -= TWO_PI;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: sinosc_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( sinosc_tick )
{
    // get the data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // sync to now
    // if( d->sync == 1 ) d->phase = now * d->num;
    // compute
    *out = (SAMPLE) ( d->sync == 2 ) ? ::sin( TWO_PI * in ) : ::sin( d->phase );
    // move phase
    if( d->sync == 0 )
    {
        d->phase += d->num;
        // keep the phase between 0 and TWO_PI
        if( d->phase > TWO_PI ) d->phase -= TWO_PI;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: osc_ctrl_freq()
// desc: set oscillator frequency
//-----------------------------------------------------------------------------
CK_DLL_CTRL( osc_ctrl_freq )
{
    // get data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // set freq
    d->freq = GET_CK_FLOAT(ARGS);
    // phase increment
    d->num = TWO_PI * d->freq / d->srate;
    // bound it
    if( d->num > TWO_PI ) d->num = ::fmod( d->num, TWO_PI );
    // return
    RETURN->v_float = (t_CKFLOAT)d->freq;
}




//-----------------------------------------------------------------------------
// name: osc_pmsg()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_PMSG( osc_pmsg )
{
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    if( !strcmp( MSG, "print" ) )
    {
        fprintf( stdout, "sinosc: (freq=%f)", d->freq );
        return TRUE;
    }
    
    // didn't handle
    return FALSE;
}
