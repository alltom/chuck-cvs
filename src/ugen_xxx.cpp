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

t_CKUINT g_srate;


//-----------------------------------------------------------------------------
// name: xxx_query()
// desc: ...
//-----------------------------------------------------------------------------
DLL_QUERY xxx_query( Chuck_DL_Query * QUERY )
{
    g_srate = QUERY->srate;

    //! \section audio output

    // add dac
    //! digital/analog converter
    //! abstraction for underlying audio output device
    QUERY->ugen_add( QUERY, "dac", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, dac_tick, NULL );

    // add adc
    //! analog/digital converter
    //! abstraction for underlying audio input device
    QUERY->ugen_add( QUERY, "adc", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, dac_tick, NULL );
    
    // add blackhole
    //! sample rate sample sucker
    //! ( like dac, ticks ugens, but no more )
    //! see \example pwm.ck
    QUERY->ugen_add( QUERY, "blackhole", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, bunghole_tick, NULL );

    // add bunghole
    //! sample rate sample sucker
    //! ( like dac, ticks ugens, but no more )
    QUERY->ugen_add( QUERY, "bunghole", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, bunghole_tick, NULL );    

    // add gain
    //! gain control
    //! (NOTE - all unit generators can themselves change their gain)
    //! (this is a way to add N outputs together and scale them) 
    //! used in \example i-robot.ck
    QUERY->ugen_add( QUERY, "gain", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, gain_ctor, gain_dtor, gain_tick, NULL );
    // ctrl func
    QUERY->ugen_ctrl( QUERY, gain_ctrl_value, gain_cget_value, "float", "value" ); //! set gain ( all ugen's have this ) 
    /*! \example
      noise n => gain g => dac;
      sinosc s => g;
      .3 => g.gain;
      while( true ) { 100::ms => now; }
    */

    //! \section wave forms

    // add noise
    //! white noise generator 
    //! see \example noise.ck \example powerup.ck
    QUERY->ugen_add( QUERY, "noise", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, noise_tick, NULL );
    
    // add impulse
    //! pulse generator - can set the value of the current sample
    //! default for each sample is 0 if not set
    QUERY->ugen_add( QUERY, "impulse", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, impulse_ctor, impulse_dtor, impulse_tick, NULL );
    // ctrl func
    QUERY->ugen_ctrl( QUERY, impulse_ctrl_value, impulse_cget_value, "float", "value" );
    QUERY->ugen_ctrl( QUERY, impulse_ctrl_value, impulse_cget_value, "float", "next" ); //! set value of next sample
    /*! \example
      impulse i => dac;

      while( true ) {
         1.0 => i.next;
         100::ms => now;
      }
    */
    
    // add step
    //! step generator - like impulse, but once a value is set, 
    //! it is held for all following samples, until value is set again
    //! see \example step.ck
    QUERY->ugen_add( QUERY, "step", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, step_ctor, step_dtor, step_tick, NULL );
    // ctrl func
    QUERY->ugen_ctrl( QUERY, step_ctrl_value, step_cget_value, "float", "value" );
    QUERY->ugen_ctrl( QUERY, step_ctrl_value, step_cget_value, "float", "next" ); //! set the step value 
    /*! \example
      step s => dac;
      -1.0 => float amp;

      // square wave using step
      while( true ) {
          -amp => amp => s.next;
          800::samp => now;
      }
    */

    //! \section filters

    // add halfrect
    //! half wave rectifier
    //! for half-wave rectification. 
    QUERY->ugen_add( QUERY, "halfrect", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, halfrect_tick, NULL );

    // add fullrect
    //! full wave rectifier
    QUERY->ugen_add( QUERY, "fullrect", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, NULL, NULL, fullrect_tick, NULL );

    // add zerox
    //! zero crossing detector
    //! emits a single pulse at the the zero crossing in the direction of the zero crossing.  
    //! (see \example zerox.ck)
    QUERY->ugen_add( QUERY, "zerox", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, zerox_ctor, zerox_dtor, zerox_tick, NULL );

    //! \section delays

      //! varible write delay.  
    QUERY->ugen_add( QUERY, "delayph" , NULL);
    QUERY->ugen_func ( QUERY, delayph_ctor, delayph_dtor, delayph_tick, delayph_pmsg);
    QUERY->ugen_ctrl( QUERY, delayph_ctrl_delay, delayph_cget_delay , "dur", "delay" ); //! delay before subsequent values emerge
    QUERY->ugen_ctrl( QUERY, delayph_ctrl_window, delayph_cget_window , "dur", "window" ); //! time for 'write head' to move
    QUERY->ugen_ctrl( QUERY, delayph_ctrl_max, delayph_cget_max , "dur", "max" ); //! max delay possible.  trashes buffer, so do it first! 
 

    //! \section sound files

    // add sndbuf
    //! sound buffer ( now interpolating ) 
    //! reads from a variety of file formats
    //! see \example sndbuf.ck
    QUERY->ugen_add( QUERY, "sndbuf", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, sndbuf_ctor, sndbuf_dtor, sndbuf_tick, NULL );
    // set ctrl
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_read, NULL, "string", "read" ); //! loads file for reading
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_write, NULL, "string", "write" ); //! loads a file for writing ( or not ) 
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_pos, sndbuf_cget_pos, "int", "pos" ); //! set position ( 0 < p < .samples ) 
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_loop, sndbuf_cget_loop, "int", "loop" );//! toggle looping 
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_interp, sndbuf_cget_interp, "int", "interp" );//! set interpolation ( 0=drop, 1=linear, 2=sinc )
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_rate, sndbuf_cget_rate, "float", "rate" ); //! playback rate ( relative to file's natural speed ) 
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_rate, sndbuf_cget_rate, "float", "play" ); //! play (same as rate) 
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_freq, sndbuf_cget_freq, "float", "freq" ); //! playback rate ( file loops / second ) 
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_phase, sndbuf_cget_phase, "float", "phase" ); //! set phase position ( 0-1 )
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_channel, sndbuf_cget_channel, "int", "channel" ); //! select channel ( 0 < p < .channels )
    QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_phase_offset, sndbuf_cget_phase, "float", "phase_offset" ); //! set a phase offset
    QUERY->ugen_ctrl( QUERY, NULL, sndbuf_cget_samples, "int", "samples" ); //! fetch number of samples
    QUERY->ugen_ctrl( QUERY, NULL, sndbuf_cget_length, "float", "length" ); //! fetch length in seconds
    QUERY->ugen_ctrl( QUERY, NULL, sndbuf_cget_channels, "int", "channels" ); //! fetch number of channels


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


