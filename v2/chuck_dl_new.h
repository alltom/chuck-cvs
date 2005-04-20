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
// name: chuck_dl.h
// desc: chuck dynamic linking header
//
// authors: Ge Wang (gewang@cs.princeton.edu)
//          Perry R. Cook (prc@cs.princeton.edu)
// mac os code based on apple's open source
//
// date: spring 2004 - 1.1
//       spring 2005 - 1.2
//-----------------------------------------------------------------------------
#ifndef __CHUCK_DL_H__
#define __CHUCK_DL_H__

#include "chuck_def.h"
#include "chuck_oo.h"
#include <string>
#include <vector>
#include <map>


// forward references
struct Chuck_DL_Query;
struct Chuck_UGen;


// param conversion - to extract values from ARGS to functions
#define GET_CK_FLOAT(ptr)      (*(t_CKFLOAT *)ptr)
#define GET_CK_SINGLE(ptr)     (*(float *)ptr)
#define GET_CK_DOUBLE(ptr)     (*(double *)ptr)
#define GET_CK_INT(ptr)        (*(t_CKINT *)ptr)
#define GET_CK_UINT(ptr)       (*(t_CKUINT *)ptr)
#define GET_CK_TIME(ptr)       (*(t_CKTIME *)ptr)
#define GET_CK_DUR(ptr)        (*(t_CKDUR *)ptr)
#define GET_CK_STRING(ptr)     (*(char **)ptr)

// param conversion with pointer advance
#define GET_NEXT_FLOAT(ptr)    (*((t_CKFLOAT *&)ptr)++)
#define GET_NEXT_SINGLE(ptr)   (*((float *&)ptr)++)
#define GET_NEXT_DOUBLE(ptr)   (*((double *&)ptr)++)
#define GET_NEXT_INT(ptr)      (*((t_CKINT *&)ptr)++)
#define GET_NEXT_UINT(ptr)     (*((t_CKUINT *&)ptr)++)
#define GET_NEXT_TIME(ptr)     (*((t_CKTIME *&)ptr)++)
#define GET_NEXT_DUR(ptr)      (*((t_CKDUR *&)ptr)++)
#define GET_NEXT_STRING(ptr)   (*((char * *&)ptr)++)

// param conversion
#define SET_CK_FLOAT(ptr,v)      (*(t_CKFLOAT *&)ptr=v)
#define SET_CK_SINGLE(ptr,v)     (*(float *&)ptr=v)
#define SET_CK_DOUBLE(ptr,v)     (*(double *&)ptr=v)
#define SET_CK_INT(ptr,v)        (*(t_CKINT *&)ptr=v)
#define SET_CK_UINT(ptr,v)       (*(t_CKUINT *&)ptr=v)
#define SET_CK_TIME(ptr,v)       (*(t_CKTIME *&)ptr=v)
#define SET_CK_DUR(ptr,v)        (*(t_CKDUR *&)ptr=v)
#define SET_CK_STRING(ptr,v)     (*(char *&)ptr=v)

// param conversion with pointer advance
#define SET_NEXT_FLOAT(ptr,v)    (*((t_CKFLOAT *&)ptr)++=v)
#define SET_NEXT_SINGLE(ptr,v)   (*((float *&)ptr)++=v)
#define SET_NEXT_DOUBLE(ptr,v)   (*((double *&)ptr)++=v)
#define SET_NEXT_INT(ptr,v)      (*((t_CKINT *&)ptr)++=v)
#define SET_NEXT_UINT(ptr,v)     (*((t_CKUINT *&)ptr)++=v)
#define SET_NEXT_TIME(ptr,v)     (*((t_CKTIME *&)ptr)++=v)
#define SET_NEXT_DUR(ptr,v)      (*((t_CKDUR *&)ptr)++=v)
#define SET_NEXT_STRING(ptr,v)   (*((char * *&)ptr)++=v)


// chuck dll export linkage and calling convention
#if defined (__PLATFORM_WIN32__) 
  #define CK_DLL_LINKAGE extern "C" __declspec( dllexport )
#else
  #define CK_DLL_LINKAGE extern "C" 
#endif 

// calling convention of functions provided by chuck to the dll
#if defined(__WINDOWS_DS__)
  #define CK_DLL_CALL    _cdecl
#else
  #define CK_DLL_CALL
#endif

