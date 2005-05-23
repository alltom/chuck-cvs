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
// file: ulib_math.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#include "ulib_math.h"
#include "util_math.h"
#include "ulib_std.h"


static double g_pi = 3.14159265358979323846;
static double g_twopi = 2.0 * 3.14159265358979323846;
static double g_e = ::exp( 1.0 );


// query
DLL_QUERY libmath_query( Chuck_DL_Query * QUERY )
{
    // name
    QUERY->setname( QUERY, "Math" );

    /*! \example
    math.sin( math.pi /2.0 ) => stdout;
    */

    // add class
    QUERY->begin_class( QUERY, "math", "Object" );
    
    // sin
    //! see \example math.ck
    QUERY->add_sfun( QUERY, sin_impl, "float", "sin" );
    QUERY->add_arg( QUERY, "float", "x" );
    
    // cos
    QUERY->add_sfun( QUERY, cos_impl, "float", "cos" );
    QUERY->add_arg( QUERY, "float", "x" );

    // tan
    QUERY->add_sfun( QUERY, tan_impl, "float", "tan" );
    QUERY->add_arg( QUERY, "float", "x" );

    // asin
    QUERY->add_sfun( QUERY, asin_impl, "float", "asin" );
    QUERY->add_arg( QUERY, "float", "x" );

    // acos
    QUERY->add_sfun( QUERY, acos_impl, "float", "acos" );
    QUERY->add_arg( QUERY, "float", "x" );

    // atan
    QUERY->add_sfun( QUERY, atan_impl, "float", "atan" );
    QUERY->add_arg( QUERY, "float", "x" );

    // atan2
    QUERY->add_sfun( QUERY, atan2_impl, "float", "atan2" );
    QUERY->add_arg( QUERY, "float", "x" );

    // sinh
    QUERY->add_sfun( QUERY, sinh_impl, "float", "sinh" );
    QUERY->add_arg( QUERY, "float", "x" );

    // cosh
    QUERY->add_sfun( QUERY, cosh_impl, "float", "cosh" );
    QUERY->add_arg( QUERY, "float", "x" );

    // tanh
    QUERY->add_sfun( QUERY, tanh_impl, "float", "tanh" );
    QUERY->add_arg( QUERY, "float", "x" );

    // hypot
    QUERY->add_sfun( QUERY, hypot_impl, "float", "hypot" );
    QUERY->add_arg( QUERY, "float", "x" );
    QUERY->add_arg( QUERY, "float", "y" );

    // pow
    QUERY->add_sfun( QUERY, pow_impl, "float", "pow" );
    QUERY->add_arg( QUERY, "float", "x" );
    QUERY->add_arg( QUERY, "float", "y" );

    // sqrt
    QUERY->add_sfun( QUERY, sqrt_impl, "float", "sqrt" );
    QUERY->add_arg( QUERY, "float", "x" );

    // exp
    QUERY->add_sfun( QUERY, exp_impl, "float", "exp" );
    QUERY->add_arg( QUERY, "float", "x" );

    // log
    QUERY->add_sfun( QUERY, log_impl, "float", "log" );
    QUERY->add_arg( QUERY, "float", "x" );

    // log2
    QUERY->add_sfun( QUERY, log2_impl, "float", "log2" );
    QUERY->add_arg( QUERY, "float", "x" );

    // log10
    QUERY->add_sfun( QUERY, log10_impl, "float", "log10" );
    QUERY->add_arg( QUERY, "float", "x" );

    // floor
    QUERY->add_sfun( QUERY, floor_impl, "float", "floor" );
    QUERY->add_arg( QUERY, "float", "x" );

    // ceil
    QUERY->add_sfun( QUERY, ceil_impl, "float", "ceil" );
    QUERY->add_arg( QUERY, "float", "x" );

    // round
    QUERY->add_sfun( QUERY, round_impl, "float", "round" );
    QUERY->add_arg( QUERY, "float", "x" );

    // trunc
    QUERY->add_sfun( QUERY, trunc_impl, "float", "trunc" );
    QUERY->add_arg( QUERY, "float", "x" );

    // fmod
    QUERY->add_sfun( QUERY, fmod_impl, "float", "fmod" );
    QUERY->add_arg( QUERY, "float", "x" );
    QUERY->add_arg( QUERY, "float", "y" );

    // remainder
    QUERY->add_sfun( QUERY, remainder_impl, "float", "remainder" );
    QUERY->add_arg( QUERY, "float", "x" );
    QUERY->add_arg( QUERY, "float", "y" );

    // min
    QUERY->add_sfun( QUERY, min_impl, "float", "min" );
    QUERY->add_arg( QUERY, "float", "x" );
    QUERY->add_arg( QUERY, "float", "y" );

    // max
    //! see \example powerup.ck
    QUERY->add_sfun( QUERY, max_impl, "float", "max" );
    QUERY->add_arg( QUERY, "float", "x" );
    QUERY->add_arg( QUERY, "float", "y" );

    // nextpow2
    QUERY->add_sfun( QUERY, nextpow2_impl, "int", "nextpow2" );
    QUERY->add_arg( QUERY, "int", "n" );

    // rand
    QUERY->add_sfun( QUERY, rand_impl, "int", "rand" ); //! return int between 0 and RAND_MAX
    
    // rand2
    QUERY->add_sfun( QUERY, rand2_impl, "int", "rand2" ); //! integer between [min,max]
    QUERY->add_arg( QUERY, "int", "min" ); 
    QUERY->add_arg( QUERY, "int", "max" ); 
    
    // randf
    QUERY->add_sfun( QUERY, randf_impl, "float", "randf" ); //! rand between -1.0,1.0

    // rand2f
    QUERY->add_sfun( QUERY, rand2f_impl, "float", "rand2f" ); //! rand between min and max
    QUERY->add_arg( QUERY, "float", "min" );
    QUERY->add_arg( QUERY, "float", "max" );

    // add mtof
    //! see \example mand-o-matic.ck
    QUERY->add_sfun( QUERY, mtof_impl, "float", "mtof" ); //! midi note to frequency
    QUERY->add_arg( QUERY, "float", "value" );

    // add ftom
    QUERY->add_sfun( QUERY, ftom_impl, "float", "ftom" ); //! frequency to midi note
    QUERY->add_arg( QUERY, "float", "value" );

    // add powtodb
    QUERY->add_sfun( QUERY, powtodb_impl, "float", "powtodb" ); //! linear power to decibel 
    QUERY->add_arg( QUERY, "float", "value" );

    // add rmstodb
    QUERY->add_sfun( QUERY, rmstodb_impl, "float", "rmstodb" ); //! rms to decibel
    QUERY->add_arg( QUERY, "float", "value" );

    // add dbtopow
    QUERY->add_sfun( QUERY, dbtopow_impl, "float", "dbtopow" ); //! decibel to linear
    QUERY->add_arg( QUERY, "float", "value" );

    // add dbtorms
    QUERY->add_sfun( QUERY, dbtorms_impl, "float", "dbtorms" ); //! decibel to rms
    QUERY->add_arg( QUERY, "float", "value" );
    
    // pi
    //! see \example math.ck
    QUERY->add_svar( QUERY, "float", "pi", TRUE, &g_pi );

    // twopi
    QUERY->add_svar( QUERY, "float", "twopi", TRUE, &g_twopi );

    // e
    QUERY->add_svar( QUERY, "float", "e", TRUE, &g_e );

    // done
    QUERY->end_class( QUERY );

    return TRUE;
}

