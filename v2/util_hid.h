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
// desc: header file for skeleton/joystick SDL support
//
// author: Spencer Salazar (ssalazar@cs.princeton.edu)
// date: spring 2006
//-----------------------------------------------------------------------------

// excepted as noted, everything below is straight from SDL

#pragma mark >begin:../include/SDL_joystick.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Include file for SDL joystick event handling */

#ifndef _SDL_joystick_h
#define _SDL_joystick_h

#pragma mark >>begin:../include/SDL_types.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* General data types used by the SDL library */

#ifndef _SDL_types_h
#define _SDL_types_h

/* The number of elements in a table */
#define SDL_TABLESIZE(table)	(sizeof(table)/sizeof(table[0]))

/* Basic data types */
typedef enum {
	SDL_FALSE = 0,
	SDL_TRUE  = 1
} SDL_bool;

#ifdef H_MMBASIC /* mmbasic.h (Tru64 MME) */
/* Some of the basic types are already defined in mmbasic.h */
typedef signed char	Sint8;
typedef signed short	Sint16;
typedef signed int	Sint32;
#else
typedef unsigned char	Uint8;
typedef signed char	Sint8;
typedef unsigned short	Uint16;
typedef signed short	Sint16;
typedef unsigned int	Uint32;
typedef signed int	Sint32;
#endif

/* Figure out how to support 64-bit datatypes */
#if !defined(__STRICT_ANSI__)
#ifdef __osf__ /* Tru64 */
#define SDL_HAS_64BIT_TYPE	long
#elif defined(__GNUC__) || defined(__MWERKS__) || defined(__SUNPRO_C) || defined(__DECC)
#define SDL_HAS_64BIT_TYPE	long long
#elif defined(_MSC_VER) /* VC++ */
#define SDL_HAS_64BIT_TYPE	__int64
#endif
#endif /* !__STRICT_ANSI__ */

/* The 64-bit type isn't available on EPOC/Symbian OS */
#ifdef __SYMBIAN32__
#undef SDL_HAS_64BIT_TYPE
#endif

/* The 64-bit datatype isn't supported on all platforms */
#ifdef SDL_HAS_64BIT_TYPE
#ifndef H_MMBASIC
typedef unsigned SDL_HAS_64BIT_TYPE Uint64;
#endif
typedef SDL_HAS_64BIT_TYPE Sint64;
#else
/* This is really just a hack to prevent the compiler from complaining */
typedef struct {
	Uint32 hi;
	Uint32 lo;
} Uint64, Sint64;
#endif

/* Make sure the types really have the right sizes */
#define SDL_COMPILE_TIME_ASSERT(name, x)               \
       typedef int SDL_dummy_ ## name[(x) * 2 - 1]

SDL_COMPILE_TIME_ASSERT(uint8, sizeof(Uint8) == 1);
SDL_COMPILE_TIME_ASSERT(sint8, sizeof(Sint8) == 1);
SDL_COMPILE_TIME_ASSERT(uint16, sizeof(Uint16) == 2);
SDL_COMPILE_TIME_ASSERT(sint16, sizeof(Sint16) == 2);
SDL_COMPILE_TIME_ASSERT(uint32, sizeof(Uint32) == 4);
SDL_COMPILE_TIME_ASSERT(sint32, sizeof(Sint32) == 4);
SDL_COMPILE_TIME_ASSERT(uint64, sizeof(Uint64) == 8);
SDL_COMPILE_TIME_ASSERT(sint64, sizeof(Sint64) == 8);

/* Check to make sure enums are the size of ints, for structure packing.
   For both Watcom C/C++ and Borland C/C++ the compiler option that makes
   enums having the size of an int must be enabled.
   This is "-b" for Borland C/C++ and "-ei" for Watcom C/C++ (v11).
*/
/* Enable enums always int in CodeWarrior (for MPW use "-enum int") */
#ifdef __MWERKS__
#pragma enumsalwaysint on
#endif

typedef enum {
	DUMMY_ENUM_VALUE
} SDL_DUMMY_ENUM;

SDL_COMPILE_TIME_ASSERT(enum, sizeof(SDL_DUMMY_ENUM) == sizeof(int));

#undef SDL_COMPILE_TIME_ASSERT

/* General keyboard/mouse state definitions */
enum { SDL_PRESSED = 0x01, SDL_RELEASED = 0x00 };

#endif
#pragma mark >>end:../include/SDL_types.h


#pragma mark >>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* In order to use these functions, SDL_Init() must have been called
   with the SDL_INIT_JOYSTICK flag.  This causes SDL to scan the system
   for joysticks, and load appropriate drivers.
*/

/* The joystick structure used to identify an SDL joystick */
struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;


/* Function prototypes */
/*
 * Count the number of joysticks attached to the system
 */
extern DECLSPEC int SDLCALL SDL_NumJoysticks(void);

/*
 * Get the implementation dependent name of a joystick.
 * This can be called before any joysticks are opened.
 * If no name can be found, this function returns NULL.
 */
extern DECLSPEC const char * SDLCALL SDL_JoystickName(int device_index);

/*
 * Open a joystick for use - the index passed as an argument refers to
 * the N'th joystick on the system.  This index is the value which will
 * identify this joystick in future joystick events.
 *
 * This function returns a joystick identifier, or NULL if an error occurred.
 */
extern DECLSPEC SDL_Joystick * SDLCALL SDL_JoystickOpen(int device_index);

/*
 * Returns 1 if the joystick has been opened, or 0 if it has not.
 */
extern DECLSPEC int SDLCALL SDL_JoystickOpened(int device_index);

/*
 * Get the device index of an opened joystick.
 */
extern DECLSPEC int SDLCALL SDL_JoystickIndex(SDL_Joystick *joystick);

/*
 * Get the number of general axis controls on a joystick
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumAxes(SDL_Joystick *joystick);

/*
 * Get the number of trackballs on a joystick
 * Joystick trackballs have only relative motion events associated
 * with them and their state cannot be polled.
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumBalls(SDL_Joystick *joystick);

/*
 * Get the number of POV hats on a joystick
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumHats(SDL_Joystick *joystick);

/*
 * Get the number of buttons on a joystick
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumButtons(SDL_Joystick *joystick);

/*
 * Update the current state of the open joysticks.
 * This is called automatically by the event loop if any joystick
 * events are enabled.
 */
extern DECLSPEC void SDLCALL SDL_JoystickUpdate(void);

/*
 * Enable/disable joystick event polling.
 * If joystick events are disabled, you must call SDL_JoystickUpdate()
 * yourself and check the state of the joystick when you want joystick
 * information.
 * The state can be one of SDL_QUERY, SDL_ENABLE or SDL_IGNORE.
 */
extern DECLSPEC int SDLCALL SDL_JoystickEventState(int state);

/*
 * Get the current state of an axis control on a joystick
 * The state is a value ranging from -32768 to 32767.
 * The axis indices start at index 0.
 */
extern DECLSPEC Sint16 SDLCALL SDL_JoystickGetAxis(SDL_Joystick *joystick, int axis);

/*
 * Get the current state of a POV hat on a joystick
 * The return value is one of the following positions:
 */
#define SDL_HAT_CENTERED	0x00
#define SDL_HAT_UP		0x01
#define SDL_HAT_RIGHT		0x02
#define SDL_HAT_DOWN		0x04
#define SDL_HAT_LEFT		0x08
#define SDL_HAT_RIGHTUP		(SDL_HAT_RIGHT|SDL_HAT_UP)
#define SDL_HAT_RIGHTDOWN	(SDL_HAT_RIGHT|SDL_HAT_DOWN)
#define SDL_HAT_LEFTUP		(SDL_HAT_LEFT|SDL_HAT_UP)
#define SDL_HAT_LEFTDOWN	(SDL_HAT_LEFT|SDL_HAT_DOWN)
/*
 * The hat indices start at index 0.
 */
extern DECLSPEC Uint8 SDLCALL SDL_JoystickGetHat(SDL_Joystick *joystick, int hat);

/*
 * Get the ball axis change since the last poll
 * This returns 0, or -1 if you passed it invalid parameters.
 * The ball indices start at index 0.
 */
extern DECLSPEC int SDLCALL SDL_JoystickGetBall(SDL_Joystick *joystick, int ball, int *dx, int *dy);

/*
 * Get the current state of a button on a joystick
 * The button indices start at index 0.
 */
extern DECLSPEC Uint8 SDLCALL SDL_JoystickGetButton(SDL_Joystick *joystick, int button);

/*
 * Close a joystick previously opened with SDL_JoystickOpen()
 */
extern DECLSPEC void SDLCALL SDL_JoystickClose(SDL_Joystick *joystick);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>end:../include/close_code.h


#endif /* _SDL_joystick_h */
#pragma mark >end:../include/SDL_joystick.h

#pragma mark >begin:../include/SDL.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Main include header for the SDL library */

#ifndef _SDL_H
#define _SDL_H

#pragma mark >>begin:../include/SDL_main.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

#ifndef _SDL_main_h
#define _SDL_main_h

/* Redefine main() on Win32 and MacOS so that it is called by winmain.c */

#if defined(WIN32) || defined(_WIN32) || \
    (defined(__MWERKS__) && !defined(__BEOS__)) || \
    defined(macintosh) || defined(__APPLE__) || \
    defined(__SYMBIAN32__) || defined(QWS)

#ifdef __cplusplus
#define C_LINKAGE	"C"
#else
#define C_LINKAGE
#endif /* __cplusplus */

/* The application's main() function must be called with C linkage,
   and should be declared like this:
#ifdef __cplusplus
extern "C"
#endif
	int main(int argc, char *argv[])
	{
	}
 */
#define main	SDL_main

/* The prototype for the application's main() function */
extern C_LINKAGE int SDL_main(int argc, char *argv[]);


/* From the SDL library code -- needed for registering the app on Win32 */
#if defined(WIN32)
#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h


