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
// date: spring 2004
//-----------------------------------------------------------------------------
#ifndef __CHUCK_DL_H__
#define __CHUCK_DL_H__

#include "chuck_def.h"
#include "chuck_oo.h"
#include "chuck_ugen.h"
#include <string>
#include <vector>
#include <map>
using namespace std;


// chuck dll export linkage and calling convention
#define CK_DLL_LINKAGE       extern "C" 
#if defined(__WINDOWS_DS__)
#define CK_DLL_CALL          _cdecl
#else
#define CK_DLL_CALL
#endif

// macro for defining ChucK DLL export functions
// example: CK_DLL_EXPORT(int) foo() { return 1; }
#define CK_DLL_EXPORT(type)  CK_DLL_LINKAGE type CK_DLL_CALL
// macro for defining ChucK DLL export query-functions
// example: CK_DLL_QUERY
#define CK_DLL_QUERY CK_DLL_EXPORT(t_CKBOOL) ck_query( Chuck_DL_Query * QUERY )
// macro for defining ChucK DLL export functions
// example: CK_DLL_FUNC(foo)
#define CK_DLL_FUNC(name) CK_DLL_EXPORT(void) name( void * ARGS, Chuck_DL_Return * RETURN )

// macros for DLL exports
// example: DLL_QUERY query( Chuck_DL_Query * QUERY )
// example: DLL_FUNC  foo( void * ARGS, Chuck_DL_Return * RETURN )
#define DLL_QUERY CK_DLL_EXPORT(t_CKBOOL)
#define DLL_FUNC CK_DLL_EXPORT(void)

// macros for unit generator export functions
// example: UGEN_CTOR foo_ctor( t_CKTIME now )
// example: UGEN_DTOR foo_dtor( t_CKTIME now, void * data )
// example: UGEN_TICK foo_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out )
// example: UGEN_CTRL foo_a( t_CKTIME now, void * data, void * value )
// example: UGEN_PMSG foo_pmsg( t_CKTIME now, void * data, const char * msg, void * value );
#define UGEN_CTOR CK_DLL_EXPORT(void *)
#define UGEN_DTOR CK_DLL_EXPORT(void)
#define UGEN_TICK CK_DLL_EXPORT(t_CKBOOL)
#define UGEN_CTRL CK_DLL_EXPORT(void)
#define UGEN_PMSG CK_DLL_EXPORT(t_CKBOOL)


// forward reference
struct Chuck_DL_Query;
struct Chuck_DL_Proto;
union  Chuck_DL_Return;

// chuck DLL interface
#define CK_QUERY_FUNC        "ck_query"
typedef t_CKBOOL (* CK_DLL_CALL f_ck_query)( Chuck_DL_Query * QUERY );
typedef void (* CK_DLL_CALL f_ck_func)( void * ARGS, Chuck_DL_Return * RETURN );

// chuck DLL query
typedef void (* CK_DLL_CALL f_ck_addexport)( Chuck_DL_Query * query, const char * type, const char * name, f_ck_func addr, t_CKBOOL is_func );
typedef void (* CK_DLL_CALL f_ck_addparam)( Chuck_DL_Query * query, const char * type, const char * name );
// functions for adding unit generators
typedef void (* CK_DLL_CALL f_ck_ugen_add)( Chuck_DL_Query * query, const char * name, void * reserved );
typedef void (* CK_DLL_CALL f_ck_ugen_func)( Chuck_DL_Query * query, f_ctor ctor, f_dtor dtor, f_tick tick, f_pmsg pmsg );
typedef void (* CK_DLL_CALL f_ck_ugen_ctrl)( Chuck_DL_Query * query, f_ctrl ctrl, const char * type, const char * name );
// set name
typedef void (* CK_DLL_CALL f_ck_setname)( Chuck_DL_Query * query, const char * name );

// internal implementation header
extern "C" {
void CK_DLL_CALL __ck_addexport( Chuck_DL_Query * query, const char * type, const char * name, f_ck_func addr, t_CKBOOL is_func );
void CK_DLL_CALL __ck_addparam( Chuck_DL_Query * query, const char * type, const char * name );
void CK_DLL_CALL __ck_ugen_add( Chuck_DL_Query * query, const char * name, void * reserved );
void CK_DLL_CALL __ck_ugen_func( Chuck_DL_Query * query, f_ctor ctor, f_dtor dtor, f_tick tick, f_pmsg pmsg );
void CK_DLL_CALL __ck_ugen_ctrl( Chuck_DL_Query * query, f_ctrl ctrl, const char * type, const char * name );
void CK_DLL_CALL __ck_setname( Chuck_DL_Query * query, const char * name );
}

// param conversion
#define GET_CK_FLOAT(ptr)      (*(t_CKFLOAT *)ptr)
#define GET_CK_SINGLE(ptr)     (*(float *)ptr)
#define GET_CK_DOUBLE(ptr)     (*(double *)ptr)
#define GET_CK_INT(ptr)        (*(int *)ptr)
#define GET_CK_UINT(ptr)       (*(t_CKUINT *)ptr)
#define GET_CK_TIME(ptr)       (*(t_CKTIME *)ptr)
#define GET_CK_DUR(ptr)        (*(t_CKDUR *)ptr)
#define GET_CK_STRING(ptr)     (*(char *)ptr)

