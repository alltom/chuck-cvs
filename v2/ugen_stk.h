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
// file: ugen_stk.h
// desc: ChucK import for Synthesis ToolKit (STK)
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Ananya Misra (amisra@cs.princeton.edu)
//         Ari Lazier (alazier@cs.princeton.edu)
//         Philip L. Davidson (philipd@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#ifndef __UGEN_STK_H__
#define __UGEN_STK_H__

#include "chuck_dl.h"


// query
DLL_QUERY stk_query( Chuck_DL_Query * QUERY );
t_CKBOOL  stk_detach( t_CKUINT type, void * data );

// ADSR
CK_DLL_CTOR( ADSR_ctor );
CK_DLL_DTOR( ADSR_dtor );
CK_DLL_TICK( ADSR_tick );
CK_DLL_PMSG( ADSR_pmsg );
CK_DLL_CTRL( ADSR_ctrl_attackTime );
CK_DLL_CTRL( ADSR_ctrl_attackRate );
CK_DLL_CTRL( ADSR_ctrl_decayTime );
CK_DLL_CTRL( ADSR_ctrl_decayRate );
CK_DLL_CTRL( ADSR_ctrl_sustainLevel );
CK_DLL_CTRL( ADSR_ctrl_releaseTime );
CK_DLL_CTRL( ADSR_ctrl_releaseRate );
CK_DLL_CTRL( ADSR_ctrl_set );
CK_DLL_CTRL( ADSR_ctrl_set2 );
CK_DLL_CGET( ADSR_cget_attackRate );
CK_DLL_CGET( ADSR_cget_decayRate );
CK_DLL_CGET( ADSR_cget_sustainLevel );
CK_DLL_CGET( ADSR_cget_releaseRate );
CK_DLL_CGET( ADSR_cget_state );

// BiQuad
CK_DLL_CTOR( BiQuad_ctor );
CK_DLL_DTOR( BiQuad_dtor );
CK_DLL_TICK( BiQuad_tick );
CK_DLL_PMSG( BiQuad_pmsg );
CK_DLL_CTRL( BiQuad_ctrl_b2 );
CK_DLL_CTRL( BiQuad_ctrl_b1 );
CK_DLL_CTRL( BiQuad_ctrl_b0 );
CK_DLL_CTRL( BiQuad_ctrl_a2 );
CK_DLL_CTRL( BiQuad_ctrl_a1 );
CK_DLL_CGET( BiQuad_ctrl_a0 );
CK_DLL_CTRL( BiQuad_ctrl_pfreq );
CK_DLL_CTRL( BiQuad_ctrl_prad );
CK_DLL_CTRL( BiQuad_ctrl_zfreq );
CK_DLL_CTRL( BiQuad_ctrl_zrad );
CK_DLL_CTRL( BiQuad_ctrl_norm );
CK_DLL_CTRL( BiQuad_ctrl_eqzs );
CK_DLL_CGET( BiQuad_cget_b2 );
CK_DLL_CGET( BiQuad_cget_b1 );
CK_DLL_CGET( BiQuad_cget_b0 );
CK_DLL_CGET( BiQuad_cget_a2 );
CK_DLL_CGET( BiQuad_cget_a1 );
CK_DLL_CGET( BiQuad_cget_a0 );
CK_DLL_CTRL( BiQuad_cget_pfreq );
CK_DLL_CTRL( BiQuad_cget_prad );
CK_DLL_CTRL( BiQuad_cget_zfreq );
CK_DLL_CTRL( BiQuad_cget_zrad );

// Chorus
CK_DLL_CTOR( Chorus_ctor );
CK_DLL_DTOR( Chorus_dtor );
CK_DLL_TICK( Chorus_tick );
CK_DLL_PMSG( Chorus_pmsg );
CK_DLL_CTRL( Chorus_ctrl_modDepth );
CK_DLL_CTRL( Chorus_ctrl_modFreq );
CK_DLL_CTRL( Chorus_ctrl_mix );
CK_DLL_CGET( Chorus_cget_modDepth );
CK_DLL_CGET( Chorus_cget_modFreq );
CK_DLL_CGET( Chorus_cget_mix );

// Delay
CK_DLL_CTOR( Delay_ctor );
CK_DLL_DTOR( Delay_dtor );
CK_DLL_TICK( Delay_tick );
CK_DLL_PMSG( Delay_pmsg );
CK_DLL_CTRL( Delay_ctrl_delay );
CK_DLL_CTRL( Delay_ctrl_max );
CK_DLL_CGET( Delay_cget_delay );
CK_DLL_CGET( Delay_cget_max );

// DelayA
CK_DLL_CTOR( DelayA_ctor );
CK_DLL_DTOR( DelayA_dtor );
CK_DLL_TICK( DelayA_tick );
CK_DLL_PMSG( DelayA_pmsg );
CK_DLL_CTRL( DelayA_ctrl_delay );
CK_DLL_CTRL( DelayA_ctrl_max );
CK_DLL_CGET( DelayA_cget_delay );
CK_DLL_CGET( DelayA_cget_max );

// DelayL
CK_DLL_CTOR( DelayL_ctor );
CK_DLL_DTOR( DelayL_dtor );
CK_DLL_TICK( DelayL_tick );
CK_DLL_PMSG( DelayL_pmsg );
CK_DLL_CTRL( DelayL_ctrl_delay );
CK_DLL_CTRL( DelayL_ctrl_max );
CK_DLL_CGET( DelayL_cget_delay );
CK_DLL_CGET( DelayL_cget_max );

// Echo
CK_DLL_CTOR( Echo_ctor );
CK_DLL_DTOR( Echo_dtor );
CK_DLL_TICK( Echo_tick );
CK_DLL_PMSG( Echo_pmsg );
CK_DLL_CTRL( Echo_ctrl_delay );
CK_DLL_CTRL( Echo_ctrl_max );
CK_DLL_CTRL( Echo_ctrl_mix );
CK_DLL_CGET( Echo_cget_delay );
CK_DLL_CGET( Echo_cget_max );
CK_DLL_CGET( Echo_cget_mix );

// Envelope
CK_DLL_CTOR( Envelope_ctor );
CK_DLL_DTOR( Envelope_dtor );
CK_DLL_TICK( Envelope_tick );
CK_DLL_PMSG( Envelope_pmsg );
CK_DLL_CTRL( Envelope_ctrl_rate );
CK_DLL_CTRL( Envelope_ctrl_target );
CK_DLL_CTRL( Envelope_cget_target );
CK_DLL_CTRL( Envelope_ctrl_time );
CK_DLL_CTRL( Envelope_ctrl_duration );
CK_DLL_CTRL( Envelope_ctrl_value );
CK_DLL_CTRL( Envelope_cget_value );
CK_DLL_CTRL( Envelope_ctrl_keyOn0 );
CK_DLL_CTRL( Envelope_ctrl_keyOn );
CK_DLL_CTRL( Envelope_ctrl_keyOff0 );
CK_DLL_CTRL( Envelope_ctrl_keyOff );
CK_DLL_CGET( Envelope_cget_rate );
CK_DLL_CGET( Envelope_cget_target );
CK_DLL_CGET( Envelope_cget_time );
CK_DLL_CGET( Envelope_cget_duration );
CK_DLL_CGET( Envelope_cget_value );

