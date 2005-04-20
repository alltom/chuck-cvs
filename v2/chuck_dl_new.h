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
struct Chuck_DL_Return;
struct Chuck_DL_Class;
struct Chuck_DL_Func;
struct Chuck_DL_Value;
struct Chuck_DL_Ctrl;
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
typedef t_CKBOOL (CK_DLL_CALL * f_ck_query)( Chuck_DL_Query * QUERY );
// object
typedef t_CKVOID (CK_DLL_CALL * f_ctor)( Chuck_Object * self, void * ARGS );
typedef t_CKVOID (CK_DLL_CALL * f_dtor)( Chuck_Object * self );
typedef t_CKVOID (CK_DLL_CALL * f_mfun)( Chuck_Object * self, void * ARGS, Chuck_DL_Return * RETURN );
typedef t_CKVOID (CK_DLL_CALL * f_sfun)( void * ARGS, Chuck_DL_Return & RETURN );
// ugen specific
typedef t_CKBOOL (CK_DLL_CALL * f_tick)( Chuck_UGen * self, SAMPLE in, SAMPLE * out );
typedef t_CKVOID (CK_DLL_CALL * f_ctrl)( Chuck_UGen * self, void * ARGS );
typedef t_CKVOID (CK_DLL_CALL * f_cget)( Chuck_UGen * self, void * OUT );
typedef t_CKBOOL (CK_DLL_CALL * f_pmsg)( Chuck_UGen * self, const char * msg, void * ARGS );
}


// default name in DLL/ckx to look for
#define CK_QUERY_FUNC        "ck_query"


