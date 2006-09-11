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
//         Ananya Misra (amisra@cs.princeton.edu)
// date: Spring 2004
//       Summer 2005 - updated
//-----------------------------------------------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>

#if defined(__CK_SNDFILE_NATIVE__)
#include <sndfile.h>
#else
#include "util_sndfile.h"
#endif

#include "ugen_xxx.h"
#include "chuck_type.h"
#include "chuck_ugen.h"
#include "chuck_vm.h"

#include <fstream>
using namespace std;

t_CKUINT g_srate;

// offset
static t_CKUINT stereo_offset_left = 0;
static t_CKUINT stereo_offset_right = 0;
static t_CKUINT stereo_offset_pan = 0;
static t_CKUINT cnoise_offset_data = 0;
static t_CKUINT impulse_offset_data = 0;
static t_CKUINT step_offset_data = 0;
static t_CKUINT zerox_offset_data = 0;
static t_CKUINT delayp_offset_data = 0;
static t_CKUINT sndbuf_offset_data = 0;

Chuck_Type * g_t_dac = NULL;
Chuck_Type * g_t_adc = NULL;


//-----------------------------------------------------------------------------
// name: xxx_query()
// desc: ...
//-----------------------------------------------------------------------------
DLL_QUERY xxx_query( Chuck_DL_Query * QUERY )
{
    g_srate = QUERY->srate;
    // get the env
    Chuck_Env * env = Chuck_Env::instance();

    Chuck_DL_Func * func = NULL;

    // deprecate
    type_engine_register_deprecate( env, "dac", "DAC" );
    type_engine_register_deprecate( env, "adc", "ADC" );
    type_engine_register_deprecate( env, "pan2", "Pan2" );
    type_engine_register_deprecate( env, "mix2", "Mix2" );
    type_engine_register_deprecate( env, "gain", "Gain" );
    type_engine_register_deprecate( env, "noise", "Noise" );
    type_engine_register_deprecate( env, "cnoise", "CNoise" );
    type_engine_register_deprecate( env, "impulse", "Impulse" );
    type_engine_register_deprecate( env, "step", "Step" );
    type_engine_register_deprecate( env, "halfrect", "HalfRect" );
    type_engine_register_deprecate( env, "fullrect", "FullRect" );
    type_engine_register_deprecate( env, "zerox", "ZeroX" );
    type_engine_register_deprecate( env, "delayp", "DelayP" );
    type_engine_register_deprecate( env, "sndbuf", "SndBuf" );

    //! \section audio output
    
    //-------------------------------------------------------------------------
    // init as base class: UGen_Multi
    //-------------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "UGen_Multi", "UGen", env->global(),
                                        multi_ctor, NULL, NULL, 0, 0 ) )
        return FALSE;

    // add chan
    func = make_new_mfun( "UGen", "chan", multi_cget_chan );
    func->add_arg( "int", "which" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add pan
    /* func = make_new_mfun( "float", "pan", multi_ctrl_pan );
    func->add_arg( "float", "val" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    func = make_new_mfun( "float", "pan", multi_cget_pan );
    if( !type_engine_import_mfun( env, func ) ) goto error; */

    
    //---------------------------------------------------------------------
    // init as base class: UGen_Stereo
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "UGen_Stereo", "UGen_Multi", env->global(), 
                                        stereo_ctor, NULL, NULL, 2, 2 ) )
        return FALSE;

    // add left
    stereo_offset_left = type_engine_import_mvar( env, "UGen", "left", FALSE );
    if( stereo_offset_left == CK_INVALID_OFFSET ) goto error;
    
    // add right
    stereo_offset_right = type_engine_import_mvar( env, "UGen", "right", FALSE );
    if( stereo_offset_right == CK_INVALID_OFFSET ) goto error;

    // add pan
    stereo_offset_pan = type_engine_import_mvar( env, "float", "@pan", FALSE );
    if( stereo_offset_pan == CK_INVALID_OFFSET ) goto error;
    
    // add pan
    func = make_new_mfun( "float", "pan", stereo_ctrl_pan );
    func->add_arg( "float", "val" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    func = make_new_mfun( "float", "pan", stereo_cget_pan );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;

    // add dac
    //! digital/analog converter
    //! abstraction for underlying audio output device
    //---------------------------------------------------------------------
    // init as base class: dac
    //---------------------------------------------------------------------
    if( !(g_t_dac = type_engine_import_ugen_begin( env, "DAC", "UGen_Stereo", env->global(), 
                                                   NULL, NULL, NULL, 2, 2 )) )
        return FALSE;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;
    
    // add adc
    //! analog/digital converter
    //! abstraction for underlying audio input device
    //---------------------------------------------------------------------
    // init as base class: adc
    //---------------------------------------------------------------------
    if( !(g_t_adc = type_engine_import_ugen_begin( env, "ADC", "UGen_Stereo", env->global(), 
                                                   NULL, NULL, NULL, 0, 2 )) )
        return FALSE;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;

    //---------------------------------------------------------------------
    // init as base class: pan2
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "Pan2", "UGen_Stereo", env->global(), 
                                        NULL, NULL, NULL, 2, 2 ) )
        return FALSE;
    
    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;

    //---------------------------------------------------------------------
    // init as base class: mix2
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "Mix2", "UGen_Stereo", env->global(), 
                                        NULL, NULL, NULL, 2, 2 ) )
        return FALSE;
    
    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;
    
    // add blackhole
    //! sample rate sample sucker
    //! ( like dac, ticks ugens, but no more )
    //! see \example pwm.ck
    //QUERY->ugen_add( QUERY, "blackhole", NULL );
    // set funcs
    //QUERY->ugen_func( QUERY, NULL, NULL, bunghole_tick, NULL );
    
    // add bunghole
    //! sample rate sample sucker
    //! ( like dac, ticks ugens, but no more )
    //QUERY->ugen_add( QUERY, "bunghole", NULL );
    // set funcs
    //QUERY->ugen_func( QUERY, NULL, NULL, bunghole_tick, NULL );
    
    // add gain
    //! gain control
    //! (NOTE - all unit generators can themselves change their gain)
    //! (this is a way to add N outputs together and scale them) 
    //! used in \example i-robot.ck
    //---------------------------------------------------------------------
    // init as base class: gain
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "Gain", "UGen", env->global(), 
                                        NULL, NULL, NULL ) )
        return FALSE;
    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;
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
    //---------------------------------------------------------------------
    // init as base class: noise
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "Noise", "UGen", env->global(), 
                                        NULL, noise_tick, NULL ) )
        return FALSE;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;
