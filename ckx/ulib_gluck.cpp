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
// file: ulib_gluck.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Philip L. Davidson (philipd@alumni.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#include "ulib_gluck.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>


// lazy phil's preprocessor functions
// if these shouldn't be here, tell me
#define GLUCK_EXPORT(t, n) QUERY->add_export( QUERY, #t, #n, gluck_##n##_impl, TRUE )
#define GLUCK_PARAM(t, n)  QUERY->add_param ( QUERY, #t, #n )

//-----------------------------------------------------------------------------
// name: gluck_query()
// desc: query entry point
//-----------------------------------------------------------------------------
//DLL_QUERY gluck_query( Chuck_DL_Query * QUERY )
CK_DLL_QUERY
{
    QUERY->set_name( QUERY, "gluck" );
    
    return TRUE;

    GLUCK_EXPORT ( void, Init );
    //Init should take argc, argv...but not yet..

    GLUCK_EXPORT ( void, InitWindowPosition );
    GLUCK_PARAM  ( int , x );
    GLUCK_PARAM  ( int , y );

    GLUCK_EXPORT ( void, InitWindowSize );
    GLUCK_PARAM  ( int , w );
    GLUCK_PARAM  ( int , h );

    GLUCK_EXPORT ( void, InitDisplayMode );
    GLUCK_PARAM  ( uint , displayMode );

    GLUCK_EXPORT ( void, InitDisplayString );
    GLUCK_PARAM  ( uint , displayMode );
    
    GLUCK_EXPORT ( void, MainLoopEvent );

    GLUCK_EXPORT ( int, CreateWindow );
    GLUCK_PARAM  ( uint , title );

    GLUCK_EXPORT ( void, DestroyWindow );
    GLUCK_PARAM  ( int , window );

    GLUCK_EXPORT ( void, SetWindow );
    GLUCK_PARAM  ( int , window );

    GLUCK_EXPORT ( void, SetWindowTitle );
    GLUCK_PARAM  ( uint , title );

    GLUCK_EXPORT ( void, ReshapeWindow );
    GLUCK_PARAM  ( int , width );
    GLUCK_PARAM  ( int , height );

    GLUCK_EXPORT ( void, PositionWindow );
    GLUCK_PARAM  ( int , x );
    GLUCK_PARAM  ( int , y );

    GLUCK_EXPORT ( void, ShowWindow );

    GLUCK_EXPORT ( void, HideWindow );

    GLUCK_EXPORT ( void, FullScreen );

    GLUCK_EXPORT ( void, PostWindowRedisplay );
    GLUCK_PARAM  ( int , window );

    GLUCK_EXPORT ( void, PostRedisplay );

    GLUCK_EXPORT ( void, SwapBuffers );

    GLUCK_EXPORT ( void, StrokeCharacter );
    GLUCK_PARAM  ( int , font );
    GLUCK_PARAM  ( int , character );

    GLUCK_EXPORT ( int, StrokeWidth );
    GLUCK_PARAM  ( int , font );
    GLUCK_PARAM  ( int , character );

    GLUCK_EXPORT ( int, StrokeLength );
    GLUCK_PARAM  ( int , font );
    GLUCK_PARAM  ( uint , string );

    GLUCK_EXPORT ( void, WireTeapot );
    GLUCK_PARAM  ( float , size );

    GLUCK_EXPORT ( void, SolidTeapot );
    GLUCK_PARAM  ( float , size );

}


//
CK_DLL_FUNC( gluck_Init_impl )
{
  char base[] = "chuck";
  int  num = 1;
  glutInit(&num, (char**)&base);
}

CK_DLL_FUNC( gluck_InitWindowPosition_impl )
{
  int x = GET_CK_INT_N(ARGS,0);
  int y = GET_CK_INT_N(ARGS,1);
  glutInitWindowPosition(x, y);
}

CK_DLL_FUNC( gluck_InitWindowSize_impl )
{
  int w = GET_CK_INT_N(ARGS,0);
  int h = GET_CK_INT_N(ARGS,1);
  glutInitWindowPosition(w, h);
}