//-----------------------------------------------------------------------------
// chuck DLL query functions, implemented on chuck side for portability
//-----------------------------------------------------------------------------
extern "C" {
// set name of ckx
typedef void ( CK_DLL_CALL * f_setname)( Chuck_DL_Query * query, const char * name );

// begin class/namespace, can be nested
typedef void (CK_DLL_CALL * f_begin_class)( Chuck_DL_Query * query, const char * name, const char * parent );
// add constructor, must be betwen begin/end_class : use f_add_arg to add arguments immediately after
typedef void (CK_DLL_CALL * f_add_ctor)( Chuck_DL_Query * query, f_ctor ctor );
// add destructor - cannot have args
typedef void (CK_DLL_CALL * f_add_dtor)( Chuck_DL_Query * query, f_dtor dtor );
// add member function - args to follow
typedef void (CK_DLL_CALL * f_add_mfun)( Chuck_DL_Query * query, f_mfun mfun, const char * type, const char * name );
// add static function - args to follow
typedef void (CK_DLL_CALL * f_add_sfun)( Chuck_DL_Query * query, f_sfun sfun, const char * type, const char * name );
// add member/static variable
typedef void (CK_DLL_CALL * f_add_var)( Chuck_DL_Query * query, const char * type, const char * name,
              t_CKBOOL is_const, void * static_addr ); // TODO: public/protected/private
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


//-----------------------------------------------------------------------------
// name: struct Chuck_DL_Query
// desc: passed to module
//-----------------------------------------------------------------------------
struct Chuck_DL_Query
{
    // function pointers - to be called from client module
    //   QUERY->setname( QUERY, ... );
    //
    // set the name of the module
    f_setname setname;
    // begin class/namespace, can be nexted
    f_begin_class begin_class;
    // add constructor, can be followed by add_arg
    f_add_ctor add_ctor;
    // add destructor, no args allowed
    f_add_dtor add_dtor;
    // add member function, can be followed by add_arg
    f_add_mfun add_mfun;
    // add static function, can be followed by add_arg
    f_add_sfun add_sfun;
    // add var - can be static or member
    f_add_var add_var;
    // add argument to function
    f_add_arg add_arg;
    // (ugen only) add tick and pmsg functions
    f_add_ugen_func add_ugen_func;
    // (ugen only) add ctrl parameters
    f_add_ugen_ctrl add_ugen_ctrl;
    // end class/namespace, compile it
    f_end_class end_class;

    // name
    std::string name;
    // current class
    Chuck_DL_Class * curr;
    // collection of class
    std::vector<Chuck_DL_Class *> classes;
    
    // constructor
    Chuck_DL_Query();
    // clear function
    void clear();
};


//-----------------------------------------------------------------------------
// name: struct Chuck_DL_Class
// desc: class built from module
//-----------------------------------------------------------------------------
struct Chuck_DL_Class
{
    // the name of the class
    std::string name;
    // the name of the parent
    std::string parent;
    // ctor
    std::vector<Chuck_DL_Func *> ctors;
    // dtor
    Chuck_DL_Func * dtor;
    // mfun
    std::vector<Chuck_DL_Func *> mfuns;
    // sfun
    std::vector<Chuck_DL_Func *> sfuns;
    // mdata
    std::vector<Chuck_DL_Value *> mvars;
    // sdata
    std::vector<Chuck_DL_Value *> svars;
    // ugen_tick
    f_tick ugen_tick;
    // ugen_pmsg
    f_pmsg ugen_pmsg;
    // ugen_ctrl/cget
    std::vector<Chuck_DL_Ctrl *> ugen_ctrl;
    // collection of recursive classes
    std::vector<Chuck_DL_Class *> classes;
    
    // constructor
    Chuck_DL_Class() { dtor = NULL; ugen_tick = NULL; ugen_pmsg = NULL; }
};


//-----------------------------------------------------------------------------
// name: struct Chuck_DL_Func
// desc: function from module
//-----------------------------------------------------------------------------
struct Chuck_DL_Func
{
    // the name of the function
    std::string name;
    // the return type
    std::string type;
    // arguments
    std::vector<Chuck_DL_Value *> args;
};


//-----------------------------------------------------------------------------
// name: struct Chuck_DL_Value
// desc: value from module
//-----------------------------------------------------------------------------
struct Chuck_DL_Value
{
    // the name of the value
    std::string name;
    // the type of the value
    std::string type;
    // is const
    t_CKBOOL is_const;
    // addr static
    void * static_addr;
    
    // constructor
    Chuck_DL_Value() { is_const = FALSE; static_addr = NULL; }
};


//-----------------------------------------------------------------------------
// name: struct Chuck_DL_Ctrl
// desc: ctrl for ugen
//-----------------------------------------------------------------------------
struct Chuck_DL_Ctrl
{
    // the name of the ctrl
    std::string name;
    // the first type
    std::string type;
    // the types of the value
    std::vector<std::string> types;
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

    Chuck_DLL( const char * id = NULL ) {
        m_handle = NULL; m_done_query = FALSE;
        m_id = id ? id : ""; m_query_func = NULL; }
    ~Chuck_DLL() {
        this->unload(); }

protected:
    void * m_handle;
    std::string m_last_error;
    std::string m_filename;
    std::string m_id;
    std::string m_func;
    t_CKBOOL m_done_query;
    
    f_ck_query m_query_func;
    f_ck_attach m_attach_func;
    f_ck_detach m_detach_func;
    Chuck_DL_Query m_query;
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

#elif defined(__WINDOWS_DS__)

  #ifdef __cplusplus
  extern "C" {
  #endif

  #define RTLD_LAZY         0x1
  #define RTLD_NOW          0x2
  #define RTLD_LOCAL        0x4
  #define RTLD_GLOBAL       0x8
  #define RTLD_NOLOAD       0x10
  #define RTLD_SHARED       0x20	/* not used, the default */
  #define RTLD_UNSHARED     0x40
  #define RTLD_NODELETE     0x80
  #define RTLD_LAZY_UNDEF   0x100

  void * dlopen( const char * path, int mode );
  void * dlsym( void * handle, const char * symbol );
  const char * dlerror( void );
  int dlclose( void * handle );
  static char dlerror_buffer[128];

  #ifdef __cplusplus
  }
  #endif

#else
  #include "dlfcn.h"
#endif




#endif