#ifdef __cplusplus
extern "C" {
#endif

/* This should be called from your WinMain() function, if any */
extern DECLSPEC void SDLCALL SDL_SetModuleHandle(void *hInst);
/* This can also be called, but is no longer necessary */
extern DECLSPEC int SDLCALL SDL_RegisterApp(char *name, Uint32 style, void *hInst);

#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h

#endif

/* From the SDL library code -- needed for registering QuickDraw on MacOS */
#if defined(macintosh)
#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h


#ifdef __cplusplus
extern "C" {
#endif

/* Forward declaration so we don't need to include QuickDraw.h */
struct QDGlobals;

/* This should be called from your main() function, if any */
extern DECLSPEC void SDLCALL SDL_InitQuickDraw(struct QDGlobals *the_qd);

#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h

#endif

#endif /* Need to redefine main()? */

#endif /* _SDL_main_h */
#pragma mark >>end:../include/SDL_main.h

#pragma mark >>begin:../include/SDL_getenv.h


/* Not all environments have a working getenv()/putenv() */

#if defined(macintosh) || defined(_WIN32_WCE)
#define NEED_SDL_GETENV
#endif

#ifdef NEED_SDL_GETENV

#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Put a variable of the form "name=value" into the environment */
extern DECLSPEC int SDLCALL SDL_putenv(const char *variable);
#define putenv(X)   SDL_putenv(X)

/* Retrieve a variable named "name" from the environment */
extern DECLSPEC char * SDLCALL SDL_getenv(const char *name);
#define getenv(X)     SDL_getenv(X)

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h


#endif /* NEED_GETENV */
#pragma mark >>end:../include/SDL_getenv.h

#pragma mark >>begin:../include/SDL_error.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Simple error message routines for SDL */

#ifndef _SDL_error_h
#define _SDL_error_h

#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Public functions */
extern DECLSPEC void SDLCALL SDL_SetError(const char *fmt, ...);
extern DECLSPEC char * SDLCALL SDL_GetError(void);
extern DECLSPEC void SDLCALL SDL_ClearError(void);

/* Private error message function - used internally */
#define SDL_OutOfMemory()	SDL_Error(SDL_ENOMEM)
typedef enum {
	SDL_ENOMEM,
	SDL_EFREAD,
	SDL_EFWRITE,
	SDL_EFSEEK,
	SDL_LASTERROR
} SDL_errorcode;
extern DECLSPEC void SDLCALL SDL_Error(SDL_errorcode code);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h


#endif /* _SDL_error_h */
#pragma mark >>end:../include/SDL_error.h

#pragma mark >>begin:../include/SDL_rwops.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* This file provides a general interface for SDL to read and write
   data sources.  It can easily be extended to files, memory, etc.
*/

#ifndef _SDL_RWops_h
#define _SDL_RWops_h

#include <stdio.h>


#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* This is the read/write operation structure -- very basic */

typedef struct SDL_RWops {
	/* Seek to 'offset' relative to whence, one of stdio's whence values:
		SEEK_SET, SEEK_CUR, SEEK_END
	   Returns the final offset in the data source.
	 */
	int (SDLCALL *seek)(struct SDL_RWops *context, int offset, int whence);

	/* Read up to 'num' objects each of size 'objsize' from the data
	   source to the area pointed at by 'ptr'.
	   Returns the number of objects read, or -1 if the read failed.
	 */
	int (SDLCALL *read)(struct SDL_RWops *context, void *ptr, int size, int maxnum);

	/* Write exactly 'num' objects each of size 'objsize' from the area
	   pointed at by 'ptr' to data source.
	   Returns 'num', or -1 if the write failed.
	 */
	int (SDLCALL *write)(struct SDL_RWops *context, const void *ptr, int size, int num);

	/* Close and free an allocated SDL_FSops structure */
	int (SDLCALL *close)(struct SDL_RWops *context);

	Uint32 type;
	union {
	    struct {
		int autoclose;
	 	FILE *fp;
	    } stdio;
	    struct {
		Uint8 *base;
	 	Uint8 *here;
		Uint8 *stop;
	    } mem;
	    struct {
		void *data1;
	    } unknown;
	} hidden;

} SDL_RWops;


/* Functions to create SDL_RWops structures from various data sources */

extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromFile(const char *file, const char *mode);

extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromFP(FILE *fp, int autoclose);

extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromMem(void *mem, int size);
extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromConstMem(const void *mem, int size);

extern DECLSPEC SDL_RWops * SDLCALL SDL_AllocRW(void);
extern DECLSPEC void SDLCALL SDL_FreeRW(SDL_RWops *area);

/* Macros to easily read and write from an SDL_RWops structure */
#define SDL_RWseek(ctx, offset, whence)	(ctx)->seek(ctx, offset, whence)
#define SDL_RWtell(ctx)			(ctx)->seek(ctx, 0, SEEK_CUR)
#define SDL_RWread(ctx, ptr, size, n)	(ctx)->read(ctx, ptr, size, n)
#define SDL_RWwrite(ctx, ptr, size, n)	(ctx)->write(ctx, ptr, size, n)
#define SDL_RWclose(ctx)		(ctx)->close(ctx)


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h


#endif /* _SDL_RWops_h */
#pragma mark >>end:../include/SDL_rwops.h

#pragma mark >>begin:../include/SDL_timer.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

#ifndef _SDL_timer_h
#define _SDL_timer_h

/* Header for the SDL time management routines */


#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* This is the OS scheduler timeslice, in milliseconds */
#define SDL_TIMESLICE		10

/* This is the maximum resolution of the SDL timer on all platforms */
#define TIMER_RESOLUTION	10	/* Experimentally determined */

/* Get the number of milliseconds since the SDL library initialization.
 * Note that this value wraps if the program runs for more than ~49 days.
 */ 
extern DECLSPEC Uint32 SDLCALL SDL_GetTicks(void);

/* Wait a specified number of milliseconds before returning */
extern DECLSPEC void SDLCALL SDL_Delay(Uint32 ms);

/* Function prototype for the timer callback function */
typedef Uint32 (SDLCALL *SDL_TimerCallback)(Uint32 interval);

/* Set a callback to run after the specified number of milliseconds has
 * elapsed. The callback function is passed the current timer interval
 * and returns the next timer interval.  If the returned value is the 
 * same as the one passed in, the periodic alarm continues, otherwise a
 * new alarm is scheduled.  If the callback returns 0, the periodic alarm
 * is cancelled.
 *
 * To cancel a currently running timer, call SDL_SetTimer(0, NULL);
 *
 * The timer callback function may run in a different thread than your
 * main code, and so shouldn't call any functions from within itself.
 *
 * The maximum resolution of this timer is 10 ms, which means that if
 * you request a 16 ms timer, your callback will run approximately 20 ms
 * later on an unloaded system.  If you wanted to set a flag signaling
 * a frame update at 30 frames per second (every 33 ms), you might set a 
 * timer for 30 ms:
 *   SDL_SetTimer((33/10)*10, flag_update);
 *
 * If you use this function, you need to pass SDL_INIT_TIMER to SDL_Init().
 *
 * Under UNIX, you should not use raise or use SIGALRM and this function
 * in the same program, as it is implemented using setitimer().  You also
 * should not use this function in multi-threaded applications as signals
 * to multi-threaded apps have undefined behavior in some implementations.
 *
 * This function returns 0 if successful, or -1 if there was an error.
 */
extern DECLSPEC int SDLCALL SDL_SetTimer(Uint32 interval, SDL_TimerCallback callback);

/* New timer API, supports multiple timers
 * Written by Stephane Peter <megastep@lokigames.com>
 */

/* Function prototype for the new timer callback function.
 * The callback function is passed the current timer interval and returns
 * the next timer interval.  If the returned value is the same as the one
 * passed in, the periodic alarm continues, otherwise a new alarm is
 * scheduled.  If the callback returns 0, the periodic alarm is cancelled.
 */
typedef Uint32 (SDLCALL *SDL_NewTimerCallback)(Uint32 interval, void *param);

/* Definition of the timer ID type */
typedef struct _SDL_TimerID *SDL_TimerID;

/* Add a new timer to the pool of timers already running.
   Returns a timer ID, or NULL when an error occurs.
 */
extern DECLSPEC SDL_TimerID SDLCALL SDL_AddTimer(Uint32 interval, SDL_NewTimerCallback callback, void *param);

/* Remove one of the multiple timers knowing its ID.
 * Returns a boolean value indicating success.
 */
extern DECLSPEC SDL_bool SDLCALL SDL_RemoveTimer(SDL_TimerID t);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h


#endif /* _SDL_timer_h */
#pragma mark >>end:../include/SDL_timer.h

#pragma mark >>begin:../include/SDL_audio.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Access to the raw audio mixing buffer for the SDL library */

#ifndef _SDL_audio_h
#define _SDL_audio_h

#include <stdio.h>

#pragma mark >>>begin:../include/SDL_byteorder.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Macros for determining the byte-order of this platform */

#ifndef _SDL_byteorder_h
#define _SDL_byteorder_h

/* The two types of endianness */
#define SDL_LIL_ENDIAN	1234
#define SDL_BIG_ENDIAN	4321

/* Pardon the mess, I'm trying to determine the endianness of this host.
   I'm doing it by preprocessor defines rather than some sort of configure
   script so that application code can use this too.  The "right" way would
   be to dynamically generate this file on install, but that's a lot of work.
 */
#if (defined(__i386__) || defined(__i386)) || \
     defined(__ia64__) || defined(WIN32) || \
    (defined(__alpha__) || defined(__alpha)) || \
     defined(__arm__) || \
    (defined(__mips__) && defined(__MIPSEL__)) || \
     defined(__SYMBIAN32__) || \
     defined(__x86_64__) || \
     defined(__LITTLE_ENDIAN__)
#define SDL_BYTEORDER	SDL_LIL_ENDIAN
#else
#define SDL_BYTEORDER	SDL_BIG_ENDIAN
#endif

#endif /* _SDL_byteorder_h */
#pragma mark >>>end:../include/SDL_byteorder.h


#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* The calculated values in this structure are calculated by SDL_OpenAudio() */
typedef struct SDL_AudioSpec {
	int freq;		/* DSP frequency -- samples per second */
	Uint16 format;		/* Audio data format */
	Uint8  channels;	/* Number of channels: 1 mono, 2 stereo */
	Uint8  silence;		/* Audio buffer silence value (calculated) */
	Uint16 samples;		/* Audio buffer size in samples (power of 2) */
	Uint16 padding;		/* Necessary for some compile environments */
	Uint32 size;		/* Audio buffer size in bytes (calculated) */
	/* This function is called when the audio device needs more data.
	   'stream' is a pointer to the audio data buffer
	   'len' is the length of that buffer in bytes.
	   Once the callback returns, the buffer will no longer be valid.
	   Stereo samples are stored in a LRLRLR ordering.
	*/
	void (SDLCALL *callback)(void *userdata, Uint8 *stream, int len);
	void  *userdata;
} SDL_AudioSpec;

/* Audio format flags (defaults to LSB byte order) */
#define AUDIO_U8	0x0008	/* Unsigned 8-bit samples */
#define AUDIO_S8	0x8008	/* Signed 8-bit samples */
#define AUDIO_U16LSB	0x0010	/* Unsigned 16-bit samples */
#define AUDIO_S16LSB	0x8010	/* Signed 16-bit samples */
#define AUDIO_U16MSB	0x1010	/* As above, but big-endian byte order */
#define AUDIO_S16MSB	0x9010	/* As above, but big-endian byte order */
#define AUDIO_U16	AUDIO_U16LSB
#define AUDIO_S16	AUDIO_S16LSB

/* Native audio byte ordering */
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define AUDIO_U16SYS	AUDIO_U16LSB
#define AUDIO_S16SYS	AUDIO_S16LSB
#else
#define AUDIO_U16SYS	AUDIO_U16MSB
#define AUDIO_S16SYS	AUDIO_S16MSB
#endif


/* A structure to hold a set of audio conversion filters and buffers */
typedef struct SDL_AudioCVT {
	int needed;			/* Set to 1 if conversion possible */
	Uint16 src_format;		/* Source audio format */
	Uint16 dst_format;		/* Target audio format */
	double rate_incr;		/* Rate conversion increment */
	Uint8 *buf;			/* Buffer to hold entire audio data */
	int    len;			/* Length of original audio buffer */
	int    len_cvt;			/* Length of converted audio buffer */
	int    len_mult;		/* buffer must be len*len_mult big */
	double len_ratio; 	/* Given len, final size is len*len_ratio */
	void (SDLCALL *filters[10])(struct SDL_AudioCVT *cvt, Uint16 format);
	int filter_index;		/* Current audio conversion function */
} SDL_AudioCVT;


/* Function prototypes */

/* These functions are used internally, and should not be used unless you
 * have a specific need to specify the audio driver you want to use.
 * You should normally use SDL_Init() or SDL_InitSubSystem().
 */
extern DECLSPEC int SDLCALL SDL_AudioInit(const char *driver_name);
extern DECLSPEC void SDLCALL SDL_AudioQuit(void);

/* This function fills the given character buffer with the name of the
 * current audio driver, and returns a pointer to it if the audio driver has
 * been initialized.  It returns NULL if no driver has been initialized.
 */
extern DECLSPEC char * SDLCALL SDL_AudioDriverName(char *namebuf, int maxlen);

/*
 * This function opens the audio device with the desired parameters, and
 * returns 0 if successful, placing the actual hardware parameters in the
 * structure pointed to by 'obtained'.  If 'obtained' is NULL, the audio
 * data passed to the callback function will be guaranteed to be in the
 * requested format, and will be automatically converted to the hardware
 * audio format if necessary.  This function returns -1 if it failed 
 * to open the audio device, or couldn't set up the audio thread.
 *
 * When filling in the desired audio spec structure,
 *  'desired->freq' should be the desired audio frequency in samples-per-second.
 *  'desired->format' should be the desired audio format.
 *  'desired->samples' is the desired size of the audio buffer, in samples.
 *     This number should be a power of two, and may be adjusted by the audio
 *     driver to a value more suitable for the hardware.  Good values seem to
 *     range between 512 and 8096 inclusive, depending on the application and
 *     CPU speed.  Smaller values yield faster response time, but can lead
 *     to underflow if the application is doing heavy processing and cannot
 *     fill the audio buffer in time.  A stereo sample consists of both right
 *     and left channels in LR ordering.
 *     Note that the number of samples is directly related to time by the
 *     following formula:  ms = (samples*1000)/freq
 *  'desired->size' is the size in bytes of the audio buffer, and is
 *     calculated by SDL_OpenAudio().
 *  'desired->silence' is the value used to set the buffer to silence,
 *     and is calculated by SDL_OpenAudio().
 *  'desired->callback' should be set to a function that will be called
 *     when the audio device is ready for more data.  It is passed a pointer
 *     to the audio buffer, and the length in bytes of the audio buffer.
 *     This function usually runs in a separate thread, and so you should
 *     protect data structures that it accesses by calling SDL_LockAudio()
 *     and SDL_UnlockAudio() in your code.
 *  'desired->userdata' is passed as the first parameter to your callback
 *     function.
 *
 * The audio device starts out playing silence when it's opened, and should
 * be enabled for playing by calling SDL_PauseAudio(0) when you are ready
 * for your audio callback function to be called.  Since the audio driver
 * may modify the requested size of the audio buffer, you should allocate
 * any local mixing buffers after you open the audio device.
 */
extern DECLSPEC int SDLCALL SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained);

/*
 * Get the current audio state:
 */
typedef enum {
	SDL_AUDIO_STOPPED = 0,
	SDL_AUDIO_PLAYING,
	SDL_AUDIO_PAUSED
} SDL_audiostatus;
extern DECLSPEC SDL_audiostatus SDLCALL SDL_GetAudioStatus(void);

/*
 * This function pauses and unpauses the audio callback processing.
 * It should be called with a parameter of 0 after opening the audio
 * device to start playing sound.  This is so you can safely initialize
 * data for your callback function after opening the audio device.
 * Silence will be written to the audio device during the pause.
 */
extern DECLSPEC void SDLCALL SDL_PauseAudio(int pause_on);

/*
 * This function loads a WAVE from the data source, automatically freeing
 * that source if 'freesrc' is non-zero.  For example, to load a WAVE file,
 * you could do:
 *	SDL_LoadWAV_RW(SDL_RWFromFile("sample.wav", "rb"), 1, ...);
 *
 * If this function succeeds, it returns the given SDL_AudioSpec,
 * filled with the audio data format of the wave data, and sets
 * 'audio_buf' to a malloc()'d buffer containing the audio data,
 * and sets 'audio_len' to the length of that audio buffer, in bytes.
 * You need to free the audio buffer with SDL_FreeWAV() when you are 
 * done with it.
 *
 * This function returns NULL and sets the SDL error message if the 
 * wave file cannot be opened, uses an unknown data format, or is 
 * corrupt.  Currently raw and MS-ADPCM WAVE files are supported.
 */
extern DECLSPEC SDL_AudioSpec * SDLCALL SDL_LoadWAV_RW(SDL_RWops *src, int freesrc, SDL_AudioSpec *spec, Uint8 **audio_buf, Uint32 *audio_len);

/* Compatibility convenience function -- loads a WAV from a file */
#define SDL_LoadWAV(file, spec, audio_buf, audio_len) \
	SDL_LoadWAV_RW(SDL_RWFromFile(file, "rb"),1, spec,audio_buf,audio_len)

/*
 * This function frees data previously allocated with SDL_LoadWAV_RW()
 */
extern DECLSPEC void SDLCALL SDL_FreeWAV(Uint8 *audio_buf);

/*
 * This function takes a source format and rate and a destination format
 * and rate, and initializes the 'cvt' structure with information needed
 * by SDL_ConvertAudio() to convert a buffer of audio data from one format
 * to the other.
 * This function returns 0, or -1 if there was an error.
 */
extern DECLSPEC int SDLCALL SDL_BuildAudioCVT(SDL_AudioCVT *cvt,
		Uint16 src_format, Uint8 src_channels, int src_rate,
		Uint16 dst_format, Uint8 dst_channels, int dst_rate);

/* Once you have initialized the 'cvt' structure using SDL_BuildAudioCVT(),
 * created an audio buffer cvt->buf, and filled it with cvt->len bytes of
 * audio data in the source format, this function will convert it in-place
 * to the desired format.
 * The data conversion may expand the size of the audio data, so the buffer
 * cvt->buf should be allocated after the cvt structure is initialized by
 * SDL_BuildAudioCVT(), and should be cvt->len*cvt->len_mult bytes long.
 */
extern DECLSPEC int SDLCALL SDL_ConvertAudio(SDL_AudioCVT *cvt);

/*
 * This takes two audio buffers of the playing audio format and mixes
 * them, performing addition, volume adjustment, and overflow clipping.
 * The volume ranges from 0 - 128, and should be set to SDL_MIX_MAXVOLUME
 * for full audio volume.  Note this does not change hardware volume.
 * This is provided for convenience -- you can mix your own audio data.
 */
#define SDL_MIX_MAXVOLUME 128
extern DECLSPEC void SDLCALL SDL_MixAudio(Uint8 *dst, const Uint8 *src, Uint32 len, int volume);

/*
 * The lock manipulated by these functions protects the callback function.
 * During a LockAudio/UnlockAudio pair, you can be guaranteed that the
 * callback function is not running.  Do not call these from the callback
 * function or you will cause deadlock.
 */
extern DECLSPEC void SDLCALL SDL_LockAudio(void);
extern DECLSPEC void SDLCALL SDL_UnlockAudio(void);

/*
 * This function shuts down audio processing and closes the audio device.
 */
extern DECLSPEC void SDLCALL SDL_CloseAudio(void);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h


#endif /* _SDL_audio_h */
#pragma mark >>end:../include/SDL_audio.h

#pragma mark >>begin:../include/SDL_cdrom.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* This is the CD-audio control API for Simple DirectMedia Layer */

#ifndef _SDL_cdrom_h
#define _SDL_cdrom_h


#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* In order to use these functions, SDL_Init() must have been called
   with the SDL_INIT_CDROM flag.  This causes SDL to scan the system
   for CD-ROM drives, and load appropriate drivers.
*/

/* The maximum number of CD-ROM tracks on a disk */
#define SDL_MAX_TRACKS	99

/* The types of CD-ROM track possible */
#define SDL_AUDIO_TRACK	0x00
#define SDL_DATA_TRACK	0x04

/* The possible states which a CD-ROM drive can be in. */
typedef enum {
	CD_TRAYEMPTY,
	CD_STOPPED,
	CD_PLAYING,
	CD_PAUSED,
	CD_ERROR = -1
} CDstatus;

/* Given a status, returns true if there's a disk in the drive */
#define CD_INDRIVE(status)	((int)(status) > 0)

typedef struct SDL_CDtrack {
	Uint8 id;		/* Track number */
	Uint8 type;		/* Data or audio track */
	Uint16 unused;
	Uint32 length;		/* Length, in frames, of this track */
	Uint32 offset;		/* Offset, in frames, from start of disk */
} SDL_CDtrack;

/* This structure is only current as of the last call to SDL_CDStatus() */
typedef struct SDL_CD {
	int id;			/* Private drive identifier */
	CDstatus status;	/* Current drive status */

	/* The rest of this structure is only valid if there's a CD in drive */
	int numtracks;		/* Number of tracks on disk */
	int cur_track;		/* Current track position */
	int cur_frame;		/* Current frame offset within current track */
	SDL_CDtrack track[SDL_MAX_TRACKS+1];
} SDL_CD;

/* Conversion functions from frames to Minute/Second/Frames and vice versa */
#define CD_FPS	75
#define FRAMES_TO_MSF(f, M,S,F)	{					\
	int value = f;							\
	*(F) = value%CD_FPS;						\
	value /= CD_FPS;						\
	*(S) = value%60;						\
	value /= 60;							\
	*(M) = value;							\
}
#define MSF_TO_FRAMES(M, S, F)	((M)*60*CD_FPS+(S)*CD_FPS+(F))

