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
    QUERY->ugen_ctrl( QUERY, impulse_ctrl_value, impulse_cget_value, "float", "next" );
    
    // add step
    QUERY->ugen_add( QUERY, "step", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, step_ctor, step_dtor, step_tick, NULL );
    // ctrl func
    QUERY->ugen_ctrl( QUERY, step_ctrl_value, step_cget_value, "float", "value" );
    QUERY->ugen_ctrl( QUERY, step_ctrl_value, step_cget_value, "float", "next" );

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
/*
    // add dac
    QUERY->ugen_add( QUERY, "dac", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, dac_tick, NULL );

    // add adc
    QUERY->ugen_add( QUERY, "adc", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, dac_tick, NULL );
    
    // add bunghole
    QUERY->ugen_add( QUERY, "bunghole", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, bunghole_tick, NULL );
    
    // add blackhole
    QUERY->ugen_add( QUERY, "blackhole", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, bunghole_tick, NULL );
*/
    // add sndbuf
    QUERY->ugen_add( QUERY, "sndbuf", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, sndbuf_ctor, sndbuf_dtor, sndbuf_tick, NULL );
    // set ctrl
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_read, NULL, "string", "read" );
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_write, NULL, "string", "write" );
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_pos, sndbuf_cget_pos, "int", "pos" );
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_interp, sndbuf_cget_interp, "int", "interp" );
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_rate, sndbuf_cget_rate, "float", "rate" );
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_freq, sndbuf_cget_freq, "float", "freq" );
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_phase, sndbuf_cget_phase, "float", "phase" );
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_channel, sndbuf_cget_channel, "int", "channel" );
    //set only
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_phase_offset, sndbuf_cget_phase, "float", "phase_offset" );
    //get only
    QUERY->ugen_ctrl( QUERY, NULL, sndbuf_cget_samples, "int", "samples" );
    QUERY->ugen_ctrl( QUERY, NULL, sndbuf_cget_length, "float", "length" );
    QUERY->ugen_ctrl( QUERY, NULL, sndbuf_cget_channels, "int", "channels" );

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
	return 0;
}

//-----------------------------------------------------------------------------
// name: bunghole_tick
// desc: ...
//-----------------------------------------------------------------------------
UGEN_TICK bunghole_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    *out = 0.0f;
    return 0;
}

enum { SNDBUF_DROP = 0, SNDBUF_INTERP, SNDBUF_SINC};
//-----------------------------------------------------------------------------
// name: sndbuf
// desc: ...
//-----------------------------------------------------------------------------
struct sndbuf_data
{
    float * buffer;
    t_CKUINT num_samples;
    t_CKUINT num_channels;
    t_CKUINT num_frames;
    t_CKUINT samplerate;
    t_CKUINT chan;
    float * eob;
    float * curr;
    double  curf;
    float   rate;
    int     interp;
    t_CKBOOL loop;
    
