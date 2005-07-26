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
// file: ugen_filter.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#include "ugen_filter.h"
#include "chuck_type.h"
#include <math.h>
#include <stdlib.h>


static t_CKUINT g_srate = 0;
//filter member data offset
static t_CKUINT filter_offset_data = 0;
static t_CKUINT biquad_offset_data = 0;


//-----------------------------------------------------------------------------
// name: filter_query()
// desc: ...
//-----------------------------------------------------------------------------
DLL_QUERY filter_query( Chuck_DL_Query * QUERY )
{
    // set srate
    g_srate = QUERY->srate;
    
    Chuck_Env * env = Chuck_Env::instance();

    Chuck_DL_Func * func = NULL;

    //---------------------------------------------------------------------
    // init as base class: filter
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "filter", "ugen", env->global(), 
                                        filter_ctor, filter_tick, filter_pmsg ) )
        return FALSE;

    // member variable
    filter_offset_data = type_engine_import_mvar ( env, "int", "@filter_data", FALSE );
    if ( filter_offset_data == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );


    //---------------------------------------------------------------------
    // init as base class: biquad
    //---------------------------------------------------------------------
    if( !type_engine_import_ugen_begin( env, "biquad", "ugen", env->global(), 
                                        biquad_ctor, biquad_tick, NULL ) )
        return FALSE;

    // member variable
    biquad_offset_data = type_engine_import_mvar ( env, "int", "@biquad_data", FALSE );
    if ( biquad_offset_data == CK_INVALID_OFFSET ) goto error;

    // pfreq
    func = make_new_mfun ( "float", "pfreq", biquad_ctrl_pfreq );
    func->add_arg ( "float", "freq" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "pfreq", biquad_cget_pfreq );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // prad
    func = make_new_mfun ( "float", "prad", biquad_ctrl_prad );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "prad", biquad_cget_prad );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // zfreq
    func = make_new_mfun ( "float", "zfreq", biquad_ctrl_zfreq );
    func->add_arg ( "float", "freq" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "zfreq", biquad_cget_zfreq );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // zrad
    func = make_new_mfun ( "float", "zrad", biquad_ctrl_zrad );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "zrad", biquad_cget_zrad );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // norm
    func = make_new_mfun ( "int", "norm", biquad_ctrl_norm );
    func->add_arg ( "int", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "int", "norm", biquad_cget_norm );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // pregain
    func = make_new_mfun ( "float", "pregain", biquad_ctrl_pregain );
    func->add_arg ( "float", "level" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "pregain", biquad_cget_pregain );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // eqzs
    func = make_new_mfun ( "int", "eqzs", biquad_ctrl_eqzs );
    func->add_arg ( "int", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // b0
    func = make_new_mfun ( "float", "b0", biquad_ctrl_b0 );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "b0", biquad_cget_b0 );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // b1
    func = make_new_mfun ( "float", "b1", biquad_ctrl_b1 );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "b1", biquad_cget_b1 );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // b2
    func = make_new_mfun ( "float", "b2", biquad_ctrl_b2 );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "b2", biquad_cget_b2 );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // a0
    func = make_new_mfun ( "float", "a0", biquad_ctrl_a0 );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "a0", biquad_cget_a0 );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // a1
    func = make_new_mfun ( "float", "a1", biquad_ctrl_a1 );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "a1", biquad_cget_a1 );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // a2
    func = make_new_mfun ( "float", "a2", biquad_ctrl_a2 );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    
    func = make_new_mfun ( "float", "a2", biquad_cget_a2 );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // end the class import
    type_engine_import_class_end( env );

    //----------------------------------
    // begin onepole ugen
    //----------------------------------
    if ( !type_engine_import_ugen_begin( env, "onepole", "biquad", env->global(),
                                         NULL, onepole_tick, NULL ) ) return FALSE;
    // ctrl func
    func = make_new_mfun ( "float", "pole", onepole_ctrl_pole );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // end the class import
    type_engine_import_class_end( env );


    //----------------------------------
    // begin onezero ugen
    //----------------------------------
    if ( !type_engine_import_ugen_begin( env, "onezero", "biquad", env->global(),
                                         NULL, onezero_tick, NULL ) ) return FALSE;
    // ctrl func
    func = make_new_mfun ( "float", "zero", onezero_ctrl_zero );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    


    // end the class import
    type_engine_import_class_end( env );


    //----------------------------------
    // begin twopole ugen
    //----------------------------------
    if ( !type_engine_import_ugen_begin( env, "twopole", "biquad", env->global(),
                                         NULL, twopole_tick, NULL ) ) return FALSE;
    // ctrl func
    func = make_new_mfun ( "float", "freq", twopole_ctrl_freq );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    func = make_new_mfun ( "float", "rad", twopole_ctrl_rad );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    func = make_new_mfun ( "int", "norm", twopole_ctrl_norm );
    func->add_arg ( "int", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // end the class import
    type_engine_import_class_end( env );

    //----------------------------------
    // begin twozero ugen
    //----------------------------------
    if ( !type_engine_import_ugen_begin( env, "twozero", "biquad", env->global(),
                                         NULL, twozero_tick, NULL ) ) return FALSE;
    // ctrl func
    func = make_new_mfun ( "float", "freq", twozero_ctrl_freq );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    func = make_new_mfun ( "float", "rad",  twozero_ctrl_rad );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    // end the class import
    type_engine_import_class_end( env );

    //----------------------------------
    // begin delay ugen
    //----------------------------------
    if ( !type_engine_import_ugen_begin( env, "delay", "ugen", env->global(),
                                         delay_ctor, delay_tick, NULL ) ) return FALSE;

    // ctrl func
    func = make_new_mfun ( "float", "delay", delay_ctrl_delay );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    func = make_new_mfun ( "float", "max", delay_ctrl_max );
    func->add_arg ( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    


    // QUERY->ugen_ctrl( QUERY, delay_ctrl_energy, NULL,"int", "energy" );
    // QUERY->ugen_ctrl( QUERY, delay_ctrl_tap, NULL, "int", "tap" );
    // QUERY->ugen_ctrl( QUERY, delay_ctrl_ftap, NULL, "float", "ftap" );

    // end the class import
    type_engine_import_class_end( env );

    //----------------------------------
    // begin delayA ugen
    //----------------------------------
    if ( !type_engine_import_ugen_begin( env, "delayA", "ugen", env->global(),
                                         delayA_ctor, delayA_tick, NULL ) ) return FALSE;

    // ctrl func
    func = make_new_mfun ( "float", "delay", delayA_ctrl_delay ); 
    func->add_arg ( "float", "length" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    func = make_new_mfun ( "float", "max", delayA_ctrl_max );
    func->add_arg ( "float", "max" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    


    // end the class import
    type_engine_import_class_end( env );

    //----------------------------------
    // begin one ugen
    //----------------------------------
    if ( !type_engine_import_ugen_begin( env, "delayL", "ugen", env->global(),
                                         delayL_ctor, delayL_tick, NULL ) ) return FALSE;
    // ctrl func
    func = make_new_mfun ( "float", "delay", delayL_ctrl_delay );
    func->add_arg ( "float", "length" );
    if( !type_engine_import_mfun( env, func ) ) goto error;    

    func = make_new_mfun ( "float", "max", delayL_ctrl_max );
    func->add_arg ( "float", "max" );
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
// name: filter
// desc: ...
//-----------------------------------------------------------------------------
struct filter_data
{
    t_CKUINT nB;
    t_CKUINT nA;
    SAMPLE * b;
    SAMPLE * a;
    SAMPLE * input;
    SAMPLE * output;
    
    filter_data()
    {
        a = b = NULL;
        alloc( 1, 1 );
        a[0] = b[0] = 1.0f;
    }
    
    void alloc( t_CKUINT _b, t_CKUINT _a )
    {
        nB = _b; nA = _a;
        if( b ) delete b;
        if( a ) delete a;
        if( input ) delete input;
        if( output ) delete output;
        b = new SAMPLE[nB];
        a = new SAMPLE[nA];
        input = new SAMPLE[nB];
        output = new SAMPLE[nA];
    }
    
    SAMPLE tick( SAMPLE in )
    {
        t_CKUINT i;

        output[0] = 0.0;
        input[0] = in;
        for( i = nB-1; i > 0; i-- )
        {
            output[0] += b[i] * input[i];
            input[i] = input[i-1];
        }
        output[0] += b[0] * input[0];

        for ( i = nA-1; i>0; i--)
        {
            output[0] += -a[i] * output[i];
            output[i] = output[i-1];
        }

        return output[0];
    }
};

//-----------------------------------------------------------------------------
// name: filter_ctor()
// desc: CTOR function ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( filter_ctor )
{
    filter_data * f =  new filter_data;
    OBJ_MEMBER_UINT( SELF, filter_offset_data ) = (t_CKUINT)f;
}


//-----------------------------------------------------------------------------
// name: filter_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( filter_tick )
{
    filter_data * d = (filter_data *)OBJ_MEMBER_UINT(SELF, filter_offset_data );
    *out = d->tick( in );
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: filter_ctrl_coefs()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( filter_ctrl_coefs )
{
}

//-----------------------------------------------------------------------------
// name: filter_pmsg()
// desc: PMSG function ...
//-----------------------------------------------------------------------------
CK_DLL_PMSG( filter_pmsg )
{
    return FALSE;
}



//-----------------------------------------------------------------------------
// name: biquad
// desc: biquad filter
//-----------------------------------------------------------------------------
struct biquad_data
{
    SAMPLE m_a0, m_a1, m_a2;
    SAMPLE m_b0, m_b1, m_b2;
    
    SAMPLE m_input0, m_input1, m_input2;
    SAMPLE m_output0, m_output1, m_output2;

    t_CKFLOAT pfreq, zfreq;
    t_CKFLOAT prad, zrad;
    t_CKBOOL norm;
    t_CKUINT srate;

    biquad_data()
    {
        m_a0 = m_b0 = 1.0f;
        m_a1 = m_a2 = 0.0f;
        m_b1 = 0.0f; m_b2 = 0.0f;

        m_input0 = m_input1 = m_input2 = 0.0f;
        m_output0 = m_output1 = m_output2 = 0.0f;

        pfreq = zfreq = 0.0f;
        prad = zrad = 0.0f;
        norm = FALSE;
        srate = g_srate;
    }
};

//-----------------------------------------------------------------------------
// name: biquad_ctor()
// desc: CTOR function ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( biquad_ctor )
{    
    biquad_data* d = new biquad_data;
    OBJ_MEMBER_UINT ( SELF, biquad_offset_data ) = ( t_CKUINT ) d;
}

//-----------------------------------------------------------------------------
// name: biquad_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( biquad_tick )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    
    d->m_input0 = d->m_a0 * in;
    d->m_output0 = d->m_b0 * d->m_input0 + d->m_b1 * d->m_input1 + d->m_b2 * d->m_input2;
    d->m_output0 -= d->m_a2 * d->m_output2 + d->m_a1 * d->m_output1;
    d->m_input2 = d->m_input1;
    d->m_input1 = d->m_input0;
    d->m_output2 = d->m_output1;
    d->m_output1 = d->m_output0;

    *out = (SAMPLE) d->m_output0;

    return TRUE;
}

void biquad_set_reson( biquad_data * d )
{
    d->m_a2 = (SAMPLE)(d->prad * d->prad);
    d->m_a1 = (SAMPLE)(-2.0 * d->prad * cos(2.0 * ONE_PI * d->pfreq / (double)d->srate));

    if ( d->norm ) {
        // Use zeros at +- 1 and normalize the filter peak gain.
        d->m_b0= 0.5f - 0.5f * d->m_a2;
        d->m_b1 = -1.0f;
        d->m_b2 = -d->m_b0;
    }    
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_pfreq()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_pfreq )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->pfreq = GET_CK_FLOAT(ARGS);
    biquad_set_reson( d ); 
    RETURN->v_float = d->pfreq;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_pfreq()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_pfreq )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->pfreq;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_prad()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_prad )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->prad = GET_CK_FLOAT(ARGS);
    biquad_set_reson( d );
    RETURN->v_float = d->prad;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_prad()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_prad )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->prad;
}

