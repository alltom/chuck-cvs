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
#include <assert.h>




//-----------------------------------------------------------------------------
// default types
//-----------------------------------------------------------------------------
struct Chuck_Type t_void = { te_void, "void", NULL, 0 };
struct Chuck_Type t_int = { te_int, "int", NULL, sizeof(t_CKINT) };
struct Chuck_Type t_float = { te_float, "float", NULL, sizeof(t_CKFLOAT) };
struct Chuck_Type t_time = { te_time, "time", NULL, sizeof(t_CKTIME) };
struct Chuck_Type t_dur = { te_dur, "dur", NULL, sizeof(t_CKTIME) };
struct Chuck_Type t_object = { te_object, "object", NULL, sizeof(void *) };
struct Chuck_Type t_null = { te_null, "@null", NULL, 0 };
struct Chuck_Type t_string = { te_string, "string", &t_object, sizeof(void *) };
struct Chuck_Type t_shred = { te_shred, "shred", &t_object, sizeof(void *) };
struct Chuck_Type t_thread = { te_thread, "thread", &t_object, sizeof(void *) };
struct Chuck_Type t_function = { te_function, "function", &t_object, sizeof(void *) };
struct Chuck_Type t_class = { te_class, "class", &t_object, sizeof(void *) };
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
    t_CKDUR second = vm->srate() * samp;
    t_CKDUR ms = second / 1000.0;
    t_CKDUR minute = second * 60.0;
    t_CKDUR hour = minute * 60.0;
    t_CKDUR day = hour * 24.0;
    t_CKDUR week = day * 7.0;

	// default global values
	env->global.value.add( "null", new Chuck_Value( &t_null, "null" ) );
	env->global.value.add( "now", new Chuck_Value( &t_time, "now", &(vm->shreduler()->now_system) ) );
	env->global.value.add( "tzero", new Chuck_Value( &t_time, "tzero" ) );
    env->global.value.add( "dzero", new Chuck_Value( &t_dur, "dzero" ) );
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
                "(type-checker): internal error: unrecognized program section in type checker..." );
            ret = FALSE;
            break;
        }

        prog = prog->next;
    }
    
    // pop the context scope
    env->context->nspc.value.pop();

    // check to see if everything passed
    if( !ret )
    {
        // make sure we still have the same context
        assert( env->contexts.back() == context );
        // TODO: remove the effects of the context in the env
        // ---> insert code here <----
        // remove the context
        env->contexts.pop_back();
        // delete it
        delete context;
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
                "(type-checker): internal compiler error - no stmt type '%i'!", stmt->s_type );
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
    env->loops.push_back( stmt->self );

    // check body
    if( !type_engine_check_stmt( env, stmt->body ) )
        return FALSE;
        
    // remove the loop from the stack
    assert( env->loops.size() && env->loops.back() == stmt->self );
    env->loops.pop_back();

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
    env->loops.push_back( stmt->self );

    // check the body
    if( !type_engine_check_stmt( env, stmt->body ) )
        return FALSE;

    // remove the loop from the stack
    assert( env->loops.size() && env->loops.back() == stmt->self );
    env->loops.pop_back();

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
    env->loops.push_back( stmt->self );

    // check the body
    if( !type_engine_check_stmt( env, stmt->body ) )
        return FALSE;

    // remove the loop from the stack
    assert( env->loops.size() && env->loops.back() == stmt->self );
    env->loops.pop_back();

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_switch()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_switch( Chuck_Env * env, a_Stmt_Switch stmt )
{
    // TODO: implement this
    EM_error2( stmt->linepos, "(type-checker): switch not implemented..." );

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: type_engine_check_break()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_check_break( Chuck_Env * env, a_Stmt_Break br )
{
    // check to see if inside valid stmt
    if( env->loops.size() <= 0 && env->swich.size() <= 0 )
    {
        EM_error2( br->linepos, "(type-checker): 'break' found outside of for/while/until/switch..." );
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
    if( env->loops.size() <= 0 )
    {
        EM_error2( cont->linepos, "(type-checker): 'continue' found outside of for/while/until..." );
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
        EM_error2( stmt->linepos, "(type-checker): 'return' statement found outside function definition" );
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
            "(type-checker): function '%s' was defined with return type '%s' -- but returning type '%s'",
            env->func->name.c_str(), env->func->def->ret_type->name.c_str(),
            ret_type->name.c_str() );
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
    // push
    env->context->nspc.value.push();
    // do it
    t_CKBOOL t = type_engine_check_stmt_list( env, stmt->stmt_list );
    // pop
    env->context->nspc.value.pop();
    
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
            curr->type = type_engine_check_exp_namespace( env, &curr->name_space );
        break;
    
        /* case ae_exp_new:
            curr->exp_type = (t_Type)S_look( env->type, curr->a_new->type );
        break;
        */
        
        default:
            EM_error2( curr->linepos, "(type-checker): internal compiler error - no expression '%i'",
                curr->s_type );
            return NULL;
        }

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
        if( !type_engine_check_op( env, binary->op, cl, cr ) )
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
            "(type-checker): multi-value binary operations not implemented..." );
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
        "(type-checker): no suitable resolution for binary operator '%s' on types '%s' and '%s'...",
        op2str( op ), left->name.c_str(), right->name.c_str() );
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
                "(type-checker): no suitable resolution for binary operator '=>' on types '%s' => '%s'...\n"
                "    (right-side operand is not mutable)",
                left->name.c_str(), right->name.c_str() );
            return NULL;
        }
        // aggregate types
        else
        {
            // TODO: check overloading of =>
            
            // no match
            EM_error2( lhs->linepos,
                "(type-checker): no suitable resolution for binary operator '=>' on types '%s' => '%s...'\n"
                "    (note: use @=> for assignment of objects)",
                left->name.c_str(), right->name.c_str() );
            return NULL;
        }
    }
    
    // time advance
    if( isa( left, &t_dur ) && isa( right, &t_time ) && rhs->s_meta == ae_meta_var )
        return right;
        
    // TODO: check overloading of =>

    // no match
    EM_error2( lhs->linepos,
        "(type-checker): no suitable resolution for binary operator '=>' on types '%s' and '%s'...",
        left->name.c_str(), right->name.c_str() );
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
        "(type-checker): no suitable resolution for binary operator '=<' on types '%s' and '%s'...",
        left->name.c_str(), right->name.c_str() );
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
                    "(type-checker): prefix unary operator '%s' cannot "
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
                     "(type-checker): only function calls can be sporked..." );
                 return NULL;
            }
        break;
    }
    
    // no match
    EM_error2( unary->linepos,
        "(type-checker): no suitable resolution for prefix unary operator '%s' on type '%s...",
        op2str( unary->op ), t->name.c_str() );
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
            v = env->curr->lookup_value( exp->var, env->dots == 0 );
            if( !v )
            {
                // error
                if( !env->dots )
                {
                    EM_error2( exp->linepos,
                        "(type-checker): undefined variable '%s'...",
                        S_name(exp->var) );
                }
                else
                {
                    EM_error2( exp->linepos,
                        "(type-checker): undefined member '%s' in class/namespace '%s'...",
                        S_name(exp->var), env->curr->name.c_str() );
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
                "(type-checker): internal error - unrecognized primary type '%i'...", exp->s_type );
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
    t_CKTYPE t2 = env->curr->lookup_type( cast->type, TRUE );
    if( !t2 )
    {
        EM_error2( cast->linepos,
            "(type-checker): undefined type '%s' in cast...",
            S_name( cast->type ) );
        return NULL;
    }
    
    // check if cast valid
    if( !type_engine_check_cast_valid( env, t2, t ) )
    {
        EM_error2( cast->linepos,
            "(type-checker): invalid cast to '%s' from '%s'...",
            S_name( cast->type ), t->name.c_str() );
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
            "(type-checker): invalid type '%s' in prefix of dur expression...\n"
            "    (must be of type 'int' or 'float')", base->name.c_str() );
        return NULL;
    }
    
    // check unit type
    if( !isa( unit, &t_dur ) )
    {
        EM_error2( dur->unit->linepos,
            "(type-checker): invalid type '%s' in postfix of dur expression...\n"
            "    (must be of type 'dur')", unit->name.c_str() );
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
                    "(type-checker): postfix operator '%s' cannot be used "
                    "on non-mutable data-type...", op2str( postfix->op ) );
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
                "(type-checker): internal compiler error: unrecognized postfix '%i'", postfix->op );
        return NULL;
    }
    
    // no match
    EM_error2( postfix->linepos,
        "(type-checker): no suitable resolutation for postfix operator '%s' on type '%s'...",
        op2str( postfix->op ), t->name.c_str() );
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
    if( !( *if_exp == *else_exp ) )
    {
        EM_error2( exp_if->linepos,
            "(type-checker): incompatible types '%s' and '%s' in if expression...",
            if_exp->name.c_str(), else_exp->name.c_str() );
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
    a_Var_Decl var_decl = decl->var_decl_list->var_decl;
    
    t_CKTYPE t = NULL;

    // look up the type
    t = env->curr->lookup_type( decl->type, TRUE );
    if( !t )
    {
        EM_error2( decl->linepos,
            "(type-checker): undefined type '%s'...",
            S_name(decl->type) );
        return NULL;
    }

    // check if locally defined
    if( env->context->nspc.value.lookup( var_decl->id, TRUE ) )
    {
        EM_error2( decl->linepos,
            "(type-checker): '%s' has already been defined in the same scope...",
            S_name(var_decl->id) );
        return NULL;
    }

    // check if array
    if( var_decl->isarray )
    {
        // make a copy of the type
        t = t->copy();
        // set the array depth
        t->array_depth = var_decl->isarray;
    }
    
    // enter into value binding
    env->context->nspc.value.add( var_decl->id, 
        new Chuck_Value( t, S_name(var_decl->id), NULL ) );

    return t;
}

