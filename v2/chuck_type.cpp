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
#include "chuck_type.h"
#include "chuck_vm.h"
#include "chuck_dl.h"
#include "chuck_errmsg.h"
#include "chuck_instr.h"
#include "chuck_ugen.h"
#include <assert.h>




//-----------------------------------------------------------------------------
// default types
//-----------------------------------------------------------------------------
Chuck_Type t_void( te_void, "void", NULL, 0 );
Chuck_Type t_int( te_int, "int", NULL, sizeof(t_CKINT) );
Chuck_Type t_float( te_float, "float", NULL, sizeof(t_CKFLOAT) );
Chuck_Type t_time( te_time, "time", NULL, sizeof(t_CKTIME) );
Chuck_Type t_dur( te_dur, "dur", NULL, sizeof(t_CKTIME) );
Chuck_Type t_object( te_object, "object", NULL, sizeof(void *) );
Chuck_Type t_null( te_null, "@null", NULL, 0 );
Chuck_Type t_string( te_string, "string", &t_object, sizeof(void *) );
Chuck_Type t_shred( te_shred, "shred", &t_object, sizeof(void *) );
Chuck_Type t_thread( te_thread, "thread", &t_object, sizeof(void *) );
Chuck_Type t_function( te_function, "function", &t_object, sizeof(void *) );
Chuck_Type t_class( te_class, "class", &t_object, sizeof(void *) );
Chuck_Type t_event( te_event, "event", &t_object, sizeof(void *) );
Chuck_Type t_ugen( te_ugen, "ugen", &t_object, sizeof(void *) );

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
t_CKBOOL type_engine_check_stmt_list( Chuck_Env * env, a_Stmt_List list );
t_CKBOOL type_engine_check_stmt( Chuck_Env * env, a_Stmt stmt );
t_CKBOOL type_engine_check_if( Chuck_Env * env, a_Stmt_If stmt );
t_CKBOOL type_engine_check_for( Chuck_Env * env, a_Stmt_For stmt );
t_CKBOOL type_engine_check_while( Chuck_Env * env, a_Stmt_While stmt );
t_CKBOOL type_engine_check_until( Chuck_Env * env, a_Stmt_Until stmt );
t_CKBOOL type_engine_check_break( Chuck_Env * env, a_Stmt_Break br );
t_CKBOOL type_engine_check_continue( Chuck_Env * env, a_Stmt_Continue cont );
t_CKBOOL type_engine_check_return( Chuck_Env * env, a_Stmt_Return stmt );
t_CKBOOL type_engine_check_switch( Chuck_Env * env, a_Stmt_Switch stmt );
t_CKTYPE type_engine_check_exp( Chuck_Env * env, a_Exp exp );
t_CKTYPE type_engine_check_exp_binary( Chuck_Env * env, a_Exp_Binary binary );
t_CKTYPE type_engine_check_op( Chuck_Env * env, ae_Operator op, a_Exp lhs, a_Exp rhs );
t_CKTYPE type_engine_check_op_chuck( Chuck_Env * env, a_Exp lhs, a_Exp rhs );
t_CKTYPE type_engine_check_op_unchuck( Chuck_Env * env, a_Exp lhs, a_Exp rhs );
t_CKTYPE type_engine_check_exp_unary( Chuck_Env * env, a_Exp_Unary unary );
t_CKTYPE type_engine_check_primary( Chuck_Env * env, a_Exp_Primary exp );
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
t_CKBOOL type_engine_check_code_segment( Chuck_Env * env, a_Stmt_Code stmt );
t_CKBOOL type_engine_check_func_def( Chuck_Env * env, a_Func_Def func_def );
t_CKBOOL type_engine_check_class_def( Chuck_Env * env, a_Class_Def class_def );
t_CKBOOL type_engine_check_func_def_import( Chuck_Env * env, a_Func_Def func_def );
t_CKBOOL type_engine_check_ugen_def_import( Chuck_Env * env, Chuck_UGen_Info * ugen );
t_CKBOOL type_engine_add_dll( Chuck_Env * env, Chuck_DLL * dll, const char * name );
t_CKBOOL type_engine_check_reserved( Chuck_Env * env, const string & id, int pos );
t_CKBOOL type_engine_check_reserved( Chuck_Env * env, S_Symbol id, int pos );
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
    // TODO:
    // t_CKDUR second = vm->srate() * samp;
    t_CKDUR second = 44100 * samp;
    t_CKDUR ms = second / 1000.0;
    t_CKDUR minute = second * 60.0;
    t_CKDUR hour = minute * 60.0;
    t_CKDUR day = hour * 24.0;
    t_CKDUR week = day * 7.0;

	// default global values
	env->global.value.add( "null", new Chuck_Value( &t_null, "null", new void *(NULL), TRUE ) );
    // TODO:
	// env->global.value.add( "now", new Chuck_Value( &t_time, "now", &(vm->shreduler()->now_system), TRUE ) );
	env->global.value.add( "t_zero", new Chuck_Value( &t_time, "time_zero", new t_CKDUR(0.0), TRUE ) );
    env->global.value.add( "d_zero", new Chuck_Value( &t_dur, "dur_zero", new t_CKDUR(0.0), TRUE ) );
	env->global.value.add( "samp", new Chuck_Value( &t_dur, "samp", new t_CKDUR(samp), TRUE ) );
	env->global.value.add( "ms", new Chuck_Value( &t_dur, "ms", new t_CKDUR(ms), TRUE ) );
	env->global.value.add( "second", new Chuck_Value( &t_dur, "second", new t_CKDUR(second), TRUE ) );
	env->global.value.add( "minute", new Chuck_Value( &t_dur, "minute", new t_CKDUR(minute), TRUE ) );
	env->global.value.add( "hour", new Chuck_Value( &t_dur, "hour", new t_CKDUR(hour), TRUE ) );
	env->global.value.add( "day", new Chuck_Value( &t_dur, "day", new t_CKDUR(day), TRUE ) );
	env->global.value.add( "week", new Chuck_Value( &t_dur, "week", new t_CKDUR(week), TRUE ) );
	env->global.value.add( "true", new Chuck_Value( &t_int, "true", new t_CKINT(1), TRUE ) );
	env->global.value.add( "false", new Chuck_Value( &t_int, "false", new t_CKINT(0), TRUE ) );
	env->global.value.add( "maybe", new Chuck_Value( &t_int, "maybe", new t_CKFLOAT(.5), FALSE ) );
	env->global.value.add( "pi", new Chuck_Value( &t_float, "pi", new t_CKFLOAT(3.14159265358979323846), TRUE ) );
	env->global.value.add( "global", new Chuck_Value( &t_class, "global", &(env->global), TRUE ) );

	/*
    S_enter( e->value, insert_symbol( "machine" ), &t_null );
    S_enter( e->value, insert_symbol( "language" ), &t_null );
    S_enter( e->value, insert_symbol( "compiler" ), &t_null );
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

    // add reserve words
    env->key_words["for"] = TRUE;
    env->key_words["while"] = TRUE;
    env->key_words["until"] = TRUE;
    env->key_words["if"] = TRUE;
    env->key_words["else"] = TRUE;
    env->key_words["spork"] = TRUE;
    env->key_words["return"] = TRUE;
    env->key_words["class"] = TRUE;
    env->key_words["extends"] = TRUE;
    env->key_words["implements"] = TRUE;
    env->key_words["function"] = TRUE;
    env->key_words["fun"] = TRUE;
    env->key_words["public"] = TRUE;
    env->key_words["protected"] = TRUE;
    env->key_words["private"] = TRUE;
    env->key_words["static"] = TRUE;

    env->key_values["now"] = TRUE;
    env->key_values["beginning"] = TRUE;
    env->key_values["true"] = TRUE;
    env->key_values["false"] = TRUE;
    env->key_values["maybe"] = TRUE;
    env->key_values["pi"] = TRUE;
    env->key_values["this"] = TRUE;
    env->key_values["super"] = TRUE;
    env->key_values["samp"] = TRUE;
    env->key_values["ms"] = TRUE;
    env->key_values["second"] = TRUE;
    env->key_values["minute"] = TRUE;
    env->key_values["hour"] = TRUE;
    env->key_values["day"] = TRUE;
    env->key_values["week"] = TRUE;
    env->key_values["adc"] = TRUE;
    env->key_values["dac"] = TRUE;
    env->key_values["global"] = TRUE;

    env->key_types["void"] = TRUE;
    env->key_types["int"] = TRUE;
    env->key_types["float"] = TRUE;
    env->key_types["dur"] = TRUE;
    env->key_types["time"] = TRUE;
    env->key_types["object"] = TRUE;
    env->key_types["string"] = TRUE;
    env->key_types["shred"] = TRUE;
    env->key_types["event"] = TRUE;
    env->key_types["ugen"] = TRUE;
    env->key_types["machine"] = TRUE;
    env->key_types["language"] = TRUE;
    env->key_types["compiler"] = TRUE;

    return env;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_prog()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_prog( Chuck_Env * env, a_Program prog )
{
    t_CKBOOL ret = TRUE;

    if( !prog )
        return FALSE;

    // reset the env
    env->reset();
    // each parse tree corresponds to a chuck context
    Chuck_Context * context = new Chuck_Context;
    // save a reference to the parse tree
    context->parse_tree = prog;
    // append the context to the env
    env->contexts.push_back( context );
    // make the context current
    env->context = context;
    // push the context scope
    env->context->nspc.value.push();
    // push the current namespaces
    env->stack.push_back( env->curr );
    // set the parent
    context->nspc.parent = env->curr;
    // set the context's namespace as current
    env->curr = &(context->nspc);

    // go through each of the program sections
    while( prog && ret )
    {
        switch( prog->section->s_type )
        {
        case ae_section_stmt:
            ret = type_engine_check_stmt_list( env, prog->section->stmt_list );
            break;
        
        case ae_section_func:
            ret = type_engine_check_func_def( env, prog->section->func_def );
            break;

        case ae_section_class:
            ret = type_engine_check_class_def( env, prog->section->class_def );
            break;
        
        default:
            EM_error2( prog->linepos,
                "internal error: unrecognized program section in type checker..." );
            ret = FALSE;
            break;
        }

        prog = prog->next;
    }
    
    // make sure we still have the same context
    assert( env->contexts.size() != 0 );
    assert( env->contexts.back() == context );

    // pop the context scope
    env->context->nspc.value.pop();
    // restore the current namespace
    env->curr = env->stack.back();
    // pop the namespace stack
    env->stack.pop_back();

    // make sure the nspc is ok
    assert( env->stack.size() != 0 );
    // assert( env->stack.back() == &(context->nspc) );

    // check to see if everything passed
    if( !ret )
    {
        // TODO: remove the effects of the context in the env
        // ---> insert code here <----
        // remove the context
        env->contexts.pop_back();
        // release it
        context->release();
    }
    
    // reset the env
    env->reset();

    return ret;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_stmt_list()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_stmt_list( Chuck_Env * env, a_Stmt_List list )
{
    // type check the stmt_list
    while( list )
    {
        // the current statement
        if( !type_engine_check_stmt( env, list->stmt ) )
            return FALSE;
        
        // advance to the next statement
        list = list->next;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_stmt(()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_stmt( Chuck_Env * env, a_Stmt stmt )
{
    t_CKBOOL ret = TRUE;

    if( !stmt )
        return TRUE;

    // the type of stmt
    switch( stmt->s_type )
    {
        case ae_stmt_if:
            ret = type_engine_check_if( env, &stmt->stmt_if );
            break;

        case ae_stmt_for:
            ret = type_engine_check_for( env, &stmt->stmt_for );
            break;

        case ae_stmt_while:
            ret = type_engine_check_while( env, &stmt->stmt_while );
            break;
            
        case ae_stmt_until:
            ret = type_engine_check_until( env, &stmt->stmt_until );
            break;
            
        case ae_stmt_exp:
            ret = ( type_engine_check_exp( env, stmt->stmt_exp ) != NULL );
            break;

        case ae_stmt_return:
            ret = type_engine_check_return( env, &stmt->stmt_return );
            break;

        case ae_stmt_code:
            ret = type_engine_check_code_segment( env, &stmt->stmt_code );
            break;

        case ae_stmt_break:
            ret = type_engine_check_break( env, &stmt->stmt_break );
            break;

        case ae_stmt_continue:
            ret = type_engine_check_continue( env, &stmt->stmt_continue );
            break;

        case ae_stmt_switch:
            ret = type_engine_check_switch( env, &stmt->stmt_switch );
            break;

        case ae_stmt_case:
            // ret = type_engine_check_case( env, &stmt->stmt_case );
            break;

        case ae_stmt_gotolabel:
            // ret = type_engine_check_gotolabel( env, &stmt->goto_label );
            break;

        //case ae_stmt_func:
        //    ret = type_engine_check_func_def( env, stmt->stmt_func );
        //    break;
        
        default:
            EM_error2( stmt->linepos, 
                "internal compiler error - no stmt type '%i'!", stmt->s_type );
            ret = FALSE;
            break;
    }

    return ret;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_if()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_if( Chuck_Env * env, a_Stmt_If stmt )
{
    // check the conditional
    if( !type_engine_check_exp( env, stmt->cond ) )
        return FALSE;
        
    // TODO: ensure that conditional has valid type

    // check if
    if( !type_engine_check_stmt( env, stmt->if_body ) )
        return FALSE;

    // check else, if there is one
    if( stmt->else_body )
        if( !type_engine_check_stmt( env, stmt->else_body ) )
            return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_for()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_for( Chuck_Env * env, a_Stmt_For stmt )
{
    // check the initial
    if( !type_engine_check_stmt( env, stmt->c1 ) )
        return FALSE;
    
    // check the conditional
    if( !type_engine_check_stmt( env, stmt->c2 ) )
        return FALSE;

    // TODO: same as if - check conditional type valid

    // check the post
    if( stmt->c3 && !type_engine_check_exp( env, stmt->c3 ) )
        return FALSE;

    // for break and continue statement
    env->breaks.push_back( stmt->self );

    // check body
    if( !type_engine_check_stmt( env, stmt->body ) )
        return FALSE;
        
    // remove the loop from the stack
    assert( env->breaks.size() && env->breaks.back() == stmt->self );
    env->breaks.pop_back();

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_while()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_while( Chuck_Env * env, a_Stmt_While stmt )
{
    // check the conditional
    if( !type_engine_check_exp( env, stmt->cond ) )
        return FALSE;
        
    // TODO: same as if - ensure the type in conditional is valid

    // for break and continue statement
    env->breaks.push_back( stmt->self );

    // check the body
    if( !type_engine_check_stmt( env, stmt->body ) )
        return FALSE;

    // remove the loop from the stack
    assert( env->breaks.size() && env->breaks.back() == stmt->self );
    env->breaks.pop_back();

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_until()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_until( Chuck_Env * env, a_Stmt_Until stmt )
{
    // check the conditional
    if( !type_engine_check_exp( env, stmt->cond ) )
        return FALSE;
        
    // TODO: same as if - ensure the type in conditional is valid

    // for break and continue statement
    env->breaks.push_back( stmt->self );

    // check the body
    if( !type_engine_check_stmt( env, stmt->body ) )
        return FALSE;

    // remove the loop from the stack
    assert( env->breaks.size() && env->breaks.back() == stmt->self );
    env->breaks.pop_back();

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_switch()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_switch( Chuck_Env * env, a_Stmt_Switch stmt )
{
    // TODO: implement this
    EM_error2( stmt->linepos, "switch not implemented..." );

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_break()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_break( Chuck_Env * env, a_Stmt_Break br )
{
    // check to see if inside valid stmt
    if( env->breaks.size() <= 0 )
    {
        EM_error2( br->linepos, "'break' found outside of for/while/until/switch..." );
        return FALSE;
    }
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_continue()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_continue( Chuck_Env * env, a_Stmt_Continue cont )
{
    // check to see if inside valid loop
    if( env->breaks.size() <= 0 )
    {
        EM_error2( cont->linepos, "'continue' found outside of for/while/until..." );
        return FALSE;
    }
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_return()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_return( Chuck_Env * env, a_Stmt_Return stmt )
{
    Chuck_Type * ret_type = NULL;
    
    // check to see if within function definition
    if( !env->func )
    {
        EM_error2( stmt->linepos, "'return' statement found outside function definition" );
        return FALSE;
    }
    
    // check the type of the return
    if( stmt->val )
        ret_type = type_engine_check_exp( env, stmt->val );
    else
        ret_type = &t_void;

    // check to see that return type matches the prototype
    if( ret_type && !isa( ret_type, env->func->def->ret_type ) )
    {
        EM_error2( stmt->linepos,
            "function '%s' was defined with return type '%s' -- but returning type '%s'",
            env->func->name.c_str(), env->func->def->ret_type->c_name(),
            ret_type->c_name() );
        return FALSE;
    }

    return ret_type != NULL;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_code_segment()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_code_segment( Chuck_Env * env, a_Stmt_Code stmt )
{
    // TODO: make sure this is in a function or is outside class

    // push
    env->curr->value.push(); // env->context->nspc.value.push();
    // do it
    t_CKBOOL t = type_engine_check_stmt_list( env, stmt->stmt_list );
    // pop
    env->curr->value.pop();  // env->context->nspc.value.pop();
    
    return t;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp( Chuck_Env * env, a_Exp exp )
{
    a_Exp curr = exp;
    
    // reset the group size
    exp->group_size = 0;

    // loop through parallel expressions
    while( curr )
    {
        // reset the type
        curr->type = NULL;
        // increment first exp's group size
        exp->group_size++;

        // examine the syntax
        switch( curr->s_type )
        {
        case ae_exp_binary:
            curr->type = type_engine_check_exp_binary( env, &curr->binary );
        break;
    
        case ae_exp_unary:
            curr->type = type_engine_check_exp_unary( env, &curr->unary );
        break;
    
        case ae_exp_cast:
            curr->type = type_engine_check_exp_cast( env, &curr->cast );
        break;
    
        case ae_exp_postfix:
            curr->type = type_engine_check_exp_postfix( env, &curr->postfix );
        break;
    
        case ae_exp_dur:
            curr->type = type_engine_check_exp_dur( env, &curr->dur );
        break;
    
        case ae_exp_primary:
            curr->type = type_engine_check_primary( env, &curr->primary );
        break;
    
        case ae_exp_array:
            curr->type = type_engine_check_exp_array( env, &curr->array );
        break;
    
        case ae_exp_func_call:
            curr->type = type_engine_check_exp_func_call( env, &curr->func_call );
            // set the return type
            curr->func_call.ret_type = curr->type;
        break;
    
        case ae_exp_dot_member:
            curr->type = type_engine_check_exp_dot_member( env, &curr->dot_member );
        break;
    
        case ae_exp_if:
            curr->type = type_engine_check_exp_if( env, &curr->exp_if );
        break;
    
        case ae_exp_decl:
            curr->type = type_engine_check_exp_decl( env, &curr->decl );
        break;

        case ae_exp_namespace:
            //curr->type = type_engine_check_exp_namespace( env, &curr->name_space );
            EM_error2( curr->linepos, "namespace expression not implemented!" );
            return FALSE;
        break;
    
        /* case ae_exp_new:
            curr->exp_type = (t_Type)S_look( env->type, curr->a_new->type );
        break;
        */
        
        default:
            EM_error2( curr->linepos, "internal compiler error - no expression '%i'",
                curr->s_type );
            return NULL;
        }

        // error
        if( !curr->type )
            return FALSE;

        // advance to next expression
        curr = curr->next;
    }

    // return type
    return exp->type;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp_binary()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp_binary( Chuck_Env * env, a_Exp_Binary binary )
{
    a_Exp cl = binary->lhs, cr = binary->rhs;
    t_CKTYPE ret = NULL;

    // type check the lhs and rhs
    t_CKTYPE left = type_engine_check_exp( env, cl );
    t_CKTYPE right = type_engine_check_exp( env, cr);
    
    // if either fails, then return NULL
    if( !left || !right )
        return NULL;

    // cross chuck
    while( cr )
    {
        // type check the pair
        if( !(ret = type_engine_check_op( env, binary->op, cl, cr )) )
            return NULL;

        cr = cr->next;
    }
        
    return ret;
}


