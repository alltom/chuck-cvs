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
// date: Summer 2007
//-----------------------------------------------------------------------------
#include "uana_extract.h"
#include "chuck_type.h"
#include "chuck_vm.h"
#include "chuck_instr.h"
#include "chuck_lang.h"
#include "chuck_errmsg.h"

#include <math.h>


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


//-----------------------------------------------------------------------------
// name: extract_query()
// desc: ...
//-----------------------------------------------------------------------------
DLL_QUERY extract_query( Chuck_DL_Query * QUERY )
{
    Chuck_Env * env = Chuck_Env::instance();

    Chuck_DL_Func * func = NULL;

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

    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );

    return FALSE;
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
        result = compute_centroid( mag, mag.capacity() );
    }
    // otherwise zero out
    else
    {
        // no input!
        result = 0.0;
    }

    // get fvals of output BLOB
    Chuck_Array8 & fvals = BLOB->fvals();
    // ensure capacity == resulting size
    if( fvals.capacity() != 1 )
        fvals.set_capacity( 1 );
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
        RETURN->v_float = compute_centroid( *array, array->capacity() );
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
    if( norm.capacity() != curr.capacity() )
        norm.set_capacity( curr.capacity() );

    // get energy
    for( i = 0; i < curr.capacity(); i++ )
    {
        curr.get( i, &v );
        energy += v * v;
    }

    // check energy
    if (energy == 0.0) 
    {
        // all zeros
        norm.zero( 0, norm.capacity() );
        return;
    }
    else 
        energy = ::sqrt( energy );
    
    for( i = 0; i < curr.capacity(); i++ )
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
    assert( curr.capacity() == prev.capacity() );

    // ensure capacity
    if( write != NULL && (write->capacity() != curr.capacity()) )
        write->set_capacity( curr.capacity() );

    // find difference
    t_CKFLOAT v, w, result = 0.0;
    for( t_CKUINT i = 0; i < curr.capacity(); i++ )
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

    // verify capacity
    if( curr.capacity() != sof.prev.capacity() )
    {
        sof.initialized = FALSE;
        // resize prev
        sof.prev.set_capacity( curr.capacity() );
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
    for( t_CKUINT i = 0; i < curr.capacity(); i++ )
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
    // ensure capacity == resulting size
    if( fvals.capacity() != 1 )
        fvals.set_capacity( 1 );
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
        if( lhs->capacity() != rhs->capacity() )
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
        if( lhs->capacity() != rhs->capacity() )
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
        result = compute_rms( mag, mag.capacity() );
    }
    // otherwise zero out
    else
    {
        // no input!
        result = 0.0;
    }

    // get fvals of output BLOB
    Chuck_Array8 & fvals = BLOB->fvals();
    // ensure capacity == resulting size
    if( fvals.capacity() != 1 )
        fvals.set_capacity( 1 );
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
        RETURN->v_float = compute_rms( *array, array->capacity() );
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
        result = compute_rolloff( mag, mag.capacity(), percent );
    }
    // otherwise zero out
    else
    {
        // no input!
        result = 0.0;
    }

    // get fvals of output BLOB
    Chuck_Array8 & fvals = BLOB->fvals();
    // ensure capacity == resulting size
    if( fvals.capacity() != 1 )
        fvals.set_capacity( 1 );
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
        RETURN->v_float = compute_rolloff( *array, array->capacity(), percent );
    }
}
