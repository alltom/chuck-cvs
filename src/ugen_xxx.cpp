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
// file: ugen_xxx.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#include "ugen_xxx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <fstream>
using namespace std;


//-----------------------------------------------------------------------------
// name: xxx_query()
// desc: ...
//-----------------------------------------------------------------------------
DLL_QUERY xxx_query( Chuck_DL_Query * QUERY )
{
    // add noise
    QUERY->ugen_add( QUERY, "noise", NULL );
    // set funcs,
    QUERY->ugen_func( QUERY, NULL, NULL, noise_tick, NULL );
    
    // add impulse
    QUERY->ugen_add( QUERY, "impulse", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, impulse_ctor, impulse_dtor, impulse_tick, NULL );
    // ctrl func
    QUERY->ugen_ctrl( QUERY, impulse_ctrl_value, impulse_cget_value, "float", "value" );
    
    // add step
    QUERY->ugen_add( QUERY, "step", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, step_ctor, step_dtor, step_tick, NULL );
    // ctrl func
    QUERY->ugen_ctrl( QUERY, step_ctrl_value, step_cget_value, "float", "value" );

    // add gain
    QUERY->ugen_add( QUERY, "gain", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, gain_ctor, gain_dtor, gain_tick, NULL );
    // ctrl func
    QUERY->ugen_ctrl( QUERY, gain_ctrl_value, gain_cget_value, "float", "value" );

    // add halfrect
    QUERY->ugen_add( QUERY, "halfrect", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, halfrect_tick, NULL );

    // add fullrect
    QUERY->ugen_add( QUERY, "fullrect", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, fullrect_tick, NULL );

    // add zerox
    QUERY->ugen_add( QUERY, "zerox", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, zerox_ctor, zerox_dtor, zerox_tick, NULL );

    // add dac
    QUERY->ugen_add( QUERY, "dac", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, dac_tick, NULL );

    // add adc
    QUERY->ugen_add( QUERY, "adc", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, dac_tick, NULL );

    // add sndbuf
    QUERY->ugen_add( QUERY, "sndbuf", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, sndbuf_ctor, sndbuf_dtor, sndbuf_tick, NULL );
    // set ctrl
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_read, NULL, "string", "read" );
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_write, NULL, "string", "write" );
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_pos, sndbuf_cget_pos, "int", "pos" );

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: noise_tick()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_TICK noise_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    *out = (SAMPLE)rand() / RAND_MAX;
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: struct Pulse_Data
// desc: ...
//-----------------------------------------------------------------------------
struct Pulse_Data
{
    SAMPLE value;
    t_CKUINT when;
    Pulse_Data( ) { value = 1.0f; when = 0; }
};


//-----------------------------------------------------------------------------
// name: impulse_ctor()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTOR impulse_ctor( t_CKTIME now )
{
    // return data to be used later
    return new Pulse_Data;
}




//-----------------------------------------------------------------------------
// name: impulse_dtor()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_DTOR impulse_dtor( t_CKTIME now, void * data )
{
    // delete
    delete (Pulse_Data *)data;
}


//-----------------------------------------------------------------------------
// name: impulse_tick()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_TICK impulse_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    Pulse_Data * d = (Pulse_Data *)data;
    if( d->when )
    {
        *out = d->value;
        d->value = 0;
    }
    else
        *out = 0.0f;

    return TRUE;
}



//-----------------------------------------------------------------------------
// name: impulse_ctrl_value()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTRL impulse_ctrl_value( t_CKTIME now, void * data, void * value )
{
    Pulse_Data * d = (Pulse_Data *)data;
    d->value = (SAMPLE)*(t_CKFLOAT *)value;
    d->when = 1;
}


//-----------------------------------------------------------------------------
// name: impulse_cget_value()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CGET impulse_cget_value( t_CKTIME now, void * data, void * out )
{
    Pulse_Data * d = (Pulse_Data *)data;
    SET_NEXT_FLOAT( out, d->value );
}



//-----------------------------------------------------------------------------
// name: step_ctor()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTOR step_ctor( t_CKTIME now )
{
    // return data to be used later
    return new SAMPLE( 1.0f );
}


//-----------------------------------------------------------------------------
// name: step_dtor()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_DTOR step_dtor( t_CKTIME now, void * data )
{
    // delete
    delete (SAMPLE *)data;
}


//-----------------------------------------------------------------------------
// name: step_tick()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_TICK step_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    SAMPLE * d = (SAMPLE *)data;
    *out = *d;
    
    return TRUE;
}


//-----------------------------------------------------------------------------
// name: step_ctrl_value()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTRL step_ctrl_value( t_CKTIME now, void * data, void * value )
{
    SAMPLE * d = (SAMPLE *)data;
    *d = (SAMPLE)*(t_CKFLOAT *)value;
}


//-----------------------------------------------------------------------------
// name: step_cget_value()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CGET step_cget_value( t_CKTIME now, void * data, void * out )
{
    SAMPLE * d = (SAMPLE *)data;
    SET_NEXT_FLOAT( out, (t_CKFLOAT)*d );
}


//-----------------------------------------------------------------------------
// name: gain_ctor()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTOR gain_ctor( t_CKTIME now )
{
    // return data to be used later
    return new SAMPLE( 1.0f );
}


//-----------------------------------------------------------------------------
// name: gain_dtor()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_DTOR gain_dtor( t_CKTIME now, void * data )
{
    // delete
    delete (SAMPLE *)data;
}