struct delayph_data 
{ 
   SAMPLE * buffer;
   int bufsize;
   
   double now;
   double readpos;
   
   double writepos; // relative to readpos
   
   double writeoff;
   
   double writeoff_start; 
   double writeoff_target;
   double writeoff_target_time; //target time
   double writeoff_window_time; //time we started shift
   
   SAMPLE sample_last;
   double writeoff_last;
   
   delayph_data() 
   { 
      bufsize  = 2 * g_srate;
      buffer   = ( SAMPLE * ) realloc ( NULL, sizeof ( SAMPLE ) * bufsize );
      for ( int i = 0 ; i < bufsize ; i++ ) buffer[i] = 0;
      readpos  = 0.0;
      writeoff  = 1000.0; 
      writeoff_last = 1000.0;
      writeoff_start = 1000.0;
      writeoff_target = 1000.0;
      sample_last = 0;
      writeoff_window_time = 1.0;
      writeoff_target_time = 0.0;
   }
};

UGEN_CTOR delayph_ctor( t_CKTIME now )
{

   return new delayph_data;
}

UGEN_DTOR delayph_dtor( t_CKTIME now, void * data )
{
    delayph_data * d = (delayph_data *)data;
    if( d->buffer ) delete [] d->buffer;
    delete d;
}
UGEN_PMSG delayph_pmsg( t_CKTIME now, void * data, const char * msg, void * value )
{
    return TRUE;
}
UGEN_TICK delayph_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out ) { 

   
   delayph_data * d = (delayph_data *)data;
   if ( !d->buffer ) return FALSE;
   
   //calculate new write-offset position
   if ( now >= d->writeoff_target_time || d->writeoff_window_time == 0 ) d->writeoff = d->writeoff_target;
   else  { 
      double dt = ( now - d->writeoff_target_time ) / ( d->writeoff_window_time ); 
      d->writeoff = d->writeoff_target + dt * ( d->writeoff_target - d->writeoff_start );
      //      fprintf (stderr, "dt %f, off %f , start %f target %f\n", dt, d->writeoff,  d->writeoff_start, d->writeoff_target );
   }

   //fprintf(stderr, "writeoff : %f %f %f %f %f\n", d->writeoff, d->writeoff_target, d->writeoff_start, now, d->writeoff_target_time );  
   //writeoff __must__ be positive!

   //find locations in buffer...
   double lastpos = d->writeoff_last + d->readpos - 1.0 ;
   double nowpos  = d->writeoff + d->readpos;

   
   //linear interpolation.  will introduce some lowpass/aliasing.

   double diff= nowpos - lastpos;
   double d_samp = in - d->sample_last;
   int i, smin, smax;
   SAMPLE sampi;
   if ( diff >= 0 ) { //forward.
      smin = ceil ( lastpos );
      smax = floor ( nowpos );
      for ( i = smin ; i <= smax ; i++ ) { 
         sampi = d->sample_last + d_samp * ( (double) i - lastpos ) / diff ;
	   //	   fprintf( stderr, "new sample %d %f %f %f \n", i,  in, sampi, d->sample_last );
         d->buffer[i%d->bufsize] += sampi;
      }
   }
   else { //moving in reverse
      smin = ceil ( nowpos );
      smax = floor ( lastpos );
      for ( i = smin ; i <= smax ; i++ ) 
         sampi = d->sample_last + d_samp * ( (double) i - lastpos ) / diff ;
         d->buffer[i%d->bufsize] += sampi ;   
   }

   d->writeoff_last = d->writeoff;
   d->sample_last = in;


   //output should go through a dc blocking filter, for cases where 
   //we are zipping around in the buffer leaving a fairly constant
   //trail of samples

   //output last sample
   int rpos = ( (int) d->readpos ) % d->bufsize ; 
   *out = d->buffer[rpos];
   d->buffer[rpos] = 0; //clear once it's been read
   d->readpos++;

   while ( d->readpos > d->bufsize ) d->readpos -= d->bufsize; 
   return TRUE;
   
}

