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
// file: chuck_type.cpp
// desc: chuck type-system / type-checker
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002 - original
//       Autumn 2004 - rewrite
//-----------------------------------------------------------------------------
#include "chuck_type_new.h"
#include "chuck_vm.h"
#include "chuck_dl.h"
#include "chuck_errmsg.h"
#include "chuck_instr.h"
#include "chuck_bbq.h"




//-----------------------------------------------------------------------------
// default types
//-----------------------------------------------------------------------------
struct Chuck_Type t_void = { te_void, "void", NULL, 0 };
struct Chuck_Type t_int = { te_int, "int", NULL, sizeof(t_CKINT) };
struct Chuck_Type t_float = { te_float, "float", NULL, sizeof(double) };
struct Chuck_Type t_time = { te_time, "time", NULL, sizeof(t_CKTIME) };
struct Chuck_Type t_dur = { te_dur, "dur", NULL, sizeof(t_CKTIME) };
struct Chuck_Type t_object = { te_object, "object", NULL, sizeof(void *) };
struct Chuck_Type t_null = { te_null, "@null", NULL, 0 };
struct Chuck_Type t_string = { te_string, "string", &t_object, sizeof(void *) };
struct Chuck_Type t_shred = { te_shred, "shred", &t_object, sizeof(void *) };
struct Chuck_Type t_thread = { te_thread, "thread", &t_object, sizeof(void *) };
struct Chuck_Type t_function = { te_function, "function", NULL, sizeof(void *) };
struct Chuck_Type t_class = { te_class, "class", NULL, sizeof(void *) };
struct Chuck_Type t_event = { te_event, "event", &t_object, sizeof(void *) };
struct Chuck_Type t_ugen = { te_ugen, "ugen", &t_object, sizeof(void *) };

/* exile
struct Chuck_Type t_adc = { te_adc, "adc", &t_ugen, t_ugen.size };
struct Chuck_Type t_dac = { te_dac, "dac", &t_ugen, t_ugen.size };
struct Chuck_Type t_bunghole = { te_bunghole, "bunghole", &t_ugen, t_ugen.size };
struct Chuck_Type t_midiout = { te_midiout, "midiout", &t_object, sizeof(void *) };
struct Chuck_Type t_midiin = { te_midiin, "midiin", &t_object, sizeof(void *) };
struct Chuck_Type t_stdout = { te_stdout, "@stdout", &t_object, sizeof(void *) };
struct Chuck_Type t_stderr ={ te_stdout, "@stderr", &t_object, sizeof(void *) };

struct Chuck_Type t_uint = { te_uint, "uint", NULL, sizeof(t_CKUINT) };
struct Chuck_Type t_single = { te_single, "single", NULL, sizeof(float) };
struct Chuck_Type t_double = { te_double, "double", NULL, sizeof(double) };
struct Chuck_Type t_code = { te_code, "code", NULL, sizeof(void *) };
struct Chuck_Type t_tuple = { te_tuple, "tuple", NULL, sizeof(void *) };
struct Chuck_Type t_pattern = { te_pattern, "pattern", &t_object, sizeof(void *) };
struct Chuck_Type t_transport = { te_transport, "transport", &t_object, sizeof(void *) };
struct Chuck_Type t_host = { te_host, "host", &t_object, sizeof(void *) }; */




//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_prog( Chuck_Env * env, a_Program prog );
t_CKBOOL type_engine_check_class_def( Chuck_Env * env, a_Class_Def class_def );
t_CKBOOL type_engine_check_func_def( Chuck_Env * env, a_Func_Def func_def );
t_CKBOOL type_engine_check_stmt_list( Chuck_Env * env, a_Stmt_List list );
t_CKBOOL type_engine_check_stmt( Chuck_Env * env, a_Stmt stmt );
t_CKBOOL type_engine_check_if( Chuck_Env * env, a_Stmt_If stmt );
t_CKBOOL type_engine_check_for( Chuck_Env * env, a_Stmt_For stmt );
t_CKBOOL type_engine_check_code_segment( Chuck_Env * env, a_Stmt_Code stmt );
t_CKBOOL type_engine_check_while( Chuck_Env * env, a_Stmt_While stmt );
t_CKBOOL type_engine_check_until( Chuck_Env * env, a_Stmt_Until stmt );
t_CKBOOL type_engine_check_switch( Chuck_Env * env, a_Stmt_Switch stmt );
t_CKBOOL type_engine_check_return( Chuck_Env * env, a_Stmt_Return stmt );
t_CKTYPE type_engine_check_exp( Chuck_Env * env, a_Exp exp );
t_CKTYPE type_engine_check_primary( Chuck_Env * env, a_Exp_Primary exp );
t_CKTYPE type_engine_check_op( Chuck_Env * env, ae_Operator op, 
                               t_CKTYPE left, t_CKTYPE right );