// macro for defining ChucK DLL export functions
// example: CK_DLL_EXPORT(int) foo() { return 1; }
#define CK_DLL_EXPORT(type) CK_DLL_LINKAGE type CK_DLL_CALL
// macro for defining ChucK DLL export query-functions
// example: CK_DLL_QUERY
#define CK_DLL_QUERY CK_DLL_EXPORT(t_CKBOOL) ck_query( Chuck_DL_Query * QUERY )
// macro for defining ChucK DLL export constructors
// example: CK_DLL_CTOR(foo)
#define CK_DLL_CTOR(name) CK_DLL_EXPORT(void) name( Chuck_Object * self, void * ARGS )
// macro for defining ChucK DLL export destructors
// example: CK_DLL_DTOR(foo)
#define CK_DLL_DTOR(name) CK_DLL_EXPORT(void) name( Chuck_Object * self )
// macro for defining ChucK DLL export member functions
// example: CK_DLL_MFUN(foo)
#define CK_DLL_MFUN(name) CK_DLL_EXPORT(void) name( Chuck_Object * self, void * ARGS, Chuck_DL_Return * RETURN )
// macro for defining ChucK DLL export static functions
// example: CK_DLL_SFUN(foo)
#define CK_DLL_SFUN(name) CK_DLL_EXPORT(void) name( void * ARGS, Chuck_DL_Return * RETURN )

// macros for DLL exports
// example: DLL_QUERY  query( Chuck_DL_Query * QUERY )
// example: DLL_FUNC   foo( void * ARGS, Chuck_DL_Return * RETURN )
// example: DLL_UGEN_F foo_tick( Chuck_UGen * self, SAMPLE, SAMPLE * out );
#define DLL_QUERY   CK_DLL_EXPORT(t_CKBOOL)
#define DLL_FUNC    CK_DLL_EXPORT(void)
#define DLL_UGEN_F  CK_DLL_EXPORT(t_CKBOOL)


//-----------------------------------------------------------------------------
// dynamic linking class interface prototypes
//-----------------------------------------------------------------------------
extern "C" {
// query
typedef CK_DLL_EXPORT(t_CKBOOL) (* f_ck_query)( Chuck_DL_Query * QUERY );
// object
typedef CK_DLL_EXPORT(void) (* f_ctor)( Chuck_Object * self, void * ARGS );
typedef CK_DLL_EXPORT(void) (* f_dtor)( Chuck_Object * self );
typedef CK_DLL_EXPORT(void) (* f_mfun)( Chuck_Object * self, void * ARGS, Chuck_DL_Return & RETURN );
typedef CK_DLL_EXPORT(void) (* f_sfun)( void * ARGS, Chuck_DL_Return & RETURN );
// ugen specific
typedef CK_DLL_EXPORT(t_CKBOOL) (* f_tick)( Chuck_UGen * self, SAMPLE in, SAMPLE * out );
typedef CK_DLL_EXPORT(t_CKBOOL) (* f_pmsg)( Chuck_UGen * self, const char * msg, void * ARGS );
}


// default name in DLL/ckx to look for
#define CK_QUERY_FUNC        "ck_query"


//-----------------------------------------------------------------------------
// chuck DLL query functions, implemented on chuck side for portability
//-----------------------------------------------------------------------------
extern "C" {
// set name of ckx
typedef void ( CK_DLL_CALL * f_ck_setname)( Chuck_DL_Query * query, const char * name );

// begin class/namespace, can be nested
typedef void (CK_DLL_CALL * f_begin_class)( Chuck_DL_Query * query, const char * name, const char * parent );
// add constructor, must be betwen begin/end_class : use f_add_arg to add arguments immediately after
typedef void (CK_DLL_CALL * f_add_ctor)( Chuck_DL_Query * query, f_ctor ctor );
// add destructor - cannot have args
typedef void (CK_DLL_CALL * f_add_dtor)( Chuck_DL_Query * query, f_dtor dtor );
// add member function - args to follow
typedef void (CK_DLL_CALL * f_add_mfun)( Chuck_DL_Query * query, f_mfun mfun, const char * name );
// add static function - args to follow
typedef void (CK_DLL_CALL * f_add_sfun)( Chuck_DL_Query * query, f_sfun sfun, const char * name );
// add member/static variable
typedef void (CK_DLL_CALL * f_add_var)( Chuck_DL_Query * query, const char * type, const char * name,
               t_CKBOOL is_const ); // TODO: public/protected/private
// add arg - follows ctor mfun sfun
typedef void (CK_DLL_CALL * f_add_arg)( Chuck_DL_Query * query, const char * type, const char * name );
// ** functions for adding unit generators, must extend ugen
typedef void (CK_DLL_CALL * f_add_ugen_func)( Chuck_DL_Query * query, f_tick tick, f_pmsg pmsg );
// ** add a ugen control
typedef void (CK_DLL_CALL * f_add_ugen_ctrl)( Chuck_DL_Query * query, f_ctrl ctrl, f_cget cget, 
                                              const char * type, const char * name );
// end class/namespace - must correspondent with begin_class.  returns false on error
typedef t_CKBOOL (CK_DLL_CALL * f_end_class)( Chuck_DL_Query * query );
}




#endif
