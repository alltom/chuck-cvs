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
// file: ulib_glu.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#include "ulib_glu.h"

#if defined(__MACOSX_CORE__)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif



//-----------------------------------------------------------------------------
// name: glu_query()
// desc: query entry point
//-----------------------------------------------------------------------------
DLL_QUERY glu_query( Chuck_DL_Query * QUERY )
{
    QUERY->set_name( QUERY, "glu" );
    
    // add LookAt
    QUERY->add_export( QUERY, "void", "LookAt", gl_Begin_impl, TRUE );
    QUERY->add_param( QUERY, "float", "eye_x" );
    QUERY->add_param( QUERY, "float", "eye_y" );
    QUERY->add_param( QUERY, "float", "eye_z" );
    QUERY->add_param( QUERY, "float", "at_x" );
    QUERY->add_param( QUERY, "float", "at_y" );
    QUERY->add_param( QUERY, "float", "at_z" );
    QUERY->add_param( QUERY, "float", "up_x" );
    QUERY->add_param( QUERY, "float", "up_y" );
    QUERY->add_param( QUERY, "float", "up_z" );
    
    return TRUE;
}


// LookAt
CK_DLL_FUNC( glu_LookAt_impl )
{
}