// FilterStk (originally Filter)
CK_DLL_CTOR( FilterStk_ctor );
CK_DLL_DTOR( FilterStk_dtor );
CK_DLL_TICK( FilterStk_tick );
CK_DLL_PMSG( FilterStk_pmsg );
CK_DLL_CTRL( FilterStk_ctrl_coefs );
CK_DLL_CGET( FilterStk_cget_coefs );

// OnePole
CK_DLL_CTOR( OnePole_ctor );
CK_DLL_DTOR( OnePole_dtor );
CK_DLL_TICK( OnePole_tick );
CK_DLL_PMSG( OnePole_pmsg );
CK_DLL_CTRL( OnePole_ctrl_a1 );
CK_DLL_CTRL( OnePole_ctrl_b0 );
CK_DLL_CTRL( OnePole_ctrl_pole );
CK_DLL_CGET( OnePole_cget_a1 );
CK_DLL_CGET( OnePole_cget_b0 );
CK_DLL_CGET( OnePole_cget_pole );

// TwoPole
CK_DLL_CTOR( TwoPole_ctor );
CK_DLL_DTOR( TwoPole_dtor );
CK_DLL_TICK( TwoPole_tick );
CK_DLL_PMSG( TwoPole_pmsg );
CK_DLL_CTRL( TwoPole_ctrl_a1 );
CK_DLL_CTRL( TwoPole_ctrl_a2 );
CK_DLL_CTRL( TwoPole_ctrl_b0 );
CK_DLL_CTRL( TwoPole_ctrl_freq );
CK_DLL_CTRL( TwoPole_ctrl_radius );
CK_DLL_CTRL( TwoPole_ctrl_norm );
CK_DLL_CGET( TwoPole_cget_a1 );
CK_DLL_CGET( TwoPole_cget_a2 );
CK_DLL_CGET( TwoPole_cget_b0 );
CK_DLL_CGET( TwoPole_cget_freq );
CK_DLL_CGET( TwoPole_cget_radius );
CK_DLL_CGET( TwoPole_cget_norm );


// OneZero
CK_DLL_CTOR( OneZero_ctor );
CK_DLL_DTOR( OneZero_dtor );
CK_DLL_TICK( OneZero_tick );
CK_DLL_PMSG( OneZero_pmsg );
CK_DLL_CTRL( OneZero_ctrl_zero );
CK_DLL_CTRL( OneZero_ctrl_b0 );
CK_DLL_CTRL( OneZero_ctrl_b1 );
CK_DLL_CGET( OneZero_cget_zero );
CK_DLL_CGET( OneZero_cget_b0 );
CK_DLL_CGET( OneZero_cget_b1 );

// TwoZero
CK_DLL_CTOR( TwoZero_ctor );
CK_DLL_DTOR( TwoZero_dtor );
CK_DLL_TICK( TwoZero_tick );
CK_DLL_PMSG( TwoZero_pmsg );
CK_DLL_CTRL( TwoZero_ctrl_b0 );
CK_DLL_CTRL( TwoZero_ctrl_b1 );
CK_DLL_CTRL( TwoZero_ctrl_b2 );
CK_DLL_CTRL( TwoZero_ctrl_freq );
CK_DLL_CTRL( TwoZero_ctrl_radius );
CK_DLL_CGET( TwoZero_cget_b0 );
CK_DLL_CGET( TwoZero_cget_b1 );
CK_DLL_CGET( TwoZero_cget_b2 );
CK_DLL_CGET( TwoZero_cget_freq );
CK_DLL_CGET( TwoZero_cget_radius );

// PoleZero
CK_DLL_CTOR( PoleZero_ctor );
CK_DLL_DTOR( PoleZero_dtor );
CK_DLL_TICK( PoleZero_tick );
CK_DLL_PMSG( PoleZero_pmsg );
CK_DLL_CTRL( PoleZero_ctrl_a1 );
CK_DLL_CTRL( PoleZero_ctrl_b0 );
CK_DLL_CTRL( PoleZero_ctrl_b1 );
CK_DLL_CTRL( PoleZero_ctrl_blockZero );
CK_DLL_CTRL( PoleZero_ctrl_allpass );
CK_DLL_CGET( PoleZero_cget_a1 );
CK_DLL_CGET( PoleZero_cget_b0 );
CK_DLL_CGET( PoleZero_cget_b1 );
CK_DLL_CGET( PoleZero_cget_blockZero );
CK_DLL_CGET( PoleZero_cget_allpass );

// Noise
CK_DLL_CTOR( Noise_ctor );
CK_DLL_DTOR( Noise_dtor );
CK_DLL_TICK( Noise_tick );
CK_DLL_PMSG( Noise_pmsg );
CK_DLL_CTRL( Noise_ctrl_seed );
CK_DLL_CGET( Noise_cget_seed );

// SubNoise
CK_DLL_CTOR( SubNoise_ctor );
CK_DLL_DTOR( SubNoise_dtor );
CK_DLL_TICK( SubNoise_tick );
CK_DLL_PMSG( SubNoise_pmsg );
CK_DLL_CTRL( SubNoise_ctrl_rate );
CK_DLL_CGET( SubNoise_cget_rate );

// BLT
CK_DLL_CTOR( BLT_ctor );
CK_DLL_DTOR( BLT_dtor );
CK_DLL_TICK( BLT_tick );
CK_DLL_PMSG( BLT_pmsg );
CK_DLL_CTRL( BLT_ctrl_phase );
CK_DLL_CGET( BLT_cget_phase );
CK_DLL_CTRL( BLT_ctrl_freq );
CK_DLL_CGET( BLT_cget_freq );
CK_DLL_CTRL( BLT_ctrl_harmonics );
CK_DLL_CGET( BLT_cget_harmonics );

// Blit
CK_DLL_CTOR( Blit_ctor );
CK_DLL_DTOR( Blit_dtor );
CK_DLL_TICK( Blit_tick );
CK_DLL_PMSG( Blit_pmsg );

// BlitSaw
CK_DLL_CTOR( BlitSaw_ctor );
CK_DLL_DTOR( BlitSaw_dtor );
CK_DLL_TICK( BlitSaw_tick );
CK_DLL_PMSG( BlitSaw_pmsg );

// BlitSquare
CK_DLL_CTOR( BlitSquare_ctor );
CK_DLL_DTOR( BlitSquare_dtor );
CK_DLL_TICK( BlitSquare_tick );
CK_DLL_PMSG( BlitSquare_pmsg );

// JCRev
CK_DLL_CTOR( JCRev_ctor );
CK_DLL_DTOR( JCRev_dtor );
CK_DLL_TICK( JCRev_tick );
CK_DLL_PMSG( JCRev_pmsg );
CK_DLL_CTRL( JCRev_ctrl_mix );
CK_DLL_CGET( JCRev_cget_mix );

// NRev
CK_DLL_CTOR( NRev_ctor );
CK_DLL_DTOR( NRev_dtor );
CK_DLL_TICK( NRev_tick );
CK_DLL_PMSG( NRev_pmsg );
CK_DLL_CTRL( NRev_ctrl_mix );
CK_DLL_CGET( NRev_cget_mix );

// PRCRev
CK_DLL_CTOR( PRCRev_ctor );
CK_DLL_DTOR( PRCRev_dtor );
CK_DLL_TICK( PRCRev_tick );
CK_DLL_PMSG( PRCRev_pmsg );
CK_DLL_CTRL( PRCRev_ctrl_mix );
CK_DLL_CGET( PRCRev_cget_mix );

