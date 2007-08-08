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
// offset
static t_CKUINT Flux_offset_data = 0;


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
    func->add_arg( "float[]", "input" );
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
static t_CKFLOAT compute_flux( Chuck_Array8 & curr, StateOfFlux & sof )
{
    // flux
    t_CKFLOAT result = 0.0;
    t_CKFLOAT v, w;

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
        // find different
        t_CKUINT i;
        for( i = 0; i < sof.norm.capacity(); i++ )
        {
            sof.norm.get( i, &v );
            sof.prev.get( i, &w );
            // accumulate into flux
            result += (v - w)*(v - w);
            // copy to prev
            sof.prev.set( i, v );
        }
        // take sqrt of flux
        result = ::sqrt( result );
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
}