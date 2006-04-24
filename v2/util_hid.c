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
// file: util_hid.c
// desc: skeleton/joystick SDL support + DIY mouse/keyboard support
//
// author: Spencer Salazar (ssalazar@cs.princeton.edu)
//         Ge Wang (gewang@cs.princeton.edu)
// date: spring 2006
//-----------------------------------------------------------------------------
#include "chuck_def.h"

#define DISABLE_THREADS
#define DISABLE_AUDIO
#define DISABLE_CDROM
#define DISABLE_TIMERS
#define DISABLE_VIDEO
#define SDL_USE_PTHREADS

#ifdef __CK_SDL_NATIVE__
#include "util_hid.h"
#else /* __CK_SDL_NATIVE__ */
/* use the customized SDL joystick support utilities */

#ifdef __WINDOWS_DS__

#define DIRECTINPUT_VERSION 0x0300

/* WinUser.h won't include rawinput stuff without this... */

#if (_WIN32_WINNT < 0x0501) && defined( __WINDOWS_RAWINPUT__ )
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#endif /* __WINDOWS_DS__ */

// except as noted, everything below is straight from SDL

#pragma mark >begin:../include/SDL_types.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
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
#pragma mark >end:../include/SDL_types.h

#pragma mark >begin:SDL_fatal.c

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* General fatal signal handling code for SDL */

#ifdef NO_SIGNAL_H

/* No signals on this platform, nothing to do.. */

void SDL_InstallParachute(void)
{
	return;
}

void SDL_UninstallParachute(void)
{
	return;
}

#else

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#pragma mark >>begin:../include/SDL.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Main include header for the SDL library */

#ifndef _SDL_H
#define _SDL_H

#pragma mark >>>begin:../include/SDL_main.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
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
#pragma mark >>>>begin:../include/SDL_types.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
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
#pragma mark >>>>end:../include/SDL_types.h

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

#endif

/* From the SDL library code -- needed for registering QuickDraw on MacOS */
#if defined(macintosh)
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

#endif

#endif /* Need to redefine main()? */

#endif /* _SDL_main_h */
#pragma mark >>>end:../include/SDL_main.h

#pragma mark >>>begin:../include/SDL_getenv.h


/* Not all environments have a working getenv()/putenv() */

#if defined(macintosh) || defined(_WIN32_WCE)
#define NEED_SDL_GETENV
#endif

#ifdef NEED_SDL_GETENV

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


#endif /* NEED_GETENV */
#pragma mark >>>end:../include/SDL_getenv.h

#pragma mark >>>begin:../include/SDL_error.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Simple error message routines for SDL */

#ifndef _SDL_error_h
#define _SDL_error_h

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


#endif /* _SDL_error_h */
#pragma mark >>>end:../include/SDL_error.h

#pragma mark >>>begin:../include/SDL_rwops.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* This file provides a general interface for SDL to read and write
   data sources.  It can easily be extended to files, memory, etc.
*/

#ifndef _SDL_RWops_h
#define _SDL_RWops_h

#include <stdio.h>


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


#endif /* _SDL_RWops_h */
#pragma mark >>>end:../include/SDL_rwops.h

#pragma mark >>>begin:../include/SDL_timer.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

#ifndef _SDL_timer_h
#define _SDL_timer_h

/* Header for the SDL time management routines */


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


#endif /* _SDL_timer_h */
#pragma mark >>>end:../include/SDL_timer.h

#pragma mark >>>begin:../include/SDL_audio.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Access to the raw audio mixing buffer for the SDL library */

#ifndef _SDL_audio_h
#define _SDL_audio_h

#include <stdio.h>

#pragma mark >>>>begin:../include/SDL_byteorder.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
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
#pragma mark >>>>end:../include/SDL_byteorder.h


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


#endif /* _SDL_audio_h */
#pragma mark >>>end:../include/SDL_audio.h

#pragma mark >>>begin:../include/SDL_cdrom.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* This is the CD-audio control API for Simple DirectMedia Layer */

#ifndef _SDL_cdrom_h
#define _SDL_cdrom_h


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


#endif /* _SDL_video_h */
#pragma mark >>>end:../include/SDL_cdrom.h

#pragma mark >>>begin:../include/SDL_joystick.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Include file for SDL joystick event handling */

#ifndef _SDL_joystick_h
#define _SDL_joystick_h


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


#endif /* _SDL_joystick_h */
#pragma mark >>>end:../include/SDL_joystick.h

#pragma mark >>>begin:../include/SDL_events.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Include file for SDL event handling */

#ifndef _SDL_events_h
#define _SDL_events_h

#pragma mark >>>>begin:../include/SDL_active.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Include file for SDL application focus event handling */

#ifndef _SDL_active_h
#define _SDL_active_h

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


#endif /* _SDL_active_h */
#pragma mark >>>>end:../include/SDL_active.h

#pragma mark >>>>begin:../include/SDL_keyboard.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Include file for SDL keyboard event handling */

#ifndef _SDL_keyboard_h
#define _SDL_keyboard_h

#pragma mark >>>>>begin:../include/SDL_keysym.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
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
#pragma mark >>>>>end:../include/SDL_keysym.h


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


#endif /* _SDL_keyboard_h */
#pragma mark >>>>end:../include/SDL_keyboard.h

#pragma mark >>>>begin:../include/SDL_mouse.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Include file for SDL mouse event handling */

#ifndef _SDL_mouse_h
#define _SDL_mouse_h

#pragma mark >>>>>begin:../include/SDL_video.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Header file for access to the SDL raw framebuffer window */

#ifndef _SDL_video_h
#define _SDL_video_h

#include <stdio.h>

#pragma mark >>>>>>begin:../include/SDL_mutex.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

#ifndef _SDL_mutex_h
#define _SDL_mutex_h

/* Functions to provide thread synchronization primitives

	These are independent of the other SDL routines.
*/


#pragma mark >>>>>>>begin:../include/begin_code.h

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
#pragma mark >>>>>>>end:../include/begin_code.h

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
#pragma mark >>>>>>>begin:../include/close_code.h

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

#pragma mark >>>>>>>end:../include/close_code.h


#endif /* _SDL_mutex_h */
#pragma mark >>>>>>end:../include/SDL_mutex.h


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


#endif /* _SDL_video_h */
#pragma mark >>>>>end:../include/SDL_video.h


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
 * current 
 cursor position.  You can pass NULL for either x or y.
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


#endif /* _SDL_mouse_h */
#pragma mark >>>>end:../include/SDL_mouse.h

#pragma mark >>>>begin:../include/SDL_quit.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
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
#pragma mark >>>>end:../include/SDL_quit.h


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


#endif /* _SDL_events_h */
#pragma mark >>>end:../include/SDL_events.h

#pragma mark >>>begin:../include/SDL_version.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* This header defines the current SDL version */

#ifndef _SDL_version_h
#define _SDL_version_h


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


#endif /* _SDL_version_h */
#pragma mark >>>end:../include/SDL_version.h


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


#endif /* _SDL_H */
#pragma mark >>end:../include/SDL.h

#pragma mark >>begin:./SDL_fatal.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* General fatal signal handling code for SDL */

extern void SDL_InstallParachute(void);
extern void SDL_UninstallParachute(void);

#pragma mark >>end:./SDL_fatal.h


#ifdef __CYGWIN__
#define DISABLE_STDIO
#endif

/* This installs some signal handlers for the more common fatal signals,
   so that if the programmer is lazy, the app doesn't die so horribly if
   the program crashes.
*/

static void print_msg(const char *text)
{
#ifndef DISABLE_STDIO
	fprintf(stderr, "%s", text);
#endif
}

static void SDL_Parachute(int sig)
{
	signal(sig, SIG_DFL);
	print_msg("Fatal signal: ");
	switch (sig) {
		case SIGSEGV:
			print_msg("Segmentation Fault");
			break;
#ifdef SIGBUS
#if SIGBUS != SIGSEGV
		case SIGBUS:
			print_msg("Bus Error");
			break;
#endif
#endif /* SIGBUS */
#ifdef SIGFPE
		case SIGFPE:
			print_msg("Floating Point Exception");
			break;
#endif /* SIGFPE */
#ifdef SIGQUIT
		case SIGQUIT:
			print_msg("Keyboard Quit");
			break;
#endif /* SIGQUIT */
#ifdef SIGPIPE
		case SIGPIPE:
			print_msg("Broken Pipe");
			break;
#endif /* SIGPIPE */
		default:
#ifndef DISABLE_STDIO
			fprintf(stderr, "# %d", sig);
#endif
			break;
	}
	print_msg(" (SDL Parachute Deployed)\n");
	SDL_Quit();
	exit(-sig);
}

static int SDL_fatal_signals[] = {
	SIGSEGV,
#ifdef SIGBUS
	SIGBUS,
#endif
#ifdef SIGFPE
	SIGFPE,
#endif
#ifdef SIGQUIT
	SIGQUIT,
#endif
	0
};

void SDL_InstallParachute(void)
{
	/* Set a handler for any fatal signal not already handled */
	int i;
#ifdef HAVE_SIGACTION
	struct sigaction action;

	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		sigaction(SDL_fatal_signals[i], NULL, &action);
		if ( action.sa_handler == SIG_DFL ) {
			action.sa_handler = SDL_Parachute;
			sigaction(SDL_fatal_signals[i], &action, NULL);
		}
	}
#ifdef SIGALRM
	/* Set SIGALRM to be ignored -- necessary on Solaris */
	sigaction(SIGALRM, NULL, &action);
	if ( action.sa_handler == SIG_DFL ) {
		action.sa_handler = SIG_IGN;
		sigaction(SIGALRM, &action, NULL);
	}
#endif
#else
	void (*ohandler)(int);

	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		ohandler = signal(SDL_fatal_signals[i], SDL_Parachute);
		if ( ohandler != SIG_DFL ) {
			signal(SDL_fatal_signals[i], ohandler);
		}
	}
#endif /* HAVE_SIGACTION */
	return;
}

void SDL_UninstallParachute(void)
{
	/* Remove a handler for any fatal signal handled */
	int i;
#ifdef HAVE_SIGACTION
	struct sigaction action;

	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		sigaction(SDL_fatal_signals[i], NULL, &action);
		if ( action.sa_handler == SDL_Parachute ) {
			action.sa_handler = SIG_DFL;
			sigaction(SDL_fatal_signals[i], &action, NULL);
		}
	}
#else
	void (*ohandler)(int);

	for ( i=0; SDL_fatal_signals[i]; ++i ) {
		ohandler = signal(SDL_fatal_signals[i], SIG_DFL);
		if ( ohandler != SDL_Parachute ) {
			signal(SDL_fatal_signals[i], ohandler);
		}
	}
#endif /* HAVE_SIGACTION */
}

#endif /* NO_SIGNAL_H */
#pragma mark >end:SDL_fatal.c

#pragma mark >begin:SDL_error.c

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Simple error handling in SDL */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#pragma mark >>begin:./SDL_error_c.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* This file defines a structure that carries language-independent
   error messages
*/

#ifndef _SDL_error_c_h
#define _SDL_error_c_h

#define ERR_MAX_STRLEN	128
#define ERR_MAX_ARGS	5

typedef struct SDL_error {
	/* This is a numeric value corresponding to the current error */
	int error;

	/* This is a key used to index into a language hashtable containing
	   internationalized versions of the SDL error messages.  If the key
	   is not in the hashtable, or no hashtable is available, the key is
	   used directly as an error message format string.
	*/
	unsigned char key[ERR_MAX_STRLEN];

	/* These are the arguments for the error functions */
	int argc;
	union {
		void *value_ptr;
#if 0	/* What is a character anyway?  (UNICODE issues) */
		unsigned char value_c;
#endif
		int value_i;
		double value_f;
		unsigned char buf[ERR_MAX_STRLEN];
	} args[ERR_MAX_ARGS];
} SDL_error;

#endif /* _SDL_error_c_h */
#pragma mark >>end:./SDL_error_c.h

#ifndef DISABLE_THREADS
#pragma mark >>begin:thread/SDL_thread_c.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

#ifndef _SDL_thread_c_h
#define _SDL_thread_c_h

#if !defined( __WINDOWS_DS__ )
#pragma mark >>>begin:thread/linux/SDL_systhread_c.h

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

#ifdef SDL_USE_PTHREADS

#include <pthread.h>

typedef pthread_t SYS_ThreadHandle;

#else

#include <sys/types.h>

typedef pid_t SYS_ThreadHandle;

#endif /* SDL_USE_PTHREADS */
#pragma mark >>>end:thread/linux/SDL_systhread_c.h

#else /* !defined( __WINDOWS_DS__ ) */

#pragma mark >>>begin:thread/win32/SDL_systhread_c.h
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

#include <windows.h>

typedef HANDLE SYS_ThreadHandle;

#pragma mark >>>end:thread/win32/SDL_systhread_c.h

#endif /* !defined( __WINDOWS_DS__ ) */

/* This is the system-independent thread info structure */
struct SDL_Thread {
	Uint32 threadid;
	SYS_ThreadHandle handle;
	int status;
	SDL_error errbuf;
	void *data;
};

/* This is the function called to run a thread */
extern void SDL_RunThread(void *data);

/* Routine to get the thread-specific error variable */
extern SDL_error *SDL_GetErrBuf(void);

#endif /* _SDL_thread_c_h */
#pragma mark >>end:thread/SDL_thread_c.h

#endif

#ifdef DISABLE_THREADS
/* The default (non-thread-safe) global error variable */
static SDL_error SDL_global_error;

#define SDL_GetErrBuf()	(&SDL_global_error)
#endif /* DISABLE_THREADS */

#ifdef __CYGWIN__
#define DISABLE_STDIO
#endif

#define SDL_ERRBUFIZE	1024

/* Private functions */

static void SDL_LookupString(const Uint8 *key, Uint16 *buf, int buflen)
{
	/* FIXME: Add code to lookup key in language string hash-table */

	/* Key not found in language string hash-table */
	while ( *key && (--buflen > 0) ) {
		*buf++ = *key++;
	}
	*buf = 0;	/* NULL terminate string */
}

/* Public functions */

void SDL_SetError (const char *fmt, ...)
{
	va_list ap;
	SDL_error *error;

	/* Copy in the key, mark error as valid */
	error = SDL_GetErrBuf();
	error->error = 1;
	strncpy((char *)error->key, fmt, sizeof(error->key));
	error->key[sizeof(error->key)-1] = '\0';

	va_start(ap, fmt);
	error->argc = 0;
	while ( *fmt ) {
		if ( *fmt++ == '%' ) {
			switch (*fmt++) {
			    case 0:  /* Malformed format string.. */
				--fmt;
				break;
#if 0	/* What is a character anyway?  (UNICODE issues) */
			    case 'c':
				error->args[error->argc++].value_c =
						va_arg(ap, unsigned char);
				break;
#endif
			    case 'd':
				error->args[error->argc++].value_i =
							va_arg(ap, int);
				break;
			    case 'f':
				error->args[error->argc++].value_f =
							va_arg(ap, double);
				break;
			    case 'p':
				error->args[error->argc++].value_ptr =
							va_arg(ap, void *);
				break;
			    case 's':
				{
				  int index = error->argc;
				  strncpy((char *)error->args[index].buf,
					va_arg(ap, char *), ERR_MAX_STRLEN);
				  error->args[index].buf[ERR_MAX_STRLEN-1] = 0;
				  error->argc++;
				}
				break;
			    default:
				break;
			}
			if ( error->argc >= ERR_MAX_ARGS ) {
				break;
			}
		}
	}
	va_end(ap);

#ifndef DISABLE_STDIO
	/* If we are in debug mode, print out an error message */
#ifdef DEBUG_ERROR
	fprintf(stderr, "SDL_SetError: %s\n", SDL_GetError());
#else
	if ( getenv("SDL_DEBUG") ) {
		fprintf(stderr, "SDL_SetError: %s\n", SDL_GetError());
	}
#endif
#endif /* !DISABLE_STDIO */
}

/* Print out an integer value to a UNICODE buffer */
static int PrintInt(Uint16 *str, unsigned int maxlen, int value)
{
	char tmp[128];
	int len, i;

	sprintf(tmp, "%d", value);
	len = 0;
	if ( strlen(tmp) < maxlen ) {
		for ( i=0; tmp[i]; ++i ) {
			*str++ = tmp[i];
			++len;
		}
	}
	return(len);
}
/* Print out a double value to a UNICODE buffer */
static int PrintDouble(Uint16 *str, unsigned int maxlen, double value)
{
	char tmp[128];
	int len, i;

	sprintf(tmp, "%f", value);
	len = 0;
	if ( strlen(tmp) < maxlen ) {
		for ( i=0; tmp[i]; ++i ) {
			*str++ = tmp[i];
			++len;
		}
	}
	return(len);
}
/* Print out a pointer value to a UNICODE buffer */
static int PrintPointer(Uint16 *str, unsigned int maxlen, void *value)
{
	char tmp[128];
	int len, i;

	sprintf(tmp, "%p", value);
	len = 0;
	if ( strlen(tmp) < maxlen ) {
		for ( i=0; tmp[i]; ++i ) {
			*str++ = tmp[i];
			++len;
		}
	}
	return(len);
}

/* This function has a bit more overhead than most error functions
   so that it supports internationalization and thread-safe errors.
*/
Uint16 *SDL_GetErrorMsgUNICODE(Uint16 *errstr, unsigned int maxlen)
{
	SDL_error *error;

	/* Clear the error string */
	*errstr = 0; --maxlen;

	/* Get the thread-safe error, and print it out */
	error = SDL_GetErrBuf();
	if ( error->error ) {
		Uint16 translated[ERR_MAX_STRLEN], *fmt, *msg;
		int len;
		int argi;

		/* Print out the UNICODE error message */
		SDL_LookupString(error->key, translated, sizeof(translated));
		msg = errstr;
		argi = 0;
		for ( fmt=translated; *fmt && (maxlen > 0); ) {
			if ( *fmt == '%' ) {
				switch (fmt[1]) {
				    case 'S':	/* Special SKIP operand */
					argi += (fmt[2] - '0');
					++fmt;
					break;
				    case '%':
					*msg++ = '%';
					maxlen -= 1;
					break;
#if 0	/* What is a character anyway?  (UNICODE issues) */
				    case 'c':
                                        *msg++ = (unsigned char)
					         error->args[argi++].value_c;
					maxlen -= 1;
					break;
#endif
				    case 'd':
					len = PrintInt(msg, maxlen,
						error->args[argi++].value_i);
					msg += len;
					maxlen -= len;
					break;
				    case 'f':
					len = PrintDouble(msg, maxlen,
						error->args[argi++].value_f);
					msg += len;
					maxlen -= len;
					break;
				    case 'p':
					len = PrintPointer(msg, maxlen,
						error->args[argi++].value_ptr);
					msg += len;
					maxlen -= len;
					break;
				    case 's': /* UNICODE string */
					{ Uint16 buf[ERR_MAX_STRLEN], *str;
					  SDL_LookupString(error->args[argi++].buf, buf, sizeof(buf));
					  str = buf;
					  while ( *str && (maxlen > 0) ) {
						*msg++ = *str++;
						maxlen -= 1;
					  }
					}
					break;
				}
				fmt += 2;
			} else {
				*msg++ = *fmt++;
				maxlen -= 1;
			}
		}
		*msg = 0;	/* NULL terminate the string */
	}
	return(errstr);
}

Uint8 *SDL_GetErrorMsg(Uint8 *errstr, unsigned int maxlen)
{
	Uint16 *errstr16;
	unsigned int i;

	/* Allocate the UNICODE buffer */
	errstr16 = (Uint16 *)malloc(maxlen * (sizeof *errstr16));
	if ( ! errstr16 ) {
		strncpy((char *)errstr, "Out of memory", maxlen);
		errstr[maxlen-1] = '\0';
		return(errstr);
	}

	/* Get the error message */
	SDL_GetErrorMsgUNICODE(errstr16, maxlen);

	/* Convert from UNICODE to Latin1 encoding */
	for ( i=0; i<maxlen; ++i ) {
		errstr[i] = (Uint8)errstr16[i];
	}

	/* Free UNICODE buffer (if necessary) */
	free(errstr16);

	return(errstr);
}

/* Available for backwards compatibility */
char *SDL_GetError (void)
{
	static char errmsg[SDL_ERRBUFIZE];

	return((char *)SDL_GetErrorMsg((unsigned char *)errmsg, SDL_ERRBUFIZE));
}

void SDL_ClearError(void)
{
	SDL_error *error;

	error = SDL_GetErrBuf();
	error->error = 0;
}

/* Very common errors go here */
void SDL_Error(SDL_errorcode code)
{
	switch (code) {
		case SDL_ENOMEM:
			SDL_SetError("Out of memory");
			break;
		case SDL_EFREAD:
			SDL_SetError("Error reading from datastream");
			break;
		case SDL_EFWRITE:
			SDL_SetError("Error writing to datastream");
			break;
		case SDL_EFSEEK:
			SDL_SetError("Error seeking in datastream");
			break;
		default:
			SDL_SetError("Unknown SDL error");
			break;
	}
}

#ifdef TEST_ERROR
int main(int argc, char *argv[])
{
	char buffer[BUFSIZ+1];

	SDL_SetError("Hi there!");
	printf("Error 1: %s\n", SDL_GetError());
	SDL_ClearError();
	memset(buffer, '1', BUFSIZ);
	buffer[BUFSIZ] = 0;
	SDL_SetError("This is the error: %s (%f)", buffer, 1.0);
	printf("Error 2: %s\n", SDL_GetError());
	exit(0);
}
#endif
#pragma mark >end:SDL_error.c

#pragma mark >begin:chuck/util_sdl.c

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
// file: util_sdl.c
// desc: skeleton libsdl for joystick support
//
// author: Spencer Salazar (ssalazar@cs.princeton.edu)
// date: spring 2006
//-----------------------------------------------------------------------------

typedef struct XMutex XMutex;
extern XMutex * XMutex_create();
extern void XMutex_acquire( XMutex * );
extern void XMutex_release( XMutex * );
extern void XMutex_free( XMutex * );

#ifndef __PLATFORM_WIN32__
#include <unistd.h>
#else
#include <windows.h>
#endif

/* forward declarations */
extern void Mouse_init();
extern void Mouse_poll();
extern void Mouse_quit();

/* Public data -- the event filter */
/* this is largely unused, since we dont need event filters.  But its here 
because some unused SDL_joystick code accesses it.   */
SDL_EventFilter SDL_EventOK = NULL;
Uint8 SDL_ProcessEvents[SDL_NUMEVENTS];

extern int SDL_JoystickInit();
extern void SDL_JoystickQuit();

/* Private data -- event queue */
#define MAXEVENTS	128
static struct {
	int active;
	int head;
	int tail;
	SDL_Event event[MAXEVENTS];
    XMutex * lock;
} SDL_EventQ;

Uint8 SDL_EventState(Uint8 type, int state)
{
    return SDL_ENABLE;
}

int SDL_Init(Uint32 flags)
{
	memset(SDL_ProcessEvents,SDL_ENABLE,sizeof(SDL_ProcessEvents));
    SDL_EventQ.lock = XMutex_create();

    return SDL_JoystickInit();
}

void SDL_Quit(void)
{
    SDL_JoystickQuit();
    XMutex_free( SDL_EventQ.lock );
}

void SDL_Lock_EventThread(void)
{
    XMutex_acquire( SDL_EventQ.lock );
}

void SDL_Unlock_EventThread(void)
{
    XMutex_release( SDL_EventQ.lock );
}

int SDL_WaitEvent(SDL_Event *event)
{
    //SDL_JoystickUpdate();
    //Mouse_poll();
    
    SDL_Lock_EventThread();
    
	while ( SDL_EventQ.tail == SDL_EventQ.head )
    {
        SDL_Unlock_EventThread();
        usleep( 10 );
        SDL_JoystickUpdate();
        Mouse_poll();
        SDL_Lock_EventThread();
	}
    
    *event = SDL_EventQ.event[SDL_EventQ.head];
    SDL_EventQ.head = ( SDL_EventQ.head + 1 ) % MAXEVENTS;
    
    SDL_Unlock_EventThread();
    
    return 0;
}


int SDL_PushEvent(SDL_Event *event)
{
	int tail, added;
    
    SDL_Lock_EventThread();
    
	tail = ( SDL_EventQ.tail + 1 ) % MAXEVENTS;
	if ( tail == SDL_EventQ.head )
    {
		/* Overflow, drop event */
		added = 0;
	} 
    
    else 
    {
		SDL_EventQ.event[SDL_EventQ.tail] = *event;
		SDL_EventQ.tail = tail;
		added = 1;
	}
    
    SDL_Unlock_EventThread();
    
	return(added);
}

#pragma mark >end:chuck/util_sdl.c

#pragma mark >begin:joystick/SDL_joystick.c

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* This is the joystick API for Simple DirectMedia Layer */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DISABLE_EVENTS
#pragma mark >>begin:events/SDL_events_c.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Useful functions and variables from SDL_events.c */

/* Start and stop the event processing loop */
extern int SDL_StartEventLoop(Uint32 flags);
extern void SDL_StopEventLoop(void);
extern void SDL_QuitInterrupt(void);

extern void SDL_Lock_EventThread();
extern void SDL_Unlock_EventThread();
extern Uint32 SDL_EventThreadID(void);

/* Event handler init routines */
extern int  SDL_AppActiveInit(void);
extern int  SDL_KeyboardInit(void);
extern int  SDL_MouseInit(void);
extern int  SDL_QuitInit(void);

/* Event handler quit routines */
extern void SDL_AppActiveQuit(void);
extern void SDL_KeyboardQuit(void);
extern void SDL_MouseQuit(void);
extern void SDL_QuitQuit(void);

/* The event filter function */
extern SDL_EventFilter SDL_EventOK;

/* The array of event processing states */
extern Uint8 SDL_ProcessEvents[SDL_NUMEVENTS];

/* Internal event queueing functions
   (from SDL_active.c, SDL_mouse.c, SDL_keyboard.c, SDL_quit.c, SDL_events.c)
 */
