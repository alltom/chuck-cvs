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
// file: chuck_emit.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002
//       Autumn 2003 - updated
//       Autumn 2004 - redesign
//-----------------------------------------------------------------------------
#include "chuck_emit.h"
#include "chuck_vm.h"
#include "chuck_errmsg.h"
#include "chuck_instr.h"




//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_stmt_list( Chuck_Emitter * emit, a_Stmt_List list );
t_CKBOOL emit_engine_emit_stmt( Chuck_Emitter * emit, a_Stmt stmt, t_CKBOOL pop = TRUE );
t_CKBOOL emit_engine_emit_if( Chuck_Emitter * emit, a_Stmt_If stmt );
t_CKBOOL emit_engine_emit_for( Chuck_Emitter * emit, a_Stmt_For stmt );
t_CKBOOL emit_engine_emit_while( Chuck_Emitter * emit, a_Stmt_While stmt );
t_CKBOOL emit_engine_emit_do_while( Chuck_Emitter * emit, a_Stmt_While stmt );
t_CKBOOL emit_engine_emit_until( Chuck_Emitter * emit, a_Stmt_Until stmt );
t_CKBOOL emit_engine_emit_do_until( Chuck_Emitter * emit, a_Stmt_Until stmt );
t_CKBOOL emit_engine_emit_break( Chuck_Emitter * emit, a_Stmt_Break br );
t_CKBOOL emit_engine_emit_continue( Chuck_Emitter * emit, a_Stmt_Continue cont );
t_CKBOOL emit_engine_emit_return( Chuck_Emitter * emit, a_Stmt_Return stmt );
t_CKBOOL emit_engine_emit_switch( Chuck_Emitter * emit, a_Stmt_Switch stmt );
t_CKBOOL emit_engine_emit_exp( Chuck_Emitter * emit, a_Exp exp );
t_CKBOOL emit_engine_emit_exp_binary( Chuck_Emitter * emit, a_Exp_Binary binary );
t_CKBOOL emit_engine_emit_op( Chuck_Emitter * emit, ae_Operator op, a_Exp lhs, a_Exp rhs );
t_CKBOOL emit_engine_emit_op_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );
t_CKBOOL emit_engine_emit_op_unchuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );
t_CKBOOL emit_engine_emit_exp_unary( Chuck_Emitter * emit, a_Exp_Unary unary );
t_CKBOOL emit_engine_emit_exp_primary( Chuck_Emitter * emit, a_Exp_Primary exp );
t_CKBOOL emit_engine_emit_exp_cast( Chuck_Emitter * emit, a_Exp_Cast cast );
t_CKBOOL emit_engine_emit_exp_postfix( Chuck_Emitter * emit, a_Exp_Postfix postfix );
t_CKBOOL emit_engine_emit_exp_dur( Chuck_Emitter * emit, a_Exp_Dur dur );
t_CKBOOL emit_engine_emit_exp_array( Chuck_Emitter * emit, a_Exp_Array array );
t_CKBOOL emit_engine_emit_exp_func_call( Chuck_Emitter * emit, a_Exp_Func_Call func_call );
t_CKBOOL emit_engine_emit_exp_dot_member( Chuck_Emitter * emit, a_Exp_Dot_Member member );
t_CKBOOL emit_engine_emit_exp_if( Chuck_Emitter * emit, a_Exp_If exp_if );
t_CKBOOL emit_engine_emit_exp_decl( Chuck_Emitter * emit, a_Exp_Decl decl );
t_CKBOOL emit_engine_emit_exp_namespace( Chuck_Emitter * emit, a_Exp_Namespace name_space );
t_CKBOOL emit_engine_emit_code_segment( Chuck_Emitter * emit, a_Stmt_Code stmt, t_CKBOOL push = TRUE );
t_CKBOOL emit_engine_emit_func_def( Chuck_Emitter * emit, a_Func_Def func_def );
t_CKBOOL emit_engine_emit_class_def( Chuck_Emitter * emit, a_Class_Def class_def );
t_CKBOOL emit_engine_emit_spork( Chuck_Emitter * emit, a_Exp_Func_Call exp );
t_CKBOOL emit_engine_emit_symbol( Chuck_Emitter * emit, S_Symbol symbol, 
                                  t_CKBOOL offset, int linepos );




