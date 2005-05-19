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
                                        osc_ctor, osc_tick, osc_pmsg ) )
        return FALSE;

    // add member variable
    osc_offset_data = type_engine_import_mvar( env, "int", "@osc_data", FALSE );
    if( osc_offset_data == CK_INVALID_OFFSET ) goto error;

    // add ctrl: freq
    func = make_new_mfun( "float", "freq", osc_ctrl_freq );
    func->add_arg( "float", "hz" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: sfreq ( == freq ) 
    func = make_new_mfun( "float", "sfreq", osc_ctrl_freq );
    func->add_arg( "float", "hz" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "float", "phase", osc_ctrl_phase );
    func->add_arg( "float", "phase" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );


    //---------------------------------------------------------------------
    // sinosc
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "sinosc", "osc", env->global(), 
                                        NULL, sinosc_tick, NULL ) )
        return FALSE;

    // end the class import
    type_engine_import_class_end( env );

    //---------------------------------------------------------------------
    // triosc - triangle oscillator
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "triosc", "osc", env->global(), 
                                        NULL, triosc_tick, NULL ) )
        return FALSE;

    func = make_new_mfun( "float", "width", osc_ctrl_width );
    func->add_arg( "float", "width" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // end the class import
    type_engine_import_class_end( env );


    //---------------------------------------------------------------------
    // sawosc - sawtooth oscillator  (  0 | 1  triangle wave  )
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "sawosc", "triosc", env->global(), 
                                        sawosc_ctor, NULL, NULL ) )
        return FALSE;

    func = make_new_mfun( "float", "width", sawosc_ctrl_width );
    func->add_arg( "float", "width" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // end the class import
    type_engine_import_class_end( env );


    //---------------------------------------------------------------------
    // pulseosc - pulse-width oscillator
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "pulseosc", "osc", env->global(), 
                                        NULL, pulseosc_tick, NULL ) )
        return FALSE;

    func = make_new_mfun( "float", "width", osc_ctrl_width );
    func->add_arg( "float", "width" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // end the class import
    type_engine_import_class_end( env );

    //---------------------------------------------------------------------
    // sqrosc - square_wave oscillator ( 0.5 pulse ) 
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "sqrosc", "pulseosc", env->global(), 
                                        sqrosc_ctor, NULL, NULL ) )
        return FALSE;

    func = make_new_mfun( "float", "width", sqrosc_ctrl_width );
    func->add_arg( "float", "width" );
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
    t_CKFLOAT num;
    t_CKFLOAT freq;
    int    sync; 
    t_CKUINT srate;
    t_CKFLOAT width;
    
    t_CKFLOAT phase;
    
    Osc_Data()
    {
        num = 0.0;
        freq = 220.0;
        sync = 0; //internal 
        width = 0.5;
        srate = g_srate;
        phase = 0.0;
    }
};




