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
// file: ulib_gluck.h
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Philip L. Davidson (philipd@alumni.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#ifndef __ULIB_GLUCK_H__
#define __ULIB_GLUCK_H__

#include "chuck_dl.h"

// query
//DLL_QUERY gluck_query( Chuck_DL_Query * QUERY );
CK_DLL_QUERY;

// exports
CK_DLL_FUNC( gluck__impl );

//gluck 'shortcut' functions
CK_DLL_FUNC( gluck_BasicWindow_impl ); // -arg for name, - makes double buff, RGBA, depth, 0,0, 640 x 480
CK_DLL_FUNC( gluck_SizedWindow_impl ); // -arg for name, x, y, w,h - basicwindow with size
CK_DLL_FUNC( gluck_FullScreenWindow_impl ); // -arg for name, x, y - fullscreen mode

//map standard glut calls for completeness
CK_DLL_FUNC( gluck_Init_impl );
CK_DLL_FUNC( gluck_CreateWindow_impl );
CK_DLL_FUNC( gluck_InitDisplayMode_impl );
CK_DLL_FUNC( gluck_Init_impl );
CK_DLL_FUNC( gluck_Init_impl );
CK_DLL_FUNC( gluck_Init_impl );
CK_DLL_FUNC( gluck_Init_impl );
CK_DLL_FUNC( gluck_Init_impl );


#endif