//-----------------------------------------------------------------------------
// name: emit_engine_init()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Emitter * emit_engine_init( Chuck_Env * env )
{
    // allocate new emit
    Chuck_Emitter * emit = new Chuck_Emitter;
    // set the reference
    emit->env = env;

    return emit;
}




//-----------------------------------------------------------------------------
// name: emit_engine_shutdown()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_shutdown( Chuck_Emitter *& emit )
{
    if( !emit ) return FALSE;

    // delete
    delete emit;
    emit = NULL;

    return TRUE;    
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_prog()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_VM_Code * emit_engine_emit_prog( Chuck_Emitter * emit, a_Program prog )
{
    // make sure the code is NULL
    assert( emit->code == NULL );
    // make sure the stack is empty
    assert( emit->stack.size() == 0 );
    // make sure there is a context to emit
    assert( emit->env->context != NULL );
    // make sure no code
    assert( emit->env->context->code == NULL );

    // return
    t_CKBOOL ret = TRUE;
    // allocate the code
    emit->code = new Chuck_Code;
    // set the current context
    emit->context = emit->env->context;
    // set the namespace
    emit->nspc = &(emit->context->nspc);
    // reset the class
    emit->class_def = NULL;
    // reset the func
    emit->func = NULL;
    // clear the code stack
    emit->stack.clear();
    // clear the stack of continue
    emit->stack_cont.clear();
    // clear the stack of break
    emit->stack_break.clear();

    // loop over the program sections
    while( prog && ret )
    {
        switch( prog->section->s_type )
        {
        case ae_section_stmt: // code section
            ret = emit_engine_emit_stmt_list( emit, prog->section->stmt_list );
            break;

        case ae_section_func: // function definition
            ret = emit_engine_emit_func_def( emit, prog->section->func_def );
            break;

        case ae_section_class: // class definition
            ret = emit_engine_emit_class_def( emit, prog->section->class_def );
            break;

        default: // bad
            EM_error2( 0,
                "internal compiler error: unrecognized program section..." );
            ret = FALSE;
            break;
        }
    }

    if( ret )
    {
        // append end of code
        emit->append( new Chuck_Instr_EOC );

        // converted to virtual machine code
        emit->context->nspc.code = emit_to_code( emit );
    }

    // clear the code
    delete emit->code;
    emit->code = NULL;

    // return the code
    return emit->context->nspc.code;
}




//-----------------------------------------------------------------------------
// name: emit_to_code()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_VM_Code * emit_to_code( Chuck_Emitter * emit, t_CKBOOL dump )
{
    // allocate the vm code
    Chuck_VM_Code * code = new Chuck_VM_Code;
    // size
    code->num_instr = emit->code->code.size();
    // allocate instruction pointers
    code->instr = new Chuck_Instr *[code->num_instr];
    // set the stack depth
    code->stack_depth = emit->code->stack_depth;

    // copy
    for( t_CKUINT i = 0; i < code->num_instr; i++ )
        code->instr[i] = emit->code->code[i];

    // dump
    if( dump )
    {
        // name of what we are dumping
        EM_error2( 0, "dumping %s...", emit->code->name.c_str() );
        EM_error2( 0, "\n" );

        // uh
        for( t_CKUINT i = 0; i < code->num_instr; i++ )
            EM_error2( 0, "'%i' %s( %s )", i, 
               code->instr[i]->name(), code->instr[i]->params() );

        EM_error2( 0, "\n" );
        EM_error2( 0, "\n" );
    }

    return code;
}




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_addr_map( Chuck_Emitter * emit, Chuck_VM_Shred * shred );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_resolve( );




//-----------------------------------------------------------------------------
// name: emit_engine_emit_stmt_list()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_stmt_list( Chuck_Emitter * emit, a_Stmt_List list )
{
    t_CKBOOL ret = TRUE;

    // emit->push();
    while( list && ret )
    {
        ret = emit_engine_emit_stmt( emit, list->stmt );
        list = list->next;
    }
    // emit->pop();

    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_stmt()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_stmt( Chuck_Emitter * emit, a_Stmt stmt, t_CKBOOL pop )
{
    // empty stmt list
    if( !stmt )
        return TRUE;

    // return
    t_CKBOOL ret = TRUE;

    // loop over statements
    switch( stmt->s_type )
    {
        case ae_stmt_exp:  // expression statement
            // emit it
            ret = emit_engine_emit_exp( emit, stmt->stmt_exp );
            // need to pop the final value from stack
            if( ret && pop && stmt->stmt_exp->type->size > 0 )
            {
                // HACK!
                if( stmt->stmt_exp->type->size == 4 )
                    emit->append( new Chuck_Instr_Reg_Pop_Word );
                else if( stmt->stmt_exp->type->size == 8 )
                    emit->append( new Chuck_Instr_Reg_Pop_Word2 );
                else
                {
                    EM_error2( stmt->stmt_exp->linepos,
                               "(emit): internal error: %i byte stack item unhandled...",
                               stmt->stmt_exp->type->size );
                    return FALSE;
                }
            }
            break;

        case ae_stmt_if:  // if statement
            ret = emit_engine_emit_if( emit, &stmt->stmt_if );
            break;

        case ae_stmt_for:  // for statement
            ret = emit_engine_emit_for( emit, &stmt->stmt_for );
            break;

        case ae_stmt_while:  // while statement
            if( stmt->stmt_while.is_do )
                ret = emit_engine_emit_do_while( emit, &stmt->stmt_while );
            else
                ret = emit_engine_emit_while( emit, &stmt->stmt_while );
            break;
        
        case ae_stmt_until:  // until statement
            if( stmt->stmt_until.is_do )
                ret = emit_engine_emit_do_until( emit, &stmt->stmt_until );
            else
                ret = emit_engine_emit_until( emit, &stmt->stmt_until );
            break;

        case ae_stmt_switch:  // switch statement
            // not implemented
            ret = FALSE;
            break;

        case ae_stmt_break:  // break statement
            ret = emit_engine_emit_break( emit, &stmt->stmt_break );
            break;

        case ae_stmt_code:  // code segment
            ret = emit_engine_emit_code_segment( emit, &stmt->stmt_code );
            break;
            
        case ae_stmt_continue:  // continue statement
            ret = emit_engine_emit_continue( emit, &stmt->stmt_continue );
            break;
            
        case ae_stmt_return:  // return statement
            ret = emit_engine_emit_return( emit, &stmt->stmt_return );
            break;
            
        case ae_stmt_case:  // case statement
            // not implemented
            ret = FALSE;
            // ret = emit_engine_emit_case( emit, &stmt->stmt_case );
            break;
       
        case ae_stmt_gotolabel:  // goto label statement
            // not implemented
            ret = FALSE;
            // ret = emit_engine_emit_gotolabel( emit, &stmt->stmt_case );
            break;
        
        default:  // bad
            EM_error2( stmt->linepos, 
                 "(emit): internal error: unhandled statement type '%i'...",
                 stmt->s_type );
            break;
    }

    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_if()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_if( Chuck_Emitter * emit, a_Stmt_If stmt )
{
    t_CKBOOL ret = TRUE;
    Chuck_Instr_Branch_Op * op = NULL, * op2 = NULL;

    // push the stack, allowing for new local variables
    emit->push_scope();

    // emit the condition
    ret = emit_engine_emit_exp( emit, stmt->cond );
    if( !ret )
        return FALSE;

    // type of the condition
    switch( stmt->cond->type->id )
    {
    case te_int:
    case te_uint:
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        op = new Chuck_Instr_Branch_Eq_uint( 0 );
        break;
    case te_float:
    case te_dur:
    case te_time:
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm2( 0.0 ) );
        op = new Chuck_Instr_Branch_Eq_double( 0 );
        break;
        
    default:
        EM_error2( stmt->cond->linepos,
            "(emit): internal error: unhandled type '%s' in if condition",
            stmt->cond->type->name.c_str() );
        return FALSE;
    }

    if( !ret ) return FALSE;

    // append the op
    emit->append( op );

    // emit the body
    ret = emit_engine_emit_stmt( emit, stmt->if_body );
    if( !ret )
        return FALSE;

    // emit the skip to the end
    emit->append( op2 = new Chuck_Instr_Goto(0) );
    
    // set the op's target
    op->set( emit->next_index() );

    // emit the body
    ret = emit_engine_emit_stmt( emit, stmt->else_body );
    if( !ret )
        return FALSE;

    // pop stack
    emit->pop_scope();

    // set the op2's target
    op2->set( emit->next_index() );

    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_for()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_for( Chuck_Emitter * emit, a_Stmt_For stmt )
{
    t_CKBOOL ret = TRUE;
    Chuck_Instr_Branch_Op * op = NULL;

    // push the stack
    emit->push_scope();

    // emit the cond
    ret = emit_engine_emit_stmt( emit, stmt->c1 );
    if( !ret )
        return FALSE;

    // the start index
    t_CKUINT start_index = emit->next_index();
    // mark the stack of continue
    emit->stack_cont.push_back( NULL );
    // mark the stack of break
    emit->stack_break.push_back( NULL );

    // emit the cond - keep the result on the stack
    emit_engine_emit_stmt( emit, stmt->c2, FALSE );

    // could be NULL
    if( stmt->c2 )
    {
        switch( stmt->c2->stmt_exp->type->id )
        {
        case te_int:
        case te_uint:
            // push 0
            emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
            op = new Chuck_Instr_Branch_Eq_uint( 0 );
            break;
        case te_float:
        case te_dur:
        case te_time:
            // push 0
            emit->append( new Chuck_Instr_Reg_Push_Imm2( 0.0 ) );
            op = new Chuck_Instr_Branch_Eq_double( 0 );
            break;
        
        default:
            EM_error2( stmt->c2->stmt_exp->linepos,
                 "(emit): internal error: unhandled type '%s' in for conditional",
                 stmt->c2->stmt_exp->type->name.c_str() );
            return FALSE;
        }
        // append the op
        emit->append( op );
    }

    // emit the body
    ret = emit_engine_emit_stmt( emit, stmt->body );
    if( !ret )
        return FALSE;

    // emit the action
    if( stmt->c3 )
    {
        ret = emit_engine_emit_exp( emit, stmt->c3 );
        if( !ret )
            return FALSE;

        // HACK!
        if( stmt->c3->type->size == 8 )
            emit->append( new Chuck_Instr_Reg_Pop_Word2 );
        else if( stmt->c3->type->size == 4 )
            emit->append( new Chuck_Instr_Reg_Pop_Word );
        else if( stmt->c3->type->size != 0 )
        {
            EM_error2( stmt->c3->linepos,
                "(emit): internal error: non-void type size %i unhandled...",
                stmt->c3->type->size );
            return FALSE;
        }
    }

    // go back to do check the condition
    emit->append( new Chuck_Instr_Goto( start_index ) );

    // could be NULL
    if( stmt->c2 )
        // set the op's target
        op->set( emit->next_index() );

    // stack of continue
    while( emit->stack_cont.size() && emit->stack_cont.back() )
    {
        emit->stack_cont.back()->set( start_index );
        emit->stack_cont.pop_back();
    }

    // stack of break
    while( emit->stack_break.size() && emit->stack_break.back() )
    {
        emit->stack_break.back()->set( emit->next_index() );
        emit->stack_cont.pop_back();
    }

    // pop stack
    emit->pop_scope();

    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_while()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_while( Chuck_Emitter * emit, a_Stmt_While stmt )
{
    t_CKBOOL ret = TRUE;
    Chuck_Instr_Branch_Op * op = NULL;

    // push stack
    emit->push_scope();

    // get the index
    uint start_index = emit->next_index();
    // mark the stack of continue
    emit->stack_cont.push_back( NULL );
    // mark the stack of break
    emit->stack_break.push_back( NULL );

    // emit the cond
    ret = emit_engine_emit_exp( emit, stmt->cond );
    if( !ret )
        return FALSE;
    
    // the condition
    switch( stmt->cond->type->id )
    {
    case te_int:
    case te_uint:
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        op = new Chuck_Instr_Branch_Eq_uint( 0 );
        break;
    case te_float:
    case te_dur:
    case te_time:
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm2( 0.0 ) );
        op = new Chuck_Instr_Branch_Eq_double( 0 );
        break;
        
    default:
        EM_error2( stmt->cond->linepos,
            "(emit): internal error: unhandled type '%s' in while conditional",
            stmt->cond->type->name.c_str() );
        return FALSE;
    }
    
    // append the op
    emit->append( op );

    // emit the body
    ret = emit_engine_emit_stmt( emit, stmt->body );
    if( !ret )
        return FALSE;
    
    // go back to do check the condition
    emit->append( new Chuck_Instr_Goto( start_index ) );
    
    // set the op's target
    op->set( emit->next_index() );

    // stack of continue
    while( emit->stack_cont.size() && emit->stack_cont.back() )
    {
        emit->stack_cont.back()->set( start_index );
        emit->stack_cont.pop_back();
    }

    // stack of break
    while( emit->stack_break.size() && emit->stack_break.back() )
    {
        emit->stack_break.back()->set( emit->next_index() );
        emit->stack_cont.pop_back();
    }

    // pop stack
    emit->pop_scope();

    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_do_while()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_do_while( Chuck_Emitter * emit, a_Stmt_While stmt )
{
    t_CKBOOL ret = TRUE;
    Chuck_Instr_Branch_Op * op = NULL;
    uint start_index = emit->next_index();
    
    // push stack
    emit->push_scope();

    // mark the stack of continue
    emit->stack_cont.push_back( NULL );
    // mark the stack of break
    emit->stack_break.push_back( NULL );

    // emit the body
    ret = emit_engine_emit_stmt( emit, stmt->body );
    if( !ret )
        return FALSE;
    
    // emit the cond
    ret = emit_engine_emit_exp( emit, stmt->cond );
    if( !ret )
        return FALSE;
    
    // the condition
    switch( stmt->cond->type->id )
    {
    case te_int:
    case te_uint:
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        op = new Chuck_Instr_Branch_Neq_uint( 0 );
        break;
    case te_float:
    case te_dur:
    case te_time:
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm2( 0.0 ) );
        op = new Chuck_Instr_Branch_Neq_double( 0 );
        break;
        
    default:
        EM_error2( stmt->cond->linepos,
                   "(emit): internal error: unhandled type '%s' in do/while conditional",
                   stmt->cond->type->name );
        return FALSE;
    }
    
    // append the op
    emit->append( op );

    // set the op's target
    op->set( start_index );

    // stack of continue
    while( emit->stack_cont.size() && emit->stack_cont.back() )
    {
        emit->stack_cont.back()->set( start_index );
        emit->stack_cont.pop_back();
    }

    // stack of break
    while( emit->stack_break.size() && emit->stack_break.back() )
    {
        emit->stack_break.back()->set( emit->next_index() );
        emit->stack_cont.pop_back();
    }

    // pop stack
    emit->pop_scope();
    
    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_until()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_until( Chuck_Emitter * emit, a_Stmt_Until stmt )
{
    t_CKBOOL ret = TRUE;
    Chuck_Instr_Branch_Op * op = NULL;

    // push stack
    emit->push_scope();

    // get index
    uint start_index = emit->next_index();
    // mark the stack of continue
    emit->stack_cont.push_back( NULL );
    // mark the stack of break
    emit->stack_break.push_back( NULL );

    // emit the cond
    ret = emit_engine_emit_exp( emit, stmt->cond );
    if( !ret )
        return FALSE;

    // condition
    switch( stmt->cond->type->id )
    {
    case te_int:
    case te_uint:
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        op = new Chuck_Instr_Branch_Neq_uint( 0 );
        break;
    case te_float:
    case te_dur:
    case te_time:
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm2( 0.0 ) );
        op = new Chuck_Instr_Branch_Neq_double( 0 );
        break;
        
    default:
        EM_error2( stmt->cond->linepos,
             "(emit): internal error: unhandled type '%s' in until conditional",
             stmt->cond->type->name.c_str() );
        return FALSE;
    }
    
    // append the op
    emit->append( op );

    // emit the body
    emit_engine_emit_stmt( emit, stmt->body );
    
    // go back to do check the condition
    emit->append( new Chuck_Instr_Goto( start_index ) );
    
    // set the op's target
    op->set( emit->next_index() );

    // stack of continue
    while( emit->stack_cont.size() && emit->stack_cont.back() )
    {
        emit->stack_cont.back()->set( start_index );
        emit->stack_cont.pop_back();
    }

    // stack of break
    while( emit->stack_break.size() && emit->stack_break.back() )
    {
        emit->stack_break.back()->set( emit->next_index() );
        emit->stack_cont.pop_back();
    }

    // pop stack
    emit->pop_scope();
    
    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_do_until()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_do_until( Chuck_Emitter * emit, a_Stmt_Until stmt )
{
    t_CKBOOL ret = TRUE;
    Chuck_Instr_Branch_Op * op = NULL;

    // push stack
    emit->push_scope();

    // the index
    uint start_index = emit->next_index();
    // mark the stack of continue
    emit->stack_cont.push_back( NULL );
    // mark the stack of break
    emit->stack_break.push_back( NULL );

    // emit the body
    ret = emit_engine_emit_stmt( emit, stmt->body );
    if( !ret )
        return FALSE;

    // emit the cond
    ret = emit_engine_emit_exp( emit, stmt->cond );
    if( !ret )
        return FALSE;

    // condition
    switch( stmt->cond->type->id )
    {
    case te_int:
    case te_uint:
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        op = new Chuck_Instr_Branch_Eq_uint( 0 );
        break;
    case te_float:
    case te_dur:
    case te_time:
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm2( 0.0 ) );
        op = new Chuck_Instr_Branch_Eq_double( 0 );
        break;

    default:
        EM_error2( stmt->cond->linepos,
             "(emit): internal error: unhandled type '%s' in do/until conditional",
             stmt->cond->type->name.c_str() );
        return FALSE;
    }

    // append the op
    emit->append( op );

    // set the op's target
    op->set( start_index );

    // stack of continue
    while( emit->stack_cont.size() && emit->stack_cont.back() )
    {
        emit->stack_cont.back()->set( start_index );
        emit->stack_cont.pop_back();
    }

    // stack of break
    while( emit->stack_break.size() && emit->stack_break.back() )
    {
        emit->stack_break.back()->set( emit->next_index() );
        emit->stack_cont.pop_back();
    }

    // pop stack
    emit->pop_scope();
    
    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_break()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_break( Chuck_Emitter * emit, a_Stmt_Break br )
{
    // append
    emit->stack_break.push_back( new Chuck_Instr_Goto( 0 ) );
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_continue()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_continue( Chuck_Emitter * emit, a_Stmt_Continue cont )
{
    // append
    emit->stack_cont.push_back( new Chuck_Instr_Goto( 0 ) );

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_return()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_return( Chuck_Emitter * emit, a_Stmt_Return stmt )
{
    if( !emit_engine_emit_exp( emit, stmt->val ) )
        return FALSE;

    // determine where later
    Chuck_Instr_Goto * instr = new Chuck_Instr_Goto( 0 );
    emit->append( instr );
//    emit->returns.push_back( instr );

    return TRUE;
}




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_switch( Chuck_Emitter * emit, a_Stmt_Switch stmt );




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp( Chuck_Emitter * emit, a_Exp exp )
{
    // for now...
    assert( exp->next == NULL );

    // loop over 
    while( exp )
    {
        switch( exp->s_type )
        {
        case ae_exp_binary:
            if( !emit_engine_emit_exp_binary( emit, &exp->binary ) )
                return FALSE;
            break;

        case ae_exp_unary:
            if( !emit_engine_emit_exp_unary( emit, &exp->unary ) )
                return FALSE;
            break;

        case ae_exp_cast:
            if( !emit_engine_emit_exp_cast( emit, &exp->cast ) )
                return FALSE;
            break;

        case ae_exp_postfix:
            if( !emit_engine_emit_exp_postfix( emit, &exp->postfix ) )
                return FALSE;
            break;

        case ae_exp_dur:
            if( !emit_engine_emit_exp_dur( emit, &exp->dur ) )
                return FALSE;
            break;

        case ae_exp_primary:
            if( !emit_engine_emit_exp_primary( emit, &exp->primary ) )
                return FALSE;
            break;

        case ae_exp_array:
            if( !emit_engine_emit_exp_array( emit, &exp->array ) )
                return FALSE;
            break;

        case ae_exp_func_call:
            if( !emit_engine_emit_exp_func_call( emit, &exp->func_call ) )
                return FALSE;
            break;

        case ae_exp_dot_member:
            if( !emit_engine_emit_exp_dot_member( emit, &exp->dot_member ) )
                return FALSE;
            break;

        case ae_exp_if:
            if( !emit_engine_emit_exp_if( emit, &exp->exp_if ) )
                return FALSE;
            break;

        case ae_exp_decl:
            if( !emit_engine_emit_exp_decl( emit, &exp->decl ) )
                return FALSE;
            break;

        //case ae_exp_namespace:
        //    if( !emit_engine_emit_exp_namespace( emit, &exp->name_space ) )
        //        return FALSE;
        //    break;

        default:
            EM_error2( exp->linepos, 
                 "(emit): internal error: unhandled expression type '%i'...",
                 exp->s_type );
            return FALSE;
        }

        exp = exp->next;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_binary()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_binary( Chuck_Emitter * emit, a_Exp_Binary binary )
{
    t_CKBOOL left = FALSE;
    t_CKBOOL right = FALSE;

    // emit
    left = emit_engine_emit_exp( emit, exp->lhs );
    right = emit_engine_emit_exp( emit, exp->rhs );

    // check
    if( !left || !right )
        return FALSE;

    // emit the op
    if( !emit_engine_emit_op( emit, exp->op, exp->lhs, exp->rhs ) )
        return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_op( Chuck_Emitter * emit, ae_Operator op, a_Exp lhs, a_Exp rhs );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_op_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_op_unchuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_unary( Chuck_Emitter * emit, a_Exp_Unary unary );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_primary( Chuck_Emitter * emit, a_Exp_Primary exp );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_cast( Chuck_Emitter * emit, a_Exp_Cast cast );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_postfix( Chuck_Emitter * emit, a_Exp_Postfix postfix );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_dur( Chuck_Emitter * emit, a_Exp_Dur dur );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_array( Chuck_Emitter * emit, a_Exp_Array array );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_func_call( Chuck_Emitter * emit, a_Exp_Func_Call func_call );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_dot_member( Chuck_Emitter * emit, a_Exp_Dot_Member member );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_if( Chuck_Emitter * emit, a_Exp_If exp_if );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_decl( Chuck_Emitter * emit, a_Exp_Decl decl );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_namespace( Chuck_Emitter * emit, a_Exp_Namespace name_space );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_code_segment( Chuck_Emitter * emit, a_Stmt_Code stmt, t_CKBOOL push );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_func_def( Chuck_Emitter * emit, a_Func_Def func_def );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_class_def( Chuck_Emitter * emit, a_Class_Def class_def );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_spork( Chuck_Emitter * emit, a_Exp_Func_Call exp );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_symbol( Chuck_Emitter * emit, S_Symbol symbol, 
                                  t_CKBOOL offset, int linepos );




//-----------------------------------------------------------------------------
// name: pop_scope()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Emitter::pop_scope( )
{
    // sanity
    assert( code != NULL );

    // clear locals
    locals.clear();

    // get locals
    code->frame->pop_scope( locals );

    // TODO: free locals
}