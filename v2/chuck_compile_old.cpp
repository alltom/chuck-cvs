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
// file: chuck_compile.cpp
// desc: chuck compile system unifying parser, type checker, and emitter
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2005 - original
//-----------------------------------------------------------------------------
#include "chuck_compile.h"
#include "chuck_lang.h"
#include "chuck_errmsg.h"
#include "chuck_otf.h"

#include "ugen_osc.h"
#include "ugen_xxx.h"
#include "ugen_filter.h"
#include "ugen_stk.h"
#include "ulib_machine.h"
#include "ulib_math.h"
#include "ulib_std.h"
#include "ulib_opsc.h"




// function prototypes
t_CKBOOL load_internal_modules( Chuck_Compiler * compiler );
t_CKBOOL load_module( Chuck_Env * env, f_ck_query query, const char * name, const char * nspc );
t_CKBOOL parse_and_check_prog( Chuck_Env * env, const string & filename, FILE * fd );




//-----------------------------------------------------------------------------
// name: Chuck_Compiler()
// desc: constructor
//-----------------------------------------------------------------------------
Chuck_Compiler::Chuck_Compiler()
{
    env = NULL;
    emitter = NULL;
    code = NULL;
}




//-----------------------------------------------------------------------------
// name: ~Chuck_Compiler()
// desc: destructor
//-----------------------------------------------------------------------------
Chuck_Compiler::~Chuck_Compiler()
{
    // call shutdown
    this->shutdown();
}




//-----------------------------------------------------------------------------
// name: initialize()
// desc: initialize the compiler
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Compiler::initialize( Chuck_VM * vm )
{
    // allocate the type checker
    env = type_engine_init( vm );
    // allocate the emitter
    emitter = emit_engine_init( env );

    // load internal libs
    if( !load_internal_modules( this ) )
        goto error;
    
    return TRUE;

error:
    // clean up
    this->shutdown();

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: shutdown()
// desc: shutdown the compiler
//-----------------------------------------------------------------------------
void Chuck_Compiler::shutdown()
{
    // TODO: free
    // type_engine_shutdown( env );
    // emit_engine_shutdown( emitter );
    env = NULL;
    emitter = NULL;
    code = NULL;
}




//-----------------------------------------------------------------------------
// name: go()
// desc: parse, type-check, and emit a program
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Compiler::go( const string & filename, FILE * fd )
{
    t_CKBOOL ret = TRUE;

    // parse the code
    if( !chuck_parse( filename.c_str(), fd ) )
        return FALSE;

    // check the program
    if( !type_engine_check_prog( env, g_program, filename ) )
    { ret = FALSE; goto cleanup; }

    // emit
    if( !(code = emit_engine_emit_prog( emitter, g_program )) )
    { ret = FALSE; goto cleanup; }

cleanup:

    // unload the context from the type-checker
    if( !type_engine_unload_context( env ) )
    {
        EM_error2( 0, "internal error unloading context...\n" );
        return FALSE;
    }

    return ret;
}




//-----------------------------------------------------------------------------
// name: output()
// desc: get the code generated by the last do()
//-----------------------------------------------------------------------------
Chuck_VM_Code * Chuck_Compiler::output()
{
    return this->code;
}




//-----------------------------------------------------------------------------
// name: load_module()
// desc: load a dll and add it
//-----------------------------------------------------------------------------
t_CKBOOL load_module( Chuck_Env * env, f_ck_query query, 
                      const char * name, const char * nspc )
{
    Chuck_DLL * dll = NULL;
    
    // load osc
    dll = new Chuck_DLL( name );
    dll->load( query );
    if( !dll->query() || !type_engine_add_dll( env, dll, nspc ) )
    {
        fprintf( stderr, 
                 "[chuck]: internal error loading module '%s.%s'...\n", 
                 nspc, name );
        if( !dll->query() )
            fprintf( stderr, "       %s\n", dll->last_error() );

        return FALSE;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: load_internal_modules()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL load_internal_modules( Chuck_Compiler * compiler )
{
    // get env
    Chuck_Env * env = compiler->env;
    // make context
    Chuck_Context * context = new Chuck_Context;
    // add ref
    context->add_ref();
    // load it
    type_engine_load_context( env, context );

    // load
    load_module( env, osc_query, "osc", "global" );
    load_module( env, xxx_query, "xxx", "global" );
    load_module( env, filter_query, "filter", "global" );
    load_module( env, stk_query, "stk", "global" );

    // load
    if( !load_module( env, machine_query, "Machine", "global" ) ) goto error;
    machine_init( compiler, otf_process_msg );
    if( !load_module( env, libstd_query, "Std", "global" ) ) goto error;
    if( !load_module( env, libmath_query, "Math", "global" ) ) goto error;
    if( !load_module( env, opensoundcontrol_query, "opsc", "global" ) ) goto error;
    // if( !load_module( env, net_query, "net", "global" ) ) goto error;

    if( !init_class_Midi( env ) ) goto error;
    if( !init_class_MidiRW( env ) ) goto error;

    // clear context
    type_engine_unload_context( env );

    // commit what is in the type checker at this point
    env->global()->commit();
    
    return TRUE;

error:

    // clear context
    type_engine_unload_context( env );

    // probably dangerous: rollback
    env->global()->rollback();

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: parse_and_check_prog()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL parse_and_check_prog( Chuck_Env * env, const string & filename,
                               FILE * fd )
{
    // parse the code
    if( !chuck_parse( filename.c_str(), fd ) )
        return FALSE;

    // check the program
    if( !type_engine_check_prog( env, g_program, filename ) )
        return FALSE;

    return TRUE;
}