/* CD-audio API functions: */

/* Returns the number of CD-ROM drives on the system, or -1 if
   SDL_Init() has not been called with the SDL_INIT_CDROM flag.
 */
extern DECLSPEC int SDLCALL SDL_CDNumDrives(void);

/* Returns a human-readable, system-dependent identifier for the CD-ROM.
   Example:
	"/dev/cdrom"
	"E:"
	"/dev/disk/ide/1/master"
*/
extern DECLSPEC const char * SDLCALL SDL_CDName(int drive);

/* Opens a CD-ROM drive for access.  It returns a drive handle on success,
   or NULL if the drive was invalid or busy.  This newly opened CD-ROM
   becomes the default CD used when other CD functions are passed a NULL
   CD-ROM handle.
   Drives are numbered starting with 0.  Drive 0 is the system default CD-ROM.
*/
extern DECLSPEC SDL_CD * SDLCALL SDL_CDOpen(int drive);

/* This function returns the current status of the given drive.
   If the drive has a CD in it, the table of contents of the CD and current
   play position of the CD will be stored in the SDL_CD structure.
*/
extern DECLSPEC CDstatus SDLCALL SDL_CDStatus(SDL_CD *cdrom);

/* Play the given CD starting at 'start_track' and 'start_frame' for 'ntracks'
   tracks and 'nframes' frames.  If both 'ntrack' and 'nframe' are 0, play 
   until the end of the CD.  This function will skip data tracks.
   This function should only be called after calling SDL_CDStatus() to 
   get track information about the CD.
   For example:
	// Play entire CD:
	if ( CD_INDRIVE(SDL_CDStatus(cdrom)) )
		SDL_CDPlayTracks(cdrom, 0, 0, 0, 0);
	// Play last track:
	if ( CD_INDRIVE(SDL_CDStatus(cdrom)) ) {
		SDL_CDPlayTracks(cdrom, cdrom->numtracks-1, 0, 0, 0);
	}
	// Play first and second track and 10 seconds of third track:
	if ( CD_INDRIVE(SDL_CDStatus(cdrom)) )
		SDL_CDPlayTracks(cdrom, 0, 0, 2, 10);

   This function returns 0, or -1 if there was an error.
*/
extern DECLSPEC int SDLCALL SDL_CDPlayTracks(SDL_CD *cdrom,
		int start_track, int start_frame, int ntracks, int nframes);

/* Play the given CD starting at 'start' frame for 'length' frames.
   It returns 0, or -1 if there was an error.
*/
extern DECLSPEC int SDLCALL SDL_CDPlay(SDL_CD *cdrom, int start, int length);

/* Pause play -- returns 0, or -1 on error */
extern DECLSPEC int SDLCALL SDL_CDPause(SDL_CD *cdrom);

/* Resume play -- returns 0, or -1 on error */
extern DECLSPEC int SDLCALL SDL_CDResume(SDL_CD *cdrom);

/* Stop play -- returns 0, or -1 on error */
extern DECLSPEC int SDLCALL SDL_CDStop(SDL_CD *cdrom);

/* Eject CD-ROM -- returns 0, or -1 on error */
extern DECLSPEC int SDLCALL SDL_CDEject(SDL_CD *cdrom);

/* Closes the handle for the CD-ROM drive */
extern DECLSPEC void SDLCALL SDL_CDClose(SDL_CD *cdrom);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h


#endif /* _SDL_video_h */
#pragma mark >>end:../include/SDL_cdrom.h

#pragma mark >>begin:../include/SDL_joystick.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Include file for SDL joystick event handling */

#ifndef _SDL_joystick_h
#define _SDL_joystick_h


#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* In order to use these functions, SDL_Init() must have been called
   with the SDL_INIT_JOYSTICK flag.  This causes SDL to scan the system
   for joysticks, and load appropriate drivers.
*/

/* The joystick structure used to identify an SDL joystick */
struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;


/* Function prototypes */
/*
 * Count the number of joysticks attached to the system
 */
extern DECLSPEC int SDLCALL SDL_NumJoysticks(void);

/*
 * Get the implementation dependent name of a joystick.
 * This can be called before any joysticks are opened.
 * If no name can be found, this function returns NULL.
 */
extern DECLSPEC const char * SDLCALL SDL_JoystickName(int device_index);

/*
 * Open a joystick for use - the index passed as an argument refers to
 * the N'th joystick on the system.  This index is the value which will
 * identify this joystick in future joystick events.
 *
 * This function returns a joystick identifier, or NULL if an error occurred.
 */
extern DECLSPEC SDL_Joystick * SDLCALL SDL_JoystickOpen(int device_index);

/*
 * Returns 1 if the joystick has been opened, or 0 if it has not.
 */
extern DECLSPEC int SDLCALL SDL_JoystickOpened(int device_index);

/*
 * Get the device index of an opened joystick.
 */
extern DECLSPEC int SDLCALL SDL_JoystickIndex(SDL_Joystick *joystick);

/*
 * Get the number of general axis controls on a joystick
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumAxes(SDL_Joystick *joystick);

/*
 * Get the number of trackballs on a joystick
 * Joystick trackballs have only relative motion events associated
 * with them and their state cannot be polled.
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumBalls(SDL_Joystick *joystick);

/*
 * Get the number of POV hats on a joystick
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumHats(SDL_Joystick *joystick);

/*
 * Get the number of buttons on a joystick
 */
extern DECLSPEC int SDLCALL SDL_JoystickNumButtons(SDL_Joystick *joystick);

/*
 * Update the current state of the open joysticks.
 * This is called automatically by the event loop if any joystick
 * events are enabled.
 */
extern DECLSPEC void SDLCALL SDL_JoystickUpdate(void);

/*
 * Enable/disable joystick event polling.
 * If joystick events are disabled, you must call SDL_JoystickUpdate()
 * yourself and check the state of the joystick when you want joystick
 * information.
 * The state can be one of SDL_QUERY, SDL_ENABLE or SDL_IGNORE.
 */
extern DECLSPEC int SDLCALL SDL_JoystickEventState(int state);

/*
 * Get the current state of an axis control on a joystick
 * The state is a value ranging from -32768 to 32767.
 * The axis indices start at index 0.
 */
extern DECLSPEC Sint16 SDLCALL SDL_JoystickGetAxis(SDL_Joystick *joystick, int axis);

/*
 * Get the current state of a POV hat on a joystick
 * The return value is one of the following positions:
 */
#define SDL_HAT_CENTERED	0x00
#define SDL_HAT_UP		0x01
#define SDL_HAT_RIGHT		0x02
#define SDL_HAT_DOWN		0x04
#define SDL_HAT_LEFT		0x08
#define SDL_HAT_RIGHTUP		(SDL_HAT_RIGHT|SDL_HAT_UP)
#define SDL_HAT_RIGHTDOWN	(SDL_HAT_RIGHT|SDL_HAT_DOWN)
#define SDL_HAT_LEFTUP		(SDL_HAT_LEFT|SDL_HAT_UP)
#define SDL_HAT_LEFTDOWN	(SDL_HAT_LEFT|SDL_HAT_DOWN)
/*
 * The hat indices start at index 0.
 */
extern DECLSPEC Uint8 SDLCALL SDL_JoystickGetHat(SDL_Joystick *joystick, int hat);

/*
 * Get the ball axis change since the last poll
 * This returns 0, or -1 if you passed it invalid parameters.
 * The ball indices start at index 0.
 */
extern DECLSPEC int SDLCALL SDL_JoystickGetBall(SDL_Joystick *joystick, int ball, int *dx, int *dy);

/*
 * Get the current state of a button on a joystick
 * The button indices start at index 0.
 */
extern DECLSPEC Uint8 SDLCALL SDL_JoystickGetButton(SDL_Joystick *joystick, int button);

/*
 * Close a joystick previously opened with SDL_JoystickOpen()
 */
extern DECLSPEC void SDLCALL SDL_JoystickClose(SDL_Joystick *joystick);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h


#endif /* _SDL_joystick_h */
#pragma mark >>end:../include/SDL_joystick.h

#pragma mark >>begin:../include/SDL_events.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Include file for SDL event handling */

#ifndef _SDL_events_h
#define _SDL_events_h

#pragma mark >>>begin:../include/SDL_active.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Include file for SDL application focus event handling */

#ifndef _SDL_active_h
#define _SDL_active_h

#pragma mark >>>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* The available application states */
#define SDL_APPMOUSEFOCUS	0x01		/* The app has mouse coverage */
#define SDL_APPINPUTFOCUS	0x02		/* The app has input focus */
#define SDL_APPACTIVE		0x04		/* The application is active */

/* Function prototypes */
/* 
 * This function returns the current state of the application, which is a
 * bitwise combination of SDL_APPMOUSEFOCUS, SDL_APPINPUTFOCUS, and
 * SDL_APPACTIVE.  If SDL_APPACTIVE is set, then the user is able to
 * see your application, otherwise it has been iconified or disabled.
 */
extern DECLSPEC Uint8 SDLCALL SDL_GetAppState(void);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>>end:../include/close_code.h


#endif /* _SDL_active_h */
#pragma mark >>>end:../include/SDL_active.h

#pragma mark >>>begin:../include/SDL_keyboard.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Include file for SDL keyboard event handling */

#ifndef _SDL_keyboard_h
#define _SDL_keyboard_h

#pragma mark >>>>begin:../include/SDL_keysym.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

#ifndef _SDL_keysym_h
#define _SDL_keysym_h

/* What we really want is a mapping of every raw key on the keyboard.
   To support international keyboards, we use the range 0xA1 - 0xFF
   as international virtual keycodes.  We'll follow in the footsteps of X11...
   The names of the keys
 */
 