// WaveLoop
CK_DLL_CTOR( WaveLoop_ctor );
CK_DLL_DTOR( WaveLoop_dtor );
CK_DLL_TICK( WaveLoop_tick );
CK_DLL_PMSG( WaveLoop_pmsg );
CK_DLL_CTRL( WaveLoop_ctrl_freq );
CK_DLL_CTRL( WaveLoop_ctrl_rate );
CK_DLL_CTRL( WaveLoop_ctrl_phase );
CK_DLL_CTRL( WaveLoop_ctrl_phaseOffset );
CK_DLL_CTRL( WaveLoop_ctrl_path );
CK_DLL_CGET( WaveLoop_cget_freq );
CK_DLL_CGET( WaveLoop_cget_rate );
CK_DLL_CGET( WaveLoop_cget_phase );
CK_DLL_CGET( WaveLoop_cget_phaseOffset );
CK_DLL_CGET( WaveLoop_cget_path );

// WvIn
CK_DLL_CTOR( WvIn_ctor );
CK_DLL_DTOR( WvIn_dtor );
CK_DLL_TICK( WvIn_tick );
CK_DLL_PMSG( WvIn_pmsg );
CK_DLL_CTRL( WvIn_ctrl_rate );
CK_DLL_CTRL( WvIn_ctrl_path );
CK_DLL_CGET( WvIn_cget_rate );
CK_DLL_CGET( WvIn_cget_path );

// WvOut
CK_DLL_CTOR( WvOut_ctor );
CK_DLL_DTOR( WvOut_dtor );
CK_DLL_TICK( WvOut_tick );
CK_DLL_PMSG( WvOut_pmsg );
CK_DLL_CTRL( WvOut_ctrl_filename );
CK_DLL_CTRL( WvOut_ctrl_matFilename );
CK_DLL_CTRL( WvOut_ctrl_sndFilename );
CK_DLL_CTRL( WvOut_ctrl_wavFilename );
CK_DLL_CTRL( WvOut_ctrl_rawFilename );
CK_DLL_CTRL( WvOut_ctrl_aifFilename );
CK_DLL_CTRL( WvOut_ctrl_closeFile );
CK_DLL_CTRL( WvOut_ctrl_record );
CK_DLL_CTRL( WvOut_ctrl_autoPrefix );
CK_DLL_CGET( WvOut_cget_filename );
CK_DLL_CGET( WvOut_cget_record );
CK_DLL_CGET( WvOut_cget_autoPrefix );

// FM
CK_DLL_CTOR( FM_ctor );
CK_DLL_DTOR( FM_dtor );
CK_DLL_TICK( FM_tick );
CK_DLL_PMSG( FM_pmsg );
CK_DLL_CTRL( FM_ctrl_freq );
CK_DLL_CTRL( FM_ctrl_noteOn );
CK_DLL_CTRL( FM_ctrl_noteOff );
CK_DLL_CTRL( FM_ctrl_modDepth );
CK_DLL_CTRL( FM_ctrl_modSpeed );
CK_DLL_CTRL( FM_ctrl_control1 );
CK_DLL_CTRL( FM_ctrl_control2 );
CK_DLL_CTRL( FM_ctrl_controlChange );
CK_DLL_CTRL( FM_ctrl_afterTouch );
CK_DLL_CGET( FM_cget_freq );
CK_DLL_CGET( FM_cget_modDepth );
CK_DLL_CGET( FM_cget_modSpeed );
CK_DLL_CGET( FM_cget_control1 );
CK_DLL_CGET( FM_cget_control2 );
CK_DLL_CGET( FM_cget_afterTouch );

// FormSwep
CK_DLL_CTOR( FormSwep_ctor );
CK_DLL_DTOR( FormSwep_dtor );
CK_DLL_TICK( FormSwep_tick );
CK_DLL_PMSG( FormSwep_pmsg );
CK_DLL_CTRL( FormSwep_ctrl_frequency );
CK_DLL_CGET( FormSwep_cget_frequency );
CK_DLL_CTRL( FormSwep_ctrl_radius );
CK_DLL_CGET( FormSwep_cget_radius );
CK_DLL_CTRL( FormSwep_ctrl_gain );
CK_DLL_CGET( FormSwep_cget_gain );
CK_DLL_CTRL( FormSwep_ctrl_sweepRate );
CK_DLL_CTRL( FormSwep_ctrl_sweepTime );
CK_DLL_CGET( FormSwep_cget_sweepRate );
CK_DLL_CGET( FormSwep_cget_sweepTime );

// Modulate
CK_DLL_CTOR( Modulate_ctor );
CK_DLL_DTOR( Modulate_dtor );
CK_DLL_TICK( Modulate_tick );
CK_DLL_PMSG( Modulate_pmsg );
CK_DLL_CTRL( Modulate_ctrl_vibratoGain );
CK_DLL_CTRL( Modulate_ctrl_vibratoRate );
CK_DLL_CTRL( Modulate_ctrl_randomGain );
CK_DLL_CGET( Modulate_cget_vibratoGain );
CK_DLL_CGET( Modulate_cget_vibratoRate );
CK_DLL_CGET( Modulate_cget_randomGain );

// PitShift
CK_DLL_CTOR( PitShift_ctor );
CK_DLL_DTOR( PitShift_dtor );
CK_DLL_TICK( PitShift_tick );
CK_DLL_PMSG( PitShift_pmsg );
CK_DLL_CTRL( PitShift_ctrl_shift );
CK_DLL_CTRL( PitShift_ctrl_effectMix );
CK_DLL_CGET( PitShift_cget_shift );
CK_DLL_CGET( PitShift_cget_effectMix );

// Sampler
CK_DLL_CTOR( Sampler_ctor );
CK_DLL_DTOR( Sampler_dtor );
CK_DLL_TICK( Sampler_tick );
CK_DLL_PMSG( Sampler_pmsg );

// Drummer
CK_DLL_CTOR( Drummer_ctor );
CK_DLL_DTOR( Drummer_dtor );
CK_DLL_TICK( Drummer_tick );
CK_DLL_PMSG( Drummer_pmsg );

// Instrmnt
CK_DLL_CTOR( Instrmnt_ctor );
CK_DLL_DTOR( Instrmnt_dtor );
CK_DLL_TICK( Instrmnt_tick );
CK_DLL_PMSG( Instrmnt_pmsg );
CK_DLL_CTRL( Instrmnt_ctrl_freq );
CK_DLL_CGET( Instrmnt_cget_freq );
CK_DLL_CTRL( Instrmnt_ctrl_noteOn );
CK_DLL_CTRL( Instrmnt_ctrl_noteOff );
CK_DLL_CTRL( Instrmnt_ctrl_controlChange );

