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
#include <stdlib.h>

#ifdef __MACOSX_CORE__ //macosx glut is sexxxy
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif


#include <vector>
using namespace std;


// lazy phil's preprocessor functions
// if these shouldn't be here, tell me
#define GLUCK_EXPORT(t, n) QUERY->add_export( QUERY, #t, #n, gluck_##n##_impl, TRUE )
#define GLUCK_PARAM(t, n)  QUERY->add_param ( QUERY, #t, #n )



//functions to help shred loops work while
//we see about doing a callback-type thing 
//in chucK..

void gluckDisplayCB();
void gluckIdleCB();
void gluckMouseCB(int button, int state, int x, int y);
void gluckMotionCB( int x, int y);
void gluckPassiveMotionCB( int x, int y);
void gluckKeyboardCB( unsigned char key, int x, int y);
void gluckSpecialCB( int key, int x, int y);
void gluckReshapeCB( int x, int y );

void gluckAddBufferedEvent( int type, int x, int y, int button, int state, unsigned char key, int skey, int mods);
bool gluckHasEvents();
int  gluckGetNextEvent();


enum { EV_MOUSE=0, EV_MOTION, EV_PMOTION, EV_KEY, EV_SPECKEY, EV_NONE};

static uint gluck_EVENT_MOUSE = EV_MOUSE;
static uint gluck_EVENT_MOTION = EV_MOTION;
static uint gluck_EVENT_PMOTION = EV_PMOTION;
static uint gluck_EVENT_KEY = EV_KEY;
static uint gluck_EVENT_SPECKEY = EV_SPECKEY;


/*
 * GLUT API macro definitions -- the special key codes:
 */
static uint glut_KEY_F1	=	0x0001;
static uint glut_KEY_F2	=	0x0002;
static uint glut_KEY_F3	=	0x0003;
static uint glut_KEY_F4	=	0x0004;
static uint glut_KEY_F5 =		0x0005;
static uint glut_KEY_F6 =		0x0006;
static uint glut_KEY_F7 =		0x0007;
static uint glut_KEY_F8 =		0x0008;
static uint glut_KEY_F9 =		0x0009;
static uint glut_KEY_F10 =		0x000A;
static uint glut_KEY_F11 =		0x000B;
static uint glut_KEY_F12 =		0x000C;
static uint glut_KEY_LEFT =		0x0064;
static uint glut_KEY_UP =		0x0065;
static uint glut_KEY_RIGHT =		0x0066;
static uint glut_KEY_DOWN =		0x0067;
static uint glut_KEY_PAGE_UP =		0x0068;
static uint glut_KEY_PAGE_DOWN =		0x0069;
static uint glut_KEY_HOME =		0x006A;
static uint glut_KEY_END =		0x006B;
static uint glut_KEY_INSERT =		0x006C;

/*
 * GLUT API macro definitions -- mouse state definitions
 */
static uint glut_LEFT_BUTTON =		0x0000;
static uint glut_MIDDLE_BUTTON =		0x0001;
static uint glut_RIGHT_BUTTON =		0x0002;
static uint glut_DOWN =		0x0000;
static uint glut_UP =		0x0001;
static uint glut_LEFT =		0x0000;
static uint glut_ENTERED =		0x0001;

/*
 * GLUT API macro definitions -- the display mode definitions
 */
static uint glut_RGB =		0x0000;
static uint glut_RGBA =		0x0000;
static uint glut_INDEX =		0x0001;
static uint glut_SINGLE =		0x0000;
static uint glut_DOUBLE =		0x0002;
static uint glut_ACCUM =		0x0004;
static uint glut_ALPHA =		0x0008;
static uint glut_DEPTH =		0x0010;
static uint glut_STENCIL =		0x0020;
static uint glut_MULTISAMPLE =		0x0080;
static uint glut_STEREO =		0x0100;
static uint glut_LUMINANCE =		0x0200;

/*
 * GLUT API macro definitions -- windows and menu related definitions
 */
static uint glut_MENU_NOT_IN_USE =		0x0000;
static uint glut_MENU_IN_USE =		0x0001;
static uint glut_NOT_VISIBLE =		0x0000;
static uint glut_VISIBLE =		0x0001;
static uint glut_HIDDEN =		0x0000;
static uint glut_FULLY_RETAINED =		0x0001;
static uint glut_PARTIALLY_RETAINED =		0x0002;
static uint glut_FULLY_COVERED =		0x0003;


/*
 * GLUT API macro definitions -- the glutGet parameters
 */
static uint glut_WINDOW_X =		0x0064;
static uint glut_WINDOW_Y =		0x0065;
static uint glut_WINDOW_WIDTH =		0x0066;
static uint glut_WINDOW_HEIGHT =		0x0067;
static uint glut_WINDOW_BUFFER_SIZE =		0x0068;
static uint glut_WINDOW_STENCIL_SIZE =		0x0069;
static uint glut_WINDOW_DEPTH_SIZE =		0x006A;
static uint glut_WINDOW_RED_SIZE =		0x006B;
static uint glut_WINDOW_GREEN_SIZE =		0x006C;
static uint glut_WINDOW_BLUE_SIZE =		0x006D;
static uint glut_WINDOW_ALPHA_SIZE =		0x006E;
static uint glut_WINDOW_ACCUM_RED_SIZE =		0x006F;
static uint glut_WINDOW_ACCUM_GREEN_SIZE =		0x0070;
static uint glut_WINDOW_ACCUM_BLUE_SIZE =		0x0071;
static uint glut_WINDOW_ACCUM_ALPHA_SIZE =		0x0072;
static uint glut_WINDOW_DOUBLEBUFFER =		0x0073;
static uint glut_WINDOW_RGBA =		0x0074;
static uint glut_WINDOW_PARENT =		0x0075;
static uint glut_WINDOW_NUM_CHILDREN =		0x0076;
static uint glut_WINDOW_COLORMAP_SIZE =		0x0077;
static uint glut_WINDOW_NUM_SAMPLES =		0x0078;
static uint glut_WINDOW_STEREO =		0x0079;
static uint glut_WINDOW_CURSOR =		0x007A;

static uint glut_SCREEN_WIDTH =		0x00C8;
static uint glut_SCREEN_HEIGHT =		0x00C9;
static uint glut_SCREEN_WIDTH_MM =		0x00CA;
static uint glut_SCREEN_HEIGHT_MM =		0x00CB;
static uint glut_MENU_NUM_ITEMS =		0x012C;
static uint glut_DISPLAY_MODE_POSSIBLE =		0x0190;
static uint glut_INIT_WINDOW_X =		0x01F4;
static uint glut_INIT_WINDOW_Y =		0x01F5;
static uint glut_INIT_WINDOW_WIDTH =		0x01F6;
static uint glut_INIT_WINDOW_HEIGHT =		0x01F7;
static uint glut_INIT_DISPLAY_MODE =		0x01F8;
static uint glut_ELAPSED_TIME =		0x02BC;
static uint glut_WINDOW_FORMAT_ID =		0x007B;
static uint glut_INIT_STATE =		0x007C;

/*
 * GLUT API macro definitions -- the glutDeviceGet parameters
 */
