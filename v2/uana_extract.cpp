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
// file: uana_extract.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Rebecca Fiebrink (fiebrink@cs.princeton.edu)
// date: 20 November 2007
//-----------------------------------------------------------------------------
#include "uana_extract.h"
#include "chuck_type.h"
#include "chuck_vm.h"
#include "chuck_instr.h"
#include "chuck_lang.h"
#include "chuck_errmsg.h"
#include "util_math.h"
#include "util_xforms.h"




// Centroid
CK_DLL_TICK( Centroid_tick );
CK_DLL_TOCK( Centroid_tock );
CK_DLL_PMSG( Centroid_pmsg );
CK_DLL_SFUN( Centroid_compute );

// Flux
CK_DLL_CTOR( Flux_ctor );
CK_DLL_DTOR( Flux_dtor );
CK_DLL_TICK( Flux_tick );
CK_DLL_TOCK( Flux_tock );
CK_DLL_PMSG( Flux_pmsg );
CK_DLL_MFUN( Flux_ctrl_reset );
CK_DLL_SFUN( Flux_compute );
CK_DLL_SFUN( Flux_compute2 );
// offset
static t_CKUINT Flux_offset_data = 0;

// RMS
CK_DLL_TICK( RMS_tick );
CK_DLL_TOCK( RMS_tock );
CK_DLL_PMSG( RMS_pmsg );
CK_DLL_SFUN( RMS_compute );

// RollOff
CK_DLL_CTOR( RollOff_ctor );
CK_DLL_DTOR( RollOff_dtor );
CK_DLL_TICK( RollOff_tick );
CK_DLL_TOCK( RollOff_tock );
CK_DLL_PMSG( RollOff_pmsg );
CK_DLL_SFUN( RollOff_compute );
CK_DLL_MFUN( RollOff_ctrl_percent );
CK_DLL_MFUN( RollOff_cget_percent );
// offset
static t_CKUINT RollOff_offset_percent = 0;

// Feature Collector
CK_DLL_TICK( FeatureCollector_tick);
CK_DLL_TOCK( FeatureCollector_tock);
CK_DLL_PMSG( FeatureCollector_pmsg);

// AutoCorr
CK_DLL_CTOR( AutoCorr_ctor );
CK_DLL_DTOR( AutoCorr_dtor );
CK_DLL_TICK( AutoCorr_tick );
CK_DLL_TOCK( AutoCorr_tock );
CK_DLL_PMSG( AutoCorr_pmsg );
CK_DLL_SFUN( AutoCorr_compute );
CK_DLL_MFUN( AutoCorr_ctrl_normalize );
CK_DLL_MFUN( AutoCorr_cget_normalize );
// offset
static t_CKUINT AutoCorr_offset_data = 0;

// XCorr
CK_DLL_CTOR( XCorr_ctor );
CK_DLL_DTOR( XCorr_dtor );
CK_DLL_TICK( XCorr_tick );
CK_DLL_TOCK( XCorr_tock );
CK_DLL_PMSG( XCorr_pmsg );
CK_DLL_SFUN( XCorr_compute );
CK_DLL_MFUN( XCorr_ctrl_normalize );
CK_DLL_MFUN( XCorr_cget_normalize );

// LPC
CK_DLL_CTOR( LPC_ctor );
CK_DLL_DTOR( LPC_dtor );
CK_DLL_TICK( LPC_tick );
CK_DLL_TOCK( LPC_tock );
CK_DLL_PMSG( LPC_pmsg );
CK_DLL_SFUN( LPC_compute );
CK_DLL_MFUN( LPC_ctrl_pitch );
CK_DLL_MFUN( LPC_cget_pitch );
CK_DLL_MFUN( LPC_ctrl_power );
CK_DLL_MFUN( LPC_cget_power );
CK_DLL_MFUN( LPC_ctrl_coefs );
CK_DLL_MFUN( LPC_cget_coefs );


// utility functions
void xcorr_fft( SAMPLE * f, t_CKINT fs, SAMPLE * g, t_CKINT gs, SAMPLE * buffer, t_CKINT bs );
void xcorr_normalize( SAMPLE * buffy, t_CKINT bs, SAMPLE * f, t_CKINT fs, SAMPLE * g, t_CKINT gs );