void biquad_set_notch( biquad_data * d )
{
    d->m_b2 = (SAMPLE)(d->zrad * d->zrad);
    d->m_b1 = (SAMPLE)(-2.0 * d->zrad * cos(2.0 * ONE_PI * d->zfreq / (double)d->srate));
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_zfreq()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_zfreq )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->zfreq = GET_CK_FLOAT(ARGS);
    biquad_set_notch( d );
    RETURN->v_float = d->zfreq;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_zfreq()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_zfreq )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->zfreq;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_zrad()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_zrad )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->zrad = GET_CK_FLOAT(ARGS);
    biquad_set_notch( d );
    RETURN->v_float = d->zrad;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_zrad()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_zrad )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->zrad;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_norm()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_norm )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->norm = *(t_CKBOOL *)ARGS;
    biquad_set_reson( d );
    RETURN->v_int = d->norm;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_norm()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_norm )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_int = d->norm;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_pregain()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_pregain )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->m_a0 = (SAMPLE)GET_CK_FLOAT(ARGS);
    RETURN->v_float = d->m_a0;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_pregain()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_pregain )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->m_a0;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_eqzs()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_eqzs )
{
    if( *(t_CKUINT *)ARGS )
    {
        biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
        d->m_b0 = 1.0f;
        d->m_b1 = 0.0f;
        d->m_b2 = -1.0f;
    }
    RETURN->v_int = *(t_CKUINT *)ARGS;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_b0()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_b0 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->m_b0 = (SAMPLE)GET_CK_FLOAT(ARGS);
    RETURN->v_float = d->m_b0;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_b0()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_b0 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->m_b0;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_b1()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_b1 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->m_b1 = (SAMPLE)GET_CK_FLOAT(ARGS);
    RETURN->v_float = d->m_b1;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_b1()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_b1 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->m_b1;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_b2()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_b2 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->m_b2 = (SAMPLE)GET_CK_FLOAT(ARGS);
    RETURN->v_float = d->m_b2;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_b2()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_b2 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->m_b2;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_a0()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_a0 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->m_a0 = (SAMPLE)GET_CK_FLOAT(ARGS);
    RETURN->v_float = d->m_a0;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_a0()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_a0 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->m_a0;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_a1()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_a1 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->m_a1 = (SAMPLE)GET_CK_FLOAT(ARGS);
    RETURN->v_float = d->m_a1;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_a1()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_a1 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->m_a1;
}