extern int SDL_PrivateAppActive(Uint8 gain, Uint8 state);
extern int SDL_PrivateMouseMotion(Uint8 buttonstate, int relative,
						Sint16 x, Sint16 y);
extern int SDL_PrivateMouseButton(Uint8 state, Uint8 button,Sint16 x,Sint16 y);
extern int SDL_PrivateKeyboard(Uint8 state, SDL_keysym *key);
extern int SDL_PrivateResize(int w, int h);
extern int SDL_PrivateExpose(void);
extern int SDL_PrivateQuit(void);
extern int SDL_PrivateSysWMEvent(SDL_SysWMmsg *message);

/* Used by the activity event handler to remove mouse focus */
extern void SDL_ResetMouse(void);

/* Used by the activity event handler to remove keyboard focus */
extern void SDL_ResetKeyboard(void);

/* Used by the event loop to queue pending keyboard repeat events */
extern void SDL_CheckKeyRepeat(void);

/* Used by the OS keyboard code to detect whether or not to do UNICODE */
#ifndef DEFAULT_UNICODE_TRANSLATION
#define DEFAULT_UNICODE_TRANSLATION 0	/* Default off because of overhead */
#endif
extern int SDL_TranslateUNICODE;
#pragma mark >>end:events/SDL_events_c.h

#endif
#pragma mark >>begin:joystick/SDL_joystick_c.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Useful functions and variables from SDL_joystick.c */

/* The number of available joysticks on the system */
extern Uint8 SDL_numjoysticks;

/* Internal event queueing functions */
extern int SDL_PrivateJoystickAxis(SDL_Joystick *joystick,
                                   Uint8 axis, Sint16 value);
extern int SDL_PrivateJoystickBall(SDL_Joystick *joystick,
                                   Uint8 ball, Sint16 xrel, Sint16 yrel);
extern int SDL_PrivateJoystickHat(SDL_Joystick *joystick,
                                 Uint8 hat, Uint8 value);
extern int SDL_PrivateJoystickButton(SDL_Joystick *joystick,
                                     Uint8 button, Uint8 state);
#pragma mark >>end:joystick/SDL_joystick_c.h

#pragma mark >>begin:joystick/SDL_sysjoystick.h

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* This is the system specific header for the SDL joystick API */


/* The SDL joystick structure */
struct _SDL_Joystick {
	Uint8 index;		/* Device index */
	const char *name;	/* Joystick name - system dependent */

	int naxes;		/* Number of axis controls on the joystick */
	Sint16 *axes;		/* Current axis states */

	int nhats;		/* Number of hats on the joystick */
	Uint8 *hats;		/* Current hat states */
	
	int nballs;		/* Number of trackballs on the joystick */
	struct balldelta {
		int dx;
		int dy;
	} *balls;		/* Current ball motion deltas */
	
	int nbuttons;		/* Number of buttons on the joystick */
	Uint8 *buttons;		/* Current button states */
	
	struct joystick_hwdata *hwdata;	/* Driver dependent information */

	int ref_count;		/* Reference count for multiple opens */
};

/* Function to scan the system for joysticks.
 * Joystick 0 should be the system default joystick.
 * This function should return the number of available joysticks, or -1
 * on an unrecoverable fatal error.
 */
extern int SDL_SYS_JoystickInit(void);

/* Function to get the device-dependent name of a joystick */
extern const char *SDL_SYS_JoystickName(int index);

/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */
extern int SDL_SYS_JoystickOpen(SDL_Joystick *joystick);

/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */
extern void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick);

/* Function to close a joystick after use */
extern void SDL_SYS_JoystickClose(SDL_Joystick *joystick);

/* Function to perform any system-specific joystick related cleanup */
extern void SDL_SYS_JoystickQuit(void);

#pragma mark >>end:joystick/SDL_sysjoystick.h


/* This is used for Quake III Arena */
#ifdef DISABLE_EVENTS
#define SDL_Lock_EventThread()
#define SDL_Unlock_EventThread()
#endif

Uint8 SDL_numjoysticks = 0;
SDL_Joystick **SDL_joysticks = NULL;
static SDL_Joystick *default_joystick = NULL;

int SDL_JoystickInit(void)
{
	int arraylen;
	int status;

	SDL_numjoysticks = 0;
	status = SDL_SYS_JoystickInit();
	if ( status >= 0 ) {
		arraylen = (status+1)*sizeof(*SDL_joysticks);
		SDL_joysticks = (SDL_Joystick **)malloc(arraylen);
		if ( SDL_joysticks == NULL ) {
			SDL_numjoysticks = 0;
		} else {
			memset(SDL_joysticks, 0, arraylen);
			SDL_numjoysticks = status;
		}
		status = 0;
	}
	default_joystick = NULL;
	return(status);
}

/*
 * Count the number of joysticks attached to the system
 */
int SDL_NumJoysticks(void)
{
	return SDL_numjoysticks;
}

/*
 * Get the implementation dependent name of a joystick
 */
const char *SDL_JoystickName(int device_index)
{
	if ( (device_index < 0) || (device_index >= SDL_numjoysticks) ) {
		SDL_SetError("There are %d joysticks available",
		             SDL_numjoysticks);
		return(NULL);
	}
	return(SDL_SYS_JoystickName(device_index));
}

/*
 * Open a joystick for use - the index passed as an argument refers to
 * the N'th joystick on the system.  This index is the value which will
 * identify this joystick in future joystick events.
 *
 * This function returns a joystick identifier, or NULL if an error occurred.
 */
SDL_Joystick *SDL_JoystickOpen(int device_index)
{
	int i;
	SDL_Joystick *joystick;

	if ( (device_index < 0) || (device_index >= SDL_numjoysticks) ) {
		SDL_SetError("There are %d joysticks available",
		             SDL_numjoysticks);
		return(NULL);
	}

	/* If the joystick is already open, return it */
	for ( i=0; SDL_joysticks[i]; ++i ) {
		if ( device_index == SDL_joysticks[i]->index ) {
			joystick = SDL_joysticks[i];
			++joystick->ref_count;
			return(joystick);
		}
	}

	/* Create and initialize the joystick */
	joystick = (SDL_Joystick *)malloc((sizeof *joystick));
	if ( joystick != NULL ) {
		memset(joystick, 0, (sizeof *joystick));
		joystick->index = device_index;
		if ( SDL_SYS_JoystickOpen(joystick) < 0 ) {
			free(joystick);
			joystick = NULL;
		} else {
			if ( joystick->naxes > 0 ) {
				joystick->axes = (Sint16 *)malloc
					(joystick->naxes*sizeof(Sint16));
			}
			if ( joystick->nhats > 0 ) {
				joystick->hats = (Uint8 *)malloc
					(joystick->nhats*sizeof(Uint8));
			}
			if ( joystick->nballs > 0 ) {
				joystick->balls = (struct balldelta *)malloc
				  (joystick->nballs*sizeof(*joystick->balls));
			}
			if ( joystick->nbuttons > 0 ) {
				joystick->buttons = (Uint8 *)malloc
					(joystick->nbuttons*sizeof(Uint8));
			}
			if ( ((joystick->naxes > 0) && !joystick->axes)
			  || ((joystick->nhats > 0) && !joystick->hats)
			  || ((joystick->nballs > 0) && !joystick->balls)
			  || ((joystick->nbuttons > 0) && !joystick->buttons)) {
				SDL_OutOfMemory();
				SDL_JoystickClose(joystick);
				joystick = NULL;
			}
			if ( joystick->axes ) {
				memset(joystick->axes, 0,
					joystick->naxes*sizeof(Sint16));
			}
			if ( joystick->hats ) {
				memset(joystick->hats, 0,
					joystick->nhats*sizeof(Uint8));
			}
			if ( joystick->balls ) {
				memset(joystick->balls, 0,
				  joystick->nballs*sizeof(*joystick->balls));
			}
			if ( joystick->buttons ) {
				memset(joystick->buttons, 0,
					joystick->nbuttons*sizeof(Uint8));
			}
		}
	}
	if ( joystick ) {
		/* Add joystick to list */
		++joystick->ref_count;
		SDL_Lock_EventThread();
		for ( i=0; SDL_joysticks[i]; ++i )
			/* Skip to next joystick */;
		SDL_joysticks[i] = joystick;
		SDL_Unlock_EventThread();
	}
	return(joystick);
}

/*
 * Returns 1 if the joystick has been opened, or 0 if it has not.
 */
int SDL_JoystickOpened(int device_index)
{
	int i, opened;

	opened = 0;
	for ( i=0; SDL_joysticks[i]; ++i ) {
		if ( SDL_joysticks[i]->index == (Uint8)device_index ) {
			opened = 1;
			break;
		}
	}
	return(opened);
}

static int ValidJoystick(SDL_Joystick **joystick)
{
	int valid;

	if ( *joystick == NULL ) {
		*joystick = default_joystick;
	}
	if ( *joystick == NULL ) {
		SDL_SetError("Joystick hasn't been opened yet");
		valid = 0;
	} else {
		valid = 1;
	}
	return valid;
}

/*
 * Get the device index of an opened joystick.
 */
int SDL_JoystickIndex(SDL_Joystick *joystick)
{
	if ( ! ValidJoystick(&joystick) ) {
		return(-1);
	}
	return(joystick->index);
}

/*
 * Get the number of multi-dimensional axis controls on a joystick
 */
int SDL_JoystickNumAxes(SDL_Joystick *joystick)
{
	if ( ! ValidJoystick(&joystick) ) {
		return(-1);
	}
	return(joystick->naxes);
}

/*
 * Get the number of hats on a joystick
 */
int SDL_JoystickNumHats(SDL_Joystick *joystick)
{
	if ( ! ValidJoystick(&joystick) ) {
		return(-1);
	}
	return(joystick->nhats);
}

/*
 * Get the number of trackballs on a joystick
 */
int SDL_JoystickNumBalls(SDL_Joystick *joystick)
{
	if ( ! ValidJoystick(&joystick) ) {
		return(-1);
	}
	return(joystick->nballs);
}

/*
 * Get the number of buttons on a joystick
 */
int SDL_JoystickNumButtons(SDL_Joystick *joystick)
{
	if ( ! ValidJoystick(&joystick) ) {
		return(-1);
	}
	return(joystick->nbuttons);
}

/*
 * Get the current state of an axis control on a joystick
 */
Sint16 SDL_JoystickGetAxis(SDL_Joystick *joystick, int axis)
{
	Sint16 state;

	if ( ! ValidJoystick(&joystick) ) {
		return(0);
	}
	if ( axis < joystick->naxes ) {
		state = joystick->axes[axis];
	} else {
		SDL_SetError("Joystick only has %d axes", joystick->naxes);
		state = 0;
	}
	return(state);
}

/*
 * Get the current state of a hat on a joystick
 */
Uint8 SDL_JoystickGetHat(SDL_Joystick *joystick, int hat)
{
	Uint8 state;

	if ( ! ValidJoystick(&joystick) ) {
		return(0);
	}
	if ( hat < joystick->nhats ) {
		state = joystick->hats[hat];
	} else {
		SDL_SetError("Joystick only has %d hats", joystick->nhats);
		state = 0;
	}
	return(state);
}

/*
 * Get the ball axis change since the last poll
 */
int SDL_JoystickGetBall(SDL_Joystick *joystick, int ball, int *dx, int *dy)
{
	int retval;

	if ( ! ValidJoystick(&joystick) ) {
		return(-1);
	}

	retval = 0;
	if ( ball < joystick->nballs ) {
		if ( dx ) {
			*dx = joystick->balls[ball].dx;
		}
		if ( dy ) {
			*dy = joystick->balls[ball].dy;
		}
		joystick->balls[ball].dx = 0;
		joystick->balls[ball].dy = 0;
	} else {
		SDL_SetError("Joystick only has %d balls", joystick->nballs);
		retval = -1;
	}
	return(retval);
}

/*
 * Get the current state of a button on a joystick
 */
Uint8 SDL_JoystickGetButton(SDL_Joystick *joystick, int button)
{
	Uint8 state;

	if ( ! ValidJoystick(&joystick) ) {
		return(0);
	}
	if ( button < joystick->nbuttons ) {
		state = joystick->buttons[button];
	} else {
		SDL_SetError("Joystick only has %d buttons",joystick->nbuttons);
		state = 0;
	}
	return(state);
}

/*
 * Close a joystick previously opened with SDL_JoystickOpen()
 */
void SDL_JoystickClose(SDL_Joystick *joystick)
{
	int i;

	if ( ! ValidJoystick(&joystick) ) {
		return;
	}

	/* First decrement ref count */
	if ( --joystick->ref_count > 0 ) {
		return;
	}

	/* Lock the event queue - prevent joystick polling */
	SDL_Lock_EventThread();

	if ( joystick == default_joystick ) {
		default_joystick = NULL;
	}
	SDL_SYS_JoystickClose(joystick);

	/* Remove joystick from list */
	for ( i=0; SDL_joysticks[i]; ++i ) {
		if ( joystick == SDL_joysticks[i] ) {
			memcpy(&SDL_joysticks[i], &SDL_joysticks[i+1],
			       (SDL_numjoysticks-i)*sizeof(joystick));
			break;
		}
	}

	/* Let the event thread keep running */
	SDL_Unlock_EventThread();

	/* Free the data associated with this joystick */
	if ( joystick->axes ) {
		free(joystick->axes);
	}
	if ( joystick->hats ) {
		free(joystick->hats);
	}
	if ( joystick->balls ) {
		free(joystick->balls);
	}
	if ( joystick->buttons ) {
		free(joystick->buttons);
	}
	free(joystick);
}

void SDL_JoystickQuit(void)
{
	/* Stop the event polling */
	SDL_Lock_EventThread();
	SDL_numjoysticks = 0;
	SDL_Unlock_EventThread();

	/* Quit the joystick setup */
	SDL_SYS_JoystickQuit();
	if ( SDL_joysticks ) {
		free(SDL_joysticks);
		SDL_joysticks = NULL;
	}
}


/* These are global for SDL_sysjoystick.c and SDL_events.c */

int SDL_PrivateJoystickAxis(SDL_Joystick *joystick, Uint8 axis, Sint16 value)
{
	int posted;

	/* Update internal joystick state */
	joystick->axes[axis] = value;

	/* Post the event, if desired */
	posted = 0;
#ifndef DISABLE_EVENTS
	if ( SDL_ProcessEvents[SDL_JOYAXISMOTION] == SDL_ENABLE ) {
		SDL_Event event;
		event.type = SDL_JOYAXISMOTION;
		event.jaxis.which = joystick->index;
		event.jaxis.axis = axis;
		event.jaxis.value = value;
		if ( (SDL_EventOK == NULL) || (*SDL_EventOK)(&event) ) {
			posted = 1;
			SDL_PushEvent(&event);
		}
	}
#endif /* !DISABLE_EVENTS */
	return(posted);
}

int SDL_PrivateJoystickHat(SDL_Joystick *joystick, Uint8 hat, Uint8 value)
{
	int posted;

	/* Update internal joystick state */
	joystick->hats[hat] = value;

	/* Post the event, if desired */
	posted = 0;
#ifndef DISABLE_EVENTS
	if ( SDL_ProcessEvents[SDL_JOYHATMOTION] == SDL_ENABLE ) {
		SDL_Event event;
		event.jhat.type = SDL_JOYHATMOTION;
		event.jhat.which = joystick->index;
		event.jhat.hat = hat;
		event.jhat.value = value;
		if ( (SDL_EventOK == NULL) || (*SDL_EventOK)(&event) ) {
			posted = 1;
			SDL_PushEvent(&event);
		}
	}
#endif /* !DISABLE_EVENTS */
	return(posted);
}

int SDL_PrivateJoystickBall(SDL_Joystick *joystick, Uint8 ball,
					Sint16 xrel, Sint16 yrel)
{
	int posted;

	/* Update internal mouse state */
	joystick->balls[ball].dx += xrel;
	joystick->balls[ball].dy += yrel;

	/* Post the event, if desired */
	posted = 0;
#ifndef DISABLE_EVENTS
	if ( SDL_ProcessEvents[SDL_JOYBALLMOTION] == SDL_ENABLE ) {
		SDL_Event event;
		event.jball.type = SDL_JOYBALLMOTION;
		event.jball.which = joystick->index;
		event.jball.ball = ball;
		event.jball.xrel = xrel;
		event.jball.yrel = yrel;
		if ( (SDL_EventOK == NULL) || (*SDL_EventOK)(&event) ) {
			posted = 1;
			SDL_PushEvent(&event);
		}
	}
#endif /* !DISABLE_EVENTS */
	return(posted);
}

int SDL_PrivateJoystickButton(SDL_Joystick *joystick, Uint8 button, Uint8 state)
{
	int posted;
#ifndef DISABLE_EVENTS
	SDL_Event event;

	switch ( state ) {
		case SDL_PRESSED:
			event.type = SDL_JOYBUTTONDOWN;
			break;
		case SDL_RELEASED:
			event.type = SDL_JOYBUTTONUP;
			break;
		default:
			/* Invalid state -- bail */
			return(0);
	}
#endif /* !DISABLE_EVENTS */

	/* Update internal joystick state */
	joystick->buttons[button] = state;

	/* Post the event, if desired */
	posted = 0;
#ifndef DISABLE_EVENTS
	if ( SDL_ProcessEvents[event.type] == SDL_ENABLE ) {
		event.jbutton.which = joystick->index;
		event.jbutton.button = button;
		event.jbutton.state = state;
		if ( (SDL_EventOK == NULL) || (*SDL_EventOK)(&event) ) {
			posted = 1;
			SDL_PushEvent(&event);
		}
	}
#endif /* !DISABLE_EVENTS */
	return(posted);
}

void SDL_JoystickUpdate(void)
{
	int i;

	for ( i=0; SDL_joysticks[i]; ++i ) {
		SDL_SYS_JoystickUpdate(SDL_joysticks[i]);
	}
}

int SDL_JoystickEventState(int state)
{
#ifdef DISABLE_EVENTS
	return SDL_IGNORE;
#else
	const Uint8 event_list[] = {
		SDL_JOYAXISMOTION, SDL_JOYBALLMOTION, SDL_JOYHATMOTION,
		SDL_JOYBUTTONDOWN, SDL_JOYBUTTONUP,
	};
	int i;

	switch (state) {
		case SDL_QUERY:
			state = SDL_IGNORE;
			for ( i=0; i<SDL_TABLESIZE(event_list); ++i ) {
				state = SDL_EventState(event_list[i],SDL_QUERY);
				if ( state == SDL_ENABLE ) {
					break;
				}
			}
			break;
		default:
			for ( i=0; i<SDL_TABLESIZE(event_list); ++i ) {
				SDL_EventState(event_list[i], state);
			}
			break;
	}
	return(state);
#endif /* DISABLE_EVENTS */
}
#pragma mark >end:joystick/SDL_joystick.c

#ifdef __MACOSX_CORE__
#pragma mark >begin:joystick/darwin/SDL_sysjoystick.c

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

/* SDL joystick driver for Darwin / MacOS X, based on the IOKit HID API */
/* Written 2001 by Max Horn */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/errno.h>
#include <sysexits.h>
#include <mach/mach.h>
#include <mach/mach_error.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#ifdef MACOS_10_0_4
#include <IOKit/hidsystem/IOHIDUsageTables.h>
#else
/* The header was moved here in MacOS X 10.1 */
#include <Kernel/IOKit/hidsystem/IOHIDUsageTables.h>
#endif
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h> /* for NewPtrClear, DisposePtr */


struct recElement
{
	IOHIDElementCookie cookie;				// unique value which identifies element, will NOT change
	long min;								// reported min value possible
	long max;								// reported max value possible
/*
	TODO: maybe should handle the following stuff somehow?

	long scaledMin;							// reported scaled min value possible
	long scaledMax;							// reported scaled max value possible
	long size;								// size in bits of data return from element
	Boolean relative;						// are reports relative to last report (deltas)
	Boolean wrapping;						// does element wrap around (one value higher than max is min)
	Boolean nonLinear;						// are the values reported non-linear relative to element movement
	Boolean preferredState;					// does element have a preferred state (such as a button)
	Boolean nullState;						// does element have null state
*/

	/* runtime variables used for auto-calibration */
	long minReport;							// min returned value
	long maxReport;							// max returned value
	
	struct recElement * pNext;				// next element in list
};
typedef struct recElement recElement;

struct joystick_hwdata
{
	IOHIDDeviceInterface ** interface;		// interface to device, NULL = no interface

	char product[256];							// name of product
	long usage;								// usage page from IOUSBHID Parser.h which defines general usage
	long usagePage;							// usage within above page from IOUSBHID Parser.h which defines specific usage

	long axes;								// number of axis (calculated, not reported by device)
	long buttons;							// number of buttons (calculated, not reported by device)
	long hats;								// number of hat switches (calculated, not reported by device)
	long elements;							// number of total elements (shouldbe total of above) (calculated, not reported by device)

	recElement* firstAxis;
	recElement* firstButton;
	recElement* firstHat;

	int removed;
	int uncentered;

	struct joystick_hwdata* pNext;			// next device
};
typedef struct joystick_hwdata recDevice;


/* Linked list of all available devices */
static recDevice *gpDeviceList = NULL;


static void HIDReportErrorNum (char * strError, long numError)
{
	SDL_SetError(strError);
}

static void HIDGetCollectionElements (CFMutableDictionaryRef deviceProperties, recDevice *pDevice);

/* returns current value for element, polling element
 * will return 0 on error conditions which should be accounted for by application
 */

static SInt32 HIDGetElementValue (recDevice *pDevice, recElement *pElement)
{
	IOReturn result = kIOReturnSuccess;
	IOHIDEventStruct hidEvent;
	hidEvent.value = 0;
	
	if (NULL != pDevice && NULL != pElement && NULL != pDevice->interface)
	{
		result = (*(pDevice->interface))->getElementValue(pDevice->interface, pElement->cookie, &hidEvent);
		if (kIOReturnSuccess == result)
		{
			/* record min and max for auto calibration */
			if (hidEvent.value < pElement->minReport)
				pElement->minReport = hidEvent.value;
			if (hidEvent.value > pElement->maxReport)
				pElement->maxReport = hidEvent.value;
		}
	}

	// auto user scale
	return hidEvent.value;
}

/* similiar to HIDGetElementValue, but auto-calibrates the value before returning it */

static SInt32 HIDCalibratedValue (recDevice *pDevice, recElement *pElement)
{
	float deviceScale = pElement->max - pElement->min;
	float readScale = pElement->maxReport - pElement->minReport;
	SInt32 value = HIDGetElementValue(pDevice, pElement);
	if (readScale == 0)
		return value; // no scaling at all
	else
		return ((value - pElement->minReport) * deviceScale / readScale) + pElement->min;
}

/* similiar to HIDCalibratedValue but calibrates to an arbitrary scale instead of the elements default scale */

static SInt32 HIDScaledCalibratedValue (recDevice *pDevice, recElement *pElement, long min, long max)
{
	float deviceScale = max - min;
	float readScale = pElement->maxReport - pElement->minReport;
	SInt32 value = HIDGetElementValue(pDevice, pElement);
	if (readScale == 0)
		return value; // no scaling at all
	else
		return ((value - pElement->minReport) * deviceScale / readScale) + min;
}


static void HIDRemovalCallback(void * target,
                               IOReturn result,
                               void * refcon,
                               void * sender)
{
	recDevice *device = (recDevice *) refcon;
	device->removed = 1;
	device->uncentered = 1;
}



/* Create and open an interface to device, required prior to extracting values or building queues.
 * Note: appliction now owns the device and must close and release it prior to exiting
 */

static IOReturn HIDCreateOpenDeviceInterface (io_object_t hidDevice, recDevice *pDevice)
{
	IOReturn result = kIOReturnSuccess;
	HRESULT plugInResult = S_OK;
	SInt32 score = 0;
	IOCFPlugInInterface ** ppPlugInInterface = NULL;
	
	if (NULL == pDevice->interface)
	{
		result = IOCreatePlugInInterfaceForService (hidDevice, kIOHIDDeviceUserClientTypeID,
													kIOCFPlugInInterfaceID, &ppPlugInInterface, &score);
		if (kIOReturnSuccess == result)
		{
			// Call a method of the intermediate plug-in to create the device interface
			plugInResult = (*ppPlugInInterface)->QueryInterface (ppPlugInInterface,
								CFUUIDGetUUIDBytes (kIOHIDDeviceInterfaceID), (void *) &(pDevice->interface));
			if (S_OK != plugInResult)
				HIDReportErrorNum ("CouldnÕt query HID class device interface from plugInInterface", plugInResult);
			(*ppPlugInInterface)->Release (ppPlugInInterface);
		}
		else
			HIDReportErrorNum ("Failed to create **plugInInterface via IOCreatePlugInInterfaceForService.", result);
	}
	if (NULL != pDevice->interface)
	{
		result = (*(pDevice->interface))->open (pDevice->interface, 0);
		if (kIOReturnSuccess != result)
			HIDReportErrorNum ("Failed to open pDevice->interface via open.", result);
		else
			(*(pDevice->interface))->setRemovalCallback (pDevice->interface, HIDRemovalCallback, pDevice, pDevice);

	}
	return result;
}

/* Closes and releases interface to device, should be done prior to exting application
 * Note: will have no affect if device or interface do not exist
 * application will "own" the device if interface is not closed
 * (device may have to be plug and re-plugged in different location to get it working again without a restart)
 */

static IOReturn HIDCloseReleaseInterface (recDevice *pDevice)
{
	IOReturn result = kIOReturnSuccess;
	
	if ((NULL != pDevice) && (NULL != pDevice->interface))
	{
		// close the interface
		result = (*(pDevice->interface))->close (pDevice->interface);
		if (kIOReturnNotOpen == result)
		{
			//  do nothing as device was not opened, thus can't be closed
		}
		else if (kIOReturnSuccess != result)
			HIDReportErrorNum ("Failed to close IOHIDDeviceInterface.", result);
		//release the interface
		result = (*(pDevice->interface))->Release (pDevice->interface);
		if (kIOReturnSuccess != result)
			HIDReportErrorNum ("Failed to release IOHIDDeviceInterface.", result);
		pDevice->interface = NULL;
	}	
	return result;
}