// sin
CK_DLL_SFUN( sin_impl )
{
    t_CKFLOAT f = GET_CK_FLOAT(ARGS);
    RETURN->v_float = ::sin( GET_CK_FLOAT(ARGS) );
}

// cos
CK_DLL_SFUN( cos_impl )
{
    RETURN->v_float = ::cos( GET_CK_FLOAT(ARGS) );
}

// tan
CK_DLL_SFUN( tan_impl )
{
    RETURN->v_float = ::tan( GET_CK_FLOAT(ARGS) );
}

// asin
CK_DLL_SFUN( asin_impl )
{
    RETURN->v_float = ::asin( GET_CK_FLOAT(ARGS) );
}

// acos
CK_DLL_SFUN( acos_impl )
{
    RETURN->v_float = ::acos( GET_CK_FLOAT(ARGS) );
}

// atan
CK_DLL_SFUN( atan_impl )
{
    RETURN->v_float = ::atan( GET_CK_FLOAT(ARGS) );
}

// atan2
CK_DLL_SFUN( atan2_impl )
{
    t_CKFLOAT x = GET_CK_FLOAT(ARGS);
    t_CKFLOAT y = *((t_CKFLOAT *)ARGS + 1 );
    RETURN->v_float = ::atan2( x, y );
}

// sinh
CK_DLL_SFUN( sinh_impl )
{
    RETURN->v_float = ::sinh( GET_CK_FLOAT(ARGS) );
}

