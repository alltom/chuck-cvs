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


#if __WINDOWS_DS__
#include <windows.h>
extern int round( double a );

int setenv( char *n, char *v, int i )
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
    QUERY->set_name( QUERY, "std" );
    
    // add abs
    QUERY->add_export( QUERY, "float", "abs", abs_impl, TRUE );
    QUERY->add_param( QUERY, "float", "value" );
    
    // add rand
    QUERY->add_export( QUERY, "int", "rand", rand_impl, TRUE );
    QUERY->add_export( QUERY, "int", "rand2", rand2_impl, TRUE );
    QUERY->add_param( QUERY, "int", "min" );
    QUERY->add_param( QUERY, "int", "max" );
    QUERY->add_export( QUERY, "float", "randf", randf_impl, TRUE );
    QUERY->add_export( QUERY, "float", "rand2f", rand2f_impl, TRUE );
    QUERY->add_param( QUERY, "float", "min" );
    QUERY->add_param( QUERY, "float", "max" );

    // add sgn
    QUERY->add_export( QUERY, "float", "sgn", sgn_impl, TRUE );
    QUERY->add_param( QUERY, "float", "value" );

    // add system
    QUERY->add_export( QUERY, "int", "system", system_impl, TRUE );
    QUERY->add_param( QUERY, "string", "cmd" );

    // add atoi
    QUERY->add_export( QUERY, "int", "atoi", atoi_impl, TRUE );
    QUERY->add_param( QUERY, "string", "value" );

    // add atof
    QUERY->add_export( QUERY, "float", "atof", atof_impl, TRUE );
    QUERY->add_param( QUERY, "string", "value" );

    // add getenv
    QUERY->add_export( QUERY, "string", "getenv", getenv_impl, TRUE );
    QUERY->add_param( QUERY, "string", "value" );

    // add setenv
    QUERY->add_export( QUERY, "int", "setenv", setenv_impl, TRUE );
    QUERY->add_param( QUERY, "string", "key" );
    QUERY->add_param( QUERY, "string", "value" );
    
    // seed the rand
    srand( time( NULL ) );

    // add mtof
    QUERY->add_export( QUERY, "float", "mtof", mtof_impl, TRUE );
    QUERY->add_param( QUERY, "float", "value" );

    // add mtof
    QUERY->add_export( QUERY, "float", "ftom", ftom_impl, TRUE );
    QUERY->add_param( QUERY, "float", "value" );

    // add mtof
    QUERY->add_export( QUERY, "float", "powtodb", powtodb_impl, TRUE );
    QUERY->add_param( QUERY, "float", "value" );

    // add mtof
    QUERY->add_export( QUERY, "float", "rmstodb", rmstodb_impl, TRUE );
    QUERY->add_param( QUERY, "float", "value" );

    // add mtof
    QUERY->add_export( QUERY, "float", "dbtopow", dbtopow_impl, TRUE );
    QUERY->add_param( QUERY, "float", "value" );

    // add mtof
    QUERY->add_export( QUERY, "float", "dbtorms", dbtorms_impl, TRUE );
    QUERY->add_param( QUERY, "float", "value" );
    
    return TRUE;
}


// abs
CK_DLL_FUNC( abs_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = v >= 0.0f ? v : -v;
}

// fabs
CK_DLL_FUNC( fabs_impl )
{
    RETURN->v_float = ::fabs( GET_CK_FLOAT(ARGS) );
}

// rand
CK_DLL_FUNC( rand_impl )
{
    RETURN->v_int = ::rand();
}

// randf
CK_DLL_FUNC( randf_impl )
{
    RETURN->v_float = ( 2.0 * ::rand() / (t_CKFLOAT)RAND_MAX - 1.0 );
}

// randf
CK_DLL_FUNC( rand2f_impl )
{
    t_CKFLOAT min = GET_CK_FLOAT(ARGS), max = *((t_CKFLOAT *)ARGS + 1);
    RETURN->v_float = min + (max-min)*(::rand()/(t_CKFLOAT)RAND_MAX);
}

// randi
CK_DLL_FUNC( rand2_impl )
{
    int min = *(int *)ARGS, max = *((int *)ARGS + 1);
    RETURN->v_int = (int)round((t_CKFLOAT)min + (max-min)*(rand()/(t_CKFLOAT)RAND_MAX));
}

// sgn
CK_DLL_FUNC( sgn_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = v == 0.0f ? 0.0f : ( v > 0.0f ? 1.0f : -1.0f );
}

// system
CK_DLL_FUNC( system_impl )
{
    char * cmd = *(char **)ARGS;
    RETURN->v_int = system( cmd );
}

// aoti
CK_DLL_FUNC( atoi_impl )
{
    char * v = *(char **)ARGS;
    RETURN->v_int = atoi( v );
}

// atof
CK_DLL_FUNC( atof_impl )
{
    char * v = *(char **)ARGS;
    RETURN->v_float = atof( v );
}

// getenv
CK_DLL_FUNC( getenv_impl )
{
    char * v = *(char **)ARGS;
    RETURN->v_string = getenv( v );
}

// setenv
CK_DLL_FUNC( setenv_impl )
{
    char * v1 = *(char **)ARGS;
    char * v2 = *( (char **)ARGS + 1 );
    RETURN->v_int = setenv( v1, v2, 1 );
}

// the following 6 functions are 
// lifted from  PD source
// specifically x_acoustics.c
// http://puredata.info/downloads

#define LOGTEN 2.302585092994

float mtof(float f)
{
    if (f <= -1500) return(0);
    else if (f > 1499) return(mtof(1499));
    else return (float)(8.17579891564 * exp(.0577622650 * (double)f));
}

CK_DLL_FUNC( mtof_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = mtof(v);
}

float ftom(float f)
{
    return (f > 0 ? 17.3123405046 * log(.12231220585 * f) : -1500);
}

CK_DLL_FUNC( ftom_impl )
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


CK_DLL_FUNC( powtodb_impl )
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

CK_DLL_FUNC( rmstodb_impl )
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

CK_DLL_FUNC( dbtopow_impl )
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

CK_DLL_FUNC( dbtorms_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = dbtorms(v);
}