static uint glut_HAS_KEYBOARD =		0x0258;
static uint glut_HAS_MOUSE =		0x0259;
static uint glut_HAS_SPACEBALL =		0x025A;
static uint glut_HAS_DIAL_AND_BUTTON_BOX =		0x025B;
static uint glut_HAS_TABLET =		0x025C;
static uint glut_NUM_MOUSE_BUTTONS =		0x025D;
static uint glut_NUM_SPACEBALL_BUTTONS =		0x025E;
static uint glut_NUM_BUTTON_BOX_BUTTONS =		0x025F;
static uint glut_NUM_DIALS =		0x0260;
static uint glut_NUM_TABLET_BUTTONS =		0x0261;
static uint glut_DEVICE_IGNORE_KEY_REPEAT =		0x0262;
static uint glut_DEVICE_KEY_REPEAT =		0x0263;
static uint glut_HAS_JOYSTICK =		0x0264;
static uint glut_OWNS_JOYSTICK =		0x0265;
static uint glut_JOYSTICK_BUTTONS =		0x0266;
static uint glut_JOYSTICK_AXES =		0x0267;
static uint glut_JOYSTICK_POLL_RATE =		0x0268;

/*
 * GLUT API macro definitions -- the glutLayerGet parameters
 */
static uint glut_OVERLAY_POSSIBLE =		0x0320;
static uint glut_LAYER_IN_USE =		0x0321;
static uint glut_HAS_OVERLAY =		0x0322;
static uint glut_TRANSPARENT_INDEX =		0x0323;
static uint glut_NORMAL_DAMAGED =		0x0324;
static uint glut_OVERLAY_DAMAGED =		0x0325;

/*
 * GLUT API macro definitions -- the glutVideoResizeGet parameters
 */
static uint glut_VIDEO_RESIZE_POSSIBLE =		0x0384;
static uint glut_VIDEO_RESIZE_IN_USE =		0x0385;
static uint glut_VIDEO_RESIZE_X_DELTA =		0x0386;
static uint glut_VIDEO_RESIZE_Y_DELTA =		0x0387;
static uint glut_VIDEO_RESIZE_WIDTH_DELTA =		0x0388;
static uint glut_VIDEO_RESIZE_HEIGHT_DELTA =		0x0389;
static uint glut_VIDEO_RESIZE_X =		0x038A;
static uint glut_VIDEO_RESIZE_Y =		0x038B;
static uint glut_VIDEO_RESIZE_WIDTH =		0x038C;
static uint glut_VIDEO_RESIZE_HEIGHT =		0x038D;

/*
 * GLUT API macro definitions -- the glutUseLayer parameters
 */
static uint glut_NORMAL =		0x0000;
static uint glut_OVERLAY =		0x0001;

/*
 * GLUT API macro definitions -- the glutGetModifiers parameters
 */
static uint glut_ACTIVE_SHIFT =		0x0001;
static uint glut_ACTIVE_CTRL =		0x0002;
static uint glut_ACTIVE_ALT =		0x0004;

/*
 * GLUT API macro definitions -- the glutSetCursor parameters
 */
static uint glut_CURSOR_RIGHT_ARROW =		0x0000;
static uint glut_CURSOR_LEFT_ARROW =		0x0001;
static uint glut_CURSOR_INFO =		0x0002;
static uint glut_CURSOR_DESTROY =		0x0003;
static uint glut_CURSOR_HELP =		0x0004;
static uint glut_CURSOR_CYCLE =		0x0005;
static uint glut_CURSOR_SPRAY =		0x0006;
static uint glut_CURSOR_WAIT =		0x0007;
static uint glut_CURSOR_TEXT =		0x0008;
static uint glut_CURSOR_CROSSHAIR =		0x0009;
static uint glut_CURSOR_UP_DOWN =		0x000A;
static uint glut_CURSOR_LEFT_RIGHT =		0x000B;
static uint glut_CURSOR_TOP_SIDE =		0x000C;
static uint glut_CURSOR_BOTTOM_SIDE =		0x000D;
static uint glut_CURSOR_LEFT_SIDE =		0x000E;
static uint glut_CURSOR_RIGHT_SIDE =		0x000F;
static uint glut_CURSOR_TOP_LEFT_CORNER =		0x0010;
static uint glut_CURSOR_TOP_RIGHT_CORNER =		0x0011;
static uint glut_CURSOR_BOTTOM_RIGHT_CORNER =		0x0012;
static uint glut_CURSOR_BOTTOM_LEFT_CORNER =		0x0013;
static uint glut_CURSOR_INHERIT =		0x0064;
static uint glut_CURSOR_NONE =		0x0065;
static uint glut_CURSOR_FULL_CROSSHAIR =		0x0066;

/*
 * GLUT API macro definitions -- RGB color component specification definitions
 */
static uint glut_RED =		0x0000;
static uint glut_GREEN =		0x0001;
static uint glut_BLUE =		0x0002;

/*
 * GLUT API macro definitions -- additional keyboard and joystick definitions
 */
static uint glut_KEY_REPEAT_OFF =		0x0000;
static uint glut_KEY_REPEAT_ON =		0x0001;
static uint glut_KEY_REPEAT_DEFAULT =		0x0002;

static uint glut_JOYSTICK_BUTTON_A =		0x0001;
static uint glut_JOYSTICK_BUTTON_B =		0x0002;
static uint glut_JOYSTICK_BUTTON_C =		0x0004;
static uint glut_JOYSTICK_BUTTON_D =		0x0008;

/*
 * GLUT API macro definitions -- game mode definitions
 */
static uint glut_GAME_MODE_ACTIVE =		0x0000;
static uint glut_GAME_MODE_POSSIBLE =		0x0001;
static uint glut_GAME_MODE_WIDTH =		0x0002;
static uint glut_GAME_MODE_HEIGHT =		0x0003;
static uint glut_GAME_MODE_PIXEL_DEPTH =		0x0004;
static uint glut_GAME_MODE_REFRESH_RATE =		0x0005;
static uint glut_GAME_MODE_DISPLAY_CHANGED =		0x0006;      






struct gluckEvent { 
    int type;
    int x;
    int y;
    int button;
    int state;
    unsigned char key;
    int skey;
    int modifiers;
    
    gluckEvent() { 
        type = EV_NONE;
        x = 0;
        y = 0;
        button = 0;
        state = 0;
        key = '\0';
        skey = 0;
    }
};

struct gluckData { 

    bool watchMouse;
    bool watchMotion;
    bool watchKeyboard;
    
    bool needDraw;
    bool needEvent;
    bool needIdle;
    bool needReshape;
    
    int  windowID;
    int  winx;
    int  winy;
    bool doubleBuffered;
    
    int  event_w;
    int  event_r;
    vector <struct gluckEvent> events;
    int curmodifiers;
    int  vp[4];
    
    gluckData( )
    {
        watchMouse = false; 
        watchMotion = false; 
        watchKeyboard = false; 
    
        needDraw = false;
        needEvent = false;
        needIdle = false;
        needReshape = false;
        
        windowID = 0;
        doubleBuffered = false;
        
        events.resize( 2 );
        event_r = 0;
        event_w = 1;
        curmodifiers = 0;
   
	winx = 640;
	winy = 480;

        vp[0] =0;
        vp[1] =0;
        vp[2] =640;
        vp[3] =480;       
    }
};