//-----------------------------------------------------------------------------
// name: extract_query()
// desc: ...
//-----------------------------------------------------------------------------
DLL_QUERY extract_query( Chuck_DL_Query * QUERY )
{
    Chuck_Env * env = Chuck_Env::instance();

    Chuck_DL_Func * func = NULL;

    //---------------------------------------------------------------------
    // init as base class: FeatureCollector
    //---------------------------------------------------------------------
    if( !type_engine_import_uana_begin( env, "FeatureCollector", "UAna", env->global(), 
                                        NULL, NULL,
                                        FeatureCollector_tick, FeatureCollector_tock, FeatureCollector_pmsg ) )
        return FALSE;

    // end the class import
    type_engine_import_class_end( env );

    //---------------------------------------------------------------------
    // init as base class: Centroid
    //---------------------------------------------------------------------
    if( !type_engine_import_uana_begin( env, "Centroid", "UAna", env->global(), 
                                        NULL, NULL,
                                        Centroid_tick, Centroid_tock, Centroid_pmsg ) )
        return FALSE;

    // compute
    func = make_new_sfun( "float", "compute", Centroid_compute );
    func->add_arg( "float[]", "input" );
    if( !type_engine_import_sfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );

    //---------------------------------------------------------------------
    // init as base class: Flux
    //---------------------------------------------------------------------
    if( !type_engine_import_uana_begin( env, "Flux", "UAna", env->global(), 
                                        Flux_ctor, Flux_dtor,
                                        Flux_tick, Flux_tock, Flux_pmsg ) )
        return FALSE;

    // data offset
    Flux_offset_data = type_engine_import_mvar( env, "int", "@Flux_data", FALSE );
    if( Flux_offset_data == CK_INVALID_OFFSET ) goto error;

    // compute
    func = make_new_mfun( "void", "reset", Flux_ctrl_reset );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // compute
    func = make_new_sfun( "float", "compute", Flux_compute );
    func->add_arg( "float[]", "lhs" );
    func->add_arg( "float[]", "rhs" );
    if( !type_engine_import_sfun( env, func ) ) goto error;

    // compute2
    func = make_new_sfun( "float", "compute", Flux_compute2 );
    func->add_arg( "float[]", "lhs" );
    func->add_arg( "float[]", "rhs" );
    func->add_arg( "float[]", "diff" );
    if( !type_engine_import_sfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );

    //---------------------------------------------------------------------
    // init as base class: RMS
    //---------------------------------------------------------------------
    if( !type_engine_import_uana_begin( env, "RMS", "UAna", env->global(), 
                                        NULL, NULL,
                                        RMS_tick, RMS_tock, RMS_pmsg ) )
        return FALSE;

    // compute
    func = make_new_sfun( "float", "compute", RMS_compute );
    func->add_arg( "float[]", "input" );
    if( !type_engine_import_sfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );

    //---------------------------------------------------------------------
    // init as base class: RollOff
    //---------------------------------------------------------------------
    if( !type_engine_import_uana_begin( env, "RollOff", "UAna", env->global(), 
                                        RollOff_ctor, RollOff_dtor,
                                        RollOff_tick, RollOff_tock, RollOff_pmsg ) )
        return FALSE;

    // data offset
    RollOff_offset_percent = type_engine_import_mvar( env, "float", "@RollOff_data", FALSE );
    if( RollOff_offset_percent == CK_INVALID_OFFSET ) goto error;

    // compute
    func = make_new_mfun( "float", "percent", RollOff_ctrl_percent );
    func->add_arg( "float", "percent" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // compute
    func = make_new_mfun( "float", "percent", RollOff_cget_percent );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // compute
    func = make_new_sfun( "float", "compute", RollOff_compute );
    func->add_arg( "float[]", "input" );
    func->add_arg( "float", "percent" );
    if( !type_engine_import_sfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );

    //---------------------------------------------------------------------
    // init as base class: AutoCorr
    //---------------------------------------------------------------------
    if( !type_engine_import_uana_begin( env, "AutoCorr", "UAna", env->global(), 
                                        AutoCorr_ctor, AutoCorr_dtor,
                                        AutoCorr_tick, AutoCorr_tock, AutoCorr_pmsg ) )
        return FALSE;

    // data offset
    AutoCorr_offset_data = type_engine_import_mvar( env, "float", "@AutoCorr_data", FALSE );
    if( AutoCorr_offset_data == CK_INVALID_OFFSET ) goto error;

    // normalize
    func = make_new_mfun( "int", "normalize", AutoCorr_ctrl_normalize );
    func->add_arg( "int", "flag" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // normalize
    func = make_new_mfun( "int", "normalize", AutoCorr_cget_normalize );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // compute
    func = make_new_sfun( "float[]", "compute", AutoCorr_compute );
    func->add_arg( "float[]", "input" );
    func->add_arg( "float[]", "output" );
    if( !type_engine_import_sfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );

    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );

    return FALSE;
}

CK_DLL_TICK( FeatureCollector_tick )
{
    // do nothing
    return TRUE;
}

// FeatureCollector_tock creates a flat vector from its upstream UAnae
// TODO: Stick complex features in Blob, too? But what about fft? (don't want duplication)
CK_DLL_TOCK( FeatureCollector_tock )
{
    //t_CKFLOAT * features; 
    t_CKINT num_feats = 0;
    t_CKINT num_incoming = UANA->numIncomingUAnae();
    t_CKINT i, j;
    
    
    // Get all incoming features and agglomerate into one vector
    if( num_incoming > 0 )
    {
        //count the number of features in the array we're making
        for( i = 0; i < num_incoming; i++ )
        {
            // get next blob
            Chuck_UAnaBlobProxy * BLOB_IN = UANA->getIncomingBlob( i );
            // sanity check
            assert( BLOB_IN != NULL );
            // count number of features from this UAna
            Chuck_Array8 & these_fvals = BLOB_IN->fvals();
            num_feats += these_fvals.size();
        }

        // get fvals of output BLOB
        Chuck_Array8 & fvals = BLOB->fvals();
        if( fvals.size() != num_feats )
            fvals.set_size( num_feats );

        t_CKINT next_index = 0;
        for( i = 0; i < num_incoming; i++ )
        {
            // get next blob
            Chuck_UAnaBlobProxy * BLOB_IN = UANA->getIncomingBlob( i );
            Chuck_Array8 & these_fvals = BLOB_IN->fvals();
            t_CKINT num_these = these_fvals.size();
            for( j = 0; j < num_these; j++ )
            {
                t_CKFLOAT v;
                these_fvals.get( j, &v );
                fvals.set( next_index, v);
                next_index++;
            }
        } 
    } else {
        // no input to collect
        BLOB->fvals().set_size(0);
    }

    return TRUE;
}

CK_DLL_PMSG( FeatureCollector_pmsg )
{
    // do nothing
    return TRUE;
}

static t_CKFLOAT compute_centroid( Chuck_Array8 & buffer, t_CKUINT size )
{
    t_CKFLOAT m0 = 0.0;
    t_CKFLOAT m1 = 0.0;
    t_CKFLOAT centroid;
    t_CKFLOAT v;
    t_CKUINT i;

    // Compute centroid using moments
    for( i = 0; i < size; i++ )
    {
        buffer.get( i, &v );
        m1 += (i * v);
        m0 += v;
    }

    if( m0 != 0.0 )
        centroid = m1 / m0;
    else 
        centroid = size / 2.0; // Perfectly balanced

    return centroid / size;
}


CK_DLL_TICK( Centroid_tick )
{
    // do nothing
    return TRUE;
}

CK_DLL_TOCK( Centroid_tock )
{
    t_CKFLOAT result = 0.0;

    // TODO: get buffer from stream, and set in ifft
    if( UANA->numIncomingUAnae() > 0 )
    {
        // get first
        Chuck_UAnaBlobProxy * BLOB_IN = UANA->getIncomingBlob( 0 );
        // sanity check
        assert( BLOB_IN != NULL );
        // get the array
        Chuck_Array8 & mag = BLOB_IN->fvals();
        // compute centroid
        result = compute_centroid( mag, mag.size() );
    }
    // otherwise zero out
    else
    {
        // no input!
        result = 0.0;
    }

    // get fvals of output BLOB
    Chuck_Array8 & fvals = BLOB->fvals();
    // ensure size == resulting size
    if( fvals.size() != 1 )
        fvals.set_size( 1 );
    // copy the result in
    fvals.set( 0, result );

    return TRUE;
}

CK_DLL_PMSG( Centroid_pmsg )
{
    // do nothing
    return TRUE;
}

CK_DLL_SFUN( Centroid_compute )
{
    // get array
    Chuck_Array8 * array = (Chuck_Array8 *)GET_NEXT_OBJECT(ARGS);
    // sanity check
    if( !array )
    {
        // no centroid
        RETURN->v_float = 0.0;
    }
    else
    {
        // do it
        RETURN->v_float = compute_centroid( *array, array->size() );
    }
}


// Flux state
struct StateOfFlux
{
    Chuck_Array8 prev;
    Chuck_Array8 norm;
    t_CKBOOL initialized;

    StateOfFlux()
    {
        initialized = FALSE;
    }
};


// compute norm rms
static void compute_norm_rms( Chuck_Array8 & curr, Chuck_Array8 & norm )
{
    t_CKUINT i;
    t_CKFLOAT energy = 0.0;
    t_CKFLOAT v;

    // check size
    if( norm.size() != curr.size() )
        norm.set_size( curr.size() );

    // get energy
    for( i = 0; i < curr.size(); i++ )
    {
        curr.get( i, &v );
        energy += v * v;
    }

    // check energy
    if (energy == 0.0) 
    {
        // all zeros
        norm.zero( 0, norm.size() );
        return;
    }
    else 
        energy = ::sqrt( energy );
    
    for( i = 0; i < curr.size(); i++ )
    {
        curr.get( i, & v );
        if( v > 0.0) 
            norm.set( i, v / energy );
        else
            norm.set( i, 0.0 );
    }
}

// compute flux
static t_CKFLOAT compute_flux( Chuck_Array8 & curr, Chuck_Array8 & prev, Chuck_Array8 * write )
{
    // sanity check
    assert( curr.size() == prev.size() );

    // ensure size
    if( write != NULL && (write->size() != curr.size()) )
        write->set_size( curr.size() );

    // find difference
    t_CKFLOAT v, w, result = 0.0;
    for( t_CKUINT i = 0; i < curr.size(); i++ )
    {
        curr.get( i, &v );
        prev.get( i, &w );
        // accumulate into flux
        result += (v - w)*(v - w);
        // copy to write
        if( write != NULL ) write->set( i, v );
    }

    // take sqrt of flux
    return ::sqrt( result );
}

// compute flux
static t_CKFLOAT compute_flux( Chuck_Array8 & curr, StateOfFlux & sof )
{
    // flux
    t_CKFLOAT result = 0.0;
    t_CKFLOAT v;

    // verify size
    if( curr.size() != sof.prev.size() )
    {
        sof.initialized = FALSE;
        // resize prev
        sof.prev.set_size( curr.size() );
    }

    // check initialized
    if( sof.initialized )
    {
        // compute normalize rms
        compute_norm_rms( curr, sof.norm );
        // do it
        result = compute_flux( sof.norm, sof.prev, &sof.prev );
    }

    // copy curr to prev
    for( t_CKUINT i = 0; i < curr.size(); i++ )
    {
        // get the value
        sof.norm.get( i, &v );
        // set it
        sof.prev.set( i, v );
    }

    // initialize
    sof.initialized = TRUE;

    // return result
    return result;
}

CK_DLL_CTOR( Flux_ctor )
{
    // allocate the flux state
    StateOfFlux * state = new StateOfFlux();
    OBJ_MEMBER_UINT( SELF, Flux_offset_data ) = (t_CKUINT)state;
}


CK_DLL_DTOR( Flux_dtor )
{
    // clean up
    StateOfFlux * state = (StateOfFlux *)OBJ_MEMBER_UINT( SELF, Flux_offset_data );
    SAFE_DELETE( state );
    OBJ_MEMBER_UINT( SELF, Flux_offset_data ) = 0;
}


CK_DLL_TICK( Flux_tick )
{
    // do nothing
    return TRUE;
}


CK_DLL_TOCK( Flux_tock )
{
    t_CKFLOAT result = 0.0;

    // get state
    StateOfFlux * state = (StateOfFlux *)OBJ_MEMBER_UINT( SELF, Flux_offset_data );

    // TODO: get buffer from stream, and set in ifft
    if( UANA->numIncomingUAnae() > 0 )
    {
        // get first
        Chuck_UAnaBlobProxy * BLOB_IN = UANA->getIncomingBlob( 0 );
        // sanity check
        assert( BLOB_IN != NULL );
        // get the array
        Chuck_Array8 & mag = BLOB_IN->fvals();
        // compute flux
        result = compute_flux( mag, *state );
    }
    // otherwise zero out
    else
    {
        // no input!
        result = 0.0;
    }

    // get fvals of output BLOB
    Chuck_Array8 & fvals = BLOB->fvals();
    // ensure size == resulting size
    if( fvals.size() != 1 )
        fvals.set_size( 1 );
    // copy the result in
    fvals.set( 0, result );

    return TRUE;
}

CK_DLL_PMSG( Flux_pmsg )
{
    // do nothing
    return TRUE;
}

CK_DLL_MFUN( Flux_ctrl_reset )
{
    // get state
    StateOfFlux * state = (StateOfFlux *)OBJ_MEMBER_UINT( SELF, Flux_offset_data );
    // set
    state->initialized = FALSE;
}

CK_DLL_SFUN( Flux_compute )
{
    // get inputs
    Chuck_Array8 * lhs = (Chuck_Array8 *)GET_NEXT_OBJECT(ARGS);
    Chuck_Array8 * rhs = (Chuck_Array8 *)GET_NEXT_OBJECT(ARGS);

    // verify
    if( !lhs || !rhs )
    {
        // 0
        RETURN->v_float = 0.0;
    }
    else
    {
        // verify size
        if( lhs->size() != rhs->size() )
        {
            // message
            EM_error3( "(via Flux): compute() expects two arrays of equal size" );
            // 0
            RETURN->v_float = 0.0;
        }
        else
        {
            // flux
            RETURN->v_float = compute_flux( *lhs, *rhs, NULL );
        }
    }
}

CK_DLL_SFUN( Flux_compute2 )
{
    // get inputs
    Chuck_Array8 * lhs = (Chuck_Array8 *)GET_NEXT_OBJECT(ARGS);
    Chuck_Array8 * rhs = (Chuck_Array8 *)GET_NEXT_OBJECT(ARGS);
    Chuck_Array8 * diff = (Chuck_Array8 *)GET_NEXT_OBJECT(ARGS);

    // verify
    if( !lhs || !rhs )
    {
        // 0
        RETURN->v_float = 0.0;
    }
    else
    {
        // verify size
        if( lhs->size() != rhs->size() )
        {
            // message
            EM_error3( "(via Flux): compute() expects two arrays of equal size" );
            // 0
            RETURN->v_float = 0.0;
        }
        else
        {
            // flux
            RETURN->v_float = compute_flux( *lhs, *rhs, diff );
        }
    }
}


static t_CKFLOAT compute_rms( Chuck_Array8 & buffer, t_CKUINT size )
{
    t_CKFLOAT rms = 0.0;
    t_CKFLOAT v;
    t_CKUINT i;

    // get sum of squares
    for( i = 0; i < size; i++ )
    {
        buffer.get( i, &v );
        rms += (v * v);
    }

    // go
    rms /= size;
    rms = ::sqrt(rms);

    return rms;
}


CK_DLL_TICK( RMS_tick )
{
    // do nothing
    return TRUE;
}

CK_DLL_TOCK( RMS_tock )
{
    t_CKFLOAT result = 0.0;

    // TODO: get buffer from stream, and set in ifft
    if( UANA->numIncomingUAnae() > 0 )
    {
        // get first
        Chuck_UAnaBlobProxy * BLOB_IN = UANA->getIncomingBlob( 0 );
        // sanity check
        assert( BLOB_IN != NULL );
        // get the array
        Chuck_Array8 & mag = BLOB_IN->fvals();
        // compute centroid
        result = compute_rms( mag, mag.size() );
    }
    // otherwise zero out
    else
    {
        // no input!
        result = 0.0;
    }

    // get fvals of output BLOB
    Chuck_Array8 & fvals = BLOB->fvals();
    // ensure size == resulting size
    if( fvals.size() != 1 )
        fvals.set_size( 1 );
    // copy the result in
    fvals.set( 0, result );

    return TRUE;
}

CK_DLL_PMSG( RMS_pmsg )
{
    // do nothing
    return TRUE;
}

CK_DLL_SFUN( RMS_compute )
{
    // get array
    Chuck_Array8 * array = (Chuck_Array8 *)GET_NEXT_OBJECT(ARGS);
    // sanity check
    if( !array )
    {
        // no RMS
        RETURN->v_float = 0.0;
    }
    else
    {
        // do it
        RETURN->v_float = compute_rms( *array, array->size() );
    }
}


static t_CKFLOAT compute_rolloff( Chuck_Array8 & buffer, t_CKUINT size, t_CKFLOAT percent )
{
    t_CKFLOAT sum = 0.0, v, target;
    t_CKINT i;

    // sanity check
    assert( percent >= 0 && percent <= 1 );

    // iterate
    for( i = 0; i < size; i++ )
    {
        buffer.get( i, &v );
        sum += v;
    }

    // the target
    target = sum * percent;
    sum = 0.0;

    // iterate
    for( i = 0; i < size; i++ )
    {
        buffer.get( i, &v );
        sum += v;
        if( sum >= target ) break;
    }

    return i/(t_CKFLOAT)size;
}


CK_DLL_CTOR( RollOff_ctor )
{
    OBJ_MEMBER_FLOAT( SELF, RollOff_offset_percent ) = .85;
}

CK_DLL_DTOR( RollOff_dtor )
{
}

CK_DLL_TICK( RollOff_tick )
{
    // do nothing
    return TRUE;
}

CK_DLL_TOCK( RollOff_tock )
{
    t_CKFLOAT result = 0.0;

    // get percent
    t_CKFLOAT percent = OBJ_MEMBER_FLOAT( SELF, RollOff_offset_percent );

    // TODO: get buffer from stream, and set in ifft
    if( UANA->numIncomingUAnae() > 0 )
    {
        // get first
        Chuck_UAnaBlobProxy * BLOB_IN = UANA->getIncomingBlob( 0 );
        // sanity check
        assert( BLOB_IN != NULL );
        // get the array
        Chuck_Array8 & mag = BLOB_IN->fvals();
        // compute rolloff
        result = compute_rolloff( mag, mag.size(), percent );
    }
    // otherwise zero out
    else
    {
        // no input!
        result = 0.0;
    }

    // get fvals of output BLOB
    Chuck_Array8 & fvals = BLOB->fvals();
    // ensure size == resulting size
    if( fvals.size() != 1 )
        fvals.set_size( 1 );
    // copy the result in
    fvals.set( 0, result );

    return TRUE;
}

CK_DLL_PMSG( RollOff_pmsg )
{
    // do nothing
    return TRUE;
}

CK_DLL_CTRL( RollOff_ctrl_percent )
{
    // get percent
    t_CKFLOAT percent = GET_NEXT_FLOAT(ARGS);
    // check it
    if( percent < 0.0 ) percent = 0.0;
    else if( percent > 1.0 ) percent = 1.0;
    // set it
    OBJ_MEMBER_FLOAT(SELF, RollOff_offset_percent) = percent;
    // return it
    RETURN->v_float = percent;
}

CK_DLL_CGET( RollOff_cget_percent )
{
    // return it
    RETURN->v_float = OBJ_MEMBER_FLOAT(SELF, RollOff_offset_percent);
}

CK_DLL_SFUN( RollOff_compute )
{
    // get array
    Chuck_Array8 * array = (Chuck_Array8 *)GET_NEXT_OBJECT(ARGS);
    // get percent
    t_CKFLOAT percent = GET_NEXT_FLOAT(ARGS);

    // sanity check
    if( !array )
    {
        // no RollOff
        RETURN->v_float = 0.0;
    }
    else
    {
        // do it
        RETURN->v_float = compute_rolloff( *array, array->size(), percent );
    }
}




// struct
struct Corr_Object
{
    // whether to normalize
    t_CKBOOL normalize;
    // f
    SAMPLE * fbuf;
    t_CKINT fcap;
    // g
    SAMPLE * gbuf;
    t_CKINT gcap;
    // result
    SAMPLE * buffy;
    t_CKINT bufcap;

    // constructor
    Corr_Object()
    {
        // default: do normalize
        normalize = FALSE;
        // zero out pointers
        fbuf = gbuf = buffy = NULL;
        fcap = gcap = bufcap = 0;
        // TODO: default
        resize( 512, 512 );
    }
    
    // destructor
    ~Corr_Object()
    {
        // done
        this->reset();
    }

    // reset
    void reset()
    {
        SAFE_DELETE_ARRAY( fbuf );
        SAFE_DELETE_ARRAY( gbuf );
        SAFE_DELETE_ARRAY( buffy );
        fcap = gcap = bufcap = 0;
    }

    // clear
    void clear()
    {
        if( fbuf ) memset( fbuf, 0, fcap * sizeof(SAMPLE) );
        if( gbuf ) memset( gbuf, 0, gcap * sizeof(SAMPLE) );
        if( buffy ) memset( buffy, 0, bufcap * sizeof(SAMPLE) );
    }

    // resize inputs
    t_CKBOOL resize( t_CKINT fs, t_CKINT gs )
    {
        // minimum size
        t_CKINT mincap = fs + gs - 1;
        // ensure power of two
        mincap = ensurepow2( mincap );
        // log
        EM_log( CK_LOG_FINE, "Corr resizing to %d-element buffers...", mincap );

        // verify
        if( fcap < mincap )
        {
            SAFE_DELETE_ARRAY( fbuf );
            fbuf = new SAMPLE[mincap];
            fcap = mincap;
        }
        if( gcap < mincap )
        {
            SAFE_DELETE_ARRAY( gbuf );
            gbuf = new SAMPLE[mincap];
            gcap = mincap;
        }
        if( bufcap < mincap )
        {
            SAFE_DELETE_ARRAY( buffy );
            buffy = new SAMPLE[mincap];
            bufcap = mincap;
        }

        // hopefully
        if( fbuf == NULL || gbuf == NULL || buffy == NULL )
        {
            // error
            fprintf( stderr, "[chuck]: Corr failed to allocate %d-element buffer(s)...",
                mincap );
            // clean up
            this->reset();
            // done
            return FALSE;
        }

        // clear it
        this->clear();

        return TRUE;
    }
};

// compute correlation
static void compute_corr( Corr_Object * corr, Chuck_Array8 & f, t_CKINT fs, 
                          Chuck_Array8 & g, t_CKINT gs, Chuck_Array8 & buffy )
{
    t_CKINT i;
    t_CKFLOAT v;
    t_CKINT size;

    // ensure size
    corr->resize( fs, gs );

    // copy into buffers
    for( i = 0; i < fs; i++ )
    {
        f.get( i, &v );
        corr->fbuf[i] = v;
    }
    for( i = 0; i < gs; i++ )
    {
        g.get( i, &v );
        corr->gbuf[i] = v;
    }

    // compute
    xcorr_fft( corr->fbuf, corr->fcap, corr->gbuf, corr->gcap,
               corr->buffy, corr->bufcap );

    // copy into result
    size = fs + gs - 1;
    buffy.set_size( size );
    for( i = 0; i < size; i++ )
    {
        buffy.set( i, corr->buffy[i] );
    }
}

// AutoCorr
CK_DLL_CTOR( AutoCorr_ctor )
{
    Corr_Object * ac = new Corr_Object();
    OBJ_MEMBER_UINT( SELF, AutoCorr_offset_data ) = (t_CKUINT)ac;
}

CK_DLL_DTOR( AutoCorr_dtor )
{
    Corr_Object * ac = (Corr_Object *)OBJ_MEMBER_UINT( SELF, AutoCorr_offset_data );
    SAFE_DELETE(ac);
    OBJ_MEMBER_UINT( SELF, AutoCorr_offset_data ) = 0;
}

CK_DLL_TICK( AutoCorr_tick )
{
    // do nothing
    return TRUE;
}

CK_DLL_TOCK( AutoCorr_tock )
{
    // get object
    Corr_Object * ac = (Corr_Object *)OBJ_MEMBER_UINT( SELF, AutoCorr_offset_data );

    // TODO: get buffer from stream, and set
    if( UANA->numIncomingUAnae() > 0 )
    {
        // get first
        Chuck_UAnaBlobProxy * BLOB_IN = UANA->getIncomingBlob( 0 );
        // sanity check
        assert( BLOB_IN != NULL );
        // get the array
        Chuck_Array8 & mag = BLOB_IN->fvals();
        // get fvals of output BLOB
        Chuck_Array8 & fvals = BLOB->fvals();
        // compute autocorr
        compute_corr( ac, mag, mag.size(), mag, mag.size(), fvals );
    }
    // otherwise zero out
    else
    {
        // get fvals of output BLOB
        Chuck_Array8 & fvals = BLOB->fvals();
        // resize
        fvals.set_size( 0 );
    }

    return TRUE;
}

CK_DLL_PMSG( AutoCorr_pmsg )
{
    // do nothing
    return TRUE;
}

CK_DLL_CTRL( AutoCorr_ctrl_normalize )
{
    // get object
    Corr_Object * ac = (Corr_Object *)OBJ_MEMBER_UINT( SELF, AutoCorr_offset_data );
    // get percent
    ac->normalize = GET_NEXT_INT(ARGS) != 0;
    // return it
    RETURN->v_int = ac->normalize;
}

CK_DLL_CGET( AutoCorr_cget_normalize )
{
    // get object
    Corr_Object * ac = (Corr_Object *)OBJ_MEMBER_UINT( SELF, AutoCorr_offset_data );
    // return it
    RETURN->v_int = ac->normalize;
}

CK_DLL_SFUN( AutoCorr_compute )
{
}


//-----------------------------------------------------------------------------
// name: xcorr_fft()
// desc: FFT-based cross correlation
//-----------------------------------------------------------------------------
void xcorr_fft( SAMPLE * f, t_CKINT fsize, SAMPLE * g, t_CKINT gsize, SAMPLE * buffy, t_CKINT size )
{
    // sanity check
    assert( fsize == gsize == size );

    // take fft
    rfft( f, size/2, FFT_FORWARD );
    rfft( g, size/2, FFT_FORWARD );

    // complex
    t_CKCOMPLEX_SAMPLE * F = (t_CKCOMPLEX_SAMPLE *)f;
    t_CKCOMPLEX_SAMPLE * G = (t_CKCOMPLEX_SAMPLE *)g;
    t_CKCOMPLEX_SAMPLE * Y = (t_CKCOMPLEX_SAMPLE *)buffy;

    // loop
    for( t_CKINT i = 0; i < size/2; i++ )
    {
        // conjugate F
        F[i].im = -F[i].im;
        // complex multiply
        Y[i].re = F[i].re*G[i].re - F[i].im*G[i].im;
        Y[i].im = F[i].im*G[i].re + F[i].re*G[i].im;
    }

    // inverse fft
    rfft( buffy, size/2, FFT_INVERSE );
}




//-----------------------------------------------------------------------------
// name: xcorr_normalize()
// desc: ...
//-----------------------------------------------------------------------------
void xcorr_normalize( SAMPLE * buffy, t_CKINT size, SAMPLE * f, t_CKINT fs, SAMPLE * g, t_CKINT gs )
{
    float sum = 0.000001f;

    // f^2(t)
    for( long i = 0; i < fs; i++ )
        sum += f[i]*f[i];
    // g^2(t)
    for( long j = 0; j < gs; j++ )
        sum += g[j]*g[j];
    // normalize: taking coherence into account
    for( long k = 0; k < size; k++ )
        buffy[k] /= sum;
}