// BandedWG
CK_DLL_CTOR( BandedWG_ctor );
CK_DLL_DTOR( BandedWG_dtor );
CK_DLL_TICK( BandedWG_tick );
CK_DLL_PMSG( BandedWG_pmsg );
CK_DLL_CTRL( BandedWG_ctrl_bowPressure );
CK_DLL_CGET( BandedWG_cget_bowPressure );
CK_DLL_CTRL( BandedWG_ctrl_bowMotion );
CK_DLL_CGET( BandedWG_cget_bowMotion );
CK_DLL_CTRL( BandedWG_ctrl_strikePosition );
CK_DLL_CGET( BandedWG_cget_strikePosition );
CK_DLL_CTRL( BandedWG_ctrl_vibratoFreq );
CK_DLL_CGET( BandedWG_cget_vibratoFreq );
CK_DLL_CTRL( BandedWG_ctrl_modesGain );
CK_DLL_CGET( BandedWG_cget_modesGain );
CK_DLL_CTRL( BandedWG_ctrl_bowRate );
CK_DLL_CGET( BandedWG_cget_bowRate );
CK_DLL_CTRL( BandedWG_ctrl_preset );
CK_DLL_CGET( BandedWG_cget_preset );
CK_DLL_CTRL( BandedWG_ctrl_startBowing );
CK_DLL_CTRL( BandedWG_ctrl_stopBowing );
CK_DLL_CTRL( BandedWG_ctrl_pluck );
CK_DLL_CTRL( BandedWG_ctrl_noteOn );
CK_DLL_CTRL( BandedWG_ctrl_noteOff );
CK_DLL_CTRL( BandedWG_ctrl_freq );
CK_DLL_CGET( BandedWG_cget_freq );
CK_DLL_CTRL( BandedWG_ctrl_controlChange );
/*
CK_DLL_CTRL( BandedWG_ctrl_aftertouch );
CK_DLL_CTRL( BandedWG_ctrl_bowTarget );
CK_DLL_CTRL( BandedWG_ctrl_sustain );
CK_DLL_CTRL( BandedWG_ctrl_trackVelocity );
CK_DLL_CTRL( BandedWG_ctrl_portamento );
*/

// BeeThree
CK_DLL_CTOR( BeeThree_ctor );
CK_DLL_DTOR( BeeThree_dtor );
CK_DLL_TICK( BeeThree_tick );
CK_DLL_PMSG( BeeThree_pmsg );
CK_DLL_CTRL( BeeThree_ctrl_noteOn );

// BlowBotl
CK_DLL_CTOR( BlowBotl_ctor );
CK_DLL_DTOR( BlowBotl_dtor );
CK_DLL_TICK( BlowBotl_tick );
CK_DLL_PMSG( BlowBotl_pmsg );
CK_DLL_CTRL( BlowBotl_ctrl_freq );
CK_DLL_CGET( BlowBotl_cget_freq );
CK_DLL_CTRL( BlowBotl_ctrl_noiseGain );
CK_DLL_CGET( BlowBotl_cget_noiseGain );
CK_DLL_CTRL( BlowBotl_ctrl_vibratoFreq );
CK_DLL_CGET( BlowBotl_cget_vibratoFreq );
CK_DLL_CTRL( BlowBotl_ctrl_vibratoGain );
CK_DLL_CGET( BlowBotl_cget_vibratoGain );
CK_DLL_CTRL( BlowBotl_ctrl_volume );
CK_DLL_CGET( BlowBotl_cget_volume );
CK_DLL_CTRL( BlowBotl_ctrl_noteOn );
CK_DLL_CTRL( BlowBotl_ctrl_noteOff );
CK_DLL_CTRL( BlowBotl_ctrl_controlChange );
CK_DLL_CTRL( BlowBotl_ctrl_startBlowing );
CK_DLL_CTRL( BlowBotl_ctrl_stopBlowing );
CK_DLL_CTRL( BlowBotl_ctrl_rate );
CK_DLL_CGET( BlowBotl_cget_rate );

// BlowHole
CK_DLL_CTOR( BlowHole_ctor );
CK_DLL_DTOR( BlowHole_dtor );
CK_DLL_TICK( BlowHole_tick );
CK_DLL_PMSG( BlowHole_pmsg );
CK_DLL_CTRL( BlowHole_ctrl_freq );
CK_DLL_CGET( BlowHole_cget_freq );
CK_DLL_CTRL( BlowHole_ctrl_reed );
CK_DLL_CGET( BlowHole_cget_reed );
CK_DLL_CTRL( BlowHole_ctrl_noiseGain );
CK_DLL_CGET( BlowHole_cget_noiseGain );
CK_DLL_CTRL( BlowHole_ctrl_tonehole );
CK_DLL_CGET( BlowHole_cget_tonehole );
CK_DLL_CTRL( BlowHole_ctrl_vent );
CK_DLL_CGET( BlowHole_cget_vent );
CK_DLL_CTRL( BlowHole_ctrl_pressure );
CK_DLL_CGET( BlowHole_cget_pressure );
CK_DLL_CTRL( BlowHole_ctrl_noteOn );
CK_DLL_CTRL( BlowHole_ctrl_noteOff );
CK_DLL_CTRL( BlowHole_ctrl_controlChange );
CK_DLL_CTRL( BlowHole_ctrl_startBlowing );
CK_DLL_CTRL( BlowHole_ctrl_stopBlowing );
CK_DLL_CTRL( BlowHole_ctrl_rate );
CK_DLL_CGET( BlowHole_cget_rate );

// Bowed
CK_DLL_CTOR( Bowed_ctor );
CK_DLL_DTOR( Bowed_dtor );
CK_DLL_TICK( Bowed_tick );
CK_DLL_PMSG( Bowed_pmsg );
CK_DLL_CTRL( Bowed_ctrl_freq );
CK_DLL_CGET( Bowed_cget_freq );
CK_DLL_CTRL( Bowed_ctrl_bowPressure );
CK_DLL_CGET( Bowed_cget_bowPressure );
CK_DLL_CTRL( Bowed_ctrl_bowPos );
CK_DLL_CGET( Bowed_cget_bowPos );
CK_DLL_CTRL( Bowed_ctrl_vibratoFreq );
CK_DLL_CGET( Bowed_cget_vibratoFreq );
CK_DLL_CTRL( Bowed_ctrl_vibratoGain );
CK_DLL_CGET( Bowed_cget_vibratoGain );
CK_DLL_CTRL( Bowed_ctrl_volume );
CK_DLL_CGET( Bowed_cget_volume );
CK_DLL_CTRL( Bowed_ctrl_noteOn );
CK_DLL_CTRL( Bowed_ctrl_noteOff );
CK_DLL_CTRL( Bowed_ctrl_controlChange );
CK_DLL_CTRL( Bowed_ctrl_startBowing );
CK_DLL_CTRL( Bowed_ctrl_stopBowing );
CK_DLL_CTRL( Bowed_ctrl_rate );
CK_DLL_CGET( Bowed_cget_rate );

// BowTabl
CK_DLL_CTOR( BowTabl_ctor );
CK_DLL_DTOR( BowTabl_dtor );
CK_DLL_TICK( BowTabl_tick );
CK_DLL_PMSG( BowTabl_pmsg );
CK_DLL_CTRL( BowTabl_ctrl_offset );
CK_DLL_CTRL( BowTabl_ctrl_slope );