// helper macros
#define LR( L, R )      if( (left->id == L) && (right->id == R) )
#define COMMUTE( L, R ) if( ( (left->id == L) && (right->id == R) ) || \
                            ( (left->id == R) && (right->id == L) ) )

//-----------------------------------------------------------------------------
// name: type_engine_check_op()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_op( Chuck_Env * env, ae_Operator op, a_Exp lhs, a_Exp rhs )
{
    t_CKTYPE left = lhs->type, right = rhs->type;
    assert( left && right );
    
    // if lhs is multi-value, then check separately
    if( lhs->next )
    {
        // TODO: implement this
        EM_error2( lhs->linepos,
            "multi-value binary operations not implemented..." );
        return NULL;
    }

    // based on the op
    switch( op )
    {
    case ae_op_chuck:
        return type_engine_check_op_chuck( env, lhs, rhs );
    
    case ae_op_unchuck:
        return type_engine_check_op_unchuck( env, lhs, rhs );
    
    case ae_op_plus_chuck:
    case ae_op_plus:
        LR( te_int, te_int ) return &t_int;
        LR( te_float, te_float ) return &t_float;
        LR( te_dur, te_dur ) return &t_dur;
        COMMUTE( te_dur, te_time ) return &t_time;
    break;
    
    case ae_op_minus_chuck:
    case ae_op_minus:
        LR( te_int, te_int ) return &t_int;
        LR( te_float, te_float ) return &t_float;
        LR( te_dur, te_dur ) return &t_dur;
        LR( te_time, te_dur ) return &t_time;
        LR( te_time, te_time ) return &t_dur;
    break;
    
    case ae_op_times_chuck:
    case ae_op_times:
        LR( te_int, te_int ) return &t_int;
        LR( te_float, te_float ) return &t_float;
        COMMUTE( te_float, te_dur ) return &t_dur;
    break;
    
    case ae_op_divide_chuck:
    case ae_op_divide:
        LR( te_int, te_int ) return &t_int;
        LR( te_float, te_float ) return &t_float;
        LR( te_dur, te_dur ) return &t_float;
        LR( te_dur, te_float ) return &t_dur;
    break;
    
    case ae_op_lt:
    case ae_op_gt:
    case ae_op_le:
    case ae_op_ge:
    case ae_op_eq:
    case ae_op_neq:
        LR( te_int, te_int ) return &t_int;
        LR( te_float, te_float ) return &t_int;
        LR( te_dur, te_dur ) return &t_int;
        LR( te_time, te_time ) return &t_int;
    break;
    
    case ae_op_s_and_chuck:
    case ae_op_s_or_chuck:
    case ae_op_s_xor_chuck:
    case ae_op_shift_right_chuck:
    case ae_op_shift_left_chuck:
    case ae_op_percent_chuck:
    case ae_op_and:
    case ae_op_or:
    case ae_op_s_xor:
    case ae_op_s_and:
    case ae_op_s_or:
    case ae_op_shift_left:
    case ae_op_shift_right:
    case ae_op_percent:
        LR( te_int, te_int ) return &t_int;
    break;

    // TODO: implement this
    case ae_op_at_chuck:
        assert( FALSE );
    break;
    }

    // no match
    EM_error2( lhs->linepos,
        "no suitable resolution for binary operator '%s' on types '%s' and '%s'...",
        op2str( op ), left->c_name(), right->c_name() );
    return NULL;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_op_chuck()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_op_chuck( Chuck_Env * env, a_Exp lhs, a_Exp rhs )
{
    t_CKTYPE left = lhs->type, right = rhs->type;
    
    // ugen => ugen
    if( isa( left, &t_ugen ) && isa( right, &t_ugen ) ) return right;

    // time advance
    if( isa( left, &t_dur ) && isa( right, &t_time ) && rhs->s_meta == ae_meta_var )
        return right;

    // assignment or something else
    if( isa( left, right ) )
    {
        // basic types?
        if( isa( left, &t_int ) || isa( left, &t_float ) || isa( left, &t_dur ) 
            || isa( left, &t_time ) || isa( left, &t_string ) )
        {
            // assigment?
            if( rhs->s_meta == ae_meta_var )
                return right;

            // error
            EM_error2( lhs->linepos,
                "no suitable resolution for binary operator '=>' on types '%s' => '%s'...\n"
                "    (right-side operand is not mutable)",
                left->c_name(), right->c_name() );
            return NULL;
        }
        // aggregate types
        else
        {
            // TODO: check overloading of =>

            // no match
            EM_error2( lhs->linepos,
                "no suitable resolution for binary operator '=>' on types '%s' => '%s...'\n"
                "    (note: use @=> for assignment of objects)",
                left->c_name(), right->c_name() );
            return NULL;
        }
    }

    // TODO: check overloading of =>

    // no match
    EM_error2( lhs->linepos,
        "no suitable resolution for binary operator '=>' on types '%s' and '%s'...",
        left->c_name(), right->c_name() );
    return NULL;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_op_unchuck()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_op_unchuck( Chuck_Env * evn, a_Exp lhs, a_Exp rhs )
{
    t_CKTYPE left = lhs->type, right = rhs->type;
    
    // ugen =< ugen
    if( isa( left, &t_ugen ) && isa( right, &t_ugen ) ) return right;
    
    // TODO: check overloading of =<
    
    // no match
    EM_error2( lhs->linepos,
        "no suitable resolution for binary operator '=<' on types '%s' and '%s'...",
        left->c_name(), right->c_name() );
    return NULL;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp_unary()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp_unary( Chuck_Env * env, a_Exp_Unary unary )
{
    t_CKTYPE t = type_engine_check_exp( env, unary->exp );
    if( !t ) return NULL;
    
    // check the op
    switch( unary->op )
    {
        case ae_op_plusplus:
        case ae_op_minusminus:
            // assignable?
            if( unary->exp->s_meta != ae_meta_var )
            {
                EM_error2( unary->linepos,
                    "prefix unary operator '%s' cannot "
                    "be used on non-mutable data-types...", op2str( unary->op ) );
                return NULL;
            }
            
            // check type
            if( isa( t, &t_int ) || isa( t, &t_float ) )
                return t;
                
            // TODO: check overloading
        break;
        
        case ae_op_minus:
            // float
            if( isa( t, &t_float ) ) return t;
        case ae_op_tilda:
        case ae_op_exclamation:
            // int
            if( isa( t, &t_int ) ) return t;
        break;
        
        case ae_op_spork:
            // spork shred
            if( unary->exp->s_type == ae_exp_func_call ) return &t_shred;
            else
            {
                 EM_error2( unary->linepos,
                     "only function calls can be sporked..." );
                 return NULL;
            }
        break;
    }
    
    // no match
    EM_error2( unary->linepos,
        "no suitable resolution for prefix unary operator '%s' on type '%s...",
        op2str( unary->op ), t->c_name() );
    return NULL;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_primary()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_primary( Chuck_Env * env, a_Exp_Primary exp )
{
    t_CKTYPE t = NULL;
    Chuck_Value * v = NULL;

    // check syntax
    switch( exp->s_type )
    {
        // variable
        case ae_primary_var:
            v = env->curr->lookup_value( exp->var, TRUE );
            if( !v )
            {
                // error
                if( !env->class_def )
                {
                    EM_error2( exp->linepos,
                        "undefined variable '%s'...",
                        S_name(exp->var) );
                }
                else
                {
                    EM_error2( exp->linepos,
                        "undefined member '%s' in class/namespace '%s'...",
                        S_name(exp->var), env->class_def->name.c_str() );
                }
                return NULL;
            }
            t = v->type;
        break;
        
        // int
        case ae_primary_num:
            t = &t_int;
        break;
        
        // float
        case ae_primary_float:
            t = &t_float;
        break;
        
        // string
        case ae_primary_str:
            t = &t_string;
        break;
        
        // expression
        case ae_primary_exp:
            t = type_engine_check_exp( env, exp->exp );
        break;

        // no match
        default:
            EM_error2( exp->linepos,
                "internal error - unrecognized primary type '%i'...", exp->s_type );
        return NULL;
    }
    
    return t;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp_cast()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp_cast( Chuck_Env * env, a_Exp_Cast cast )
{
    // check the exp
    t_CKTYPE t = type_engine_check_exp( env, cast->exp );
    if( !t ) return NULL;

    // the type to cast to
    t_CKTYPE t2 = env->curr->lookup_type( cast->type->id->id, TRUE );
    if( !t2 )
    {
        EM_error2( cast->linepos,
            "undefined type '%s' in cast...",
            S_name( cast->type->id->id ) );
        return NULL;
    }
    
    // check if cast valid
    if( !type_engine_check_cast_valid( env, t2, t ) )
    {
        EM_error2( cast->linepos,
            "invalid cast to '%s' from '%s'...",
            S_name( cast->type->id->id ), t->c_name() );
        return NULL;
    }
    
    return t2;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_cast_valid()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_cast_valid( Chuck_Env * env, t_CKTYPE to, t_CKTYPE from )
{
    // down cast
    if( isa( from, to ) ) return TRUE;

    // up cast
    if( isa( to, from ) ) return TRUE;

    // TODO: dynamic type checking
    
    // int to float, float to int
    if( isa( to, &t_float ) && isa( from, &t_int ) ) return TRUE;
    if( isa( to, &t_int ) && isa( from, &t_float ) ) return TRUE;

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp_dur()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp_dur( Chuck_Env * env, a_Exp_Dur dur )
{
    // type check the two components
    t_CKTYPE base = type_engine_check_exp( env, dur->base );
    t_CKTYPE unit = type_engine_check_exp( env, dur->unit );
    
    // make sure both type check
    if( !base || !unit ) return NULL;
    
    // check base type
    if( !isa( base, &t_int ) && !isa( base, &t_float ) )
    {
        EM_error2( dur->base->linepos,
            "invalid type '%s' in prefix of dur expression...\n"
            "    (must be of type 'int' or 'float')", base->c_name() );
        return NULL;
    }
    
    // check unit type
    if( !isa( unit, &t_dur ) )
    {
        EM_error2( dur->unit->linepos,
            "invalid type '%s' in postfix of dur expression...\n"
            "    (must be of type 'dur')", unit->c_name() );
        return NULL;
    }
    
    return unit;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp_postfix()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp_postfix( Chuck_Env * env, a_Exp_Postfix postfix )
{
    // check the exp
    t_CKTYPE t = type_engine_check_exp( env, postfix->exp );
    if( !t ) return NULL;
    
    // syntax
    switch( postfix->op )
    {
        case ae_op_plusplus:
        case ae_op_minusminus:
            // assignable?
            if( postfix->exp->s_meta != ae_meta_var )
            {
                EM_error2( postfix->exp->linepos,
                    "postfix operator '%s' cannot be used on non-mutable data-type...",
                    op2str( postfix->op ) );
                return NULL;
            }
            
            // TODO: mark somewhere we need to post increment
            
            // check type
            if( isa( t, &t_int ) || isa( t, &t_float ) )
                return t;
        break;
        
        default:
            // no match
            EM_error2( postfix->linepos,
                "internal compiler error: unrecognized postfix '%i'", postfix->op );
        return NULL;
    }
    
    // no match
    EM_error2( postfix->linepos,
        "no suitable resolutation for postfix operator '%s' on type '%s'...",
        op2str( postfix->op ), t->c_name() );
    return NULL;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp_if()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp_if( Chuck_Env * env, a_Exp_If exp_if )
{
    // check the components
    t_CKTYPE cond = type_engine_check_exp( env, exp_if->cond );
    t_CKTYPE if_exp = type_engine_check_exp( env, exp_if->if_exp );
    t_CKTYPE else_exp = type_engine_check_exp( env, exp_if->else_exp );
    
    // make sure everything good
    if( !cond || !if_exp || !else_exp ) return NULL;
    
    // check the type
    if( !isa( cond, &t_int ) ) return NULL;
    
    // make sure the if and else have compatible types
    // TODO: the lesser of two types
    if( !( *if_exp == *else_exp ) )
    {
        EM_error2( exp_if->linepos,
            "incompatible types '%s' and '%s' in if expression...",
            if_exp->c_name(), else_exp->c_name() );
        return NULL;
    }
    
    return if_exp;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp_decl( )
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp_decl( Chuck_Env * env, a_Exp_Decl decl )
{
    a_Var_Decl_List list = decl->var_decl_list;
    a_Var_Decl var_decl = NULL;
    Chuck_Value * value = NULL;

    // TODO: handle T a, b, c ...
    // look up the type
    t_CKTYPE t = env->curr->lookup_type( decl->type->id->id, TRUE );
    if( !t )
    {
        EM_error2( decl->linepos,
            "undefined type '%s'...", S_name(decl->type->id->id) );
        return NULL;
    }

    while( list != NULL )
    {
        // get the decl
        var_decl = list->var_decl;

        // check if reserved
        if( type_engine_check_reserved( env, var_decl->id, var_decl->linepos ) )
        {
            EM_error2( var_decl->linepos,
                "...in variable declaration",
                S_name(var_decl->id) );
            return FALSE;
        }

        // check if locally defined
        // if( env->context->nspc.value.lookup( var_decl->id, TRUE ) )
        if( env->curr->lookup_value( var_decl->id, FALSE ) )
        {
            EM_error2( var_decl->linepos,
                "'%s' has already been defined in the same scope...",
                S_name(var_decl->id) );
            return NULL;
        }

        // TODO: this needs to be redone
        // check if array
        if( var_decl->array != NULL )
        {
            // type check the exp
            if( !type_engine_check_exp( env, var_decl->array->exp_list ) )
                return NULL;
            // make a copy of the type
            t = t->copy( env );
            // set the array depth
            t->array_depth = var_decl->array->depth;
        }

        // make sure
        // if( var_decl->isarray )
        // {
        //     EM_error2( decl->linepos,
        //         "for declaration, array subscripts must be placed after type" );
        //     return NULL;
        // }

        // enter into value binding
        // env->context->nspc.value.add( var_decl->id, 
        //    new Chuck_Value( t, S_name(var_decl->id), NULL ) );
        env->curr->value.add( var_decl->id,
            value = new Chuck_Value( t, S_name(var_decl->id), NULL ) );
        // assign the offset
        value->offset = env->curr->offset;
        // move the offset (TODO: check the size)
        env->curr->offset += t->size;

        // the next var decl
        list = list->next;
    }

    return t;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp_func_call()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp_func_call( Chuck_Env * env, a_Exp_Func_Call func_call )
{
    Chuck_Func * func = NULL;

    // type check the func
    t_CKTYPE f = type_engine_check_exp( env, func_call->func );
    if( !f ) return NULL;

    // void type for args
    t_CKTYPE a = &t_void;

    // make sure we have a function
    if( isa( f, &t_function ) )
    {
        EM_error2( func_call->linepos, "function call using a non-function value" );
        return NULL;
    }

    // get the function and set it in the func_call
    func_call->ck_func = func = env->func;

    // check the arguments
    if( func_call->args )
    {
        a = type_engine_check_exp( env, func_call->args );
        if( !a ) return NULL;
    }

    a_Exp e = func_call->args;
    a_Arg_List e1 = func->def->arg_list;
    unsigned int count = 1;

    // check arguments against the definition
    while( e )
    {
        if( e1 == NULL )
        {
            EM_error2( func_call->linepos,
                "extra argument(s) in function call '%s' %i %s",
                func->name.c_str(), e->type->c_name() );
            return NULL;
        }

        // no match
        if( !isa( e->type, e1->type ) )
        {
            EM_error2( func_call->linepos,
                "argument '%i' of function call '%s' has type '%s' -- expecting type '%s'",
                count, func->name.c_str(), e->type->c_name(), e1->type->c_name() );
            return NULL;
        }

        e = e->next;
        e1 = e1->next;
        count++;
    }

    // anything left
    if( e1 != NULL )
    {
        EM_error2( func_call->linepos,
            "missing argument(s) in function call '%s', next arg type: '%s'",
            func->name.c_str(), e1->type->c_name() );
        return NULL;
    }

    return func->def->ret_type;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp_dot_member()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp_dot_member( Chuck_Env * env, a_Exp_Dot_Member member )
{
    // type check the base
    member->t_base = type_engine_check_exp( env, member->base );
    if( !member->t_base ) return NULL;
    
    if( !member->t_base->info )
    {
        // base type does not have members
        EM_error2( member->base->linepos,
            "type '%s' does not have members - invalid use in dot expression",
            member->t_base->c_name() );
        return NULL;
    }

    // find the value
    Chuck_Value * m = member->t_base->info->lookup_value( member->id, FALSE );
    if( !m )
    {
        // can't find member
        EM_error2( member->base->linepos,
            "class '%s' has no member '%s'",
            member->t_base->c_name(), S_name(member->id) );
        return NULL;
    }
    
    return m->type;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_exp_array()
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE type_engine_check_exp_array( Chuck_Env * env, a_Exp_Array array )
{
    // type check the base
    t_CKTYPE t_base = type_engine_check_exp( env, array->base );
    if( !t_base ) return NULL;
    
    // dimension
    if( array->indices->depth > t_base->array_depth )
    {
        EM_error2( array->linepos,
            "array subscripts (%i) exceeds defined dimension (%i)",
            array->indices->depth, t_base->array_depth );
        return NULL;
    }
    
    // type check the index
    t_CKTYPE t_index = type_engine_check_exp( env, array->indices->exp_list );
    if( !t_index ) return NULL;
    
    // cycle through each exp
    a_Exp e = array->indices->exp_list;
    // count the dimension
    t_CKUINT depth = 0;
    
    while( e )
    {
        // increment
        depth++;
        // check if index is of valid type
        if( !isa( e->type, &t_int ) && !isa( e->type, &t_string ) )
        {
            // not int or string
            EM_error2( e->linepos,
                "array index %i must be of type 'int' or 'string', not '%s'",
                depth, e->type->c_name() );
            return NULL;
        }

        // advance the list
        e = e->next;
    }

    // sanity
    assert( array->indices->depth == depth );
    
    // make sure depth <= max
    t_CKTYPE t = t_base->copy( env );
    if( depth == t_base->array_depth ) t->array_depth = 0;
    else t->array_depth = t_base->array_depth - depth;

    return t;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_class_def()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_class_def( Chuck_Env * env, a_Class_Def class_def )
{
    // make new type for class def
    t_CKTYPE t_class = NULL;
    // the parent class
    t_CKTYPE t_parent = NULL;
    // the return type
    t_CKBOOL ret = TRUE;
    // the class body
    a_Class_Body body = class_def->body;

    // make sure class not already in namespace
    if( env->curr->lookup_type( class_def->name->id, TRUE ) )
    {
        EM_error2( class_def->name->linepos,
            "class name '%s' is already a defined type in current context",
            S_name(class_def->name->id) );
        return FALSE;
    }

    // check if reserved
    if( type_engine_check_reserved( env, class_def->name->id, class_def->name->linepos ) )
    {
        EM_error2( class_def->name->linepos,
            "...in class definition '%s'",
            S_name(class_def->name->id) );
        return FALSE;
    }

    // make sure inheritance
    if( class_def->ext )
    {
        t_parent = env->curr->lookup_type( class_def->ext->extend_id, TRUE );
        if( !t_parent )
        {
            EM_error2( class_def->ext->linepos,
                "undefined super class '%s' in definition of class '%s'",
                S_name(class_def->ext->extend_id), S_name(class_def->name->id) );
            return FALSE;
        }
    }

    // allocate new type
    t_class = new Chuck_Type;
    // set the fields
    t_class->id = te_user;
    t_class->name = S_name(class_def->name->id);
    t_class->parent = t_parent;
    t_class->size = 0; // to be filled in
    t_class->owner = env->curr;
    t_class->array_depth = 0;
    t_class->info = new Chuck_Namespace;
    t_class->info->name = t_class->name;
    t_class->info->parent = env->curr;
    t_class->func = NULL;

    // set the new type as current
    env->stack.push_back( env->curr );
    env->curr = t_class->info;
    env->class_def = t_class;

    // type check the body
    while( body && ret )
    {
        // check the section
        switch( body->section->s_type )
        {
        case ae_section_stmt:
            ret = type_engine_check_stmt_list( env, body->section->stmt_list );
            break;
        
        case ae_section_func:
            ret = type_engine_check_func_def( env, body->section->func_def );
            break;
        
        case ae_section_class:
            EM_error2( body->section->class_def->linepos,
                "nested class definitions are not yet supported..." );
            ret = FALSE;
            break;
        }
        
        // move to the next section
        body = body->next;
    }

    // pop the new type
    env->class_def = NULL;
    env->curr = env->stack.back();
    env->stack.pop_back();
    
    // if things checked out
    if( ret )
    {
        // add to env
        env->curr->type.add( t_class->name, t_class );
        // TODO: clean up if the context failed
    }
    else
    {
        // delete the class definition
        t_class->release();
    }

    return ret;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_func_def()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_func_def( Chuck_Env * env, a_Func_Def f )
{
    Chuck_Value * value = NULL;
    Chuck_Func * func = NULL;
    a_Arg_List arg_list = NULL;
    t_CKUINT count = 0;

    // see if we are already in a function definition
    if( env->func != NULL )
    {
        EM_error2( f->linepos,
            "nested function definitions are not (yet) allowed" );
        return FALSE;
    }

    // check if reserved
    if( type_engine_check_reserved( env, f->name, f->linepos ) )
    {
        EM_error2( f->linepos,
            "...in function definition '%s'",
            S_name(f->name) );
        return FALSE;
    }

    // look up the value
    if( env->curr->lookup_value( f->name, TRUE ) )
    {
        EM_error2( f->linepos, 
            "function name '%s' is already used by another value", S_name(f->name) );
        return FALSE;
    }

    // make sure a code segment is in stmt - else we should push scope
    assert( !f->code || f->code->s_type == ae_stmt_code );

    // make a new func object
    func = new Chuck_Func;
    // set the name
    func->name = S_name(f->name);
    // reference the function definition
    func->def = f;
    // note whether the function is marked (so far) as instance
    func->instance = f->static_decl != ae_key_static;

    // set the current function to this
    env->func = func;
    // push the value stack
    env->curr->value.push();

    // make room for return address and pc
    // S_enter( env->value, insert_symbol( "@__pc__" ), &t_uint );
    // S_enter( env->value, insert_symbol( "@__mem_sp__" ), &t_uint );

    // look up the return type
    f->ret_type = env->curr->lookup_type( f->type_decl->id->id );
    // no return type
    if( !f->ret_type )
    {
        EM_error2( f->linepos, "for function '%s':", S_name(f->name) );
        EM_error2( f->linepos, "undefined return type '%s'", S_name(f->type_decl->id->id) );
        goto error;
    }

    // TODO: deal with arrays

    // look up types for the function arguments
    arg_list = f->arg_list;
    count = 1;
    f->stack_depth = 0;
    while( arg_list )
    {
        // look up in type table
        arg_list->type = env->curr->lookup_type( arg_list->type_decl->id->id );
        if( !arg_list->type )
        {
            EM_error2( arg_list->linepos, "in function '%s':", S_name(f->name) );
            EM_error2( arg_list->linepos, "argument %i '%s' has undefined type '%s'", 
                count, S_name(arg_list->id), S_name(arg_list->type_decl->id->id) );
            goto error;
        }

        // check if reserved
        if( type_engine_check_reserved( env, arg_list->id, arg_list->linepos ) )
        {
            EM_error2( arg_list->linepos, "in function '%s'", S_name(f->name) );
            goto error;
        }

        // look up in scope
        if( env->curr->lookup_value( arg_list->id, FALSE ) )
        {
            EM_error2( arg_list->linepos, "in function '%s':", S_name(f->name) );
            EM_error2( arg_list->linepos, "argument %i '%s' is already defined in this scope",
                count, S_name(arg_list->id) );
            goto error;
        }

        // enter into value table
        value = new Chuck_Value( 
            arg_list->type, S_name(arg_list->id), NULL, FALSE, 0, NULL );
        env->curr->value.add( arg_list->id, value );

        // stack
        f->stack_depth += arg_list->type->size;

        // next arg
        arg_list = arg_list->next;
    }

    // type check the code
    if( f->code && !type_engine_check_stmt( env, f->code ) )
    {
        EM_error2( 0, "...in function '%s'", S_name(f->name) );
        goto error;
    }

    // pop the value stack
    env->curr->value.pop();
    // enter the name into the value table
    value = new Chuck_Value( func->def->ret_type, S_name(f->name), NULL, TRUE, 0, NULL );
    env->curr->value.add( f->name, value );
    // enter the name into the function table
    env->curr->func.add( f->name, func );
    // clear the env's function definition
    env->func = NULL;

    return TRUE;

error:

    // clean up
    if( func )
    {
        env->func = NULL;
        func->release();
    }

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_func_def_import()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_func_def_import( Chuck_Env * env, a_Func_Def f )
{
    assert( f->code == NULL );

    // builtin
    f->s_type = ae_func_builtin;
    // type check it
    if( !type_engine_check_func_def( env, f ) )
    {
        EM_error2( 0, "...in imported function '%s'", S_name(f->name) );
        return FALSE;
    }

    return TRUE;
}




// ctrl op
UGEN_CTRL ugen_ctrl_op( t_CKTIME now, void * data, void * value ) { }
UGEN_CGET ugen_cget_op( t_CKTIME now, void * data, void * out ) { }
// ctrl gain
UGEN_CTRL ugen_ctrl_gain( t_CKTIME now, void * data, void * value ) { }
UGEN_CGET ugen_cget_gain( t_CKTIME now, void * data, void * out ) { }
// cget last
UGEN_CGET ugen_cget_last( t_CKTIME now, void * data, void * out ) { }
// ctrl next
UGEN_CTRL ugen_ctrl_next( t_CKTIME now, void * data, void * out ) { }


//-----------------------------------------------------------------------------
// name: type_engine_check_ugen_def_import()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_ugen_def_import( Chuck_Env * env, Chuck_UGen_Info * ugen )
{
    map<string, bool> params;
    Chuck_Type * type = NULL;
    Chuck_Type * t_class = NULL, * t_parent = NULL;

    // look up the type
    if( env->curr->lookup_type( ugen->name, FALSE ) )
    {
        EM_error2( ugen->linepos,
            "imported ugen '%s.%s': \n  type identifier '%s' is in use in namespace '%s'",
            env->curr->name.c_str(), ugen->name.c_str(), 
            ugen->name.c_str(), env->curr->name.c_str() );
        return FALSE;
    }

    // check if reserved
    if( type_engine_check_reserved( env, ugen->name, ugen->linepos ) )
    {
        EM_error2( ugen->linepos,
            "...in imported ugen '%s.%s'",
            env->curr->name.c_str(), ugen->name.c_str() );
        return FALSE;
    }
    
    // no return type
    if( !ugen->tick )
    {
        EM_error2( ugen->linepos, 
            "imported ugen '%s.%s': no [tick] method defined",
            env->curr->name.c_str(), ugen->name.c_str() );
        return FALSE;
    }

    // type check parent
    if( ugen->parent != "" )  // trim later
    {
        type = env->curr->lookup_type( ugen->parent, TRUE );
        if( !type )
        {
            EM_error2( ugen->linepos,
                "imported ugen '%s.%s' extends undefined type '%s'",
                env->curr->name.c_str(), ugen->name.c_str(),
                ugen->parent.c_str() );
            return FALSE;
        }

        // make sure we are extending from ugen
        if( !isa( type, &t_ugen ) )
        {
            EM_error2( ugen->linepos,
                "imported ugen '%s.%s' inherits from '%s' -> not a 'ugen'",
                env->curr->name.c_str(), ugen->name.c_str(),
                ugen->parent.c_str() );
            return FALSE;
        }

        t_parent = type;
    }
    else
    {
        t_parent = &t_ugen;
    }
    
    // add default
    ugen->add( ugen_ctrl_op, ugen_cget_op, "int", "op" );
    ugen->add( ugen_ctrl_gain, ugen_cget_gain, "float", "gain" );
    ugen->add( NULL, ugen_cget_last, "float", "last" );
    ugen->add( ugen_ctrl_next, NULL, "float", "next" );

    // loop through ctrl parameters
    for( unsigned int i = 0; i < ugen->param_list.size(); i++ )
    {
        const Chuck_Info_Param * param = &ugen->param_list[i];
        
        // check the type
        type = env->curr->lookup_type( param->type, TRUE );
        if( !type )
        {
            EM_error2( ugen->linepos,
                "imported ugen '%s.%s': unrecognized type '%s' for control parameter '%s'",
                env->curr->name.c_str(), ugen->name.c_str(), 
                param->type.c_str(), param->name.c_str() );
            return FALSE;
        }

        // check if reserved
        if( type_engine_check_reserved( env, param->name, 0 ) )
        {
            EM_error2( 0, "...in imported ugen '%s.%s'",
                env->curr->name.c_str(), ugen->name.c_str() );
            return FALSE;
        }

        // XXX - pld
        // in general, this just updates the param_map and the
        // older functions are 'forgotten' though still present 
        // in the param list
        if( params[param->name] )
        {
            EM_error2( ugen->linepos, 
                "imported ugen '%s.%s': duplicate control parameter name '%s'",
                env->curr->name.c_str(), ugen->name.c_str(), param->name.c_str() );
            return FALSE;
        }

        // make sure there is a function
        if( !param->ctrl_addr && !param->cget_addr )
        {
            EM_error2( ugen->linepos,
                "imported ugen '%s.%s': no ctrl or cget function defined for param '%s'",
                env->curr->name.c_str(), ugen->name.c_str(), param->name.c_str() );
            return FALSE;
        }
        
        params[param->name] = true;
    }

    // allocate new type
    t_class = new Chuck_Type;
    // init as vm object
    // t_class->init();
    // set the fields
    t_class->id = te_user;
    t_class->name = ugen->name;
    t_class->parent = t_parent;
    t_class->size = 0; // to be filled in
    t_class->owner = env->curr;
    t_class->array_depth = 0;
    t_class->info = NULL;
    t_class->func = NULL;

    // add to env
    env->curr->type.add( t_class->name, t_class );
    // add as ugen
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_value_import()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_value_import( Chuck_Env * env, const string & name,
										 const string & type, void * addr )
{
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: do_make_args()
// desc: ...
//-----------------------------------------------------------------------------
a_Arg_List do_make_args( const vector<Chuck_Info_Param> & params, t_CKUINT index )
{
    a_Arg_List args = NULL;
    if( index >= params.size() )
        return NULL;

    if( index == (params.size()-1) )
        args = new_arg_list( new_type_decl( new_id_list((char*)params[index].type.c_str(), 0), 0 ),
                             (char*)params[index].name.c_str(), 0 );
    else
        args = prepend_arg_list(
            new_type_decl( new_id_list((char*)params[index].type.c_str(), 0), 0 ),
            (char*)params[index].name.c_str(),
            do_make_args( params, index + 1 ), 0 );
    return args;
}




//-----------------------------------------------------------------------------
// name: type_engine_add_dll()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_add_dll( Chuck_Env * env, Chuck_DLL * dll, const char * name )
{    
    // lookup the namesapce
    Chuck_Namespace * nspc = NULL;
    Chuck_Type * type = NULL;
    
    // go through type - which is what namespaces are in ChucK
    if( type = env->global.lookup_type( string(name) ) )
        nspc = type->info;

    // if already there
    if( env->dlls[dll] )
        return FALSE;

    // if not there
    if( !nspc )
    {
        // allocate new namespace
        nspc = new Chuck_Namespace;
        if( !type )
        {
            // add as new type/namespace
            type = new Chuck_Type( te_user, name, NULL, 0 );
            env->global.type.add( string(name), type );
        }
        // set the nspc
        type->info = nspc;
        nspc->name = type->name;
        nspc->parent = type->info;
    }

    // add all the prototypes
    Chuck_DL_Query * query = (Chuck_DL_Query *)dll->query();
    for( t_CKUINT i = 0; i < query->dll_exports.size(); i++ )
    {
        // the prototype
        Chuck_DL_Proto * proto = &query->dll_exports[i];

        // add func
        if( proto->is_func )
        {
            // type decl
            a_Type_Decl rtype = new_type_decl( new_id_list((char *)proto->type.c_str(), 0), 0 );
            // arg list
            a_Arg_List args = NULL;
            if( proto->params.size() )
                args = do_make_args( proto->params, 0 );
            // allocate a new function
            a_Func_Def func = new_func_def( 
                ae_key_func, ae_key_public, rtype, (char*)proto->name.c_str(), args, NULL, 0 );
            // set the pointer
            func->builtin = (builtin_func_ptr)proto->addr;
            func->linepos = query->linepos;
            // type check it
            if( !type_engine_check_func_def_import( env, func ) )
            {
                // clean up
                // env->dlls.erase( dll );
                return FALSE;
            }
        }
        else
        {
            // add value
            if( !type_engine_check_value_import( env,
                proto->name, proto->type, (void *)proto->addr ) )
            {
                // info->dlls.erase( dll );
                return FALSE;
            }
        }
    }

    // add the unit generators
    for( t_CKUINT j = 0; j < query->ugen_exports.size(); j++ )
    {
        Chuck_UGen_Info * ugen = new Chuck_UGen_Info( query->ugen_exports[j] );
        
        if( !type_engine_check_ugen_def_import( env, ugen ) )
        {
            // clean up
            // info->dlls.erase( dll );
            return FALSE;
        }
    }

    // flag
    env->dlls[dll] = true;

    return TRUE;
}

t_CKTYPE type_engine_check_exp_namespace( Chuck_Env * env, a_Exp_Namespace name_space );




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
// name: lookup_type()
// desc: lookup type in the env
//-----------------------------------------------------------------------------
Chuck_Type * Chuck_Namespace::lookup_type( S_Symbol name, t_CKBOOL climb )
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
// name: lookup_value()
// desc: lookup value in the env
//-----------------------------------------------------------------------------
Chuck_Value * Chuck_Namespace::lookup_value( S_Symbol name, t_CKBOOL climb )
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
// name: lookup_func()
// desc: lookup func in the env
//-----------------------------------------------------------------------------
Chuck_Func * Chuck_Namespace::lookup_func( S_Symbol name, t_CKBOOL climb )
{
    Chuck_Func * f = func.lookup( name );
    if( climb && !f && parent )
        return parent->lookup_func( name, climb );
    return f;
}




//-----------------------------------------------------------------------------
// name: lookup_ugen()
// desc: lookup ugen in the env
//-----------------------------------------------------------------------------
Chuck_UGen_Info * Chuck_Namespace::lookup_ugen( const string & name, t_CKBOOL climb )
{
    Chuck_UGen_Info * u = ugen.lookup( name );
    if( climb && !u && parent )
        return parent->lookup_ugen( name, climb );
    return u;
}




//-----------------------------------------------------------------------------
// name: lookup_ugen()
// desc: lookup ugen in the env
//-----------------------------------------------------------------------------
Chuck_UGen_Info * Chuck_Namespace::lookup_ugen( S_Symbol name, t_CKBOOL climb )
{
    Chuck_UGen_Info * u = ugen.lookup( name );
    if( climb && !u && parent )
        return parent->lookup_ugen( name, climb );
    return u;
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




//-----------------------------------------------------------------------------
// name: lookup_addr()
// desc: lookup addr in the env
//-----------------------------------------------------------------------------
void * Chuck_Namespace::lookup_addr( S_Symbol name, t_CKBOOL climb )
{
    void * a = addr.lookup( name );
    if( climb && !a && parent )
        return parent->lookup_addr( name, climb );
    return a;
}




//-----------------------------------------------------------------------------
// name: operator ==
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL operator ==( const Chuck_Type & lhs, const Chuck_Type & rhs )
{
    // check id
    if( lhs.id != rhs.id ) return FALSE;
    // check array depth
    if( lhs.array_depth != rhs.array_depth ) return FALSE;
     
    // if user-defined type
    if( lhs.id == te_user )
    {
        // check name
        if( lhs.name != rhs.name ) return FALSE;
        // check owner
        if( lhs.owner != rhs.owner ) return FALSE;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: equals()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL equals( Chuck_Type * lhs, Chuck_Type * rhs ) { return (*lhs) == (*rhs); }




//-----------------------------------------------------------------------------
// name: operator <=
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL operator <=( const Chuck_Type & lhs, const Chuck_Type & rhs )
{
    // check to see if type L == type R
    if( lhs == rhs ) return TRUE;
    
    // if lhs is a child of rhs
    Chuck_Type * curr = lhs.parent;
    while( curr )
    {
        if( *curr == rhs ) return TRUE;
        curr = curr->parent;
    }
    
    return FALSE;
}




//-----------------------------------------------------------------------------
// name: isa()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL isa( Chuck_Type * lhs, Chuck_Type * rhs ) { return (*lhs) <= (*rhs); }




//-----------------------------------------------------------------------------
// name: ~Chuck_Context()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Context::~Chuck_Context()
{
    // delete the types
    for( int i = 0; i < new_types.size(); i++ )
        new_types[i]->release();

    // TODO: delete abstract syntax tree * 
}




//-----------------------------------------------------------------------------
// name: type_engine_check_reserved()
// desc: ...
//--------------------------------------------------------------------------
t_CKBOOL type_engine_check_reserved( Chuck_Env * env, const string & id, int pos )
{
    // key word?
    if( env->key_words[id] )
    {
        EM_error2( pos, "illegal use of keyword '%s'.", id.c_str() );
        return TRUE;
    }

    // key value?
    if( env->key_values[id] )
    {
        EM_error2( pos, "illegal re-declaration of reserved value '%s'.",
            id.c_str() );
        return TRUE;
    }

    // key type?
    if( env->key_types[id] )
    {
        EM_error2( pos, "illegal use of reserved type id '%s'.",
            id.c_str() );
        return TRUE;
    }

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_reserved()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_reserved( Chuck_Env * env, S_Symbol id, int pos )
{
    return type_engine_check_reserved( env, string(S_name(id)), pos );
}