//-----------------------------------------------------------------------------
// name: biquad_ctrl_a2()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_ctrl_a2 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->m_a2 = (SAMPLE)GET_CK_FLOAT(ARGS);
    RETURN->v_float = d->m_a2;
}

//-----------------------------------------------------------------------------
// name: biquad_cget_a2()
// desc: CGET function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( biquad_cget_a2 )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    RETURN->v_float = d->m_a2;
}


//-----------------------------------------------------------------------------
// name: onepole
// desc: onepole filter
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// name: onepole_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( onepole_tick )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );

    d->m_input0 = in;
    d->m_output0 = d->m_b0 * d->m_input0 - d->m_a1 * d->m_output1;
    d->m_output1 = d->m_output0;

    *out = d->m_output0;

    return TRUE;
}

//-----------------------------------------------------------------------------
// name: onepole_ctrl_pole()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( onepole_ctrl_pole )
{
    float f = (float)GET_CK_FLOAT(ARGS);
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    
    if( f > 0.0f )
        d->m_b0 = 1.0f - f;
    else
        d->m_b0 = 1.0f + f;

    d->m_a0 = -f;
}




//-----------------------------------------------------------------------------
// name: onezero
// desc: onezero filter
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// name: onezero_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( onezero_tick )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    
    d->m_input0 = in;
    d->m_output0 = d->m_b1 * d->m_input1 + d->m_b0 * d->m_input0;
    d->m_input1 = d->m_input0;

    *out = d->m_output0;

    return TRUE;
}