/* extracts actual specific element information from each element CF dictionary entry */

static void HIDGetElementInfo (CFTypeRef refElement, recElement *pElement)
{
	long number;
	CFTypeRef refType;

	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementCookieKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->cookie = (IOHIDElementCookie) number;
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementMinKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->min = number;
		pElement->maxReport = pElement->min;
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementMaxKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->max = number;
		pElement->minReport = pElement->max;
/*
	TODO: maybe should handle the following stuff somehow?

	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementScaledMinKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->scaledMin = number;
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementScaledMaxKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->scaledMax = number;
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementSizeKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->size = number;
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementIsRelativeKey));
	if (refType)
		pElement->relative = CFBooleanGetValue (refType);
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementIsWrappingKey));
	if (refType)
		pElement->wrapping = CFBooleanGetValue (refType);
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementIsNonLinearKey));
	if (refType)
		pElement->nonLinear = CFBooleanGetValue (refType);
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementHasPreferedStateKey));
	if (refType)
		pElement->preferredState = CFBooleanGetValue (refType);
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementHasNullStateKey));
	if (refType)
		pElement->nullState = CFBooleanGetValue (refType);
*/
}			

/* examines CF dictionary vlaue in device element hierarchy to determine if it is element of interest or a collection of more elements
 * if element of interest allocate storage, add to list and retrieve element specific info
 * if collection then pass on to deconstruction collection into additional individual elements
 */

static void HIDAddElement (CFTypeRef refElement, recDevice* pDevice)
{
	recElement* element = NULL;
	recElement** headElement = NULL;
	long elementType, usagePage, usage;
	CFTypeRef refElementType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementTypeKey));
	CFTypeRef refUsagePage = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementUsagePageKey));
	CFTypeRef refUsage = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementUsageKey));


	if ((refElementType) && (CFNumberGetValue (refElementType, kCFNumberLongType, &elementType)))
	{
		/* look at types of interest */
		if ((elementType == kIOHIDElementTypeInput_Misc) || (elementType == kIOHIDElementTypeInput_Button) ||
			(elementType == kIOHIDElementTypeInput_Axis))
		{
			if (refUsagePage && CFNumberGetValue (refUsagePage, kCFNumberLongType, &usagePage) &&
				refUsage && CFNumberGetValue (refUsage, kCFNumberLongType, &usage))
			{
				switch (usagePage) /* only interested in kHIDPage_GenericDesktop and kHIDPage_Button */
				{
					case kHIDPage_GenericDesktop:
						{
							switch (usage) /* look at usage to determine function */
							{
								case kHIDUsage_GD_X:
								case kHIDUsage_GD_Y:
								case kHIDUsage_GD_Z:
								case kHIDUsage_GD_Rx:
								case kHIDUsage_GD_Ry:
								case kHIDUsage_GD_Rz:
								case kHIDUsage_GD_Slider:
								case kHIDUsage_GD_Dial:
								case kHIDUsage_GD_Wheel:
									element = (recElement *) NewPtrClear (sizeof (recElement));
									if (element)
									{
										pDevice->axes++;
										headElement = &(pDevice->firstAxis);
									}
								break;
								case kHIDUsage_GD_Hatswitch:
									element = (recElement *) NewPtrClear (sizeof (recElement));
									if (element)
									{
										pDevice->hats++;
										headElement = &(pDevice->firstHat);
									}
								break;
							}							
						}
						break;
					case kHIDPage_Button:
						element = (recElement *) NewPtrClear (sizeof (recElement));
						if (element)
						{
							pDevice->buttons++;
							headElement = &(pDevice->firstButton);
						}
						break;
					default:
						break;
				}
			}
		}
		else if (kIOHIDElementTypeCollection == elementType)
			HIDGetCollectionElements ((CFMutableDictionaryRef) refElement, pDevice);
	}

	if (element && headElement) /* add to list */
	{
		pDevice->elements++;
		if (NULL == *headElement)
			*headElement = element;
		else
		{
			recElement *elementPrevious, *elementCurrent;
			elementCurrent = *headElement;
			while (elementCurrent)
			{
				elementPrevious = elementCurrent;
				elementCurrent = elementPrevious->pNext;
			}
			elementPrevious->pNext = element;
		}
		element->pNext = NULL;
		HIDGetElementInfo (refElement, element);
	}
}

/* collects information from each array member in device element list (each array memeber = element) */

static void HIDGetElementsCFArrayHandler (const void * value, void * parameter)
{
	if (CFGetTypeID (value) == CFDictionaryGetTypeID ())
		HIDAddElement ((CFTypeRef) value, (recDevice *) parameter);
}

/* handles retrieval of element information from arrays of elements in device IO registry information */

static void HIDGetElements (CFTypeRef refElementCurrent, recDevice *pDevice)
{
	CFTypeID type = CFGetTypeID (refElementCurrent);
	if (type == CFArrayGetTypeID()) /* if element is an array */
	{
		CFRange range = {0, CFArrayGetCount (refElementCurrent)};
		/* CountElementsCFArrayHandler called for each array member */
		CFArrayApplyFunction (refElementCurrent, range, HIDGetElementsCFArrayHandler, pDevice);
	}
}			

/* handles extracting element information from element collection CF types
 * used from top level element decoding and hierarchy deconstruction to flatten device element list
 */

static void HIDGetCollectionElements (CFMutableDictionaryRef deviceProperties, recDevice *pDevice)
{
	CFTypeRef refElementTop = CFDictionaryGetValue (deviceProperties, CFSTR(kIOHIDElementKey));
	if (refElementTop)
		HIDGetElements (refElementTop, pDevice);
}

/* use top level element usage page and usage to discern device usage page and usage setting appropriate vlaues in device record */

static void HIDTopLevelElementHandler (const void * value, void * parameter)
{
	CFTypeRef refCF = 0;
	if (CFGetTypeID (value) != CFDictionaryGetTypeID ())
		return;
	refCF = CFDictionaryGetValue (value, CFSTR(kIOHIDElementUsagePageKey));
	if (!CFNumberGetValue (refCF, kCFNumberLongType, &((recDevice *) parameter)->usagePage))
		SDL_SetError ("CFNumberGetValue error retrieving pDevice->usagePage.");
	refCF = CFDictionaryGetValue (value, CFSTR(kIOHIDElementUsageKey));
	if (!CFNumberGetValue (refCF, kCFNumberLongType, &((recDevice *) parameter)->usage))
		SDL_SetError ("CFNumberGetValue error retrieving pDevice->usage.");
}

/* extracts device info from CF dictionary records in IO registry */

static void HIDGetDeviceInfo (io_object_t hidDevice, CFMutableDictionaryRef hidProperties, recDevice *pDevice)
{
	CFMutableDictionaryRef usbProperties = 0;
	io_registry_entry_t parent1, parent2;
	
	/* Mac OS X currently is not mirroring all USB properties to HID page so need to look at USB device page also
	 * get dictionary for usb properties: step up two levels and get CF dictionary for USB properties
	 */
	if ((KERN_SUCCESS == IORegistryEntryGetParentEntry (hidDevice, kIOServicePlane, &parent1)) &&
		(KERN_SUCCESS == IORegistryEntryGetParentEntry (parent1, kIOServicePlane, &parent2)) &&
		(KERN_SUCCESS == IORegistryEntryCreateCFProperties (parent2, &usbProperties, kCFAllocatorDefault, kNilOptions)))
	{
		if (usbProperties)
		{
			CFTypeRef refCF = 0;
			/* get device info
			 * try hid dictionary first, if fail then go to usb dictionary
			 */
			
			
			/* get product name */
			refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDProductKey));
			if (!refCF)
				refCF = CFDictionaryGetValue (usbProperties, CFSTR("USB Product Name"));
			if (refCF)
			{
				if (!CFStringGetCString (refCF, pDevice->product, 256, CFStringGetSystemEncoding ()))
					SDL_SetError ("CFStringGetCString error retrieving pDevice->product.");
			}
			
			/* get usage page and usage */
			refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDPrimaryUsagePageKey));
			if (refCF)
			{
				if (!CFNumberGetValue (refCF, kCFNumberLongType, &pDevice->usagePage))
					SDL_SetError ("CFNumberGetValue error retrieving pDevice->usagePage.");
				refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDPrimaryUsageKey));
				if (refCF)
					if (!CFNumberGetValue (refCF, kCFNumberLongType, &pDevice->usage))
						SDL_SetError ("CFNumberGetValue error retrieving pDevice->usage.");
			}

			if (NULL == refCF) /* get top level element HID usage page or usage */
			{
				/* use top level element instead */
				CFTypeRef refCFTopElement = 0;
				refCFTopElement = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDElementKey));
				{
					/* refCFTopElement points to an array of element dictionaries */
					CFRange range = {0, CFArrayGetCount (refCFTopElement)};
					CFArrayApplyFunction (refCFTopElement, range, HIDTopLevelElementHandler, pDevice);
				}
			}

			CFRelease (usbProperties);
		}
		else
			SDL_SetError ("IORegistryEntryCreateCFProperties failed to create usbProperties.");

		if (kIOReturnSuccess != IOObjectRelease (parent2))
			SDL_SetError ("IOObjectRelease error with parent2.");
		if (kIOReturnSuccess != IOObjectRelease (parent1))
			SDL_SetError ("IOObjectRelease error with parent1.");
	}
}


static recDevice *HIDBuildDevice (io_object_t hidDevice)
{
	recDevice *pDevice = (recDevice *) NewPtrClear (sizeof (recDevice));
	if (pDevice)
	{
		/* get dictionary for HID properties */
		CFMutableDictionaryRef hidProperties = 0;
		kern_return_t result = IORegistryEntryCreateCFProperties (hidDevice, &hidProperties, kCFAllocatorDefault, kNilOptions);
		if ((result == KERN_SUCCESS) && hidProperties)
		{
			/* create device interface */
			result = HIDCreateOpenDeviceInterface (hidDevice, pDevice);
			if (kIOReturnSuccess == result)
			{
				HIDGetDeviceInfo (hidDevice, hidProperties, pDevice); /* hidDevice used to find parents in registry tree */
				HIDGetCollectionElements (hidProperties, pDevice);
			}
			else
			{
				DisposePtr((Ptr)pDevice);
				pDevice = NULL;
			}
			CFRelease (hidProperties);
		}
		else
		{
			DisposePtr((Ptr)pDevice);
			pDevice = NULL;
		}
	}
	return pDevice;
}

/* disposes of the element list associated with a device and the memory associated with the list
 */

static void HIDDisposeElementList (recElement **elementList)
{
	recElement *pElement = *elementList;
	while (pElement)
	{
		recElement *pElementNext = pElement->pNext;
		DisposePtr ((Ptr) pElement);
		pElement = pElementNext;
	}
	*elementList = NULL;
}

/* disposes of a single device, closing and releaseing interface, freeing memory fro device and elements, setting device pointer to NULL
 * all your device no longer belong to us... (i.e., you do not 'own' the device anymore)
 */

static recDevice *HIDDisposeDevice (recDevice **ppDevice)
{
	kern_return_t result = KERN_SUCCESS;
	recDevice *pDeviceNext = NULL;
	if (*ppDevice)
	{
		// save next device prior to disposing of this device
		pDeviceNext = (*ppDevice)->pNext;
		
		/* free element lists */
		HIDDisposeElementList (&(*ppDevice)->firstAxis);
		HIDDisposeElementList (&(*ppDevice)->firstButton);
		HIDDisposeElementList (&(*ppDevice)->firstHat);
		
		result = HIDCloseReleaseInterface (*ppDevice); /* function sanity checks interface value (now application does not own device) */
		if (kIOReturnSuccess != result)
			HIDReportErrorNum ("HIDCloseReleaseInterface failed when trying to dipose device.", result);
		DisposePtr ((Ptr)*ppDevice);
		*ppDevice = NULL;
	}
	return pDeviceNext;
}


/* Function to scan the system for joysticks.
 * Joystick 0 should be the system default joystick.
 * This function should return the number of available joysticks, or -1
 * on an unrecoverable fatal error.
 */
int SDL_SYS_JoystickInit(void)
{
	IOReturn result = kIOReturnSuccess;
	mach_port_t masterPort = 0;
	io_iterator_t hidObjectIterator = 0;
	CFMutableDictionaryRef hidMatchDictionary = NULL;
	recDevice *device, *lastDevice;
	io_object_t ioHIDDeviceObject = 0;
	
	SDL_numjoysticks = 0;
	
	if (NULL != gpDeviceList)
	{
		SDL_SetError("Joystick: Device list already inited.");
		return -1;
	}
	
	result = IOMasterPort (bootstrap_port, &masterPort);
	if (kIOReturnSuccess != result)
	{
		SDL_SetError("Joystick: IOMasterPort error with bootstrap_port.");
		return -1;
	}

	/* Set up a matching dictionary to search I/O Registry by class name for all HID class devices. */
	hidMatchDictionary = IOServiceMatching (kIOHIDDeviceKey);
	if ((hidMatchDictionary != NULL))
	{
		/* Add key for device type (joystick, in this case) to refine the matching dictionary. */
		
		/* NOTE: we now perform this filtering later
		UInt32 usagePage = kHIDPage_GenericDesktop;
		UInt32 usage = kHIDUsage_GD_Joystick;
		CFNumberRef refUsage = NULL, refUsagePage = NULL;

		refUsage = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &usage);
		CFDictionarySetValue (hidMatchDictionary, CFSTR (kIOHIDPrimaryUsageKey), refUsage);
		refUsagePage = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &usagePage);
		CFDictionarySetValue (hidMatchDictionary, CFSTR (kIOHIDPrimaryUsagePageKey), refUsagePage);
		*/
	}
	else
	{
		SDL_SetError("Joystick: Failed to get HID CFMutableDictionaryRef via IOServiceMatching.");
		return -1;
	}
	
	/*/ Now search I/O Registry for matching devices. */
	result = IOServiceGetMatchingServices (masterPort, hidMatchDictionary, &hidObjectIterator);
	/* Check for errors */
	if (kIOReturnSuccess != result)
	{
		SDL_SetError("Joystick: Couldn't create a HID object iterator.");
		return -1;
	}
	if (0 == hidObjectIterator) /* there are no joysticks */
	{
		gpDeviceList = NULL;
		SDL_numjoysticks = 0;
		return 0;
	}
	/* IOServiceGetMatchingServices consumes a reference to the dictionary, so we don't need to release the dictionary ref. */

	/* build flat linked list of devices from device iterator */

	gpDeviceList = lastDevice = NULL;
	
	while ((ioHIDDeviceObject = IOIteratorNext (hidObjectIterator)))
	{
		/* build a device record */
		device = HIDBuildDevice (ioHIDDeviceObject);
		if (!device)
			continue;

		/* dump device object, it is no longer needed */
		result = IOObjectRelease (ioHIDDeviceObject);
//		if (KERN_SUCCESS != result)
//			HIDReportErrorNum ("IOObjectRelease error with ioHIDDeviceObject.", result);

		/* Filter device list to non-keyboard/mouse stuff */ 
		if ( (device->usagePage != kHIDPage_GenericDesktop) ||
		     ((device->usage != kHIDUsage_GD_Joystick &&
		      device->usage != kHIDUsage_GD_GamePad)) ) {

			/* release memory for the device */
			HIDDisposeDevice (&device);
			DisposePtr((Ptr)device);
			continue;
		}
		
		/* Add device to the end of the list */
		if (lastDevice)
			lastDevice->pNext = device;
		else
			gpDeviceList = device;
		lastDevice = device;
	}
	result = IOObjectRelease (hidObjectIterator); /* release the iterator */

	/* Count the total number of devices we found */
	device = gpDeviceList;
	while (device)
	{
		SDL_numjoysticks++;
		device = device->pNext;
	}
	
	return SDL_numjoysticks;
}

/* Function to get the device-dependent name of a joystick */
const char *SDL_SYS_JoystickName(int index)
{
	recDevice *device = gpDeviceList;
	
	for (; index > 0; index--)
		device = device->pNext;

	return device->product;
}

/* Function to open a joystick for use.
 * The joystick to open is specified by the index field of the joystick.
 * This should fill the nbuttons and naxes fields of the joystick structure.
 * It returns 0, or -1 if there is an error.
 */
int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	recDevice *device = gpDeviceList;
	int index;
	
	for (index = joystick->index; index > 0; index--)
		device = device->pNext;

	joystick->hwdata = device;
	joystick->name = device->product;

	joystick->naxes = device->axes;
	joystick->nhats = device->hats;
	joystick->nballs = 0;
	joystick->nbuttons = device->buttons;

	return 0;
}

/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */
void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
	recDevice *device = joystick->hwdata;
	recElement *element;
	SInt32 value;
	int i;

	if (device->removed)  /* device was unplugged; ignore it. */
	{
		if (device->uncentered)
		{
			device->uncentered = 0;

			/* Tell the app that everything is centered/unpressed... */
			for (i = 0; i < device->axes; i++)
				SDL_PrivateJoystickAxis(joystick, i, 0);

			for (i = 0; i < device->buttons; i++)
				SDL_PrivateJoystickButton(joystick, i, 0);

			for (i = 0; i < device->hats; i++)
				SDL_PrivateJoystickHat(joystick, i, SDL_HAT_CENTERED);
		}

		return;
	}

	element = device->firstAxis;
	i = 0;
	while (element)
	{
		value = HIDScaledCalibratedValue(device, element, -32768, 32767);
		if ( value != joystick->axes[i] )
			SDL_PrivateJoystickAxis(joystick, i, value);
		element = element->pNext;
		++i;
	}
	
	element = device->firstButton;
	i = 0;
	while (element)
	{
		value = HIDGetElementValue(device, element);
        if (value > 1)  /* handle pressure-sensitive buttons */
            value = 1;
		if ( value != joystick->buttons[i] )
			SDL_PrivateJoystickButton(joystick, i, value);
		element = element->pNext;
		++i;
	}
	    
	element = device->firstHat;
	i = 0;
	while (element)
	{
		Uint8 pos = 0;

		value = HIDGetElementValue(device, element);
		if (element->max == 3) /* 4 position hatswitch - scale up value */
			value *= 2;
		else if (element->max != 7) /* Neither a 4 nor 8 positions - fall back to default position (centered) */
			value = -1;
		switch(value)
		{
			case 0:
				pos = SDL_HAT_UP;
				break;
			case 1:
				pos = SDL_HAT_RIGHTUP;
				break;
			case 2:
				pos = SDL_HAT_RIGHT;
				break;
			case 3:
				pos = SDL_HAT_RIGHTDOWN;
				break;
			case 4:
				pos = SDL_HAT_DOWN;
				break;
			case 5:
				pos = SDL_HAT_LEFTDOWN;
				break;
			case 6:
				pos = SDL_HAT_LEFT;
				break;
			case 7:
				pos = SDL_HAT_LEFTUP;
				break;
			default:
				/* Every other value is mapped to center. We do that because some
				 * joysticks use 8 and some 15 for this value, and apparently
				 * there are even more variants out there - so we try to be generous.
				 */
				pos = SDL_HAT_CENTERED;
				break;
		}
		if ( pos != joystick->hats[i] )
			SDL_PrivateJoystickHat(joystick, i, pos);
		element = element->pNext;
		++i;
	}
	
	return;
}

/* Function to close a joystick after use */
void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
	/* Should we do anything here? */
	return;
}

/* Function to perform any system-specific joystick related cleanup */
void SDL_SYS_JoystickQuit(void)
{
	while (NULL != gpDeviceList)
		gpDeviceList = HIDDisposeDevice (&gpDeviceList);
}
#pragma mark >end:joystick/darwin/SDL_sysjoystick.c

#elif defined( __WINDOWS_DS__ )

#pragma mark >begin:joystick/win32/SDL_mmjoystick.c

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* Win32 MultiMedia Joystick driver, contributed by Andrei de A. Formiga */

#include <stdlib.h>
#include <stdio.h>		/* For the definition of NULL */


#include <windows.h>
#include <mmsystem.h>
#include <regstr.h>

#define MAX_JOYSTICKS	16
#define MAX_AXES	6	/* each joystick can have up to 6 axes */
#define MAX_BUTTONS	32	/* and 32 buttons                      */
#define AXIS_MIN	-32768  /* minimum value for axis coordinate */
#define AXIS_MAX	32767   /* maximum value for axis coordinate */
/* limit axis to 256 possible positions to filter out noise */
#define JOY_AXIS_THRESHOLD      (((AXIS_MAX)-(AXIS_MIN))/256)
#define JOY_BUTTON_FLAG(n)	(1<<n)


/* array to hold joystick ID values */
static UINT	SYS_JoystickID[MAX_JOYSTICKS];
static JOYCAPS	SYS_Joystick[MAX_JOYSTICKS];
static char	*SYS_JoystickName[MAX_JOYSTICKS];

/* The private structure used to keep track of a joystick */
struct joystick_hwdata
{
	/* joystick ID */
	UINT	id;

	/* values used to translate device-specific coordinates into
	   SDL-standard ranges */
	struct _transaxis {
		int offset;
		float scale;
	} transaxis[6];
};

/* Convert a win32 Multimedia API return code to a text message */
static void SetMMerror(char *function, int code);


static char *GetJoystickName(int index, const char *szRegKey)
{
	/* added 7/24/2004 by Eckhard Stolberg */
	/*
		see if there is a joystick for the current
		index (1-16) listed in the registry
	*/
	char *name = NULL;
	HKEY hKey;
	DWORD regsize;
	LONG regresult;
	unsigned char regkey[256];
	unsigned char regvalue[256];
	unsigned char regname[256];

	sprintf(regkey, "%s\\%s\\%s",
		REGSTR_PATH_JOYCONFIG,
		szRegKey,
		REGSTR_KEY_JOYCURR);
	regresult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		(LPTSTR) &regkey, 0, KEY_READ, &hKey);
	if (regresult == ERROR_SUCCESS)
	{
		/*
			find the registry key name for the
			joystick's properties
		*/
		regsize = sizeof(regname);
		sprintf(regvalue,
			"Joystick%d%s", index+1,
			REGSTR_VAL_JOYOEMNAME);
		regresult = RegQueryValueExA(hKey,
			regvalue, 0, 0, (LPBYTE) &regname,
			(LPDWORD) &regsize);
		RegCloseKey(hKey);
		if (regresult == ERROR_SUCCESS)
		{
			/* open that registry key */
			sprintf(regkey, "%s\\%s",
				REGSTR_PATH_JOYOEM, regname);
			regresult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
				regkey, 0, KEY_READ, &hKey);
			if (regresult == ERROR_SUCCESS)
			{
				/* find the size for the OEM name text */
				regsize = sizeof(regvalue);
				regresult =
					RegQueryValueExA(hKey,
					REGSTR_VAL_JOYOEMNAME,
					0, 0, NULL,
					(LPDWORD) &regsize);
				if (regresult == ERROR_SUCCESS)
				{
					/*
						allocate enough memory
						for the OEM name text ...
					*/
					name = (char *) malloc(regsize);
					/* ... and read it from the registry */
					regresult =
						RegQueryValueExA(hKey,
						REGSTR_VAL_JOYOEMNAME, 0, 0,
						(LPBYTE) name,
						(LPDWORD) &regsize);
					RegCloseKey(hKey);
				}
			}
		}
	}
	return(name);
}

/* Function to scan the system for joysticks.
 * This function should set SDL_numjoysticks to the number of available
 * joysticks.  Joystick 0 should be the system default joystick.
 * It should return 0, or -1 on an unrecoverable fatal error.
 */
int SDL_SYS_JoystickInit(void)
{
	int	i;
	int maxdevs;
	int numdevs;
	JOYINFOEX joyinfo;
	JOYCAPS	joycaps;
	MMRESULT result;

	numdevs = 0;
	maxdevs = joyGetNumDevs();

	if ( maxdevs > MAX_JOYSTICKS ) {
		maxdevs = MAX_JOYSTICKS;
	}


	for ( i = 0; i < MAX_JOYSTICKS; i++ ) {
		SYS_JoystickID[i] = JOYSTICKID1 + i;
		SYS_JoystickName[i] = NULL;
	}


	for ( i = 0; (i < maxdevs); ++i ) {
		
		/* added 8/31/2001 By Vitaliy Mikitchenko */
		joyinfo.dwSize = sizeof(joyinfo);
		joyinfo.dwFlags = JOY_RETURNALL;
		/* end addition */

		result = joyGetPosEx(SYS_JoystickID[i], &joyinfo);
		if ( result == JOYERR_NOERROR ) {
			result = joyGetDevCaps(SYS_JoystickID[i], &joycaps, sizeof(joycaps));
			if ( result == JOYERR_NOERROR ) {
				SYS_JoystickID[numdevs] = SYS_JoystickID[i];
				SYS_Joystick[numdevs] = joycaps;
				SYS_JoystickName[numdevs] = GetJoystickName(numdevs, joycaps.szRegKey);
				numdevs++;
			}
		}
	}
	return(numdevs);
}

/* Function to get the device-dependent name of a joystick */
const char *SDL_SYS_JoystickName(int index)
{
	if ( SYS_JoystickName[index] != NULL ) {
		return(SYS_JoystickName[index]);
	} else {
		return(SYS_Joystick[index].szPname);
	}
}