typedef enum {
	/* The keyboard syms have been cleverly chosen to map to ASCII */
	SDLK_UNKNOWN		= 0,
	SDLK_FIRST		= 0,
	SDLK_BACKSPACE		= 8,
	SDLK_TAB		= 9,
	SDLK_CLEAR		= 12,
	SDLK_RETURN		= 13,
	SDLK_PAUSE		= 19,
	SDLK_ESCAPE		= 27,
	SDLK_SPACE		= 32,
	SDLK_EXCLAIM		= 33,
	SDLK_QUOTEDBL		= 34,
	SDLK_HASH		= 35,
	SDLK_DOLLAR		= 36,
	SDLK_AMPERSAND		= 38,
	SDLK_QUOTE		= 39,
	SDLK_LEFTPAREN		= 40,
	SDLK_RIGHTPAREN		= 41,
	SDLK_ASTERISK		= 42,
	SDLK_PLUS		= 43,
	SDLK_COMMA		= 44,
	SDLK_MINUS		= 45,
	SDLK_PERIOD		= 46,
	SDLK_SLASH		= 47,
	SDLK_0			= 48,
	SDLK_1			= 49,
	SDLK_2			= 50,
	SDLK_3			= 51,
	SDLK_4			= 52,
	SDLK_5			= 53,
	SDLK_6			= 54,
	SDLK_7			= 55,
	SDLK_8			= 56,
	SDLK_9			= 57,
	SDLK_COLON		= 58,
	SDLK_SEMICOLON		= 59,
	SDLK_LESS		= 60,
	SDLK_EQUALS		= 61,
	SDLK_GREATER		= 62,
	SDLK_QUESTION		= 63,
	SDLK_AT			= 64,
	/* 
	   Skip uppercase letters
	 */
	SDLK_LEFTBRACKET	= 91,
	SDLK_BACKSLASH		= 92,
	SDLK_RIGHTBRACKET	= 93,
	SDLK_CARET		= 94,
	SDLK_UNDERSCORE		= 95,
	SDLK_BACKQUOTE		= 96,
	SDLK_a			= 97,
	SDLK_b			= 98,
	SDLK_c			= 99,
	SDLK_d			= 100,
	SDLK_e			= 101,
	SDLK_f			= 102,
	SDLK_g			= 103,
	SDLK_h			= 104,
	SDLK_i			= 105,
	SDLK_j			= 106,
	SDLK_k			= 107,
	SDLK_l			= 108,
	SDLK_m			= 109,
	SDLK_n			= 110,
	SDLK_o			= 111,
	SDLK_p			= 112,
	SDLK_q			= 113,
	SDLK_r			= 114,
	SDLK_s			= 115,
	SDLK_t			= 116,
	SDLK_u			= 117,
	SDLK_v			= 118,
	SDLK_w			= 119,
	SDLK_x			= 120,
	SDLK_y			= 121,
	SDLK_z			= 122,
	SDLK_DELETE		= 127,
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
	SDLK_WORLD_0		= 160,		/* 0xA0 */
	SDLK_WORLD_1		= 161,
	SDLK_WORLD_2		= 162,
	SDLK_WORLD_3		= 163,
	SDLK_WORLD_4		= 164,
	SDLK_WORLD_5		= 165,
	SDLK_WORLD_6		= 166,
	SDLK_WORLD_7		= 167,
	SDLK_WORLD_8		= 168,
	SDLK_WORLD_9		= 169,
	SDLK_WORLD_10		= 170,
	SDLK_WORLD_11		= 171,
	SDLK_WORLD_12		= 172,
	SDLK_WORLD_13		= 173,
	SDLK_WORLD_14		= 174,
	SDLK_WORLD_15		= 175,
	SDLK_WORLD_16		= 176,
	SDLK_WORLD_17		= 177,
	SDLK_WORLD_18		= 178,
	SDLK_WORLD_19		= 179,
	SDLK_WORLD_20		= 180,
	SDLK_WORLD_21		= 181,
	SDLK_WORLD_22		= 182,
	SDLK_WORLD_23		= 183,
	SDLK_WORLD_24		= 184,
	SDLK_WORLD_25		= 185,
	SDLK_WORLD_26		= 186,
	SDLK_WORLD_27		= 187,
	SDLK_WORLD_28		= 188,
	SDLK_WORLD_29		= 189,
	SDLK_WORLD_30		= 190,
	SDLK_WORLD_31		= 191,
	SDLK_WORLD_32		= 192,
	SDLK_WORLD_33		= 193,
	SDLK_WORLD_34		= 194,
	SDLK_WORLD_35		= 195,
	SDLK_WORLD_36		= 196,
	SDLK_WORLD_37		= 197,
	SDLK_WORLD_38		= 198,
	SDLK_WORLD_39		= 199,
	SDLK_WORLD_40		= 200,
	SDLK_WORLD_41		= 201,
	SDLK_WORLD_42		= 202,
	SDLK_WORLD_43		= 203,
	SDLK_WORLD_44		= 204,
	SDLK_WORLD_45		= 205,
	SDLK_WORLD_46		= 206,
	SDLK_WORLD_47		= 207,
	SDLK_WORLD_48		= 208,
	SDLK_WORLD_49		= 209,
	SDLK_WORLD_50		= 210,
	SDLK_WORLD_51		= 211,
	SDLK_WORLD_52		= 212,
	SDLK_WORLD_53		= 213,
	SDLK_WORLD_54		= 214,
	SDLK_WORLD_55		= 215,
	SDLK_WORLD_56		= 216,
	SDLK_WORLD_57		= 217,
	SDLK_WORLD_58		= 218,
	SDLK_WORLD_59		= 219,
	SDLK_WORLD_60		= 220,
	SDLK_WORLD_61		= 221,
	SDLK_WORLD_62		= 222,
	SDLK_WORLD_63		= 223,
	SDLK_WORLD_64		= 224,
	SDLK_WORLD_65		= 225,
	SDLK_WORLD_66		= 226,
	SDLK_WORLD_67		= 227,
	SDLK_WORLD_68		= 228,
	SDLK_WORLD_69		= 229,
	SDLK_WORLD_70		= 230,
	SDLK_WORLD_71		= 231,
	SDLK_WORLD_72		= 232,
	SDLK_WORLD_73		= 233,
	SDLK_WORLD_74		= 234,
	SDLK_WORLD_75		= 235,
	SDLK_WORLD_76		= 236,
	SDLK_WORLD_77		= 237,
	SDLK_WORLD_78		= 238,
	SDLK_WORLD_79		= 239,
	SDLK_WORLD_80		= 240,
	SDLK_WORLD_81		= 241,
	SDLK_WORLD_82		= 242,
	SDLK_WORLD_83		= 243,
	SDLK_WORLD_84		= 244,
	SDLK_WORLD_85		= 245,
	SDLK_WORLD_86		= 246,
	SDLK_WORLD_87		= 247,
	SDLK_WORLD_88		= 248,
	SDLK_WORLD_89		= 249,
	SDLK_WORLD_90		= 250,
	SDLK_WORLD_91		= 251,
	SDLK_WORLD_92		= 252,
	SDLK_WORLD_93		= 253,
	SDLK_WORLD_94		= 254,
	SDLK_WORLD_95		= 255,		/* 0xFF */

	/* Numeric keypad */
	SDLK_KP0		= 256,
	SDLK_KP1		= 257,
	SDLK_KP2		= 258,
	SDLK_KP3		= 259,
	SDLK_KP4		= 260,
	SDLK_KP5		= 261,
	SDLK_KP6		= 262,
	SDLK_KP7		= 263,
	SDLK_KP8		= 264,
	SDLK_KP9		= 265,
	SDLK_KP_PERIOD		= 266,
	SDLK_KP_DIVIDE		= 267,
	SDLK_KP_MULTIPLY	= 268,
	SDLK_KP_MINUS		= 269,
	SDLK_KP_PLUS		= 270,
	SDLK_KP_ENTER		= 271,
	SDLK_KP_EQUALS		= 272,

	/* Arrows + Home/End pad */
	SDLK_UP			= 273,
	SDLK_DOWN		= 274,
	SDLK_RIGHT		= 275,
	SDLK_LEFT		= 276,
	SDLK_INSERT		= 277,
	SDLK_HOME		= 278,
	SDLK_END		= 279,
	SDLK_PAGEUP		= 280,
	SDLK_PAGEDOWN		= 281,

	/* Function keys */
	SDLK_F1			= 282,
	SDLK_F2			= 283,
	SDLK_F3			= 284,
	SDLK_F4			= 285,
	SDLK_F5			= 286,
	SDLK_F6			= 287,
	SDLK_F7			= 288,
	SDLK_F8			= 289,
	SDLK_F9			= 290,
	SDLK_F10		= 291,
	SDLK_F11		= 292,
	SDLK_F12		= 293,
	SDLK_F13		= 294,
	SDLK_F14		= 295,
	SDLK_F15		= 296,

	/* Key state modifier keys */
	SDLK_NUMLOCK		= 300,
	SDLK_CAPSLOCK		= 301,
	SDLK_SCROLLOCK		= 302,
	SDLK_RSHIFT		= 303,
	SDLK_LSHIFT		= 304,
	SDLK_RCTRL		= 305,
	SDLK_LCTRL		= 306,
	SDLK_RALT		= 307,
	SDLK_LALT		= 308,
	SDLK_RMETA		= 309,
	SDLK_LMETA		= 310,
	SDLK_LSUPER		= 311,		/* Left "Windows" key */
	SDLK_RSUPER		= 312,		/* Right "Windows" key */
	SDLK_MODE		= 313,		/* "Alt Gr" key */
	SDLK_COMPOSE		= 314,		/* Multi-key compose key */

	/* Miscellaneous function keys */
	SDLK_HELP		= 315,
	SDLK_PRINT		= 316,
	SDLK_SYSREQ		= 317,
	SDLK_BREAK		= 318,
	SDLK_MENU		= 319,
	SDLK_POWER		= 320,		/* Power Macintosh power key */
	SDLK_EURO		= 321,		/* Some european keyboards */
	SDLK_UNDO		= 322,		/* Atari keyboard has Undo */

	/* Add any other keys here */

	SDLK_LAST
} SDLKey;

/* Enumeration of valid key mods (possibly OR'd together) */
typedef enum {
	KMOD_NONE  = 0x0000,
	KMOD_LSHIFT= 0x0001,
	KMOD_RSHIFT= 0x0002,
	KMOD_LCTRL = 0x0040,
	KMOD_RCTRL = 0x0080,
	KMOD_LALT  = 0x0100,
	KMOD_RALT  = 0x0200,
	KMOD_LMETA = 0x0400,
	KMOD_RMETA = 0x0800,
	KMOD_NUM   = 0x1000,
	KMOD_CAPS  = 0x2000,
	KMOD_MODE  = 0x4000,
	KMOD_RESERVED = 0x8000
} SDLMod;

#define KMOD_CTRL	(KMOD_LCTRL|KMOD_RCTRL)
#define KMOD_SHIFT	(KMOD_LSHIFT|KMOD_RSHIFT)
#define KMOD_ALT	(KMOD_LALT|KMOD_RALT)
#define KMOD_META	(KMOD_LMETA|KMOD_RMETA)

#endif /* _SDL_keysym_h */
#pragma mark >>>>end:../include/SDL_keysym.h


#pragma mark >>>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Keysym structure
   - The scancode is hardware dependent, and should not be used by general
     applications.  If no hardware scancode is available, it will be 0.

   - The 'unicode' translated character is only available when character
     translation is enabled by the SDL_EnableUNICODE() API.  If non-zero,
     this is a UNICODE character corresponding to the keypress.  If the
     high 9 bits of the character are 0, then this maps to the equivalent
     ASCII character:
	char ch;
	if ( (keysym.unicode & 0xFF80) == 0 ) {
		ch = keysym.unicode & 0x7F;
	} else {
		An international character..
	}
 */
typedef struct SDL_keysym {
	Uint8 scancode;			/* hardware specific scancode */
	SDLKey sym;			/* SDL virtual keysym */
	SDLMod mod;			/* current key modifiers */
	Uint16 unicode;			/* translated character */
} SDL_keysym;

/* This is the mask which refers to all hotkey bindings */
#define SDL_ALL_HOTKEYS		0xFFFFFFFF

/* Function prototypes */
/*
 * Enable/Disable UNICODE translation of keyboard input.
 * This translation has some overhead, so translation defaults off.
 * If 'enable' is 1, translation is enabled.
 * If 'enable' is 0, translation is disabled.
 * If 'enable' is -1, the translation state is not changed.
 * It returns the previous state of keyboard translation.
 */
extern DECLSPEC int SDLCALL SDL_EnableUNICODE(int enable);

/*
 * Enable/Disable keyboard repeat.  Keyboard repeat defaults to off.
 * 'delay' is the initial delay in ms between the time when a key is
 * pressed, and keyboard repeat begins.
 * 'interval' is the time in ms between keyboard repeat events.
 */
#define SDL_DEFAULT_REPEAT_DELAY	500
#define SDL_DEFAULT_REPEAT_INTERVAL	30
/*
 * If 'delay' is set to 0, keyboard repeat is disabled.
 */
extern DECLSPEC int SDLCALL SDL_EnableKeyRepeat(int delay, int interval);

/*
 * Get a snapshot of the current state of the keyboard.
 * Returns an array of keystates, indexed by the SDLK_* syms.
 * Used:
 * 	Uint8 *keystate = SDL_GetKeyState(NULL);
 *	if ( keystate[SDLK_RETURN] ) ... <RETURN> is pressed.
 */
extern DECLSPEC Uint8 * SDLCALL SDL_GetKeyState(int *numkeys);

/*
 * Get the current key modifier state
 */
extern DECLSPEC SDLMod SDLCALL SDL_GetModState(void);

/*
 * Set the current key modifier state
 * This does not change the keyboard state, only the key modifier flags.
 */
extern DECLSPEC void SDLCALL SDL_SetModState(SDLMod modstate);

/*
 * Get the name of an SDL virtual keysym
 */
extern DECLSPEC char * SDLCALL SDL_GetKeyName(SDLKey key);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>>end:../include/close_code.h


#endif /* _SDL_keyboard_h */
#pragma mark >>>end:../include/SDL_keyboard.h

#pragma mark >>>begin:../include/SDL_mouse.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Include file for SDL mouse event handling */

#ifndef _SDL_mouse_h
#define _SDL_mouse_h

#pragma mark >>>>begin:../include/SDL_video.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Header file for access to the SDL raw framebuffer window */

#ifndef _SDL_video_h
#define _SDL_video_h

#include <stdio.h>

#pragma mark >>>>>begin:../include/SDL_mutex.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

#ifndef _SDL_mutex_h
#define _SDL_mutex_h

/* Functions to provide thread synchronization primitives

	These are independent of the other SDL routines.
*/


#pragma mark >>>>>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>>>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Synchronization functions which can time out return this value
   if they time out.
*/
#define SDL_MUTEX_TIMEDOUT	1

/* This is the timeout value which corresponds to never time out */
#define SDL_MUTEX_MAXWAIT	(~(Uint32)0)


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Mutex functions                                               */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* The SDL mutex structure, defined in SDL_mutex.c */
struct SDL_mutex;
typedef struct SDL_mutex SDL_mutex;

/* Create a mutex, initialized unlocked */
extern DECLSPEC SDL_mutex * SDLCALL SDL_CreateMutex(void);

/* Lock the mutex  (Returns 0, or -1 on error) */
#define SDL_LockMutex(m)	SDL_mutexP(m)
extern DECLSPEC int SDLCALL SDL_mutexP(SDL_mutex *mutex);

/* Unlock the mutex  (Returns 0, or -1 on error)
   It is an error to unlock a mutex that has not been locked by
   the current thread, and doing so results in undefined behavior.
 */
#define SDL_UnlockMutex(m)	SDL_mutexV(m)
extern DECLSPEC int SDLCALL SDL_mutexV(SDL_mutex *mutex);

/* Destroy a mutex */
extern DECLSPEC void SDLCALL SDL_DestroyMutex(SDL_mutex *mutex);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Semaphore functions                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* The SDL semaphore structure, defined in SDL_sem.c */
struct SDL_semaphore;
typedef struct SDL_semaphore SDL_sem;

/* Create a semaphore, initialized with value, returns NULL on failure. */
extern DECLSPEC SDL_sem * SDLCALL SDL_CreateSemaphore(Uint32 initial_value);

/* Destroy a semaphore */
extern DECLSPEC void SDLCALL SDL_DestroySemaphore(SDL_sem *sem);

/* This function suspends the calling thread until the semaphore pointed 
 * to by sem has a positive count. It then atomically decreases the semaphore
 * count.
 */
extern DECLSPEC int SDLCALL SDL_SemWait(SDL_sem *sem);

/* Non-blocking variant of SDL_SemWait(), returns 0 if the wait succeeds,
   SDL_MUTEX_TIMEDOUT if the wait would block, and -1 on error.
*/
extern DECLSPEC int SDLCALL SDL_SemTryWait(SDL_sem *sem);

/* Variant of SDL_SemWait() with a timeout in milliseconds, returns 0 if
   the wait succeeds, SDL_MUTEX_TIMEDOUT if the wait does not succeed in
   the allotted time, and -1 on error.
   On some platforms this function is implemented by looping with a delay
   of 1 ms, and so should be avoided if possible.
*/
extern DECLSPEC int SDLCALL SDL_SemWaitTimeout(SDL_sem *sem, Uint32 ms);

/* Atomically increases the semaphore's count (not blocking), returns 0,
   or -1 on error.
 */
extern DECLSPEC int SDLCALL SDL_SemPost(SDL_sem *sem);

/* Returns the current count of the semaphore */
extern DECLSPEC Uint32 SDLCALL SDL_SemValue(SDL_sem *sem);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Condition variable functions                                  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* The SDL condition variable structure, defined in SDL_cond.c */
struct SDL_cond;
typedef struct SDL_cond SDL_cond;

/* Create a condition variable */
extern DECLSPEC SDL_cond * SDLCALL SDL_CreateCond(void);

/* Destroy a condition variable */
extern DECLSPEC void SDLCALL SDL_DestroyCond(SDL_cond *cond);

/* Restart one of the threads that are waiting on the condition variable,
   returns 0 or -1 on error.
 */
extern DECLSPEC int SDLCALL SDL_CondSignal(SDL_cond *cond);

/* Restart all threads that are waiting on the condition variable,
   returns 0 or -1 on error.
 */
extern DECLSPEC int SDLCALL SDL_CondBroadcast(SDL_cond *cond);

/* Wait on the condition variable, unlocking the provided mutex.
   The mutex must be locked before entering this function!
   Returns 0 when it is signaled, or -1 on error.
 */
extern DECLSPEC int SDLCALL SDL_CondWait(SDL_cond *cond, SDL_mutex *mut);

/* Waits for at most 'ms' milliseconds, and returns 0 if the condition
   variable is signaled, SDL_MUTEX_TIMEDOUT if the condition is not
   signaled in the allotted time, and -1 on error.
   On some platforms this function is implemented by looping with a delay
   of 1 ms, and so should be avoided if possible.
*/
extern DECLSPEC int SDLCALL SDL_CondWaitTimeout(SDL_cond *cond, SDL_mutex *mutex, Uint32 ms);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>>>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>>>>end:../include/close_code.h


#endif /* _SDL_mutex_h */
#pragma mark >>>>>end:../include/SDL_mutex.h


#pragma mark >>>>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Transparency definitions: These define alpha as the opacity of a surface */
#define SDL_ALPHA_OPAQUE 255
#define SDL_ALPHA_TRANSPARENT 0

/* Useful data types */
typedef struct SDL_Rect {
	Sint16 x, y;
	Uint16 w, h;
} SDL_Rect;

typedef struct SDL_Color {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 unused;
} SDL_Color;
#define SDL_Colour SDL_Color

typedef struct SDL_Palette {
	int       ncolors;
	SDL_Color *colors;
} SDL_Palette;

