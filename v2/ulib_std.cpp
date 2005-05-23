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
// file: ulib_std.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#include "ulib_std.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "util_math.h"

#if __WINDOWS_DS__
#include <windows.h>

int setenv( const char *n, const char *v, int i )
{
    return SetEnvironmentVariable(n, v);
}

#endif


//-----------------------------------------------------------------------------
// name: libstd_query()
// desc: query entry point
//-----------------------------------------------------------------------------
DLL_QUERY libstd_query( Chuck_DL_Query * QUERY )
{
    // set name
    QUERY->setname( QUERY, "Std" );

    /*! \example
    std.rand2f( 100.0, 1000.0 ) => stdout;
    */

    // begin class
    QUERY->begin_class( QUERY, "std", "Object" );

    // add abs
    QUERY->add_sfun( QUERY, abs_impl, "float", "abs" );
    QUERY->add_arg( QUERY, "float", "value" );
    
    // add rand
    QUERY->add_sfun( QUERY, rand_impl, "int", "rand"); //! return int between 0 and RAND_MAX
    
    // add rand2
    QUERY->add_sfun( QUERY, rand2_impl, "int", "rand2" ); //! integer between [min,max]
    QUERY->add_arg( QUERY, "int", "min" ); 
    QUERY->add_arg( QUERY, "int", "max" ); 
    
    // add rand
    QUERY->add_sfun( QUERY, randf_impl, "float", "randf" ); //! rand between -1.0,1.0

    // add rand2
    QUERY->add_sfun( QUERY, rand2f_impl, "float", "rand2f" ); //! rand between min and max
    QUERY->add_arg( QUERY, "float", "min" );
    QUERY->add_arg( QUERY, "float", "max" );

    // add sgn
    QUERY->add_sfun( QUERY, sgn_impl, "float", "sgn" ); //! return sign of value (-1, 0, 1)
    QUERY->add_arg( QUERY, "float", "value" );

    // add system
    //! see \example std.ck
    QUERY->add_sfun( QUERY, system_impl, "int", "system" ); //! issue system command
    QUERY->add_arg( QUERY, "string", "cmd" );

    // add atoi
    QUERY->add_sfun( QUERY, atoi_impl, "int", "atoi" ); //! string to integer
    QUERY->add_arg( QUERY, "string", "value" );

    // add atof
    QUERY->add_sfun( QUERY, atof_impl, "float", "atof" ); //! string to float
    QUERY->add_arg( QUERY, "string", "value" );

    // add getenv
    QUERY->add_sfun( QUERY, getenv_impl, "string", "getenv" ); //! fetch environment variable
    QUERY->add_arg( QUERY, "string", "value" );

    // add setenv
    QUERY->add_sfun( QUERY, setenv_impl, "int", "setenv" ); //! set environment variable
    QUERY->add_arg( QUERY, "string", "key" );
    QUERY->add_arg( QUERY, "string", "value" );

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

    // finish class
    QUERY->end_class( QUERY );

    // seed the rand
    srand( time( NULL ) );

    return TRUE;
}

#define RAND_INV_RANGE(r) (RAND_MAX / (r))

int irand_exclusive ( int max ) { 
  int x = ::rand();
  
  while (x >= max * RAND_INV_RANGE (max)) 
    x = ::rand();

  x /= RAND_INV_RANGE (max);
  return x;
}


// abs
CK_DLL_SFUN( abs_impl )
{
  t_CKFLOAT v = GET_CK_FLOAT(ARGS);
  RETURN->v_float = v >= 0.0f ? v : -v;
}

// fabs
CK_DLL_SFUN( fabs_impl )
{
    RETURN->v_float = ::fabs( GET_CK_FLOAT(ARGS) );
}

// rand
CK_DLL_SFUN( rand_impl )
{
    RETURN->v_int = ::rand();
}

// randf
CK_DLL_SFUN( randf_impl )
{
    RETURN->v_float = ( 2.0 * ::rand() / (t_CKFLOAT)RAND_MAX - 1.0 );
}