    sndbuf_data()
    {
        buffer = NULL;
	interp = SNDBUF_INTERP;
        num_channels = 0;
        num_frames = 0;
        num_samples = 0;
	samplerate = 0;
	curf = 0.0;
	rate = 1.0;
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

inline void sndbuf_setpos(sndbuf_data *d, double pos) { 

    if ( !d->buffer ) return;
  
    d->curf = pos;

    if ( d->loop ) { 
      while ( d->curf >= d->num_frames ) d->curf -= d->num_frames;
      while ( d->curf < 0 ) d->curf += d->num_frames;
    } 
    else { //invalid...
      if ( d->curf < 0 || d->curf >= d->num_frames ) d->curr = d->eob;
      return;
    }

    d->curr = d->buffer + d->chan + (long) d->curf * d->num_channels;

}

//PRC's sinc interpolation function.. as found 
// http://www.cs.princeton.edu/courses/archive/spring03/cs325/src/TimeStuf/srconvrt.c
//
// there's probably a lot in there that could be optimized, if we care to..
//

#define PI 3.14159265358979323846

//wow... we are sensitive.. 
#define WIDTH 16                /* this controls the number of neighboring samples
				   which are used to interpolate the new samples.  The
				   processing time is linearly related to this width */
#define DELAY_SIZE 140

#define USE_TABLE TRUE          /* this controls whether a linearly interpolated lookup
				   table is used for sinc function calculation, or the
				   sinc is calculated by floating point trig function calls.  */

#define USE_INTERP TRUE        /*  this controls whether the table is linear
				interpolated or not.  If you re not using the
				table, this has no effect         */

#define SAMPLES_PER_ZERO_CROSSING 32    /* this defines how finely the sinc function 
					   is sampled for storage in the table  */

float sinc_table[WIDTH * SAMPLES_PER_ZERO_CROSSING] = { 0.0 };

double sinc ( double x );

bool sinc_table_built = false;


void sndbuf_sinc_interpolate ( sndbuf_data *d, SAMPLE * out ) { 
	//punt!
	unsigned long left_limit, right_limit, j;
	double factor = d->rate;
	double time_now = d->curf;
	double one_over_factor;
	double int_time = 0;
	double last_time = 0;
	double temp1 = 0.0;

	float * samplebase = d->buffer + d->chan;
	int stride = d->num_channels;

	left_limit = (long)time_now - WIDTH + 1;      /* leftmost neighboring sample used for interp.*/
	right_limit = (long)time_now + WIDTH; /* rightmost leftmost neighboring sample used for interp.*/
	if (left_limit < 0) left_limit = 0;
	if (right_limit > d->num_frames) right_limit = d->num_frames;
	if (factor<1.0) {
		for (j=left_limit;j<right_limit;j++)
			temp1 += *(samplebase + j * stride ) * sinc(time_now - (double) j);
		*out = (SAMPLE) temp1;
	}
	
	else {
		one_over_factor = 1.0 / factor;
		for (j=left_limit;j<right_limit;j++)
			temp1 += *(samplebase + j * stride ) * one_over_factor * sinc(one_over_factor * (time_now - (double) j));
		*out = (SAMPLE) temp1;
	}
}

void make_sinc()
{
    int i;
    double temp,win_freq,win;
    win_freq = PI / WIDTH / SAMPLES_PER_ZERO_CROSSING;
    sinc_table[0] = 1.0;
    for (i=1;i<WIDTH * SAMPLES_PER_ZERO_CROSSING;i++)   {
		temp = (double) i * PI / SAMPLES_PER_ZERO_CROSSING;
		sinc_table[i] = (float)(sin(temp) / temp);
		win = 0.5 + 0.5 * cos(win_freq * i);
		sinc_table[i] *= (float)win;
    }
    sinc_table_built = true;
}

double linear_interp(double first_number,double second_number,double fraction)
{
    return (first_number + ((second_number - first_number) * fraction));
}

double t_sinc(double x)
{
    int low;
    double temp,delta;
    if ( !sinc_table_built ) make_sinc();
    if (fabs(x)>=WIDTH-1)
		return 0.0;
    else {
		temp = fabs(x) * (double) SAMPLES_PER_ZERO_CROSSING;
		low = (int)temp;          /* these are interpolation steps */
		if (USE_INTERP) {
			delta = temp - low;  /* and can be ommited if desired */
			return linear_interp(sinc_table[low],sinc_table[low + 1],delta);
		}
		else return sinc_table[low];
    }
}


double sinc(double x)
{
    double temp;
    if(USE_TABLE) return t_sinc(x);
    else        {
		if (x==0.0) return 1.0;
		else {
			temp = PI * x;
			return sin(temp) / (temp);
		}
	}
}



UGEN_TICK sndbuf_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    if( !d->buffer ) return FALSE;
    
    //we're ticking once per sample ( system )
    //curf in samples;
    
    if ( !d->loop && d->curr == d->eob ) return FALSE;

    //calculate frame

    if ( d->interp == SNDBUF_DROP ) { 
      *out = (SAMPLE)( (*(d->curr)) ) ;
    }
    else if ( d->interp == SNDBUF_INTERP ) { //samplewise linear interp
      double alpha = d->curf - floor(d->curf);
      *out = (SAMPLE)( (*(d->curr)) ) ;
      *out += (float)alpha * ( *(d->curr + d->num_channels) - *out );
    }
    else if ( d->interp == SNDBUF_SINC ) { 
      //do that fancy sinc function!
      sndbuf_sinc_interpolate(d, out);
    }

    //advance
    d->curf += d->rate;

    sndbuf_setpos(d, d->curf);
   
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
    d->chan = 0;
    d->num_frames = info.frames;
    d->num_channels = info.channels;
    d->num_samples = sf_read_float(file, d->buffer, size) ;
    d->samplerate = info.samplerate;
    d->interp     = SNDBUF_INTERP;
    if( d->num_samples != size )
    {
        fprintf( stderr, "[chuck](via sndbuf): read %d rather than %d frames from %s\n",
                 d->num_samples, size, filename );
        return;
    }
    //fprintf( stderr, "read file : %d frames %d channels, samplerate: %dHZ, duration %f\n", d->num_frames, d->num_channels, d->samplerate, (double) d->num_frames / (double) d->samplerate ) ; 
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


UGEN_CTRL sndbuf_ctrl_rate( t_CKTIME now, void * data, void * value ) { 
    sndbuf_data * d = ( sndbuf_data * ) data;
    t_CKFLOAT rate = * (t_CKFLOAT *) value;  //samples per tick..

    d->rate = (float)rate; 
}

UGEN_CGET sndbuf_cget_rate( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_FLOAT( out, d->rate );
}


UGEN_CTRL sndbuf_ctrl_freq( t_CKTIME now, void * data, void * value ) { 
    sndbuf_data * d = ( sndbuf_data * ) data;
    t_CKFLOAT freq = * (t_CKFLOAT *) value;  //hz

    d->rate = (float)(freq * (double) d->num_frames / (double) d->samplerate);
}

UGEN_CGET sndbuf_cget_freq( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_FLOAT( out, d->rate * (t_CKFLOAT) d->samplerate / (t_CKFLOAT) d->num_frames );
}


UGEN_CTRL sndbuf_ctrl_phase( t_CKTIME now, void * data, void * value ) { 
    sndbuf_data * d = ( sndbuf_data * ) data;
    t_CKFLOAT phase = * (t_CKFLOAT *) value;
    sndbuf_setpos(d, phase * (double)d->num_frames);
}

UGEN_CGET sndbuf_cget_phase( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_FLOAT( out, (t_CKFLOAT) d->curf / (t_CKFLOAT)d->num_frames );
}


UGEN_CTRL sndbuf_ctrl_channel( t_CKTIME now, void * data, void * value ) { 
    sndbuf_data * d = ( sndbuf_data * ) data;
    unsigned int chan = * (int *) value;
    if ( chan >= 0 && chan < d->num_channels ) { 
      d->chan = chan;
    }
}

UGEN_CGET sndbuf_cget_channel( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_INT( out, d->chan );
}



UGEN_CTRL sndbuf_ctrl_pos( t_CKTIME now, void * data, void * value ) { 
    sndbuf_data * d = ( sndbuf_data * ) data;
    int pos = * (int *) value;
    sndbuf_setpos(d, pos);
}

UGEN_CGET sndbuf_cget_pos( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_INT( out, d->curr - d->buffer );
}

UGEN_CTRL sndbuf_ctrl_interp( t_CKTIME now, void * data, void * value ) { 
    sndbuf_data * d = ( sndbuf_data * ) data;
    int interp = * (int *) value;
    d->interp = interp;
}

UGEN_CGET sndbuf_cget_interp( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_INT( out, d->interp );
}


UGEN_CTRL sndbuf_ctrl_phase_offset( t_CKTIME now, void * data, void * value ) { 
    sndbuf_data * d = (sndbuf_data *)data;
    t_CKFLOAT phase_offset = * (t_CKFLOAT *) value;
    sndbuf_setpos(d, d->curf + phase_offset * (t_CKFLOAT)d->num_frames );
}

UGEN_CGET sndbuf_cget_samples( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_INT( out, d->num_frames );
}

UGEN_CGET sndbuf_cget_length( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_FLOAT( out, (t_CKFLOAT)d->num_frames / (t_CKFLOAT)d->samplerate );
}

UGEN_CGET sndbuf_cget_channels( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_INT( out, d->num_channels );
}