// param conversion with pointer advance
#define GET_NEXT_FLOAT(ptr)    (*((t_CKFLOAT *)ptr)++)
#define GET_NEXT_SINGLE(ptr)   (*((float *)ptr)++)
#define GET_NEXT_DOUBLE(ptr)   (*((double *)ptr)++)
#define GET_NEXT_INT(ptr)      (*((int *)ptr)++)
#define GET_NEXT_UINT(ptr)     (*((t_CKUINT *)ptr)++)
#define GET_NEXT_TIME(ptr)     (*((t_CKTIME *)ptr)++)
#define GET_NEXT_DUR(ptr)      (*((t_CKDUR *)ptr)++)
#define GET_NEXT_STRING(ptr)   (*((char * *)ptr)++)


//-----------------------------------------------------------------------------
// name: struct Chuck_DL_Proto
// desc: dynamic link proto
//-----------------------------------------------------------------------------
struct Chuck_DL_Proto
{
public: // these should not be used directly by the DLL
    string name;       // name of the thing
    string type;       // (return) type
    t_CKBOOL is_func;  // is a func?
    vector<Chuck_Info_Param> params;  // see Chuck_Info_Param in chuck_ugen.h
    f_ck_func addr;    // addr in the DLL

    // constructor
    Chuck_DL_Proto( const char * t = "", const char * n = "", 
                    f_ck_func a = NULL, t_CKBOOL f = TRUE )
    { type = t; name = n; addr = a, is_func = f; }

    // clear proto
    void clear()
    { type = ""; name = ""; addr = NULL; params.clear(); }

    // add a param
    void add_param( const char * t, const char * n )
    { params.push_back( Chuck_Info_Param( t, n ) ); }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_DL_Query
// desc: dynamic link query
//-----------------------------------------------------------------------------
struct Chuck_DL_Query
{
public: // call these from the DLL
    f_ck_addexport add_export;  // call this to add export
    f_ck_addparam  add_param;   // call this to add parameter to last export
    f_ck_ugen_add  ugen_add;    // call this to add a ugen
    f_ck_ugen_func ugen_func;   // call this (once) to set functions for last ugen
    f_ck_ugen_ctrl ugen_ctrl;   // call this (>= 0 times) to add ctrl to last ugen
    f_ck_setname   set_name;    // call this to set name
    const char * dll_name;      // name of the DLL
    void * reserved;
    
public: // these should not be used directly by the DLL
        vector<Chuck_DL_Proto> dll_exports;
    vector<Chuck_UGen_Info> ugen_exports;
    int linepos;
    
    // constructor
    Chuck_DL_Query()
    { add_export = __ck_addexport; add_param = __ck_addparam;
    ugen_add = __ck_ugen_add; ugen_func = __ck_ugen_func; 
    ugen_ctrl = __ck_ugen_ctrl; set_name = __ck_setname;
    dll_name = "[noname]"; reserved = NULL; linepos = 0; }
    
    // clear the query
    void clear() { dll_exports.clear(); ugen_exports.clear(); }
};




//------------------------------------------------------------------------------
// name: union Chuck_DL_Return
// desc: dynamic link return function return struct
//------------------------------------------------------------------------------
union Chuck_DL_Return
{
    int    v_int;
    double  v_float;
    char * v_string;
    void * v_user;
    t_CKUINT v_uint;
    
    Chuck_DL_Return() { v_int = 0; }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_DLL
// desc: dynamic link library
//-----------------------------------------------------------------------------
struct Chuck_DLL : public Chuck_VM_Object
{
public:
    t_CKBOOL load( const char * filename, const char * func = CK_QUERY_FUNC,
                   t_CKBOOL lazy = FALSE );
    t_CKBOOL load( f_ck_query query_func, t_CKBOOL lazy = FALSE );
    void * get_addr( const char * symbol );
    const char * last_error() const;
    t_CKBOOL unload( );

    t_CKBOOL good() const;
    const Chuck_DL_Query * query( );
    const Chuck_DL_Proto * proto( const char * symbol );
    const Chuck_UGen_Info * ugen( const char * symbol );

    Chuck_DLL( const char * id = NULL ) {
        m_handle = NULL; m_done_query = FALSE;
        this->init_ref();
        m_id = id ? id : ""; m_query_func = NULL; }
    ~Chuck_DLL() {
        this-unload(); }

protected:
    void * m_handle;
    string m_last_error;
    string m_filename;
    string m_id;
    string m_func;
    t_CKBOOL m_done_query;
    
    f_ck_query m_query_func;
    Chuck_DL_Query m_query;
    map<string, Chuck_DL_Proto *> m_name2proto;
    map<string, Chuck_UGen_Info *> m_name2ugen;
};




// dlfcn interface
#if defined(__MACOSX_CORE__)

  #ifdef __cplusplus
  extern "C" {
  #endif

  void * dlopen( const char * path, int mode );
  void * dlsym( void * handle, const char * symbol );
  const char * dlerror( void );
  int dlclose( void * handle );

  #define RTLD_LAZY         0x1
  #define RTLD_NOW          0x2
  #define RTLD_LOCAL        0x4
  #define RTLD_GLOBAL       0x8
  #define RTLD_NOLOAD       0x10
  #define RTLD_SHARED       0x20	/* not used, the default */
  #define RTLD_UNSHARED     0x40
  #define RTLD_NODELETE     0x80
  #define RTLD_LAZY_UNDEF   0x100

  #ifdef __cplusplus
  }
  #endif

#else

  #include "dlfcn.h"

#endif




#endif