// Brass
CK_DLL_CTOR( Brass_ctor );
CK_DLL_DTOR( Brass_dtor );
CK_DLL_TICK( Brass_tick );
CK_DLL_PMSG( Brass_pmsg );
CK_DLL_CTRL( Brass_ctrl_freq );
CK_DLL_CGET( Brass_cget_freq );
CK_DLL_CTRL( Brass_ctrl_lip );
CK_DLL_CGET( Brass_cget_lip );
CK_DLL_CTRL( Brass_ctrl_slide );
CK_DLL_CGET( Brass_cget_slide );
CK_DLL_CTRL( Brass_ctrl_vibratoFreq );
CK_DLL_CGET( Brass_cget_vibratoFreq );
CK_DLL_CTRL( Brass_ctrl_vibratoGain );
CK_DLL_CGET( Brass_cget_vibratoGain );
CK_DLL_CTRL( Brass_ctrl_volume );
CK_DLL_CGET( Brass_cget_volume );
CK_DLL_CTRL( Brass_ctrl_clear );
CK_DLL_CTRL( Brass_ctrl_noteOn );
CK_DLL_CTRL( Brass_ctrl_noteOff );
CK_DLL_CTRL( Brass_ctrl_controlChange );
CK_DLL_CTRL( Brass_ctrl_startBlowing );
CK_DLL_CTRL( Brass_ctrl_stopBlowing );
CK_DLL_CTRL( Brass_ctrl_rate );
CK_DLL_CGET( Brass_cget_rate );

// Clarinet
CK_DLL_CTOR( Clarinet_ctor );
CK_DLL_DTOR( Clarinet_dtor );
CK_DLL_TICK( Clarinet_tick );
CK_DLL_PMSG( Clarinet_pmsg );
CK_DLL_CTRL( Clarinet_ctrl_freq );
CK_DLL_CGET( Clarinet_cget_freq );
CK_DLL_CTRL( Clarinet_ctrl_reed );
CK_DLL_CGET( Clarinet_cget_reed );
CK_DLL_CTRL( Clarinet_ctrl_noiseGain );
CK_DLL_CGET( Clarinet_cget_noiseGain );
CK_DLL_CTRL( Clarinet_ctrl_vibratoFreq );
CK_DLL_CGET( Clarinet_cget_vibratoFreq );
CK_DLL_CTRL( Clarinet_ctrl_vibratoGain );
CK_DLL_CGET( Clarinet_cget_vibratoGain );
CK_DLL_CTRL( Clarinet_ctrl_pressure );
CK_DLL_CGET( Clarinet_cget_pressure );
CK_DLL_CTRL( Clarinet_ctrl_clear );
CK_DLL_CTRL( Clarinet_ctrl_noteOn );
CK_DLL_CTRL( Clarinet_ctrl_noteOff );
CK_DLL_CTRL( Clarinet_ctrl_startBlowing );
CK_DLL_CTRL( Clarinet_ctrl_stopBlowing );
CK_DLL_CTRL( Clarinet_ctrl_rate );
CK_DLL_CGET( Clarinet_cget_rate );
CK_DLL_CTRL( Clarinet_ctrl_controlChange );

// Flute
CK_DLL_CTOR( Flute_ctor );
CK_DLL_DTOR( Flute_dtor );
CK_DLL_TICK( Flute_tick );
CK_DLL_PMSG( Flute_pmsg );
CK_DLL_CTRL( Flute_ctrl_freq );
CK_DLL_CGET( Flute_cget_freq );
CK_DLL_CTRL( Flute_ctrl_jetDelay );
CK_DLL_CGET( Flute_cget_jetDelay );
CK_DLL_CTRL( Flute_ctrl_jetReflection );
CK_DLL_CGET( Flute_cget_jetReflection );
CK_DLL_CTRL( Flute_ctrl_endReflection );
CK_DLL_CGET( Flute_cget_endReflection );
CK_DLL_CTRL( Flute_ctrl_noiseGain );
CK_DLL_CGET( Flute_cget_noiseGain );
CK_DLL_CTRL( Flute_ctrl_vibratoFreq );
CK_DLL_CGET( Flute_cget_vibratoFreq );
CK_DLL_CTRL( Flute_ctrl_vibratoGain );
CK_DLL_CGET( Flute_cget_vibratoGain );
CK_DLL_CTRL( Flute_ctrl_pressure );
CK_DLL_CGET( Flute_cget_pressure );
CK_DLL_CTRL( Flute_ctrl_clear );
CK_DLL_CTRL( Flute_ctrl_noteOn );
CK_DLL_CTRL( Flute_ctrl_noteOff );
CK_DLL_CTRL( Flute_ctrl_controlChange );
CK_DLL_CTRL( Flute_ctrl_startBlowing );
CK_DLL_CTRL( Flute_ctrl_stopBlowing );
CK_DLL_CTRL( Flute_ctrl_rate );
CK_DLL_CGET( Flute_cget_rate );

// FMVoices
CK_DLL_CTOR( FMVoices_ctor );
CK_DLL_DTOR( FMVoices_dtor );
CK_DLL_TICK( FMVoices_tick );
CK_DLL_PMSG( FMVoices_pmsg );
CK_DLL_CTRL( FMVoices_ctrl_vowel );
CK_DLL_CTRL( FMVoices_cget_vowel );
CK_DLL_CTRL( FMVoices_ctrl_spectralTilt );
CK_DLL_CTRL( FMVoices_cget_spectralTilt );
CK_DLL_CTRL( FMVoices_ctrl_adsrTarget );
CK_DLL_CTRL( FMVoices_cget_adsrTarget );

// HevyMetl
CK_DLL_CTOR( HevyMetl_ctor );
CK_DLL_DTOR( HevyMetl_dtor );
CK_DLL_TICK( HevyMetl_tick );
CK_DLL_PMSG( HevyMetl_pmsg );

// JetTabl
CK_DLL_CTOR( JetTabl_ctor );
CK_DLL_DTOR( JetTabl_dtor );
CK_DLL_TICK( JetTabl_tick );
CK_DLL_PMSG( JetTabl_pmsg );

// Mandolin
CK_DLL_CTOR( Mandolin_ctor );
CK_DLL_DTOR( Mandolin_dtor );
CK_DLL_TICK( Mandolin_tick );
CK_DLL_PMSG( Mandolin_pmsg );
CK_DLL_CTRL( Mandolin_ctrl_freq );
CK_DLL_CGET( Mandolin_cget_freq );
CK_DLL_CTRL( Mandolin_ctrl_pluckPos );
CK_DLL_CGET( Mandolin_cget_pluckPos );
CK_DLL_CTRL( Mandolin_ctrl_bodySize );
CK_DLL_CGET( Mandolin_cget_bodySize );
CK_DLL_CTRL( Mandolin_ctrl_stringDamping );
CK_DLL_CGET( Mandolin_cget_stringDamping );
CK_DLL_CTRL( Mandolin_ctrl_stringDetune );
CK_DLL_CGET( Mandolin_cget_stringDetune );
CK_DLL_CTRL( Mandolin_ctrl_controlChange );
CK_DLL_CTRL( Mandolin_ctrl_afterTouch );
CK_DLL_CTRL( Mandolin_ctrl_pluck );
CK_DLL_CTRL( Mandolin_ctrl_noteOn );
CK_DLL_CTRL( Mandolin_ctrl_noteOff );
CK_DLL_CTRL( Mandolin_ctrl_bodyIR );
CK_DLL_CGET( Mandolin_cget_bodyIR );