struct gluckData * gluckstate;



//-----------------------------------------------------------------------------
// name: gluck_query()
// desc: query entry point
//-----------------------------------------------------------------------------
//DLL_QUERY gluck_query( Chuck_DL_Query * QUERY )
CK_DLL_QUERY
{
    QUERY->set_name( QUERY, "gluck" );

    // DISPLAY
    setenv( "DISPLAY", "0:0", 0 );
    
    //gluck functions
    GLUCK_EXPORT ( int, InitBasicWindow );
    GLUCK_PARAM  ( string, name );

    GLUCK_EXPORT ( int, InitSizedWindow );
    GLUCK_PARAM  ( string, name );
    GLUCK_PARAM  ( int, x );
    GLUCK_PARAM  ( int, y );
    GLUCK_PARAM  ( int, w );
    GLUCK_PARAM  ( int, h );
    
    GLUCK_EXPORT ( int, InitFullScreenWindow );
    GLUCK_PARAM  ( string, name );


    //glut loop handlers...
    GLUCK_EXPORT ( int, NeedDraw  );
    GLUCK_EXPORT ( int, NeedEvent );
    GLUCK_EXPORT ( int, NeedIdle  );
    GLUCK_EXPORT ( int, NeedReshape  );

    //glut event watching toggles
    GLUCK_EXPORT ( int, WatchMouse );
    GLUCK_PARAM  ( int, toggle );
    
    GLUCK_EXPORT ( int, WatchMotion );
    GLUCK_PARAM  ( int, toggle );
    
    GLUCK_EXPORT ( int, WatchKeyboard );
    GLUCK_PARAM  ( int, toggle );
    
    GLUCK_EXPORT ( int, InitCallbacks );    
    GLUCK_PARAM  ( int, mouse );
    GLUCK_PARAM  ( int, motion );
    GLUCK_PARAM  ( int, keyboard );
    
    GLUCK_EXPORT ( int, HasEvents );
    //kludgy stuff until we can pass events back out as objects...
    GLUCK_EXPORT ( int, GetNextEvent );
    GLUCK_EXPORT ( int, GetEventType );
    GLUCK_PARAM  ( int, id );
    GLUCK_EXPORT ( int, GetEventX );
    GLUCK_PARAM  ( int, id );
    GLUCK_EXPORT ( int, GetEventY );
    GLUCK_PARAM  ( int, id );
    GLUCK_EXPORT ( int, GetEventButton );
    GLUCK_PARAM  ( int, id );
    GLUCK_EXPORT ( int, GetEventState );
    GLUCK_PARAM  ( int, id );
    GLUCK_EXPORT ( uint, GetEventKey );
    GLUCK_PARAM  ( int, id );
    GLUCK_EXPORT ( int, GetEventSKey );
    GLUCK_PARAM  ( int, id );
    
    GLUCK_EXPORT ( int, GetViewDims );
    GLUCK_PARAM  ( int, which );
    
    //..standard glut library functions

    GLUCK_EXPORT ( int, Init );
    //Init should take argc, argv...but not yet..

    GLUCK_EXPORT ( int, InitWindowPosition );
    GLUCK_PARAM  ( int , x );
    GLUCK_PARAM  ( int , y );

    GLUCK_EXPORT ( int, InitWindowSize );
    GLUCK_PARAM  ( int , w );
    GLUCK_PARAM  ( int , h );

    GLUCK_EXPORT ( int, InitDisplayMode );
    GLUCK_PARAM  ( uint , displayMode );

    GLUCK_EXPORT ( int, InitDisplayString );
    GLUCK_PARAM  ( string , displayMode );
    
    GLUCK_EXPORT ( int, MainLoopEvent );

    GLUCK_EXPORT ( int, CreateWindow );
    GLUCK_PARAM  ( string , title );

    GLUCK_EXPORT ( int, DestroyWindow );
    GLUCK_PARAM  ( int , window );

    GLUCK_EXPORT ( int, SetWindow );
    GLUCK_PARAM  ( int , window );

    GLUCK_EXPORT ( int, SetWindowTitle );
    GLUCK_PARAM  ( string , title );

    GLUCK_EXPORT ( int, ReshapeWindow );
    GLUCK_PARAM  ( int , width );
    GLUCK_PARAM  ( int , height );

    GLUCK_EXPORT ( int, PositionWindow );
    GLUCK_PARAM  ( int , x );
    GLUCK_PARAM  ( int , y );

    GLUCK_EXPORT ( int, ShowWindow );

    GLUCK_EXPORT ( int, HideWindow );

    GLUCK_EXPORT ( int, FullScreen );

    GLUCK_EXPORT ( int, PostWindowRedisplay );
    GLUCK_PARAM  ( int , window );

    GLUCK_EXPORT ( int, PostRedisplay );

    GLUCK_EXPORT ( int, SwapBuffers );

    GLUCK_EXPORT ( int, StrokeCharacter );
    GLUCK_PARAM  ( int , font );
    GLUCK_PARAM  ( int , character );

    GLUCK_EXPORT ( int, StrokeString );
    GLUCK_PARAM  ( int , font );
    GLUCK_PARAM  ( string , string );

    GLUCK_EXPORT ( int, StrokeWidth );
    GLUCK_PARAM  ( int , font );
    GLUCK_PARAM  ( int , character );

    GLUCK_EXPORT ( int, StrokeLength );
    GLUCK_PARAM  ( int , font );
    GLUCK_PARAM  ( string , string );

    GLUCK_EXPORT ( int, WireTeapot );
    GLUCK_PARAM  ( float , size );

    GLUCK_EXPORT ( int, SolidTeapot );
    GLUCK_PARAM  ( float , size );

    //special gluck values
    QUERY->add_export( QUERY, "uint", "EVENT_MOUSE", (f_ck_func)&gluck_EVENT_MOUSE, FALSE);
    QUERY->add_export( QUERY, "uint", "EVENT_MOTION", (f_ck_func)&gluck_EVENT_MOTION, FALSE);
    QUERY->add_export( QUERY, "uint", "EVENT_PMOTION", (f_ck_func)&gluck_EVENT_PMOTION, FALSE);
    QUERY->add_export( QUERY, "uint", "EVENT_KEY", (f_ck_func)&gluck_EVENT_KEY, FALSE);
    QUERY->add_export( QUERY, "uint", "EVENT_SPECKEY", (f_ck_func)&gluck_EVENT_SPECKEY, FALSE);

       
/*
 * GLUT API macro definitions -- the special key codes:
 */
	QUERY->add_export( QUERY, "uint", "KEY_F1", (f_ck_func)&glut_KEY_F1, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F2", (f_ck_func)&glut_KEY_F2, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F3", (f_ck_func)&glut_KEY_F3, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F4", (f_ck_func)&glut_KEY_F4, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F5", (f_ck_func)&glut_KEY_F5, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F6", (f_ck_func)&glut_KEY_F6, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F7", (f_ck_func)&glut_KEY_F7, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F8", (f_ck_func)&glut_KEY_F8, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F9", (f_ck_func)&glut_KEY_F9, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F10", (f_ck_func)&glut_KEY_F10, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F11", (f_ck_func)&glut_KEY_F11, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_F12", (f_ck_func)&glut_KEY_F12, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_LEFT", (f_ck_func)&glut_KEY_LEFT, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_UP", (f_ck_func)&glut_KEY_UP, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_RIGHT", (f_ck_func)&glut_KEY_RIGHT, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_DOWN", (f_ck_func)&glut_KEY_DOWN, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_PAGE_UP", (f_ck_func)&glut_KEY_PAGE_UP, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_PAGE_DOWN", (f_ck_func)&glut_KEY_PAGE_DOWN, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_HOME", (f_ck_func)&glut_KEY_HOME, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_END", (f_ck_func)&glut_KEY_END, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_INSERT", (f_ck_func)&glut_KEY_INSERT, FALSE);

/*
 * GLUT API macro definitions -- mouse state definitions
 */
	QUERY->add_export( QUERY, "uint", "LEFT_BUTTON", (f_ck_func)&glut_LEFT_BUTTON, FALSE);
	QUERY->add_export( QUERY, "uint", "MIDDLE_BUTTON", (f_ck_func)&glut_MIDDLE_BUTTON, FALSE);
	QUERY->add_export( QUERY, "uint", "RIGHT_BUTTON", (f_ck_func)&glut_RIGHT_BUTTON, FALSE);
	QUERY->add_export( QUERY, "uint", "DOWN", (f_ck_func)&glut_DOWN, FALSE);
	QUERY->add_export( QUERY, "uint", "UP", (f_ck_func)&glut_UP, FALSE);
	QUERY->add_export( QUERY, "uint", "LEFT", (f_ck_func)&glut_LEFT, FALSE);
	QUERY->add_export( QUERY, "uint", "ENTERED", (f_ck_func)&glut_ENTERED, FALSE);

/*
 * GLUT API macro definitions -- the display mode definitions
 */
	QUERY->add_export( QUERY, "uint", "RGB", (f_ck_func)&glut_RGB, FALSE);
	QUERY->add_export( QUERY, "uint", "RGBA", (f_ck_func)&glut_RGBA, FALSE);
	QUERY->add_export( QUERY, "uint", "INDEX", (f_ck_func)&glut_INDEX, FALSE);
	QUERY->add_export( QUERY, "uint", "SINGLE", (f_ck_func)&glut_SINGLE, FALSE);
	QUERY->add_export( QUERY, "uint", "DOUBLE", (f_ck_func)&glut_DOUBLE, FALSE);
	QUERY->add_export( QUERY, "uint", "ACCUM", (f_ck_func)&glut_ACCUM, FALSE);
	QUERY->add_export( QUERY, "uint", "ALPHA", (f_ck_func)&glut_ALPHA, FALSE);
	QUERY->add_export( QUERY, "uint", "DEPTH", (f_ck_func)&glut_DEPTH, FALSE);
	QUERY->add_export( QUERY, "uint", "STENCIL", (f_ck_func)&glut_STENCIL, FALSE);
	QUERY->add_export( QUERY, "uint", "MULTISAMPLE", (f_ck_func)&glut_MULTISAMPLE, FALSE);
	QUERY->add_export( QUERY, "uint", "STEREO", (f_ck_func)&glut_STEREO, FALSE);
	QUERY->add_export( QUERY, "uint", "LUMINANCE", (f_ck_func)&glut_LUMINANCE, FALSE);

/*
 * GLUT API macro definitions -- windows and menu related definitions
 */
	QUERY->add_export( QUERY, "uint", "MENU_NOT_IN_USE", (f_ck_func)&glut_MENU_NOT_IN_USE, FALSE);
	QUERY->add_export( QUERY, "uint", "MENU_IN_USE", (f_ck_func)&glut_MENU_IN_USE, FALSE);
	QUERY->add_export( QUERY, "uint", "NOT_VISIBLE", (f_ck_func)&glut_NOT_VISIBLE, FALSE);
	QUERY->add_export( QUERY, "uint", "VISIBLE", (f_ck_func)&glut_VISIBLE, FALSE);
	QUERY->add_export( QUERY, "uint", "HIDDEN", (f_ck_func)&glut_HIDDEN, FALSE);
	QUERY->add_export( QUERY, "uint", "FULLY_RETAINED", (f_ck_func)&glut_FULLY_RETAINED, FALSE);
	QUERY->add_export( QUERY, "uint", "PARTIALLY_RETAINED", (f_ck_func)&glut_PARTIALLY_RETAINED, FALSE);
	QUERY->add_export( QUERY, "uint", "FULLY_COVERED", (f_ck_func)&glut_FULLY_COVERED, FALSE);


/*
 * GLUT API macro definitions -- the glutGet parameters
 */
	QUERY->add_export( QUERY, "uint", "WINDOW_X", (f_ck_func)&glut_WINDOW_X, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_Y", (f_ck_func)&glut_WINDOW_Y, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_WIDTH", (f_ck_func)&glut_WINDOW_WIDTH, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_HEIGHT", (f_ck_func)&glut_WINDOW_HEIGHT, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_BUFFER_SIZE", (f_ck_func)&glut_WINDOW_BUFFER_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_STENCIL_SIZE", (f_ck_func)&glut_WINDOW_STENCIL_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_DEPTH_SIZE", (f_ck_func)&glut_WINDOW_DEPTH_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_RED_SIZE", (f_ck_func)&glut_WINDOW_RED_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_GREEN_SIZE", (f_ck_func)&glut_WINDOW_GREEN_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_BLUE_SIZE", (f_ck_func)&glut_WINDOW_BLUE_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_ALPHA_SIZE", (f_ck_func)&glut_WINDOW_ALPHA_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_ACCUM_RED_SIZE", (f_ck_func)&glut_WINDOW_ACCUM_RED_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_ACCUM_GREEN_SIZE", (f_ck_func)&glut_WINDOW_ACCUM_GREEN_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_ACCUM_BLUE_SIZE", (f_ck_func)&glut_WINDOW_ACCUM_BLUE_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_ACCUM_ALPHA_SIZE", (f_ck_func)&glut_WINDOW_ACCUM_ALPHA_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_DOUBLEBUFFER", (f_ck_func)&glut_WINDOW_DOUBLEBUFFER, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_RGBA", (f_ck_func)&glut_WINDOW_RGBA, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_PARENT", (f_ck_func)&glut_WINDOW_PARENT, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_NUM_CHILDREN", (f_ck_func)&glut_WINDOW_NUM_CHILDREN, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_COLORMAP_SIZE", (f_ck_func)&glut_WINDOW_COLORMAP_SIZE, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_NUM_SAMPLES", (f_ck_func)&glut_WINDOW_NUM_SAMPLES, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_STEREO", (f_ck_func)&glut_WINDOW_STEREO, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_CURSOR", (f_ck_func)&glut_WINDOW_CURSOR, FALSE);

	QUERY->add_export( QUERY, "uint", "SCREEN_WIDTH", (f_ck_func)&glut_SCREEN_WIDTH, FALSE);
	QUERY->add_export( QUERY, "uint", "SCREEN_HEIGHT", (f_ck_func)&glut_SCREEN_HEIGHT, FALSE);
	QUERY->add_export( QUERY, "uint", "SCREEN_WIDTH_MM", (f_ck_func)&glut_SCREEN_WIDTH_MM, FALSE);
	QUERY->add_export( QUERY, "uint", "SCREEN_HEIGHT_MM", (f_ck_func)&glut_SCREEN_HEIGHT_MM, FALSE);
	QUERY->add_export( QUERY, "uint", "MENU_NUM_ITEMS", (f_ck_func)&glut_MENU_NUM_ITEMS, FALSE);
	QUERY->add_export( QUERY, "uint", "DISPLAY_MODE_POSSIBLE", (f_ck_func)&glut_DISPLAY_MODE_POSSIBLE, FALSE);
	QUERY->add_export( QUERY, "uint", "INIT_WINDOW_X", (f_ck_func)&glut_INIT_WINDOW_X, FALSE);
	QUERY->add_export( QUERY, "uint", "INIT_WINDOW_Y", (f_ck_func)&glut_INIT_WINDOW_Y, FALSE);
	QUERY->add_export( QUERY, "uint", "INIT_WINDOW_WIDTH", (f_ck_func)&glut_INIT_WINDOW_WIDTH, FALSE);
	QUERY->add_export( QUERY, "uint", "INIT_WINDOW_HEIGHT", (f_ck_func)&glut_INIT_WINDOW_HEIGHT, FALSE);
	QUERY->add_export( QUERY, "uint", "INIT_DISPLAY_MODE", (f_ck_func)&glut_INIT_DISPLAY_MODE, FALSE);
	QUERY->add_export( QUERY, "uint", "ELAPSED_TIME", (f_ck_func)&glut_ELAPSED_TIME, FALSE);
	QUERY->add_export( QUERY, "uint", "WINDOW_FORMAT_ID", (f_ck_func)&glut_WINDOW_FORMAT_ID, FALSE);
	QUERY->add_export( QUERY, "uint", "INIT_STATE", (f_ck_func)&glut_INIT_STATE, FALSE);

/*
 * GLUT API macro definitions -- the glutDeviceGet parameters
 */
	QUERY->add_export( QUERY, "uint", "HAS_KEYBOARD", (f_ck_func)&glut_HAS_KEYBOARD, FALSE);
	QUERY->add_export( QUERY, "uint", "HAS_MOUSE", (f_ck_func)&glut_HAS_MOUSE, FALSE);
	QUERY->add_export( QUERY, "uint", "HAS_SPACEBALL", (f_ck_func)&glut_HAS_SPACEBALL, FALSE);
	QUERY->add_export( QUERY, "uint", "HAS_DIAL_AND_BUTTON_BOX", (f_ck_func)&glut_HAS_DIAL_AND_BUTTON_BOX, FALSE);
	QUERY->add_export( QUERY, "uint", "HAS_TABLET", (f_ck_func)&glut_HAS_TABLET, FALSE);
	QUERY->add_export( QUERY, "uint", "NUM_MOUSE_BUTTONS", (f_ck_func)&glut_NUM_MOUSE_BUTTONS, FALSE);
	QUERY->add_export( QUERY, "uint", "NUM_SPACEBALL_BUTTONS", (f_ck_func)&glut_NUM_SPACEBALL_BUTTONS, FALSE);
	QUERY->add_export( QUERY, "uint", "NUM_BUTTON_BOX_BUTTONS", (f_ck_func)&glut_NUM_BUTTON_BOX_BUTTONS, FALSE);
	QUERY->add_export( QUERY, "uint", "NUM_DIALS", (f_ck_func)&glut_NUM_DIALS, FALSE);
	QUERY->add_export( QUERY, "uint", "NUM_TABLET_BUTTONS", (f_ck_func)&glut_NUM_TABLET_BUTTONS, FALSE);
	QUERY->add_export( QUERY, "uint", "DEVICE_IGNORE_KEY_REPEAT", (f_ck_func)&glut_DEVICE_IGNORE_KEY_REPEAT, FALSE);
	QUERY->add_export( QUERY, "uint", "DEVICE_KEY_REPEAT", (f_ck_func)&glut_DEVICE_KEY_REPEAT, FALSE);
	QUERY->add_export( QUERY, "uint", "HAS_JOYSTICK", (f_ck_func)&glut_HAS_JOYSTICK, FALSE);
	QUERY->add_export( QUERY, "uint", "OWNS_JOYSTICK", (f_ck_func)&glut_OWNS_JOYSTICK, FALSE);
	QUERY->add_export( QUERY, "uint", "JOYSTICK_BUTTONS", (f_ck_func)&glut_JOYSTICK_BUTTONS, FALSE);
	QUERY->add_export( QUERY, "uint", "JOYSTICK_AXES", (f_ck_func)&glut_JOYSTICK_AXES, FALSE);
	QUERY->add_export( QUERY, "uint", "JOYSTICK_POLL_RATE", (f_ck_func)&glut_JOYSTICK_POLL_RATE, FALSE);

/*
 * GLUT API macro definitions -- the glutLayerGet parameters
 */
	QUERY->add_export( QUERY, "uint", "OVERLAY_POSSIBLE", (f_ck_func)&glut_OVERLAY_POSSIBLE, FALSE);
	QUERY->add_export( QUERY, "uint", "LAYER_IN_USE", (f_ck_func)&glut_LAYER_IN_USE, FALSE);
	QUERY->add_export( QUERY, "uint", "HAS_OVERLAY", (f_ck_func)&glut_HAS_OVERLAY, FALSE);
	QUERY->add_export( QUERY, "uint", "TRANSPARENT_INDEX", (f_ck_func)&glut_TRANSPARENT_INDEX, FALSE);
	QUERY->add_export( QUERY, "uint", "NORMAL_DAMAGED", (f_ck_func)&glut_NORMAL_DAMAGED, FALSE);
	QUERY->add_export( QUERY, "uint", "OVERLAY_DAMAGED", (f_ck_func)&glut_OVERLAY_DAMAGED, FALSE);

/*
 * GLUT API macro definitions -- the glutVideoResizeGet parameters
 */
	QUERY->add_export( QUERY, "uint", "VIDEO_RESIZE_POSSIBLE", (f_ck_func)&glut_VIDEO_RESIZE_POSSIBLE, FALSE);
	QUERY->add_export( QUERY, "uint", "VIDEO_RESIZE_IN_USE", (f_ck_func)&glut_VIDEO_RESIZE_IN_USE, FALSE);
	QUERY->add_export( QUERY, "uint", "VIDEO_RESIZE_X_DELTA", (f_ck_func)&glut_VIDEO_RESIZE_X_DELTA, FALSE);
	QUERY->add_export( QUERY, "uint", "VIDEO_RESIZE_Y_DELTA", (f_ck_func)&glut_VIDEO_RESIZE_Y_DELTA, FALSE);
	QUERY->add_export( QUERY, "uint", "VIDEO_RESIZE_WIDTH_DELTA", (f_ck_func)&glut_VIDEO_RESIZE_WIDTH_DELTA, FALSE);
	QUERY->add_export( QUERY, "uint", "VIDEO_RESIZE_HEIGHT_DELTA", (f_ck_func)&glut_VIDEO_RESIZE_HEIGHT_DELTA, FALSE);
	QUERY->add_export( QUERY, "uint", "VIDEO_RESIZE_X", (f_ck_func)&glut_VIDEO_RESIZE_X, FALSE);
	QUERY->add_export( QUERY, "uint", "VIDEO_RESIZE_Y", (f_ck_func)&glut_VIDEO_RESIZE_Y, FALSE);
	QUERY->add_export( QUERY, "uint", "VIDEO_RESIZE_WIDTH", (f_ck_func)&glut_VIDEO_RESIZE_WIDTH, FALSE);
	QUERY->add_export( QUERY, "uint", "VIDEO_RESIZE_HEIGHT", (f_ck_func)&glut_VIDEO_RESIZE_HEIGHT, FALSE);

/*
 * GLUT API macro definitions -- the glutUseLayer parameters
 */
	QUERY->add_export( QUERY, "uint", "NORMAL", (f_ck_func)&glut_NORMAL, FALSE);
	QUERY->add_export( QUERY, "uint", "OVERLAY", (f_ck_func)&glut_OVERLAY, FALSE);

/*
 * GLUT API macro definitions -- the glutGetModifiers parameters
 */
	QUERY->add_export( QUERY, "uint", "ACTIVE_SHIFT", (f_ck_func)&glut_ACTIVE_SHIFT, FALSE);
	QUERY->add_export( QUERY, "uint", "ACTIVE_CTRL", (f_ck_func)&glut_ACTIVE_CTRL, FALSE);
	QUERY->add_export( QUERY, "uint", "ACTIVE_ALT", (f_ck_func)&glut_ACTIVE_ALT, FALSE);

/*
 * GLUT API macro definitions -- the glutSetCursor parameters
 */
	QUERY->add_export( QUERY, "uint", "CURSOR_RIGHT_ARROW", (f_ck_func)&glut_CURSOR_RIGHT_ARROW, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_LEFT_ARROW", (f_ck_func)&glut_CURSOR_LEFT_ARROW, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_INFO", (f_ck_func)&glut_CURSOR_INFO, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_DESTROY", (f_ck_func)&glut_CURSOR_DESTROY, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_HELP", (f_ck_func)&glut_CURSOR_HELP, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_CYCLE", (f_ck_func)&glut_CURSOR_CYCLE, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_SPRAY", (f_ck_func)&glut_CURSOR_SPRAY, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_WAIT", (f_ck_func)&glut_CURSOR_WAIT, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_TEXT", (f_ck_func)&glut_CURSOR_TEXT, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_CROSSHAIR", (f_ck_func)&glut_CURSOR_CROSSHAIR, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_UP_DOWN", (f_ck_func)&glut_CURSOR_UP_DOWN, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_LEFT_RIGHT", (f_ck_func)&glut_CURSOR_LEFT_RIGHT, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_TOP_SIDE", (f_ck_func)&glut_CURSOR_TOP_SIDE, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_BOTTOM_SIDE", (f_ck_func)&glut_CURSOR_BOTTOM_SIDE, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_LEFT_SIDE", (f_ck_func)&glut_CURSOR_LEFT_SIDE, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_RIGHT_SIDE", (f_ck_func)&glut_CURSOR_RIGHT_SIDE, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_TOP_LEFT_CORNER", (f_ck_func)&glut_CURSOR_TOP_LEFT_CORNER, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_TOP_RIGHT_CORNER", (f_ck_func)&glut_CURSOR_TOP_RIGHT_CORNER, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_BOTTOM_RIGHT_CORNER", (f_ck_func)&glut_CURSOR_BOTTOM_RIGHT_CORNER, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_BOTTOM_LEFT_CORNER", (f_ck_func)&glut_CURSOR_BOTTOM_LEFT_CORNER, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_INHERIT", (f_ck_func)&glut_CURSOR_INHERIT, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_NONE", (f_ck_func)&glut_CURSOR_NONE, FALSE);
	QUERY->add_export( QUERY, "uint", "CURSOR_FULL_CROSSHAIR", (f_ck_func)&glut_CURSOR_FULL_CROSSHAIR, FALSE);

/*
 * GLUT API macro definitions -- RGB color component specification definitions
 */
	QUERY->add_export( QUERY, "uint", "RED", (f_ck_func)&glut_RED, FALSE);
	QUERY->add_export( QUERY, "uint", "GREEN", (f_ck_func)&glut_GREEN, FALSE);
	QUERY->add_export( QUERY, "uint", "BLUE", (f_ck_func)&glut_BLUE, FALSE);

/*
 * GLUT API macro definitions -- additional keyboard and joystick definitions
 */
	QUERY->add_export( QUERY, "uint", "KEY_REPEAT_OFF", (f_ck_func)&glut_KEY_REPEAT_OFF, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_REPEAT_ON", (f_ck_func)&glut_KEY_REPEAT_ON, FALSE);
	QUERY->add_export( QUERY, "uint", "KEY_REPEAT_DEFAULT", (f_ck_func)&glut_KEY_REPEAT_DEFAULT, FALSE);

	QUERY->add_export( QUERY, "uint", "JOYSTICK_BUTTON_A", (f_ck_func)&glut_JOYSTICK_BUTTON_A, FALSE);
	QUERY->add_export( QUERY, "uint", "JOYSTICK_BUTTON_B", (f_ck_func)&glut_JOYSTICK_BUTTON_B, FALSE);
	QUERY->add_export( QUERY, "uint", "JOYSTICK_BUTTON_C", (f_ck_func)&glut_JOYSTICK_BUTTON_C, FALSE);
	QUERY->add_export( QUERY, "uint", "JOYSTICK_BUTTON_D", (f_ck_func)&glut_JOYSTICK_BUTTON_D, FALSE);

/*
 * GLUT API macro definitions -- game mode definitions
 */
	QUERY->add_export( QUERY, "uint", "GAME_MODE_ACTIVE", (f_ck_func)&glut_GAME_MODE_ACTIVE, FALSE);
	QUERY->add_export( QUERY, "uint", "GAME_MODE_POSSIBLE", (f_ck_func)&glut_GAME_MODE_POSSIBLE, FALSE);
	QUERY->add_export( QUERY, "uint", "GAME_MODE_WIDTH", (f_ck_func)&glut_GAME_MODE_WIDTH, FALSE);
	QUERY->add_export( QUERY, "uint", "GAME_MODE_HEIGHT", (f_ck_func)&glut_GAME_MODE_HEIGHT, FALSE);
	QUERY->add_export( QUERY, "uint", "GAME_MODE_PIXEL_DEPTH", (f_ck_func)&glut_GAME_MODE_PIXEL_DEPTH, FALSE);
	QUERY->add_export( QUERY, "uint", "GAME_MODE_REFRESH_RATE", (f_ck_func)&glut_GAME_MODE_REFRESH_RATE, FALSE);
	QUERY->add_export( QUERY, "uint", "GAME_MODE_DISPLAY_CHANGED", (f_ck_func)&glut_GAME_MODE_DISPLAY_CHANGED, FALSE);


    return TRUE;
}


CK_DLL_FUNC ( gluck_InitBasicWindow_impl ) { 

    char*  title = GET_NEXT_STRING(ARGS);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    gluckstate->doubleBuffered = true;
    gluckstate->windowID = glutCreateWindow(title);

    glViewport(0,0,640,480);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

CK_DLL_FUNC ( gluck_InitSizedWindow_impl ) { 
    char*  title = GET_NEXT_STRING(ARGS);
    int x = GET_NEXT_INT(ARGS);
    int y = GET_NEXT_INT(ARGS);
    int w = GET_NEXT_INT(ARGS);
    int h = GET_NEXT_INT(ARGS);
    
    glutInitWindowPosition(x, y);
    glutInitWindowSize(w, h);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    gluckstate->doubleBuffered = true;

    gluckstate->windowID = glutCreateWindow(title);
    
    glViewport(x,y,w,h);
}

CK_DLL_FUNC ( gluck_InitFullScreenWindow_impl ) { 
    char*  title = GET_NEXT_STRING(ARGS);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    gluckstate->doubleBuffered = true;

    gluckstate->windowID = glutCreateWindow(title);
    glutFullScreen();

}

CK_DLL_FUNC ( gluck_NeedDraw_impl ) { 
    RETURN->v_int = gluckstate->needDraw;
}

CK_DLL_FUNC ( gluck_NeedEvent_impl ) { 
    RETURN->v_int = gluckstate->needEvent;
}

CK_DLL_FUNC ( gluck_NeedIdle_impl ) { 
    RETURN->v_int = gluckstate->needIdle;
}

CK_DLL_FUNC ( gluck_NeedReshape_impl ) { 
    RETURN->v_int = gluckstate->needReshape;
}

CK_DLL_FUNC ( gluck_WatchMouse_impl ) { 
    gluckstate->watchMouse      = ( GET_NEXT_INT(ARGS) != 0 );
    if ( gluckstate->watchMouse ) { 
        glutMouseFunc   (gluckMouseCB);
    }
    else { 
        glutMouseFunc   (NULL);
    }
}

CK_DLL_FUNC ( gluck_WatchMotion_impl ) { 
    gluckstate->watchMotion     = ( GET_NEXT_INT(ARGS) != 0 );
    if ( gluckstate->watchMotion ) { 
        glutPassiveMotionFunc ( gluckPassiveMotionCB);
        glutMotionFunc ( gluckMotionCB);
    }
    else { 
        glutPassiveMotionFunc ( NULL );
        glutMotionFunc ( NULL);
    }    
}

CK_DLL_FUNC ( gluck_WatchKeyboard_impl ) { 
    gluckstate->watchKeyboard   = ( GET_NEXT_INT(ARGS) != 0 );
    if ( gluckstate->watchKeyboard ) { 
        glutKeyboardFunc ( gluckKeyboardCB);
        glutSpecialFunc  ( gluckSpecialCB );
    }
    else  { 
        glutKeyboardFunc ( NULL );
        glutSpecialFunc  ( NULL );
    }
}

//gluck helper functions ( callbacks! );


void gluckDisplayCB() { 
    gluckstate->needDraw = true;
}

void gluckIdleCB() { 
  //  glutPostRedisplay();
    gluckstate->needIdle = true;
}

void gluckReshapeCB(int x, int y) { 
    glViewport (0,0, x, y );
    gluckstate->winx = x;
    gluckstate->winy = y;
    gluckstate->needReshape = true;
}

void gluckMouseCB ( int button, int state, int x, int y) { 

    gluckAddBufferedEvent ( EV_MOUSE, x, y , button, state, '\0', 0, glutGetModifiers() );
}

void gluckMotionCB ( int x, int y ) {
    gluckAddBufferedEvent ( EV_MOTION, x, y , 0, 0, '\0', 0, -1);
}

void gluckPassiveMotionCB( int x, int y) { 
    gluckAddBufferedEvent ( EV_PMOTION, x, y , 0, 0, '\0', 0, -1);
}

void gluckKeyboardCB ( unsigned char key, int x, int y ) { 
    gluckAddBufferedEvent ( EV_KEY, x, y , 0, 0, key, 0, glutGetModifiers());
}

void gluckSpecialCB ( int key, int x, int y ) { 
    gluckAddBufferedEvent ( EV_SPECKEY, x, y , 0, 0, '\0', key, glutGetModifiers());
}

void gluckAddBufferedEvent(  int type, int x, int y, int button, int state, unsigned char key, int skey, int mods ) {


    gluckstate->events[gluckstate->event_w].type = type;
    gluckstate->events[gluckstate->event_w].x = x;
    gluckstate->events[gluckstate->event_w].y = y;
    gluckstate->events[gluckstate->event_w].button = button;
    gluckstate->events[gluckstate->event_w].state = state;
    gluckstate->events[gluckstate->event_w].key = key;
    gluckstate->events[gluckstate->event_w].skey = skey;
    if ( mods >= 0 ) gluckstate->curmodifiers = mods;
    gluckstate->events[gluckstate->event_w].modifiers = gluckstate->curmodifiers;
        
    int nextw = (gluckstate->event_w+1) % gluckstate->events.size(); 

    if ( nextw == gluckstate->event_r ) { 
        //resize dynamically,
        gluckstate->events.insert(gluckstate->events.begin() + gluckstate->event_r, gluckstate->events[gluckstate->event_r]);
        gluckstate->event_r = (gluckstate->event_r+1);
    }

    gluckstate->event_w = nextw;
    gluckstate->needEvent = true;

} 

bool gluckHasEvents() { 
    return (  (gluckstate->event_r+1) % gluckstate->events.size() != gluckstate->event_w );
}

int gluckGetNextEvent() { 
    if ( gluckHasEvents() ) { 
        gluckstate->event_r = (gluckstate->event_r+1) % gluckstate->events.size();
        return gluckstate->event_r;
    }
    else return -1;
}

CK_DLL_FUNC( gluck_HasEvents_impl ) {
    RETURN->v_int = gluckHasEvents() ? 1 : 0 ; 
}

CK_DLL_FUNC( gluck_GetNextEvent_impl ) {
    RETURN->v_int = gluckGetNextEvent();
}

CK_DLL_FUNC( gluck_GetEventType_impl ) { 
    int id = GET_NEXT_INT(ARGS);
    RETURN->v_int = gluckstate->events[id].type;
}


CK_DLL_FUNC( gluck_GetEventX_impl ) { 
    int id = GET_NEXT_INT(ARGS);
    RETURN->v_int = gluckstate->events[id].x;
}

CK_DLL_FUNC( gluck_GetEventY_impl )  {
    int id = GET_NEXT_INT(ARGS);
    RETURN->v_int = gluckstate->events[id].y;
}

CK_DLL_FUNC( gluck_GetEventButton_impl ){
    int id = GET_NEXT_INT(ARGS);
    RETURN->v_int = gluckstate->events[id].button;
}

CK_DLL_FUNC( gluck_GetEventState_impl ) {
    int id = GET_NEXT_INT(ARGS);
    RETURN->v_int = gluckstate->events[id].state;
}

CK_DLL_FUNC( gluck_GetEventKey_impl ) {
    int id = GET_NEXT_INT(ARGS);
    RETURN->v_uint = (uint) gluckstate->events[id].key;
}

CK_DLL_FUNC( gluck_GetEventSKey_impl ) {
    int id = GET_NEXT_INT(ARGS);
    RETURN->v_int = gluckstate->events[id].skey;
}

CK_DLL_FUNC( gluck_GetViewDims_impl ) { 
  int which = GET_NEXT_INT (ARGS);
  if ( which == 0 ) RETURN->v_int = gluckstate->winx;
  else if ( which == 1 ) RETURN->v_int = gluckstate->winy;
  else RETURN->v_int = 0;
}

CK_DLL_FUNC ( gluck_InitCallbacks_impl ) { 

    gluckstate->watchMouse      = ( GET_NEXT_INT(ARGS) != 0 );
    gluckstate->watchMotion     = ( GET_NEXT_INT(ARGS) != 0 );
    gluckstate->watchKeyboard   = ( GET_NEXT_INT(ARGS) != 0 );

    //test
    glutDisplayFunc (gluckDisplayCB);
    glutReshapeFunc (gluckReshapeCB);
    glutIdleFunc(gluckIdleCB);
    if ( gluckstate->watchMouse ) { 
        glutMouseFunc   (gluckMouseCB);
    }
    if ( gluckstate->watchMotion ) { 
        glutPassiveMotionFunc ( gluckPassiveMotionCB);
        glutMotionFunc ( gluckMotionCB);
    }
    if ( gluckstate->watchKeyboard ) { 
        glutKeyboardFunc ( gluckKeyboardCB);
        glutSpecialFunc  ( gluckSpecialCB );
    }

}

//
CK_DLL_FUNC( gluck_Init_impl )
{
  gluckstate = new gluckData;
  int foo = 0;
  glutInit(&foo, NULL);
}

CK_DLL_FUNC( gluck_InitWindowPosition_impl )
{
  int x = GET_NEXT_INT(ARGS);
  int y = GET_NEXT_INT(ARGS);
  glutInitWindowPosition(x, y);
}

CK_DLL_FUNC( gluck_InitWindowSize_impl )
{
  int w = GET_NEXT_INT(ARGS);
  int h = GET_NEXT_INT(ARGS);
  glutInitWindowSize(w, h);
}

CK_DLL_FUNC( gluck_InitDisplayMode_impl )
{
  t_CKUINT mode = GET_NEXT_UINT(ARGS);
  glutInitDisplayMode(mode);
}


CK_DLL_FUNC( gluck_InitDisplayString_impl )
{
  char*  mode = GET_NEXT_STRING(ARGS);
  glutInitDisplayString(mode);
}


CK_DLL_FUNC( gluck_MainLoopEvent_impl )
{

  gluckstate->needDraw = 0;
  gluckstate->needEvent = 0;
  gluckstate->needIdle = 0;
  gluckstate->needReshape = 0;

#ifdef __MACOSX_CORE__
  glutCheckLoop();
#else
  glutMainLoopEvent();
#endif
  //hork;
}

CK_DLL_FUNC( gluck_CreateWindow_impl )
{
  //  t_CKSTRING mode = GET_NEXT_STRING(ARGS);
  t_CKUINT ctitle = GET_NEXT_UINT(ARGS);
  char title[] = "=gluck>";
  gluckstate->windowID = glutCreateWindow( (char*)title );
  RETURN->v_int = gluckstate->windowID; }

CK_DLL_FUNC( gluck_DestroyWindow_impl )
{
  int window = GET_NEXT_INT(ARGS);
  glutDestroyWindow( window );
}

CK_DLL_FUNC( gluck_SetWindow_impl )
{
  int window = GET_NEXT_INT(ARGS);
  glutSetWindow(window);
}

CK_DLL_FUNC( gluck_SetWindowTitle_impl )
{
  char * title = GET_NEXT_STRING(ARGS);
  glutSetWindowTitle(title);
}

CK_DLL_FUNC( gluck_ReshapeWindow_impl )
{
  int width = GET_NEXT_INT(ARGS);
  int height = GET_NEXT_INT(ARGS);
  glutReshapeWindow(width, height);
}

CK_DLL_FUNC( gluck_PositionWindow_impl )
{
  int x = GET_NEXT_INT(ARGS);
  int y = GET_NEXT_INT(ARGS);
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
  glGetIntegerv(GL_VIEWPORT, (GLint*)gluckstate->vp);

}

CK_DLL_FUNC( gluck_PostWindowRedisplay_impl )
{
  int window = GET_NEXT_INT(ARGS);
  glutPostWindowRedisplay(window);
}

CK_DLL_FUNC( gluck_PostRedisplay_impl )
{
  glutPostRedisplay();
}

CK_DLL_FUNC( gluck_SwapBuffers_impl )
{
  if ( gluckstate->doubleBuffered ) glutSwapBuffers();
}

CK_DLL_FUNC( gluck_StrokeCharacter_impl )
{
  t_CKUINT font = GET_NEXT_UINT(ARGS);
  int c = GET_NEXT_INT(ARGS);
  void * pfont = ( font == 0 ) ? GLUT_STROKE_ROMAN : GLUT_STROKE_MONO_ROMAN ; 
  glutStrokeCharacter(pfont, c);
}

CK_DLL_FUNC( gluck_StrokeWidth_impl )
{
  t_CKUINT font = GET_NEXT_UINT(ARGS);
  int c = GET_NEXT_INT(ARGS);
  void * pfont = ( font == 0 ) ? GLUT_STROKE_ROMAN : GLUT_STROKE_MONO_ROMAN ; 
  RETURN->v_int = glutStrokeWidth( pfont, c);
}

CK_DLL_FUNC( gluck_StrokeLength_impl )
{  
  t_CKUINT font = GET_NEXT_UINT(ARGS);
  char* str = GET_NEXT_STRING(ARGS);
  void * pfont = ( font == 0 ) ? GLUT_STROKE_ROMAN : GLUT_STROKE_MONO_ROMAN ; 
  RETURN->v_int = glutStrokeLength( pfont, (unsigned char*)str);
}

CK_DLL_FUNC( gluck_StrokeString_impl )
{  
  t_CKUINT font = GET_NEXT_UINT(ARGS);
  char* str  = GET_NEXT_STRING(ARGS);
  void * pfont = ( font == 0 ) ? GLUT_STROKE_ROMAN : GLUT_STROKE_MONO_ROMAN ; 

#ifdef __MACOSX_CORE__
  int n = strlen (str);
  char c;
  float length = 0.0;
  for ( int i= 0; i < n ;i++ ) { 
    c = str[i];
    if ( c == '\n' ) { 
      glTranslated( -length, -100, 0.0 );
      length = 0.0;
    }
    else { 
      glutStrokeCharacter (pfont, c);
      length += glutStrokeWidth ( pfont , c );
    }
  }
#else 
  //  freeglut
  glutStrokeString( pfont, (unsigned char*)str);
#endif
}


CK_DLL_FUNC( gluck_WireTeapot_impl )
{
  t_CKFLOAT size = GET_NEXT_FLOAT(ARGS);
  glutWireTeapot(size);
}
CK_DLL_FUNC( gluck_SolidTeapot_impl )
{
  t_CKFLOAT size = GET_NEXT_FLOAT(ARGS);
  glutSolidTeapot(size);
}