/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */
int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	int index, i;
	int caps_flags[MAX_AXES-2] =
		{ JOYCAPS_HASZ, JOYCAPS_HASR, JOYCAPS_HASU, JOYCAPS_HASV };
	int axis_min[MAX_AXES], axis_max[MAX_AXES];


	/* shortcut */
	index = joystick->index;
	axis_min[0] = SYS_Joystick[index].wXmin;
	axis_max[0] = SYS_Joystick[index].wXmax;
	axis_min[1] = SYS_Joystick[index].wYmin;
	axis_max[1] = SYS_Joystick[index].wYmax;
	axis_min[2] = SYS_Joystick[index].wZmin;
	axis_max[2] = SYS_Joystick[index].wZmax;
	axis_min[3] = SYS_Joystick[index].wRmin;
	axis_max[3] = SYS_Joystick[index].wRmax;
	axis_min[4] = SYS_Joystick[index].wUmin;
	axis_max[4] = SYS_Joystick[index].wUmax;
	axis_min[5] = SYS_Joystick[index].wVmin;
	axis_max[5] = SYS_Joystick[index].wVmax;

	/* allocate memory for system specific hardware data */
	joystick->hwdata = (struct joystick_hwdata *) malloc(sizeof(*joystick->hwdata));
	if (joystick->hwdata == NULL)
	{
		SDL_OutOfMemory();
		return(-1);
	}
	memset(joystick->hwdata, 0, sizeof(*joystick->hwdata));

	/* set hardware data */
	joystick->hwdata->id = SYS_JoystickID[index];
	for ( i = 0; i < MAX_AXES; ++i ) {
		if ( (i<2) || (SYS_Joystick[index].wCaps & caps_flags[i-2]) ) {
			joystick->hwdata->transaxis[i].offset =
				AXIS_MIN - axis_min[i];
			joystick->hwdata->transaxis[i].scale =
				(float)(AXIS_MAX - AXIS_MIN) / (axis_max[i] - axis_min[i]);
		} else {
			joystick->hwdata->transaxis[i].offset = 0;
			joystick->hwdata->transaxis[i].scale = 1.0; /* Just in case */
		}
	}

	/* fill nbuttons, naxes, and nhats fields */
	joystick->nbuttons = SYS_Joystick[index].wNumButtons;
	joystick->naxes = SYS_Joystick[index].wNumAxes;
	if ( SYS_Joystick[index].wCaps & JOYCAPS_HASPOV ) {
		joystick->nhats = 1;
	} else {
		joystick->nhats = 0;
	}
	return(0);
}

static Uint8 TranslatePOV(DWORD value)
{
	Uint8 pos;

	pos = SDL_HAT_CENTERED;
	if ( value != JOY_POVCENTERED ) {
		if ( (value > JOY_POVLEFT) || (value < JOY_POVRIGHT) ) {
			pos |= SDL_HAT_UP;
		}
		if ( (value > JOY_POVFORWARD) && (value < JOY_POVBACKWARD) ) {
			pos |= SDL_HAT_RIGHT;
		}
		if ( (value > JOY_POVRIGHT) && (value < JOY_POVLEFT) ) {
			pos |= SDL_HAT_DOWN;
		}
		if ( value > JOY_POVBACKWARD ) {
			pos |= SDL_HAT_LEFT;
		}
	}
	return(pos);
}

/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */
void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
	MMRESULT result;
	int i;
	DWORD flags[MAX_AXES] = { JOY_RETURNX, JOY_RETURNY, JOY_RETURNZ, 
				  JOY_RETURNR, JOY_RETURNU, JOY_RETURNV };
	DWORD pos[MAX_AXES];
	struct _transaxis *transaxis;
	int value, change;
	JOYINFOEX joyinfo;

	joyinfo.dwSize = sizeof(joyinfo);
	joyinfo.dwFlags = JOY_RETURNALL|JOY_RETURNPOVCTS;
	if ( ! joystick->hats ) {
		joyinfo.dwFlags &= ~(JOY_RETURNPOV|JOY_RETURNPOVCTS);
	}
	result = joyGetPosEx(joystick->hwdata->id, &joyinfo);
	if ( result != JOYERR_NOERROR ) {
		SetMMerror("joyGetPosEx", result);
		return;
	}

	/* joystick motion events */
	pos[0] = joyinfo.dwXpos;
	pos[1] = joyinfo.dwYpos;
	pos[2] = joyinfo.dwZpos;
	pos[3] = joyinfo.dwRpos;
	pos[4] = joyinfo.dwUpos;
	pos[5] = joyinfo.dwVpos;

	transaxis = joystick->hwdata->transaxis;
	for (i = 0; i < joystick->naxes; i++) {
		if (joyinfo.dwFlags & flags[i]) {
			value = (int)(((float)pos[i] + transaxis[i].offset) * transaxis[i].scale);
			change = (value - joystick->axes[i]);
			if ( (change < -JOY_AXIS_THRESHOLD) || (change > JOY_AXIS_THRESHOLD) ) {
				SDL_PrivateJoystickAxis(joystick, (Uint8)i, (Sint16)value);
			}
		}
	}

	/* joystick button events */
	if ( joyinfo.dwFlags & JOY_RETURNBUTTONS ) {
		for ( i = 0; i < joystick->nbuttons; ++i ) {
			if ( joyinfo.dwButtons & JOY_BUTTON_FLAG(i) ) {
				if ( ! joystick->buttons[i] ) {
					SDL_PrivateJoystickButton(joystick, (Uint8)i, SDL_PRESSED);
				}
			} else {
				if ( joystick->buttons[i] ) {
					SDL_PrivateJoystickButton(joystick, (Uint8)i, SDL_RELEASED);
				}
			}
		}
	}

	/* joystick hat events */
	if ( joyinfo.dwFlags & JOY_RETURNPOV ) {
		Uint8 pos;

		pos = TranslatePOV(joyinfo.dwPOV);
		if ( pos != joystick->hats[0] ) {
			SDL_PrivateJoystickHat(joystick, 0, pos);
		}
	}
}

/* Function to close a joystick after use */
void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
	if (joystick->hwdata != NULL) {
		/* free system specific hardware data */
		free(joystick->hwdata);
	}
}

/* Function to perform any system-specific joystick related cleanup */
void SDL_SYS_JoystickQuit(void)
{
	int i;
	for (i = 0; i < MAX_JOYSTICKS; i++) {
		if ( SYS_JoystickName[i] != NULL ) {
			free(SYS_JoystickName[i]);
		}
	}
}


/* implementation functions */
void SetMMerror(char *function, int code)
{
	static char *error;
	static char  errbuf[BUFSIZ];

	errbuf[0] = 0;
	switch (code) 
	{
		case MMSYSERR_NODRIVER:
			error = "Joystick driver not present";
		break;

		case MMSYSERR_INVALPARAM:
		case JOYERR_PARMS:
			error = "Invalid parameter(s)";
		break;
		
		case MMSYSERR_BADDEVICEID:
			error = "Bad device ID";
		break;

		case JOYERR_UNPLUGGED:
			error = "Joystick not attached";
		break;

		case JOYERR_NOCANDO:
			error = "Can't capture joystick input";
		break;

		default:
			sprintf(errbuf, "%s: Unknown Multimedia system error: 0x%x",
								function, code);
		break;
	}

	if ( ! errbuf[0] ) {
		sprintf(errbuf, "%s: %s", function, error);
	}
	SDL_SetError("%s", errbuf);
}
#pragma mark >end:joystick/win32/SDL_mmjoystick.c

#else

#pragma mark >begin:joystick/linux/SDL_sysjoystick.c

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
 "@(#) $Id: util_hid.c,v 1.13 2006/04/24 04:15:15 gw-chuck Exp $";
#endif

/* This is the system specific header for the SDL joystick API */

#include <stdio.h>		/* For the definition of NULL */
#include <stdlib.h>		/* For getenv() prototype */
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <limits.h>		/* For the definition of PATH_MAX */
#ifdef __arm__
#include <linux/limits.h> /* Arm cross-compiler needs this */
#endif
#include <linux/joystick.h>
#ifdef USE_INPUT_EVENTS
#include <linux/input.h>
#endif


/* Special joystick configurations */
static struct {
	const char *name;
	int naxes;
	int nhats;
	int nballs;
} special_joysticks[] = {
	{ "MadCatz Panther XL", 3, 2, 1 }, /* We don't handle rudder (axis 8) */
	{ "SideWinder Precision Pro", 4, 1, 0 },
	{ "SideWinder 3D Pro", 4, 1, 0 },
	{ "Microsoft SideWinder 3D Pro", 4, 1, 0 },
	{ "Microsoft SideWinder Dual Strike USB version 1.0", 2, 1, 0 },
	{ "WingMan Interceptor", 3, 3, 0 },
	{ "WingMan Extreme Digital 3D", 4, 1, 0 },
	{ "Microsoft SideWinder Precision 2 Joystick", 4, 1, 0 },
	{ "Logitech Inc. WingMan Extreme Digital 3D", 4, 1, 0 },
	{ "Saitek Saitek X45", 6, 1, 0 }
};

#ifndef NO_LOGICAL_JOYSTICKS

static struct joystick_logical_values {
        int njoy;
        int nthing;
} joystick_logical_values[] = {

/* +0 */
   /* MP-8800 axes map - map to {logical joystick #, logical axis #} */
   {0,0},{0,1},{0,2},{1,0},{1,1},{0,3},{1,2},{1,3},{2,0},{2,1},{2,2},{2,3},
   {3,0},{3,1},{3,2},{3,3},{0,4},{1,4},{2,4},

/* +19 */
   /* MP-8800 hat map - map to {logical joystick #, logical hat #} */
   {0,0},{1,0},{2,0},{3,0},

/* +23 */
   /* MP-8800 button map - map to {logical joystick #, logical button #} */
   {0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8},{0,9},{0,10},{0,11},
   {1,0},{1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{1,8},{1,9},{1,10},{1,11},
   {2,0},{2,1},{2,2},{2,3},{2,4},{2,5},{2,6},{2,7},{2,8},{2,9},{2,10},{2,11},
   {3,0},{3,1},{3,2},{3,3},{3,4},{3,5},{3,6},{3,7},{3,8},{3,9},{3,10},{3,11}
};

static struct joystick_logical_layout {
        int naxes;
        int nhats;
        int nballs;
        int nbuttons;
} joystick_logical_layout[] = {
        /* MP-8800 logical layout */
        {5, 1, 0, 12},
        {5, 1, 0, 12},
        {5, 1, 0, 12},
        {4, 1, 0, 12}
};

/*
   Some USB HIDs show up as a single joystick even though they actually
   control 2 or more joysticks.  This array sets up a means of mapping
   a single physical joystick to multiple logical joysticks. (djm)
                                                                                
   njoys
        the number of logical joysticks
                                                                                
   layouts
        an array of layout structures, one to describe each logical joystick
                                                                                
   axes, hats, balls, buttons
        arrays that map a physical thingy to a logical thingy
 */
static struct joystick_logicalmap {
        const char *name;
        int njoys;
        struct joystick_logical_layout *layouts;
        struct joystick_logical_values *axes;
        struct joystick_logical_values *hats;
        struct joystick_logical_values *balls;
        struct joystick_logical_values *buttons;

} joystick_logicalmap[] = {
        {"WiseGroup.,Ltd MP-8800 Quad USB Joypad", 4, joystick_logical_layout,
         joystick_logical_values, joystick_logical_values+19, NULL,
         joystick_logical_values+23}
};

/* find the head of a linked list, given a point in it
 */
#define SDL_joylist_head(i, start)\
        for(i = start; SDL_joylist[i].fname == NULL;) i = SDL_joylist[i].prev;

#define SDL_logical_joydecl(d) d


#else

#define SDL_logical_joydecl(d)

#endif /* USE_LOGICAL_JOYSTICKS */

/* The maximum number of joysticks we'll detect */
#define MAX_JOYSTICKS	32

/* A list of available joysticks */
static struct
{
        char* fname;
#ifndef NO_LOGICAL_JOYSTICKS
        SDL_Joystick* joy;
        struct joystick_logicalmap* map;
        int prev;
        int next;
        int logicalno;
#endif /* USE_LOGICAL_JOYSTICKS */
} SDL_joylist[MAX_JOYSTICKS];


/* The private structure used to keep track of a joystick */
struct joystick_hwdata {
	int fd;
	/* The current linux joystick driver maps hats to two axes */
	struct hwdata_hat {
		int axis[2];
	} *hats;
	/* The current linux joystick driver maps balls to two axes */
	struct hwdata_ball {
		int axis[2];
	} *balls;

	/* Support for the Linux 2.4 unified input interface */
#ifdef USE_INPUT_EVENTS
	SDL_bool is_hid;
	Uint8 key_map[KEY_MAX-BTN_MISC];
	Uint8 abs_map[ABS_MAX];
	struct axis_correct {
		int used;
		int coef[3];
	} abs_correct[ABS_MAX];
#endif
};

static char *mystrdup(const char *string)
{
	char *newstring;

	newstring = (char *)malloc(strlen(string)+1);
	if ( newstring ) {
		strcpy(newstring, string);
	}
	return(newstring);
}


#ifndef NO_LOGICAL_JOYSTICKS

static int CountLogicalJoysticks(int max)
{
   register int i, j, k, ret, prev;
   const char* name;

   ret = 0;

   for(i = 0; i < max; i++) {
      name = SDL_SYS_JoystickName(i);

      if (name) {
         for(j = 0; j < SDL_TABLESIZE(joystick_logicalmap); j++) {
            if (!strcmp(name, joystick_logicalmap[j].name)) {

               prev = i;
               SDL_joylist[prev].map = joystick_logicalmap+j;

               for(k = 1; k < joystick_logicalmap[j].njoys; k++) {
                  SDL_joylist[prev].next = max + ret;

                  if (prev != i)
                     SDL_joylist[max+ret].prev = prev;

                  prev = max + ret;
                  SDL_joylist[prev].logicalno = k;
                  SDL_joylist[prev].map = joystick_logicalmap+j;
                  ret++;
               }

               break;
            }
         }
      }
   }

   return ret;
}

static void LogicalSuffix(int logicalno, char* namebuf, int len)
{
   register int slen;
   const static char suffixs[] =
      "01020304050607080910111213141516171819"
      "20212223242526272829303132";
   const char* suffix;

   slen = strlen(namebuf);

   suffix = NULL;

   if (logicalno*2<sizeof(suffixs))
      suffix = suffixs + (logicalno*2);

   if (slen + 4 < len && suffix) {
      namebuf[slen++] = ' ';
      namebuf[slen++] = '#';
      namebuf[slen++] = suffix[0];
      namebuf[slen++] = suffix[1];
      namebuf[slen++] = 0;
   }
}

#endif /* USE_LOGICAL_JOYSTICKS */

#ifdef USE_INPUT_EVENTS
#define test_bit(nr, addr) \
	(((1UL << ((nr) & 31)) & (((const unsigned int *) addr)[(nr) >> 5])) != 0)

static int EV_IsJoystick(int fd)
{
	unsigned long evbit[40];
	unsigned long keybit[40];
	unsigned long absbit[40];

	if ( (ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), evbit) < 0) ||
	     (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) < 0) ||
	     (ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absbit)), absbit) < 0) ) {
		return(0);
	}
	if (!(test_bit(EV_KEY, evbit) && test_bit(EV_ABS, evbit) &&
	      test_bit(ABS_X, absbit) && test_bit(ABS_Y, absbit) &&
	     (test_bit(BTN_TRIGGER, keybit) || test_bit(BTN_A, keybit) || test_bit(BTN_1, keybit)))) return 0;
	return(1);
}

#endif /* USE_INPUT_EVENTS */

/* Function to scan the system for joysticks */
int SDL_SYS_JoystickInit(void)
{
	/* The base path of the joystick devices */
	const char *joydev_pattern[] = {
#ifdef USE_INPUT_EVENTS
		"/dev/input/event%d",
#endif
		"/dev/input/js%d",
		"/dev/js%d"
	};
	int numjoysticks;
	int i, j;
	int fd;
	char path[PATH_MAX];
	dev_t dev_nums[MAX_JOYSTICKS];  /* major/minor device numbers */
	struct stat sb;
	int n, duplicate;

	numjoysticks = 0;

	/* First see if the user specified a joystick to use */
	if ( getenv("SDL_JOYSTICK_DEVICE") != NULL ) {
		strncpy(path, getenv("SDL_JOYSTICK_DEVICE"), sizeof(path));
		path[sizeof(path)-1] = '\0';
		if ( stat(path, &sb) == 0 ) {
			fd = open(path, O_RDONLY, 0);
			if ( fd >= 0 ) {
				/* Assume the user knows what they're doing. */
				SDL_joylist[numjoysticks].fname =mystrdup(path);
				if ( SDL_joylist[numjoysticks].fname ) {
					dev_nums[numjoysticks] = sb.st_rdev;
					++numjoysticks;
				}
				close(fd);
			}
		}
	}

	for ( i=0; i<SDL_TABLESIZE(joydev_pattern); ++i ) {
		for ( j=0; j < MAX_JOYSTICKS; ++j ) {
			sprintf(path, joydev_pattern[i], j);

			/* rcg06302000 replaced access(F_OK) call with stat().
			 * stat() will fail if the file doesn't exist, so it's
			 * equivalent behaviour.
			 */
			if ( stat(path, &sb) == 0 ) {
				/* Check to make sure it's not already in list.
				 * This happens when we see a stick via symlink.
				 */
				duplicate = 0;
				for (n=0; (n<numjoysticks) && !duplicate; ++n) {
					if ( sb.st_rdev == dev_nums[n] ) {
						duplicate = 1;
					}
				}
				if (duplicate) {
					continue;
				}

				fd = open(path, O_RDONLY, 0);
				if ( fd < 0 ) {
					continue;
				}
#ifdef USE_INPUT_EVENTS
#ifdef DEBUG_INPUT_EVENTS
				printf("Checking %s\n", path);
#endif
				if ( (i == 0) && ! EV_IsJoystick(fd) ) {
					close(fd);
					continue;
				}
#endif
				close(fd);

				/* We're fine, add this joystick */
				SDL_joylist[numjoysticks].fname =mystrdup(path);
				if ( SDL_joylist[numjoysticks].fname ) {
					dev_nums[numjoysticks] = sb.st_rdev;
					++numjoysticks;
				}
			} else
				break;
		}

#ifdef USE_INPUT_EVENTS
		/* This is a special case...
		   If the event devices are valid then the joystick devices
		   will be duplicates but without extra information about their
		   hats or balls. Unfortunately, the event devices can't
		   currently be calibrated, so it's a win-lose situation.
		   So : /dev/input/eventX = /dev/input/jsY = /dev/jsY
		*/
		if ( (i == 0) && (numjoysticks > 0) )
			break;
#endif
	}
#ifndef NO_LOGICAL_JOYSTICKS
	numjoysticks += CountLogicalJoysticks(numjoysticks);
#endif

	return(numjoysticks);
}

/* Function to get the device-dependent name of a joystick */
const char *SDL_SYS_JoystickName(int index)
{
	int fd;
	static char namebuf[128];
	char *name;
	SDL_logical_joydecl(int oindex = index);

#ifndef NO_LOGICAL_JOYSTICKS
	SDL_joylist_head(index, index);
#endif
	name = NULL;
	fd = open(SDL_joylist[index].fname, O_RDONLY, 0);
	if ( fd >= 0 ) {
		if ( 
#ifdef USE_INPUT_EVENTS
		     (ioctl(fd, EVIOCGNAME(sizeof(namebuf)), namebuf) <= 0) &&
#endif
		     (ioctl(fd, JSIOCGNAME(sizeof(namebuf)), namebuf) <= 0) ) {
			name = SDL_joylist[index].fname;
		} else {
			name = namebuf;
		}
		close(fd);

#ifndef NO_LOGICAL_JOYSTICKS
		if (SDL_joylist[oindex].prev || SDL_joylist[oindex].next)
       		   LogicalSuffix(SDL_joylist[oindex].logicalno, namebuf, 128);
#endif
	}
	return name;
}

static int allocate_hatdata(SDL_Joystick *joystick)
{
	int i;

	joystick->hwdata->hats = (struct hwdata_hat *)malloc(
		joystick->nhats * sizeof(struct hwdata_hat));
	if ( joystick->hwdata->hats == NULL ) {
		return(-1);
	}
	for ( i=0; i<joystick->nhats; ++i ) {
		joystick->hwdata->hats[i].axis[0] = 1;
		joystick->hwdata->hats[i].axis[1] = 1;
	}
	return(0);
}

static int allocate_balldata(SDL_Joystick *joystick)
{
	int i;

	joystick->hwdata->balls = (struct hwdata_ball *)malloc(
		joystick->nballs * sizeof(struct hwdata_ball));
	if ( joystick->hwdata->balls == NULL ) {
		return(-1);
	}
	for ( i=0; i<joystick->nballs; ++i ) {
		joystick->hwdata->balls[i].axis[0] = 0;
		joystick->hwdata->balls[i].axis[1] = 0;
	}
	return(0);
}

static SDL_bool JS_ConfigJoystick(SDL_Joystick *joystick, int fd)
{
	SDL_bool handled;
	unsigned char n;
	int old_axes, tmp_naxes, tmp_nhats, tmp_nballs;
	const char *name;
	char *env, env_name[128];
	int i;

	handled = SDL_FALSE;

	/* Default joystick device settings */
	if ( ioctl(fd, JSIOCGAXES, &n) < 0 ) {
		joystick->naxes = 2;
	} else {
		joystick->naxes = n;
	}
	if ( ioctl(fd, JSIOCGBUTTONS, &n) < 0 ) {
		joystick->nbuttons = 2;
	} else {
		joystick->nbuttons = n;
	}

	name = SDL_SYS_JoystickName(joystick->index);
	old_axes = joystick->naxes;

	/* Generic analog joystick support */
	if ( strstr(name, "Analog") == name && strstr(name, "-hat") ) {
		if ( sscanf(name,"Analog %d-axis %*d-button %d-hat",
			&tmp_naxes, &tmp_nhats) == 2 ) {

			joystick->naxes = tmp_naxes;
			joystick->nhats = tmp_nhats;

			handled = SDL_TRUE;
		}
	}

	/* Special joystick support */
	for ( i=0; i < SDL_TABLESIZE(special_joysticks); ++i ) {
		if ( strcmp(name, special_joysticks[i].name) == 0 ) {

			joystick->naxes = special_joysticks[i].naxes;
			joystick->nhats = special_joysticks[i].nhats;
			joystick->nballs = special_joysticks[i].nballs;

			handled = SDL_TRUE;
			break;
		}
	}

	/* User environment joystick support */
	if ( (env = getenv("SDL_LINUX_JOYSTICK")) ) {
		strcpy(env_name, "");
		if ( *env == '\'' && sscanf(env, "'%[^']s'", env_name) == 1 )
			env += strlen(env_name)+2;
		else if ( sscanf(env, "%s", env_name) == 1 )
			env += strlen(env_name);

		if ( strcmp(name, env_name) == 0 ) {

			if ( sscanf(env, "%d %d %d", &tmp_naxes, &tmp_nhats,
				&tmp_nballs) == 3 ) {

				joystick->naxes = tmp_naxes;
				joystick->nhats = tmp_nhats;
				joystick->nballs = tmp_nballs;

				handled = SDL_TRUE;
			}
		}
	}

	/* Remap hats and balls */
	if (handled) {
		if ( joystick->nhats > 0 ) {
			if ( allocate_hatdata(joystick) < 0 ) {
				joystick->nhats = 0;
			}
		}
		if ( joystick->nballs > 0 ) {
			if ( allocate_balldata(joystick) < 0 ) {
				joystick->nballs = 0;
			}
		}
	}

	return(handled);
}

#ifdef USE_INPUT_EVENTS

static SDL_bool EV_ConfigJoystick(SDL_Joystick *joystick, int fd)
{
	int i, t;
	unsigned long keybit[40];
	unsigned long absbit[40];
	unsigned long relbit[40];

	/* See if this device uses the new unified event API */
	if ( (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) >= 0) &&
	     (ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absbit)), absbit) >= 0) &&
	     (ioctl(fd, EVIOCGBIT(EV_REL, sizeof(relbit)), relbit) >= 0) ) {
		joystick->hwdata->is_hid = SDL_TRUE;

		/* Get the number of buttons, axes, and other thingamajigs */
		for ( i=BTN_JOYSTICK; i < KEY_MAX; ++i ) {
			if ( test_bit(i, keybit) ) {
#ifdef DEBUG_INPUT_EVENTS
				printf("Joystick has button: 0x%x\n", i);
#endif
				joystick->hwdata->key_map[i-BTN_MISC] =
						joystick->nbuttons;
				++joystick->nbuttons;
			}
		}
		for ( i=BTN_MISC; i < BTN_JOYSTICK; ++i ) {
			if ( test_bit(i, keybit) ) {
#ifdef DEBUG_INPUT_EVENTS
				printf("Joystick has button: 0x%x\n", i);
#endif
				joystick->hwdata->key_map[i-BTN_MISC] =
						joystick->nbuttons;
				++joystick->nbuttons;
			}
		}
		for ( i=0; i<ABS_MAX; ++i ) {
			/* Skip hats */
			if ( i == ABS_HAT0X ) {
				i = ABS_HAT3Y;
				continue;
			}
			if ( test_bit(i, absbit) ) {
				int values[5];

				if ( ioctl(fd, EVIOCGABS(i), values) < 0 )
					continue;
#ifdef DEBUG_INPUT_EVENTS
				printf("Joystick has absolute axis: %x\n", i);
				printf("Values = { %d, %d, %d, %d, %d }\n",
					values[0], values[1],
					values[2], values[3], values[4]);
#endif /* DEBUG_INPUT_EVENTS */
				joystick->hwdata->abs_map[i] = joystick->naxes;
				if ( values[1] == values[2] ) {
				    joystick->hwdata->abs_correct[i].used = 0;
				} else {
				    joystick->hwdata->abs_correct[i].used = 1;
				    joystick->hwdata->abs_correct[i].coef[0] =
					(values[2] + values[1]) / 2 - values[4];
				    joystick->hwdata->abs_correct[i].coef[1] =
					(values[2] + values[1]) / 2 + values[4];
				    t = ((values[2] - values[1]) / 2 - 2 * values[4]);
				    if ( t != 0 ) {
					joystick->hwdata->abs_correct[i].coef[2] = (1 << 29) / t;
				    } else {
					joystick->hwdata->abs_correct[i].coef[2] = 0;
				    }
				}
				++joystick->naxes;
			}
		}
		for ( i=ABS_HAT0X; i <= ABS_HAT3Y; i += 2 ) {
			if ( test_bit(i, absbit) || test_bit(i+1, absbit) ) {
#ifdef DEBUG_INPUT_EVENTS
				printf("Joystick has hat %d\n",(i-ABS_HAT0X)/2);
#endif
				++joystick->nhats;
			}
		}
		if ( test_bit(REL_X, relbit) || test_bit(REL_Y, relbit) ) {
			++joystick->nballs;
		}

		/* Allocate data to keep track of these thingamajigs */
		if ( joystick->nhats > 0 ) {
			if ( allocate_hatdata(joystick) < 0 ) {
				joystick->nhats = 0;
			}
		}
		if ( joystick->nballs > 0 ) {
			if ( allocate_balldata(joystick) < 0 ) {
				joystick->nballs = 0;
			}
		}
	}
	return(joystick->hwdata->is_hid);
}

