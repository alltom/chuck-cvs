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
// desc: chuck instruction emitter
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
t_CKBOOL emit_engine_emit_op_at_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );
t_CKBOOL emit_engine_emit_exp_unary( Chuck_Emitter * emit, a_Exp_Unary unary );
t_CKBOOL emit_engine_emit_exp_primary( Chuck_Emitter * emit, a_Exp_Primary exp );
t_CKBOOL emit_engine_emit_exp_cast( Chuck_Emitter * emit, a_Exp_Cast cast );
t_CKBOOL emit_engine_emit_exp_postfix( Chuck_Emitter * emit, a_Exp_Postfix postfix );
t_CKBOOL emit_engine_emit_exp_dur( Chuck_Emitter * emit, a_Exp_Dur dur );
t_CKBOOL emit_engine_emit_exp_array( Chuck_Emitter * emit, a_Exp_Array array );
t_CKBOOL emit_engine_emit_exp_func_call( Chuck_Emitter * emit, a_Exp_Func_Call func_call,
                                         t_CKBOOL spork = FALSE );
t_CKBOOL emit_engine_emit_exp_dot_member( Chuck_Emitter * emit, a_Exp_Dot_Member member );
t_CKBOOL emit_engine_emit_exp_if( Chuck_Emitter * emit, a_Exp_If exp_if );
t_CKBOOL emit_engine_emit_exp_decl( Chuck_Emitter * emit, a_Exp_Decl decl );
t_CKBOOL emit_engine_emit_exp_namespace( Chuck_Emitter * emit, a_Exp_Namespace name_space );
t_CKBOOL emit_engine_emit_code_segment( Chuck_Emitter * emit, a_Stmt_Code stmt,
                                        t_CKBOOL push = TRUE );
t_CKBOOL emit_engine_emit_func_def( Chuck_Emitter * emit, a_Func_Def func_def );
t_CKBOOL emit_engine_emit_class_def( Chuck_Emitter * emit, a_Class_Def class_def );
t_CKBOOL emit_engine_emit_spork( Chuck_Emitter * emit, a_Exp_Func_Call exp );
t_CKBOOL emit_engine_emit_cast( Chuck_Emitter * emit, Chuck_Type * to, Chuck_Type * from );
t_CKBOOL emit_engine_emit_symbol( Chuck_Emitter * emit, S_Symbol symbol, 
                                  t_CKBOOL offset, int linepos );




//-----------------------------------------------------------------------------
// name: emit_engine_init()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Emitter * emit_engine_init( Chuck_Env * env )
{
    // TODO: ensure this in a better way
    assert( sizeof(t_CKUINT) == sizeof(void *) );

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
    assert( emit->env->context->nspc.code == NULL );

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

        // the next
        prog = prog->next;
    }

    if( ret )
    {
        // append end of code
        emit->append( new Chuck_Instr_EOC );

        // converted to virtual machine code
        emit->context->nspc.code = emit_to_code( emit, TRUE );
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
                // sanity
                assert( stmt->stmt_exp->cast_to == NULL );

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
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        op = new Chuck_Instr_Branch_Eq_int( 0 );
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
            // push 0
            emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
            op = new Chuck_Instr_Branch_Eq_int( 0 );
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
    t_CKUINT start_index = emit->next_index();
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
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        op = new Chuck_Instr_Branch_Eq_int( 0 );
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
    t_CKUINT start_index = emit->next_index();
    
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
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        op = new Chuck_Instr_Branch_Neq_int( 0 );
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
                   stmt->cond->type->c_name() );
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
    t_CKUINT start_index = emit->next_index();
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
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        op = new Chuck_Instr_Branch_Neq_int( 0 );
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
    t_CKUINT start_index = emit->next_index();
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
        // push 0
        emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        op = new Chuck_Instr_Branch_Eq_int( 0 );
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

        // implicit cast
        if( exp->cast_to != NULL )
            if( !emit_engine_emit_cast( emit, exp->cast_to, exp->type ) )
                return FALSE;

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
    // sanity
    assert( binary->self->emit_var == FALSE );

    t_CKBOOL left = FALSE;
    t_CKBOOL right = FALSE;

    // emit
    left = emit_engine_emit_exp( emit, binary->lhs );
    right = emit_engine_emit_exp( emit, binary->rhs );

    // check
    if( !left || !right )
        return FALSE;

    // emit the op
    if( !emit_engine_emit_op( emit, binary->op, binary->lhs, binary->rhs ) )
        return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_op()
