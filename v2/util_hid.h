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
// file: util_hid.h
// desc: header file for joystick/mouse/keyboard support
//
// author: Spencer Salazar (ssalazar@cs.princeton.edu)
// date: spring 2006
//-----------------------------------------------------------------------------

#ifndef __UTIL_HID_H__
#define __UTIL_HID_H__

//-----------------------------------------------------------------------------
// definitions
//-----------------------------------------------------------------------------
struct HidMsg
{
    t_CKINT device_type; // device type
    t_CKINT device_num;  // device number
    t_CKINT type;        // message type
    t_CKINT eid;         // element id
    t_CKINT idata[4];    // int data
    t_CKFLOAT fdata[4];  // float data
    
#ifdef __cplusplus
    HidMsg()
    { this->clear(); }

    void clear()
    {
        memset( this, 0, sizeof(HidMsg) );
    }
#endif
};

/* device types */
static const t_CKUINT CK_HID_DEV_NONE = 0;
static const t_CKUINT CK_HID_DEV_JOYSTICK = 1;
static const t_CKUINT CK_HID_DEV_MOUSE = 2;
static const t_CKUINT CK_HID_DEV_KEYBOARD = 3;
static const t_CKUINT CK_HID_DEV_WIIREMOTE = 4;
static const t_CKUINT CK_HID_DEV_TABLET = 5;
static const t_CKUINT CK_HID_DEV_COUNT = 6;

/* message types */
static const t_CKUINT CK_HID_JOYSTICK_AXIS = 0;
static const t_CKUINT CK_HID_BUTTON_DOWN = 1;
static const t_CKUINT CK_HID_BUTTON_UP = 2;
static const t_CKUINT CK_HID_JOYSTICK_HAT = 3;
static const t_CKUINT CK_HID_JOYSTICK_BALL = 4;
static const t_CKUINT CK_HID_MOUSE_MOTION = 5;
static const t_CKUINT CK_HID_MOUSE_WHEEL = 6;
static const t_CKUINT CK_HID_DEVICE_CONNECTED = 7;
static const t_CKUINT CK_HID_DEVICE_DISCONNECTED = 8;
static const t_CKUINT CK_HID_ACCELEROMETER = 9;
static const t_CKUINT CK_HID_TABLET_PRESSURE = 10;
static const t_CKUINT CK_HID_TABLET_MOTION = 10;
static const t_CKUINT CK_HID_TABLET_ROTATION = 10;
static const t_CKUINT CK_HID_MSG_COUNT = 10;

/* keys */

/* extension API */
typedef struct
{
    void (*init)(void);
} Chuck_Hid_Driver;

/* functions */
extern void Hid_init();
extern void Hid_poll();
extern void Hid_quit();

extern void Joystick_init();
extern void Joystick_poll();
extern void Joystick_quit();
extern int Joystick_count();
extern int Joystick_open( int js );
extern int Joystick_open( const char * name );
extern int Joystick_close( int js );
extern const char * Joystick_name( int js );

extern int Joystick_axes( int js );
extern int Joystick_buttons( int js );
extern int Joystick_hats( int js );

extern void Mouse_init();
extern void Mouse_poll();
extern void Mouse_quit();
extern int Mouse_count();
extern int Mouse_open( int m );
extern int Mouse_open( const char * name );
extern int Mouse_close( int m );
extern const char * Mouse_name( int m );
extern int Mouse_buttons( int m );
#define __CK_HID_CURSORTRACK__
#ifdef __CK_HID_CURSORTRACK__
extern int Mouse_start_cursor_track();
extern int Mouse_stop_cursor_track();
#endif // __CK_HID_CURSORTRACK__
    
extern void Keyboard_init();
extern void Keyboard_poll();
extern void Keyboard_quit();
extern int Keyboard_count();
extern int Keyboard_open( int kb );
extern int Keyboard_open( const char * name );
extern int Keyboard_close( int kb );
extern const char * Keyboard_name( int kb );

#define __CK_HID_WIIREMOTE__
#ifdef __CK_HID_WIIREMOTE__
extern void WiiRemote_init();
extern void WiiRemote_poll();
extern void WiiRemote_quit();
extern int WiiRemote_count();
extern int WiiRemote_open( int kb );
extern int WiiRemote_open( const char * name );
extern int WiiRemote_close( int kb );
extern const char * WiiRemote_name( int kb );
#endif // __CK_HID_WIIREMOTE__

extern int TiltSensor_read( t_CKINT * x, t_CKINT * y, t_CKINT * z );


#endif