//-----------------------------------------------------------------------------
// name: onezero_ctrl_zero()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( onezero_ctrl_zero )
{
    float f = (float)GET_CK_FLOAT(ARGS);
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    
    if( f > 0.0f )
        d->m_b0 = 1.0f / ( 1.0f + f );
    else
        d->m_b0 = 1.0f / ( 1.0f - f );
        
    d->m_b1 = -f * d->m_b0;
}




//-----------------------------------------------------------------------------
// name: twopole
// desc: twopole filter
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// name: twopole_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( twopole_tick )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    
    d->m_input0 = in;
    d->m_output0 = d->m_b0 * d->m_input0 - d->m_a2 * d->m_output2 - d->m_a1 * d->m_output1;
    d->m_output2 = d->m_output1;
    d->m_output1 = d->m_output0;

    *out = d->m_output0;

    return TRUE;
}

//-----------------------------------------------------------------------------
// name: twopole_ctrl_freq()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( twopole_ctrl_freq )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->pfreq = (float)GET_CK_FLOAT(ARGS);
    biquad_set_reson( d );
    
    if( d->norm )
    {
        // Normalize the filter gain ... not terribly efficient.
        double real = 1.0 - d->prad + (d->m_a2 - d->prad) * cos( 2.0 * ONE_PI * d->pfreq / d->srate );
        double imag = (d->m_a2 - d->prad) * sin( 2.0 * ONE_PI * d->pfreq / d->srate );
        d->m_b0 = sqrt( real*real + imag*imag );
    }
}

