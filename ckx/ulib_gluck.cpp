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

    return TRUE;
}


CK_DLL_FUNC ( gluck_InitBasicWindow_impl ) { 

    char*  title = GET_NEXT_STRING(ARGS);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
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
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    gluckstate->doubleBuffered = true;

    gluckstate->windowID = glutCreateWindow(title);
    
    glViewport(x,y,w,h);
}

CK_DLL_FUNC ( gluck_InitFullScreenWindow_impl ) { 
    char*  title = GET_NEXT_STRING(ARGS);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
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
  glutMainLoopEvent();
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
  glGetIntegerv(GL_VIEWPORT, gluckstate->vp);

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
  glutStrokeString( pfont, (unsigned char*)str);
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