#endif /* USE_INPUT_EVENTS */

#ifndef NO_LOGICAL_JOYSTICKS
static void ConfigLogicalJoystick(SDL_Joystick *joystick)
{
        struct joystick_logical_layout* layout;
                                                                                
        layout = SDL_joylist[joystick->index].map->layouts +
                SDL_joylist[joystick->index].logicalno;
                                                                                
        joystick->nbuttons = layout->nbuttons;
        joystick->nhats = layout->nhats;
        joystick->naxes = layout->naxes;
        joystick->nballs = layout->nballs;
}
#endif


/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */
int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	int fd;
	SDL_logical_joydecl(int realindex);
	SDL_logical_joydecl(SDL_Joystick *realjoy = NULL);

	/* Open the joystick and set the joystick file descriptor */
#ifndef NO_LOGICAL_JOYSTICKS
	if (SDL_joylist[joystick->index].fname == NULL) {
		SDL_joylist_head(realindex, joystick->index);
		realjoy = SDL_JoystickOpen(realindex);

		if (realjoy == NULL)
			return(-1);
                                                                                
		fd = realjoy->hwdata->fd;

	} else {
		fd = open(SDL_joylist[joystick->index].fname, O_RDONLY, 0);
	}
	SDL_joylist[joystick->index].joy = joystick;
#else
	fd = open(SDL_joylist[joystick->index].fname, O_RDONLY, 0);
#endif

	if ( fd < 0 ) {
		SDL_SetError("Unable to open %s\n",
		             SDL_joylist[joystick->index]);
		return(-1);
	}
	joystick->hwdata = (struct joystick_hwdata *)
	                   malloc(sizeof(*joystick->hwdata));
	if ( joystick->hwdata == NULL ) {
		SDL_OutOfMemory();
		close(fd);
		return(-1);
	}
	memset(joystick->hwdata, 0, sizeof(*joystick->hwdata));
	joystick->hwdata->fd = fd;

	/* Set the joystick to non-blocking read mode */
	fcntl(fd, F_SETFL, O_NONBLOCK);

	/* Get the number of buttons and axes on the joystick */
#ifndef NO_LOGICAL_JOYSTICKS
	if (realjoy)
		ConfigLogicalJoystick(joystick);
	else
#endif
#ifdef USE_INPUT_EVENTS
	if ( ! EV_ConfigJoystick(joystick, fd) )
#endif
		JS_ConfigJoystick(joystick, fd);

	return(0);
}

#ifndef NO_LOGICAL_JOYSTICKS

static SDL_Joystick* FindLogicalJoystick(
   SDL_Joystick *joystick, struct joystick_logical_values* v)
{
        SDL_Joystick *logicaljoy;
        register int i;

        i = joystick->index;
        logicaljoy = NULL;

        /* get the fake joystick that will receive the event
         */
        for(;;) {

           if (SDL_joylist[i].logicalno == v->njoy) {
              logicaljoy = SDL_joylist[i].joy;
              break;
           }

           if (SDL_joylist[i].next == 0)
              break;

           i = SDL_joylist[i].next;

        }

        return logicaljoy;
}

static int LogicalJoystickButton(
   SDL_Joystick *joystick, Uint8 button, Uint8 state){
        struct joystick_logical_values* buttons;
        SDL_Joystick *logicaljoy = NULL;

        /* if there's no map then this is just a regular joystick
         */
        if (SDL_joylist[joystick->index].map == NULL)
           return 0;

        /* get the logical joystick that will receive the event
         */
        buttons = SDL_joylist[joystick->index].map->buttons+button;
        logicaljoy = FindLogicalJoystick(joystick, buttons);

        if (logicaljoy == NULL)
           return 1;

        SDL_PrivateJoystickButton(logicaljoy, buttons->nthing, state);

        return 1;
}

static int LogicalJoystickAxis(
	SDL_Joystick *joystick, Uint8 axis, Sint16 value)
{
        struct joystick_logical_values* axes;
        SDL_Joystick *logicaljoy = NULL;

        /* if there's no map then this is just a regular joystick
         */
        if (SDL_joylist[joystick->index].map == NULL)
           return 0;

        /* get the logical joystick that will receive the event
         */
        axes = SDL_joylist[joystick->index].map->axes+axis;
        logicaljoy = FindLogicalJoystick(joystick, axes);

        if (logicaljoy == NULL)
           return 1;

        SDL_PrivateJoystickAxis(logicaljoy, axes->nthing, value);

        return 1;
}
#endif /* USE_LOGICAL_JOYSTICKS */

static __inline__
void HandleHat(SDL_Joystick *stick, Uint8 hat, int axis, int value)
{
	struct hwdata_hat *the_hat;
	const Uint8 position_map[3][3] = {
		{ SDL_HAT_LEFTUP, SDL_HAT_UP, SDL_HAT_RIGHTUP },
		{ SDL_HAT_LEFT, SDL_HAT_CENTERED, SDL_HAT_RIGHT },
		{ SDL_HAT_LEFTDOWN, SDL_HAT_DOWN, SDL_HAT_RIGHTDOWN }
	};
	SDL_logical_joydecl(SDL_Joystick *logicaljoy = NULL);
	SDL_logical_joydecl(struct joystick_logical_values* hats = NULL);

	the_hat = &stick->hwdata->hats[hat];
	if ( value < 0 ) {
		value = 0;
	} else
	if ( value == 0 ) {
		value = 1;
	} else
	if ( value > 0 ) {
		value = 2;
	}
	if ( value != the_hat->axis[axis] ) {
		the_hat->axis[axis] = value;

#ifndef NO_LOGICAL_JOYSTICKS
		/* if there's no map then this is just a regular joystick
		*/
		if (SDL_joylist[stick->index].map != NULL) {

			/* get the fake joystick that will receive the event
			*/
			hats = SDL_joylist[stick->index].map->hats+hat;
			logicaljoy = FindLogicalJoystick(stick, hats);
		}

		if (logicaljoy) {
			stick = logicaljoy;
			hat = hats->nthing;
		}
#endif /* USE_LOGICAL_JOYSTICKS */

		SDL_PrivateJoystickHat(stick, hat,
			position_map[the_hat->axis[1]][the_hat->axis[0]]);
	}
}

static __inline__
void HandleBall(SDL_Joystick *stick, Uint8 ball, int axis, int value)
{
	stick->hwdata->balls[ball].axis[axis] += value;
}

/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */
static __inline__ void JS_HandleEvents(SDL_Joystick *joystick)
{
	struct js_event events[32];
	int i, len;
	Uint8 other_axis;

#ifndef NO_LOGICAL_JOYSTICKS
	if (SDL_joylist[joystick->index].fname == NULL) {
		SDL_joylist_head(i, joystick->index);
		return JS_HandleEvents(SDL_joylist[i].joy);
	}
#endif

	while ((len=read(joystick->hwdata->fd, events, (sizeof events))) > 0) {
		len /= sizeof(events[0]);
		for ( i=0; i<len; ++i ) {
			switch (events[i].type & ~JS_EVENT_INIT) {
			    case JS_EVENT_AXIS:
				if ( events[i].number < joystick->naxes ) {
#ifndef NO_LOGICAL_JOYSTICKS
					if (!LogicalJoystickAxis(joystick,
				           events[i].number, events[i].value))
#endif
					SDL_PrivateJoystickAxis(joystick,
				           events[i].number, events[i].value);
					break;
				}
				events[i].number -= joystick->naxes;
				other_axis = (events[i].number / 2);
				if ( other_axis < joystick->nhats ) {
					HandleHat(joystick, other_axis,
						events[i].number%2,
						events[i].value);
					break;
				}
				events[i].number -= joystick->nhats*2;
				other_axis = (events[i].number / 2);
				if ( other_axis < joystick->nballs ) {
					HandleBall(joystick, other_axis,
						events[i].number%2,
						events[i].value);
					break;
				}
				break;
			    case JS_EVENT_BUTTON:
#ifndef NO_LOGICAL_JOYSTICKS
				if (!LogicalJoystickButton(joystick,
				           events[i].number, events[i].value))
#endif
				SDL_PrivateJoystickButton(joystick,
				           events[i].number, events[i].value);
				break;
			    default:
				/* ?? */
				break;
			}
		}
	}
}
#ifdef USE_INPUT_EVENTS
static __inline__ int EV_AxisCorrect(SDL_Joystick *joystick, int which, int value)
{
	struct axis_correct *correct;

	correct = &joystick->hwdata->abs_correct[which];
	if ( correct->used ) {
		if ( value > correct->coef[0] ) {
			if ( value < correct->coef[1] ) {
				return 0;
			}
			value -= correct->coef[1];
		} else {
			value -= correct->coef[0];
		}
		value *= correct->coef[2];
		value >>= 14;
	}

	/* Clamp and return */
	if ( value < -32767 ) return -32767;
	if ( value >  32767 ) return  32767;

	return value;
}

static __inline__ void EV_HandleEvents(SDL_Joystick *joystick)
{
	struct input_event events[32];
	int i, len;
	int code;

#ifndef NO_LOGICAL_JOYSTICKS
	if (SDL_joylist[joystick->index].fname == NULL) {
		SDL_joylist_head(i, joystick->index);
		return EV_HandleEvents(SDL_joylist[i].joy);
	}
#endif

	while ((len=read(joystick->hwdata->fd, events, (sizeof events))) > 0) {
		len /= sizeof(events[0]);
		for ( i=0; i<len; ++i ) {
			code = events[i].code;
			switch (events[i].type) {
			    case EV_KEY:
				if ( code >= BTN_MISC ) {
					code -= BTN_MISC;
#ifndef NO_LOGICAL_JOYSTICKS
					if (!LogicalJoystickButton(joystick,
				           joystick->hwdata->key_map[code],
					   events[i].value))
#endif
					SDL_PrivateJoystickButton(joystick,
				           joystick->hwdata->key_map[code],
					   events[i].value);
				}
				break;
			    case EV_ABS:
				switch (code) {
				    case ABS_HAT0X:
				    case ABS_HAT0Y:
				    case ABS_HAT1X:
				    case ABS_HAT1Y:
				    case ABS_HAT2X:
				    case ABS_HAT2Y:
				    case ABS_HAT3X:
				    case ABS_HAT3Y:
					code -= ABS_HAT0X;
					HandleHat(joystick, code/2, code%2,
							events[i].value);
					break;
				    default:
					events[i].value = EV_AxisCorrect(joystick, code, events[i].value);
#ifndef NO_LOGICAL_JOYSTICKS
					if (!LogicalJoystickAxis(joystick,
				           joystick->hwdata->abs_map[code],
					   events[i].value))
#endif
					SDL_PrivateJoystickAxis(joystick,
				           joystick->hwdata->abs_map[code],
					   events[i].value);
					break;
				}
				break;
			    case EV_REL:
				switch (code) {
				    case REL_X:
				    case REL_Y:
					code -= REL_X;
					HandleBall(joystick, code/2, code%2,
							events[i].value);
					break;
				    default:
					break;
				}
				break;
			    default:
				break;
			}
		}
	}
}
#endif /* USE_INPUT_EVENTS */

void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
	int i;
	
#ifdef USE_INPUT_EVENTS
	if ( joystick->hwdata->is_hid )
		EV_HandleEvents(joystick);
	else
#endif
		JS_HandleEvents(joystick);

	/* Deliver ball motion updates */
	for ( i=0; i<joystick->nballs; ++i ) {
		int xrel, yrel;

		xrel = joystick->hwdata->balls[i].axis[0];
		yrel = joystick->hwdata->balls[i].axis[1];
		if ( xrel || yrel ) {
			joystick->hwdata->balls[i].axis[0] = 0;
			joystick->hwdata->balls[i].axis[1] = 0;
			SDL_PrivateJoystickBall(joystick, (Uint8)i, xrel, yrel);
		}
	}
}

/* Function to close a joystick after use */
void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
#ifndef NO_LOGICAL_JOYSTICKS
	register int i;
	if (SDL_joylist[joystick->index].fname == NULL) {
		SDL_joylist_head(i, joystick->index);
		SDL_JoystickClose(SDL_joylist[i].joy);
	}
#endif

	if ( joystick->hwdata ) {
#ifndef NO_LOGICAL_JOYSTICKS
		if (SDL_joylist[joystick->index].fname != NULL)
#endif
		close(joystick->hwdata->fd);
		if ( joystick->hwdata->hats ) {
			free(joystick->hwdata->hats);
		}
		if ( joystick->hwdata->balls ) {
			free(joystick->hwdata->balls);
		}
		free(joystick->hwdata);
		joystick->hwdata = NULL;
	}
}

/* Function to perform any system-specific joystick related cleanup */
void SDL_SYS_JoystickQuit(void)
{
	int i;

	for ( i=0; SDL_joylist[i].fname; ++i ) {
		free(SDL_joylist[i].fname);
	}
	SDL_joylist[0].fname = NULL;
}

#pragma mark >end:joystick/linux/SDL_sysjoystick.c


#endif /* __MACOSX_CORE__ */

#endif /* __CK_SDL_NATIVE__ */

/******************************************************************************
 *
 * coming up: mouse support.  this code will actually stick SDL_Events into the
 * SDL event queue.  SDL doesnt seem to support console-based mouse/kb
 * access.  Thus, a new thread will have to be spawned if the mouse code does
 * any polling.  
 *
 * author: spencer salazar
 * This is mostly lifted from Ryan Gordon's ManyMouse.  My modifications noted.
 *
 ******************************************************************************/

#ifdef __PLATFORM_MACOSX__

/*
 * Support for MacOS X via the HID Manager APIs.
 *
 * Please see the file LICENSE in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 *
 * Copyright (c) 2003 Ryan C. Gordon and others.
 * 
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software in a
 * product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 * 
 * Ryan C. Gordon <icculus@icculus.org>
 * 
 */

/* spencer commented this out */
/* #if ( (defined(__MACH__)) && (defined(__APPLE__)) ) */

/*
 * This source is almost entirely lifted from Apple's HID Utilities
 *  example source code, written by George Warner:
 *
 * http://developer.apple.com/samplecode/HID_Utilities_Source/HID_Utilities_Source.html
 *
 * The source license to HID Utilities allows this sort of blatant stealing.
 *
 * Patches to HID Utilities have comments like "ryan added this", otherwise,
 *  I just tried to cut down that package to the smallest set of functions
 *  I needed.
 *
 * Scroll down for "-- END HID UTILITIES --" to see the ManyMouse glue code.
 */
/* spencer added the #ifdef below */
#ifdef __CK_SDL_NATIVE__
#include <Carbon/Carbon.h>

#include <IOKit/IOTypes.h>
// 10.0.x
//#include <IOKit/IOUSBHIDParser.h>
// 10.1.x
#include <IOKit/hid/IOHIDUsageTables.h>
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/IOKitLib.h>
#endif /* __CK_SDL_NATIVE__ */
#include <IOKit/IOMessage.h>

#define USE_NOTIFICATIONS 1

#define HIDREPORTERRORNUM(s,n)	do {} while (false)
#define HIDREPORTERROR(s)		do {} while (false)

typedef enum HIDElementTypeMask
{
	kHIDElementTypeInput				= 1 << 1,
	kHIDElementTypeOutput            	= 1 << 2,
	kHIDElementTypeFeature           	= 1 << 3,
	kHIDElementTypeCollection        	= 1 << 4,
	kHIDElementTypeIO					= kHIDElementTypeInput | kHIDElementTypeOutput | kHIDElementTypeFeature,
	kHIDElementTypeAll					= kHIDElementTypeIO | kHIDElementTypeCollection
}HIDElementTypeMask;

enum
{
    kDefaultUserMin = 0,					// default user min and max used for scaling
    kDefaultUserMax = 255
};

enum
{
    kDeviceQueueSize = 50	// this is wired kernel memory so should be set to as small as possible
							// but should account for the maximum possible events in the queue
							// USB updates will likely occur at 100 Hz so one must account for this rate of
							// if states change quickly (updates are only posted on state changes)
};

struct mmRecElement
{
    unsigned long type;						// the type defined by IOHIDElementType in IOHIDKeys.h
    long usagePage;							// usage page from IOUSBHIDParser.h which defines general usage
    long usage;								// usage within above page from IOUSBHIDParser.h which defines specific usage
    void * cookie;				 			// unique value (within device of specific vendorID and productID) which identifies element, will NOT change
    long min;								// reported min value possible
    long max;								// reported max value possible
    long scaledMin;							// reported scaled min value possible
    long scaledMax;							// reported scaled max value possible
    long size;								// size in bits of data return from element
    unsigned char relative;					// are reports relative to last report (deltas)
    unsigned char wrapping;					// does element wrap around (one value higher than max is min)
    unsigned char nonLinear;				// are the values reported non-linear relative to element movement
    unsigned char preferredState;			// does element have a preferred state (such as a button)
    unsigned char nullState;				// does element have null state
    long units;								// units value is reported in (not used very often)
    long unitExp;							// exponent for units (also not used very often)
    char name[256];							// name of element (c string)
    
    // runtime variables
    long calMin; 							// min returned value
    long calMax; 							// max returned value (calibrate call)
    long userMin; 							// user set value to scale to (scale call)
    long userMax;							
    
	struct mmRecElement * pPrevious;			// previous element (NULL at list head)
    struct mmRecElement * pChild;				// next child (only of collections)
    struct mmRecElement * pSibling;			// next sibling (for elements and collections)
    
	long depth;
};
typedef struct mmRecElement mmRecElement;
typedef mmRecElement* pmmRecElement;

// ryan added this.
typedef enum
{
    DISCONNECT_CONNECTED,
    DISCONNECT_TELLUSER,
    DISCONNECT_COMPLETE
} DisconnectState;

struct mmRecDevice
{
    void * interface;						// interface to device, NULL = no interface
    void * queue;							// device queue, NULL = no queue
	void * queueRunLoopSource;				// device queue run loop source, NULL == no source
	void * transaction;						// output transaction interface, NULL == no interface
	void * notification;					// notifications
    char transport[256];					// device transport (c string)
    long vendorID;							// id for device vendor, unique across all devices
    long productID;							// id for particular product, unique across all of a vendors devices
    long version;							// version of product
    char manufacturer[256];					// name of manufacturer
    char product[256];						// name of product
    char serial[256];						// serial number of specific product, can be assumed unique across specific product or specific vendor (not used often)
    long locID;								// long representing location in USB (or other I/O) chain which device is pluged into, can identify specific device on machine
    long usage;								// usage page from IOUSBHID Parser.h which defines general usage
    long usagePage;							// usage within above page from IOUSBHID Parser.h which defines specific usage
    long totalElements;						// number of total elements (should be total of all elements on device including collections) (calculated, not reported by device)
	long features;							// number of elements of type kIOHIDElementTypeFeature
	long inputs;							// number of elements of type kIOHIDElementTypeInput_Misc or kIOHIDElementTypeInput_Button or kIOHIDElementTypeInput_Axis or kIOHIDElementTypeInput_ScanCodes
	long outputs;							// number of elements of type kIOHIDElementTypeOutput
	long collections;						// number of elements of type kIOHIDElementTypeCollection
    long axis;								// number of axis (calculated, not reported by device)
    long buttons;							// number of buttons (calculated, not reported by device)
    long hats;								// number of hat switches (calculated, not reported by device)
    long sliders;							// number of sliders (calculated, not reported by device)
    long dials;								// number of dials (calculated, not reported by device)
    long wheels;							// number of wheels (calculated, not reported by device)
    mmRecElement* pListElements; 				// head of linked list of elements 
    DisconnectState disconnect; // (ryan added this.)
    struct mmRecDevice* pNext; 				// next device
};

typedef struct mmRecDevice mmRecDevice;
typedef mmRecDevice* pmmRecDevice;


#if USE_NOTIFICATIONS
static IONotificationPortRef	gNotifyPort;
static io_iterator_t		gAddedIter;
static CFRunLoopRef		gRunLoop;
#endif USE_NOTIFICATIONS

// for element retrieval
static pmmRecDevice gCurrentGetDevice = NULL;
static Boolean gAddAsChild = false;
static int gDepth = false;

static pmmRecDevice gpmmDeviceList = NULL;
static UInt32 gNumDevices = 0;

static Boolean HIDIsValidDevice(const pmmRecDevice pSearchDevice);
static pmmRecElement HIDGetFirstDeviceElement (pmmRecDevice pDevice, HIDElementTypeMask typeMask);
static pmmRecElement HIDGetNextDeviceElement (pmmRecElement pElement, HIDElementTypeMask typeMask);
static pmmRecDevice HIDGetFirstDevice (void);
static pmmRecDevice HIDGetNextDevice (pmmRecDevice pDevice);
static void HIDReleaseDeviceList (void);
static unsigned long  HIDDequeueDevice (pmmRecDevice pDevice);
static void hid_GetElements (CFTypeRef refElementCurrent, pmmRecElement *ppCurrentElement);


static void HIDReportError(const char *err) {}
static void mmHIDReportErrorNum(const char *err, int num) {}


static void hid_GetCollectionElements (CFMutableDictionaryRef deviceProperties, pmmRecElement *ppCurrentCollection)
{
    CFTypeRef refElementTop = CFDictionaryGetValue (deviceProperties, CFSTR(kIOHIDElementKey));
    if (refElementTop)
        hid_GetElements (refElementTop, ppCurrentCollection);
    else
        HIDReportError ("hid_GetCollectionElements: CFDictionaryGetValue error when creating CFTypeRef for kIOHIDElementKey.");
}


// extracts actual specific element information from each element CF dictionary entry
static void hid_GetElementInfo (CFTypeRef refElement, pmmRecElement pElement)
{
	long number;
	CFTypeRef refType;
	// type, usagePage, usage already stored
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementCookieKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->cookie = (IOHIDElementCookie) number;
	else
		pElement->cookie = (IOHIDElementCookie) 0;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementMinKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->min = number;
	else
		pElement->min = 0;
    
	pElement->calMax = pElement->min;
	pElement->userMin = kDefaultUserMin;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementMaxKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->max = number;
	else
		pElement->max = 0;
    
	pElement->calMin = pElement->max;
	pElement->userMax = kDefaultUserMax;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementScaledMinKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->scaledMin = number;
	else
		pElement->scaledMin = 0;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementScaledMaxKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->scaledMax = number;
	else
		pElement->scaledMax = 0;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementSizeKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->size = number;
	else
		pElement->size = 0;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementIsRelativeKey));
	if (refType)
		pElement->relative = CFBooleanGetValue (refType);
	else
		pElement->relative = 0;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementIsWrappingKey));
	if (refType)
		pElement->wrapping = CFBooleanGetValue (refType);
	else
		pElement->wrapping = false;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementIsNonLinearKey));
	if (refType)
		pElement->nonLinear = CFBooleanGetValue (refType);
	else
		pElement->wrapping = false;
    
#ifdef kIOHIDElementHasPreferredStateKey
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementHasPreferredStateKey));
#else // Mac OS X 10.0 has spelling error
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementHasPreferedStateKey));
#endif
	if (refType)
		pElement->preferredState = CFBooleanGetValue (refType);
	else
		pElement->preferredState = false;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementHasNullStateKey));
	if (refType)
		pElement->nullState = CFBooleanGetValue (refType);
	else
		pElement->nullState = false;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementUnitKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->units = number;
	else
		pElement->units = 0;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementUnitExponentKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->unitExp = number;
	else
		pElement->unitExp = 0;
    
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementNameKey));
	if (refType)
		if (!CFStringGetCString (refType, pElement->name, 256, CFStringGetSystemEncoding ()))
			HIDReportError ("CFStringGetCString error retrieving pElement->name.");
    
#if 0
	if (!*pElement->name)
	{
		// set name from vendor id, product id & usage info look up
		if (!HIDGetElementNameFromVendorProductUsage (gCurrentGetDevice->vendorID, gCurrentGetDevice->productID, pElement->usagePage, pElement->usage, pElement->name))
		{
			// set name from vendor id/product id look up
			HIDGetElementNameFromVendorProductCookie (gCurrentGetDevice->vendorID, gCurrentGetDevice->productID, (long) pElement->cookie, pElement->name);
			if (!*pElement->name) { // if no name
				HIDGetUsageName (pElement->usagePage, pElement->usage, pElement->name);
				if (!*pElement->name) // if not usage
					sprintf (pElement->name, "Element");
			}
		}
	}
#endif
}