// cosh
CK_DLL_SFUN( cosh_impl )
{
    RETURN->v_float = ::cosh( GET_CK_FLOAT(ARGS) );
}

// tanh
CK_DLL_SFUN( tanh_impl )
{
    RETURN->v_float = ::tanh( GET_CK_FLOAT(ARGS) );
}

// hypot
CK_DLL_SFUN( hypot_impl )
{
    t_CKFLOAT x = GET_CK_FLOAT(ARGS);
    t_CKFLOAT y = *((t_CKFLOAT *)ARGS + 1);
    RETURN->v_float = ::hypot( x, y );
}

// pow
CK_DLL_SFUN( pow_impl )
{
    t_CKFLOAT x = GET_CK_FLOAT(ARGS);
    t_CKFLOAT y = *((t_CKFLOAT *)ARGS + 1);
    RETURN->v_float = ::pow( x, y );
}

// sqrt
CK_DLL_SFUN( sqrt_impl )
{
    RETURN->v_float = ::sqrt( GET_CK_FLOAT(ARGS) );
}

// exp
CK_DLL_SFUN( exp_impl )
{
    RETURN->v_float = ::exp( GET_CK_FLOAT(ARGS) );
}

// log
CK_DLL_SFUN( log_impl )
{
    RETURN->v_float = ::log( GET_CK_FLOAT(ARGS) );
}

// log2
CK_DLL_SFUN( log2_impl )
{
    RETURN->v_float = ::log( GET_CK_FLOAT(ARGS) )/::log( 2.0 );
}

// log10
CK_DLL_SFUN( log10_impl )
{
    RETURN->v_float = ::log10( GET_CK_FLOAT(ARGS) );
}

// floor
CK_DLL_SFUN( floor_impl )
{
    RETURN->v_float = ::floor( GET_CK_FLOAT(ARGS) );
}

// ceil
CK_DLL_SFUN( ceil_impl )
{
    RETURN->v_float = ::ceil( GET_CK_FLOAT(ARGS) );
}

// round
CK_DLL_SFUN( round_impl )
{
    RETURN->v_float = round( GET_CK_FLOAT(ARGS) );
}

// trunc
CK_DLL_SFUN( trunc_impl )
{
    RETURN->v_float = trunc( GET_CK_FLOAT(ARGS) );
}

// fmod
CK_DLL_SFUN( fmod_impl )
{
    t_CKFLOAT x = GET_CK_FLOAT(ARGS);
    t_CKFLOAT y = *((t_CKFLOAT *)ARGS + 1);
    RETURN->v_float = fmod( x, y );
}

// remainder
CK_DLL_SFUN( remainder_impl )
{
    t_CKFLOAT x = GET_CK_FLOAT(ARGS);
    t_CKFLOAT y = *((t_CKFLOAT *)ARGS + 1);
    RETURN->v_float = remainder( x, y );
}

// min
CK_DLL_SFUN( min_impl )
{
    t_CKFLOAT x = GET_CK_FLOAT(ARGS);
    t_CKFLOAT y = *((t_CKFLOAT *)ARGS + 1);
    RETURN->v_float = x < y ? x : y;
}

// max
CK_DLL_SFUN( max_impl )
{
    t_CKFLOAT x = GET_CK_FLOAT(ARGS);
    t_CKFLOAT y = *((t_CKFLOAT *)ARGS + 1);
    RETURN->v_float = x > y ? x : y;
}

// nextpow2 - thanks to Niklas Werner, via music-dsp
CK_DLL_SFUN( nextpow2_impl )
{
    t_CKINT x = GET_CK_INT(ARGS);
    t_CKINT xx = x;
    for( ; x &= x-1; xx = x );
    RETURN->v_int = xx * 2;
}