// randf
CK_DLL_SFUN( rand2f_impl )
{
    t_CKFLOAT min = GET_CK_FLOAT(ARGS), max = *((t_CKFLOAT *)ARGS + 1);
    RETURN->v_float = min + (max-min)*(::rand()/(t_CKFLOAT)RAND_MAX);
}

// randi
CK_DLL_SFUN( rand2_impl ) // inclusive.
{
    int min = *(int *)ARGS, max = *((int *)ARGS + 1);
    int range = max - min; 
    if ( range == 0 ) { 
     RETURN->v_int = min;
    }
    //else if ( range < RAND_MAX / 2 ) { 
    //  RETURN->v_int = ( range > 0 ) ? min + irand_exclusive(1 + range) : max + irand_exclusive ( -range + 1 ) ;
    //}
    else { 
      if ( range > 0 ) { 
	RETURN->v_int = min + (int) ( (1.0 + range) * ( ::rand()/(RAND_MAX+1.0) ) );
      }
      else { 
	RETURN->v_int = min - (int) ( (-range + 1.0) * ( ::rand()/(RAND_MAX+1.0) ) );
      }
    }
}

// sgn
CK_DLL_SFUN( sgn_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = v == 0.0f ? 0.0f : ( v > 0.0f ? 1.0f : -1.0f );
}

// system
CK_DLL_SFUN( system_impl )
{
    const char * cmd = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_int = system( cmd );
}

// aoti
CK_DLL_SFUN( atoi_impl )
{
    const char * v = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_int = atoi( v );
}

// atof
CK_DLL_SFUN( atof_impl )
{
    const char * v = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_float = atof( v );
}

// getenv
static Chuck_String g_str; // PROBLEM: not thread friendly
CK_DLL_SFUN( getenv_impl )
{
    const char * v = GET_CK_STRING(ARGS)->str.c_str();
    g_str.str = getenv( v );
    RETURN->v_string = &g_str;
}

// setenv
CK_DLL_SFUN( setenv_impl )
{
    const char * v1 = GET_NEXT_STRING(ARGS)->str.c_str();
    const char * v2 = GET_NEXT_STRING(ARGS)->str.c_str();
    RETURN->v_int = setenv( v1, v2, 1 );
}

// the following 6 functions are 
// lifted from  PD source
// specifically x_acoustics.c
// http://puredata.info/downloads

#define LOGTEN 2.302585092994

double mtof(double f)
{
    if (f <= -1500) return(0);
    else if (f > 1499) return(mtof(1499));
    else return (float)(8.17579891564 * exp(.0577622650 * (double)f));
}

CK_DLL_SFUN( mtof_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = mtof(v);
}

double ftom(double f)
{
    return (f > 0 ? 17.3123405046 * log(.12231220585 * f) : -1500);
}

CK_DLL_SFUN( ftom_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = ftom(v);
}


float powtodb(float f)
{
    if (f <= 0) return (0);
    else
    {
    	float val = 100 + 10./LOGTEN * log(f);
    	return (val < 0 ? 0 : val);
    }
}


CK_DLL_SFUN( powtodb_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = powtodb(v);
}


float rmstodb(float f)
{
    if (f <= 0) return (0);
    else
    {
    	float val = 100 + 20./LOGTEN * log(f);
    	return (val < 0 ? 0 : val);
    }
}

CK_DLL_SFUN( rmstodb_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = rmstodb(v);
}


float dbtopow(float f)
{
    if (f <= 0)
    	return(0);
    else
    {
    	if (f > 870)
	    f = 870;
    	return (exp((LOGTEN * 0.1) * (f-100.)));
    }
}

CK_DLL_SFUN( dbtopow_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = dbtopow(v);
}

float dbtorms(float f)
{
    if (f <= 0)
    	return(0);
    else
    {
    	if (f > 485)
	    f = 485;
    }
    return (exp((LOGTEN * 0.05) * (f-100.)));
}

CK_DLL_SFUN( dbtorms_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = dbtorms(v);
}