static void hid_AddElement (CFTypeRef refElement, pmmRecElement * ppElementCurrent)
{
	pmmRecDevice pDevice = gCurrentGetDevice;
    pmmRecElement pElement = NULL;
    long elementType, usagePage, usage;
    CFTypeRef refElementType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementTypeKey));
    CFTypeRef refUsagePage = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementUsagePageKey));
    CFTypeRef refUsage = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementUsageKey));
    
    if (refElementType)
		CFNumberGetValue (refElementType, kCFNumberLongType, &elementType);
	if (refUsagePage)
		CFNumberGetValue (refUsagePage, kCFNumberLongType, &usagePage);
	if (refUsage)
		CFNumberGetValue (refUsage, kCFNumberLongType, &usage);
    
	if (NULL == pDevice)
		return;
    
    if (elementType)
    {
        // look at types of interest
        if (elementType != kIOHIDElementTypeCollection)
		{
            if (usagePage && usage) // if valid usage and page
			{
				switch (usagePage) // only interested in kHIDPage_GenericDesktop and  kHIDPage_Button
				{
					case kHIDPage_GenericDesktop:
					{
						switch (usage) // look at usage to determine function
						{
							case kHIDUsage_GD_X:
							case kHIDUsage_GD_Y:
							case kHIDUsage_GD_Z:
							case kHIDUsage_GD_Rx:
							case kHIDUsage_GD_Ry:
							case kHIDUsage_GD_Rz:
								pElement = (pmmRecElement) malloc (sizeof (mmRecElement));
								if (pElement) pDevice->axis++;
									break;
							case kHIDUsage_GD_Slider:
								pElement = (pmmRecElement) malloc (sizeof (mmRecElement));
								if (pElement) pDevice->sliders++;
									break;
							case kHIDUsage_GD_Dial:
								pElement = (pmmRecElement) malloc (sizeof (mmRecElement));
								if (pElement) pDevice->dials++;
									break;
							case kHIDUsage_GD_Wheel:
								pElement = (pmmRecElement) malloc (sizeof (mmRecElement));
								if (pElement) pDevice->wheels++;
									break;
							case kHIDUsage_GD_Hatswitch:
								pElement = (pmmRecElement) malloc (sizeof (mmRecElement));
								if (pElement) pDevice->hats++;
									break;
							default:
								pElement = (pmmRecElement) malloc (sizeof (mmRecElement));
								break;
						}
					}
						break;
					case kHIDPage_Button:
						pElement = (pmmRecElement) malloc (sizeof (mmRecElement));
						if (pElement) pDevice->buttons++;
							break;
					default:
						// just add a generic element
						pElement = (pmmRecElement) malloc (sizeof (mmRecElement));
						break;
				}
			}
#if 0
            else
                HIDReportError ("CFNumberGetValue error when getting value for refUsage or refUsagePage.");
#endif 0
        }
        else // collection
			pElement = (pmmRecElement) malloc (sizeof (mmRecElement));
    }
    else
        HIDReportError ("CFNumberGetValue error when getting value for refElementType.");
    
    if (pElement) // add to list
    {
		// this code builds a binary tree based on the collection hierarchy of inherent in the device element layout
		// it preserves the structure of the lements as collections have children and elements are siblings to each other
        
		// clear record
		bzero(pElement,sizeof(mmRecElement));
        
		// get element info
        pElement->type = elementType;
        pElement->usagePage = usagePage;
        pElement->usage = usage;
        pElement->depth = 0;		// assume root object
        hid_GetElementInfo (refElement, pElement);
        
		// count elements
		pDevice->totalElements++;
        
		switch (pElement->type)
		{
			case kIOHIDElementTypeInput_Misc:
			case kIOHIDElementTypeInput_Button:
			case kIOHIDElementTypeInput_Axis:
			case kIOHIDElementTypeInput_ScanCodes:
				pDevice->inputs++;
				break;
			case kIOHIDElementTypeOutput:
				pDevice->outputs++;
				break;
			case kIOHIDElementTypeFeature:
				pDevice->features++;
				break;
			case kIOHIDElementTypeCollection:
				pDevice->collections++;
				break;
			default:
				mmHIDReportErrorNum ("Unknown element type : ", pElement->type);
		}
        
        if (NULL == *ppElementCurrent) // if at list head
		{
            pDevice->pListElements = pElement; // add current element
			*ppElementCurrent = pElement; // set current element to element we just added
		}
		else // have exsiting structure
		{
			if (gAddAsChild) // if the previous element was a collection, let's add this as a child of the previous
			{
				// this iteration should not be needed but there maybe some untested degenerate case which this code will ensure works
				while ((*ppElementCurrent)->pChild) // step down tree until free child node found
					*ppElementCurrent = (*ppElementCurrent)->pChild;
				(*ppElementCurrent)->pChild = pElement; // insert there
				pElement->depth = (*ppElementCurrent)->depth + 1;
			}
			else // add as sibling
			{
				// this iteration should not be needed but there maybe some untested degenerate case which this code will ensure works
				while ((*ppElementCurrent)->pSibling) // step down tree until free sibling node found
					*ppElementCurrent = (*ppElementCurrent)->pSibling;
				(*ppElementCurrent)->pSibling = pElement; // insert there
				pElement->depth = (*ppElementCurrent)->depth;
			}
			pElement->pPrevious = *ppElementCurrent; // point to previous
			*ppElementCurrent = pElement; // set current to our collection
		}
        
		if (elementType == kIOHIDElementTypeCollection) // if this element is a collection of other elements
		{
			gAddAsChild = true; // add next set as children to this element
			gDepth++;
			hid_GetCollectionElements ((CFMutableDictionaryRef) refElement, &pElement); // recursively process the collection
			gDepth--;
		}
		gAddAsChild = false; // add next as this elements sibling (when return from a collection or with non-collections)
    }
#if 0
    else
        HIDReportError ("hid_AddElement - no element added.");
#endif
}


static void hid_GetElementsCFArrayHandler (const void * value, void * parameter)
{
    if (CFGetTypeID (value) == CFDictionaryGetTypeID ())
        hid_AddElement ((CFTypeRef) value, (pmmRecElement *) parameter);
}

// ---------------------------------
// handles retrieval of element information from arrays of elements in device IO registry information

static void hid_GetElements (CFTypeRef refElementCurrent, pmmRecElement *ppCurrentElement)
{
    CFTypeID type = CFGetTypeID (refElementCurrent);
    if (type == CFArrayGetTypeID()) // if element is an array
    {
        CFRange range = {0, CFArrayGetCount (refElementCurrent)};
        // CountElementsCFArrayHandler called for each array member
        CFArrayApplyFunction (refElementCurrent, range, hid_GetElementsCFArrayHandler, ppCurrentElement);
    }
}

static void hid_TopLevelElementHandler (const void * value, void * parameter)
{
    CFTypeRef refCF = 0;
    if ((NULL == value) || (NULL == parameter))
        return;	// (kIOReturnBadArgument)
    if (CFGetTypeID (value) != CFDictionaryGetTypeID ())
        return;	// (kIOReturnBadArgument)
    refCF = CFDictionaryGetValue (value, CFSTR(kIOHIDElementUsagePageKey));
    if (!CFNumberGetValue (refCF, kCFNumberLongType, &((pmmRecDevice) parameter)->usagePage))
        HIDReportError ("CFNumberGetValue error retrieving pDevice->usagePage.");
    refCF = CFDictionaryGetValue (value, CFSTR(kIOHIDElementUsageKey));
    if (!CFNumberGetValue (refCF, kCFNumberLongType, &((pmmRecDevice) parameter)->usage))
        HIDReportError ("CFNumberGetValue error retrieving pDevice->usage.");
}


static void hid_GetDeviceInfo (io_object_t hidDevice, CFMutableDictionaryRef hidProperties, pmmRecDevice pDevice)
{
	CFMutableDictionaryRef usbProperties = 0;
	io_registry_entry_t parent1, parent2;
    
    // Mac OS X currently is not mirroring all USB properties to HID page so need to look at USB device page also
    // get dictionary for usb properties: step up two levels and get CF dictionary for USB properties
    if ((KERN_SUCCESS == IORegistryEntryGetParentEntry (hidDevice, kIOServicePlane, &parent1)) &&
        (KERN_SUCCESS == IORegistryEntryGetParentEntry (parent1, kIOServicePlane, &parent2)) &&
        (KERN_SUCCESS == IORegistryEntryCreateCFProperties (parent2, &usbProperties, kCFAllocatorDefault, kNilOptions)))
    {
        if (usbProperties)
        {
            CFTypeRef refCF = 0;
            // get device info
            // try hid dictionary first, if fail then go to usb dictionary
            
            // get transport
            refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDTransportKey));
            if (refCF)
            {
				if (!CFStringGetCString (refCF, pDevice->transport, 256, CFStringGetSystemEncoding ()))
                    HIDReportError ("CFStringGetCString error retrieving pDevice->transport.");
            }
            
            // get vendorID
            refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDVendorIDKey));
            if (!refCF)
                refCF = CFDictionaryGetValue (usbProperties, CFSTR("idVendor"));
            if (refCF)
            {
                if (!CFNumberGetValue (refCF, kCFNumberLongType, &pDevice->vendorID))
                    HIDReportError ("CFNumberGetValue error retrieving pDevice->vendorID.");
            }
            
            // get product ID
            refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDProductIDKey));
            if (!refCF)
                refCF = CFDictionaryGetValue (usbProperties, CFSTR("idProduct"));
            if (refCF)
            {
                if (!CFNumberGetValue (refCF, kCFNumberLongType, &pDevice->productID))
                    HIDReportError ("CFNumberGetValue error retrieving pDevice->productID.");
            }
            
            // get product version
            refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDVersionNumberKey));
            if (refCF)
            {
                if (!CFNumberGetValue (refCF, kCFNumberLongType, &pDevice->version))
                    HIDReportError ("CFNumberGetValue error retrieving pDevice->version.");
            }
            
            // get manufacturer name
            refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDManufacturerKey));
            if (!refCF)
                refCF = CFDictionaryGetValue (usbProperties, CFSTR("USB Vendor Name"));
            if (refCF)
            {
                if (!CFStringGetCString (refCF, pDevice->manufacturer, 256, CFStringGetSystemEncoding ()))
                    HIDReportError ("CFStringGetCString error retrieving pDevice->manufacturer.");
            }
            
            // get product name
            refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDProductKey));
            if (!refCF)
                refCF = CFDictionaryGetValue (usbProperties, CFSTR("USB Product Name"));
            if (refCF)
            {
                if (!CFStringGetCString (refCF, pDevice->product, 256, CFStringGetSystemEncoding ()))
                    HIDReportError ("CFStringGetCString error retrieving pDevice->product.");
            }
            
            // get serial
            refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDSerialNumberKey));
            if (refCF)
            {
                if (!CFStringGetCString (refCF, pDevice->serial, 256, CFStringGetSystemEncoding ()))
                    HIDReportError ("CFStringGetCString error retrieving pDevice->serial.");
            }
            
            // get location ID
            refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDLocationIDKey));
            if (!refCF)
                refCF = CFDictionaryGetValue (usbProperties, CFSTR("locationID"));
            if (refCF)
            {
                if (!CFNumberGetValue (refCF, kCFNumberLongType, &pDevice->locID))
                    HIDReportError ("CFNumberGetValue error retrieving pDevice->locID.");
            }
            
            // get usage page and usage
            refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDPrimaryUsagePageKey));
            if (refCF)
            {
                if (!CFNumberGetValue (refCF, kCFNumberLongType, &pDevice->usagePage))
                    HIDReportError ("CFNumberGetValue error retrieving pDevice->usagePage.");
                refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDPrimaryUsageKey));
                if (refCF)
                    if (!CFNumberGetValue (refCF, kCFNumberLongType, &pDevice->usage))
                        HIDReportError ("CFNumberGetValue error retrieving pDevice->usage.");
            }
            if (NULL == refCF) // get top level element HID usage page or usage
            {
                // use top level element instead
                CFTypeRef refCFTopElement = 0;
                refCFTopElement = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDElementKey));
                {
                    // refCFTopElement points to an array of element dictionaries
                    CFRange range = {0, CFArrayGetCount (refCFTopElement)};
                    CFArrayApplyFunction (refCFTopElement, range, hid_TopLevelElementHandler, NULL);
                }
            }
        }
        else
            HIDReportError ("IORegistryEntryCreateCFProperties failed to create usbProperties.");
        
        CFRelease (usbProperties);
        if (kIOReturnSuccess != IOObjectRelease (parent2))
            HIDReportError ("IOObjectRelease error with parent2.");
        if (kIOReturnSuccess != IOObjectRelease (parent1))
            HIDReportError ("IOObjectRelease error with parent1.");
    }
}


static Boolean hid_MatchElementTypeMask (IOHIDElementType type, HIDElementTypeMask typeMask)
{
	if (typeMask & kHIDElementTypeInput)
		if ((type == kIOHIDElementTypeInput_Misc) || (type == kIOHIDElementTypeInput_Button) || (type == kIOHIDElementTypeInput_Axis) || (type == kIOHIDElementTypeInput_ScanCodes))
			return true;
	if (typeMask & kHIDElementTypeOutput)
		if (type == kIOHIDElementTypeOutput)
			return true;
	if (typeMask & kHIDElementTypeFeature)
		if (type == kIOHIDElementTypeFeature)
			return true;
	if (typeMask & kHIDElementTypeCollection)
		if (type == kIOHIDElementTypeCollection)
			return true;
	return false;
}

static pmmRecElement hid_GetDeviceElement (pmmRecElement pElement, HIDElementTypeMask typeMask)
{
	// we are asking for this element
    if (NULL != pElement)
	{
		if (hid_MatchElementTypeMask (pElement->type, typeMask)) // if the type match what we are looking for
			return pElement; // return the element
		else
			return HIDGetNextDeviceElement (pElement, typeMask); // else get the next one
	}
	return NULL;
}

static unsigned long mmHIDCloseReleaseInterface (pmmRecDevice pDevice)
{
	IOReturn result = kIOReturnSuccess;
	
	if (HIDIsValidDevice(pDevice) && (NULL != pDevice->interface))
	{
		// close the interface
		result = (*(IOHIDDeviceInterface**) pDevice->interface)->close (pDevice->interface);
		if (kIOReturnNotOpen == result)
		{
			//  do nothing as device was not opened, thus can't be closed
		}
		else if (kIOReturnSuccess != result)
			HIDREPORTERRORNUM ("mmHIDCloseReleaseInterface - Failed to close IOHIDDeviceInterface.", result);
		//release the interface
		result = (*(IOHIDDeviceInterface**) pDevice->interface)->Release (pDevice->interface);
		if (kIOReturnSuccess != result)
			HIDREPORTERRORNUM ("mmHIDCloseReleaseInterface - Failed to release interface.", result);
		pDevice->interface = NULL;
	}	
	return result;
}      


// ---------------------------------
// count number of devices in global device list (gpmmDeviceList)
static UInt32 hid_CountCurrentDevices (void)
{
    pmmRecDevice pDevice = gpmmDeviceList;
    UInt32 devices = 0;
    while (pDevice)
    {
        devices++;
        pDevice = pDevice->pNext;
    }
    return devices;
}

static UInt32 HIDCountDevices (void)
{
	gNumDevices = hid_CountCurrentDevices ();
    
    return gNumDevices;
}

static void hid_DisposeDeviceElements (pmmRecElement pElement)
{
	if (pElement)
	{
		if (pElement->pChild)
			hid_DisposeDeviceElements (pElement->pChild);
		if (pElement->pSibling)
			hid_DisposeDeviceElements (pElement->pSibling);
		free (pElement);
	}
}

static pmmRecDevice hid_DisposeDevice (pmmRecDevice pDevice)
{
    kern_return_t result = KERN_SUCCESS;
    pmmRecDevice pDeviceNext = NULL;
    
    if (HIDIsValidDevice(pDevice))
    {
        // save next device prior to disposing of this device
        pDeviceNext = pDevice->pNext;
        
		result = HIDDequeueDevice (pDevice);
#if 0
		if (kIOReturnSuccess != result)
			mmHIDReportErrorNum ("hid_DisposeDevice: HIDDequeueDevice error: 0x%8.8X.", result);
#endif 1
        
        hid_DisposeDeviceElements (pDevice->pListElements);
		pDevice->pListElements = NULL;
        
		result = mmHIDCloseReleaseInterface (pDevice); // function sanity checks interface value (now application does not own device)
		if (kIOReturnSuccess != result)
			mmHIDReportErrorNum ("hid_DisposeDevice: mmHIDCloseReleaseInterface error: 0x%8.8X.", result);
        
#if USE_NOTIFICATIONS
        if (pDevice->interface)
        {
			// replace (*pDevice->interface)->Release(pDevice->interface);
			result = IODestroyPlugInInterface (pDevice->interface);
			if (kIOReturnSuccess != result)
				mmHIDReportErrorNum ("hid_DisposeDevice: IODestroyPlugInInterface error: 0x%8.8X.", result);
        }
        
        if (pDevice->notification)
		{
			result = IOObjectRelease((io_object_t) pDevice->notification);
			if (kIOReturnSuccess != result)
				mmHIDReportErrorNum ("hid_DisposeDevice: IOObjectRelease error: 0x%8.8X.", result);
		}
#endif USE_NOTIFICATIONS
        
		// remove this device from the device list
		if (gpmmDeviceList == pDevice)	// head of list?
			gpmmDeviceList = pDeviceNext;
		else
		{
			pmmRecDevice pDeviceTemp = pDeviceNext = gpmmDeviceList;	// we're going to return this if we don't find ourselfs in the list
			while (pDeviceTemp)
			{
				if (pDeviceTemp->pNext == pDevice) // found us!
				{
					// take us out of linked list
					pDeviceTemp->pNext = pDeviceNext = pDevice->pNext;
					break;
				}
				pDeviceTemp = pDeviceTemp->pNext;
			}
		}
        free (pDevice);
    }
    
	// update device count
	gNumDevices = hid_CountCurrentDevices ();
    
    return pDeviceNext;
}


// ---------------------------------
// disposes and releases queue, sets queue to NULL,.
// Note: will have no effect if device or queue do not exist

static IOReturn hid_DisposeReleaseQueue (pmmRecDevice pDevice)
{
    IOReturn result = kIOReturnError;	// assume failure (pessimist!)
    
	if (HIDIsValidDevice(pDevice))	// need valid device
	{
		if (pDevice->queue) // and queue
		{
			// stop queue
			result = (*(IOHIDQueueInterface**) pDevice->queue)->stop (pDevice->queue);
			if (kIOReturnSuccess != result)
				HIDREPORTERRORNUM ("hid_DisposeReleaseQueue - Failed to stop queue.", result);
			// dispose of queue
			result = (*(IOHIDQueueInterface**) pDevice->queue)->dispose (pDevice->queue);
			if (kIOReturnSuccess != result)
				HIDREPORTERRORNUM ("hid_DisposeReleaseQueue - Failed to dipose queue.", result);
			// release the queue
			result = (*(IOHIDQueueInterface**) pDevice->queue)->Release (pDevice->queue);
			if (kIOReturnSuccess != result)
				HIDREPORTERRORNUM ("hid_DisposeReleaseQueue - Failed to release queue.", result);
            
			pDevice->queue = NULL;
		}
		else
			HIDREPORTERROR ("hid_DisposeReleaseQueue - no queue.");
	}
	else
		HIDREPORTERROR ("hid_DisposeReleaseQueue - Invalid device.");
    return result;
}


// ---------------------------------
// completely removes all elements from queue and releases queue and closes device interface
// does not release device interfaces, application must call HIDReleaseDeviceList on exit

static unsigned long  HIDDequeueDevice (pmmRecDevice pDevice)
{
    IOReturn result = kIOReturnSuccess;
    
    if (HIDIsValidDevice(pDevice))
	{
		if ((pDevice->interface) && (pDevice->queue))
		{
			// iterate through elements and if queued, remove
			pmmRecElement pElement = HIDGetFirstDeviceElement (pDevice, kHIDElementTypeIO);
			while (pElement)
			{
				if ((*(IOHIDQueueInterface**) pDevice->queue)->hasElement (pDevice->queue, pElement->cookie))
				{
					result = (*(IOHIDQueueInterface**) pDevice->queue)->removeElement (pDevice->queue, pElement->cookie);
					if (kIOReturnSuccess != result)
						HIDREPORTERRORNUM ("HIDDequeueDevice - Failed to remove element from queue.", result);
				}
				pElement = HIDGetNextDeviceElement (pElement, kHIDElementTypeIO);
			}
		}
		// ensure queue is disposed and released
		// interface will be closed and released on call to HIDReleaseDeviceList
		result = hid_DisposeReleaseQueue (pDevice);
		if (kIOReturnSuccess != result)
			HIDREPORTERRORNUM ("removeElement - Failed to dispose and release queue.", result);
#if USE_ASYNC_EVENTS
		else if (NULL != pDevice->queueRunLoopSource)
		{
			if (CFRunLoopContainsSource(CFRunLoopGetCurrent(), pDevice->queueRunLoopSource, kCFRunLoopDefaultMode))
				CFRunLoopRemoveSource(CFRunLoopGetCurrent(), pDevice->queueRunLoopSource, kCFRunLoopDefaultMode);
			CFRelease(pDevice->queueRunLoopSource);
			pDevice->queueRunLoopSource = NULL;
		}
#endif USE_ASYNC_EVENTS
	}
	else
	{
		HIDREPORTERROR ("HIDDequeueDevice - Invalid device.");
		result = kIOReturnBadArgument;
	}
    return result;
}

// ---------------------------------
// releases all device queues for quit or rebuild (must be called)
// does not release device interfaces, application must call HIDReleaseDeviceList on exit

static unsigned long HIDReleaseAllDeviceQueues (void)
{
    IOReturn result = kIOReturnBadArgument;
    pmmRecDevice pDevice = HIDGetFirstDevice ();
    
    while (pDevice)
    {
        result = HIDDequeueDevice (pDevice);
        if (kIOReturnSuccess != result)
            HIDREPORTERRORNUM ("HIDReleaseAllDeviceQueues - Could not dequeue device.", result);
        pDevice = HIDGetNextDevice (pDevice);
    }
    return result;
}


// ---------------------------------
// Get the next event in the queue for a device
// elements or entire device should be queued prior to calling this with HIDQueueElement or HIDQueueDevice
// returns true if an event is avialable for the element and fills out *pHIDEvent structure, returns false otherwise
// Note: kIOReturnUnderrun returned from getNextEvent indicates an empty queue not an error condition
// Note: application should pass in a pointer to a IOHIDEventStruct cast to a void (for CFM compatibility)

static unsigned char HIDGetEvent (pmmRecDevice pDevice, void * pHIDEvent)
{
    IOReturn result = kIOReturnBadArgument;
    AbsoluteTime zeroTime = {0,0};
    
    if (HIDIsValidDevice(pDevice))
	{
		if (pDevice->queue)
		{
			result = (*(IOHIDQueueInterface**) pDevice->queue)->getNextEvent (pDevice->queue, (IOHIDEventStruct *)pHIDEvent, zeroTime, 0);
			if (kIOReturnUnderrun == result)
				return false;  // no events in queue not an error per say
			else if (kIOReturnSuccess != result) // actual error versus just an empty queue
				HIDREPORTERRORNUM ("HIDGetEvent - Could not get HID event via getNextEvent.", result);
			else
				return true;
		}
		else
			HIDREPORTERROR ("HIDGetEvent - queue does not exist.");
	}
	else
		HIDREPORTERROR ("HIDGetEvent - invalid device.");
    
    return false; // did not get event
}


static unsigned long mmHIDCreateOpenDeviceInterface (UInt32 hidDevice, pmmRecDevice pDevice)
{
    IOReturn result = kIOReturnSuccess;
    HRESULT plugInResult = S_OK;
    SInt32 score = 0;
    IOCFPlugInInterface ** ppPlugInInterface = NULL;
    
	if (NULL == pDevice->interface)
	{
		result = IOCreatePlugInInterfaceForService (hidDevice, kIOHIDDeviceUserClientTypeID,
                                                    kIOCFPlugInInterfaceID, &ppPlugInInterface, &score);
		if (kIOReturnSuccess == result)
		{
			// Call a method of the intermediate plug-in to create the device interface
			plugInResult = (*ppPlugInInterface)->QueryInterface (ppPlugInInterface,
                                                                 CFUUIDGetUUIDBytes (kIOHIDDeviceInterfaceID), (void *) &(pDevice->interface));
			if (S_OK != plugInResult)
				mmHIDReportErrorNum ("CouldnÕt query HID class device interface from plugInInterface", plugInResult);
			IODestroyPlugInInterface (ppPlugInInterface); // replace (*ppPlugInInterface)->Release (ppPlugInInterface)
		}
		else
			mmHIDReportErrorNum ("Failed to create **plugInInterface via IOCreatePlugInInterfaceForService.", result);
	}
	if (NULL != pDevice->interface)
	{
		result = (*(IOHIDDeviceInterface**)pDevice->interface)->open (pDevice->interface, 0);
		if (kIOReturnSuccess != result)
			mmHIDReportErrorNum ("Failed to open pDevice->interface via open.", result);
	}
    return result;
}


// ---------------------------------
// adds device to linked list of devices passed in (handles NULL lists properly)
// (returns where you just stored it)
static pmmRecDevice* hid_AddDevice (pmmRecDevice *ppListDeviceHead, pmmRecDevice pNewDevice)
{
	pmmRecDevice* result = NULL;
	
    if (NULL == *ppListDeviceHead)
        result = ppListDeviceHead;
    else
    {
        pmmRecDevice pDevicePrevious = NULL, pDevice = *ppListDeviceHead;
        while (pDevice)
        {
            pDevicePrevious = pDevice;
            pDevice = pDevicePrevious->pNext;
        }
        result = &pDevicePrevious->pNext;
    }
    pNewDevice->pNext = NULL;
    
	*result = pNewDevice;
    
	return result;
}

