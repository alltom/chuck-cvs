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
// file: ulib_std.h
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#ifndef __ULIB_STD_H__
#define __ULIB_STD_H__

#include "chuck_dl.h"

// query
DLL_QUERY libstd_query( Chuck_DL_Query * QUERY );

// exports
CK_DLL_SFUN( abs_impl );
CK_DLL_SFUN( fabs_impl );
CK_DLL_SFUN( rand_impl );
CK_DLL_SFUN( randf_impl );
CK_DLL_SFUN( rand2f_impl );
CK_DLL_SFUN( rand2_impl );
CK_DLL_SFUN( srand_impl );
CK_DLL_SFUN( sgn_impl );
CK_DLL_SFUN( system_impl );
CK_DLL_SFUN( atoi_impl );
CK_DLL_SFUN( atof_impl );
CK_DLL_SFUN( itoa_impl );
CK_DLL_SFUN( ftoa_impl );
CK_DLL_SFUN( getenv_impl );
CK_DLL_SFUN( setenv_impl );

CK_DLL_SFUN( mtof_impl );
CK_DLL_SFUN( ftom_impl );
CK_DLL_SFUN( powtodb_impl );
CK_DLL_SFUN( rmstodb_impl );
CK_DLL_SFUN( dbtopow_impl );
CK_DLL_SFUN( dbtorms_impl );


#endif