t_CKTYPE type_engine_check_exp_binary( Chuck_Env * env, a_Exp_Binary binary );
t_CKTYPE type_engine_check_exp_unary( Chuck_Env * env, a_Exp_Unary unary );
t_CKTYPE type_engine_check_exp_cast( Chuck_Env * env, a_Exp_Cast cast );
t_CKTYPE type_engine_check_exp_postfix( Chuck_Env * env, a_Exp_Postfix postfix );
t_CKTYPE type_engine_check_exp_dur( Chuck_Env * env, a_Exp_Dur dur );
t_CKTYPE type_engine_check_exp_array( Chuck_Env * env, a_Exp_Array array );
t_CKTYPE type_engine_check_exp_func_call( Chuck_Env * env, a_Exp_Func_Call func_call );
t_CKTYPE type_engine_check_exp_dot_member( Chuck_Env * env, a_Exp_Dot_Member member );
t_CKTYPE type_engine_check_exp_if( Chuck_Env * env, a_Exp_If exp_if );
t_CKTYPE type_engine_check_exp_decl( Chuck_Env * env, a_Exp_Decl decl );
t_CKTYPE type_engine_check_exp_namespace( Chuck_Env * env, a_Exp_Namespace name_space );
t_CKBOOL type_engine_check_cast_valid( Chuck_Env * env, t_CKTYPE to, t_CKTYPE from );
// import
t_CKBOOL type_engine_check_func_def_import( Chuck_Env * env, a_Func_Def func_def );
t_CKBOOL type_engine_check_ugen_def_import( Chuck_Env * env, Chuck_UGen_Info * ugen );
t_CKBOOL type_engine_check_value_import( Chuck_Env * env, const string & name, 
										 const string & type, void * addr );