CK_DLL_FUNC( gluck_InitDisplayMode_impl )
{
  t_CKUINT mode = GET_CK_UINT_N(ARGS,0);
  glutInitDisplayMode(mode);
}


CK_DLL_FUNC( gluck_InitDisplayString_impl )
{
  t_CKUINT mode = GET_CK_UINT_N(ARGS,0);
  glutInitDisplayString((char*)mode);
}


CK_DLL_FUNC( gluck_MainLoopEvent_impl )
{
  glutMainLoopEvent();
}

CK_DLL_FUNC( gluck_CreateWindow_impl )
{
  //  t_CKSTRING mode = GET_CK_STRING_N(ARGS,0);
  t_CKUINT ctitle = GET_CK_UINT_N (ARGS,0 );
  char title[] = "=gluck>";
  RETURN->v_int = glutCreateWindow( (char*)title );
}

CK_DLL_FUNC( gluck_DestroyWindow_impl )
{
  int window = GET_CK_INT_N(ARGS,0);
  glutDestroyWindow( window );
}

CK_DLL_FUNC( gluck_SetWindow_impl )
{
  int window = GET_CK_INT_N(ARGS,0);
  glutSetWindow(window);
}

CK_DLL_FUNC( gluck_SetWindowTitle_impl )
{
  t_CKUINT ctitle = GET_CK_UINT_N(ARGS,0);
  char title[] = "gluck!"; //no chance here
  glutSetWindowTitle((char*)title);
}

CK_DLL_FUNC( gluck_ReshapeWindow_impl )
{
  int width = GET_CK_INT_N(ARGS,0);
  int height = GET_CK_INT_N(ARGS,1);
  glutReshapeWindow(width, height);
}

CK_DLL_FUNC( gluck_PositionWindow_impl )
{
  int x = GET_CK_INT_N(ARGS,0);
  int y = GET_CK_INT_N(ARGS,1);
  glutPositionWindow(x,y);
}

CK_DLL_FUNC( gluck_ShowWindow_impl )
{
  glutShowWindow();
}

CK_DLL_FUNC( gluck_HideWindow_impl )
{
  glutHideWindow();
}

CK_DLL_FUNC( gluck_FullScreen_impl )
{
  glutFullScreen();
}

CK_DLL_FUNC( gluck_PostWindowRedisplay_impl )
{
  int window = GET_CK_INT_N(ARGS,0);
  glutPostWindowRedisplay(window);
}

CK_DLL_FUNC( gluck_PostRedisplay_impl )
{
  glutPostRedisplay();
}

CK_DLL_FUNC( gluck_SwapBuffers_impl )
{
  glutSwapBuffers();
}

CK_DLL_FUNC( gluck_StrokeCharacter_impl )
{
  t_CKUINT font = GET_CK_UINT_N(ARGS,0);
  int c = GET_CK_INT_N(ARGS,1);

  glutStrokeCharacter((void*)&font, c);
}

CK_DLL_FUNC( gluck_StrokeWidth_impl )
{
  t_CKUINT font = GET_CK_UINT_N(ARGS,0);
  int c = GET_CK_INT_N(ARGS,1);
  RETURN->v_int = glutStrokeWidth( (void*)&font, c);
}

CK_DLL_FUNC( gluck_StrokeLength_impl )
{  
  t_CKUINT font = GET_CK_UINT_N(ARGS,0);
  t_CKUINT str = GET_CK_UINT_N(ARGS,1);

  RETURN->v_int = glutStrokeLength( (void*)&font, (unsigned char*)str);
}

CK_DLL_FUNC( gluck_WireTeapot_impl )
{
  t_CKFLOAT size = GET_CK_FLOAT_N(ARGS,0);
  glutWireTeapot(size);
}
CK_DLL_FUNC( gluck_SolidTeapot_impl )
{
  t_CKFLOAT size = GET_CK_FLOAT_N(ARGS,0);
  glutSolidTeapot(size);
}
