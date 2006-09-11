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

#include <string>

extern void Hid_init();
extern void Hid_poll();
extern void Hid_quit();

extern void Joystick_init();
extern void Joystick_poll();
extern void Joystick_quit();
extern int Joystick_count();
extern int Joystick_open( int js );
extern int Joystick_open( std::string & name );
extern int Joystick_close( int js );
extern const char * Joystick_name( int js );

extern void Mouse_init();
extern void Mouse_poll();
extern void Mouse_quit();
extern int Mouse_count();
extern int Mouse_open( int m );
extern int Mouse_open( std::string & name );
extern int Mouse_close( int m );
extern const char * Mouse_name( int m );
    
extern void Keyboard_init();
extern void Keyboard_poll();
extern void Keyboard_quit();
extern int Keyboard_count();
extern int Keyboard_open( int kb );
extern int Keyboard_open( std::string & name );
extern int Keyboard_close( int kb );
extern const char * Keyboard_name( int kb );