static pmmRecDevice hid_BuildDevice (io_object_t hidDevice)
{
    pmmRecDevice pDevice = (pmmRecDevice) malloc (sizeof (mmRecDevice));
    
    if (NULL != pDevice)
    {
		// get dictionary for HID properties
        CFMutableDictionaryRef hidProperties = 0;
        kern_return_t result = IORegistryEntryCreateCFProperties (hidDevice, &hidProperties, kCFAllocatorDefault, kNilOptions);
        
		// clear record
		bzero(pDevice, sizeof(mmRecDevice));
        
        if ((result == KERN_SUCCESS) && (NULL != hidProperties))
        {
			pmmRecElement pCurrentElement = NULL;
			// create device interface
			result = mmHIDCreateOpenDeviceInterface (hidDevice, pDevice);
			if (kIOReturnSuccess != result)
				mmHIDReportErrorNum ("mmHIDCreateOpenDeviceInterface failed.", result);
            hid_GetDeviceInfo (hidDevice, hidProperties, pDevice); // hidDevice used to find parents in registry tree
																   // set current device for use in getting elements
			gCurrentGetDevice = pDevice;
			// Add all elements
            hid_GetCollectionElements (hidProperties, &pCurrentElement);
			gCurrentGetDevice = NULL;
            CFRelease (hidProperties);
        }
        else
            mmHIDReportErrorNum ("IORegistryEntryCreateCFProperties error when creating deviceProperties.", result);
    }
    else
        HIDReportError ("malloc error when allocating pmmRecDevice.");
    return pDevice;
}



#if USE_NOTIFICATIONS
//================================================================================================
//
//	hid_DeviceNotification
//
//	This routine will get called whenever any kIOGeneralInterest notification happens.  We are
//	interested in the kIOMessageServiceIsTerminated message so that's what we look for.  Other
//	messages are defined in IOMessage.h.
//
//================================================================================================
//
static void hid_DeviceNotification( void *refCon,
									io_service_t service,
									natural_t messageType,
									void *messageArgument )
{
    pmmRecDevice pDevice = (pmmRecDevice) refCon;
    
    if (messageType == kIOMessageServiceIsTerminated)
    {
        //printf("Device 0x%08x \"%s\"removed.\n", service, pDevice->product);
        // ryan added this.
        if (pDevice->disconnect == DISCONNECT_CONNECTED)
    	    pDevice->disconnect = DISCONNECT_TELLUSER;
        
        // Free the data we're no longer using now that the device is going away
        // ryan commented this out.
		//hid_DisposeDevice (pDevice);
    }
}
#else

static void hid_RemovalCallbackFunction(void * target, IOReturn result, void * refcon, void * sender)
{
    // ryan commented this out.
	//hid_DisposeDevice ((pmmRecDevice) target);
    
    // ryan added this.
    pmmRecDevice = (pmmRecDevice) target;
    if (pDevice->disconnect == DISCONNECT_CONNECTED)
        pDevice->disconnect = DISCONNECT_TELLUSER;
}

#endif USE_NOTIFICATIONS



static void hid_AddDevices (void *refCon, io_iterator_t iterator)
{
	// NOTE: refcon passed in is used to point to the device list head
    pmmRecDevice* pListDeviceHead = (pmmRecDevice*) refCon;
    IOReturn result = kIOReturnSuccess;
    io_object_t ioHIDDeviceObject = 0;
    
    while ((ioHIDDeviceObject = IOIteratorNext (iterator)) != 0)
    {
		pmmRecDevice* pNewDeviceAt = NULL;
		pmmRecDevice pNewDevice = hid_BuildDevice (ioHIDDeviceObject);
		if (pNewDevice)
		{
#if 0	// set true for verbose output
			printf("\nhid_AddDevices: pNewDevice = {t: \"%s\", v: %ld, p: %ld, v: %ld, m: \"%s\", " \
                   "p: \"%s\", l: %ld, u: %4.4lX:%4.4lX, #e: %ld, #f: %ld, #i: %ld, #o: %ld, " \
                   "#c: %ld, #a: %ld, #b: %ld, #h: %ld, #s: %ld, #d: %ld, #w: %ld}.",
                   pNewDevice->transport,
                   pNewDevice->vendorID,
                   pNewDevice->productID,
                   pNewDevice->version,
                   pNewDevice->manufacturer,
                   pNewDevice->product,
                   pNewDevice->locID,
                   pNewDevice->usagePage,
                   pNewDevice->usage,
                   pNewDevice->totalElements,
                   pNewDevice->features,
                   pNewDevice->inputs,
                   pNewDevice->outputs,
                   pNewDevice->collections,
                   pNewDevice->axis,
                   pNewDevice->buttons,
                   pNewDevice->hats,
                   pNewDevice->sliders,
                   pNewDevice->dials,
                   pNewDevice->wheels
                   );
			fflush(stdout);
#elif	0	// otherwise output brief description
			printf("\nhid_AddDevices: pNewDevice = {m: \"%s\" p: \"%s\", vid: %ld, pid: %ld, loc: %8.8lX, usage: %4.4lX:%4.4lX}.",
                   pNewDevice->manufacturer,
                   pNewDevice->product,
                   pNewDevice->vendorID,
                   pNewDevice->productID,
                   pNewDevice->locID,
                   pNewDevice->usagePage,
                   pNewDevice->usage
                   );
			fflush(stdout);
#endif
			pNewDeviceAt = hid_AddDevice (pListDeviceHead, pNewDevice);
		}
        
#if USE_NOTIFICATIONS
        // Register for an interest notification of this device being removed. Use a reference to our
        // private data as the refCon which will be passed to the notification callback.
        result = IOServiceAddInterestNotification( gNotifyPort,					// notifyPort
												   ioHIDDeviceObject,			// service
												   kIOGeneralInterest,			// interestType
												   hid_DeviceNotification,		// callback
												   pNewDevice,					// refCon
												   (io_object_t*) &pNewDevice->notification);	// notification
		if (KERN_SUCCESS != result)
			mmHIDReportErrorNum ("hid_AddDevices: IOServiceAddInterestNotification error: x0%8.8lX.", result);
#else
		result = (*(IOHIDDeviceInterface**)pNewDevice->interface)->setRemovalCallback (pNewDevice->interface, hid_RemovalCallbackFunction,pNewDeviceAt,0);
#endif USE_NOTIFICATIONS
        
		// release the device object, it is no longer needed
		result = IOObjectRelease (ioHIDDeviceObject);
		if (KERN_SUCCESS != result)
			mmHIDReportErrorNum ("hid_AddDevices: IOObjectRelease error with ioHIDDeviceObject.", result);
    }
}


static Boolean HIDBuildDeviceList (UInt32 usagePage, UInt32 usage)
{
    IOReturn result = kIOReturnSuccess;
    mach_port_t masterPort = 0;
    
    if (NULL != gpmmDeviceList)
        HIDReleaseDeviceList ();
    
    result = IOMasterPort (bootstrap_port, &masterPort);
    if (kIOReturnSuccess != result)
        mmHIDReportErrorNum ("IOMasterPort error with bootstrap_port.", result);
    else
    {
		CFMutableDictionaryRef hidMatchDictionary = NULL;
        
		// Set up matching dictionary to search the I/O Registry for HID devices we are interested in. Dictionary reference is NULL if error.
		{
			CFNumberRef refUsage = NULL, refUsagePage = NULL;
            
			// Set up a matching dictionary to search I/O Registry by class name for all HID class devices.
			hidMatchDictionary = IOServiceMatching (kIOHIDDeviceKey);
			if (NULL != hidMatchDictionary)
			{
				if (usagePage)
				{
					// Add key for device type (joystick, in this case) to refine the matching dictionary.
					refUsagePage = CFNumberCreate (kCFAllocatorDefault, kCFNumberLongType, &usagePage);
					CFDictionarySetValue (hidMatchDictionary, CFSTR (kIOHIDPrimaryUsagePageKey), refUsagePage);
					CFRelease (refUsagePage);
					if (usage)
					{
						refUsage = CFNumberCreate (kCFAllocatorDefault, kCFNumberLongType, &usage);
						CFDictionarySetValue (hidMatchDictionary, CFSTR (kIOHIDPrimaryUsageKey), refUsage);
						CFRelease (refUsage);
					}
				}
				CFRetain(hidMatchDictionary);
			}
			else
				HIDReportError ("Failed to get HID CFMutableDictionaryRef via IOServiceMatching.");
		}
        
#if USE_NOTIFICATIONS
		// Create a notification port and add its run loop event source to our run loop
		// This is how async notifications get set up.
		{
			CFRunLoopSourceRef		runLoopSource;
            
			gNotifyPort = IONotificationPortCreate(masterPort);
			runLoopSource = IONotificationPortGetRunLoopSource(gNotifyPort);
            
			gRunLoop = CFRunLoopGetCurrent();
			CFRunLoopAddSource(gRunLoop, runLoopSource, kCFRunLoopDefaultMode);
            
			// Now set up a notification to be called when a device is first matched by I/O Kit.
			result = IOServiceAddMatchingNotification(gNotifyPort,			// notifyPort
                                                      kIOFirstMatchNotification,		// notificationType
                                                      hidMatchDictionary,			// matching
                                                      hid_AddDevices,				// callback
                                                      &gpmmDeviceList,					// refCon
                                                      &gAddedIter					// notification
                                                      );
            
			// call it now to add all existing devices
			hid_AddDevices(&gpmmDeviceList,gAddedIter);
            return true;
		}
#else
		{
			io_iterator_t hidObjectIterator = NULL;
            
			// Now search I/O Registry for matching devices.
			result = IOServiceGetMatchingServices (masterPort, hidMatchDictionary, &hidObjectIterator);
			if (kIOReturnSuccess != result)
				mmHIDReportErrorNum ("Failed to create IO object iterator, error:", result);
			else if (NULL == hidObjectIterator) // likely no HID devices which matched selection criteria are connected
				HIDReportError ("Warning: Could not find any matching devices, thus iterator creation failed.");
            
			if (NULL != hidObjectIterator)
			{
				hid_AddDevices(&gpmmDeviceList,hidObjectIterator);
                
				result = IOObjectRelease (hidObjectIterator); // release the iterator
				if (kIOReturnSuccess != result)
					mmHIDReportErrorNum ("IOObjectRelease error with hidObjectIterator.", result);
                
				gNumDevices = hid_CountCurrentDevices ();
				return true;
			}
		}
#endif USE_NOTIFICATIONS
		// IOServiceGetMatchingServices consumes a reference to the dictionary, so we don't need to release the dictionary ref.
		hidMatchDictionary = NULL;
    }
	return false;
}

// ---------------------------------
// release list built by above function
// MUST be called prior to application exit to properly release devices
// if not called (or app crashes) devices can be recovered by pluging into different location in USB chain

static void HIDReleaseDeviceList (void)
{
    while (NULL != gpmmDeviceList)
		gpmmDeviceList = hid_DisposeDevice (gpmmDeviceList); // dispose current device return next device will set gpmmDeviceList to NULL
    gNumDevices = 0;
}

// ---------------------------------
// get the first device in the device list
// returns NULL if no list exists

static pmmRecDevice HIDGetFirstDevice (void)
{
    return gpmmDeviceList;
}

// ---------------------------------
// get next device in list given current device as parameter
// returns NULL if end of list

static pmmRecDevice HIDGetNextDevice (pmmRecDevice pDevice)
{
    if (NULL != pDevice)
        return pDevice->pNext;
    else
        return NULL;
}

// ---------------------------------
// get the first element of device passed in as parameter
// returns NULL if no list exists or device does not exists or is NULL
static pmmRecElement HIDGetFirstDeviceElement (pmmRecDevice pDevice, HIDElementTypeMask typeMask)
{
    if (HIDIsValidDevice(pDevice))
	{
        if (hid_MatchElementTypeMask (pDevice->pListElements->type, typeMask)) // ensure first type matches
			return pDevice->pListElements;
		else
			return HIDGetNextDeviceElement (pDevice->pListElements, typeMask);
	}
    else
        return NULL;
}

// ---------------------------------
// get next element of given device in list given current element as parameter
// will walk down each collection then to next element or collection (depthwise traverse)
// returns NULL if end of list
// uses mask of HIDElementTypeMask to restrict element found
// use kHIDElementTypeIO to get previous HIDGetNextDeviceElement functionality
static pmmRecElement HIDGetNextDeviceElement (pmmRecElement pElement, HIDElementTypeMask typeMask)
{
	// should only have elements passed in (though someone could mix calls and pass us a collection)
	// collection means return the next child or sibling (in that order)
	// element means returnt he next sibling (as elements can't have children
    if (NULL != pElement)
	{
		if (pElement->pChild)
		{
			if (pElement->type != kIOHIDElementTypeCollection)
				HIDReportError ("Malformed element list: found child of element.");
			else
				return hid_GetDeviceElement (pElement->pChild, typeMask); // return the child of this element
		}
		else if (pElement->pSibling)
		{
			return hid_GetDeviceElement (pElement->pSibling, typeMask); //return the sibling of this element
		}
		else // at end back up correctly
		{
			pmmRecElement pPreviousElement = NULL;
			// malformed device ending in collection
			if (pElement->type == kIOHIDElementTypeCollection)
				HIDReportError ("Malformed device: found collection at end of element chain.");
			// walk back up tree to element prior to first collection ecountered and take next element
			while (NULL != pElement->pPrevious)
			{
				pPreviousElement = pElement;
				pElement = pElement->pPrevious; // look at previous element
                                                // if we have a collection and the previous element is the branch element (should have both a colection and next element attached to it)
                                                // if we found a collection, which we are not at the sibling level that actually does have siblings
				if (((pElement->type == kIOHIDElementTypeCollection) && (pPreviousElement != pElement->pSibling) && pElement->pSibling) ||
                    // or if we are at the top
                    (NULL == pElement->pPrevious)) // at top of tree
					break;
			}
			if (NULL == pElement->pPrevious)
				return NULL; // got to top of list with only a collection as the first element
                             // now we must have been down the child route so go down the sibling route
			pElement = pElement->pSibling; // element of interest
			return hid_GetDeviceElement (pElement, typeMask); // otherwise return this element
		}
	}
	return NULL;
}


// return true if this is a valid device pointer
Boolean HIDIsValidDevice(const pmmRecDevice pSearchDevice)
{
	pmmRecDevice pDevice = gpmmDeviceList;
    
	while (pDevice)
	{
		if (pDevice == pSearchDevice)
			return true;
		pDevice = pDevice->pNext;
	}
	return false;
}


static IOReturn hid_CreateQueue (pmmRecDevice pDevice)
{
    IOReturn result = kIOReturnError;	// assume failure (pessimist!)
    
	if (HIDIsValidDevice(pDevice))
	{
		if (NULL == pDevice->queue) // do we already have a queue
		{
			if (NULL != pDevice->interface)
			{
				pDevice->queue = (void *) (*(IOHIDDeviceInterface**) pDevice->interface)->allocQueue (pDevice->interface); // alloc queue
				if (pDevice->queue)
				{
					result = (*(IOHIDQueueInterface**) pDevice->queue)->create (pDevice->queue, 0, kDeviceQueueSize); // create actual queue
					if (kIOReturnSuccess != result)
						HIDREPORTERRORNUM ("hid_CreateQueue - Failed to create queue via create", result);
				}
				else
				{
					HIDREPORTERROR ("hid_CreateQueue - Failed to alloc IOHIDQueueInterface ** via allocQueue");
					result = kIOReturnError; // synthesis error
				}
			}
			else
				HIDREPORTERRORNUM ("hid_CreateQueue - Device inteface does not exist for queue creation", result);
		}
	}
	else
		HIDREPORTERRORNUM ("hid_CreateQueue - Invalid Device", result);
    return result;
}

static unsigned long  HIDQueueDevice (pmmRecDevice pDevice)
{
    IOReturn result = kIOReturnError;	// assume failure (pessimist!)
    pmmRecElement pElement;
    
	if (HIDIsValidDevice(pDevice))
	{
		// error checking
		if (NULL == pDevice)
		{
			HIDREPORTERROR ("HIDQueueDevice - Device does not exist.");
			return kIOReturnBadArgument;
		}
		if (NULL == pDevice->interface) // must have interface
		{
			HIDREPORTERROR ("HIDQueueDevice - Device does not have interface.");
			return kIOReturnError;
		}
		if (NULL == pDevice->queue) // if no queue create queue
			result = hid_CreateQueue (pDevice);
		if ((kIOReturnSuccess != result) || (NULL == pDevice->queue))
		{
			HIDREPORTERRORNUM ("HIDQueueDevice - problem creating queue.", result);
			if (kIOReturnSuccess != result)
				return result;
			else
				return kIOReturnError;
		}
        
		// stop queue
		result = (*(IOHIDQueueInterface**) pDevice->queue)->stop (pDevice->queue);
		if (kIOReturnSuccess != result)
			HIDREPORTERRORNUM ("HIDQueueDevice - Failed to stop queue.", result);
        
		// queue element
        //¥ pElement = HIDGetFirstDeviceElement (pDevice, kHIDElementTypeIO);
		pElement = HIDGetFirstDeviceElement (pDevice, kHIDElementTypeInput | kHIDElementTypeFeature);
        
		while (pElement)
		{
			if (!(*(IOHIDQueueInterface**) pDevice->queue)->hasElement (pDevice->queue, pElement->cookie))
			{
				result = (*(IOHIDQueueInterface**) pDevice->queue)->addElement (pDevice->queue, pElement->cookie, 0);
				if (kIOReturnSuccess != result)
					HIDREPORTERRORNUM ("HIDQueueDevice - Failed to add element to queue.", result);
			}
			//¥ pElement = HIDGetNextDeviceElement (pElement, kHIDElementTypeIO);
			pElement = HIDGetNextDeviceElement (pElement, kHIDElementTypeInput | kHIDElementTypeFeature);
		}
        
		// start queue
		result = (*(IOHIDQueueInterface**) pDevice->queue)->start (pDevice->queue);
		if (kIOReturnSuccess != result)
			HIDREPORTERRORNUM ("HIDQueueDevice - Failed to start queue.", result);
		
	}
	else
		HIDREPORTERROR ("HIDQueueDevice - Invalid device.");
    
    return result;
}


/* -- END HID UTILITIES -- */

/* spencer commented this out */
/* #include "manymouse.h" */

/* spencer added this array - should this be statically allocated instead? */
static char * open_mice = NULL;
static int available_mice = 0;
static pmmRecDevice *devices = NULL;

/* spencer renamed this function, and changed its argument type */
/* spencer modified this function to call SDL_PushEvent with an appropriate
SDLEvent instead of filling out a ManyMouseEvent */
static int Mouse_private_poll( int i )
{    
    SDL_Event outevent;
    
    pmmRecDevice mouse = devices[i];
    int unhandled = 1;
    while (unhandled)  /* read until failure or valid event. */
    {
        pmmRecElement recelem;
        IOHIDEventStruct event;
        
        if (!HIDGetEvent(mouse, &event))
            return 0;  /* no new event. */
        
        unhandled = 0;  /* will reset if necessary. */
        recelem = HIDGetFirstDeviceElement(mouse, kHIDElementTypeInput);
        while (recelem != NULL)
        {
            if (recelem->cookie == event.elementCookie)
                break;
            recelem = HIDGetNextDeviceElement(recelem, kHIDElementTypeInput);
        }

        if (recelem == NULL)
            continue;  /* unknown device element. Can this actually happen? */

        if ( recelem->usagePage == kHIDPage_GenericDesktop && event.value )
        {
            /* !!! FIXME: absolute motion? */
            outevent.type = SDL_MOUSEMOTION;
            outevent.motion.x = i;  /* store the mouse number in .x - a total hack */
            if ( recelem->usage == kHIDUsage_GD_X )
            {
                outevent.motion.xrel = event.value;
                outevent.motion.yrel = 0;
            }
            else if (recelem->usage == kHIDUsage_GD_Y)
            {
                outevent.motion.xrel = 0;
                outevent.motion.yrel = event.value;
            }
            else if (recelem->usage == kHIDUsage_GD_Wheel)
                /* not supported right now */
            {
                return 0;
                /* !!! FIXME: find horizontal scroll? */
                /*
                outevent->type = MANYMOUSE_EVENT_SCROLL;
                outevent->item = 0;
                 */
            }
            else
            {
                unhandled = 1;
            }
        }

        else if (recelem->usagePage == kHIDPage_Button)
        {
            outevent.button.x = i; /* store the mouse number in .x - a total hack */
            outevent.type = event.value ? SDL_MOUSEBUTTONDOWN : SDL_MOUSEBUTTONUP;
            outevent.button.button = ((int) recelem->usage) - 1;
        }

        else
        {
            unhandled = 1;
        }
    }
    
    SDL_PushEvent( &outevent );

    return(1);  /* got a valid event */
} /* poll_mouse */


/* spencer renamed this function, and added free( open_mice )  */
void Mouse_quit(void)
{
    HIDReleaseAllDeviceQueues();
    HIDReleaseDeviceList();
    free(devices);
    devices = NULL;
    free( open_mice );
    open_mice = NULL;
    available_mice = 0;
} /* macosx_hidmanager_quit */

/* spencer renamed this function, and changed its return type, and made many 
 modifications */
void Mouse_init(void)
{    
    if (!HIDBuildDeviceList(kHIDPage_GenericDesktop, kHIDUsage_GD_Mouse))
        return;
    
    available_mice = HIDCountDevices();
    if (available_mice > 0)
    {
        int i;
        pmmRecDevice dev = NULL;
        
        dev = HIDGetFirstDevice();
        devices = (pmmRecDevice *) malloc(sizeof (pmmRecDevice) * available_mice);
        if ((devices == NULL) || (dev == NULL))
        {
            free(devices);
            HIDReleaseDeviceList();
            available_mice = 0;
        }

        for (i = 0; i < available_mice; i++)
        {
            if (dev == NULL)  /* what? list ended? Truncate final list... */
                available_mice = i;
            
            if (HIDQueueDevice(dev) == kIOReturnSuccess)
                devices[i] = dev;
            else  /* failed? Chop this device from the list... */
            {
                i--;
                available_mice--;
            }

            dev = HIDGetNextDevice(dev);
        }
        
        open_mice = ( char * ) malloc( sizeof( char ) * available_mice );
        memset( open_mice, 0, sizeof( char ) * available_mice );
    }

}


static const char *macosx_hidmanager_name(unsigned int index)
{
    if (index >= available_mice)
        return(NULL);
    
    return((const char *) devices[index]->product);
} /* macosx_hidmanager_name */

/* spencer renamed this function, changed its argument and return types, and 
 modified the function significantly to fit it into the SDL event system */
void Mouse_poll(void)
{
    /*
     * (i) is static so we iterate through all mice round-robin. This
     *  prevents a chatty mouse from dominating the queue.
     */
    unsigned int i = 0;
        
    while (i < available_mice)
    {
        pmmRecDevice dev = devices[i];
        
        if ( open_mice[i] && (dev) && (dev->disconnect != DISCONNECT_COMPLETE))
        {
            /* see if mouse was unplugged since last polling... */
            /*if (dev->disconnect == DISCONNECT_TELLUSER)
        {
                dev->disconnect = DISCONNECT_COMPLETE;
                event->type = MANYMOUSE_EVENT_DISCONNECT;
                return;
        } */
            
            Mouse_private_poll( i );
        }
        
        i++;
    }

    /* no new events */
} /* macosx_hidmanager_poll */

/* spencer added this function */
int Mouse_count()
{
    return available_mice;
}

/* spencer added this function */
extern void Mouse_absolute_position( int * x, int * y, double * fx, double * fy )
{
    Point p;
    CGPoint cgp;
    CGDisplayCount cgdc;
    CGDirectDisplayID cgddid;
    
    GetGlobalMouse( &p );
    if( x != NULL )
        *x = p.h;
    if( y != NULL )
        *y = p.v;
    
    if( fx != NULL )
        *fx = ( *x ) / - ( ( t_CKFLOAT ) SHRT_MIN );
    if( fy != NULL )
        *fy = ( *y ) / - ( ( t_CKFLOAT ) SHRT_MIN );
}

/* spencer added this function */
int Mouse_open( int mouse )
{
    if( mouse < 0 || mouse >= available_mice )
        return -1;
    
    open_mice[mouse] = 1;
    return 0;
}

/* spencer added this function */
int Mouse_close( int mouse )
{
    if( mouse < 0 || mouse >= available_mice || open_mice[mouse] == 0 )
        return -1;
    open_mice[mouse] = 0;
    return 0;
}



/* spencer commented this out, and deleted nested comments */
/* #endif   MacOSX blocker  */

/* end of macosx_hidmanager.c ... */

#elif defined( __WINDOWS_DS__ )

static int available_mice = 0;

#if defined( __WINDOWS_RAWINPUT__ )
/*****************************************************************************
 *
 *  DirectX on Windows XP doesnt support accessing multiple mice.  Instead 
 *  we are supposed to use RawInput/WM_INPUT.  Uhh, awesome.  Most of this 
 *  code copied from Ryan Gordon's ManyMouse project.  
 *
 *****************************************************************************/

static char * open_mice = NULL;

/*
 * Support for Windows via the WM_INPUT message.
 *
 * Please see the file LICENSE in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

/* WinUser.h won't include rawinput stuff without this... */
#if (_WIN32_WINNT < 0x0501)
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif


#include <windows.h>
#include <malloc.h>

/* Cygwin's headers don't have WM_INPUT right now... */
#ifndef WM_INPUT
#define WM_INPUT 0x00FF
#endif

/* that should be enough, knock on wood. */
#define MAX_MICE 1024

static int did_api_lookup = 0;
static HWND raw_hwnd = NULL;
static const char *class_name = "ChucKRawInputCatcher";
static const char *win_name = "ChucKRawInputMsgWindow";
static ATOM class_atom = 0;
static CRITICAL_SECTION mutex;

typedef struct
{
    HANDLE handle;
    char name[256];
} MouseStruct;
static MouseStruct mice[MAX_MICE];


/*
 * The RawInput APIs only exist in Windows XP and later, so you want this
 *  to fail gracefully on earlier systems instead of refusing to start the
 *  process due to missing symbols. To this end, we do a symbol lookup on
 *  User32.dll, etc to get the entry points.
 *
 * A lot of these are available all the way back to the start of win32 in
 *  Windows 95 and WinNT 3.1, but just so you don't have to track down any
 *  import libraries, I've added those here, too. That fits well with the
 *  idea of just adding the sources to your build and going forward.
 */