/* Everything in the pixel format structure is read-only */
typedef struct SDL_PixelFormat {
	SDL_Palette *palette;
	Uint8  BitsPerPixel;
	Uint8  BytesPerPixel;
	Uint8  Rloss;
	Uint8  Gloss;
	Uint8  Bloss;
	Uint8  Aloss;
	Uint8  Rshift;
	Uint8  Gshift;
	Uint8  Bshift;
	Uint8  Ashift;
	Uint32 Rmask;
	Uint32 Gmask;
	Uint32 Bmask;
	Uint32 Amask;

	/* RGB color key information */
	Uint32 colorkey;
	/* Alpha value information (per-surface alpha) */
	Uint8  alpha;
} SDL_PixelFormat;

/* This structure should be treated as read-only, except for 'pixels',
   which, if not NULL, contains the raw pixel data for the surface.
*/
typedef struct SDL_Surface {
	Uint32 flags;				/* Read-only */
	SDL_PixelFormat *format;		/* Read-only */
	int w, h;				/* Read-only */
	Uint16 pitch;				/* Read-only */
	void *pixels;				/* Read-write */
	int offset;				/* Private */

	/* Hardware-specific surface info */
	struct private_hwdata *hwdata;

	/* clipping information */
	SDL_Rect clip_rect;			/* Read-only */
	Uint32 unused1;				/* for binary compatibility */

	/* Allow recursive locks */
	Uint32 locked;				/* Private */

	/* info for fast blit mapping to other surfaces */
	struct SDL_BlitMap *map;		/* Private */

	/* format version, bumped at every change to invalidate blit maps */
	unsigned int format_version;		/* Private */

	/* Reference count -- used when freeing surface */
	int refcount;				/* Read-mostly */
} SDL_Surface;

/* These are the currently supported flags for the SDL_surface */
/* Available for SDL_CreateRGBSurface() or SDL_SetVideoMode() */
#define SDL_SWSURFACE	0x00000000	/* Surface is in system memory */
#define SDL_HWSURFACE	0x00000001	/* Surface is in video memory */
#define SDL_ASYNCBLIT	0x00000004	/* Use asynchronous blits if possible */
/* Available for SDL_SetVideoMode() */
#define SDL_ANYFORMAT	0x10000000	/* Allow any video depth/pixel-format */
#define SDL_HWPALETTE	0x20000000	/* Surface has exclusive palette */
#define SDL_DOUBLEBUF	0x40000000	/* Set up double-buffered video mode */
#define SDL_FULLSCREEN	0x80000000	/* Surface is a full screen display */
#define SDL_OPENGL      0x00000002      /* Create an OpenGL rendering context */
#define SDL_OPENGLBLIT	0x0000000A	/* Create an OpenGL rendering context and use it for blitting */
#define SDL_RESIZABLE	0x00000010	/* This video mode may be resized */
#define SDL_NOFRAME	0x00000020	/* No window caption or edge frame */
/* Used internally (read-only) */
#define SDL_HWACCEL	0x00000100	/* Blit uses hardware acceleration */
#define SDL_SRCCOLORKEY	0x00001000	/* Blit uses a source color key */
#define SDL_RLEACCELOK	0x00002000	/* Private flag */
#define SDL_RLEACCEL	0x00004000	/* Surface is RLE encoded */
#define SDL_SRCALPHA	0x00010000	/* Blit uses source alpha blending */
#define SDL_PREALLOC	0x01000000	/* Surface uses preallocated memory */

/* Evaluates to true if the surface needs to be locked before access */
#define SDL_MUSTLOCK(surface)	\
  (surface->offset ||		\
  ((surface->flags & (SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_RLEACCEL)) != 0))

/* typedef for private surface blitting functions */
typedef int (*SDL_blit)(struct SDL_Surface *src, SDL_Rect *srcrect,
			struct SDL_Surface *dst, SDL_Rect *dstrect);


/* Useful for determining the video hardware capabilities */
typedef struct SDL_VideoInfo {
	Uint32 hw_available :1;	/* Flag: Can you create hardware surfaces? */
	Uint32 wm_available :1;	/* Flag: Can you talk to a window manager? */
	Uint32 UnusedBits1  :6;
	Uint32 UnusedBits2  :1;
	Uint32 blit_hw      :1;	/* Flag: Accelerated blits HW --> HW */
	Uint32 blit_hw_CC   :1;	/* Flag: Accelerated blits with Colorkey */
	Uint32 blit_hw_A    :1;	/* Flag: Accelerated blits with Alpha */
	Uint32 blit_sw      :1;	/* Flag: Accelerated blits SW --> HW */
	Uint32 blit_sw_CC   :1;	/* Flag: Accelerated blits with Colorkey */
	Uint32 blit_sw_A    :1;	/* Flag: Accelerated blits with Alpha */
	Uint32 blit_fill    :1;	/* Flag: Accelerated color fill */
	Uint32 UnusedBits3  :16;
	Uint32 video_mem;	/* The total amount of video memory (in K) */
	SDL_PixelFormat *vfmt;	/* Value: The format of the video surface */
} SDL_VideoInfo;


/* The most common video overlay formats.
   For an explanation of these pixel formats, see:
	http://www.webartz.com/fourcc/indexyuv.htm

   For information on the relationship between color spaces, see:
   http://www.neuro.sfc.keio.ac.jp/~aly/polygon/info/color-space-faq.html
 */
#define SDL_YV12_OVERLAY  0x32315659	/* Planar mode: Y + V + U  (3 planes) */
#define SDL_IYUV_OVERLAY  0x56555949	/* Planar mode: Y + U + V  (3 planes) */
#define SDL_YUY2_OVERLAY  0x32595559	/* Packed mode: Y0+U0+Y1+V0 (1 plane) */
#define SDL_UYVY_OVERLAY  0x59565955	/* Packed mode: U0+Y0+V0+Y1 (1 plane) */
#define SDL_YVYU_OVERLAY  0x55595659	/* Packed mode: Y0+V0+Y1+U0 (1 plane) */

/* The YUV hardware video overlay */
typedef struct SDL_Overlay {
	Uint32 format;				/* Read-only */
	int w, h;				/* Read-only */
	int planes;				/* Read-only */
	Uint16 *pitches;			/* Read-only */
	Uint8 **pixels;				/* Read-write */

	/* Hardware-specific surface info */
	struct private_yuvhwfuncs *hwfuncs;
	struct private_yuvhwdata *hwdata;

	/* Special flags */
	Uint32 hw_overlay :1;	/* Flag: This overlay hardware accelerated? */
	Uint32 UnusedBits :31;
} SDL_Overlay;


/* Public enumeration for setting the OpenGL window attributes. */
typedef enum {
    SDL_GL_RED_SIZE,
    SDL_GL_GREEN_SIZE,
    SDL_GL_BLUE_SIZE,
    SDL_GL_ALPHA_SIZE,
    SDL_GL_BUFFER_SIZE,
    SDL_GL_DOUBLEBUFFER,
    SDL_GL_DEPTH_SIZE,
    SDL_GL_STENCIL_SIZE,
    SDL_GL_ACCUM_RED_SIZE,
    SDL_GL_ACCUM_GREEN_SIZE,
    SDL_GL_ACCUM_BLUE_SIZE,
    SDL_GL_ACCUM_ALPHA_SIZE,
    SDL_GL_STEREO,
    SDL_GL_MULTISAMPLEBUFFERS,
    SDL_GL_MULTISAMPLESAMPLES
} SDL_GLattr;

/* flags for SDL_SetPalette() */
#define SDL_LOGPAL 0x01
#define SDL_PHYSPAL 0x02

/* Function prototypes */

/* These functions are used internally, and should not be used unless you
 * have a specific need to specify the video driver you want to use.
 * You should normally use SDL_Init() or SDL_InitSubSystem().
 *
 * SDL_VideoInit() initializes the video subsystem -- sets up a connection
 * to the window manager, etc, and determines the current video mode and
 * pixel format, but does not initialize a window or graphics mode.
 * Note that event handling is activated by this routine.
 *
 * If you use both sound and video in your application, you need to call
 * SDL_Init() before opening the sound device, otherwise under Win32 DirectX,
 * you won't be able to set full-screen display modes.
 */
extern DECLSPEC int SDLCALL SDL_VideoInit(const char *driver_name, Uint32 flags);
extern DECLSPEC void SDLCALL SDL_VideoQuit(void);

/* This function fills the given character buffer with the name of the
 * video driver, and returns a pointer to it if the video driver has
 * been initialized.  It returns NULL if no driver has been initialized.
 */
extern DECLSPEC char * SDLCALL SDL_VideoDriverName(char *namebuf, int maxlen);

/*
 * This function returns a pointer to the current display surface.
 * If SDL is doing format conversion on the display surface, this
 * function returns the publicly visible surface, not the real video
 * surface.
 */
extern DECLSPEC SDL_Surface * SDLCALL SDL_GetVideoSurface(void);

/*
 * This function returns a read-only pointer to information about the
 * video hardware.  If this is called before SDL_SetVideoMode(), the 'vfmt'
 * member of the returned structure will contain the pixel format of the
 * "best" video mode.
 */
extern DECLSPEC const SDL_VideoInfo * SDLCALL SDL_GetVideoInfo(void);

/* 
 * Check to see if a particular video mode is supported.
 * It returns 0 if the requested mode is not supported under any bit depth,
 * or returns the bits-per-pixel of the closest available mode with the
 * given width and height.  If this bits-per-pixel is different from the
 * one used when setting the video mode, SDL_SetVideoMode() will succeed,
 * but will emulate the requested bits-per-pixel with a shadow surface.
 *
 * The arguments to SDL_VideoModeOK() are the same ones you would pass to
 * SDL_SetVideoMode()
 */
extern DECLSPEC int SDLCALL SDL_VideoModeOK(int width, int height, int bpp, Uint32 flags);

/*
 * Return a pointer to an array of available screen dimensions for the
 * given format and video flags, sorted largest to smallest.  Returns 
 * NULL if there are no dimensions available for a particular format, 
 * or (SDL_Rect **)-1 if any dimension is okay for the given format.
 *
 * If 'format' is NULL, the mode list will be for the format given 
 * by SDL_GetVideoInfo()->vfmt
 */
extern DECLSPEC SDL_Rect ** SDLCALL SDL_ListModes(SDL_PixelFormat *format, Uint32 flags);

/*
 * Set up a video mode with the specified width, height and bits-per-pixel.
 *
 * If 'bpp' is 0, it is treated as the current display bits per pixel.
 *
 * If SDL_ANYFORMAT is set in 'flags', the SDL library will try to set the
 * requested bits-per-pixel, but will return whatever video pixel format is
 * available.  The default is to emulate the requested pixel format if it
 * is not natively available.
 *
 * If SDL_HWSURFACE is set in 'flags', the video surface will be placed in
 * video memory, if possible, and you may have to call SDL_LockSurface()
 * in order to access the raw framebuffer.  Otherwise, the video surface
 * will be created in system memory.
 *
 * If SDL_ASYNCBLIT is set in 'flags', SDL will try to perform rectangle
 * updates asynchronously, but you must always lock before accessing pixels.
 * SDL will wait for updates to complete before returning from the lock.
 *
 * If SDL_HWPALETTE is set in 'flags', the SDL library will guarantee
 * that the colors set by SDL_SetColors() will be the colors you get.
 * Otherwise, in 8-bit mode, SDL_SetColors() may not be able to set all
 * of the colors exactly the way they are requested, and you should look
 * at the video surface structure to determine the actual palette.
 * If SDL cannot guarantee that the colors you request can be set, 
 * i.e. if the colormap is shared, then the video surface may be created
 * under emulation in system memory, overriding the SDL_HWSURFACE flag.
 *
 * If SDL_FULLSCREEN is set in 'flags', the SDL library will try to set
 * a fullscreen video mode.  The default is to create a windowed mode
 * if the current graphics system has a window manager.
 * If the SDL library is able to set a fullscreen video mode, this flag 
 * will be set in the surface that is returned.
 *
 * If SDL_DOUBLEBUF is set in 'flags', the SDL library will try to set up
 * two surfaces in video memory and swap between them when you call 
 * SDL_Flip().  This is usually slower than the normal single-buffering
 * scheme, but prevents "tearing" artifacts caused by modifying video 
 * memory while the monitor is refreshing.  It should only be used by 
 * applications that redraw the entire screen on every update.
 *
 * If SDL_RESIZABLE is set in 'flags', the SDL library will allow the
 * window manager, if any, to resize the window at runtime.  When this
 * occurs, SDL will send a SDL_VIDEORESIZE event to you application,
 * and you must respond to the event by re-calling SDL_SetVideoMode()
 * with the requested size (or another size that suits the application).
 *
 * If SDL_NOFRAME is set in 'flags', the SDL library will create a window
 * without any title bar or frame decoration.  Fullscreen video modes have
 * this flag set automatically.
 *
 * This function returns the video framebuffer surface, or NULL if it fails.
 *
 * If you rely on functionality provided by certain video flags, check the
 * flags of the returned surface to make sure that functionality is available.
 * SDL will fall back to reduced functionality if the exact flags you wanted
 * are not available.
 */
extern DECLSPEC SDL_Surface * SDLCALL SDL_SetVideoMode
			(int width, int height, int bpp, Uint32 flags);

/*
 * Makes sure the given list of rectangles is updated on the given screen.
 * If 'x', 'y', 'w' and 'h' are all 0, SDL_UpdateRect will update the entire
 * screen.
 * These functions should not be called while 'screen' is locked.
 */
extern DECLSPEC void SDLCALL SDL_UpdateRects
		(SDL_Surface *screen, int numrects, SDL_Rect *rects);
extern DECLSPEC void SDLCALL SDL_UpdateRect
		(SDL_Surface *screen, Sint32 x, Sint32 y, Uint32 w, Uint32 h);

/*
 * On hardware that supports double-buffering, this function sets up a flip
 * and returns.  The hardware will wait for vertical retrace, and then swap
 * video buffers before the next video surface blit or lock will return.
 * On hardware that doesn not support double-buffering, this is equivalent
 * to calling SDL_UpdateRect(screen, 0, 0, 0, 0);
 * The SDL_DOUBLEBUF flag must have been passed to SDL_SetVideoMode() when
 * setting the video mode for this function to perform hardware flipping.
 * This function returns 0 if successful, or -1 if there was an error.
 */
extern DECLSPEC int SDLCALL SDL_Flip(SDL_Surface *screen);

/*
 * Set the gamma correction for each of the color channels.
 * The gamma values range (approximately) between 0.1 and 10.0
 * 
 * If this function isn't supported directly by the hardware, it will
 * be emulated using gamma ramps, if available.  If successful, this
 * function returns 0, otherwise it returns -1.
 */
extern DECLSPEC int SDLCALL SDL_SetGamma(float red, float green, float blue);

/*
 * Set the gamma translation table for the red, green, and blue channels
 * of the video hardware.  Each table is an array of 256 16-bit quantities,
 * representing a mapping between the input and output for that channel.
 * The input is the index into the array, and the output is the 16-bit
 * gamma value at that index, scaled to the output color precision.
 * 
 * You may pass NULL for any of the channels to leave it unchanged.
 * If the call succeeds, it will return 0.  If the display driver or
 * hardware does not support gamma translation, or otherwise fails,
 * this function will return -1.
 */
extern DECLSPEC int SDLCALL SDL_SetGammaRamp(const Uint16 *red, const Uint16 *green, const Uint16 *blue);