// Modal
CK_DLL_CTOR( Modal_ctor );
CK_DLL_DTOR( Modal_dtor );
CK_DLL_TICK( Modal_tick );
CK_DLL_PMSG( Modal_pmsg );
CK_DLL_CTRL( Modal_ctrl_freq );
CK_DLL_CTRL( Modal_ctrl_masterGain );
CK_DLL_CTRL( Modal_ctrl_directGain );
CK_DLL_CTRL( Modal_ctrl_mode );
CK_DLL_CTRL( Modal_ctrl_modeRatio );
CK_DLL_CTRL( Modal_ctrl_modeRadius );
CK_DLL_CTRL( Modal_ctrl_modeGain );
CK_DLL_CTRL( Modal_ctrl_strike );
CK_DLL_CTRL( Modal_ctrl_damp );
CK_DLL_CTRL( Modal_ctrl_noteOn );
CK_DLL_CTRL( Modal_ctrl_noteOff );

// ModalBar
CK_DLL_CTOR( ModalBar_ctor );
CK_DLL_DTOR( ModalBar_dtor );
CK_DLL_TICK( ModalBar_tick );
CK_DLL_PMSG( ModalBar_pmsg );
CK_DLL_CTRL( ModalBar_ctrl_strike );
CK_DLL_CTRL( ModalBar_ctrl_damp );
CK_DLL_CTRL( ModalBar_ctrl_clear );
CK_DLL_CTRL( ModalBar_ctrl_noteOn );
CK_DLL_CTRL( ModalBar_ctrl_noteOff );
CK_DLL_CTRL( ModalBar_ctrl_stickHardness );
CK_DLL_CGET( ModalBar_cget_stickHardness );
CK_DLL_CTRL( ModalBar_ctrl_strikePosition );
CK_DLL_CGET( ModalBar_cget_strikePosition );
CK_DLL_CTRL( ModalBar_ctrl_vibratoGain );
CK_DLL_CGET( ModalBar_cget_vibratoGain );
CK_DLL_CTRL( ModalBar_ctrl_vibratoFreq );
CK_DLL_CGET( ModalBar_cget_vibratoFreq );
CK_DLL_CTRL( ModalBar_ctrl_preset );
CK_DLL_CGET( ModalBar_cget_preset );
CK_DLL_CTRL( ModalBar_ctrl_freq );
CK_DLL_CGET( ModalBar_cget_freq );
CK_DLL_CTRL( ModalBar_ctrl_directGain );
CK_DLL_CGET( ModalBar_cget_directGain );
CK_DLL_CTRL( ModalBar_ctrl_masterGain );
CK_DLL_CGET( ModalBar_cget_masterGain );
CK_DLL_CTRL( ModalBar_ctrl_mode );
CK_DLL_CGET( ModalBar_cget_mode );
CK_DLL_CTRL( ModalBar_ctrl_modeRatio );
CK_DLL_CGET( ModalBar_cget_modeRatio );
CK_DLL_CTRL( ModalBar_ctrl_modeRadius );
CK_DLL_CGET( ModalBar_cget_modeRadius );
CK_DLL_CTRL( ModalBar_ctrl_modeGain );
CK_DLL_CGET( ModalBar_cget_modeGain );
CK_DLL_CTRL( ModalBar_ctrl_volume );
CK_DLL_CGET( ModalBar_cget_volume );
CK_DLL_CTRL( ModalBar_ctrl_controlChange );


// Moog
CK_DLL_CTOR( Moog_ctor );
CK_DLL_DTOR( Moog_dtor );
CK_DLL_TICK( Moog_tick );
CK_DLL_PMSG( Moog_pmsg );
CK_DLL_CTRL( Moog_ctrl_freq );
CK_DLL_CTRL( Moog_ctrl_noteOn );
CK_DLL_CTRL( Moog_ctrl_modSpeed );
CK_DLL_CTRL( Moog_ctrl_modDepth );
CK_DLL_CTRL( Moog_ctrl_filterQ );
CK_DLL_CTRL( Moog_ctrl_filterSweepRate );
CK_DLL_CTRL( Moog_ctrl_afterTouch );
CK_DLL_CTRL( Moog_ctrl_vibratoFreq );
CK_DLL_CTRL( Moog_ctrl_vibratoGain );
CK_DLL_CTRL( Moog_ctrl_volume );

CK_DLL_CGET( Moog_cget_freq );
CK_DLL_CGET( Moog_cget_modSpeed );
CK_DLL_CGET( Moog_cget_modDepth );
CK_DLL_CGET( Moog_cget_filterQ );
CK_DLL_CGET( Moog_cget_filterSweepRate );
CK_DLL_CGET( Moog_cget_vibratoFreq );
CK_DLL_CGET( Moog_cget_vibratoGain );
CK_DLL_CGET( Moog_cget_volume );

CK_DLL_CTRL( Moog_ctrl_controlChange );

// PercFlut
CK_DLL_CTOR( PercFlut_ctor );
CK_DLL_DTOR( PercFlut_dtor );
CK_DLL_TICK( PercFlut_tick );
CK_DLL_PMSG( PercFlut_pmsg );
CK_DLL_CTRL( PercFlut_ctrl_noteOn );
CK_DLL_CTRL( PercFlut_ctrl_freq );
CK_DLL_CGET( PercFlut_cget_freq );

// Plucked
CK_DLL_CTOR( Plucked_ctor );
CK_DLL_DTOR( Plucked_dtor );
CK_DLL_TICK( Plucked_tick );
CK_DLL_PMSG( Plucked_pmsg );
CK_DLL_CTRL( Plucked_ctrl_freq );
CK_DLL_CTRL( Plucked_ctrl_pluck );
CK_DLL_CTRL( Plucked_ctrl_noteOn );
CK_DLL_CTRL( Plucked_ctrl_noteOff );

// PluckTwo
CK_DLL_CTOR( PluckTwo_ctor );
CK_DLL_DTOR( PluckTwo_dtor );
CK_DLL_TICK( PluckTwo_tick );
CK_DLL_PMSG( PluckTwo_pmsg );
CK_DLL_CTRL( PluckTwo_ctrl_detune );
CK_DLL_CTRL( PluckTwo_ctrl_freq );
CK_DLL_CTRL( PluckTwo_ctrl_pluckPosition );
CK_DLL_CTRL( PluckTwo_ctrl_baseLoopGain );
CK_DLL_CTRL( PluckTwo_ctrl_pluck );
CK_DLL_CTRL( PluckTwo_ctrl_noteOn );
CK_DLL_CTRL( PluckTwo_ctrl_noteOff );

// ReedTabl
CK_DLL_CTOR( ReedTabl_ctor );
CK_DLL_DTOR( ReedTabl_dtor );
CK_DLL_TICK( ReedTabl_tick );
CK_DLL_PMSG( ReedTabl_pmsg );
CK_DLL_CTRL( ReedTabl_ctrl_offset );
CK_DLL_CTRL( ReedTabl_ctrl_slope );

// Resonate
CK_DLL_CTOR( Resonate_ctor );
CK_DLL_DTOR( Resonate_dtor );
CK_DLL_TICK( Resonate_tick );
CK_DLL_PMSG( Resonate_pmsg );

// Rhodey
CK_DLL_CTOR( Rhodey_ctor );
CK_DLL_DTOR( Rhodey_dtor );
CK_DLL_TICK( Rhodey_tick );
CK_DLL_PMSG( Rhodey_pmsg );
CK_DLL_CTRL( Rhodey_ctrl_freq );
CK_DLL_CTRL( Rhodey_ctrl_noteOn );
CK_DLL_CTRL( Rhodey_ctrl_noteOff );
CK_DLL_CTRL( Rhodey_cget_freq );

