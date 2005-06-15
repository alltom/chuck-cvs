/*----------------------------------------------------------------------------
    ChucK Concurrent, On-the-fly Audio Programming Language
      Compiler and Virtual Machine

    Copyright (c) 2004 Ge Wang and Perry R. Cook.  All rights reserved.
      http://chuck.cs.princeton.edu/
      http://soundlab.cs.princeton.edu/

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version._scan_

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
// file: chuck_scan.cpp
// desc: chuck type-system / type-checker pre-scan
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Summer 2005 - original
//-----------------------------------------------------------------------------
#include "chuck_type.h"
#include "chuck_scan.h"
#include "chuck_errmsg.h"
#include "chuck_vm.h"
#include "util_string.h"




//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_stmt_list( Chuck_Env * env, a_Stmt_List list );
t_CKBOOL type_engine_scan_stmt( Chuck_Env * env, a_Stmt stmt );
t_CKBOOL type_engine_scan_if( Chuck_Env * env, a_Stmt_If stmt );
t_CKBOOL type_engine_scan_for( Chuck_Env * env, a_Stmt_For stmt );
t_CKBOOL type_engine_scan_while( Chuck_Env * env, a_Stmt_While stmt );
t_CKBOOL type_engine_scan_until( Chuck_Env * env, a_Stmt_Until stmt );
t_CKBOOL type_engine_scan_break( Chuck_Env * env, a_Stmt_Break br );
t_CKBOOL type_engine_scan_continue( Chuck_Env * env, a_Stmt_Continue cont );
t_CKBOOL type_engine_scan_return( Chuck_Env * env, a_Stmt_Return stmt );
t_CKBOOL type_engine_scan_switch( Chuck_Env * env, a_Stmt_Switch stmt );
t_CKBOOL type_engine_scan_exp( Chuck_Env * env, a_Exp exp );
t_CKBOOL type_engine_scan_exp_binary( Chuck_Env * env, a_Exp_Binary binary );
t_CKBOOL type_engine_scan_op( Chuck_Env * env, ae_Operator op, a_Exp lhs, a_Exp rhs, a_Exp_Binary binary );
t_CKBOOL type_engine_scan_op_chuck( Chuck_Env * env, a_Exp lhs, a_Exp rhs, a_Exp_Binary binary );
t_CKBOOL type_engine_scan_op_unchuck( Chuck_Env * env, a_Exp lhs, a_Exp rhs );
t_CKBOOL type_engine_scan_op_at_chuck( Chuck_Env * env, a_Exp lhs, a_Exp rhs );
t_CKBOOL type_engine_scan_exp_unary( Chuck_Env * env, a_Exp_Unary unary );
t_CKBOOL type_engine_scan_exp_primary( Chuck_Env * env, a_Exp_Primary exp );
t_CKBOOL type_engine_scan_exp_array_lit( Chuck_Env * env, a_Exp_Primary exp );
t_CKBOOL type_engine_scan_exp_cast( Chuck_Env * env, a_Exp_Cast cast );
t_CKBOOL type_engine_scan_exp_postfix( Chuck_Env * env, a_Exp_Postfix postfix );
t_CKBOOL type_engine_scan_exp_dur( Chuck_Env * env, a_Exp_Dur dur );
t_CKBOOL type_engine_scan_exp_array( Chuck_Env * env, a_Exp_Array array );
t_CKBOOL type_engine_scan_exp_func_call( Chuck_Env * env, a_Exp_Func_Call func_call );
t_CKBOOL type_engine_scan_exp_func_call( Chuck_Env * env, a_Exp exp_func, a_Exp args, 
                                          t_CKFUNC & ck_func, int linepos );
t_CKBOOL type_engine_scan_exp_dot_member( Chuck_Env * env, a_Exp_Dot_Member member );
t_CKBOOL type_engine_scan_exp_if( Chuck_Env * env, a_Exp_If exp_if );
t_CKBOOL type_engine_scan_exp_decl( Chuck_Env * env, a_Exp_Decl decl );
t_CKBOOL type_engine_scan_array_subscripts( Chuck_Env * env, a_Exp exp_list );
t_CKBOOL type_engine_scan_cast_valid( Chuck_Env * env, t_CKTYPE to, t_CKTYPE from );
t_CKBOOL type_engine_scan_code_segment( Chuck_Env * env, a_Stmt_Code stmt, t_CKBOOL push = TRUE );
t_CKBOOL type_engine_scan_func_def( Chuck_Env * env, a_Func_Def func_def );
t_CKBOOL type_engine_scan_class_def( Chuck_Env * env, a_Class_Def class_def );




//-----------------------------------------------------------------------------
// name: type_engine_scan_init()
// desc: initialize a type engine scan
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_init( Chuck_Env * env )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_prog()
// desc: data in env should be ready
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_prog( Chuck_Env * env, a_Program prog )
{
    t_CKBOOL ret = TRUE;

    if( !prog )
        return FALSE;

    // go through each of the program sections
    while( prog && ret )
    {
        switch( prog->section->s_type )
        {
        case ae_section_stmt:
            ret = type_engine_scan_stmt_list( env, prog->section->stmt_list );
            break;
        
        case ae_section_func:
            ret = type_engine_scan_func_def( env, prog->section->func_def );
            break;

        case ae_section_class:
            // make global
            prog->section->class_def->home = env->global();
            ret = type_engine_scan_class_def( env, prog->section->class_def );
            break;
        
        default:
            EM_error2( prog->linepos,
                "internal error: unrecognized program section in type checker pre-scan..." );
            ret = FALSE;
            break;
        }

        prog = prog->next;
    }

    return ret;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_stmt_list()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_stmt_list( Chuck_Env * env, a_Stmt_List list )
{
    // type check the stmt_list
    while( list )
    {
        // the current statement
        if( !type_engine_scan_stmt( env, list->stmt ) )
            return FALSE;
        
        // advance to the next statement
        list = list->next;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_stmt(()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_stmt( Chuck_Env * env, a_Stmt stmt )
{
    t_CKBOOL ret = FALSE;

    if( !stmt )
        return TRUE;

    // the type of stmt
    switch( stmt->s_type )
    {
        case ae_stmt_if:
            // count scope to help determine class member
            env->class_scope++;
            env->curr->value.push();
            ret = type_engine_scan_if( env, &stmt->stmt_if );
            env->curr->value.pop();
            env->class_scope--;
            break;

        case ae_stmt_for:
            env->class_scope++;
            env->curr->value.push();
            ret = type_engine_scan_for( env, &stmt->stmt_for );
            env->curr->value.pop();
            env->class_scope--;
            break;

        case ae_stmt_while:
            env->class_scope++;
            env->curr->value.push();
            ret = type_engine_scan_while( env, &stmt->stmt_while );
            env->curr->value.pop();
            env->class_scope--;
            break;
            
        case ae_stmt_until:
            env->class_scope++;
            env->curr->value.push();
            ret = type_engine_scan_until( env, &stmt->stmt_until );
            env->curr->value.pop();
            env->class_scope--;
            break;

        case ae_stmt_exp:
            ret = type_engine_scan_exp( env, stmt->stmt_exp );
            break;

        case ae_stmt_return:
            ret = type_engine_scan_return( env, &stmt->stmt_return );
            break;

        case ae_stmt_code:
            env->class_scope++;
            ret = type_engine_scan_code_segment( env, &stmt->stmt_code );
            env->class_scope--;
            break;

        case ae_stmt_break:
            ret = type_engine_scan_break( env, &stmt->stmt_break );
            break;

        case ae_stmt_continue:
            ret = type_engine_scan_continue( env, &stmt->stmt_continue );
            break;

        case ae_stmt_switch:
            env->class_scope++;
            ret = type_engine_scan_switch( env, &stmt->stmt_switch );
            env->class_scope--;
            break;

        case ae_stmt_case:
            // ret = type_engine_scan_case( env, &stmt->stmt_case );
            break;

        case ae_stmt_gotolabel:
            // ret = type_engine_scan_gotolabel( env, &stmt->goto_label );
            break;

        //case ae_stmt_func:
        //    ret = type_engine_scan_func_def( env, stmt->stmt_func );
        //    break;
        
        default:
            EM_error2( stmt->linepos, 
                "internal compiler error (pre-scan) - no stmt type '%i'!", stmt->s_type );
            ret = FALSE;
            break;
    }

    return ret;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_if()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_if( Chuck_Env * env, a_Stmt_If stmt )
{
    // check the conditional
    if( !type_engine_scan_exp( env, stmt->cond ) )
        return FALSE;
        
    // TODO: ensure that conditional has valid type

    // check if
    if( !type_engine_scan_stmt( env, stmt->if_body ) )
        return FALSE;

    // check else, if there is one
    if( stmt->else_body )
        if( !type_engine_scan_stmt( env, stmt->else_body ) )
            return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_for()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_for( Chuck_Env * env, a_Stmt_For stmt )
{
    // check the initial
    if( !type_engine_scan_stmt( env, stmt->c1 ) )
        return FALSE;
    
    // check the conditional
    if( !type_engine_scan_stmt( env, stmt->c2 ) )
        return FALSE;

    // TODO: same as if - check conditional type valid

    // check the post
    if( stmt->c3 && !type_engine_scan_exp( env, stmt->c3 ) )
        return FALSE;

    // check body
    if( !type_engine_scan_stmt( env, stmt->body ) )
        return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_while()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_while( Chuck_Env * env, a_Stmt_While stmt )
{
    // check the conditional
    if( !type_engine_scan_exp( env, stmt->cond ) )
        return FALSE;
        
    // TODO: same as if - ensure the type in conditional is valid

    // check the body
    if( !type_engine_scan_stmt( env, stmt->body ) )
        return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_until()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_until( Chuck_Env * env, a_Stmt_Until stmt )
{
    // check the conditional
    if( !type_engine_scan_exp( env, stmt->cond ) )
        return FALSE;
        
    // TODO: same as if - ensure the type in conditional is valid

    // check the body
    if( !type_engine_scan_stmt( env, stmt->body ) )
        return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_switch()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_switch( Chuck_Env * env, a_Stmt_Switch stmt )
{
    // TODO: implement this
    EM_error2( stmt->linepos, "switch not implemented..." );

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_break()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_break( Chuck_Env * env, a_Stmt_Break br )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_continue()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_continue( Chuck_Env * env, a_Stmt_Continue cont )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_return()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_return( Chuck_Env * env, a_Stmt_Return stmt )
{
    t_CKBOOL ret = FALSE;

    // check the type of the return
    if( stmt->val )
        ret = type_engine_scan_exp( env, stmt->val );
    else
        ret = TRUE;

    return ret;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_code_segment()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_code_segment( Chuck_Env * env, a_Stmt_Code stmt,
                                        t_CKBOOL push )
{
    // push
    if( push ) env->curr->value.push(); // env->context->nspc.value.push();
    // do it
    t_CKBOOL t = type_engine_scan_stmt_list( env, stmt->stmt_list );
    // pop
    if( push ) env->curr->value.pop();  // env->context->nspc.value.pop();
    
    return t;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp( Chuck_Env * env, a_Exp exp )
{
    a_Exp curr = exp;
    t_CKBOOL ret = TRUE;
    
    // loop through parallel expressions
    while( curr )
    {
        // examine the syntax
        switch( curr->s_type )
        {
        case ae_exp_binary:
            ret = type_engine_scan_exp_binary( env, &curr->binary );
        break;
    
        case ae_exp_unary:
            ret = type_engine_scan_exp_unary( env, &curr->unary );
        break;
    
        case ae_exp_cast:
            ret = type_engine_scan_exp_cast( env, &curr->cast );
        break;
    
        case ae_exp_postfix:
            ret = type_engine_scan_exp_postfix( env, &curr->postfix );
        break;
    
        case ae_exp_dur:
            ret = type_engine_scan_exp_dur( env, &curr->dur );
        break;
    
        case ae_exp_primary:
            ret = type_engine_scan_exp_primary( env, &curr->primary );
        break;
    
        case ae_exp_array:
            ret = type_engine_scan_exp_array( env, &curr->array );
        break;
    
        case ae_exp_func_call:
            ret = type_engine_scan_exp_func_call( env, &curr->func_call );
        break;
    
        case ae_exp_dot_member:
            ret = type_engine_scan_exp_dot_member( env, &curr->dot_member );
        break;
    
        case ae_exp_if:
            ret = type_engine_scan_exp_if( env, &curr->exp_if );
        break;
    
        case ae_exp_decl:
            ret = type_engine_scan_exp_decl( env, &curr->decl );
        break;

        default:
            EM_error2( curr->linepos,
                "internal compiler error - no expression type '%i'...",
                curr->s_type );
            return FALSE;
        }

        // error
        if( !ret )
            return FALSE;

        // advance to next expression
        curr = curr->next;
    }

    // return type
    return ret;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_binary()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_binary( Chuck_Env * env, a_Exp_Binary binary )
{
    a_Exp cl = binary->lhs, cr = binary->rhs;

    // type check the lhs and rhs
    t_CKBOOL left = type_engine_scan_exp( env, cl );
    t_CKBOOL right = type_engine_scan_exp( env, cr);
    
    // if either fails, then return FALSE
    if( !left || !right )
        return FALSE;

    // cross chuck
    while( cr )
    {
        // type check the pair
        if( !type_engine_scan_op( env, binary->op, cl, cr, binary ) )
            return FALSE;

        cr = cr->next;
    }
        
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_op()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_op( Chuck_Env * env, ae_Operator op, a_Exp lhs, a_Exp rhs, 
                              a_Exp_Binary binary )
{
    // TODO: check for static here
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_op_chuck()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_op_chuck( Chuck_Env * env, a_Exp lhs, a_Exp rhs,
                                    a_Exp_Binary binary )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_op_unchuck()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_op_unchuck( Chuck_Env * env, a_Exp lhs, a_Exp rhs )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_op_at_chuck()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_op_at_chuck( Chuck_Env * env, a_Exp lhs, a_Exp rhs )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_unary()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_unary( Chuck_Env * env, a_Exp_Unary unary )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_primary()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_primary( Chuck_Env * env, a_Exp_Primary exp )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_array_lit()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_array_lit( Chuck_Env * env, a_Exp_Primary exp )
{
    // verify there are no errors from the parser...
    if( !verify_array( exp->array ) )
        return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_cast()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_cast( Chuck_Env * env, a_Exp_Cast cast )
{
    // check the exp
    t_CKBOOL t = type_engine_scan_exp( env, cast->exp );
    if( !t ) return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_dur()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_dur( Chuck_Env * env, a_Exp_Dur dur )
{
    // type check the two components
    t_CKBOOL base = type_engine_scan_exp( env, dur->base );
    t_CKBOOL unit = type_engine_scan_exp( env, dur->unit );
    
    // make sure both type check
    if( !base || !unit ) return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_postfix()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_postfix( Chuck_Env * env, a_Exp_Postfix postfix )
{
    // check the exp
    t_CKBOOL t = type_engine_scan_exp( env, postfix->exp );
    if( !t ) return FALSE;
    
    // syntax
    // TODO: figure out ++/--
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
                return FALSE;
            }
            
            // TODO: mark somewhere we need to post increment
            
            return TRUE;
        break;
        
        default:
            // no match
            EM_error2( postfix->linepos,
                "internal compiler error (pre-scan): unrecognized postfix '%i'", postfix->op );
            return FALSE;
    }
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_if()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_if( Chuck_Env * env, a_Exp_If exp_if )
{
    // check the components
    t_CKBOOL cond = type_engine_scan_exp( env, exp_if->cond );
    t_CKBOOL if_exp = type_engine_scan_exp( env, exp_if->if_exp );
    t_CKBOOL else_exp = type_engine_scan_exp( env, exp_if->else_exp );

    // make sure everything good
    if( !cond || !if_exp || !else_exp ) return FALSE;
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_array_subscripts( )
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_array_subscripts( Chuck_Env * env, a_Exp exp_list )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_decl( )
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_decl( Chuck_Env * env, a_Exp_Decl decl )
{
    a_Var_Decl_List list = decl->var_decl_list;
    a_Var_Decl var_decl = NULL;
    Chuck_Value * value = NULL;
    t_CKBOOL primitive = FALSE;
    t_CKBOOL do_alloc = TRUE;
    t_CKBOOL is_member = FALSE;
    t_CKINT is_static = -1;

    // TODO: handle T a, b, c ...
    // look up the type

    // loop through the variables
    while( list != NULL )
    {
        // get the decl
        var_decl = list->var_decl;

        // TODO: this needs to be redone
        // check if array
        if( var_decl->array != NULL )
        {
            // verify there are no errors from the parser...
            if( !verify_array( var_decl->array ) )
                return FALSE;

            // may be partial and empty []
            if( var_decl->array->exp_list )
            {
                // type check the exp
                if( !type_engine_scan_exp( env, var_decl->array->exp_list ) )
                    return FALSE;
                // make sure types are of int
                if( !type_engine_scan_array_subscripts( env, var_decl->array->exp_list ) )
                    return FALSE;
            }
        }

        // member?
        if( is_member )
        {
            // check to see if consistent with stmt
            if( is_static == 1 ) // static
            {
                EM_error2( var_decl->linepos,
                    "cannot mix static and non-static declarations in the same statement" );
                return FALSE;
            }
            else is_static = 0;
        }
        else if( env->class_def != NULL && decl->is_static ) // static
        {
            // base scope
            if( env->class_scope > 0 )
            {
                EM_error2( decl->linepos,
                    "static variables must be declared at class scope..." );
                return FALSE;
            }

            // check to see if consistent with stmt
            if( is_static == 0 ) // non static
            {
                EM_error2( var_decl->linepos,
                    "cannot mix static and non-static declarations in the same statement" );
                return FALSE;
            }
            else is_static = 1;
        }
        else // local variable
        {
            // do nothing?
        }

        // the next var decl
        list = list->next;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_func_call()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_func_call( Chuck_Env * env, a_Exp exp_func, a_Exp args, 
                                         t_CKFUNC & ck_func, int linepos )
{
    Chuck_Func * func = NULL;
    Chuck_Func * up = NULL;

    // type check the func
    t_CKBOOL f = type_engine_scan_exp( env, exp_func );
    if( !f ) return FALSE;

    // check the arguments
    if( args )
    {
        t_CKBOOL a = type_engine_scan_exp( env, args );
        if( !a ) return FALSE;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_func_call()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_func_call( Chuck_Env * env, a_Exp_Func_Call func_call )
{
    // type check it
    return type_engine_scan_exp_func_call( env, func_call->func, func_call->args,
                                           func_call->ck_func, func_call->linepos );
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_dot_member()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_dot_member( Chuck_Env * env, a_Exp_Dot_Member member )
{
    // type check the base
    t_CKBOOL base = type_engine_scan_exp( env, member->base );
    if( !base ) return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_exp_array()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_exp_array( Chuck_Env * env, a_Exp_Array array )
{
    // verify there are no errors from the parser...
    if( !verify_array( array->indices ) )
        return FALSE;

    // type check the base
    t_CKBOOL base = type_engine_scan_exp( env, array->base );
    if( !base ) return FALSE;
        
    // type check the index
    t_CKBOOL index = type_engine_scan_exp( env, array->indices->exp_list );
    if( !index ) return FALSE;
    
    // cycle through each exp
    a_Exp e = array->indices->exp_list;
    // count the dimension
    t_CKUINT depth = 0;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_class_def()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_class_def( Chuck_Env * env, a_Class_Def class_def )
{
    // make new type for class def
    t_CKTYPE the_class = NULL;
    // the parent class
    t_CKTYPE t_parent = NULL;
    // the return type
    t_CKBOOL ret = TRUE;
    // the class body
    a_Class_Body body = class_def->body;

    // if nspc is attached to class_def, that means the class_def is to be
    // put in that namespace.  this is usually the case when doing import
    if( class_def->home != NULL )
    {
        // set the new type as current
        env->nspc_stack.push_back( env->curr );
        env->curr = class_def->home;
    }

    // make sure class not already in namespace
    if( env->curr->lookup_type( class_def->name->id, TRUE ) )
    {
        EM_error2( class_def->name->linepos,
            "class/type '%s' is already defined in namespace '%s'",
            S_name(class_def->name->id), env->curr->name.c_str() );
        return FALSE;
    }

    // check if reserved
    if( type_engine_check_reserved( env, class_def->name->id, class_def->name->linepos ) )
    {
        EM_error2( class_def->name->linepos, "...in class definition '%s'",
            S_name(class_def->name->id) );
        return FALSE;
    }

    // allocate new type
    assert( env->context != NULL );
    the_class = env->context->new_Chuck_Type();
    // set the fields
    the_class->id = te_user;
    the_class->name = S_name(class_def->name->id);
    the_class->owner = env->curr;
    the_class->array_depth = 0;
    the_class->size = sizeof(void *);
    the_class->obj_size = 0;  // TODO:
    the_class->info = env->context->new_Chuck_Namespace();
    the_class->info->name = the_class->name;
    the_class->info->parent = env->curr;
    the_class->func = NULL;
    the_class->def = class_def;
    // add to env
    env->curr->type.add( the_class->name, the_class );  // URGENT: make this global
    // incomplete
    the_class->is_complete = FALSE;

    // set the new type as current
    env->nspc_stack.push_back( env->curr );
    env->curr = the_class->info;
    // push the class def
    env->class_stack.push_back( env->class_def );
    env->class_def = the_class;
    // reset the nest list
    env->class_scope = 0;

    // type check the body
    while( body && ret )
    {
        // check the section
        switch( body->section->s_type )
        {
        case ae_section_stmt:
            // flag as having a constructor
            env->class_def->has_constructor |= (body->section->stmt_list->stmt != NULL);
            ret = type_engine_scan_stmt_list( env, body->section->stmt_list );
            break;
        
        case ae_section_func:
            // set to complete
            env->class_def->is_complete = TRUE;
            ret = type_engine_scan_func_def( env, body->section->func_def );
            // back
            env->class_def->is_complete = FALSE;
            break;
        
        case ae_section_class:
            // do the class
            ret = type_engine_scan_class_def( env, body->section->class_def );
            break;
        }
        
        // move to the next section
        body = body->next;
    }


    // pop the class
    env->class_def = env->class_stack.back();
    env->class_stack.pop_back();
    // pop the namesapce
    env->curr = env->nspc_stack.back();
    env->nspc_stack.pop_back();

    // if things checked out
    if( ret )
    {
        Chuck_Value * value = NULL;
        Chuck_Type * type = NULL;

        // allocate value
        type = t_class.copy( env );
        type->actual_type = the_class;
        value = env->context->new_Chuck_Value( type, the_class->name );
        value->owner = env->curr;
        value->is_const = TRUE;
        value->is_member = FALSE;
        // add to env
        env->curr->value.add( the_class->name, value );

        // remember
        class_def->type = the_class;
    }

    // if nspc is attached to class_def, that means the class_def is to be
    // put in that namespace.  this is usually the case when doing import
    // we undo that extra namespace layer here...
    if( class_def->home != NULL )
    {
        // pop the namesapce
        env->curr = env->nspc_stack.back();
        env->nspc_stack.pop_back();
    }

    return ret;
}




//-----------------------------------------------------------------------------
// name: type_engine_scan_func_def()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL type_engine_scan_func_def( Chuck_Env * env, a_Func_Def f )
{
    Chuck_Type * type = NULL;
    Chuck_Value * value = NULL;
    Chuck_Func * func = NULL;

    Chuck_Type * parent = NULL;
    Chuck_Value * overload = NULL;
    Chuck_Value * override = NULL;
    Chuck_Value * v = NULL;
    Chuck_Func * parent_func = NULL;
    a_Arg_List arg_list = NULL;
    t_CKBOOL parent_match = FALSE;
    string func_name = S_name(f->name);
    vector<Chuck_Value *> values;
    vector<a_Arg_List> symbols;
    t_CKUINT count = 0;
    t_CKBOOL has_code = FALSE;  // use this for both user and imported
    t_CKUINT i;

    // see if we are already in a function definition
    if( env->func != NULL )
    {
        EM_error2( f->linepos,
            "nested function definitions are not (yet) allowed" );
        return FALSE;
    }

    // if not imported, then check to make sure no reserved word conflict
    // if( f->s_type != ae_func_builtin )  // TODO: fix this

    // check if reserved
    if( type_engine_check_reserved( env, f->name, f->linepos ) )
    {
        EM_error2( f->linepos, "...in function definition '%s'",
            S_name(f->name) );
        return FALSE;
    }

    // look up the value in the current class (can shadow?)
    if( overload = env->curr->lookup_value( f->name, FALSE ) )
    {
        // if value
        if( !isa( overload->type, &t_function ) )
        {
            EM_error2( f->linepos, 
                "function name '%s' is already used by another value", S_name(f->name) );
            return FALSE;
        }
        else 
        {
            // overload
            if( !overload->func_ref )
            {
                // error
                EM_error2( f->linepos,
                    "internal error: missing function '%s'",
                    overload->name.c_str() );
                return FALSE;
            }

            // make the new name
            func_name += "@" + itoa( ++overload->func_num_overloads ) + "@" + env->curr->name;
        }
    }

    // look up the value in the parent class
    if( env->class_def && 
        ( override = type_engine_find_value( env->class_def->parent, f->name ) ) )
    {
        // see if the target is a function
        if( !isa( override->type, &t_function ) )
        {
            EM_error2( f->linepos, "function name '%s' conflicts with previously defined value...",
                S_name(f->name) );
            EM_error2( f->linepos, "from super class '%s'...", override->owner_class->c_name() );
            return FALSE;
        }
    }

    // make sure a code segment is in stmt - else we should push scope
    assert( !f->code || f->code->s_type == ae_stmt_code );

    // make a new func object
    func = env->context->new_Chuck_Func();
    // set the name
    func->name = func_name;
    // reference the function definition
    func->def = f;
    // note whether the function is marked as member
    func->is_member = (f->static_decl != ae_key_static) && 
                      (env->class_def != NULL);
    // copy the native code, for imported functions
    if( f->s_type == ae_func_builtin )
    {
        // we can emit code now
        func->code = new Chuck_VM_Code;
        // whether the function needs 'this'
        func->code->need_this = func->is_member;
        // set the function pointer
        func->code->native_func = (t_CKUINT)func->def->dl_func_ptr;
    }

    // make a new type for the function
    type = env->context->new_Chuck_Type();
    type->id = te_function;
    type->name = "[function]";
    type->parent = &t_function;
    type->size = sizeof(void *);
    type->func = func;

    // make new value, with potential overloaded name
    value = env->context->new_Chuck_Value( type, func_name );
    // it is const
    value->is_const = TRUE;
    // remember the owner
    value->owner = env->curr;
    value->owner_class = env->class_def;
    value->is_member = func->is_member;
    // is global context
    value->is_context_global = env->class_def == NULL;
    // remember the func
    value->func_ref = func;
    // remember the value
    func->value_ref = value;

    // set the func
    f->ck_func = func;

    // if overload
    if( overload )
    {
        // add
        func->next = overload->func_ref->next;
        overload->func_ref->next = func;
    }

    // if override
    if( override )
    {
        // make reference to parent
        // TODO: ref count
        func->up = override;
    }

    // look up the return type
    // f->ret_type = env->curr->lookup_type( f->type_decl->id->id );
    f->ret_type = type_engine_find_type( env, f->type_decl->id );
    // no return type
    if( !f->ret_type )
    {
        EM_error2( f->linepos, "... in return type of function '%s' ...", S_name(f->name) );
        goto error;
    }

    // look up types for the function arguments
    arg_list = f->arg_list;
    count = 1;
    // make room (this)
    f->stack_depth = func->is_member ? sizeof(void *) : 0;
    while( arg_list )
    {
        // look up in type table
        // arg_list->type = env->curr->lookup_type( arg_list->type_decl->id->id );
        arg_list->type = type_engine_find_type( env, arg_list->type_decl->id );
        if( !arg_list->type )
        {
            // EM_error2( arg_list->linepos, "in function '%s':", S_name(f->name) );
            EM_error2( arg_list->linepos, 
                "... in argument %i '%s' of function '%s(.)' ...", 
                count, S_name(arg_list->var_decl->id), S_name(f->name) );
            goto error;
        }

        // make sure it's not void
        if( arg_list->type->size == 0 )
        {
            EM_error2( arg_list->linepos,
                "cannot declare variables of size '0' (i.e. 'void')..." );
            goto error;
        }

        // check if reserved
        if( type_engine_check_reserved( env, arg_list->var_decl->id, arg_list->linepos ) )
        {
            EM_error2( arg_list->linepos, "in function '%s'", S_name(f->name) );
            goto error;
        }

        // look up in scope: later
        //if( env->curr->lookup_value( arg_list->var_decl->id, FALSE ) )
        //{
        //    EM_error2( arg_list->linepos, "in function '%s':", S_name(f->name) );
        //    EM_error2( arg_list->linepos, "argument %i '%s' is already defined in this scope",
        //        count, S_name(arg_list->var_decl->id) );
        //    goto error;
        //}

        // check if array
        if( arg_list->var_decl->array != NULL )
        {
            // verify there are no errors from the parser...
            if( !verify_array( arg_list->var_decl->array ) )
                return FALSE;
            
            Chuck_Type * t = arg_list->type;
            Chuck_Type * t2 = t;
            // should be partial and empty []
            if( arg_list->var_decl->array->exp_list )
            {
                EM_error2( arg_list->linepos, "in function '%s':", S_name(f->name) );
                EM_error2( arg_list->linepos, "argument %i '%s' must be defined with empty []'s",
                    count, S_name(arg_list->var_decl->id) );
                return FALSE;
            }
            // make new type
            t = env->context->new_Chuck_Type();
            // set the id
            t->id = te_array;
            // set the name
            t->name = t2->name;
            // set the parent
            t->parent = &t_array;
            // is a ref
            t->size = t_array.size;
            // set the array depth
            t->array_depth = arg_list->var_decl->array->depth;
            // set the base type
            t->array_type = t2; // TODO: ref
            // set owner
            t->owner = env->curr;
            // set ref
            arg_list->type_decl->ref = TRUE;
            // set type
            arg_list->type = t;
        }
        
        // make new value
        v = env->context->new_Chuck_Value( 
            arg_list->type, S_name(arg_list->var_decl->id) );
        // remember the owner
        v->owner = env->curr;
        // function args not owned
        v->owner_class = NULL;
        v->is_member = FALSE;
        // add as value
        symbols.push_back( arg_list );
        values.push_back( v );
        // later: env->curr->value.add( arg_list->var_decl->id, v );

        // stack
        v->offset = f->stack_depth;
        f->stack_depth += arg_list->type->size;

        // remember
        arg_list->var_decl->value = v;

        // next arg
        arg_list = arg_list->next;
    }

    // only class functions can be pure
    if( !env->class_def && f->static_decl == ae_key_abstract )
    {
        EM_error2( f->linepos, "non-class function cannot be declared as 'pure'..." );
        EM_error2( f->linepos, "...at function '%s'", S_name(f->name) );
        goto error;
    }

    // figure out if the function has code associated with it
    if( f->s_type == ae_func_user ) has_code = ( f->code != NULL );
    else has_code = (f->dl_func_ptr != NULL); // imported

    // if interface, then cannot have code
    if( env->class_def && env->class_def->def && env->class_def->def->iface && has_code )
    {
        EM_error2( f->linepos, "interface function signatures cannot contain code..." );
        EM_error2( f->linepos, "...at function '%s'", S_name(f->name) );
        goto error;
    }
    // if pure, then cannot have code
    if( f->static_decl == ae_key_abstract && has_code )
    {
        EM_error2( f->linepos, "'pure' function signatures cannot contain code..." );
        EM_error2( f->linepos, "...at function '%s'", S_name(f->name) );
        goto error;
    }
    // yeah
    if( f->static_decl != ae_key_abstract && !has_code )
    {
        EM_error2( f->linepos, "function declaration must contain code..." );
        EM_error2( f->linepos, "(unless in interface, or is declared 'pure')" );
        EM_error2( f->linepos, "...at function '%s'", S_name(f->name) );
        goto error;
    }

    // if overriding super class function, then check signatures
    if( env->class_def )
    {
        // get parent
        parent = env->class_def->parent;
        while( parent && !parent_match )
        {
            if( v = type_engine_find_value( env->class_def->parent, f->name ) )
            {
                // see if the target is a function
                if( !isa( v->type, &t_function ) )
                {
                    EM_error2( f->linepos, "function name '%s' conflicts with previously defined value...",
                        S_name(f->name) );
                    EM_error2( f->linepos, "from super class '%s'...", v->owner_class->c_name() );
                    goto error;
                }

                // parent func
                parent_func = v->func_ref;

                // go through all overloading
                while( parent_func && !parent_match )
                {
                    // match the prototypes
                    string err;
                    if( !type_engine_compat_func( f, parent_func->def, f->linepos, err, FALSE ) )
                    {
                        // next
                        parent_func = parent_func->next;
                        // move on
                        continue;
                    }
                    /*{
                        EM_error2( f->linepos,
                            "function '%s.%s' resembles '%s.%s' but cannot override...",
                            env->class_def->c_name(), S_name(f->name),
                            value->owner_class->c_name(), S_name(f->name) );
                        if( err != "" ) EM_error2( f->linepos, "...(reason: %s)", err.c_str() );
                        goto error;
                    }*/

                    // see if parent function is static
                    if( parent_func->def->static_decl == ae_key_static )
                    {
                        EM_error2( f->linepos,
                            "function '%s.%s' resembles '%s.%s' but cannot override...",
                            env->class_def->c_name(), S_name(f->name), 
                            v->owner_class->c_name(), S_name(f->name) );
                        EM_error2( f->linepos,
                            "...(reason: '%s.%s' is declared as 'static')",
                            v->owner_class->c_name(), S_name(f->name) );
                        goto error;
                    }

                    // see if function is static
                    if( f->static_decl == ae_key_static )
                    {
                        EM_error2( f->linepos,
                            "function '%s.%s' resembles '%s.%s' but cannot override...",
                            env->class_def->c_name(), S_name(f->name), 
                            v->owner_class->c_name(), S_name(f->name) );
                        EM_error2( f->linepos,
                            "...(reason: '%s.%s' is declared as 'static')",
                            env->class_def->c_name(), S_name(f->name) );
                        goto error;
                    }

                    // see if function is pure
                    if( f->static_decl == ae_key_abstract )
                    {
                        EM_error2( f->linepos,
                            "function '%s.%s' resembles '%s.%s' but cannot override...",
                            env->class_def->c_name(), S_name(f->name), 
                            v->owner_class->c_name(), S_name(f->name) );
                        EM_error2( f->linepos,
                            "...(reason: '%s.%s' is declared as 'pure')",
                            env->class_def->c_name(), S_name(f->name) );
                        goto error;
                    }

                    // make sure returns are equal
                    if( *(f->ret_type) != *(parent_func->def->ret_type) )
                    {
                        EM_error2( f->linepos, "function signatures differ in return type..." );
                        EM_error2( f->linepos,
                            "function '%s.%s' matches '%s.%s' but cannot override...",
                            env->class_def->c_name(), S_name(f->name),
                            v->owner_class->c_name(), S_name(f->name) );
                        goto error;
                    }

                    // match
                    parent_match = TRUE;

                    // update virtual table
                    func->vt_index = parent_func->vt_index;
                    assert( func->vt_index < env->curr->obj_v_table.funcs.size() );
                    env->curr->obj_v_table.funcs[func->vt_index] = func;
                    // update name
                    func_name = parent_func->name;
                    func->name = func_name;
                    value->name = func_name;
                }
            }

            // move to next parent
            parent = parent->parent;        
        }
    }

    if( func->is_member && !parent_match )
    {
        // remember virtual table index
        func->vt_index = env->curr->obj_v_table.funcs.size();
        // append to virtual table
        env->curr->obj_v_table.funcs.push_back( func );
    }

    // add as value
    env->curr->value.add( value->name, value );
    // enter the name into the function table
    env->curr->func.add( func->name, func );

    // set the current function to this
    env->func = func;
    // push the value stack
    env->curr->value.push();
    
    // add args in the new scope
    for( i = 0; i < values.size(); i++ )
    {
        // look up in scope
        if( env->curr->lookup_value( symbols[i]->var_decl->id, FALSE ) )
        {
            EM_error2( symbols[i]->var_decl->linepos, "in function '%s':", S_name(f->name) );
            EM_error2( symbols[i]->var_decl->linepos, "argument %i '%s' is already defined in this scope",
                i+1, S_name(symbols[i]->var_decl->id) );
            goto error;
        }

        env->curr->value.add( symbols[i]->var_decl->id, values[i] );
    }

    // type check the code
    assert( f->code == NULL || f->code->s_type == ae_stmt_code );
    if( f->code && !type_engine_scan_code_segment( env, &f->code->stmt_code, FALSE ) )
    {
        EM_error2( 0, "...in function '%s'", S_name(f->name) );
        goto error;
    }

    // if imported, add the stack depth
    if( f->s_type == ae_func_builtin )
    {
        // set the stack depth?
        func->code->stack_depth = f->stack_depth;
        // if member add room for this
        // done: if( func->is_member ) func->code->stack_depth += sizeof(t_CKUINT);
    }

    // if overload
    if( overload )
    {
        // make sure returns are equal
        if( *(f->ret_type) != *(overload->func_ref->def->ret_type) )
        {
            EM_error2( f->linepos, "function signatures differ in return type..." );
            EM_error2( f->linepos,
                "function '%s.%s' matches '%s.%s' but cannot overload...",
                env->class_def->c_name(), S_name(f->name),
                value->owner_class->c_name(), S_name(f->name) );
            goto error;
        }
    }

    // pop the value stack
    env->curr->value.pop();
    
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
