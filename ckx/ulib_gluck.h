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

//functions to toggle what events we track
CK_DLL_FUNC( gluck_CatchMouse_impl );
CK_DLL_FUNC( gluck_CatchMotion_impl );
CK_DLL_FUNC( gluck_CatchKeyboard_impl );

//functions to help shred loops work while
//we see about doing a callback-type thing 
//in chucK..

CK_DLL_FUNC( gluck_NeedDraw_impl );
CK_DLL_FUNC( gluck_NeedIdle_impl );
CK_DLL_FUNC( gluck_NeedEvent_impl );

//map standard glut calls for completeness
CK_DLL_FUNC( gluck_Init_impl );
CK_DLL_FUNC( gluck_InitWindowPosition_impl );
CK_DLL_FUNC( gluck_InitWindowSize_impl );
CK_DLL_FUNC( gluck_InitDisplayMode_impl );
CK_DLL_FUNC( gluck_InitDisplayString_impl );
CK_DLL_FUNC( gluck_InitWindowPosition_impl );

CK_DLL_FUNC( gluck_MainLoopEvent_impl );
//CK_DLL_FUNC( gluck_MainLoop_impl );  //bad!

CK_DLL_FUNC( gluck_CreateWindow_impl );
CK_DLL_FUNC( gluck_DestroyWindow_impl );
CK_DLL_FUNC( gluck_SetWindow_impl );
CK_DLL_FUNC( gluck_SetWindowTitle_impl );
CK_DLL_FUNC( gluck_ReshapeWindow_impl );
CK_DLL_FUNC( gluck_PositionWindow_impl );
CK_DLL_FUNC( gluck_ShowWindow_impl );
CK_DLL_FUNC( gluck_HideWindow_impl );
CK_DLL_FUNC( gluck_FullScreen_impl );

//display calls...
CK_DLL_FUNC( gluck_PostWindowRedisplay_impl );
CK_DLL_FUNC( gluck_PostRedisplay_impl );
CK_DLL_FUNC( gluck_SwapBuffers_impl );

//pass on overlay

//passing on the menu functions.. 

//don't know what to do about assigning callback functions
//those ought to be interior to gluck, who 
//provides a more chuck-like exterior to the user..


//font
CK_DLL_FUNC ( gluck_StrokeCharacter_impl );
CK_DLL_FUNC ( gluck_StrokeWidth_impl );
CK_DLL_FUNC ( gluck_StrokeLength_impl );

//shapes
CK_DLL_FUNC ( gluck_WireTeapot_impl );
CK_DLL_FUNC ( gluck_SolidTeapot_impl );

#endif