UGEN_CTRL delayph_ctrl_delay( t_CKTIME now, void * data, void * value )
{
    delayph_data * d = ( delayph_data * ) data;
    t_CKDUR target = * (t_CKDUR *) value; // rate     
    if ( target != d->writeoff_target ) {
       if ( target > d->bufsize ) { 
          fprintf( stderr, "[chuck](via delayph): delay time %f over max!  set max first!\n", target);
          return;
       }
       d->writeoff_target = target;
       d->writeoff_start = d->writeoff_last;
       d->writeoff_target_time = now + d->writeoff_window_time; 
    }
}

UGEN_CGET delayph_cget_delay( t_CKTIME now, void * data, void * out )
{
    delayph_data * d = ( delayph_data * ) data;
    SET_NEXT_DUR( out, d->writeoff_last );
}

UGEN_CTRL delayph_ctrl_window( t_CKTIME now, void * data, void * value )
{
    delayph_data * d = ( delayph_data * ) data;
    t_CKDUR window = * (t_CKDUR *) value; // rate     
    if ( window >= 0 ) {
       d->writeoff_window_time = window;
       //fprintf ( stderr, "set window time %f , %f , %d \n", d->writeoff_window_time, d->writeoff, d->bufsize );
    }
}

UGEN_CGET delayph_cget_window( t_CKTIME now, void * data, void * out )
{
    delayph_data * d = ( delayph_data * ) data;
    SET_NEXT_DUR( out, d->writeoff_last );
}


UGEN_CTRL delayph_ctrl_max( t_CKTIME now, void * data, void * value )
{   
    delayph_data * d = ( delayph_data * ) data;
    t_CKDUR nmax = * (t_CKDUR *) value; // rate 
    if ( d->bufsize != (int)nmax && nmax > 1.0 ) { 
      d->bufsize = nmax; 
      d->buffer = ( SAMPLE * ) realloc ( d->buffer, sizeof ( SAMPLE ) * d->bufsize );
      for ( int i = 0; i < d->bufsize; i++ ) d->buffer[i] = 0;
    }


}
UGEN_CGET delayph_cget_max( t_CKTIME now, void * data, void * out )
{
    delayph_data * d = ( delayph_data * ) data;
    SET_NEXT_DUR( out, (t_CKDUR) d->bufsize );
}

//-----------------------------------------------------------------------------
// name: sndbuf
// desc: ...
//-----------------------------------------------------------------------------
enum { SNDBUF_DROP = 0, SNDBUF_INTERP, SNDBUF_SINC};


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