// Saxofony
CK_DLL_CTOR( Saxofony_ctor );
CK_DLL_DTOR( Saxofony_dtor );
CK_DLL_TICK( Saxofony_tick );
CK_DLL_PMSG( Saxofony_pmsg );
CK_DLL_CTRL( Saxofony_ctrl_freq );
CK_DLL_CGET( Saxofony_cget_freq );
CK_DLL_CTRL( Saxofony_ctrl_reed );
CK_DLL_CGET( Saxofony_cget_reed );
CK_DLL_CTRL( Saxofony_ctrl_aperture );
CK_DLL_CTRL( Saxofony_cget_aperture );
CK_DLL_CTRL( Saxofony_ctrl_noiseGain );
CK_DLL_CGET( Saxofony_cget_noiseGain );
CK_DLL_CTRL( Saxofony_ctrl_vibratoGain );
CK_DLL_CGET( Saxofony_cget_vibratoGain );
CK_DLL_CTRL( Saxofony_ctrl_vibratoFreq );
CK_DLL_CTRL( Saxofony_cget_vibratoFreq );
CK_DLL_CTRL( Saxofony_ctrl_blowPosition );
CK_DLL_CGET( Saxofony_cget_blowPosition );
CK_DLL_CTRL( Saxofony_ctrl_pressure );
CK_DLL_CGET( Saxofony_cget_pressure );
CK_DLL_CTRL( Saxofony_ctrl_controlChange );
CK_DLL_CTRL( Saxofony_ctrl_startBlowing );
CK_DLL_CTRL( Saxofony_ctrl_stopBlowing );
CK_DLL_CTRL( Saxofony_ctrl_clear );
CK_DLL_CTRL( Saxofony_ctrl_rate );
CK_DLL_CGET( Saxofony_cget_rate );
CK_DLL_CTRL( Saxofony_ctrl_noteOn );
CK_DLL_CTRL( Saxofony_ctrl_noteOff );

// Shakers
CK_DLL_CTOR( Shakers_ctor );
CK_DLL_DTOR( Shakers_dtor );
CK_DLL_TICK( Shakers_tick );
CK_DLL_PMSG( Shakers_pmsg );
CK_DLL_CTRL( Shakers_ctrl_energy );
CK_DLL_CGET( Shakers_cget_energy );
CK_DLL_CTRL( Shakers_ctrl_decay );
CK_DLL_CGET( Shakers_cget_decay );
CK_DLL_CTRL( Shakers_ctrl_objects );
CK_DLL_CGET( Shakers_cget_objects );
CK_DLL_CTRL( Shakers_ctrl_which );
CK_DLL_CGET( Shakers_cget_which );
CK_DLL_CTRL( Shakers_ctrl_noteOn );
CK_DLL_CTRL( Shakers_ctrl_noteOff );
CK_DLL_CTRL( Shakers_ctrl_freq );
CK_DLL_CGET( Shakers_cget_freq );
CK_DLL_CTRL( Shakers_ctrl_controlChange );

// Simple
CK_DLL_CTOR( Simple_ctor );
CK_DLL_DTOR( Simple_dtor );
CK_DLL_PMSG( Simple_pmsg );
CK_DLL_CTRL( Simple_ctrl_freq );

// SingWave
CK_DLL_CTOR( SingWave_ctor );
CK_DLL_DTOR( SingWave_dtor );
CK_DLL_TICK( SingWave_tick );
CK_DLL_PMSG( SingWave_pmsg );
CK_DLL_CTRL( SingWave_ctrl_wavFilename );
CK_DLL_CTRL( SingWave_ctrl_rawFilename );
CK_DLL_CTRL( SingWave_ctrl_norm );
CK_DLL_CTRL( SingWave_ctrl_sweepRate );
CK_DLL_CTRL( SingWave_ctrl_freq );
CK_DLL_CTRL( SingWave_ctrl_vibratoRate );
CK_DLL_CTRL( SingWave_ctrl_vibratoGain );
CK_DLL_CTRL( SingWave_ctrl_randomGain );
CK_DLL_CTRL( SingWave_ctrl_gainTarget );
CK_DLL_CTRL( SingWave_ctrl_gainRate );

// Sitar
CK_DLL_CTOR( Sitar_ctor );
CK_DLL_DTOR( Sitar_dtor );
CK_DLL_TICK( Sitar_tick );
CK_DLL_PMSG( Sitar_pmsg );
CK_DLL_CTRL( Sitar_ctrl_noteOn );
CK_DLL_CTRL( Sitar_ctrl_noteOff );
CK_DLL_CTRL( Sitar_ctrl_pluck );
CK_DLL_CTRL( Sitar_ctrl_clear );
CK_DLL_CTRL( Sitar_ctrl_freq );
CK_DLL_CTRL( Sitar_cget_freq );

// Sphere
CK_DLL_CTOR( Sphere_ctor );
CK_DLL_DTOR( Sphere_dtor );
CK_DLL_TICK( Sphere_tick );
CK_DLL_PMSG( Sphere_pmsg );
CK_DLL_CTRL( Sphere_ctrl_radius );
CK_DLL_CTRL( Sphere_ctrl_mass );

// StifKarp
CK_DLL_CTOR( StifKarp_ctor );
CK_DLL_DTOR( StifKarp_dtor );
CK_DLL_TICK( StifKarp_tick );
CK_DLL_PMSG( StifKarp_pmsg );
CK_DLL_CTRL( StifKarp_ctrl_pluck );
CK_DLL_CTRL( StifKarp_ctrl_clear );
CK_DLL_CTRL( StifKarp_ctrl_noteOn );
CK_DLL_CTRL( StifKarp_ctrl_noteOff );

CK_DLL_CTRL( StifKarp_ctrl_freq );
CK_DLL_CGET( StifKarp_cget_freq );
CK_DLL_CTRL( StifKarp_ctrl_pickupPosition );
CK_DLL_CGET( StifKarp_cget_pickupPosition );
CK_DLL_CTRL( StifKarp_ctrl_stretch );
CK_DLL_CGET( StifKarp_cget_stretch );
CK_DLL_CTRL( StifKarp_ctrl_sustain );
CK_DLL_CGET( StifKarp_cget_sustain );
CK_DLL_CTRL( StifKarp_ctrl_baseLoopGain );
CK_DLL_CGET( StifKarp_cget_baseLoopGain );

// TubeBell
CK_DLL_CTOR( TubeBell_ctor );
CK_DLL_DTOR( TubeBell_dtor );
CK_DLL_TICK( TubeBell_tick );
CK_DLL_PMSG( TubeBell_pmsg );
CK_DLL_CTRL( TubeBell_ctrl_noteOn );
CK_DLL_CTRL( TubeBell_ctrl_freq );
CK_DLL_CGET( TubeBell_cget_freq );

// Voicer
CK_DLL_CTOR( Voicer_ctor );
CK_DLL_DTOR( Voicer_dtor );
CK_DLL_TICK( Voicer_tick );
CK_DLL_PMSG( Voicer_pmsg );