//-----------------------------------------------------------------------------
// name: twopole_ctrl_rad()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( twopole_ctrl_rad )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->prad = (float)GET_CK_FLOAT(ARGS);
    biquad_set_reson( d );
    
    if( d->norm )
    {
        // Normalize the filter gain ... not terrbly efficient
        double real = 1.0 - d->prad + (d->m_a2 - d->prad) * cos( 2.0 * ONE_PI * d->pfreq / d->srate );
        double imag = (d->m_a2 - d->prad) * sin( 2.0 * ONE_PI * d->pfreq / d->srate );
        d->m_b0 = sqrt( real*real + imag*imag );
    }
}

//-----------------------------------------------------------------------------
// name: twopole_ctrl_norm()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( twopole_ctrl_norm )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->norm = *(t_CKBOOL *)ARGS;
    
    if( d->norm )
    {
        // Normalize the filter gain ... not terribly efficient
        double real = 1.0 - d->prad + (d->m_a2 - d->prad) * cos( 2.0 * ONE_PI * d->pfreq / d->srate );
        double imag = (d->m_a2 - d->prad) * sin( 2.0 * ONE_PI * d->pfreq / d->srate );
        d->m_b0 = sqrt( real*real + imag*imag );
    }
}




//-----------------------------------------------------------------------------
// name: twozero
// desc: twozero filter
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// name: twozero_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( twozero_tick )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    
    d->m_input0 = in;
    d->m_output0 = d->m_b0 * d->m_input0 + d->m_b1 * d->m_input1 + d->m_b2 * d->m_input2;
    d->m_input2 = d->m_input1;
    d->m_input1 = d->m_input0;

    *out = d->m_output0;
    
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: twozero_ctrl_freq()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( twozero_ctrl_freq )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->zfreq = (float)GET_CK_FLOAT(ARGS);
    biquad_set_notch( d );
    
    // normalize the filter gain
    if( d->m_b1 > 0.0f )
        d->m_b0 = 1.0f / (1.0f+d->m_b1+d->m_b2);
    else
        d->m_b0 = 1.0f / (1.0f-d->m_b1+d->m_b2);
    d->m_b1 *= d->m_b0;
    d->m_b2 *= d->m_b0;
}