struct sndbuf_data
{
    SAMPLE * buffer;
    t_CKUINT num_samples;
    t_CKUINT num_channels;
    t_CKUINT num_frames;
    t_CKUINT samplerate;
    t_CKUINT chan;
    double sampleratio;
    float * eob;
    float * curr;
    double  curf;
    float   rate;
    int     interp;
    t_CKBOOL loop;
    
    bool    sinc_table_built;
    bool    sinc_use_table;
    bool    sinc_use_interp;
    int     sinc_samples_per_zero_crossing ;
    int     sinc_width ;
    double * sinc_table ; 
    sndbuf_data()
    {
        buffer = NULL;
        interp = SNDBUF_INTERP;
        num_channels = 0;
        num_frames = 0;
        num_samples = 0;

        samplerate = 0;

        sampleratio = 1.0;
        chan = 0;
        curf = 0.0;
        rate = 0.0;

        eob = NULL;
        curr = NULL;

        sinc_table_built = false;
	    sinc_use_table = USE_TABLE;
	    sinc_use_interp = USE_INTERP;
	    sinc_width = WIDTH;
	    sinc_samples_per_zero_crossing = SAMPLES_PER_ZERO_CROSSING;
	    sinc_table = NULL;

        loop = FALSE;
    }
};

double  sndbuf_sinc  ( sndbuf_data * d, double x );
double  sndbuf_t_sinc( sndbuf_data * d, double x );
void    sndbuf_make_sinc( sndbuf_data * d );
void    sndbuf_sinc_interpolate ( sndbuf_data * d, SAMPLE * out);


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

inline void sndbuf_setpos(sndbuf_data *d, double pos)
{
    if( !d->buffer ) return;
  
    d->curf = pos;

    //set curf within bounds

    if( d->loop )
    {
        while ( d->curf >= d->num_frames ) d->curf -= d->num_frames;
        while ( d->curf < 0 ) d->curf += d->num_frames;
    } 
    else
    {
        if( d->curf < 0 ) d->curf = 0;
        else if( d->curf >= d->num_frames ) d->curf = d->num_frames-1;
    }
    //sets curr to correct position ( account for channels ) 
    d->curr = d->buffer + d->chan + (long) d->curf * d->num_channels;
}

inline SAMPLE sndbuf_sampleAt( sndbuf_data * d, int pos ) { 
  //boundary cases

  int nf = d->num_frames;
  if ( d->loop ) { 
    while ( pos >= nf ) pos -= nf;
    while ( pos <  0  ) pos += nf;
  }
  else { 
    if ( pos >= nf ) pos = nf-1;
    if ( pos < 0   ) pos = 0;
  }

  return d->buffer[d->chan + (long) d->curf * d->num_channels];

}

inline double sndbuf_getpos(sndbuf_data *d)
{
    if( !d->buffer ) return 0;
    return floor(d->curf);
}


UGEN_CTRL sndbuf_ctrl_loop( t_CKTIME now, void * data, void * value )
{
    sndbuf_data * d = (sndbuf_data *)data;
    d->loop = *(int *)value;
}

UGEN_CGET sndbuf_cget_loop( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_INT( out, d->loop );
}

// PRC's sinc interpolation function.. as found 
// http://www.cs.princeton.edu/courses/archive/spring03/cs325/src/TimeStuf/srconvrt.c
//
// there's probably a lot in there that could be optimized, if we care to..
//

#define PI 3.14159265358979323846

//wow... we are sensitive.. 


inline double sndbuf_linear_interp (double * first, double * second, double * frac);
bool sinc_table_built = false;

void sndbuf_sinc_interpolate ( sndbuf_data *d, SAMPLE * out )
{
    signed long j;
	double factor = d->rate;
	double time_now = d->curf;
	double one_over_factor;
	double int_time = 0;
	double last_time = 0;
	double temp1 = 0.0;

	long time_i = (long)time_now;

	//bounds checking now in sampleAt function...
	if (factor<1.0) {
		for (j= -d->sinc_width + 1 ; j < d->sinc_width; j++)
		  {

			temp1 += sndbuf_sampleAt(d,time_i+j) * sndbuf_sinc(d,(double)j );
		  }
		*out = (SAMPLE) temp1;
	}
	
	else {

	    one_over_factor = 1.0 / factor;
		for (j= -d->sinc_width + 1; j< d->sinc_width; j++) {


				    temp1 += sndbuf_sampleAt(d,time_i+j) * one_over_factor * sndbuf_sinc(d,one_over_factor * (double) j );
		} 
		*out = (SAMPLE) temp1;
	}
}