// VoicForm
CK_DLL_CTOR( VoicForm_ctor );
CK_DLL_DTOR( VoicForm_dtor );
CK_DLL_TICK( VoicForm_tick );
CK_DLL_PMSG( VoicForm_pmsg );
CK_DLL_CTRL( VoicForm_ctrl_voiceMix );
CK_DLL_CGET( VoicForm_cget_voiceMix );
CK_DLL_CTRL( VoicForm_ctrl_phoneme );
CK_DLL_CGET( VoicForm_cget_phoneme );
CK_DLL_CTRL( VoicForm_ctrl_vibratoFreq );
CK_DLL_CGET( VoicForm_cget_vibratoFreq );
CK_DLL_CTRL( VoicForm_ctrl_vibratoGain );
CK_DLL_CGET( VoicForm_cget_vibratoGain );
CK_DLL_CTRL( VoicForm_ctrl_loudness );
CK_DLL_CGET( VoicForm_cget_loudness );
CK_DLL_CTRL( VoicForm_ctrl_freq );
CK_DLL_CGET( VoicForm_cget_freq );
CK_DLL_CTRL( VoicForm_ctrl_noteOn );
CK_DLL_CTRL( VoicForm_ctrl_noteOff );
CK_DLL_CTRL( VoicForm_ctrl_speak );
CK_DLL_CTRL( VoicForm_ctrl_quiet );
CK_DLL_CTRL( VoicForm_ctrl_voiced );
CK_DLL_CGET( VoicForm_cget_voiced );
CK_DLL_CTRL( VoicForm_ctrl_unVoiced );
CK_DLL_CGET( VoicForm_cget_unVoiced );
CK_DLL_CTRL( VoicForm_ctrl_pitchSweepRate );
CK_DLL_CGET( VoicForm_cget_pitchSweepRate );
CK_DLL_CTRL( VoicForm_ctrl_selPhoneme );
CK_DLL_CGET( VoicForm_cget_selPhoneme );

// Whistle
CK_DLL_CTOR( Whistle_ctor );
CK_DLL_DTOR( Whistle_dtor );
CK_DLL_TICK( Whistle_tick );
CK_DLL_PMSG( Whistle_pmsg );
CK_DLL_CTRL( Whistle_ctrl_freq );
CK_DLL_CTRL( Whistle_ctrl_startBlowing );
CK_DLL_CTRL( Whistle_ctrl_stopBlowing );
CK_DLL_CTRL( Whistle_ctrl_noteOn );
CK_DLL_CTRL( Whistle_ctrl_noteOff );

// Wurley
CK_DLL_CTOR( Wurley_ctor );
CK_DLL_DTOR( Wurley_dtor );
CK_DLL_TICK( Wurley_tick );
CK_DLL_PMSG( Wurley_pmsg );
CK_DLL_CTRL( Wurley_ctrl_freq );
CK_DLL_CTRL( Wurley_ctrl_noteOn );
CK_DLL_CTRL( Wurley_ctrl_noteOff );
CK_DLL_CGET( Wurley_cget_freq );



// Instrmnt
CK_DLL_CTOR( Instrmnt_ctor );
CK_DLL_DTOR( Instrmnt_dtor );
CK_DLL_TICK( Instrmnt_tick );
CK_DLL_PMSG( Instrmnt_pmsg );
CK_DLL_CTRL( Instrmnt_ctrl_freq );

// Mesh2D
CK_DLL_CTOR( Mesh2D_ctor );
CK_DLL_DTOR( Mesh2D_dtor );
CK_DLL_TICK( Mesh2D_tick );
CK_DLL_PMSG( Mesh2D_pmsg );
//CK_DLL_CTRL( Mesh2D_ctrl_NY );
//CK_DLL_CTRL( Mesh2D_ctrl_NY );
//CK_DLL_CTRL( Mesh2D_ctrl_NY );
//CK_DLL_CTRL( Mesh2D_ctrl_NX );
//CK_DLL_CTRL( Mesh2D_ctrl_NX );
//CK_DLL_CTRL( Mesh2D_ctrl_NX );
CK_DLL_CTRL( Mesh2D_ctrl_decay );

// Reverb
CK_DLL_CTOR( Reverb_ctor );
CK_DLL_DTOR( Reverb_dtor );
CK_DLL_TICK( Reverb_tick );
CK_DLL_PMSG( Reverb_pmsg );
CK_DLL_CTRL( Reverb_ctrl_effectMix );

// Socket
CK_DLL_CTOR( Socket_ctor );
CK_DLL_DTOR( Socket_dtor );
CK_DLL_TICK( Socket_tick );
CK_DLL_PMSG( Socket_pmsg );

// Stk
CK_DLL_CTOR( Stk_ctor );
CK_DLL_DTOR( Stk_dtor );
CK_DLL_TICK( Stk_tick );
CK_DLL_PMSG( Stk_pmsg );
CK_DLL_CTRL( Stk_ctrl_sampleRate );
CK_DLL_CTRL( Stk_ctrl_rawwavePath );

// Table
CK_DLL_CTOR( Table_ctor );
CK_DLL_DTOR( Table_dtor );
CK_DLL_TICK( Table_tick );
CK_DLL_PMSG( Table_pmsg );

// TcpWvIn
CK_DLL_CTOR( TcpWvIn_ctor );
CK_DLL_DTOR( TcpWvIn_dtor );
CK_DLL_TICK( TcpWvIn_tick );
CK_DLL_PMSG( TcpWvIn_pmsg );

// TcpWvOut
CK_DLL_CTOR( TcpWvOut_ctor );
CK_DLL_DTOR( TcpWvOut_dtor );
CK_DLL_TICK( TcpWvOut_tick );
CK_DLL_PMSG( TcpWvOut_pmsg );

// Vector3D
CK_DLL_CTOR( Vector3D_ctor );
CK_DLL_DTOR( Vector3D_dtor );
CK_DLL_TICK( Vector3D_tick );
CK_DLL_PMSG( Vector3D_pmsg );
CK_DLL_CTRL( Vector3D_ctrl_x );
CK_DLL_CTRL( Vector3D_ctrl_y );
CK_DLL_CTRL( Vector3D_ctrl_z );  

// JetTable
CK_DLL_CTOR( JetTabl_ctor );
CK_DLL_DTOR( JetTabl_dtor );
CK_DLL_TICK( JetTabl_tick );
CK_DLL_PMSG( JetTabl_pmsg );

// Mesh2D
CK_DLL_CTOR( Mesh2D_ctor );
CK_DLL_DTOR( Mesh2D_dtor );
CK_DLL_TICK( Mesh2D_tick );
CK_DLL_PMSG( Mesh2D_pmsg );
CK_DLL_CTRL( Mesh2D_ctrl_nx );
CK_DLL_CTRL( Mesh2D_ctrl_ny );
CK_DLL_CTRL( Mesh2D_cget_nx );
CK_DLL_CTRL( Mesh2D_cget_ny );
CK_DLL_CTRL( Mesh2D_ctrl_input_position );
CK_DLL_CTRL( Mesh2D_cget_input_position );
CK_DLL_CTRL( Mesh2D_ctrl_decay );
CK_DLL_CTRL( Mesh2D_cget_decay );
CK_DLL_CTRL( Mesh2D_ctrl_note_on );
CK_DLL_CTRL( Mesh2D_ctrl_note_off );
CK_DLL_CTRL( Mesh2D_cget_energy );
CK_DLL_CTRL( Mesh2D_ctrl_control_change );
// SKINI
/*
SkiniIn SkiniMessage => while(skin.getnextmessage(msg))
msg - type, data parameters, time stamp
SkiniOut.send( midi message )
horn ok please -> written at the back of trucks, always, makes no sense.
*/




#endif