//-----------------------------------------------------------------------------
// name: twozero_ctrl_rad()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( twozero_ctrl_rad )
{
    biquad_data * d = (biquad_data *)OBJ_MEMBER_UINT(SELF, biquad_offset_data );
    d->zrad = (float)GET_CK_FLOAT(ARGS);
    biquad_set_notch( d );

    // normalize the filter gain
    if( d->m_b1 > 0.0f )
        d->m_b0 = 1.0f / (1.0f+d->m_b1+d->m_b2);
    else
        d->m_b0 = 1.0f / (1.0f-d->m_b1+d->m_b2);
    d->m_b1 *= d->m_b0;
    d->m_b2 *= d->m_b0;    
}




//-----------------------------------------------------------------------------
// name: gQ
// desc: gQ filter - a la Dan Trueman
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// name: gQ_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( gQ_tick )
{
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: gQ_ctrl_freq()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( gQ_ctrl_freq )
{
}

//-----------------------------------------------------------------------------
// name: gQ_ctrl_rad()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( gQ_ctrl_rad )
{
}




//-----------------------------------------------------------------------------
// name: allpass
// desc: allpass filter
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// name: allpass_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( allpass_tick )
{
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: allpass_pmsg()
// desc: PMSG function ...
//-----------------------------------------------------------------------------
CK_DLL_PMSG( allpass_pmsg )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: delay
// desc: ...
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// name: delay_ctor()
// desc: CTOR function ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( delay_ctor )
{
}


//-----------------------------------------------------------------------------
// name: delay_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( delay_tick )
{
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: delay_ctrl_delay()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( delay_ctrl_delay )
{
}

//-----------------------------------------------------------------------------
// name: delay_ctrl_max()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( delay_ctrl_max )
{
}

//-----------------------------------------------------------------------------
// name: delay_ctrl_tap()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( delay_ctrl_tap )
{
}

//-----------------------------------------------------------------------------
// name: delay_ctrl_energy()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( delay_ctrl_energy )
{
}




//-----------------------------------------------------------------------------
// name: delayA
// desc: delay - allpass interpolation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// name: delayA_ctor()
// desc: CTOR function ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( delayA_ctor )
{
}

//-----------------------------------------------------------------------------
// name: delayA_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( delayA_tick )
{
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: delayA_ctrl_delay()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( delayA_ctrl_delay )
{
}

//-----------------------------------------------------------------------------
// name: delayA_ctrl_max()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( delayA_ctrl_max )
{
}


//-----------------------------------------------------------------------------
// name: delayL
// desc: delay - linear interpolation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// name: delayL_ctor()
// desc: CTOR function ...
//-----------------------------------------------------------------------------
CK_DLL_CTOR( delayL_ctor )
{
}

//-----------------------------------------------------------------------------
// name: delayL_tick()
// desc: TICK function ...
//-----------------------------------------------------------------------------
CK_DLL_TICK( delayL_tick )
{
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: delayL_ctrl_delay()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( delayL_ctrl_delay )
{
}

//-----------------------------------------------------------------------------
// name: delayL_ctrl_max()
// desc: CTRL function ...
//-----------------------------------------------------------------------------
CK_DLL_CTRL( delayL_ctrl_max )
{
}
