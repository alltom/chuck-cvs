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
// name: util_math.h
// desc: a mini-compatibility library for math functions.
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Philip Davidson (philipd@alumni.princeton.edu)
//-----------------------------------------------------------------------------
#ifndef __UTIL_MATH_H__
#define __UTIL_MATH_H__

#include <math.h>


#ifdef __PLATFORM_WIN32__

#if defined (__cplusplus) || defined(_cplusplus)  
extern "C" {
#endif 

    double round(double a);
    double trunc(double a);
    double remainder(int a, int b);

#if defined (__cplusplus) || defined(_cplusplus)  
}
#endif

#endif


#endif 