//zzz

    // add cnoise 
    /*QUERY->ugen_add( QUERY, "cnoise", NULL );
    QUERY->ugen_func( QUERY, cnoise_ctor, cnoise_dtor, cnoise_tick, NULL );
    QUERY->ugen_ctrl( QUERY, cnoise_ctrl_mode, NULL, "string", "mode" );
    QUERY->ugen_ctrl( QUERY, cnoise_ctrl_fprob, NULL, "float", "fprob" );*/

    //---------------------------------------------------------------------
    // init as base class: cnoise
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "CNoise", "UGen", env->global(), 
                                        cnoise_ctor, cnoise_tick, NULL ) )
        return FALSE;

    // add member variable
    cnoise_offset_data = type_engine_import_mvar( env, "int", "@cnoise_data", FALSE );
    if( cnoise_offset_data == CK_INVALID_OFFSET ) goto error;
    
    // add ctrl: mode
    func = make_new_mfun( "string", "mode", cnoise_ctrl_mode );
    func->add_arg( "string", "mode" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: mode
    //func = make_new_mfun( "string", "mode", cnoise_cget_mode );
    //if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: fprob
    func = make_new_mfun( "float", "fprob", cnoise_ctrl_fprob );
    func->add_arg( "float", "fprob" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: fprob
    //func = make_new_mfun( "float", "fprob", cnoise_cget_fprob );
    //if( !type_engine_import_mfun( env, func ) ) goto error;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;


    // add impulse
    //! pulse generator - can set the value of the current sample
    //! default for each sample is 0 if not set
        //QUERY->ugen_add( QUERY, "impulse", NULL );
    // set funcs
        //QUERY->ugen_func( QUERY, impulse_ctor, impulse_dtor, impulse_tick, NULL );
    // ctrl func
        //QUERY->ugen_ctrl( QUERY, impulse_ctrl_value, impulse_cget_value, "float", "value" );
        //QUERY->ugen_ctrl( QUERY, impulse_ctrl_value, impulse_cget_value, "float", "next" ); //! set value of next sample
    /*! \example
        impulse i => dac;
    
    while( true ) {
        1.0 => i.next;
        100::ms => now;
    }
    */
    //---------------------------------------------------------------------
    // init as base class: impulse
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "Impulse", "UGen", env->global(), 
                                        impulse_ctor, impulse_tick, NULL ) )
        return FALSE;

    // add ctrl: value
    //func = make_new_mfun( "float", "value", impulse_ctrl_value );
    //func->add_arg( "float", "value" );
    //if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: value
    //func = make_new_mfun( "float", "value", impulse_cget_value );
    //if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    impulse_offset_data = type_engine_import_mvar( env, "int", "@impulse_data", FALSE );
    if( impulse_offset_data == CK_INVALID_OFFSET ) goto error;

    // add ctrl: next
    func = make_new_mfun( "float", "next", impulse_ctrl_next );
    func->add_arg( "float", "next" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: next
    func = make_new_mfun( "float", "next", impulse_cget_next );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;
    
    // add step
    //! step generator - like impulse, but once a value is set, 
    //! it is held for all following samples, until value is set again
    //! see \example step.ck
        //QUERY->ugen_add( QUERY, "step", NULL );
    // set funcs
        //QUERY->ugen_func( QUERY, step_ctor, step_dtor, step_tick, NULL );
    // ctrl func
        //QUERY->ugen_ctrl( QUERY, step_ctrl_value, step_cget_value, "float", "value" );
        //QUERY->ugen_ctrl( QUERY, step_ctrl_value, step_cget_value, "float", "next" ); //! set the step value 
    /*! \example
        step s => dac;
    -1.0 => float amp;
    
    // square wave using step
    while( true ) {
        -amp => amp => s.next;
        800::samp => now;
    }
    */
    //---------------------------------------------------------------------
    // init as base class: step
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "Step", "UGen", env->global(), 
                                        step_ctor, step_tick, NULL ) )
        return FALSE;

    // add ctrl: value
    //func = make_new_mfun( "float", "value", step_ctrl_value );
    //func->add_arg( "float", "value" );
    //if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: value
    //func = make_new_mfun( "float", "value", step_cget_value );
    //if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    step_offset_data = type_engine_import_mvar( env, "int", "@step_data", FALSE );
    if( step_offset_data == CK_INVALID_OFFSET ) goto error;

    // add ctrl: next
    func = make_new_mfun( "float", "next", step_ctrl_next );
    func->add_arg( "float", "next" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: next
    func = make_new_mfun( "float", "next", step_cget_next );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;
    
    // add halfrect
    //! half wave rectifier
    //! for half-wave rectification. 
        //QUERY->ugen_add( QUERY, "halfrect", NULL );
    // set funcs
        //QUERY->ugen_func( QUERY, NULL, NULL, halfrect_tick, NULL );
    //---------------------------------------------------------------------
    // init as base class: halfrect
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "HalfRect", "UGen", env->global(), 
                                        NULL, halfrect_tick, NULL ) )
        return FALSE;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;


    // add fullrect
    //! full wave rectifier
        //QUERY->ugen_add( QUERY, "fullrect", NULL );
    // set funcs
        //QUERY->ugen_func( QUERY, NULL, NULL, fullrect_tick, NULL );
    //---------------------------------------------------------------------
    // init as base class: fullrect
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "FullRect", "UGen", env->global(), 
                                        NULL, fullrect_tick, NULL ) )
        return FALSE;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;


    // add zerox
    //! zero crossing detector
    //! emits a single pulse at the the zero crossing in the direction of the zero crossing.  
    //! (see \example zerox.ck)
    //QUERY->ugen_add( QUERY, "zerox", NULL );
    // set funcs
    //QUERY->ugen_func( QUERY, zerox_ctor, zerox_dtor, zerox_tick, NULL );
    //---------------------------------------------------------------------
    // init as base class: zerox
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "ZeroX", "UGen", env->global(), 
                                        zerox_ctor, zerox_tick, NULL ) )
        return FALSE;

    // add member variable
    zerox_offset_data = type_engine_import_mvar( env, "int", "@zerox_data", FALSE );
    if( zerox_offset_data == CK_INVALID_OFFSET ) goto error;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;


    //! \section delay lines
    
    //! delay with varying write position ( instead of read position ) 
    //! change to delay length will not affect the delay of samples already in
    //! the buffer.
    //! see \example delayp.ck

    //QUERY->ugen_add( QUERY, "delayp" , NULL);
    //QUERY->ugen_func ( QUERY, delayp_ctor, delayp_dtor, delayp_tick, delayp_pmsg);
    //QUERY->ugen_ctrl( QUERY, delayp_ctrl_delay, delayp_cget_delay , "dur", "delay" ); //! delay before subsequent values emerge
    //QUERY->ugen_ctrl( QUERY, delayp_ctrl_window, delayp_cget_window , "dur", "window" ); //! time for 'write head' to move
    //QUERY->ugen_ctrl( QUERY, delayp_ctrl_max, delayp_cget_max , "dur", "max" ); //! max delay possible.  trashes buffer, so do it first! 
    
    //---------------------------------------------------------------------
    // init as base class: delayp
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "DelayP", "UGen", env->global(), 
                                        delayp_ctor, delayp_tick, delayp_pmsg ) )
        return FALSE;

    // add member variable
    delayp_offset_data = type_engine_import_mvar( env, "int", "@delayp_data", FALSE );
    if( delayp_offset_data == CK_INVALID_OFFSET ) goto error;

    // add ctrl: delay
    func = make_new_mfun( "dur", "delay", delayp_ctrl_delay );
    func->add_arg( "dur", "delay" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: delay
    func = make_new_mfun( "dur", "delay", delayp_cget_delay );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: window
    func = make_new_mfun( "dur", "window", delayp_ctrl_window );
    func->add_arg( "dur", "window" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: window
    func = make_new_mfun( "dur", "window", delayp_cget_window );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: max
    func = make_new_mfun( "dur", "max", delayp_ctrl_max );
    func->add_arg( "dur", "max" );
    if( !type_engine_import_mfun( env, func ) ) goto error; 
    // add cget: max
    func = make_new_mfun( "dur", "max", delayp_cget_max );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;
    
    //! \section sound files
    
    // add sndbuf
    //! sound buffer ( now interpolating ) 
    //! reads from a variety of file formats
    //! see \example sndbuf.ck
    //QUERY->ugen_add( QUERY, "sndbuf", NULL );
    // set funcs
    //QUERY->ugen_func( QUERY, sndbuf_ctor, sndbuf_dtor, sndbuf_tick, NULL );
    // set ctrl
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_read, NULL, "string", "read" ); //! loads file for reading
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_write, NULL, "string", "write" ); //! loads a file for writing ( or not ) 
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_pos, sndbuf_cget_pos, "int", "pos" ); //! set position ( 0 < p < .samples ) 
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_loop, sndbuf_cget_loop, "int", "loop" ); //! toggle looping 
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_interp, sndbuf_cget_interp, "int", "interp" ); //! set interpolation ( 0=drop, 1=linear, 2=sinc )
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_rate, sndbuf_cget_rate, "float", "rate" ); //! playback rate ( relative to file's natural speed ) 
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_rate, sndbuf_cget_rate, "float", "play" ); //! play (same as rate) 
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_freq, sndbuf_cget_freq, "float", "freq" ); //! playback rate ( file loops / second ) 
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_phase, sndbuf_cget_phase, "float", "phase" ); //! set phase position ( 0-1 )
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_channel, sndbuf_cget_channel, "int", "channel" ); //! select channel ( 0 < p < .channels )
    //QUERY->ugen_ctrl( QUERY, sndbuf_ctrl_phase_offset, sndbuf_cget_phase, "float", "phase_offset" ); //! set a phase offset
    //QUERY->ugen_ctrl( QUERY, NULL, sndbuf_cget_samples, "int", "samples" ); //! fetch number of samples
    //QUERY->ugen_ctrl( QUERY, NULL, sndbuf_cget_length, "dur", "length" ); //! fetch length
    //QUERY->ugen_ctrl( QUERY, NULL, sndbuf_cget_channels, "int", "channels" ); //! fetch number of channels
    
    //---------------------------------------------------------------------
    // init as base class: sndbuf
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "SndBuf", "UGen", env->global(), 
                                        sndbuf_ctor, sndbuf_tick, NULL ) )
        return FALSE;

    // add member variable
    sndbuf_offset_data = type_engine_import_mvar( env, "int", "@sndbuf_data", FALSE );
    if( sndbuf_offset_data == CK_INVALID_OFFSET ) goto error;

    // add ctrl: read
    func = make_new_mfun( "string", "read", sndbuf_ctrl_read );
    func->add_arg( "string", "read" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: read // area
    //func = make_new_mfun( "string", "read", sndbuf_cget_read );
    //if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: write
    func = make_new_mfun( "string", "write", sndbuf_ctrl_write );
    func->add_arg( "string", "read" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: write
    //func = make_new_mfun( "string", "write", sndbuf_cget_write );
    //if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: pos
    func = make_new_mfun( "int", "pos", sndbuf_ctrl_pos );
    func->add_arg( "int", "pos" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: pos
    func = make_new_mfun( "int", "pos", sndbuf_cget_pos );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: loop
    func = make_new_mfun( "int", "loop", sndbuf_ctrl_loop );
    func->add_arg( "int", "loop" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: loop
    func = make_new_mfun( "int", "loop", sndbuf_cget_loop );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: interp
    func = make_new_mfun( "int", "interp", sndbuf_ctrl_interp );
    func->add_arg( "int", "interp" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: interp
    func = make_new_mfun( "int", "interp", sndbuf_cget_interp );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: rate
    func = make_new_mfun( "float", "rate", sndbuf_ctrl_rate );
    func->add_arg( "float", "rate" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: rate
    func = make_new_mfun( "float", "rate", sndbuf_cget_rate );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: play
    func = make_new_mfun( "float", "play", sndbuf_ctrl_rate );
    func->add_arg( "float", "play" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: play // good
    func = make_new_mfun( "float", "play", sndbuf_cget_rate );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: freq
    func = make_new_mfun( "float", "freq", sndbuf_ctrl_freq );
    func->add_arg( "float", "freq" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: freq
    func = make_new_mfun( "float", "freq", sndbuf_cget_freq );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: phase
    func = make_new_mfun( "float", "phase", sndbuf_ctrl_phase );
    func->add_arg( "float", "phase" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: phase
    func = make_new_mfun( "float", "phase", sndbuf_cget_phase );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: channel
    func = make_new_mfun( "int", "channel", sndbuf_ctrl_channel );
    func->add_arg( "int", "channel" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: channel
    func = make_new_mfun( "int", "channel", sndbuf_cget_channel );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: phase_offset
    func = make_new_mfun( "float", "phaseOffset", sndbuf_ctrl_phase_offset );
    func->add_arg( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: phase_offset
    // func = make_new_mfun( "float", "phaseOffset", sndbuf_cget_phase_offset );
    // if( !type_engine_import_mfun( env, func ) ) goto error;

    // add ctrl: chunks
    func = make_new_mfun( "int", "chunks", sndbuf_ctrl_chunks );
    func->add_arg( "int", "frames" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    // add cget: chunks
    func = make_new_mfun( "int", "chunks", sndbuf_cget_chunks );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add cget: samples
    func = make_new_mfun( "int", "samples", sndbuf_cget_samples );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add cget: length
    func = make_new_mfun( "dur", "length", sndbuf_cget_length );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add cget: channels
    func = make_new_mfun( "int", "channels", sndbuf_cget_channels );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add cget: valueAt
    func = make_new_mfun( "float", "valueAt", sndbuf_cget_valueAt );
    func->add_arg( "int", "pos" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end import
    if( !type_engine_import_class_end( env ) )
        return FALSE;

    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}




//-----------------------------------------------------------------------------
// name: multi_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( multi_ctor )
{
    // do nothing
}




//-----------------------------------------------------------------------------
// name: multi_cget_chan()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CGET( multi_cget_chan )
{
    // get ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // value
    t_CKINT index = GET_NEXT_INT( ARGS );
    // return
    RETURN->v_object = index >= 0 && index < ugen->m_multi_chan_size ?
        ugen->m_multi_chan[index] : NULL ;
}




//-----------------------------------------------------------------------------
// name: stereo_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( stereo_ctor )
{
    // get ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;

    // multi
    if( ugen->m_multi_chan_size )
    {
        // set left
        OBJ_MEMBER_UINT( SELF, stereo_offset_left ) = (t_CKUINT)(ugen->m_multi_chan[0]);
        // set right
        OBJ_MEMBER_UINT( SELF, stereo_offset_right ) = (t_CKUINT)(ugen->m_multi_chan[1]);
    }
    else // mono
    {
        // set left and right to self
        OBJ_MEMBER_UINT( SELF, stereo_offset_left ) = (t_CKUINT)ugen;
        OBJ_MEMBER_UINT( SELF, stereo_offset_right ) = (t_CKUINT)ugen;
    }
}




//-----------------------------------------------------------------------------
// name: stereo_ctrl_pan()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( stereo_ctrl_pan )
{
    Chuck_UGen * ugen = (Chuck_UGen * )SELF;
    Chuck_UGen * left = ugen->m_multi_chan[0];
    Chuck_UGen * right = ugen->m_multi_chan[1];
    // get arg
    t_CKFLOAT pan = GET_CK_FLOAT(ARGS);
    // clip it
    if( pan < -1.0 ) pan = -1.0;
    else if( pan > 1.0 ) pan = 1.0;
    // set it
    OBJ_MEMBER_FLOAT( SELF, stereo_offset_pan ) = pan;
    // pan it
    left->m_pan = pan < 0.0 ? 1.0 : 1.0 - pan;
    right->m_pan = pan > 0.0 ? 1.0 : 1.0 + pan;

    RETURN->v_float = pan;
}




//-----------------------------------------------------------------------------
// name: stereo_cget_pan()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CGET( stereo_cget_pan )
{
    RETURN->v_float = OBJ_MEMBER_FLOAT( SELF, stereo_offset_pan );
}




//-----------------------------------------------------------------------------
// name: dac_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( dac_tick )
{
    *out = in; return TRUE;
}




//-----------------------------------------------------------------------------
// name: bunghole_tick
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( bunghole_tick )
{
    *out = 0.0f; return 0;
}




//-----------------------------------------------------------------------------
// name: noise_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( noise_tick )
{
    *out = -1.0 + 2.0 * (SAMPLE)rand() / RAND_MAX;
    return TRUE;
}


enum { NOISE_WHITE=0, NOISE_PINK, NOISE_BROWN, NOISE_FBM, NOISE_FLIP, NOISE_XOR };

class CNoise_Data
{
private:
  SAMPLE value;

  t_CKFLOAT fbmH;
  t_CKINT counter;
  t_CKINT * pink_array;
  t_CKINT  pink_depth;
  bool pink_rand;
  t_CKINT rand_bits;
  double scale;
  double bias;

  t_CKINT last;

public:
  CNoise_Data() { 
    value = 0; 
    mode = NOISE_PINK; 
    pink_depth = 24;
    pink_array = NULL;
    counter = 1;
    scale = 2.0 / (double) RAND_MAX ;
    bias = -1.0;
    pink_rand = false;
    t_CKINT randt = RAND_MAX;
    rand_bits = 0;
    fprob = (t_CKINT)( (double)RAND_MAX * 1.0 / 32.0 );
    while ( randt > 0 ) { 
      rand_bits++;
      randt = randt >> 1;
    }
    fprintf(stderr, "random bits - %d", rand_bits );
    setMode ( "pink" );
  } 
  ~CNoise_Data() {}
   
  t_CKINT fprob;
  t_CKUINT mode;
  void tick( t_CKTIME now, SAMPLE * out );
  void setMode(char * c);

  t_CKINT pink_tick( SAMPLE * out);
  t_CKINT brown_tick( SAMPLE * out);
  t_CKINT xor_tick( SAMPLE * out);
  t_CKINT flip_tick( SAMPLE * out);
  t_CKINT fbm_tick( SAMPLE * out);
};


CK_DLL_CTOR( cnoise_ctor )
{
    OBJ_MEMBER_UINT( SELF, cnoise_offset_data ) = (t_CKUINT)new CNoise_Data;
}

CK_DLL_DTOR( cnoise_dtor )
{
    delete (CNoise_Data *)OBJ_MEMBER_UINT( SELF, cnoise_offset_data );
}

CK_DLL_TICK( cnoise_tick)
{
  CNoise_Data * d = ( CNoise_Data * )OBJ_MEMBER_UINT( SELF, cnoise_offset_data );
  switch( d->mode ) { 
  case NOISE_WHITE: 
    return noise_tick(SELF,in,out);
    break;
  case NOISE_PINK:
    return d->pink_tick(out);
    break;
  case NOISE_BROWN:
    return d->brown_tick(out);
    break;
  case NOISE_XOR:
    return d->xor_tick(out);
    break;
  case NOISE_FLIP:
    return d->flip_tick(out);
    break;
  case NOISE_FBM:
    return d->fbm_tick(out);
    break;
  }
  return TRUE;
}

t_CKINT CNoise_Data::pink_tick( SAMPLE * out)
{ 
  //based on Voss-McCartney

  if ( pink_array == NULL ) { 
    pink_array = (t_CKINT *) malloc ( sizeof ( t_CKINT ) * pink_depth );
    last = 0;
    for ( t_CKINT i = 0 ; i < pink_depth ; i++ ) { pink_array[i] = rand(); last += pink_array[i]; } 
    scale = 2.0 / ((double)RAND_MAX  * ( pink_depth + 1.0 ) );
    bias = 0.0;
    fprintf( stderr, "scale %f %f %d %d \n", scale, bias, RAND_MAX, pink_depth + 1 );
  }

  t_CKINT pind = 0;

  //count trailing zeroes
  while ( pind < pink_depth && ! (counter & ( 1 << pind ) ) ) pind++;

  //  fprintf (stderr, "counter %d pink - %d \n", counter, pind );

  if ( pind < pink_depth ) { 
    t_CKINT diff = rand() - pink_array[pind];
    pink_array[pind] += diff;
    last += diff;
  }

  *out = bias + scale * ( rand() + last );
  counter++;
  if ( pink_rand ) counter = rand();
  return TRUE;
}

t_CKINT CNoise_Data::xor_tick( SAMPLE * out )
{ 
  t_CKINT mask = 0;
  for ( t_CKINT i = 0; i < rand_bits ; i++ ) 
    if ( rand() <= fprob ) 
      mask |= ( 1 << i );
  last = last ^ mask;  
  *out = bias + scale * (SAMPLE)last;
  return TRUE;
}

t_CKINT CNoise_Data::flip_tick( SAMPLE * out )
{
  t_CKINT ind = (t_CKINT) ( (double) rand_bits * rand() / ( RAND_MAX + 1.0 ) );
 
  last = last ^ ( 1 << ind );
  //  fprintf ( stderr, "ind - %d %d %f %f", ind, last, bias, scale );
  *out = bias + scale * (SAMPLE)last;
  return TRUE;
}

t_CKINT
CNoise_Data::brown_tick( SAMPLE * out ) { 
  //brownian noise function..later!
  *out = 0;
  return TRUE;
}

t_CKINT
CNoise_Data::fbm_tick( SAMPLE * out ) { 
  //non-brownian noise function..later!
  *out = 0;
  return TRUE;
}

void
CNoise_Data::setMode( char * c ) { 
  if ( strcmp ( c, "white" ) == 0 ) { 
    fprintf(stderr, "white noise\n");
    mode = NOISE_WHITE;    
    scale = 2.0 / (t_CKFLOAT)RAND_MAX;
    bias = -1.0;
  }
  if ( strcmp ( c, "pink" ) == 0 ) { 
    fprintf(stderr, "pink noise\n");
    mode = NOISE_PINK;
    scale = 2.0 / (double)(RAND_MAX  * ( pink_depth + 1 ) );
    bias = -1.0;
  }
  if ( strcmp ( c, "flip" ) == 0) { 
    fprintf(stderr, "bitflip noise\n");
    mode = NOISE_FLIP;
    scale = 2.0 / (t_CKFLOAT)RAND_MAX;
    bias = -1.0;
  }
  if ( strcmp ( c, "xor" ) == 0) {
    fprintf(stderr, "xor noise\n"); 
    mode = NOISE_XOR;
    scale = 2.0 / (t_CKFLOAT)RAND_MAX;
    bias = -1.0;
  }
  if ( strcmp ( c, "brown" ) == 0) { 
    fprintf(stderr, "brownian noise\n");
    mode = NOISE_BROWN;
    scale = 2.0 / (t_CKFLOAT)RAND_MAX;
    bias = -1.0;
  }
  if ( strcmp ( c, "fbm" ) == 0) {   
    fprintf(stderr, "fbm noise\n");
    mode = NOISE_FBM;
    scale = 2.0 / (t_CKFLOAT)RAND_MAX;
    bias = -1.0;
  }

}

CK_DLL_CTRL( cnoise_ctrl_mode )
{
    CNoise_Data * d = ( CNoise_Data * )OBJ_MEMBER_UINT( SELF, cnoise_offset_data );
    char * mode= *(char **)GET_CK_STRING(ARGS);
    d->setMode(mode);
}

CK_DLL_CTRL( cnoise_ctrl_fprob )
{
    CNoise_Data * d = ( CNoise_Data * )OBJ_MEMBER_UINT( SELF, cnoise_offset_data );
    t_CKFLOAT p= GET_CK_FLOAT(ARGS);
    d->fprob = (t_CKINT) ( (double)RAND_MAX * p );
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
CK_DLL_CTOR( impulse_ctor )
{
    // return data to be used later
    OBJ_MEMBER_UINT( SELF, impulse_offset_data ) = (t_CKUINT)new Pulse_Data;
}




//-----------------------------------------------------------------------------
// name: impulse_dtor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_DTOR( impulse_dtor )
{
    // delete
    delete (Pulse_Data *)OBJ_MEMBER_UINT( SELF, impulse_offset_data );
}




//-----------------------------------------------------------------------------
// name: impulse_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( impulse_tick )
{
    Pulse_Data * d = (Pulse_Data *)OBJ_MEMBER_UINT( SELF, impulse_offset_data );
    if( d->when )
    {
        *out = d->value;
        d->when = 0;
    }
    else
        *out = 0.0f;
    
    return TRUE;
}



//-----------------------------------------------------------------------------
// name: impulse_ctrl_next()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( impulse_ctrl_next )
{
    Pulse_Data * d = (Pulse_Data *)OBJ_MEMBER_UINT( SELF, impulse_offset_data );
    d->value = (SAMPLE)GET_CK_FLOAT(ARGS);
    d->when = 1;
    RETURN->v_float = (t_CKFLOAT)d->value;
}




//-----------------------------------------------------------------------------
// name: impulse_cget_next()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CGET( impulse_cget_next )
{
    Pulse_Data * d = (Pulse_Data *)OBJ_MEMBER_UINT( SELF, impulse_offset_data );
    RETURN->v_float = (t_CKFLOAT)d->value;
}




//-----------------------------------------------------------------------------
// name: step_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( step_ctor )
{
    // return data to be used later
    OBJ_MEMBER_UINT( SELF, step_offset_data ) = (t_CKUINT)new SAMPLE( 1.0f );
}




//-----------------------------------------------------------------------------
// name: step_dtor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_DTOR( step_dtor )
{
    // delete
    delete (SAMPLE *)OBJ_MEMBER_UINT( SELF, step_offset_data );
}


//-----------------------------------------------------------------------------
// name: step_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( step_tick )
{
    SAMPLE * d = (SAMPLE *)OBJ_MEMBER_UINT( SELF, step_offset_data );
    *out = *d;
    
    return TRUE;
}


//-----------------------------------------------------------------------------
// name: step_ctrl_next()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( step_ctrl_next )
{
    SAMPLE * d = (SAMPLE *)OBJ_MEMBER_UINT( SELF, step_offset_data );
    *d = (SAMPLE)GET_CK_FLOAT(ARGS);
    RETURN->v_float = (t_CKFLOAT)(*d);
}


//-----------------------------------------------------------------------------
// name: step_cget_next()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CGET( step_cget_next )
{
    SAMPLE * d = (SAMPLE *)OBJ_MEMBER_UINT( SELF, step_offset_data );
    RETURN->v_float = (t_CKFLOAT)(*d);
}




//-----------------------------------------------------------------------------
// name: halfrect_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( halfrect_tick )
{
    *out = in > 0.0f ? in : 0.0f;
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: fullrect_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( fullrect_tick )
{
    *out = in >= 0.0f ? in : -in;
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: zerox_ctor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( zerox_ctor )
{
    OBJ_MEMBER_UINT( SELF, zerox_offset_data ) = (t_CKUINT)new SAMPLE( 0.0f );
}


//-----------------------------------------------------------------------------
// name: zerox_dtor()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_DTOR( zerox_dtor )
{
    delete (SAMPLE *)OBJ_MEMBER_UINT( SELF, zerox_offset_data );
}


#define __SGN(x)  (x >= 0.0f ? 1.0f : -1.0f )
//-----------------------------------------------------------------------------
// name: zerox_tick()
// desc: ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( zerox_tick )
{
    SAMPLE * d = (SAMPLE *)OBJ_MEMBER_UINT( SELF, zerox_offset_data );
    *out = __SGN(in) != __SGN(*d);
    *out *= __SGN(in);
    *d = in;
    
    return TRUE;
}




struct delayp_data 
{ 

    SAMPLE *	buffer;
    t_CKINT		bufsize;
    
	t_CKTIME	now;

    double		readpos;   //readpos ( moves at constant speed, sample per sample
    double		writepos; // relative to read position
    
    t_CKTIME	offset; // distance between read and write
    
    t_CKDUR		offset_start; 
    t_CKDUR		offset_target;

    t_CKTIME	move_end_time; //target time
    t_CKDUR		move_duration; //time we started shift
    
    SAMPLE		last_sample;
    t_CKDUR		last_offset;
    
#ifdef _DEBUG
	int			lasti;
#endif

    double		acoeff[2];
    double		bcoeff[2];
    SAMPLE		outputs[3];
    SAMPLE		inputs[3];
    
    delayp_data() 
    { 
        bufsize  = 2 * g_srate;
        buffer   = ( SAMPLE * ) realloc ( NULL, sizeof ( SAMPLE ) * bufsize );
        t_CKINT i;
        
        for ( i = 0 ; i < bufsize ; i++ ) buffer[i] = 0;
        for ( i = 0 ; i < 3 ; i++ ) { acoeff[i] = 0; bcoeff[i] = 0; }
        
		acoeff[0] = 1.0;
        acoeff[1] = -.99;
        bcoeff[0] = 1.0;
        bcoeff[1] = -1.0;
        
		readpos  = 0.0;        

		now				= 0.0;

        offset			= 0.0; 
        last_offset		= 0.0;
        offset_start	= 0.0;
        offset_target	= 0.0;

        move_duration	= 1.0;
        move_end_time	= 0.0;

#ifdef _DEBUG
		lasti		= -1;
#endif
        last_sample = 0;
    }
};

CK_DLL_CTOR( delayp_ctor )
{
    OBJ_MEMBER_UINT( SELF, delayp_offset_data ) = (t_CKUINT)new delayp_data;    
}

CK_DLL_DTOR( delayp_dtor )
{
    delayp_data * d = (delayp_data *)OBJ_MEMBER_UINT( SELF, delayp_offset_data );
    if( d->buffer ) delete [] d->buffer;
    delete d;
}

CK_DLL_PMSG( delayp_pmsg )
{
    return TRUE;
}


CK_DLL_TICK( delayp_tick )
{
    delayp_data * d = (delayp_data *)OBJ_MEMBER_UINT( SELF, delayp_offset_data );
    if ( !d->buffer ) return FALSE;

    // area
    d->now = ((Chuck_UGen*)SELF)->shred->vm_ref->shreduler()->now_system;
    
    //calculate new write-offset position ( we interpolate if we've been assigned a new write-offset )
    if ( d->now >= d->move_end_time || d->move_duration == 0 ) d->offset = d->offset_target;
    else  { 
        double dt = 1.0 + ( d->now - d->move_end_time ) / ( d->move_duration ); 
        d->offset = d->offset_start + dt * ( d->offset_target - d->offset_start );
        //      fprintf (stderr, "dt %f, off %f , start %f target %f\n", dt, d->writeoff,  d->writeoff_start, d->writeoff_target );
    }
    
    //find locations in buffer...
    double write		= (d->readpos )			+ d->offset;
    double last_write	= (d->readpos - 1.0)	+ d->last_offset ;
    
    //linear interpolation.  will introduce some lowpass/aliasing.
    double write_delta  = write - last_write;
    double sample_delta = in - d->last_sample;

	double duck_constant = 0.69;

	
	double gee = fabs(write_delta) - 1.0;

	if ( gee < 24.0 ) { 
		double head_contact = ( gee > 0 ) ? exp ( - duck_constant * gee ) : 1.0;
		t_CKINT i, smin, smax, sampi;
	    SAMPLE sampf = 0;
		if ( write_delta >= 0 ) { //forward.
			smin = (t_CKINT) floor ( last_write );
			smax = (t_CKINT) floor ( write );
			for ( i = smin+1 ; i <= smax ; i++ ) { 
				sampf = d->last_sample + sample_delta * ( double(i) - last_write ) / write_delta;
				sampi = ( i + d->bufsize * 2 ) % d->bufsize;
#ifdef _DEBUG 
				if ( d->lasti == sampi ) { 
					fprintf(stderr, "over!\n");
				}
				d->lasti = sampi;
#endif
				d->buffer[sampi] += sampf * head_contact ;
			}
		}
		else { //moving in reverse
			smin = (t_CKINT) floor ( write );
			smax = (t_CKINT) floor ( last_write );
			for ( i = smin+1 ; i <= smax ; i++ ) { 
				sampf = d->last_sample + sample_delta * ( double(i) - last_write ) / write_delta;
				sampi = ( i + d->bufsize * 2 ) % d->bufsize;
#ifdef _DEBUG
				if ( d->lasti == sampi ) { 
					fprintf(stderr, "over!\n");
				}
				d->lasti = sampi;
#endif
				d->buffer[sampi] += sampf * head_contact;   
			}
		}
	}
    
    d->last_offset = d->offset;
    d->last_sample = in;
    
    
    //output should go through a dc blocking filter, for cases where 
    //we are zipping around in the buffer leaving a fairly constant
    //trail of samples
    
    //output last sample
    
	t_CKINT rpos = (t_CKINT) fmod( d->readpos, d->bufsize ) ; 
    
    //   *out = d->buffer[rpos];

	/*    
    // did i try to write a dc blocking filter? 
    d->outputs[0] =  0.0;
    d->inputs [0] =  d->buffer[rpos];

    d->outputs[0] += d->bcoeff[1] * d->inputs[1];
    d->inputs [1] =  d->inputs[0];
    
    d->outputs[0] += d->bcoeff[0] * d->inputs[0];
    
    d->outputs[0] += -d->acoeff[1] * d->outputs[1];
    d->outputs[1] =  d->outputs[0];

    //clear at readpos ( write doesn't !)
    *out = d->outputs[0];
	*/

	*out = d->buffer[rpos];
    
    d->buffer[rpos] = 0; //clear once it's been read
	d->readpos = fmod ( d->readpos + 1.0 , double( d->bufsize ) );
 
	return TRUE;
    
}

CK_DLL_CTRL( delayp_ctrl_delay )
{
    delayp_data * d = ( delayp_data * ) OBJ_MEMBER_UINT( SELF, delayp_offset_data );
    t_CKDUR target = GET_CK_DUR(ARGS); // rate     
    // area

    if ( target != d->offset_target ) {
	    if ( target > d->bufsize ) { 
            fprintf( stderr, "[chuck](via delayp): delay time %f over max!  set max first!\n", target);
            return;
        }
        d->offset_target = target;
        d->offset_start  = d->last_offset;

		t_CKTIME snow = ((Chuck_UGen*)SELF)->shred->now;
        d->move_end_time = snow + d->move_duration; 
    }
    RETURN->v_dur = d->last_offset; // TODO:
}

CK_DLL_CGET( delayp_cget_delay )
{
    delayp_data * d = ( delayp_data * ) OBJ_MEMBER_UINT( SELF, delayp_offset_data );
    //SET_NEXT_DUR( out, d->writeoff_last );
    RETURN->v_dur = d->last_offset; // TODO:
}

CK_DLL_CTRL( delayp_ctrl_window )
{
    delayp_data * d = ( delayp_data * ) OBJ_MEMBER_UINT( SELF, delayp_offset_data );
    t_CKDUR window = GET_CK_DUR(ARGS); // rate     
    if ( window >= 0 ) {
        d->move_duration = window;
        //fprintf ( stderr, "set window time %f , %f , %d \n", d->writeoff_window_time, d->writeoff, d->bufsize );
    }
    RETURN->v_dur = d->move_duration; // TODO:
}

CK_DLL_CGET( delayp_cget_window )
{
    delayp_data * d = ( delayp_data * ) OBJ_MEMBER_UINT( SELF, delayp_offset_data );
    RETURN->v_dur = d->move_duration; // TODO:
}


CK_DLL_CTRL( delayp_ctrl_max )
{   
    delayp_data * d = ( delayp_data * ) OBJ_MEMBER_UINT( SELF, delayp_offset_data );
    t_CKDUR nmax = GET_CK_DUR(ARGS); // rate 
    if ( d->bufsize != (t_CKINT)nmax && nmax > 1.0 ) { 
        d->bufsize = (t_CKINT)(nmax+.5); 
        d->buffer = ( SAMPLE * ) realloc ( d->buffer, sizeof ( SAMPLE ) * d->bufsize );
        for ( t_CKINT i = 0; i < d->bufsize; i++ ) d->buffer[i] = 0;
    }
    RETURN->v_dur = d->bufsize; // TODO??
}

CK_DLL_CGET( delayp_cget_max )
{
    delayp_data * d = ( delayp_data * ) OBJ_MEMBER_UINT( SELF, delayp_offset_data );
    //SET_NEXT_DUR( out, (t_CKDUR) d->bufsize );
    RETURN->v_dur = d->bufsize;
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

// data for each sndbuf
struct sndbuf_data
{
    SAMPLE * buffer;
    t_CKUINT num_samples;
    t_CKUINT num_channels;
    t_CKUINT num_frames;
    t_CKUINT samplerate;
    t_CKUINT chan;
    t_CKUINT chunks;
    t_CKUINT chunks_read;
    t_CKUINT chunks_total;
    t_CKUINT chunks_size;
    bool * chunk_table;

    SAMPLE * eob;
    SAMPLE * curr;
    t_CKFLOAT sampleratio;
    t_CKFLOAT curf;
    t_CKFLOAT rate_factor;
    t_CKFLOAT rate;
    t_CKINT interp;
    t_CKBOOL loop;
    
    bool sinc_table_built;
    bool sinc_use_table;
    bool sinc_use_interp;
    t_CKINT sinc_samples_per_zero_crossing;
    t_CKINT sinc_width;
    double * sinc_table;

    SNDFILE * fd;

    // constructor
    sndbuf_data()
    {
        buffer = NULL;
        interp = SNDBUF_INTERP;
        num_channels = 0;
        num_frames = 0;
        num_samples = 0;
        chunks = 0;
        chunks_read = 0;
        chunks_total = 0;
        chunks_size = 0;
        chunk_table = NULL;
        samplerate = 0;
        sampleratio = 1.0;
        chan = 0;
        curf = 0.0;
        rate_factor = 1.0;
        rate = 1.0;
        eob = NULL;
        curr = NULL;
        
        sinc_table_built = false;
        sinc_use_table = USE_TABLE;
        sinc_use_interp = USE_INTERP;
        sinc_width = WIDTH;
        sinc_samples_per_zero_crossing = SAMPLES_PER_ZERO_CROSSING;
        sinc_table = NULL;
        
        loop = FALSE;
        fd = NULL;
    }
};


double sndbuf_sinc( sndbuf_data * d, double x );
double sndbuf_t_sinc( sndbuf_data * d, double x );
void   sndbuf_make_sinc( sndbuf_data * d );
void   sndbuf_sinc_interpolate( sndbuf_data * d, SAMPLE * out );

CK_DLL_CTOR( sndbuf_ctor )
{
    OBJ_MEMBER_UINT( SELF, sndbuf_offset_data ) = (t_CKUINT)new sndbuf_data;
}

CK_DLL_DTOR( sndbuf_dtor )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    if( d->buffer ) delete [] d->buffer;
    if( d->chunk_table ) delete [] d->chunk_table;
    delete d;
}

inline t_CKUINT sndbuf_read( sndbuf_data * d, t_CKUINT offset, t_CKUINT howmuch )
{
    // check
    if( d->fd == NULL ) return 0;
    if( offset >= d->num_frames ) return 0;
    
    // log
    // EM_log( CK_LOG_FINE, "(sndbuf): reading %d:%d frames...", offset, howmuch );

    // prevent overflow
    if( howmuch > d->num_frames - offset )
        howmuch = d->num_frames - offset;

    t_CKUINT n;
    // seek
    sf_seek( d->fd, offset, SEEK_SET );
#if defined(CK_S_DOUBLE)
    n = sf_readf_double( d->fd, d->buffer+offset*d->num_channels, howmuch );
#else
    n = sf_readf_float( d->fd, d->buffer+offset*d->num_channels, howmuch );
#endif

    d->chunks_read += n;

    // close
    if( d->chunks_read >= d->num_frames )
    {
        // log
        EM_log( CK_LOG_INFO, "(sndbuf): all frames read, closing file..." );
        sf_close( d->fd );
        d->fd = NULL;
    }

    return n;
}

inline t_CKINT sndbuf_load( sndbuf_data * d, t_CKUINT where )
{
    // map to bin
    t_CKUINT bin = (t_CKUINT)(where / (t_CKFLOAT)d->chunks_size);
    if( bin >= d->chunks_total ) return 0;

    // already loaded
    if( d->chunk_table[bin] ) return 0;

    // read it
    t_CKINT ret = sndbuf_read( d, bin*d->chunks_size, d->chunks_size );

    // flag it
    d->chunk_table[bin] = true;

    // log
    // EM_log( CK_LOG_FINER, "chunk test: pos: %d bin: %d read:%d/%d", where, bin, d->chunks_read, d->num_frames );

    return ret;
}

inline void sndbuf_setpos( sndbuf_data *d, double pos )
{
    if( !d->buffer ) return;

    d->curf = pos;

    // set curf within bounds
    if( d->loop )
    {
        while( d->curf >= d->num_frames ) d->curf -= d->num_frames;
        while( d->curf < 0 ) d->curf += d->num_frames;
    } 
    else
    {
        if( d->curf < 0 ) d->curf = 0;
        else if( d->curf >= d->num_frames ) d->curf = d->num_frames;
    }

    t_CKINT i = (t_CKINT)d->curf;
    // ensure load
    if( d->fd != NULL ) sndbuf_load( d, i );
    // sets curr to correct position ( account for channels ) 
    d->curr = d->buffer + d->chan + i * d->num_channels;
}

inline SAMPLE sndbuf_sampleAt( sndbuf_data * d, t_CKINT pos )
{ 
    // boundary cases
    t_CKINT nf = d->num_frames;
    if( d->loop ) { 
        while( pos > nf ) pos -= nf;
        while( pos <  0  ) pos += nf;
    }
    else { 
        if( pos > nf ) pos = nf;
        if( pos < 0   ) pos = 0;
    }

    t_CKUINT index = d->chan + pos * d->num_channels;
    // ensure load
    if( d->fd != NULL ) sndbuf_load( d, pos );
    // return sample
    return d->buffer[index];
}

inline double sndbuf_getpos( sndbuf_data * d )
{
    if( !d->buffer ) return 0;
    return floor(d->curf);
}

CK_DLL_CTRL( sndbuf_ctrl_loop )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    d->loop = GET_CK_INT(ARGS);
    RETURN->v_int = d->loop; // TODO: Check (everything)
} 

CK_DLL_CGET( sndbuf_cget_loop )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    RETURN->v_int = d->loop;
}

// PRC's sinc interpolation function.. as found 
// http://www.cs.princeton.edu/courses/archive/spring03/cs325/src/TimeStuf/srconvrt.c
//
// there's probably a lot in there that could be optimized, if we care to..

// #define PI 3.14159265358979323846
// wow... we are sensitive.. 

inline double sndbuf_linear_interp( double * first, double * second, double * frac );
bool sinc_table_built = false;

void sndbuf_sinc_interpolate( sndbuf_data *d, SAMPLE * out )
{
    signed long j;
    double factor = d->rate;
    double time_now = d->curf;
    double one_over_factor;
    // UNUSED: double int_time = 0;
    // UNUSED: double last_time = 0;
    double temp1 = 0.0;
    
    long time_i = (long)time_now;
    
    // bounds checking now in sampleAt function...
    if( factor < 1.0 ) {
        for( j = -d->sinc_width + 1; j < d->sinc_width; j++ )
        {
            temp1 += sndbuf_sampleAt(d,time_i+j) * sndbuf_sinc( d, (double)j );
        }
        *out = (SAMPLE)temp1;
    }
    else {
        one_over_factor = 1.0 / factor;
        for( j = -d->sinc_width + 1; j < d->sinc_width; j++ ) {
            temp1 += sndbuf_sampleAt(d,time_i+j) * one_over_factor * sndbuf_sinc( d, one_over_factor * (double)j );
        } 
        *out = (SAMPLE)temp1;
    }
}

void sndbuf_make_sinc( sndbuf_data * d )
{
    t_CKINT i;
    // fprintf(stderr, "building sinc table\n" );
    double temp, win_freq, win;
    win_freq = ONE_PI / d->sinc_width / d->sinc_samples_per_zero_crossing;
    t_CKINT tabsize = d->sinc_width * d->sinc_samples_per_zero_crossing;
    
    d->sinc_table = (double *) realloc( d->sinc_table, tabsize * sizeof(double) );
    d->sinc_table[0] = 1.0;
    for( i = 1; i < tabsize; i++ ) {
        temp = (double) i * ONE_PI / d->sinc_samples_per_zero_crossing;
        d->sinc_table[i] = (float)(sin(temp) / temp);
        win = 0.5 + 0.5 * cos(win_freq * i);
        d->sinc_table[i] *= (float)win;
    }
    d->sinc_table_built = true;
}

inline double sndbuf_linear_interp( double first_number, double second_number, double fraction )
{
    return (first_number + ((second_number - first_number) * fraction));
}

double sndbuf_t_sinc( sndbuf_data * d, double x )
{
    t_CKINT low;
    double temp,delta;
    if( !d->sinc_table_built ) sndbuf_make_sinc(d);
    if( fabs(x) >= d->sinc_width-1 )
        return 0.0;
    else {
        temp = fabs(x) * (double) d->sinc_samples_per_zero_crossing;
        low = (t_CKINT)temp; /* these are interpolation steps */
        if( d->sinc_use_interp ) {
            delta = temp - low;  /* and can be ommited if desired */
            return sndbuf_linear_interp( d->sinc_table[low], d->sinc_table[low + 1], delta );
        }
        else return d->sinc_table[low];
    }
}

double sndbuf_sinc( sndbuf_data * d, double x )
{
    double temp;
    
    if( d->sinc_use_table ) return sndbuf_t_sinc(d,x);
    else {
        if( x == 0.0 ) return 1.0;
        else {
            temp = ONE_PI * x;
            return sin(temp) / (temp);
        }
    }
}

CK_DLL_TICK( sndbuf_tick )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    if( !d->buffer ) return FALSE;
    
    // we're ticking once per sample ( system )
    // curf in samples;
    
    if( !d->loop && d->curr >= d->eob + d->num_channels ) return FALSE;
    
    // calculate frame    
    if( d->interp == SNDBUF_DROP )
    { 
        *out = (SAMPLE)( (*(d->curr)) ) ;
    }
    else if( d->interp == SNDBUF_INTERP )
    {
        // samplewise linear interp
        double alpha = d->curf - floor(d->curf);
        *out = (SAMPLE)( (*(d->curr)) ) ;
        *out += (float)alpha * ( sndbuf_sampleAt(d, (long)d->curf+1 ) - *out );
    }
    else if( d->interp == SNDBUF_SINC ) {
        // do that fancy sinc function!
        sndbuf_sinc_interpolate(d, out);
    }
    
    // advance
    d->curf += d->rate;
    sndbuf_setpos(d, d->curf);

    return TRUE;    
}


#include "util_raw.h"


CK_DLL_CTRL( sndbuf_ctrl_read )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    const char * filename = GET_CK_STRING(ARGS)->str.c_str();
    
    if( d->buffer )
    {
        delete [] d->buffer;
        d->buffer = NULL;
    }

    if( d->chunk_table )
    {
        delete [] d->chunk_table;
        d->chunk_table = NULL;
    }

    if( d->fd )
    {
        sf_close( d->fd );
        d->fd = NULL;
    }

    // log
    EM_log( CK_LOG_INFO, "(sndbuf): reading '%s'...", filename );
    
    // built in
    if( strstr(filename, "special:") )
    {
        SAMPLE * rawdata = NULL;
        t_CKUINT rawsize = 0;
        t_CKUINT srate = 22050;

        // which
        if( strstr(filename, "special:sinewave") ) {
            rawsize = 256; rawdata = NULL;
        }
        else if( strstr(filename, "special:ahh") ) {
            rawsize = ahh_size; rawdata = ahh_data;
        }
        else if( strstr(filename, "special:britestk") ) {
            rawsize = britestk_size; rawdata = britestk_data;
        }
        else if( strstr(filename, "special:dope") ) {
            rawsize = dope_size; rawdata = dope_data;
        }
        else if( strstr(filename, "special:eee") ) {
            rawsize = eee_size; rawdata = eee_data;
        }
        else if( strstr(filename, "special:fwavblnk") ) {
            rawsize = fwavblnk_size; rawdata = fwavblnk_data;
        }
        else if( strstr(filename, "special:halfwave") ) {
            rawsize = halfwave_size; rawdata = halfwave_data;
        }
        else if( strstr(filename, "special:impuls10") ) {
            rawsize = impuls10_size; rawdata = impuls10_data;
        }
        else if( strstr(filename, "special:impuls20") ) {
            rawsize = impuls20_size; rawdata = impuls20_data;
        }
        else if( strstr(filename, "special:impuls40") ) {
            rawsize = impuls40_size; rawdata = impuls40_data;
        }
        else if( strstr(filename, "special:mand1") ) {
            rawsize = mand1_size; rawdata = mand1_data;
        }
        else if( strstr(filename, "special:mandpluk") ) {
            rawsize = mandpluk_size; rawdata = mandpluk_data;
        }
        else if( strstr(filename, "special:marmstk1") ) {
            rawsize = marmstk1_size; rawdata = marmstk1_data;
        }
        else if( strstr(filename, "special:ooo") ) {
            rawsize = ooo_size; rawdata = ooo_data;
        }
        else if( strstr(filename, "special:peksblnk") ) {
            rawsize = peksblnk_size; rawdata = peksblnk_data;
        }
        else if( strstr(filename, "special:ppksblnk") ) {
            rawsize = ppksblnk_size; rawdata = ppksblnk_data;
        }
        else if( strstr(filename, "special:silence") ) {
            rawsize = silence_size; rawdata = silence_data;
        }
        else if( strstr(filename, "special:sineblnk") ) {
            rawsize = sineblnk_size; rawdata = sineblnk_data;
        }
        else if( strstr(filename, "special:sinewave") ) {
            rawsize = sinewave_size; rawdata = sinewave_data;
        }
        else if( strstr(filename, "special:snglpeak") ) {
            rawsize = snglpeak_size; rawdata = snglpeak_data;
        }
        else if( strstr(filename, "special:twopeaks") ) {
            rawsize = twopeaks_size; rawdata = twopeaks_data;
        }
        else if( strstr(filename, "special:glot_ahh") ) {
            rawsize = glot_ahh_size; rawdata = glot_ahh_data; srate = 44100;
        }
        else if( strstr(filename, "special:glot_eee") ) {
            rawsize = glot_eee_size; rawdata = glot_eee_data; srate = 44100;
        }
        else if( strstr(filename, "special:glot_ooo") ) {
            rawsize = glot_ooo_size; rawdata = glot_ooo_data; srate = 44100;
        }
        else if( strstr(filename, "special:glot_pop") ) {
            rawsize = glot_pop_size; rawdata = glot_pop_data; srate = 44100;
        }

        d->num_frames = rawsize;
        d->num_channels = 1;
        d->chan = 0;
        d->samplerate = srate;
        d->num_samples = rawsize;

        // no chunking
        d->chunks_read = d->num_frames;

        if( rawdata ) {
            d->buffer = new SAMPLE[rawsize+1];
            for( t_CKUINT j = 0; j < rawsize; j++ ) {
                d->buffer[j] = (SAMPLE)rawdata[j]/(SAMPLE)SHRT_MAX;
            }
        }
        else if( strstr(filename, "special:sinewave") ) {
            d->buffer = new SAMPLE[rawsize+1];
            for( t_CKUINT j = 0; j < rawsize; j++ )
                d->buffer[j] = sin(2*ONE_PI*j/rawsize);
        }
        else {
            fprintf( stderr, "[chuck](via sndbuf): cannot load '%s'\n", filename );
            return;
        }

        d->buffer[rawsize] = d->buffer[0];
    }
    else // read file
    {
        // stat the file first
        struct stat s;
        if( stat( filename, &s ) )
        {
            fprintf( stderr, "[chuck](via sndbuf): cannot stat file '%s'...\n", filename );
            return;
        }

        // open it
        SF_INFO info;
        info.format = 0;
        char * format = strrchr( filename, '.');
        if( format && strcmp( format, ".raw" ) == 0 )
        { 
            fprintf( stderr, "[chuck](via sndbuf) %s :: type is '.raw'...\n    assuming 16 bit signed mono (PCM)\n", filename );
            info.format = SF_FORMAT_RAW | SF_FORMAT_PCM_16 | SF_ENDIAN_CPU ;
            info.channels = 1;
            info.samplerate = 44100;
        }

        // open the handle
        d->fd = sf_open( filename, SFM_READ, &info );
        t_CKINT er = sf_error( d->fd );
        if( er )
        {
            fprintf( stderr, "[chuck](via sndbuf): sndfile error '%i' opening '%s'...\n", er, filename );
            fprintf( stderr, "[chuck](via sndbuf): (reason: %s)\n", sf_strerror( d->fd ) );
            if( d->fd ) sf_close( d->fd );
            // escape
            return;
        }

        // allocate
        t_CKINT size = info.channels * info.frames;
        d->buffer = new SAMPLE[size+info.channels];
        memset( d->buffer, 0, (size+info.channels)*sizeof(SAMPLE) );
        d->chan = 0;
        d->num_frames = info.frames;
        d->num_channels = info.channels;
        d->samplerate = info.samplerate;
        d->num_samples = size;

        // log
        EM_pushlog();
        EM_log( CK_LOG_INFO, "channels: %d", d->num_channels );
        EM_log( CK_LOG_INFO, "frames: %d", d->num_frames );
        EM_log( CK_LOG_INFO, "srate: %d", d->samplerate );
        EM_log( CK_LOG_INFO, "chunks: %d", d->chunks );
        EM_poplog();

        // read
        sf_seek( d->fd, 0, SEEK_SET );
        d->chunks_read = 0;

        // no chunk
        if( !d->chunks )
        {
            // read all
            t_CKUINT f = sndbuf_read( d, 0, d->num_frames );
            // check
            if( f != (t_CKUINT)d->num_frames )
            {
                fprintf( stderr, "[chuck](via sndbuf): read %d rather than %d frames from %s\n",
                         f, size, filename );
                sf_close( d->fd ); d->fd = NULL;
                return;
            }

            assert( d->fd == NULL );
        }
        else
        {
            // reset
            d->chunks_size = d->chunks;
            d->chunks_total = d->num_frames / d->chunks;
            d->chunks_total += d->num_frames % d->chunks ? 1 : 0;
            d->chunks_read = 0;
            d->chunk_table = new bool[d->chunks_total];
            memset( d->chunk_table, 0, d->chunks_total * sizeof(bool) );

            // read chunk
            // sndbuf_load( d, 0 );
        }
    }

    // d->interp = SNDBUF_INTERP;
    d->sampleratio = (double)d->samplerate / (double)g_srate;
    // set the rate
    d->rate = d->sampleratio * d->rate_factor;
    d->curr = d->buffer;
    d->curf = 0;
    d->eob = d->buffer + d->num_samples;
}

CK_DLL_CTRL( sndbuf_ctrl_write )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    const char * filename = GET_CK_STRING(ARGS)->str.c_str();
    
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


CK_DLL_CTRL( sndbuf_ctrl_rate )
{
    sndbuf_data * d = ( sndbuf_data * ) OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    t_CKFLOAT rate = GET_CK_FLOAT(ARGS); // rate
    d->rate = rate * d->sampleratio;
    d->rate_factor = rate;
    RETURN->v_float = d->rate_factor; // TODO: (or not TODO:)
}

CK_DLL_CGET( sndbuf_cget_rate )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    RETURN->v_float = d->rate_factor;
}


CK_DLL_CTRL( sndbuf_ctrl_freq )
{ 
    sndbuf_data * d = ( sndbuf_data * ) OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    t_CKFLOAT freq = GET_CK_FLOAT(ARGS);  //hz
    
    d->rate = ( freq * (double) d->num_frames / (double) g_srate );
    d->rate_factor = d->rate / d->sampleratio;
    RETURN->v_float = d->rate * (t_CKFLOAT) g_srate / ( (t_CKFLOAT) d->num_frames ); // TODO: really?
}

CK_DLL_CGET( sndbuf_cget_freq )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    RETURN->v_float = d->rate * (t_CKFLOAT) g_srate / ( (t_CKFLOAT) d->num_frames ); 
}

CK_DLL_CTRL( sndbuf_ctrl_phase )
{ 
    sndbuf_data * d = ( sndbuf_data * ) OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    t_CKFLOAT phase = GET_CK_FLOAT(ARGS);
    sndbuf_setpos(d, phase * (double)d->num_frames);
    RETURN->v_float = (t_CKFLOAT) d->curf / (t_CKFLOAT)d->num_frames; // TODO: 
}

CK_DLL_CGET( sndbuf_cget_phase )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    //SET_NEXT_FLOAT( out, (t_CKFLOAT) d->curf / (t_CKFLOAT)d->num_frames );
    RETURN->v_float = (t_CKFLOAT) d->curf / (t_CKFLOAT)d->num_frames;
}

CK_DLL_CTRL( sndbuf_ctrl_channel )
{ 
    sndbuf_data * d = ( sndbuf_data * ) OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    unsigned long chan = (unsigned long)GET_CK_INT(ARGS);
    if ( chan >= 0 && chan < d->num_channels ) { 
        d->chan = chan;
    }
    RETURN->v_int = (t_CKINT)d->chan;
}

CK_DLL_CGET( sndbuf_cget_channel )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    //SET_NEXT_INT( out, d->chan );
    RETURN->v_int = (t_CKINT)d->chan;
}

CK_DLL_CTRL( sndbuf_ctrl_pos )
{ 
    sndbuf_data * d = ( sndbuf_data * ) OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    t_CKINT pos = GET_CK_INT(ARGS);
    sndbuf_setpos(d, pos);
    RETURN->v_int = (t_CKINT)sndbuf_getpos(d); // TODO TODO TODOOO
}

CK_DLL_CGET( sndbuf_cget_pos )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    RETURN->v_int = (t_CKINT)sndbuf_getpos(d); 
}

CK_DLL_CTRL( sndbuf_ctrl_interp )
{ 
    sndbuf_data * d = ( sndbuf_data * ) OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    t_CKINT interp = GET_CK_INT(ARGS);
    d->interp = interp;
    RETURN->v_int = d->interp;
}

CK_DLL_CGET( sndbuf_cget_interp )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    RETURN->v_int = d->interp;
}

CK_DLL_CTRL( sndbuf_ctrl_chunks )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    t_CKINT frames = GET_NEXT_INT(ARGS);
    d->chunks = frames >= 0 ? frames : 0;
    RETURN->v_int = d->chunks;
}

CK_DLL_CGET( sndbuf_cget_chunks )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    RETURN->v_int = d->chunks;
}

CK_DLL_CTRL( sndbuf_ctrl_phase_offset )
{ 
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    t_CKFLOAT phase_offset = GET_CK_FLOAT(ARGS);
    sndbuf_setpos(d, d->curf + phase_offset * (t_CKFLOAT)d->num_frames );
}

CK_DLL_CGET( sndbuf_cget_samples )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    //SET_NEXT_INT( out, d->num_frames );
    RETURN->v_int = d->num_frames;
}

CK_DLL_CGET( sndbuf_cget_length )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    //SET_NEXT_DUR( out, (t_CKDUR)d->num_frames );
    RETURN->v_dur = (t_CKDUR)d->num_frames / d->sampleratio;
}

CK_DLL_CGET( sndbuf_cget_channels )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    //SET_NEXT_INT( out, d->num_channels );
    RETURN->v_int = d->num_channels;
}

CK_DLL_CGET( sndbuf_cget_valueAt )
{
    sndbuf_data * d = (sndbuf_data *)OBJ_MEMBER_UINT( SELF, sndbuf_offset_data );
    t_CKINT i = GET_CK_INT(ARGS);
    if( d->fd ) sndbuf_load( d, i );
    RETURN->v_float = ( i > d->num_frames || i < 0 ) ? 0 : d->buffer[i];
}