void sndbuf_make_sinc( sndbuf_data * d)
{
    int i;
    fprintf(stderr, "building sinc table\n" );
    double temp,win_freq,win;
    win_freq = PI / d->sinc_width / d->sinc_samples_per_zero_crossing;
    int tabsize = d->sinc_width * d->sinc_samples_per_zero_crossing;

    d->sinc_table = (double *) realloc ( d->sinc_table, tabsize * sizeof(double) );
    d->sinc_table[0] = 1.0;
    for (i=1;i< tabsize ;i++)   {
		temp = (double) i * PI / d->sinc_samples_per_zero_crossing;
		d->sinc_table[i] = (float)(sin(temp) / temp);
		win = 0.5 + 0.5 * cos(win_freq * i);
		d->sinc_table[i] *= (float)win;
    }
    d->sinc_table_built = true;
}

inline double sndbuf_linear_interp(double first_number,double second_number,double fraction)
{
    return (first_number + ((second_number - first_number) * fraction));
}

double sndbuf_t_sinc(sndbuf_data *d, double x)
{
    int low;
    double temp,delta;
    if ( !d->sinc_table_built ) sndbuf_make_sinc(d);
    if (fabs(x)>= d->sinc_width-1)
		return 0.0;
    else {
		temp = fabs(x) * (double) d->sinc_samples_per_zero_crossing;
		low = (int)temp;          /* these are interpolation steps */
		if (d->sinc_use_interp) {
			delta = temp - low;  /* and can be ommited if desired */
			return sndbuf_linear_interp(d->sinc_table[low],d->sinc_table[low + 1],delta);
		}
		else return d->sinc_table[low];
    }
}


double sndbuf_sinc(sndbuf_data * d, double x)
{
    double temp;

    if(d->sinc_use_table) return sndbuf_t_sinc(d,x);
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
    
    if ( !d->loop && d->curr >= d->eob ) return FALSE;

    //calculate frame

    if( d->interp == SNDBUF_DROP )
    { 
        *out = (SAMPLE)( (*(d->curr)) ) ;
    }
    else if( d->interp == SNDBUF_INTERP )
    { //samplewise linear interp
        double alpha = d->curf - floor(d->curf);
        *out = (SAMPLE)( (*(d->curr)) ) ;
        *out += (float)alpha * ( sndbuf_sampleAt(d, (long)d->curf+1 ) - *out );
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
    char * format = strrchr ( filename, '.');
    if ( format && strcmp ( format, ".raw" ) == 0 ) { 
      fprintf(stderr, "%s :: type is '.raw'.  assuming 16 bit signed mono (PCM)\n", filename);
      info.format = SF_FORMAT_RAW | SF_FORMAT_PCM_16 | SF_ENDIAN_CPU ;
      info.channels = 1;
      info.samplerate = 44100;
    }
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

    d->sampleratio = (double)d->samplerate / (double)g_srate;
    d->interp     = SNDBUF_INTERP;

    if( d->num_samples != size )
    {
        fprintf( stderr, "[chuck](via sndbuf): read %d rather than %d frames from %s\n",
                 d->num_samples, size, filename );
        return;
    }
    //    fprintf(stderr, "read file : %d frames  %d chan %d rate\n", d->num_frames, d->num_channels, d->samplerate );
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


UGEN_CTRL sndbuf_ctrl_rate( t_CKTIME now, void * data, void * value )
{
    sndbuf_data * d = ( sndbuf_data * ) data;
    t_CKFLOAT rate = * (t_CKFLOAT *) value; // rate     
    d->rate = rate * d->sampleratio; 
}

UGEN_CGET sndbuf_cget_rate( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_FLOAT( out, d->rate / d->sampleratio );
}


UGEN_CTRL sndbuf_ctrl_freq( t_CKTIME now, void * data, void * value ) 
{ 
    sndbuf_data * d = ( sndbuf_data * ) data;
    t_CKFLOAT freq = * (t_CKFLOAT *) value;  //hz
 
    d->rate = ( freq * (double) d->num_frames / (double) g_srate );
}

UGEN_CGET sndbuf_cget_freq( t_CKTIME now, void * data, void * out )
{
    sndbuf_data * d = (sndbuf_data *)data;
    SET_NEXT_FLOAT( out, d->rate * (t_CKFLOAT) g_srate / ( (t_CKFLOAT) d->num_frames ) );
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
    SET_NEXT_INT( out, (int) sndbuf_getpos(d) );
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