/*
 * Retrieve the current values of the gamma translation tables.
 * 
 * You must pass in valid pointers to arrays of 256 16-bit quantities.
 * Any of the pointers may be NULL to ignore that channel.
 * If the call succeeds, it will return 0.  If the display driver or
 * hardware does not support gamma translation, or otherwise fails,
 * this function will return -1.
 */
extern DECLSPEC int SDLCALL SDL_GetGammaRamp(Uint16 *red, Uint16 *green, Uint16 *blue);

/*
 * Sets a portion of the colormap for the given 8-bit surface.  If 'surface'
 * is not a palettized surface, this function does nothing, returning 0.
 * If all of the colors were set as passed to SDL_SetColors(), it will
 * return 1.  If not all the color entries were set exactly as given,
 * it will return 0, and you should look at the surface palette to
 * determine the actual color palette.
 *
 * When 'surface' is the surface associated with the current display, the
 * display colormap will be updated with the requested colors.  If 
 * SDL_HWPALETTE was set in SDL_SetVideoMode() flags, SDL_SetColors()
 * will always return 1, and the palette is guaranteed to be set the way
 * you desire, even if the window colormap has to be warped or run under
 * emulation.
 */
extern DECLSPEC int SDLCALL SDL_SetColors(SDL_Surface *surface, 
			SDL_Color *colors, int firstcolor, int ncolors);

/*
 * Sets a portion of the colormap for a given 8-bit surface.
 * 'flags' is one or both of:
 * SDL_LOGPAL  -- set logical palette, which controls how blits are mapped
 *                to/from the surface,
 * SDL_PHYSPAL -- set physical palette, which controls how pixels look on
 *                the screen
 * Only screens have physical palettes. Separate change of physical/logical
 * palettes is only possible if the screen has SDL_HWPALETTE set.
 *
 * The return value is 1 if all colours could be set as requested, and 0
 * otherwise.
 *
 * SDL_SetColors() is equivalent to calling this function with
 *     flags = (SDL_LOGPAL|SDL_PHYSPAL).
 */
extern DECLSPEC int SDLCALL SDL_SetPalette(SDL_Surface *surface, int flags,
				   SDL_Color *colors, int firstcolor,
				   int ncolors);

/*
 * Maps an RGB triple to an opaque pixel value for a given pixel format
 */
extern DECLSPEC Uint32 SDLCALL SDL_MapRGB
			(SDL_PixelFormat *format, Uint8 r, Uint8 g, Uint8 b);

/*
 * Maps an RGBA quadruple to a pixel value for a given pixel format
 */