t_CKTYPE type_engine_check_exp_dot_member( Chuck_Env * env, a_Exp_Dot_Member member );
t_CKTYPE type_engine_check_exp_func_call( Chuck_Env * env, a_Exp_Func_Call func_call );
t_CKTYPE type_engine_check_exp_array( Chuck_Env * env, a_Exp_Array array );
t_CKTYPE type_engine_check_exp_namespace( Chuck_Env * env, a_Exp_Namespace name_space );
t_CKBOOL type_engine_check_class_def( Chuck_Env * env, a_Class_Def class_def );
t_CKBOOL type_engine_check_func_def( Chuck_Env * env, a_Func_Def func_def );
// import
t_CKBOOL type_engine_check_func_def_import( Chuck_Env * env, a_Func_Def func_def );
t_CKBOOL type_engine_check_ugen_def_import( Chuck_Env * env, Chuck_UGen_Info * ugen );
t_CKBOOL type_engine_check_value_import( Chuck_Env * env, const string & name, 
										 const string & type, void * addr );




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
// name: lookup_class()
// desc: lookup class in the namespace
//-----------------------------------------------------------------------------
Chuck_Namespace * Chuck_Namespace::lookup_class( S_Symbol name, t_CKBOOL climb )
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
     
     // if user-defined type
     if( lhs.id == te_user )
     {
         // check name
         if( lhs.name != rhs.name ) return FALSE;
         
         // check array depth
         if( lhs.array_depth != rhs.array_depth ) return FALSE;
         
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