// desc: emit binary operator
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_op( Chuck_Emitter * emit, ae_Operator op, a_Exp lhs, a_Exp rhs )
{
    // any implicit cast happens before this
    Chuck_Type * t_left = lhs->cast_to ? lhs->cast_to : lhs->type;
    Chuck_Type * t_right = rhs->cast_to ? rhs->cast_to : rhs->type;
    // get the types of the left and right
    te_Type left = t_left->id;
    te_Type right = t_right->id;
    // op
    Chuck_Instr * instr = NULL;

    // emit op
    switch( op )
    {    
    // ----------------------------- num --------------------------------------
    case ae_op_plus:
        // time + dur
        if( ( left == te_dur && right == te_time ) ||
            ( left == te_time && right == te_dur ) )
        {
            emit->append( instr = new Chuck_Instr_Add_double );
        }
        else // other types
        {
            switch( left )
            {
            case te_int:
                emit->append( instr = new Chuck_Instr_Add_int );
                break;
            case te_float:
            case te_dur:
                emit->append( instr = new Chuck_Instr_Add_double );
                break;
            }
        }
        break;
    
    case ae_op_plus_chuck:
        // time + dur
        if( ( left == te_dur && right == te_time ) ||
            ( left == te_time && right == te_dur ) )
        {
            emit->append( instr = new Chuck_Instr_Add_double_Assign );
        }
        else // other types
        {
            switch( left )
            {
            case te_int:
                emit->append( instr = new Chuck_Instr_Add_int_Assign );
                break;
            case te_float:
            case te_dur:
                emit->append( instr = new Chuck_Instr_Add_double_Assign );
                break;
            }
        }
        break;

    case ae_op_minus:
        if( ( left == te_time && right == te_dur ) ) // time - dur = time
            emit->append( instr = new Chuck_Instr_Minus_double );
        else if( ( left == te_time && right == te_time ) ) // time - time = dur
            emit->append( instr = new Chuck_Instr_Minus_double );
        else // other types
        {
            switch( left )
            {
            case te_int:
                emit->append( instr = new Chuck_Instr_Minus_int );
                break;
            case te_float:
            case te_dur:
                emit->append( instr = new Chuck_Instr_Minus_double );
                break;
            }
        }
        break;
    
    case ae_op_minus_chuck:
        if( ( left == te_dur && right == te_time ) ) // time - dur = time
            emit->append( instr = new Chuck_Instr_Minus_double_Assign );
        else if( ( left == te_time && right == te_time ) ) // time - time = dur
            emit->append( instr = new Chuck_Instr_Minus_double_Assign );
        else // other types
        {
            switch( left )
            {
            case te_int:
                emit->append( instr = new Chuck_Instr_Minus_int_Assign );
                break;
            case te_float:
            case te_dur:
                emit->append( instr = new Chuck_Instr_Minus_double_Assign );
                break;
            }
        }
        break;

    case ae_op_times:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Times_int );
            break;
        case te_float:
        case te_dur:
            emit->append( instr = new Chuck_Instr_Times_double );
            break;
        }
        break;
    
    case ae_op_times_chuck:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Times_int_Assign );
            break;
        case te_float:
            emit->append( instr = new Chuck_Instr_Times_double_Assign );
            break;
        }
        break;

    case ae_op_divide:
        if( ( left == te_time && right == te_dur ) ) // time / dur = float
            emit->append( instr = new Chuck_Instr_Divide_double );
        else // other types
        {
            switch( left )
            {
            case te_int:
                emit->append( instr = new Chuck_Instr_Divide_int );
                break;
            case te_float:
            case te_dur:
                emit->append( instr = new Chuck_Instr_Divide_double );
                break;
            }
        }
        break;
    
    case ae_op_divide_chuck:
        // reverse
        if( ( left == te_dur && right == te_time ) ) // time / dur = float
            emit->append( instr = new Chuck_Instr_Divide_double_Assign );
        else // other types
        {
            switch( left )
            {
            case te_int:
                emit->append( instr = new Chuck_Instr_Divide_int_Assign );
                break;
            case te_float:
                emit->append( instr = new Chuck_Instr_Divide_double_Assign );
                break;
            }
        }
        break;

    case ae_op_s_or:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Binary_Or );
            break;
        }
        break;
    
    case ae_op_s_or_chuck:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Binary_Or_Assign );
            break;
        }
        break;

    case ae_op_s_and:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Binary_And );
            break;
        }
        break;

    case ae_op_s_and_chuck:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Binary_And_Assign );
            break;
        }
        break;

    case ae_op_shift_left:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Binary_Shift_Left );
            break;
        }
        break;

    case ae_op_shift_left_chuck:
        // reverse
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Binary_Shift_Left_Assign );
            break;
        }
        break;
    
    case ae_op_shift_right:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Binary_Shift_Right );
            break;
        }
        break;

    case ae_op_shift_right_chuck:
        // reverse
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Binary_Shift_Right_Assign );
            break;
        }
        break;

    case ae_op_percent:
        if( ( left == te_time && right == te_dur ) ) // time % dur = dur
        {
            emit->append( instr = new Chuck_Instr_Divide_double );
        }
        else // other types
        {
            switch( left )
            {
            case te_int:
                emit->append( instr = new Chuck_Instr_Mod_int );
                break;
            case te_float:
            case te_dur:
                emit->append( instr = new Chuck_Instr_Mod_double );
                break;
            }
        }
        break;

    case ae_op_percent_chuck:
        // reverse
        if( ( left == te_dur && right == te_time ) ) // time % dur = dur
        {
            emit->append( instr = new Chuck_Instr_Divide_double_Reverse );
        }
        else // other types
        {
            switch( left )
            {
            case te_int:
                emit->append( instr = new Chuck_Instr_Mod_int_Reverse );
                break;
            case te_float:
            case te_dur:
                emit->append( instr = new Chuck_Instr_Mod_double_Reverse );
                break;
            }
        }
        break;

    case ae_op_s_xor:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Binary_Xor );
            break;
        }
        break;
    
    case ae_op_s_xor_chuck:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Binary_Xor_Assign );
            break;
        }
        break;

    // ----------------------------- chuck -------------------------------------
    case ae_op_chuck:
    {
        a_Exp cl = lhs, cr = rhs;

        // TODO: cross chuck
        assert( cl->next == NULL && cr->next == NULL );
        while( cr )
        {
            cl = lhs;
            while( cl )
            {
                if( !emit_engine_emit_op_chuck( emit, cl, cr ) )
                    return FALSE;
                cl = cl->next;
            }
            
            cr = cr->next;
        }

        return TRUE;
    }
    
    case ae_op_unchuck:
    {
        a_Exp cl = lhs, cr = rhs;
        
        // TODO: cross chuck
        assert( cl->next == NULL && cr->next == NULL );
        while( cr )
        {
            cl = lhs;
            while( cl )
            {
                if( !emit_engine_emit_op_unchuck( emit, cl, cr ) )
                    return FALSE;
                cl = cl->next;
            }
            
            cr = cr->next;
        }

        return TRUE;
    }
    
    case ae_op_at_chuck:
    {
        a_Exp cl = lhs, cr = rhs;
        
        // TODO: cross chuck
        assert( cl->next == NULL && cr->next == NULL );
        while( cr )
        {
            cl = lhs;
            while( cl )
            {
                if( !emit_engine_emit_op_at_chuck( emit, cl, cr ) )
                    return FALSE;
                cl = cl->next;
            }
            
            cr = cr->next;
        }

        return TRUE;
    }
    
    case ae_op_s_chuck:
    break;

    // -------------------------------- bool -----------------------------------        
    case ae_op_eq:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Eq_int );
            break;
        case te_float:
        case te_dur:
        case te_time:
            emit->append( instr = new Chuck_Instr_Eq_double );
            break;
        }
        break;
    
    case ae_op_neq:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Neq_int );
            break;
        case te_float:
        case te_dur:
        case te_time:
            emit->append( instr = new Chuck_Instr_Neq_double );
            break;
        }
        break;
    
    case ae_op_lt:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Lt_int );
            break;
        case te_float:
        case te_dur:
        case te_time:
            emit->append( instr = new Chuck_Instr_Lt_double );
            break;
        }
        break;
    
    case ae_op_le:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Le_int );
            break;
        case te_float:
        case te_dur:
        case te_time:
            emit->append( instr = new Chuck_Instr_Le_double );
            break;
        }
        break;
    
    case ae_op_gt:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Gt_int );
            break;
        case te_float:
        case te_dur:
        case te_time:
            emit->append( instr = new Chuck_Instr_Gt_double );
            break;
        }
        break;
    
    case ae_op_ge:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Ge_int );
            break;
        case te_float:
        case te_dur:
        case te_time:
            emit->append( instr = new Chuck_Instr_Ge_double );
            break;
        }
        break;
    
    case ae_op_and:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_And );
            break;
        }
        break;
    
    case ae_op_or:
        switch( left )
        {
        case te_int:
            emit->append( instr = new Chuck_Instr_Or );
            break;
        }
        break;

    default:
        EM_error2( lhs->linepos,
            "(emit): internal error: unhandled op '%s' %s '%s'",
            t_left->c_name(), op2str( op ), t_right->c_name() );
        return FALSE;
    }
    
    // make sure emit
    if( !instr )
    {
        EM_error2( lhs->linepos,
            "(emit): internal error: unhandled op '%s' %s '%s'",
            t_left->c_name(), op2str( op ), t_right->c_name() );
        return FALSE;
    }
        
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_op_chuck()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_op_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs )
{
    // any implicit cast happens before this
    Chuck_Type * left = lhs->cast_to ? lhs->cast_to : lhs->type;
    Chuck_Type * right = rhs->cast_to ? rhs->cast_to : rhs->type;
    
    // ugen => ugen
    if( isa( left, &t_ugen ) && isa( right, &t_ugen ) )
    {
        // connect the ugens
        emit->append( new Chuck_Instr_UGen_Link );
        // done
        return TRUE;
    }

    // time advance
    if( isa( left, &t_dur ) && isa( right, &t_time ) && rhs->s_meta == ae_meta_var )
    {
        // add the two
        emit->append( new Chuck_Instr_Add_double );

        // see if rhs is 'now'
        if( rhs->s_type == ae_exp_primary && !strcmp( "now", S_name(rhs->primary.var) ) )
        {
            // advance time
            emit->append( new Chuck_Instr_Time_Advance );
        }

        return TRUE;
    }

    // assignment or something else
    if( isa( left, right ) )
    {
        // basic types?
        if( type_engine_check_primitive( left ) || isa( left, &t_string ) )
        {
            // use at assign
            return emit_engine_emit_op_at_chuck( emit, lhs, rhs );
        }
    }

    // TODO: check overloading of =>
    // TODO: deal with const

    // no match
    EM_error2( lhs->linepos,
        "(emit): internal error: unhandled '=>' on types '%s' and '%s'...",
        left->c_name(), right->c_name() );

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_op_unchuck()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_op_unchuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs )
{
    // any implicit cast happens before this
    Chuck_Type * left = lhs->cast_to ? lhs->cast_to : lhs->type;
    Chuck_Type * right = rhs->cast_to ? rhs->cast_to : rhs->type;
    
    // if ugen
    if( isa( left, &t_ugen ) && isa( right, &t_ugen ) )
    {
        // no connect
        emit->append( new Chuck_Instr_UGen_UnLink );
    }
    else
    {
        EM_error2( lhs->linepos,
            "(emit): internal error: unhandled '=<' on types '%s' and '%s'",
            left->c_name(), right->c_name() );
        return FALSE;
    }
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_op_at_chuck()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_op_at_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs )
{
    // any implicit cast happens before this
    Chuck_Type * left = lhs->cast_to ? lhs->cast_to : lhs->type;
    Chuck_Type * right = rhs->cast_to ? rhs->cast_to : rhs->type;
    
    // assignment or something else
    if( isa( left, right ) )
    {
        // basic types?
        if( type_engine_check_primitive( left ) || isa( left, &t_string ) )
        {
            // assigment?
            if( rhs->s_meta != ae_meta_var )
            {
                EM_error2( lhs->linepos,
                    "(emit): internal error: assignment to non-variable..." );
                return FALSE;
            }

            // see if rhs is 'now' - time => now
            if( rhs->s_type == ae_exp_primary && !strcmp( "now", S_name(rhs->primary.var) ) )
            {
                // pop the now addr
                emit->append( new Chuck_Instr_Reg_Pop_Word2 );
                // advance time
                emit->append( new Chuck_Instr_Time_Advance );
            }
            else
            {
                // assign primitive
                emit->append( new Chuck_Instr_Assign_Primitive );
            }

            return TRUE;
        }
        else // objects
        {
            // assign object
            emit->append( new Chuck_Instr_Assign_Object );

            return TRUE;
        }
    }

    // TODO: check overloading of =>
    // TODO: deal with const

    // no match
    EM_error2( lhs->linepos,
        "(emit): internal error: unhandled '@=>' on types '%s' and '%s'...",
        left->c_name(), right->c_name() );

    return FALSE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_unary()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_unary( Chuck_Emitter * emit, a_Exp_Unary unary )
{
    if( unary->op != ae_op_spork && !emit_engine_emit_exp( emit, unary->exp ) )
        return FALSE;

    // emit the operator
    switch( unary->op )
    {
    case ae_op_plusplus:
        // make sure assignment is legal
        if( unary->self->s_meta != ae_meta_var )  // TODO: const
        {
            EM_error2( unary->self->linepos,
                "(emit): target for '++' not mutable..." );
            return FALSE;
        }

        // increment
        if( equals( unary->exp->type, &t_int ) )
            emit->append( new Chuck_Instr_Inc_int );
        else
        {
            EM_error2( unary->linepos, 
                "(emit): internal error: unhandled type '%s' for pre '++'' operator",
                unary->exp->type->c_name() );
            return FALSE;
        }
        break;

    case ae_op_minusminus:
        // make sure assignment is legal
        if( unary->self->s_meta != ae_meta_var )  // TODO: const
        {
            EM_error2( unary->self->linepos,
                "(emit): target for '--' not mutable..." );
            return FALSE;
        }

        // decrement
        if( equals( unary->exp->type, &t_int ) )
            emit->append( new Chuck_Instr_Dec_int );
        else
        {
            EM_error2( unary->linepos, 
                "(emit): internal error: unhandled type '%s' for pre '--' operator",
                unary->exp->type->c_name() );
            return FALSE;
        }
        break;

    case ae_op_tilda:
        // complement
        if( equals( unary->exp->type, &t_int ) )
            emit->append( new Chuck_Instr_Complement_int );
        else
        {
            EM_error2( unary->linepos, 
                "(emit): internal error: unhandled type '%s' for '~' operator",
                unary->exp->type->c_name() );
            return FALSE;
        }
        break;

    case ae_op_exclamation:
        // !
        if( equals( unary->exp->type, &t_int ) )
            emit->append( new Chuck_Instr_Not_int );
        else
        {
            EM_error2( unary->linepos, 
                "(emit): internal error: unhandled type '%s' for '!' operator",
                unary->exp->type->c_name() );
            return FALSE;
        }
        break;
        
    case ae_op_minus:
        // negate
        if( equals( unary->exp->type, &t_int ) )
            emit->append( new Chuck_Instr_Negate_int );
        else if( equals( unary->exp->type, &t_float ) )
            emit->append( new Chuck_Instr_Negate_double );
        else
        {
            EM_error2( unary->linepos, 
                "(emit): internal error: unhandled type '%s' for unary '-' operator",
                unary->exp->type->c_name() );
            return FALSE;
        }
        break;

    case ae_op_spork:
        // spork ~ func()
        if( unary->exp->s_type == ae_exp_func_call )
        {
            if( !emit_engine_emit_spork( emit, &unary->exp->func_call ) )
                return FALSE;
        }
        else
        {
            EM_error2( unary->linepos,
                "(emit): internal error: sporking non-function call..." );
            return FALSE;
        }
        break;

    default:
        EM_error2( unary->linepos, 
            "(emit): internal error: unhandled unary op '%s",
            op2str( unary->op ) );
        return FALSE;
    }
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_primary()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_primary( Chuck_Emitter * emit, a_Exp_Primary exp )
{
    t_CKUINT temp;
    t_CKDUR dur;
    Chuck_Instr_Unary_Op * op = NULL, * op2 = NULL;

    // find out exp
    switch( exp->s_type )
    {
    case ae_primary_var:
        if( exp->var == insert_symbol( "now" ) )
        {
            emit->append( new Chuck_Instr_Reg_Push_Now );
        }
        else if( exp->var == insert_symbol( "dac" ) )
        {
            emit->append( new Chuck_Instr_DAC );
        }
        else if( exp->var == insert_symbol( "adc" ) )
        {
            emit->append( new Chuck_Instr_ADC );
        }
        else if( exp->var == insert_symbol( "bunghole" ) )
        {
            emit->append( new Chuck_Instr_Bunghole );
        }
        else if( exp->var == insert_symbol( "blackhole" ) )
        {
            emit->append( new Chuck_Instr_Bunghole );
        }
        else if( exp->var == insert_symbol( "true" ) )
        {
            emit->append( new Chuck_Instr_Reg_Push_Imm( 1 ) );
        }
        else if( exp->var == insert_symbol( "false" ) )
        {
            emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        }
        else if( exp->var == insert_symbol("maybe") )
        {
            emit->append( new Chuck_Instr_Reg_Push_Maybe() );
        }
        else if( exp->var == insert_symbol( "pi" ) )
        {
            double pi = 3.14159265358979323846;
            emit->append( new Chuck_Instr_Reg_Push_Imm2( pi ) );
        }
        else if( emit->find_dur( S_name(exp->var), &dur ) )
        {
            emit->append( new Chuck_Instr_Reg_Push_Imm2( dur ) );
        }
        else
        {
            // emit the symbol
            return emit_engine_emit_symbol( emit, exp->var, TRUE, exp->linepos );
        }
        break;
    
    case ae_primary_num:
        memcpy( &temp, &exp->num, sizeof(temp) );
        emit->append( new Chuck_Instr_Reg_Push_Imm( temp ) );
        break;
        
    case ae_primary_float:
        emit->append( new Chuck_Instr_Reg_Push_Imm2( exp->fnum ) );
        break;
        
    case ae_primary_str:
        // TODO: fix this
        temp = (t_CKUINT)exp->str;
        emit->append( new Chuck_Instr_Reg_Push_Imm( temp ) );
        break;
        
    case ae_primary_exp:
        if( !emit_engine_emit_exp( emit, exp->exp ) )
            return FALSE;
        break;
    }
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_cast()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_cast( Chuck_Emitter * emit, a_Exp_Cast cast )
{
    Chuck_Type * to = cast->self->type;
    Chuck_Type * from = cast->exp->type;

    // the actual work to be done
    return emit_engine_emit_cast( emit, to, from );
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_cast()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_cast( Chuck_Emitter * emit, 
                                Chuck_Type * to, Chuck_Type * from )
{
    // if type is already the same
    if( equals( to, from ) )
        return TRUE;

    // int to float
    if( equals( to, &t_int ) && equals( from, &t_float ) )
        emit->append( new Chuck_Instr_Cast_double2int );
    // float to int
    else if( equals( to, &t_float ) && equals( from, &t_int ) )
        emit->append( new Chuck_Instr_Cast_int2double );
    // up cast - do nothing
    else if( !isa( to, from ) && !isa( from, to ) )
    {
        EM_error2( 0, "(emit): internal error: cannot cast type '%s' to '%s'",
             from->c_name(), to->c_name() );
        return FALSE;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_postfix()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_postfix( Chuck_Emitter * emit, a_Exp_Postfix postfix )
{
    // emit the exp
    if( !emit_engine_emit_exp( emit, postfix->exp ) )
        return FALSE;

    // emit
    switch( postfix->op )
    {
    case ae_op_plusplus:
        if( equals( postfix->exp->type, &t_int ) )
            emit->append( new Chuck_Instr_Inc_int );
        else
        {
            EM_error2( postfix->linepos,
                "(emit): internal error: unhandled type '%s' for post '++' operator",
                postfix->exp->type->c_name() );
            return FALSE;
        }
    break;

    case ae_op_minusminus:
        if( equals( postfix->exp->type, &t_int ) )
            emit->append( new Chuck_Instr_Dec_int );
        else
        {
            EM_error2( postfix->linepos,
                "(emit): internal error: unhandled type '%s' for post '--' operator",
                postfix->exp->type->c_name() );
            return FALSE;
        }
    break;

    default:
        EM_error2( postfix->linepos,
            "(emit): internal error: unhandled postfix operator '%s'",
            op2str( postfix->op ) );
        return FALSE;
    }        
     
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_dur()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_dur( Chuck_Emitter * emit, a_Exp_Dur dur )
{
    // emit base
    if( !emit_engine_emit_exp( emit, dur->base ) )
        return FALSE;

    // cast
    if( equals( dur->base->type, &t_int ) )
        emit->append( new Chuck_Instr_Cast_int2double );

    // emit unit
    if( !emit_engine_emit_exp( emit, dur->unit ) )
        return FALSE;
        
    // multiply
    emit->append( new Chuck_Instr_Times_double );
        
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_array()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_array( Chuck_Emitter * emit, a_Exp_Array array )
{
    Chuck_Type * type = NULL;
    t_CKUINT depth = 0;
    a_Array_Sub sub = NULL;
    a_Exp exp = NULL;

    // get the type
    type = array->self->type;
    // get the dimension
    depth = type->array_depth;
    // make sure
    if( depth == 0 )
    {
        EM_error2( array->linepos,
            "(emit): internal error: array with 0 depth..." );
        return FALSE;
    }
    // get the sub
    sub = array->indices;
    if( !sub )
    {
        EM_error2( array->linepos,
            "(emit): internal error: NULL array sub..." );
        return FALSE;
    }
    // get the exp list
    exp = sub->exp_list;
    if( !exp )
    {
        EM_error2( array->linepos,
            "(emit): internal error: NULL array exp..." );
        return FALSE;
    }

    // emit the base (it should be a pointer to array)
    if( !emit_engine_emit_exp( emit, array->base ) )
        return FALSE;

    // loop over the exp
    while( exp )
    {
        // emit the exp
        if( !emit_engine_emit_exp( emit, exp ) )
            return FALSE;
        
        // move to the next
        exp = exp->next;
    }

    // emit the array access
    emit->append( new Chuck_Instr_Array_Access );

    // TODO: variable?

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_func_call()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_func_call( Chuck_Emitter * emit,
                                         a_Exp_Func_Call func_call,
                                         t_CKBOOL spork )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_dot_member()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_dot_member( Chuck_Emitter * emit,
                                          a_Exp_Dot_Member member )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_if()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_if( Chuck_Emitter * emit, a_Exp_If exp_if )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_decl()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_decl( Chuck_Emitter * emit, a_Exp_Decl decl )
{
    a_Var_Decl_List list = decl->var_decl_list;
    a_Var_Decl var_decl = NULL;
    Chuck_Value * value = NULL;
    Chuck_Type * type = NULL;
    Chuck_Local * local = NULL;
    t_CKUINT size = 0;
    t_CKBOOL is_ref = FALSE;
    t_CKBOOL do_alloc = FALSE;

    // loop through vars
    while( list )
    {
        // the var
        var_decl = list->var_decl;
        // get the value determined in type checker
        value = var_decl->value;
        // get the type of the value
        type = value->type;
        // is the variable a reference
        is_ref = isobj( type );
        // do alloc or not
        do_alloc = !decl->type->ref;

        // allocate a place on the local stack
        local = emit->alloc_local( type->size, value->name, is_ref );
        if( !local )
        {
            EM_error2( decl->linepos,
                "(emit): internal error: cannot allocate local '%s'...",
                value->name.c_str() );
            return FALSE;
        }

        // zero out location in memory, and leave offset on operand stack
        if( type->size == 4 )
            emit->append( new Chuck_Instr_Alloc_Word( local->offset ) );
        else if( type->size == 8 )
            emit->append( new Chuck_Instr_Alloc_DWord( local->offset ) );
        else
        {
            EM_error2( decl->linepos,
                "(emit): unhandle decl size of '%i'...",
                type->size );
            return FALSE;
        }

        // if this is an object
        if( is_ref )
        {
            // if array
            if( type->array_depth )
            {
                EM_error2( decl->linepos,
                    "(emit): internal error: array not impl" );
                    return FALSE;

                // emit 
            }
            else // not array
            {
                // if ugen
                if( isa( type, &t_ugen ) )
                {
                    // get the ugen info
                    Chuck_UGen_Info * info = decl->self->type->ugen;
                    if( !info )
                    {
                        EM_error2( decl->linepos,
                            "(emit): internal error: undefined ugen type '%s'",
                            value->name.c_str() );
                        return FALSE;
                    }
                    emit->append( new Chuck_Instr_Reg_Push_Imm( (t_CKUINT)info ) );
                    emit->append( new Chuck_Instr_UGen_Alloc() );
                    emit->append( new Chuck_Instr_Assign_Object );
                }
                else
                {
                    // TODO:
                }
            }
        }
        
        list = list->next;
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_namespace
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_namespace( Chuck_Emitter * emit,
                                         a_Exp_Namespace name_space )
{
    return FALSE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_code_segment()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_code_segment( Chuck_Emitter * emit, 
                                        a_Stmt_Code stmt, t_CKBOOL push )
{
    a_Stmt_List list = stmt->stmt_list;

    // loop through
    while( list )
    {
        // emit the statement
        if( !emit_engine_emit_stmt( emit, list->stmt ) )
            return FALSE;

        // next
        list = list->next;
    }

    // TODO: push
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_func_def()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_func_def( Chuck_Emitter * emit, a_Func_Def func_def )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_class_def()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_class_def( Chuck_Emitter * emit, a_Class_Def class_def )
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_spork()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_spork( Chuck_Emitter * emit, a_Exp_Func_Call exp )
{
    // emit the function call, with special flag
    if( !emit_engine_emit_exp_func_call( emit, exp, TRUE ) )
        return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_symbol()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_symbol( Chuck_Emitter * emit, S_Symbol symbol, 
                                  t_CKBOOL offset, int linepos )
{
    return TRUE;
}




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




//-----------------------------------------------------------------------------
// name: find_dur()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Emitter::find_dur( const string & name, t_CKDUR * out )
{
    // sanity
    assert( env != NULL );
    assert( out != NULL );

    // zero
    *out = 0.0;
    // get value from env
    Chuck_Value * value = env->global.lookup_value( name, FALSE );
    if( !value || !equals( value->type, &t_dur ) ) return FALSE;
    // copy
    *out = *( (t_CKDUR *)value->addr );
    
    return TRUE;
}