extern DECLSPEC Uint32 SDLCALL SDL_MapRGBA(SDL_PixelFormat *format,
				   Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/*
 * Maps a pixel value into the RGB components for a given pixel format
 */
extern DECLSPEC void SDLCALL SDL_GetRGB(Uint32 pixel, SDL_PixelFormat *fmt,
				Uint8 *r, Uint8 *g, Uint8 *b);

/*
 * Maps a pixel value into the RGBA components for a given pixel format
 */
extern DECLSPEC void SDLCALL SDL_GetRGBA(Uint32 pixel, SDL_PixelFormat *fmt,
				 Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);

/*
 * Allocate and free an RGB surface (must be called after SDL_SetVideoMode)
 * If the depth is 4 or 8 bits, an empty palette is allocated for the surface.
 * If the depth is greater than 8 bits, the pixel format is set using the
 * flags '[RGB]mask'.
 * If the function runs out of memory, it will return NULL.
 *
 * The 'flags' tell what kind of surface to create.
 * SDL_SWSURFACE means that the surface should be created in system memory.
 * SDL_HWSURFACE means that the surface should be created in video memory,
 * with the same format as the display surface.  This is useful for surfaces
 * that will not change much, to take advantage of hardware acceleration
 * when being blitted to the display surface.
 * SDL_ASYNCBLIT means that SDL will try to perform asynchronous blits with
 * this surface, but you must always lock it before accessing the pixels.
 * SDL will wait for current blits to finish before returning from the lock.
 * SDL_SRCCOLORKEY indicates that the surface will be used for colorkey blits.
 * If the hardware supports acceleration of colorkey blits between
 * two surfaces in video memory, SDL will try to place the surface in
 * video memory. If this isn't possible or if there is no hardware
 * acceleration available, the surface will be placed in system memory.
 * SDL_SRCALPHA means that the surface will be used for alpha blits and 
 * if the hardware supports hardware acceleration of alpha blits between
 * two surfaces in video memory, to place the surface in video memory
 * if possible, otherwise it will be placed in system memory.
 * If the surface is created in video memory, blits will be _much_ faster,
 * but the surface format must be identical to the video surface format,
 * and the only way to access the pixels member of the surface is to use
 * the SDL_LockSurface() and SDL_UnlockSurface() calls.
 * If the requested surface actually resides in video memory, SDL_HWSURFACE
 * will be set in the flags member of the returned surface.  If for some
 * reason the surface could not be placed in video memory, it will not have
 * the SDL_HWSURFACE flag set, and will be created in system memory instead.
 */
#define SDL_AllocSurface    SDL_CreateRGBSurface
extern DECLSPEC SDL_Surface * SDLCALL SDL_CreateRGBSurface
			(Uint32 flags, int width, int height, int depth, 
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
extern DECLSPEC SDL_Surface * SDLCALL SDL_CreateRGBSurfaceFrom(void *pixels,
			int width, int height, int depth, int pitch,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
extern DECLSPEC void SDLCALL SDL_FreeSurface(SDL_Surface *surface);

/*
 * SDL_LockSurface() sets up a surface for directly accessing the pixels.
 * Between calls to SDL_LockSurface()/SDL_UnlockSurface(), you can write
 * to and read from 'surface->pixels', using the pixel format stored in 
 * 'surface->format'.  Once you are done accessing the surface, you should 
 * use SDL_UnlockSurface() to release it.
 *
 * Not all surfaces require locking.  If SDL_MUSTLOCK(surface) evaluates
 * to 0, then you can read and write to the surface at any time, and the
 * pixel format of the surface will not change.  In particular, if the
 * SDL_HWSURFACE flag is not given when calling SDL_SetVideoMode(), you
 * will not need to lock the display surface before accessing it.
 * 
 * No operating system or library calls should be made between lock/unlock
 * pairs, as critical system locks may be held during this time.
 *
 * SDL_LockSurface() returns 0, or -1 if the surface couldn't be locked.
 */
extern DECLSPEC int SDLCALL SDL_LockSurface(SDL_Surface *surface);
extern DECLSPEC void SDLCALL SDL_UnlockSurface(SDL_Surface *surface);

/*
 * Load a surface from a seekable SDL data source (memory or file.)
 * If 'freesrc' is non-zero, the source will be closed after being read.
 * Returns the new surface, or NULL if there was an error.
 * The new surface should be freed with SDL_FreeSurface().
 */
extern DECLSPEC SDL_Surface * SDLCALL SDL_LoadBMP_RW(SDL_RWops *src, int freesrc);

/* Convenience macro -- load a surface from a file */
#define SDL_LoadBMP(file)	SDL_LoadBMP_RW(SDL_RWFromFile(file, "rb"), 1)

/*
 * Save a surface to a seekable SDL data source (memory or file.)
 * If 'freedst' is non-zero, the source will be closed after being written.
 * Returns 0 if successful or -1 if there was an error.
 */
extern DECLSPEC int SDLCALL SDL_SaveBMP_RW
		(SDL_Surface *surface, SDL_RWops *dst, int freedst);

/* Convenience macro -- save a surface to a file */
#define SDL_SaveBMP(surface, file) \
		SDL_SaveBMP_RW(surface, SDL_RWFromFile(file, "wb"), 1)

/*
 * Sets the color key (transparent pixel) in a blittable surface.
 * If 'flag' is SDL_SRCCOLORKEY (optionally OR'd with SDL_RLEACCEL), 
 * 'key' will be the transparent pixel in the source image of a blit.
 * SDL_RLEACCEL requests RLE acceleration for the surface if present,
 * and removes RLE acceleration if absent.
 * If 'flag' is 0, this function clears any current color key.
 * This function returns 0, or -1 if there was an error.
 */
extern DECLSPEC int SDLCALL SDL_SetColorKey
			(SDL_Surface *surface, Uint32 flag, Uint32 key);

/*
 * This function sets the alpha value for the entire surface, as opposed to
 * using the alpha component of each pixel. This value measures the range
 * of transparency of the surface, 0 being completely transparent to 255
 * being completely opaque. An 'alpha' value of 255 causes blits to be
 * opaque, the source pixels copied to the destination (the default). Note
 * that per-surface alpha can be combined with colorkey transparency.
 *
 * If 'flag' is 0, alpha blending is disabled for the surface.
 * If 'flag' is SDL_SRCALPHA, alpha blending is enabled for the surface.
 * OR:ing the flag with SDL_RLEACCEL requests RLE acceleration for the
 * surface; if SDL_RLEACCEL is not specified, the RLE accel will be removed.
 *
 * The 'alpha' parameter is ignored for surfaces that have an alpha channel.
 */
extern DECLSPEC int SDLCALL SDL_SetAlpha(SDL_Surface *surface, Uint32 flag, Uint8 alpha);

/*
 * Sets the clipping rectangle for the destination surface in a blit.
 *
 * If the clip rectangle is NULL, clipping will be disabled.
 * If the clip rectangle doesn't intersect the surface, the function will
 * return SDL_FALSE and blits will be completely clipped.  Otherwise the
 * function returns SDL_TRUE and blits to the surface will be clipped to
 * the intersection of the surface area and the clipping rectangle.
 *
 * Note that blits are automatically clipped to the edges of the source
 * and destination surfaces.
 */
extern DECLSPEC SDL_bool SDLCALL SDL_SetClipRect(SDL_Surface *surface, const SDL_Rect *rect);

/*
 * Gets the clipping rectangle for the destination surface in a blit.
 * 'rect' must be a pointer to a valid rectangle which will be filled
 * with the correct values.
 */
extern DECLSPEC void SDLCALL SDL_GetClipRect(SDL_Surface *surface, SDL_Rect *rect);

/*
 * Creates a new surface of the specified format, and then copies and maps 
 * the given surface to it so the blit of the converted surface will be as 
 * fast as possible.  If this function fails, it returns NULL.
 *
 * The 'flags' parameter is passed to SDL_CreateRGBSurface() and has those 
 * semantics.  You can also pass SDL_RLEACCEL in the flags parameter and
 * SDL will try to RLE accelerate colorkey and alpha blits in the resulting
 * surface.
 *
 * This function is used internally by SDL_DisplayFormat().
 */
extern DECLSPEC SDL_Surface * SDLCALL SDL_ConvertSurface
			(SDL_Surface *src, SDL_PixelFormat *fmt, Uint32 flags);

/*
 * This performs a fast blit from the source surface to the destination
 * surface.  It assumes that the source and destination rectangles are
 * the same size.  If either 'srcrect' or 'dstrect' are NULL, the entire
 * surface (src or dst) is copied.  The final blit rectangles are saved
 * in 'srcrect' and 'dstrect' after all clipping is performed.
 * If the blit is successful, it returns 0, otherwise it returns -1.
 *
 * The blit function should not be called on a locked surface.
 *
 * The blit semantics for surfaces with and without alpha and colorkey
 * are defined as follows:
 *
 * RGBA->RGB:
 *     SDL_SRCALPHA set:
 * 	alpha-blend (using alpha-channel).
 * 	SDL_SRCCOLORKEY ignored.
 *     SDL_SRCALPHA not set:
 * 	copy RGB.
 * 	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 * 	RGB values of the source colour key, ignoring alpha in the
 * 	comparison.
 * 
 * RGB->RGBA:
 *     SDL_SRCALPHA set:
 * 	alpha-blend (using the source per-surface alpha value);
 * 	set destination alpha to opaque.
 *     SDL_SRCALPHA not set:
 * 	copy RGB, set destination alpha to source per-surface alpha value.
 *     both:
 * 	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 * 	source colour key.
 * 
 * RGBA->RGBA:
 *     SDL_SRCALPHA set:
 * 	alpha-blend (using the source alpha channel) the RGB values;
 * 	leave destination alpha untouched. [Note: is this correct?]
 * 	SDL_SRCCOLORKEY ignored.
 *     SDL_SRCALPHA not set:
 * 	copy all of RGBA to the destination.
 * 	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 * 	RGB values of the source colour key, ignoring alpha in the
 * 	comparison.
 * 
 * RGB->RGB: 
 *     SDL_SRCALPHA set:
 * 	alpha-blend (using the source per-surface alpha value).
 *     SDL_SRCALPHA not set:
 * 	copy RGB.
 *     both:
 * 	if SDL_SRCCOLORKEY set, only copy the pixels matching the
 * 	source colour key.
 *
 * If either of the surfaces were in video memory, and the blit returns -2,
 * the video memory was lost, so it should be reloaded with artwork and 
 * re-blitted:
	while ( SDL_BlitSurface(image, imgrect, screen, dstrect) == -2 ) {
		while ( SDL_LockSurface(image) < 0 )
			Sleep(10);
		-- Write image pixels to image->pixels --
		SDL_UnlockSurface(image);
	}
 * This happens under DirectX 5.0 when the system switches away from your
 * fullscreen application.  The lock will also fail until you have access
 * to the video memory again.
 */
/* You should call SDL_BlitSurface() unless you know exactly how SDL
   blitting works internally and how to use the other blit functions.
*/
#define SDL_BlitSurface SDL_UpperBlit

/* This is the public blit function, SDL_BlitSurface(), and it performs
   rectangle validation and clipping before passing it to SDL_LowerBlit()
*/
extern DECLSPEC int SDLCALL SDL_UpperBlit
			(SDL_Surface *src, SDL_Rect *srcrect,
			 SDL_Surface *dst, SDL_Rect *dstrect);
/* This is a semi-private blit function and it performs low-level surface
   blitting only.
*/
extern DECLSPEC int SDLCALL SDL_LowerBlit
			(SDL_Surface *src, SDL_Rect *srcrect,
			 SDL_Surface *dst, SDL_Rect *dstrect);

/*
 * This function performs a fast fill of the given rectangle with 'color'
 * The given rectangle is clipped to the destination surface clip area
 * and the final fill rectangle is saved in the passed in pointer.
 * If 'dstrect' is NULL, the whole surface will be filled with 'color'
 * The color should be a pixel of the format used by the surface, and 
 * can be generated by the SDL_MapRGB() function.
 * This function returns 0 on success, or -1 on error.
 */
extern DECLSPEC int SDLCALL SDL_FillRect
		(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color);

/* 
 * This function takes a surface and copies it to a new surface of the
 * pixel format and colors of the video framebuffer, suitable for fast
 * blitting onto the display surface.  It calls SDL_ConvertSurface()
 *
 * If you want to take advantage of hardware colorkey or alpha blit
 * acceleration, you should set the colorkey and alpha value before
 * calling this function.
 *
 * If the conversion fails or runs out of memory, it returns NULL
 */
extern DECLSPEC SDL_Surface * SDLCALL SDL_DisplayFormat(SDL_Surface *surface);

/* 
 * This function takes a surface and copies it to a new surface of the
 * pixel format and colors of the video framebuffer (if possible),
 * suitable for fast alpha blitting onto the display surface.
 * The new surface will always have an alpha channel.
 *
 * If you want to take advantage of hardware colorkey or alpha blit
 * acceleration, you should set the colorkey and alpha value before
 * calling this function.
 *
 * If the conversion fails or runs out of memory, it returns NULL
 */
extern DECLSPEC SDL_Surface * SDLCALL SDL_DisplayFormatAlpha(SDL_Surface *surface);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* YUV video surface overlay functions                                       */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* This function creates a video output overlay
   Calling the returned surface an overlay is something of a misnomer because
   the contents of the display surface underneath the area where the overlay
   is shown is undefined - it may be overwritten with the converted YUV data.
*/
extern DECLSPEC SDL_Overlay * SDLCALL SDL_CreateYUVOverlay(int width, int height,
				Uint32 format, SDL_Surface *display);

/* Lock an overlay for direct access, and unlock it when you are done */
extern DECLSPEC int SDLCALL SDL_LockYUVOverlay(SDL_Overlay *overlay);
extern DECLSPEC void SDLCALL SDL_UnlockYUVOverlay(SDL_Overlay *overlay);

/* Blit a video overlay to the display surface.
   The contents of the video surface underneath the blit destination are
   not defined.  
   The width and height of the destination rectangle may be different from
   that of the overlay, but currently only 2x scaling is supported.
*/
extern DECLSPEC int SDLCALL SDL_DisplayYUVOverlay(SDL_Overlay *overlay, SDL_Rect *dstrect);

/* Free a video overlay */
extern DECLSPEC void SDLCALL SDL_FreeYUVOverlay(SDL_Overlay *overlay);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* OpenGL support functions.                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Dynamically load a GL driver, if SDL is built with dynamic GL.
 *
 * SDL links normally with the OpenGL library on your system by default,
 * but you can compile it to dynamically load the GL driver at runtime.
 * If you do this, you need to retrieve all of the GL functions used in
 * your program from the dynamic library using SDL_GL_GetProcAddress().
 *
 * This is disabled in default builds of SDL.
 */
extern DECLSPEC int SDLCALL SDL_GL_LoadLibrary(const char *path);

/*
 * Get the address of a GL function (for extension functions)
 */
extern DECLSPEC void * SDLCALL SDL_GL_GetProcAddress(const char* proc);

/*
 * Set an attribute of the OpenGL subsystem before intialization.
 */
extern DECLSPEC int SDLCALL SDL_GL_SetAttribute(SDL_GLattr attr, int value);

/*
 * Get an attribute of the OpenGL subsystem from the windowing
 * interface, such as glX. This is of course different from getting
 * the values from SDL's internal OpenGL subsystem, which only
 * stores the values you request before initialization.
 *
 * Developers should track the values they pass into SDL_GL_SetAttribute
 * themselves if they want to retrieve these values.
 */
extern DECLSPEC int SDLCALL SDL_GL_GetAttribute(SDL_GLattr attr, int* value);

/*
 * Swap the OpenGL buffers, if double-buffering is supported.
 */
extern DECLSPEC void SDLCALL SDL_GL_SwapBuffers(void);

/*
 * Internal functions that should not be called unless you have read
 * and understood the source code for these functions.
 */
extern DECLSPEC void SDLCALL SDL_GL_UpdateRects(int numrects, SDL_Rect* rects);
extern DECLSPEC void SDLCALL SDL_GL_Lock(void);
extern DECLSPEC void SDLCALL SDL_GL_Unlock(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* These functions allow interaction with the window manager, if any.        */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Sets/Gets the title and icon text of the display window
 */
extern DECLSPEC void SDLCALL SDL_WM_SetCaption(const char *title, const char *icon);
extern DECLSPEC void SDLCALL SDL_WM_GetCaption(char **title, char **icon);

/*
 * Sets the icon for the display window.
 * This function must be called before the first call to SDL_SetVideoMode().
 * It takes an icon surface, and a mask in MSB format.
 * If 'mask' is NULL, the entire icon surface will be used as the icon.
 */
extern DECLSPEC void SDLCALL SDL_WM_SetIcon(SDL_Surface *icon, Uint8 *mask);

/*
 * This function iconifies the window, and returns 1 if it succeeded.
 * If the function succeeds, it generates an SDL_APPACTIVE loss event.
 * This function is a noop and returns 0 in non-windowed environments.
 */
extern DECLSPEC int SDLCALL SDL_WM_IconifyWindow(void);

/*
 * Toggle fullscreen mode without changing the contents of the screen.
 * If the display surface does not require locking before accessing
 * the pixel information, then the memory pointers will not change.
 *
 * If this function was able to toggle fullscreen mode (change from 
 * running in a window to fullscreen, or vice-versa), it will return 1.
 * If it is not implemented, or fails, it returns 0.
 *
 * The next call to SDL_SetVideoMode() will set the mode fullscreen
 * attribute based on the flags parameter - if SDL_FULLSCREEN is not
 * set, then the display will be windowed by default where supported.
 *
 * This is currently only implemented in the X11 video driver.
 */
extern DECLSPEC int SDLCALL SDL_WM_ToggleFullScreen(SDL_Surface *surface);

/*
 * This function allows you to set and query the input grab state of
 * the application.  It returns the new input grab state.
 */
typedef enum {
	SDL_GRAB_QUERY = -1,
	SDL_GRAB_OFF = 0,
	SDL_GRAB_ON = 1,
	SDL_GRAB_FULLSCREEN	/* Used internally */
} SDL_GrabMode;
/*
 * Grabbing means that the mouse is confined to the application window,
 * and nearly all keyboard input is passed directly to the application,
 * and not interpreted by a window manager, if any.
 */
extern DECLSPEC SDL_GrabMode SDLCALL SDL_WM_GrabInput(SDL_GrabMode mode);

/* Not in public API at the moment - do not use! */
extern DECLSPEC int SDLCALL SDL_SoftStretch(SDL_Surface *src, SDL_Rect *srcrect,
                                    SDL_Surface *dst, SDL_Rect *dstrect);
                    
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>>>end:../include/close_code.h


#endif /* _SDL_video_h */
#pragma mark >>>>end:../include/SDL_video.h


#pragma mark >>>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct WMcursor WMcursor;	/* Implementation dependent */
typedef struct SDL_Cursor {
	SDL_Rect area;			/* The area of the mouse cursor */
	Sint16 hot_x, hot_y;		/* The "tip" of the cursor */
	Uint8 *data;			/* B/W cursor data */
	Uint8 *mask;			/* B/W cursor mask */
	Uint8 *save[2];			/* Place to save cursor area */
	WMcursor *wm_cursor;		/* Window-manager cursor */
} SDL_Cursor;

/* Function prototypes */
/*
 * Retrieve the current state of the mouse.
 * The current button state is returned as a button bitmask, which can
 * be tested using the SDL_BUTTON(X) macros, and x and y are set to the
 * current mouse cursor position.  You can pass NULL for either x or y.
 */
extern DECLSPEC Uint8 SDLCALL SDL_GetMouseState(int *x, int *y);

/*
 * Retrieve the current state of the mouse.
 * The current button state is returned as a button bitmask, which can
 * be tested using the SDL_BUTTON(X) macros, and x and y are set to the
 * mouse deltas since the last call to SDL_GetRelativeMouseState().
 */
extern DECLSPEC Uint8 SDLCALL SDL_GetRelativeMouseState(int *x, int *y);

/*
 * Set the position of the mouse cursor (generates a mouse motion event)
 */
extern DECLSPEC void SDLCALL SDL_WarpMouse(Uint16 x, Uint16 y);

/*
 * Create a cursor using the specified data and mask (in MSB format).
 * The cursor width must be a multiple of 8 bits.
 *
 * The cursor is created in black and white according to the following:
 * data  mask    resulting pixel on screen
 *  0     1       White
 *  1     1       Black
 *  0     0       Transparent
 *  1     0       Inverted color if possible, black if not.
 *
 * Cursors created with this function must be freed with SDL_FreeCursor().
 */
extern DECLSPEC SDL_Cursor * SDLCALL SDL_CreateCursor
		(Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y);

/*
 * Set the currently active cursor to the specified one.
 * If the cursor is currently visible, the change will be immediately 
 * represented on the display.
 */
extern DECLSPEC void SDLCALL SDL_SetCursor(SDL_Cursor *cursor);

/*
 * Returns the currently active cursor.
 */
extern DECLSPEC SDL_Cursor * SDLCALL SDL_GetCursor(void);

/*
 * Deallocates a cursor created with SDL_CreateCursor().
 */
extern DECLSPEC void SDLCALL SDL_FreeCursor(SDL_Cursor *cursor);

/*
 * Toggle whether or not the cursor is shown on the screen.
 * The cursor start off displayed, but can be turned off.
 * SDL_ShowCursor() returns 1 if the cursor was being displayed
 * before the call, or 0 if it was not.  You can query the current
 * state by passing a 'toggle' value of -1.
 */
extern DECLSPEC int SDLCALL SDL_ShowCursor(int toggle);

/* Used as a mask when testing buttons in buttonstate
   Button 1:	Left mouse button
   Button 2:	Middle mouse button
   Button 3:	Right mouse button
   Button 4:	Mouse wheel up	 (may also be a real button)
   Button 5:	Mouse wheel down (may also be a real button)
 */
#define SDL_BUTTON(X)		(SDL_PRESSED << ((X)-1))
#define SDL_BUTTON_LEFT		1
#define SDL_BUTTON_MIDDLE	2
#define SDL_BUTTON_RIGHT	3
#define SDL_BUTTON_WHEELUP	4
#define SDL_BUTTON_WHEELDOWN	5
#define SDL_BUTTON_LMASK	SDL_BUTTON(SDL_BUTTON_LEFT)
#define SDL_BUTTON_MMASK	SDL_BUTTON(SDL_BUTTON_MIDDLE)
#define SDL_BUTTON_RMASK	SDL_BUTTON(SDL_BUTTON_RIGHT)


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>>end:../include/close_code.h


#endif /* _SDL_mouse_h */
#pragma mark >>>end:../include/SDL_mouse.h

#pragma mark >>>begin:../include/SDL_quit.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* Include file for SDL quit event handling */

#ifndef _SDL_quit_h
#define _SDL_quit_h

/* 
  An SDL_QUITEVENT is generated when the user tries to close the application
  window.  If it is ignored or filtered out, the window will remain open.
  If it is not ignored or filtered, it is queued normally and the window
  is allowed to close.  When the window is closed, screen updates will 
  complete, but have no effect.

  SDL_Init() installs signal handlers for SIGINT (keyboard interrupt)
  and SIGTERM (system termination request), if handlers do not already
  exist, that generate SDL_QUITEVENT events as well.  There is no way
  to determine the cause of an SDL_QUITEVENT, but setting a signal
  handler in your application will override the default generation of
  quit events for that signal.
*/

/* There are no functions directly affecting the quit event */
#define SDL_QuitRequested() \
        (SDL_PumpEvents(), SDL_PeepEvents(NULL,0,SDL_PEEKEVENT,SDL_QUITMASK))

#endif /* _SDL_quit_h */
#pragma mark >>>end:../include/SDL_quit.h


#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Event enumerations */
enum { SDL_NOEVENT = 0,			/* Unused (do not remove) */
       SDL_ACTIVEEVENT,			/* Application loses/gains visibility */
       SDL_KEYDOWN,			/* Keys pressed */
       SDL_KEYUP,			/* Keys released */
       SDL_MOUSEMOTION,			/* Mouse moved */
       SDL_MOUSEBUTTONDOWN,		/* Mouse button pressed */
       SDL_MOUSEBUTTONUP,		/* Mouse button released */
       SDL_JOYAXISMOTION,		/* Joystick axis motion */
       SDL_JOYBALLMOTION,		/* Joystick trackball motion */
       SDL_JOYHATMOTION,		/* Joystick hat position change */
       SDL_JOYBUTTONDOWN,		/* Joystick button pressed */
       SDL_JOYBUTTONUP,			/* Joystick button released */
       SDL_QUIT,			/* User-requested quit */
       SDL_SYSWMEVENT,			/* System specific event */
       SDL_EVENT_RESERVEDA,		/* Reserved for future use.. */
       SDL_EVENT_RESERVEDB,		/* Reserved for future use.. */
       SDL_VIDEORESIZE,			/* User resized video mode */
       SDL_VIDEOEXPOSE,			/* Screen needs to be redrawn */
       SDL_EVENT_RESERVED2,		/* Reserved for future use.. */
       SDL_EVENT_RESERVED3,		/* Reserved for future use.. */
       SDL_EVENT_RESERVED4,		/* Reserved for future use.. */
       SDL_EVENT_RESERVED5,		/* Reserved for future use.. */
       SDL_EVENT_RESERVED6,		/* Reserved for future use.. */
       SDL_EVENT_RESERVED7,		/* Reserved for future use.. */
       /* Events SDL_USEREVENT through SDL_MAXEVENTS-1 are for your use */
       SDL_USEREVENT = 24,
       /* This last event is only for bounding internal arrays
	  It is the number of bits in the event mask datatype -- Uint32
        */
       SDL_NUMEVENTS = 32
};

/* Predefined event masks */
#define SDL_EVENTMASK(X)	(1<<(X))
enum {
	SDL_ACTIVEEVENTMASK	= SDL_EVENTMASK(SDL_ACTIVEEVENT),
	SDL_KEYDOWNMASK		= SDL_EVENTMASK(SDL_KEYDOWN),
	SDL_KEYUPMASK		= SDL_EVENTMASK(SDL_KEYUP),
	SDL_MOUSEMOTIONMASK	= SDL_EVENTMASK(SDL_MOUSEMOTION),
	SDL_MOUSEBUTTONDOWNMASK	= SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN),
	SDL_MOUSEBUTTONUPMASK	= SDL_EVENTMASK(SDL_MOUSEBUTTONUP),
	SDL_MOUSEEVENTMASK	= SDL_EVENTMASK(SDL_MOUSEMOTION)|
	                          SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN)|
	                          SDL_EVENTMASK(SDL_MOUSEBUTTONUP),
	SDL_JOYAXISMOTIONMASK	= SDL_EVENTMASK(SDL_JOYAXISMOTION),
	SDL_JOYBALLMOTIONMASK	= SDL_EVENTMASK(SDL_JOYBALLMOTION),
	SDL_JOYHATMOTIONMASK	= SDL_EVENTMASK(SDL_JOYHATMOTION),
	SDL_JOYBUTTONDOWNMASK	= SDL_EVENTMASK(SDL_JOYBUTTONDOWN),
	SDL_JOYBUTTONUPMASK	= SDL_EVENTMASK(SDL_JOYBUTTONUP),
	SDL_JOYEVENTMASK	= SDL_EVENTMASK(SDL_JOYAXISMOTION)|
	                          SDL_EVENTMASK(SDL_JOYBALLMOTION)|
	                          SDL_EVENTMASK(SDL_JOYHATMOTION)|
	                          SDL_EVENTMASK(SDL_JOYBUTTONDOWN)|
	                          SDL_EVENTMASK(SDL_JOYBUTTONUP),
	SDL_VIDEORESIZEMASK	= SDL_EVENTMASK(SDL_VIDEORESIZE),
	SDL_VIDEOEXPOSEMASK	= SDL_EVENTMASK(SDL_VIDEOEXPOSE),
	SDL_QUITMASK		= SDL_EVENTMASK(SDL_QUIT),
	SDL_SYSWMEVENTMASK	= SDL_EVENTMASK(SDL_SYSWMEVENT)
};
#define SDL_ALLEVENTS		0xFFFFFFFF

/* Application visibility event structure */
typedef struct SDL_ActiveEvent {
	Uint8 type;	/* SDL_ACTIVEEVENT */
	Uint8 gain;	/* Whether given states were gained or lost (1/0) */
	Uint8 state;	/* A mask of the focus states */
} SDL_ActiveEvent;

/* Keyboard event structure */
typedef struct SDL_KeyboardEvent {
	Uint8 type;	/* SDL_KEYDOWN or SDL_KEYUP */
	Uint8 which;	/* The keyboard device index */
	Uint8 state;	/* SDL_PRESSED or SDL_RELEASED */
	SDL_keysym keysym;
} SDL_KeyboardEvent;

/* Mouse motion event structure */
typedef struct SDL_MouseMotionEvent {
	Uint8 type;	/* SDL_MOUSEMOTION */
	Uint8 which;	/* The mouse device index */
	Uint8 state;	/* The current button state */
	Uint16 x, y;	/* The X/Y coordinates of the mouse */
	Sint16 xrel;	/* The relative motion in the X direction */
	Sint16 yrel;	/* The relative motion in the Y direction */
} SDL_MouseMotionEvent;

/* Mouse button event structure */
typedef struct SDL_MouseButtonEvent {
	Uint8 type;	/* SDL_MOUSEBUTTONDOWN or SDL_MOUSEBUTTONUP */
	Uint8 which;	/* The mouse device index */
	Uint8 button;	/* The mouse button index */
	Uint8 state;	/* SDL_PRESSED or SDL_RELEASED */
	Uint16 x, y;	/* The X/Y coordinates of the mouse at press time */
} SDL_MouseButtonEvent;

/* Joystick axis motion event structure */
typedef struct SDL_JoyAxisEvent {
	Uint8 type;	/* SDL_JOYAXISMOTION */
	Uint8 which;	/* The joystick device index */
	Uint8 axis;	/* The joystick axis index */
	Sint16 value;	/* The axis value (range: -32768 to 32767) */
} SDL_JoyAxisEvent;

/* Joystick trackball motion event structure */
typedef struct SDL_JoyBallEvent {
	Uint8 type;	/* SDL_JOYBALLMOTION */
	Uint8 which;	/* The joystick device index */
	Uint8 ball;	/* The joystick trackball index */
	Sint16 xrel;	/* The relative motion in the X direction */
	Sint16 yrel;	/* The relative motion in the Y direction */
} SDL_JoyBallEvent;

/* Joystick hat position change event structure */
typedef struct SDL_JoyHatEvent {
	Uint8 type;	/* SDL_JOYHATMOTION */
	Uint8 which;	/* The joystick device index */
	Uint8 hat;	/* The joystick hat index */
	Uint8 value;	/* The hat position value:
			    SDL_HAT_LEFTUP   SDL_HAT_UP       SDL_HAT_RIGHTUP
			    SDL_HAT_LEFT     SDL_HAT_CENTERED SDL_HAT_RIGHT
			    SDL_HAT_LEFTDOWN SDL_HAT_DOWN     SDL_HAT_RIGHTDOWN
			   Note that zero means the POV is centered.
			*/
} SDL_JoyHatEvent;

/* Joystick button event structure */
typedef struct SDL_JoyButtonEvent {
	Uint8 type;	/* SDL_JOYBUTTONDOWN or SDL_JOYBUTTONUP */
	Uint8 which;	/* The joystick device index */
	Uint8 button;	/* The joystick button index */
	Uint8 state;	/* SDL_PRESSED or SDL_RELEASED */
} SDL_JoyButtonEvent;

/* The "window resized" event
   When you get this event, you are responsible for setting a new video
   mode with the new width and height.
 */
typedef struct SDL_ResizeEvent {
	Uint8 type;	/* SDL_VIDEORESIZE */
	int w;		/* New width */
	int h;		/* New height */
} SDL_ResizeEvent;

/* The "screen redraw" event */
typedef struct SDL_ExposeEvent {
	Uint8 type;	/* SDL_VIDEOEXPOSE */
} SDL_ExposeEvent;

/* The "quit requested" event */
typedef struct SDL_QuitEvent {
	Uint8 type;	/* SDL_QUIT */
} SDL_QuitEvent;

/* A user-defined event type */
typedef struct SDL_UserEvent {
	Uint8 type;	/* SDL_USEREVENT through SDL_NUMEVENTS-1 */
	int code;	/* User defined event code */
	void *data1;	/* User defined data pointer */
	void *data2;	/* User defined data pointer */
} SDL_UserEvent;

/* If you want to use this event, you should include SDL_syswm.h */
struct SDL_SysWMmsg;
typedef struct SDL_SysWMmsg SDL_SysWMmsg;
typedef struct SDL_SysWMEvent {
	Uint8 type;
	SDL_SysWMmsg *msg;
} SDL_SysWMEvent;

/* General event structure */
typedef union {
	Uint8 type;
	SDL_ActiveEvent active;
	SDL_KeyboardEvent key;
	SDL_MouseMotionEvent motion;
	SDL_MouseButtonEvent button;
	SDL_JoyAxisEvent jaxis;
	SDL_JoyBallEvent jball;
	SDL_JoyHatEvent jhat;
	SDL_JoyButtonEvent jbutton;
	SDL_ResizeEvent resize;
	SDL_ExposeEvent expose;
	SDL_QuitEvent quit;
	SDL_UserEvent user;
	SDL_SysWMEvent syswm;
} SDL_Event;


/* Function prototypes */

/* Pumps the event loop, gathering events from the input devices.
   This function updates the event queue and internal input device state.
   This should only be run in the thread that sets the video mode.
*/
extern DECLSPEC void SDLCALL SDL_PumpEvents(void);

/* Checks the event queue for messages and optionally returns them.
   If 'action' is SDL_ADDEVENT, up to 'numevents' events will be added to
   the back of the event queue.
   If 'action' is SDL_PEEKEVENT, up to 'numevents' events at the front
   of the event queue, matching 'mask', will be returned and will not
   be removed from the queue.
   If 'action' is SDL_GETEVENT, up to 'numevents' events at the front 
   of the event queue, matching 'mask', will be returned and will be
   removed from the queue.
   This function returns the number of events actually stored, or -1
   if there was an error.  This function is thread-safe.
*/
typedef enum {
	SDL_ADDEVENT,
	SDL_PEEKEVENT,
	SDL_GETEVENT
} SDL_eventaction;
/* */
extern DECLSPEC int SDLCALL SDL_PeepEvents(SDL_Event *events, int numevents,
				SDL_eventaction action, Uint32 mask);

/* Polls for currently pending events, and returns 1 if there are any pending
   events, or 0 if there are none available.  If 'event' is not NULL, the next
   event is removed from the queue and stored in that area.
 */
extern DECLSPEC int SDLCALL SDL_PollEvent(SDL_Event *event);

/* Waits indefinitely for the next available event, returning 1, or 0 if there
   was an error while waiting for events.  If 'event' is not NULL, the next
   event is removed from the queue and stored in that area.
 */
extern DECLSPEC int SDLCALL SDL_WaitEvent(SDL_Event *event);

/* Add an event to the event queue.
   This function returns 0 on success, or -1 if the event queue was full
   or there was some other error.
 */
extern DECLSPEC int SDLCALL SDL_PushEvent(SDL_Event *event);

/*
  This function sets up a filter to process all events before they
  change internal state and are posted to the internal event queue.

  The filter is protypted as:
*/
typedef int (SDLCALL *SDL_EventFilter)(const SDL_Event *event);
/*
  If the filter returns 1, then the event will be added to the internal queue.
  If it returns 0, then the event will be dropped from the queue, but the 
  internal state will still be updated.  This allows selective filtering of
  dynamically arriving events.

  WARNING:  Be very careful of what you do in the event filter function, as 
            it may run in a different thread!

  There is one caveat when dealing with the SDL_QUITEVENT event type.  The
  event filter is only called when the window manager desires to close the
  application window.  If the event filter returns 1, then the window will
  be closed, otherwise the window will remain open if possible.
  If the quit event is generated by an interrupt signal, it will bypass the
  internal queue and be delivered to the application at the next event poll.
*/
extern DECLSPEC void SDLCALL SDL_SetEventFilter(SDL_EventFilter filter);

/*
  Return the current event filter - can be used to "chain" filters.
  If there is no event filter set, this function returns NULL.
*/
extern DECLSPEC SDL_EventFilter SDLCALL SDL_GetEventFilter(void);

/*
  This function allows you to set the state of processing certain events.
  If 'state' is set to SDL_IGNORE, that event will be automatically dropped
  from the event queue and will not event be filtered.
  If 'state' is set to SDL_ENABLE, that event will be processed normally.
  If 'state' is set to SDL_QUERY, SDL_EventState() will return the 
  current processing state of the specified event.
*/
#define SDL_QUERY	-1
#define SDL_IGNORE	 0
#define SDL_DISABLE	 0
#define SDL_ENABLE	 1
extern DECLSPEC Uint8 SDLCALL SDL_EventState(Uint8 type, int state);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h


#endif /* _SDL_events_h */
#pragma mark >>end:../include/SDL_events.h

#pragma mark >>begin:../include/SDL_version.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: util_hid.h,v 1.2 2006/03/20 08:24:23 sds-chuck Exp $";
#endif

/* This header defines the current SDL version */

#ifndef _SDL_version_h
#define _SDL_version_h


#pragma mark >>>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Printable format: "%d.%d.%d", MAJOR, MINOR, PATCHLEVEL
*/
#define SDL_MAJOR_VERSION	1
#define SDL_MINOR_VERSION	2
#define SDL_PATCHLEVEL		9

typedef struct SDL_version {
	Uint8 major;
	Uint8 minor;
	Uint8 patch;
} SDL_version;

/* This macro can be used to fill a version structure with the compile-time
 * version of the SDL library.
 */
#define SDL_VERSION(X)							\
{									\
	(X)->major = SDL_MAJOR_VERSION;					\
	(X)->minor = SDL_MINOR_VERSION;					\
	(X)->patch = SDL_PATCHLEVEL;					\
}

/* This macro turns the version numbers into a numeric value:
   (1,2,3) -> (1203)
   This assumes that there will never be more than 100 patchlevels
*/
#define SDL_VERSIONNUM(X, Y, Z)						\
	((X)*1000 + (Y)*100 + (Z))

/* This is the version number macro for the current SDL version */
#define SDL_COMPILEDVERSION \
	SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL)

/* This macro will evaluate to true if compiled with SDL at least X.Y.Z */
#define SDL_VERSION_ATLEAST(X, Y, Z) \
	(SDL_COMPILEDVERSION >= SDL_VERSIONNUM(X, Y, Z))

/* This function gets the version of the dynamically linked SDL library.
   it should NOT be used to fill a version structure, instead you should
   use the SDL_Version() macro.
 */
extern DECLSPEC const SDL_version * SDLCALL SDL_Linked_Version(void);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>>end:../include/close_code.h


#endif /* _SDL_version_h */
#pragma mark >>end:../include/SDL_version.h


#pragma mark >>begin:../include/begin_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file sets things up for C dynamic library function definitions,
   static inlined functions, and structures aligned at 4-byte alignment.
   If you don't like ugly C preprocessor code, don't look at this file. :)