static UINT (WINAPI *pGetRawInputDeviceList)(
    PRAWINPUTDEVICELIST pRawInputDeviceList,
    PUINT puiNumDevices,
    UINT cbSize
);
/* !!! FIXME: use unicode version */
static UINT (WINAPI *pGetRawInputDeviceInfoA)(
    HANDLE hDevice,
    UINT uiCommand,
    LPVOID pData,
    PUINT pcbSize
);
static BOOL (WINAPI *pRegisterRawInputDevices)(
    PCRAWINPUTDEVICE pRawInputDevices,
    UINT uiNumDevices,
    UINT cbSize
);
static LRESULT (WINAPI *pDefRawInputProc)(
    PRAWINPUT *paRawInput,
    INT nInput,
    UINT cbSizeHeader
);
static UINT (WINAPI *pGetRawInputBuffer)(
    PRAWINPUT pData,
    PUINT pcbSize,
    UINT cbSizeHeader
);
static UINT (WINAPI *pGetRawInputData)(
    HRAWINPUT hRawInput,
    UINT uiCommand,
    LPVOID pData,
    PUINT pcbSize,
    UINT cbSizeHeader
);
static LONG (WINAPI *pRegQueryValueExA)(
    HKEY hKey,
    LPCTSTR lpValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE lpData,
    LPDWORD lpcbData
);
static LONG (WINAPI *pRegOpenKeyExA)(
    HKEY hKey,
    LPCTSTR lpSubKey,
    DWORD ulOptions,
    REGSAM samDesired,
    PHKEY phkResult
);
static LONG (WINAPI *pRegCloseKey)(
    HKEY hKey
);
static HWND (WINAPI *pCreateWindowExA)(
    DWORD dwExStyle,
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam
);
static ATOM (WINAPI *pRegisterClassExA)(
    CONST WNDCLASSEX *lpwcx
);
static LRESULT (WINAPI *pDefWindowProcA)(
    HWND hWnd,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam
);
static BOOL (WINAPI *pUnregisterClassA)(
    LPCTSTR lpClassName,
    HINSTANCE hInstance
);
static HMODULE (WINAPI *pGetModuleHandleA)(
    LPCTSTR lpModuleName
);
static BOOL (WINAPI *pPeekMessageA)(
    LPMSG lpMsg,
    HWND hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax,
    UINT wRemoveMsg
);
static BOOL (WINAPI *pTranslateMessage)(
    const MSG *lpMsg
);
static LRESULT (WINAPI *pDispatchMessageA)(
    const MSG *lpmsg
);
static BOOL (WINAPI *pDestroyWindow)(
    HWND hWnd
);
static void (WINAPI *pInitializeCriticalSection)(
  LPCRITICAL_SECTION lpCriticalSection
);
static void (WINAPI *pEnterCriticalSection)(
  LPCRITICAL_SECTION lpCriticalSection
);
static void (WINAPI *pLeaveCriticalSection)(
  LPCRITICAL_SECTION lpCriticalSection
);
static void (WINAPI *pDeleteCriticalSection)(
  LPCRITICAL_SECTION lpCriticalSection
);

static int symlookup(HMODULE dll, void **addr, const char *sym)
{
    *addr = GetProcAddress(dll, sym);
    if (*addr == NULL)
    {
        FreeLibrary(dll);
        return(0);
    } /* if */

    return(1);
} /* symlookup */

static int find_api_symbols(void)
{
    HMODULE dll;

    if (did_api_lookup)
        return(1);

#define LOOKUP(x) { if (!symlookup(dll, (void **) &p##x, #x)) { fprintf( stderr, "lookup on %s failed\n", #x ); return(0); } }
    dll = GetModuleHandle("user32.dll");
    if (dll == NULL)
        return(0);

    LOOKUP(GetRawInputDeviceInfoA);
    LOOKUP(RegisterRawInputDevices);
    LOOKUP(GetRawInputDeviceList);
    LOOKUP(DefRawInputProc);
    LOOKUP(GetRawInputBuffer);
    LOOKUP(GetRawInputData);
    LOOKUP(CreateWindowExA);
    LOOKUP(RegisterClassExA);
    LOOKUP(UnregisterClassA);
    LOOKUP(DefWindowProcA);
    LOOKUP(PeekMessageA);
    LOOKUP(TranslateMessage);
    LOOKUP(DispatchMessageA);
    LOOKUP(DestroyWindow);

    dll = GetModuleHandle("advapi32.dll");
    if (dll == NULL)
        return(0);

    LOOKUP(RegOpenKeyExA);
    LOOKUP(RegQueryValueExA);
    LOOKUP(RegCloseKey);

    dll = GetModuleHandle("kernel32.dll");
    if (dll == NULL)
        return(0);

    LOOKUP(GetModuleHandleA);
    LOOKUP(InitializeCriticalSection);
    LOOKUP(EnterCriticalSection);
    LOOKUP(LeaveCriticalSection);
    LOOKUP(DeleteCriticalSection);

    #undef LOOKUP

    did_api_lookup = 1;
    return(1);
} /* find_api_symbols */



static void queue_from_rawinput(const RAWINPUT *raw)
{
    int i;
    const RAWINPUTHEADER *header = &raw->header;
    const RAWMOUSE *mouse = &raw->data.mouse;
    SDL_Event event;

    if (raw->header.dwType != RIM_TYPEMOUSE)
        return;

    for (i = 0; i < available_mice; i++)  /* find the device for event. */
    {
        if (mice[i].handle == header->hDevice)
            break;
    } /* for */

    if (i == available_mice)
        return;  /* not found?! */

    /*
     * RAWINPUT packs a bunch of events into one, so we split it up into
     *  a bunch of ManyMouseEvents here and store them in an internal queue.
     *  Then ManyMouse_PollEvent() just shuffles items off that queue
     *  without any complicated processing.
     */

	if( !open_mice[i] )
		/* device not open */
		return;

    /* pEnterCriticalSection(&mutex); */

    if (mouse->usFlags & MOUSE_MOVE_ABSOLUTE)
    {
        /* !!! FIXME: How do we get the min and max values for absmotion? */
		event.motion.x = i;
        event.type = SDL_MOUSEMOTION;
		event.motion.xrel = mouse->lLastX;
        event.motion.yrel = mouse->lLastY;
        SDL_PushEvent( &event );
    }

    else /*if (mouse->usFlags & MOUSE_MOVE_RELATIVE)*/
    {/*
        event.type = MANYMOUSE_EVENT_RELMOTION;
        if (mouse->lLastX != 0)
        {
            event.item = 0;
            event.value = mouse->lLastX;
            queue_event(&event);
        }

        if (mouse->lLastY != 0)
        {
            event.item = 1;
            event.value = mouse->lLastY;
            queue_event(&event);
        }*/
    }

    #define QUEUE_BUTTON(y) { \
        if (mouse->usButtonFlags & RI_MOUSE_BUTTON_##y##_DOWN) { \
            event.button.button = y-1; \
            event.button.x = i; \
			event.type = SDL_MOUSEBUTTONDOWN; \
            SDL_PushEvent(&event); \
        } \
        if (mouse->usButtonFlags & RI_MOUSE_BUTTON_##y##_UP) { \
            event.button.button = y-1; \
            event.button.x = i; \
			event.type = SDL_MOUSEBUTTONUP; \
            SDL_PushEvent(&event); \
        } \
    }

    QUEUE_BUTTON(1);
    QUEUE_BUTTON(2);
    QUEUE_BUTTON(3);
    QUEUE_BUTTON(4);
    QUEUE_BUTTON(5);

    #undef QUEUE_BUTTON

#if 0
    if (mouse->usButtonFlags & RI_MOUSE_WHEEL)
    {
        if (mouse->usButtonData != 0)  /* !!! FIXME: can this ever be zero? */
        {
            event.type = MANYMOUSE_EVENT_SCROLL;
            event.item = 0;  /* !!! FIXME: horizontal wheel? */
            event.value = (mouse->usButtonData > 0) ? 1 : -1;
            queue_event(&event);
        } /* if */
    } /* if */
#endif
    /* pLeaveCriticalSection(&mutex); */
} /* queue_from_rawinput */


static void wminput_handler(WPARAM wParam, LPARAM lParam)
{
    UINT dwSize = 0;
    LPBYTE lpb;

    pGetRawInputData((HRAWINPUT) lParam, RID_INPUT, NULL, &dwSize,
                      sizeof (RAWINPUTHEADER));

    if (dwSize < sizeof (RAWINPUT))
        return;  /* unexpected packet? */

    lpb = (LPBYTE) alloca(dwSize);
    if (lpb == NULL) 
        return;
    if (pGetRawInputData((HRAWINPUT) lParam, RID_INPUT, lpb, &dwSize,
                          sizeof (RAWINPUTHEADER)) != dwSize)
        return;

    queue_from_rawinput((RAWINPUT *) lpb);
} /* wminput_handler */


static LRESULT CALLBACK RawWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (Msg == WM_INPUT)
        wminput_handler(wParam, lParam);

    else if (Msg == WM_DESTROY)
        return(0);

    return pDefWindowProcA(hWnd, Msg, wParam, lParam);
} /* RawWndProc */


static int init_event_queue(void)
{
    HINSTANCE hInstance = pGetModuleHandleA(NULL);
    WNDCLASSEX wce;
    RAWINPUTDEVICE rid;

    ZeroMemory(&wce, sizeof (wce));
    wce.cbSize = sizeof(WNDCLASSEX);
    wce.lpfnWndProc = RawWndProc;
    wce.lpszClassName = class_name;
    wce.hInstance = hInstance;
    class_atom = pRegisterClassExA(&wce);
    if (class_atom == 0)
        return(0);

    raw_hwnd = pCreateWindowExA(0, class_name, win_name, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, HWND_MESSAGE, NULL, hInstance, NULL);

    if (raw_hwnd == NULL)
        return(0);

    pInitializeCriticalSection(&mutex);

    ZeroMemory(&rid, sizeof (rid));
    rid.usUsagePage = 1; /* GenericDesktop page */
    rid.usUsage = 2; /* GeneralDestop Mouse usage. */
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = raw_hwnd;
    if (!pRegisterRawInputDevices(&rid, 1, sizeof (rid)))
    {
        pDeleteCriticalSection(&mutex);
        return(0);
    } /* if */

    return(1);
} /* init_event_queue */


static void cleanup_window(void)
{
    if (raw_hwnd)
    {
        MSG Msg;
        pDestroyWindow(raw_hwnd);
        while (pPeekMessageA(&Msg, raw_hwnd, 0, 0, PM_REMOVE))
        {
            pTranslateMessage(&Msg);
            pDispatchMessageA(&Msg);
        } /* while */
        raw_hwnd = 0;
    } /* if */

    if (class_atom)
    {
        pUnregisterClassA(class_name, pGetModuleHandleA(NULL));
        class_atom = 0;
    } /* if */
} /* cleanup_window */


static int accept_device(const RAWINPUTDEVICELIST *dev)
{
    const char rdp_ident[] = "\\??\\Root#RDP_MOU#0000#";
    char *buf = NULL;
    UINT ct = 0;

    if (dev->dwType != RIM_TYPEMOUSE)
        return(0);  /* keyboard or some other fruity thing. */

    if (pGetRawInputDeviceInfoA(dev->hDevice, RIDI_DEVICENAME, NULL, &ct) < 0)
        return(0);

    /* ct == is chars, not bytes, but we used the ASCII version. */
    buf = (char *) alloca(ct);
    if (buf == NULL)
        return(0);

    if (pGetRawInputDeviceInfoA(dev->hDevice, RIDI_DEVICENAME, buf, &ct) < 0)
        return(0);

    /*
     * Apparently there's a fake "RDP" device...I guess this is
     *  "Remote Desktop Protocol" for controlling the system pointer
     *  remotely via Windows Remote Desktop, but that's just a guess.
     * At any rate, we don't want that device, so skip it if detected.
     *
     * Idea for this found here:
     *   http://link.mywwwserver.com/~jstookey/arcade/rawmouse/raw_mouse.c
     */

    /* avoiding memcmp here so we don't get a C runtime dependency... */
    if (ct >= sizeof (rdp_ident) - 1)
    {
        int i;
        for (i = 0; i < sizeof (rdp_ident) - 1; i++)
        {
            if (buf[i] != rdp_ident[i])
                break;
        } /* for */

        if (i == sizeof (rdp_ident) - 1)
            return(0);  /* this is an RDP thing. Skip this device. */
    } /* if */

    return(1);  /* we want this device. */
} /* accept_device */


/* !!! FIXME: this code sucks. */
static void get_device_product_name(char *name, size_t namesize,
                                     const RAWINPUTDEVICELIST *dev)
{
    const char regkeyroot[] = "System\\CurrentControlSet\\Enum\\";
    const char default_device_name[] = "Unidentified input device";
    DWORD outsize = namesize;
    DWORD regtype = REG_SZ;
    char *buf = NULL;
    char *ptr = NULL;
    char *keyname = NULL;
    UINT i = 0;
    UINT ct = 0;
    LONG rc = 0;
    HKEY hkey;

    *name = '\0';  /* really insane default. */
    if (sizeof (default_device_name) >= namesize)
        return;

    /* in case we can't stumble upon something better... */
    CopyMemory(name, default_device_name, sizeof (default_device_name));

    if (pGetRawInputDeviceInfoA(dev->hDevice, RIDI_DEVICENAME, NULL, &ct) < 0)
        return;

    /* ct == is chars, not bytes, but we used the ASCII version. */
    buf = (char *) alloca(ct+1);
    keyname = (char *) alloca(ct + sizeof (regkeyroot));
    if ((buf == NULL) || (keyname == NULL))
        return;

    if (pGetRawInputDeviceInfoA(dev->hDevice, RIDI_DEVICENAME, buf, &ct) < 0)
        return;

    /*
     * This string tap dancing gets us a registry keyname in this form:
     *   SYSTEM\CurrentControlSet\Enum\BUSTYPE\DEVICECLASS\DEVICEID
     * (those are my best-guess for the actual elements, but the format
     *  appears to be sound.)
     */
    ct -= 4;
    buf += 4;  /* skip the "\\??\\" on the front of the string. */
    for (i = 0, ptr = buf; i < ct; i++, ptr++)  /* convert '#' to '\\' ... */
    {
        if (*ptr == '#')
            *ptr = '\\';
        else if (*ptr == '{')  /* hit the GUID part of the string. */
            break;
    } /* for */

    *ptr = '\0';
    CopyMemory(keyname, regkeyroot, sizeof (regkeyroot) - 1);
    CopyMemory(keyname + (sizeof (regkeyroot) - 1), buf, i + 1);
    rc = pRegOpenKeyExA(HKEY_LOCAL_MACHINE, keyname, 0, KEY_READ, &hkey);
    if (rc != ERROR_SUCCESS)
        return;

    rc = pRegQueryValueExA(hkey, "DeviceDesc", NULL, &regtype, name, &outsize);
    pRegCloseKey(hkey);
    if (rc != ERROR_SUCCESS)
    {
        /* msdn says failure may mangle the buffer, so default it again. */
        CopyMemory(name, default_device_name, sizeof (default_device_name));
        return;
    } /* if */
    name[namesize-1] = '\0';  /* just in case. */
} /* get_device_product_name */


static void init_mouse(const RAWINPUTDEVICELIST *dev)
{
    MouseStruct *mouse = &mice[available_mice];

    if (accept_device(dev))
    {
        ZeroMemory(mouse, sizeof (MouseStruct));
        get_device_product_name(mouse->name, sizeof (mouse->name), dev);
        mouse->handle = dev->hDevice;
        available_mice++;  /* we're good. */
    } /* if */
} /* init_mouse */


static int windows_wminput_init(void)
{
    RAWINPUTDEVICELIST *devlist = NULL;
    UINT ct = 0;
    UINT i;

    available_mice = 0;

    if (!find_api_symbols())  /* only supported on WinXP and later. */
        return(0);

    pGetRawInputDeviceList(NULL, &ct, sizeof (RAWINPUTDEVICELIST));
    if (ct == 0)  /* no devices. */
        return(0);

    devlist = (PRAWINPUTDEVICELIST) alloca(sizeof (RAWINPUTDEVICELIST) * ct);
    pGetRawInputDeviceList(devlist, &ct, sizeof (RAWINPUTDEVICELIST));
    for (i = 0; i < ct; i++)
        init_mouse(&devlist[i]);

    if (!init_event_queue())
    {
        cleanup_window();
        available_mice = 0;
    } /* if */

    return(available_mice);
} /* windows_wminput_init */


static void windows_wminput_quit(void)
{
    /* unregister WM_INPUT devices... */
    RAWINPUTDEVICE rid;
    ZeroMemory(&rid, sizeof (rid));
    rid.usUsagePage = 1; /* GenericDesktop page */
    rid.usUsage = 2; /* GeneralDestop Mouse usage. */
    rid.dwFlags |= RIDEV_REMOVE;
    pRegisterRawInputDevices(&rid, 1, sizeof (rid));
    cleanup_window();
    available_mice = 0;
    pDeleteCriticalSection(&mutex);
} /* windows_wminput_quit */


static const char *windows_wminput_name(unsigned int index)
{
    if (index < available_mice)
        return(mice[index].name);
    return(NULL);
} /* windows_wminput_name */

#if 0
/*
 * Windows doesn't send a WM_INPUT event when you unplug a mouse,
 *  so we try to do a basic query by device handle here; if the
 *  query fails, we assume the device has vanished and generate a
 *  disconnect.
 */
static int check_for_disconnects()
{
    /*
     * (i) is static so we iterate through all mice round-robin and check
     *  one mouse per call to ManyMouse_PollEvent(). This makes this test O(1).
     */
    static unsigned int i = 0;
    MouseStruct *mouse = NULL;

    if (++i >= available_mice)  /* check first in case of redetect */
        i = 0;

    mouse = &mice[i];
    if (mouse->handle != NULL)  /* not NULL == still plugged in. */
    {
        UINT size = 0;
        UINT rc = pGetRawInputDeviceInfoA(mouse->handle, RIDI_DEVICEINFO,
                                          NULL, &size);
        if (rc == (UINT) -1)  /* failed...probably unplugged... */
        {
            mouse->handle = NULL;
            ev->type = MANYMOUSE_EVENT_DISCONNECT;
            ev->device = i;
            return(1);
        } /* if */
    } /* if */

    return(0);  /* no disconnect event this time. */
} /* check_for_disconnects */
#endif /* 0 */

static int windows_wminput_poll()
{
    MSG Msg;  /* run the queue for WM_INPUT messages, etc ... */
    int found = 0;

    if (!found)
    {
        /* pump Windows for new hardware events... */
        while (pPeekMessageA(&Msg, raw_hwnd, 0, 0, PM_REMOVE))
        {
            fprintf( stderr, "here2\n" );
            pTranslateMessage(&Msg);
            pDispatchMessageA(&Msg);
        } /* while */
    } /* if */

    /*
     * Check for disconnects if queue is totally empty and Windows didn't
     *  report anything new at this time. This ensures that we don't send a
     *  disconnect event through ManyMouse and then later give a valid
     *  event to the app for a device that is now missing.
     */
	/*
    if (!found)
        found = check_for_disconnects(ev);
	*/
    return(found);
} /* windows_wminput_poll */


/* end of windows_wminput.c ... */

#endif /* defined( __WINDOWS_RAWINPUT__ ) */

/*****************************************************************************
 *
 *  Spencer's DirectInput HID mouse implementation
 *  Has pre-WinXP support, but doesn't work with multiple mice on WinXP.
 *
 *****************************************************************************/

#include <dinput.h>


struct win32_mouse
{
	LPDIRECTINPUTDEVICE lpdiMouse;
	char open;
	DIMOUSESTATE previous_state;
};
typedef struct win32_mouse win32_mouse;

static win32_mouse * wmice;
//static int available_mice;
static LPDIRECTINPUT lpdi;
static int use_rawinput = 0;


static BOOL CALLBACK DIEnumMouseProc1( LPCDIDEVICEINSTANCE lpddi,
									   LPVOID pvRef )
{
	available_mice++;		

	return DIENUM_CONTINUE;
}

static BOOL CALLBACK DIEnumMouseProc2( LPCDIDEVICEINSTANCE lpddi,
									   LPVOID pvRef )
{
	int * lpInt = pvRef;
	int i = * ( int * ) lpInt;
	GUID guid = lpddi->guidProduct;

	if( i >= available_mice )
	{
		return DIENUM_CONTINUE;
	}

	if( lpdi->lpVtbl->CreateDevice( lpdi, &guid, &wmice[i].lpdiMouse, NULL )
		!= DI_OK )
	{
		wmice[i].open = -1;
		return DIENUM_CONTINUE;
	}

	if( wmice[i].lpdiMouse->lpVtbl->SetDataFormat( wmice[i].lpdiMouse, &c_dfDIMouse ) != DI_OK )
	{
		wmice[i].open = -1;
		return DIENUM_CONTINUE;
	}

	return DIENUM_CONTINUE;
}

void Mouse_init()
{
	HINSTANCE hInstance = GetModuleHandleA( NULL );
	int i = 0;

	available_mice = 0;
	use_rawinput = 0;

#ifdef __WINDOWS_RAWINPUT__
	open_mice = NULL;

	if( windows_wminput_init() )
	{
		fprintf( stderr, "using rawinput\n%d mice available\n", available_mice );
		use_rawinput = 1;
		open_mice = ( char * ) malloc( sizeof( char * ) * available_mice );
		memset( open_mice, 0, sizeof( char * ) * available_mice );
		return;
	}

    fprintf( stderr, "using directinput\n" );
#endif /* __WINDOWS_RAWINPUT__ */

	if( DirectInputCreate( hInstance, DIRECTINPUT_VERSION, &lpdi, NULL) != DI_OK )
	{
		return;
	}

	if( lpdi->lpVtbl->EnumDevices( lpdi, DIDEVTYPE_MOUSE, DIEnumMouseProc1, 
								   NULL, DIEDFL_ATTACHEDONLY ) != DI_OK )
	{
		return;
	}

	if( available_mice )
	{
		wmice = ( win32_mouse * ) malloc( sizeof( win32_mouse ) * available_mice );
		memset( wmice, 0, sizeof( win32_mouse ) * available_mice );


		if( lpdi->lpVtbl->EnumDevices( lpdi, DIDEVTYPE_MOUSE, DIEnumMouseProc2, 
									   &i, DIEDFL_ATTACHEDONLY ) != DI_OK )
		{
			free( wmice );
			return;
		}
	}
}

void Mouse_poll()
{
 	int i, j;
	DIMOUSESTATE dims;

#ifdef __WINDOWS_RAWINPUT__
	if( use_rawinput )
	{
 		windows_wminput_poll();
		return;
	}
#endif /* WINDOWS_RAWINPUT__ */

	for( i = 0; i < available_mice; i++ )
	{
		if( wmice[i].open == 1 )
		{
			wmice[i].lpdiMouse->lpVtbl->GetDeviceState( wmice[i].lpdiMouse, 
													   sizeof( dims ), &dims );
			if( dims.lX != 0 || dims.lY != 0 )
			{
				SDL_Event event;
				event.type = SDL_MOUSEMOTION;
				event.motion.x = i;
				event.motion.xrel = dims.lX;
				event.motion.yrel = dims.lY;
				SDL_PushEvent( &event );
			}

			for( j = 0; j < 4; j++ )
			{
				if( ( dims.rgbButtons[j] & 0x80 ) && 
					!( wmice[i].previous_state.rgbButtons[j] & 0x80 ) )
				{
					SDL_Event event;
					event.type = SDL_MOUSEBUTTONDOWN;
					event.button.button = j;
					event.button.x = i;
					SDL_PushEvent( &event );
				}

				else if( !( dims.rgbButtons[j] & 0x80 ) && 
						 wmice[i].previous_state.rgbButtons[j] & 0x80 )
				{
					SDL_Event event;
					event.type = SDL_MOUSEBUTTONUP;
					event.button.button = j;
					event.button.x = i;
					SDL_PushEvent( &event );
				}
			}

			wmice[i].previous_state = dims;
		}
	}
}

void Mouse_quit()
{
#ifdef __WINDOWS_RAWINPUT__
	if( use_rawinput )
	{
		windows_wminput_quit();
		free( open_mice );
		return;
	}
#endif /* __WINDOWS_RAWINPUT__ */
	if( lpdi != NULL )
		lpdi->lpVtbl->Release( lpdi );
	if( wmice != NULL )
		free( wmice );
}

int Mouse_open( int mouse )
{
#ifdef __WINDOWS_RAWINPUT__
	if( use_rawinput )
	{
		assert( open_mice != NULL );
		if( mouse < 0 || mouse >= available_mice )
			return -1;
		open_mice[mouse] = 1;
		return 0;
	}
#endif /* __WINDOWS_RAWINPUT__ */
	if( mouse >= 0 && mouse < available_mice && wmice[mouse].open != -1 )
	{
		wmice[mouse].open = 1;
		wmice[mouse].lpdiMouse->lpVtbl->Acquire( wmice[mouse].lpdiMouse );
		return 0;
	}

    return -1;
}

int Mouse_close( int mouse )
{
#ifdef __WINDOWS_RAWINPUT__
	if( use_rawinput )
	{
		assert( open_mice != NULL );
		if( mouse < 0 || mouse >= available_mice )
			return -1;
		open_mice[mouse] = 0;
		return 0;
	}
#endif /* WINDOWS_RAWINPUT__ */
	if( mouse >= 0 && mouse < available_mice && wmice[mouse].open != -1 )
	{
		wmice[mouse].lpdiMouse->lpVtbl->Unacquire( wmice[mouse].lpdiMouse );
		wmice[mouse].open = 0;
		return 0;
	}

    return -1;
}

int Mouse_count()
{
    return available_mice;
}


#else /* Linux */

void Mouse_init()
{
}

void Mouse_poll()
{
}

void Mouse_quit()
{
}

int Mouse_open( int mouse )
{
    return -1;
}

int Mouse_close( int mouse )
{
    return -1;
}

int Mouse_count()
{
    return 0;
}

#endif /* __PLATFORM_MACOSX__ */