//-----------------------------------------------------------------------------
// name: type_engine_init()
// desc: initialize a type engine
//-----------------------------------------------------------------------------
Chuck_Env * type_engine_init( Chuck_VM * vm )
{
    // allocate a new env
    Chuck_Env * env = new Chuck_Env;
	// set the VM reference
    env->vm = vm;
	// set the name of global namespace
	env->global.name = "global";

    // enter the default global type mapping
	env->global.type.add( t_void.name, &t_void );
	env->global.type.add( t_int.name, &t_int );
	env->global.type.add( t_float.name, &t_float );
	env->global.type.add( t_time.name, &t_time );
	env->global.type.add( t_dur.name, &t_dur );
	env->global.type.add( t_object.name, &t_object );
	env->global.type.add( t_string.name, &t_string );
	env->global.type.add( t_shred.name, &t_shred );
	env->global.type.add( t_thread.name, &t_thread );
	env->global.type.add( t_function.name, &t_function );
	env->global.type.add( t_class.name, &t_class );
	env->global.type.add( t_event.name, &t_event );
	env->global.type.add( t_ugen.name, &t_ugen );

	// dur value
    t_CKDUR samp = 1.0;
    t_CKDUR second = Digitalio::sampling_rate() * samp;
    t_CKDUR ms = second / 1000.0;
    t_CKDUR minute = second * 60.0;
    t_CKDUR hour = minute * 60.0;
    t_CKDUR day = hour * 24.0;
    t_CKDUR week = day * 7.0;
	
	// default global values
	env->global.value.add( "null", new Chuck_Value( &t_null, "null" ) );
	env->global.value.add( "now", new Chuck_Value( &t_time, "now" ) );
	env->global.value.add( "start", new Chuck_Value( &t_time, "start" ) );
	env->global.value.add( "samp", new Chuck_Value( &t_dur, "samp", new t_CKDUR(samp) ) );
	env->global.value.add( "ms", new Chuck_Value( &t_dur, "ms", new t_CKDUR(ms) ) );
	env->global.value.add( "second", new Chuck_Value( &t_dur, "second", new t_CKDUR(second) ) );
	env->global.value.add( "minute", new Chuck_Value( &t_dur, "minute", new t_CKDUR(minute) ) );
	env->global.value.add( "hour", new Chuck_Value( &t_dur, "hour", new t_CKDUR(hour) ) );
	env->global.value.add( "day", new Chuck_Value( &t_dur, "day", new t_CKDUR(day) ) );
	env->global.value.add( "week", new Chuck_Value( &t_dur, "week", new t_CKDUR(week) ) );
	env->global.value.add( "true", new Chuck_Value( &t_int, "true", new t_CKINT(1) ) );
	env->global.value.add( "false", new Chuck_Value( &t_int, "false", new t_CKINT(0) ) );
	env->global.value.add( "maybe", new Chuck_Value( &t_int, "maybe" ) );
	env->global.value.add( "pi", new Chuck_Value( &t_float, "pi", new t_CKFLOAT(3.14159265358979323846) ) );
	env->global.value.add( "global", new Chuck_Value( &t_class, "global", &(env->global) ) );

	/*
    S_enter( e->value, insert_symbol( "language" ), &t_null );
    S_enter( e->value, insert_symbol( "compiler" ), &t_null );
    S_enter( e->value, insert_symbol( "machine" ), &t_null );
    S_enter( e->value, insert_symbol( "chout" ), &t_system_out );
    S_enter( e->value, insert_symbol( "cherr" ), &t_system_err );
    S_enter( e->value, insert_symbol( "stdout" ), &t_system_out );
    S_enter( e->value, insert_symbol( "stderr" ), &t_system_err );
    S_enter( e->value, insert_symbol( "midiout" ), &t_midiout );
    S_enter( e->value, insert_symbol( "midiin" ), &t_midiin );
    S_enter( e->value, insert_symbol( "adc" ), &t_adc );
    S_enter( e->value, insert_symbol( "dac" ), &t_dac );
    S_enter( e->value, insert_symbol( "bunghole" ), &t_bunghole );
    S_enter( e->value, insert_symbol( "blackhole" ), &t_bunghole );
    S_enter( e->value, insert_symbol( "endl" ), &t_string );
    */

    return env;
}




//-----------------------------------------------------------------------------
// name: lookup_type()
// desc: lookup type in the env
//-----------------------------------------------------------------------------
Chuck_Type * Chuck_Namespace::lookup_type( const string & name, t_CKBOOL climb )
{
    Chuck_Type * t = type.lookup( name );
    if( climb && !t && parent )
        return parent->lookup_type( name, climb );
    return t;
}




//-----------------------------------------------------------------------------
// name: lookup_value()
// desc: lookup value in the env
//-----------------------------------------------------------------------------
Chuck_Value * Chuck_Namespace::lookup_value( const string & name, t_CKBOOL climb )
{
    Chuck_Value * v = value.lookup( name );
    if( climb && !v && parent )
        return parent->lookup_value( name, climb );
    return v;
}




//-----------------------------------------------------------------------------
// name: lookup_func()
// desc: lookup func in the env
//-----------------------------------------------------------------------------
Chuck_Func * Chuck_Namespace::lookup_func( const string & name, t_CKBOOL climb )
{
    Chuck_Func * f = func.lookup( name );
    if( climb && !f && parent )
        return parent->lookup_func( name, climb );
    return f;
}




//-----------------------------------------------------------------------------
// name: lookup_class()
// desc: lookup class in the namespace
//-----------------------------------------------------------------------------
Chuck_Namespace * Chuck_Namespace::lookup_class( const string & name, t_CKBOOL climb )
{
    Chuck_Namespace * e = class_defs.lookup( name );
    if( climb && !e && parent )
        return parent->lookup_class( name, climb );
    return e;
}




//-----------------------------------------------------------------------------
// name: lookup_addr()
// desc: lookup addr in the env
//-----------------------------------------------------------------------------
void * Chuck_Namespace::lookup_addr( const string & name, t_CKBOOL climb )
{
    void * a = addr.lookup( name );
    if( climb && !a && parent )
        return parent->lookup_addr( name, climb );
    return a;
}