//-----------------------------------------------------------------------------
// name: osc_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( osc_ctor )
{
    Osc_Data * d = new Osc_Data;
    Chuck_DL_Return r;
    // return data to be used later
    OBJ_MEMBER_UINT(SELF, osc_offset_data) = (t_CKUINT)d;
    osc_ctrl_freq( SELF, &(d->freq), &r );
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
    // basic osx is a phasor... 
    // we use a duty-cycle rep ( 0 - 1 ) rather than angular ( 0 - TWOPI )
    // sinusoidal oscillators are special

    // get the data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // sync to now
    // if( d->sync == 1 ) d->phase = now * d->num;

    // as a rule, we store external phase control values
    // so that we can have a smooth change back to internal control -pld 
    if ( d->sync == 2 ) d->phase = in; 

    *out = (SAMPLE)  d->phase;

    // step the phase.
    // technically this should happen even with external phase control
    // that we'd be in the right place when translating back to internal... 
    // this was decidely inefficient and nit-picky.  -pld 

    if( d->sync == 0 ) 
    {
        d->phase += d->num;
        // keep the phase between 0 and 1
        if( d->phase > 1.0 ) d->phase -= 1.0;
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

    if ( d->sync == 2 ) d->phase = in;

    *out = (SAMPLE) ::sin( d->phase * TWO_PI );
    // move phase

    if( d->sync == 0 )
    {
        d->phase += d->num;
        // keep the phase between 0 and 1
        if( d->phase > 1.0 ) d->phase -= 1.0;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
// name: triosc_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( triosc_tick )
{
    // get the data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // sync to now
    // if( d->sync == 1 ) d->phase = now * d->num;
    // compute

    if ( d->sync == 2 ) d->phase = in;

    if ( d->phase < d->width ) *out = (SAMPLE) ( d->width == 0.0 ) ? 1.0 : -1.0 + 2.0 * d->phase / d->width; 
    else *out = (SAMPLE) ( d->width == 1.0 ) ? 0 : 1.0 - 2.0 * ( d->phase - d->width ) / ( 1.0 - d->width );

    // advance internal phase
    if( d->sync == 0 )
    {
        d->phase += d->num;
        // keep the phase between 0 and 1
        if( d->phase > 1.0 ) d->phase -= 1.0;
    }

    return TRUE;
}

//sawosc_tick is tri_osc tick with width=0.0 or width=1.0  -pld 


//-----------------------------------------------------------------------------
// name: pulseosc_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( pulseosc_tick )
{
    // get the data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // sync to now
    // if( d->sync == 1 ) d->phase = now * d->num;
    // compute
    
    if ( d->sync == 2 ) d->phase = in;  //don't bound-check the in value...

    *out = (SAMPLE) ( d->phase < d->width ) ? -1.0 : 1.0 ;

    // move phase
    if( d->sync == 0 )
    {
        d->phase += d->num;
        // keep the phase between 0 and 1
        if( d->phase > 1.0 ) d->phase -= 1.0;
    }

    return TRUE;
}

//sqrosc_tick is pulseosc_tick at width=0.5 -pld;

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
    d->num = d->freq / d->srate;
    // bound it
    if ( d->num >= 1.0 ) d->num -= floor( d->num );
    // return
    RETURN->v_float = (t_CKFLOAT)d->freq;
}

//-----------------------------------------------------------------------------
// name: osc_ctrl_phase()
// desc: set oscillator phase wrapped to ( 0 - 1 )
//-----------------------------------------------------------------------------
CK_DLL_CTRL( osc_ctrl_phase )
{
    // get data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // set freq
    d->phase = GET_CK_FLOAT(ARGS);
    //bound ( this could be set arbitrarily high or low ) 
    if ( d->phase >= 1.0 || d->phase < 0.0 ) d->phase -= floor( d->num );
    // return
    RETURN->v_float = (t_CKFLOAT)d->phase;
}

//-----------------------------------------------------------------------------
// name: osc_ctrl_width()
// desc: set width of active phase ( bound 0.0 - 1.0 );
//-----------------------------------------------------------------------------
CK_DLL_CTRL( osc_ctrl_width )
{
    // get data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // set freq
    d->width = GET_CK_FLOAT(ARGS);
    //bound ( this could be set arbitrarily high or low ) 
    d->width = max ( 0.0, min ( 1.0, d->width ) );
    // return
    RETURN->v_float = (t_CKFLOAT)d->width;
}


//-----------------------------------------------------------------------------
// name: sqrosc_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( sqrosc_ctor )
{
    Osc_Data * d = new Osc_Data;
    Chuck_DL_Return r;
    sqrosc_ctrl_width( SELF, &(d->width), &r );
}

//-----------------------------------------------------------------------------
// name: sqrosc_ctrl_width()
// desc: force width to 0.5;
//-----------------------------------------------------------------------------
CK_DLL_CTRL( sqrosc_ctrl_width )
{
    // get data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // force value
    d->width = 0.5; 
    // return
    RETURN->v_float = (t_CKFLOAT)d->width;
}


//-----------------------------------------------------------------------------
// name: sawosc_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( sawosc_ctor )
{
    Osc_Data * d = new Osc_Data;
    Chuck_DL_Return r;
    sawosc_ctrl_width( SELF, &(d->width), &r );
}


//-----------------------------------------------------------------------------
// name: sawosc_ctrl_width()
// force width to 0.0 ( falling ) or 1.0 ( rising ) 
//-----------------------------------------------------------------------------
CK_DLL_CTRL( sawosc_ctrl_width )
{
    // get data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // set freq
    d->width = GET_CK_FLOAT(ARGS);
    //bound ( this could be set arbitrarily high or low ) 
    d->width = ( d->width < 0.5 ) ? 0.0 : 1.0;  //rising or falling
    // return
    RETURN->v_float = (t_CKFLOAT)d->width;
}


//-----------------------------------------------------------------------------
// name: osc_ctrl_sync()
// desc: select sync mode for oscillator
//-----------------------------------------------------------------------------
CK_DLL_CTRL( osc_ctrl_sync )
{
    // get data
    Osc_Data * d = (Osc_Data *)OBJ_MEMBER_UINT(SELF, osc_offset_data );
    // set sync
    t_CKINT psync = d->sync;
    d->sync = GET_CK_INT(ARGS);
    //bound ( this could be set arbitrarily high or low ) 
    if ( d->sync < 0 || d->sync > 2 ) d->sync = 0;
    if ( d->sync == 0 && psync != d->sync ) { 
        //if we are switching to internal tick
        //we need to pre-advance the phase... 
        //this is probably stupid.  -pld
        d->phase += d->num;
        // keep the phase between 0 and 1
        if( d->phase > 1.0 ) d->phase -= 1.0;
    }
    // return
    RETURN->v_int = (t_CKINT)d->sync;
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