*/

/* This shouldn't be nested -- included it around code only. */
#ifdef _begin_code_h
#error Nested inclusion of begin_code.h
#endif
#define _begin_code_h

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if defined(WIN32) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__ 
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
   This is necessary if the header is included in code which has structure
   packing set to an alternate value, say for loading structures from disk.
   The packing is reset to the previous value in close_code.h
 */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#pragma pack(push,4)
#elif (defined(__MWERKS__) && defined(macintosh))
#pragma options align=mac68k4byte
#pragma enumsalwaysint on
#endif /* Compiler needs structure packing set */

/* Set up compiler-specific options for inlining functions */
#ifndef SDL_INLINE_OKAY
#ifdef __GNUC__
#define SDL_INLINE_OKAY
#else
/* Add any special compiler-specific cases here */
#if defined(_MSC_VER) || defined(__BORLANDC__) || \
    defined(__DMC__) || defined(__SC__) || \
    defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__DECC)
#ifndef __inline__
#define __inline__	__inline
#endif
#define SDL_INLINE_OKAY
#else
#if !defined(__MRC__) && !defined(_SGI_SOURCE)
#define __inline__ inline
#define SDL_INLINE_OKAY
#endif /* Not a funky compiler */
#endif /* Visual C++ */
#endif /* GNU C */
#endif /* SDL_INLINE_OKAY */

/* If inlining isn't supported, remove "__inline__", turning static
   inlined functions into static functions (resulting in code bloat
   in all files which include the offending header files)
*/
#ifndef SDL_INLINE_OKAY
#define __inline__
#endif

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! MacOS X - breaks precompiled headers */
#pragma mark >>end:../include/begin_code.h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* As of version 0.5, SDL is loaded dynamically into the application */

/* These are the flags which may be passed to SDL_Init() -- you should
   specify the subsystems which you will be using in your application.
*/
#define	SDL_INIT_TIMER		0x00000001
#define SDL_INIT_AUDIO		0x00000010
#define SDL_INIT_VIDEO		0x00000020
#define SDL_INIT_CDROM		0x00000100
#define SDL_INIT_JOYSTICK	0x00000200
#define SDL_INIT_NOPARACHUTE	0x00100000	/* Don't catch fatal signals */
#define SDL_INIT_EVENTTHREAD	0x01000000	/* Not supported on all OS's */
#define SDL_INIT_EVERYTHING	0x0000FFFF

/* This function loads the SDL dynamically linked library and initializes 
 * the subsystems specified by 'flags' (and those satisfying dependencies)
 * Unless the SDL_INIT_NOPARACHUTE flag is set, it will install cleanup
 * signal handlers for some commonly ignored fatal signals (like SIGSEGV)
 */
extern DECLSPEC int SDLCALL SDL_Init(Uint32 flags);

/* This function initializes specific SDL subsystems */
extern DECLSPEC int SDLCALL SDL_InitSubSystem(Uint32 flags);

/* This function cleans up specific SDL subsystems */
extern DECLSPEC void SDLCALL SDL_QuitSubSystem(Uint32 flags);

/* This function returns mask of the specified subsystems which have
   been initialized.
   If 'flags' is 0, it returns a mask of all initialized subsystems.
*/
extern DECLSPEC Uint32 SDLCALL SDL_WasInit(Uint32 flags);

/* This function cleans up all initialized subsystems and unloads the
 * dynamically linked library.  You should call it upon all exit conditions.
 */
extern DECLSPEC void SDLCALL SDL_Quit(void);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#pragma mark >>begin:../include/close_code.h

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* This file reverses the effects of begin_code.h and should be included
   after you finish any function and structure declarations in your headers
*/

#undef _begin_code_h

/* Reset structure packing at previous byte alignment */
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__WATCOMC__)  || defined(__BORLANDC__)
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#if (defined(__MWERKS__) && defined(macintosh))
#pragma options align=reset
#pragma enumsalwaysint reset
#else
#pragma pack(pop)
#endif
#endif /* Compiler needs structure packing set */

#pragma mark >>end:../include/close_code.h


#endif /* _SDL_H */
#pragma mark >end:../include/SDL.h


/* lastly, undo the damage that SDL has done: undefine the main macro */
#undef main