//-----------------------------------------------------------------------------
// name: gain_tick()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_TICK gain_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    SAMPLE * d = (SAMPLE *)data;
    *out = in * (*d);

    return TRUE;
}


//-----------------------------------------------------------------------------
// name: gain_ctrl_value()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTRL gain_ctrl_value( t_CKTIME now, void * data, void * value )
{
    SAMPLE * d = (SAMPLE *)data;
    *d = (SAMPLE)*(t_CKFLOAT *)value;
}


//-----------------------------------------------------------------------------
// name: gain_cget_value()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CGET gain_cget_value( t_CKTIME now, void * data, void * out )
{
    SAMPLE * d = (SAMPLE *)data;
    SET_NEXT_FLOAT( out, (t_CKFLOAT)*d );
}


//-----------------------------------------------------------------------------
// name: halfrect_tick()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_TICK halfrect_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    *out = in > 0.0f ? in : 0.0f;
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: fullrect_tick()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_TICK fullrect_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    *out = in >= 0.0f ? in : -in;
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: zerox_ctor()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTOR zerox_ctor( t_CKTIME now )
{
    return new SAMPLE( 0.0f );
}


//-----------------------------------------------------------------------------
// name: zerox_dtor()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_DTOR zerox_dtor( t_CKTIME now, void * data )
{
    delete (SAMPLE *)data;
}


#define __SGN(x)  (x >= 0.0f ? 1.0f : -1.0f )
//-----------------------------------------------------------------------------
// name: zerox_tick()
// desc: ...
//-----------------------------------------------------------------------------
UGEN_TICK zerox_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    SAMPLE * d = (SAMPLE *)data;
    *out = __SGN(in) != __SGN(*d);
    *out *= __SGN(in);
    *d = in;
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: dac_tick
// desc: ...
//-----------------------------------------------------------------------------
UGEN_TICK dac_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    // this is a placeholder - the real tick is the VM
}




//-----------------------------------------------------------------------------
// name: sndbuf
// desc: ...
//-----------------------------------------------------------------------------
struct sndbuf_data
{
    float * buffer;
    t_CKUINT num_samples;
    float * eob;
    float * curr;
    t_CKBOOL loop;
    
    sndbuf_data()
    {
        buffer = NULL;
        num_samples = 0;
        eob = NULL;
        curr = NULL;
        loop = TRUE;
    }
};

UGEN_CTOR sndbuf_ctor( t_CKTIME now )
{
    return new sndbuf_data;
}

UGEN_DTOR sndbuf_dtor( t_CKTIME now, void * data )
{
    sndbuf_data * d = (sndbuf_data *)data;
    if( d->buffer ) delete [] d->buffer;
    delete d;
}

UGEN_TICK sndbuf_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    if( !d->buffer ) return FALSE;
    
    if( d->curr == d->eob )
    {
        if( d->loop )
            d->curr = d->buffer;
        else
            return FALSE;
    }
    else
        *out = (SAMPLE)( (*(d->curr)++) ) ;
    
    return TRUE;
}

#include "util_sndfile.h"

UGEN_CTRL sndbuf_ctrl_read( t_CKTIME now, void * data, void * value )
{
    sndbuf_data * d = (sndbuf_data *)data;
    char * filename = *(char **)value;
	
    if( d->buffer )
    {
        delete [] d->buffer;
        d->buffer = NULL;
    }

    struct stat s;
    if( stat( filename, &s ) )
    {
        fprintf( stderr, "[chuck](via sndbuf): cannot stat file '%s'...\n", filename );
        return;
    }

    SF_INFO info;
    info.format = 0;
    SNDFILE* file = sf_open(filename, SFM_READ, &info);
    int er = sf_error(file);
    if(er) fprintf( stderr, "sndfile error %i\n", er );
    int size = info.channels * info.frames;
    d->buffer = new float[size];

    d->num_samples = sf_read_float(file, d->buffer, size) ;
    if( d->num_samples != size )
    {
        fprintf( stderr, "[chuck](via sndbuf): read %d rather than %d frames from %s\n",
                 d->num_samples, size, filename );
        return;
    }

    d->curr = d->buffer;
    d->eob = d->buffer + d->num_samples;
}


UGEN_CTRL sndbuf_ctrl_write( t_CKTIME now, void * data, void * value )
{
    sndbuf_data * d = (sndbuf_data *)data;
    char * filename = *(char **)value;

    if( d->buffer )
    {
        delete [] d->buffer;
        d->buffer = NULL;
    }

    struct stat s;
    if( stat( filename, &s ) )
    {
        fprintf( stderr, "[chuck](via sndbuf): cannot stat file '%s'...\n", filename );
        return;
    }


    d->curr = d->buffer;
    d->eob = d->buffer + d->num_samples;
}



UGEN_CTRL sndbuf_ctrl_pos( t_CKTIME now, void * data, void * value )
{
    sndbuf_data * d = (sndbuf_data *)data;
    int pos = *(int *)value;
    
    if( !d->buffer ) return;
    
    if( pos >= d->num_samples )
    {
        if( d->loop )
            d->curr = d->buffer;
        else
            d->curr = d->eob;
    }
    else
        d->curr = d->buffer + pos;
}


UGEN_CGET sndbuf_cget_pos( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_INT( out, d->curr - d->buffer );
}
