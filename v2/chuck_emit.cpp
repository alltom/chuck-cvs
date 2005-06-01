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
t_CKBOOL emit_engine_emit_op( Chuck_Emitter * emit, ae_Operator op, a_Exp lhs, a_Exp rhs, a_Exp_Binary binary );
t_CKBOOL emit_engine_emit_op_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs, a_Exp_Binary binary );
t_CKBOOL emit_engine_emit_op_unchuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );
t_CKBOOL emit_engine_emit_op_at_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );
t_CKBOOL emit_engine_emit_exp_unary( Chuck_Emitter * emit, a_Exp_Unary unary );
t_CKBOOL emit_engine_emit_exp_primary( Chuck_Emitter * emit, a_Exp_Primary exp );
t_CKBOOL emit_engine_emit_exp_cast( Chuck_Emitter * emit, a_Exp_Cast cast );
t_CKBOOL emit_engine_emit_exp_postfix( Chuck_Emitter * emit, a_Exp_Postfix postfix );
t_CKBOOL emit_engine_emit_exp_dur( Chuck_Emitter * emit, a_Exp_Dur dur );
t_CKBOOL emit_engine_emit_exp_array( Chuck_Emitter * emit, a_Exp_Array array );
t_CKBOOL emit_engine_emit_exp_func_call( Chuck_Emitter * emit, Chuck_Func * func,
                                         Chuck_Type * type, int linepos, t_CKBOOL spork = FALSE );
t_CKBOOL emit_engine_emit_exp_func_call( Chuck_Emitter * emit, a_Exp_Func_Call func_call,
                                         t_CKBOOL spork = FALSE );
t_CKBOOL emit_engine_emit_exp_dot_member( Chuck_Emitter * emit, a_Exp_Dot_Member member );
t_CKBOOL emit_engine_emit_exp_if( Chuck_Emitter * emit, a_Exp_If exp_if );
t_CKBOOL emit_engine_emit_exp_decl( Chuck_Emitter * emit, a_Exp_Decl decl );
t_CKBOOL emit_engine_emit_array_lit( Chuck_Emitter * emit, a_Array_Sub array );
t_CKBOOL emit_engine_emit_code_segment( Chuck_Emitter * emit, a_Stmt_Code stmt,
                                        t_CKBOOL push = TRUE );
t_CKBOOL emit_engine_emit_func_def( Chuck_Emitter * emit, a_Func_Def func_def );
t_CKBOOL emit_engine_emit_class_def( Chuck_Emitter * emit, a_Class_Def class_def );
t_CKBOOL emit_engine_pre_constructor( Chuck_Emitter * emit, Chuck_Type * type );
t_CKBOOL emit_engine_instantiate_object( Chuck_Emitter * emit, Chuck_Type * type,
                                         a_Array_Sub array, t_CKBOOL is_ref );
t_CKBOOL emit_engine_emit_spork( Chuck_Emitter * emit, a_Exp_Func_Call exp );
t_CKBOOL emit_engine_emit_cast( Chuck_Emitter * emit, Chuck_Type * to, Chuck_Type * from );
t_CKBOOL emit_engine_emit_symbol( Chuck_Emitter * emit, S_Symbol symbol, 
                                  Chuck_Value * v, t_CKBOOL emit_var, int linepos );




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
    assert( emit->env->context->nspc->pre_ctor == NULL );

    // return
    t_CKBOOL ret = TRUE;
    // allocate the code
    emit->code = new Chuck_Code;
    // set the current context
    emit->context = emit->env->context;
    // set the namespace
    emit->nspc = emit->context->nspc;
    // reset the func
    emit->func = NULL;
    // clear the code stack
    emit->stack.clear();
    // name the code
    emit->code->name = "(shred main)";
    // whether code need this
    emit->code->need_this = TRUE;

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
        emit->context->nspc->pre_ctor = emit_to_code( emit->code, NULL, emit->dump );
    }

    // clear the code
    delete emit->code;
    emit->code = NULL;

    // return the code
    return emit->context->nspc->pre_ctor;
}




//-----------------------------------------------------------------------------
// name: emit_to_code()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_VM_Code * emit_to_code( Chuck_Code * in,
                              Chuck_VM_Code * out,
                              t_CKBOOL dump )
{
    // allocate the vm code
    Chuck_VM_Code * code = out ? out : new Chuck_VM_Code;
    // make sure
    assert( code->num_instr == 0 );
    // size
    code->num_instr = in->code.size();
    // allocate instruction pointers+
    code->instr = new Chuck_Instr *[code->num_instr];
    // set the stack depth
    code->stack_depth = in->stack_depth;
    // set whether code need this base pointer
    code->need_this = in->need_this;
    // set name
    code->name = in->name;

    // copy
    for( t_CKUINT i = 0; i < code->num_instr; i++ )
        code->instr[i] = in->code[i];

    Chuck_Instr_Goto * a = (Chuck_Instr_Goto *)in->code[22];

    // dump
    if( dump )
    {
        // name of what we are dumping
        EM_error2( 0, "dumping %s:", in->name.c_str() );

        // uh
        EM_error2( 0, "-------" );
        for( t_CKUINT i = 0; i < code->num_instr; i++ )
            EM_error2( 0, "[%i] %s( %s )", i, 
               code->instr[i]->name(), code->instr[i]->params() );
        EM_error2( 0, "-------\n" );
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
            if( !ret )
                return FALSE;
            // need to pop the final value from stack
            if( ret && pop && stmt->stmt_exp->type->size > 0 )
            {
                // sanity
                assert( stmt->stmt_exp->cast_to == NULL );

                // HACK!
                if( stmt->stmt_exp->type->size == 4 || stmt->stmt_exp->s_type == ae_exp_decl )
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
    emit->code->stack_cont.push_back( NULL );
    // mark the stack of break
    emit->code->stack_break.push_back( NULL );

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
    while( emit->code->stack_cont.size() && emit->code->stack_cont.back() )
    {
        emit->code->stack_cont.back()->set( start_index );
        emit->code->stack_cont.pop_back();
    }

    // stack of break
    while( emit->code->stack_break.size() && emit->code->stack_break.back() )
    {
        emit->code->stack_break.back()->set( emit->next_index() );
        emit->code->stack_break.pop_back();
    }

    // pop stack
    emit->pop_scope();
    // pop continue stack
    emit->code->stack_cont.pop_back();
    // pop break stack
    emit->code->stack_break.pop_back();

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
    emit->code->stack_cont.push_back( NULL );
    // mark the stack of break
    emit->code->stack_break.push_back( NULL );

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
    while( emit->code->stack_cont.size() && emit->code->stack_cont.back() )
    {
        emit->code->stack_cont.back()->set( start_index );
        Chuck_Instr_Goto * a = emit->code->stack_cont.back();
        emit->code->stack_cont.pop_back();
    }

    // stack of break
    while( emit->code->stack_break.size() && emit->code->stack_break.back() )
    {
        emit->code->stack_break.back()->set( emit->next_index() );
        emit->code->stack_break.pop_back();
    }

    // pop stack
    emit->pop_scope();
    // pop continue stack
    emit->code->stack_cont.pop_back();
    // pop break stack
    emit->code->stack_break.pop_back();

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
    emit->code->stack_cont.push_back( NULL );
    // mark the stack of break
    emit->code->stack_break.push_back( NULL );

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
    while( emit->code->stack_cont.size() && emit->code->stack_cont.back() )
    {
        emit->code->stack_cont.back()->set( start_index );
        emit->code->stack_cont.pop_back();
    }

    // stack of break
    while( emit->code->stack_break.size() && emit->code->stack_break.back() )
    {
        emit->code->stack_break.back()->set( emit->next_index() );
        emit->code->stack_break.pop_back();
    }

    // pop stack
    emit->pop_scope();
    // pop continue stack
    emit->code->stack_cont.pop_back();
    // pop break stack
    emit->code->stack_break.pop_back();
    
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
    emit->code->stack_cont.push_back( NULL );
    // mark the stack of break
    emit->code->stack_break.push_back( NULL );

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
    while( emit->code->stack_cont.size() && emit->code->stack_cont.back() )
    {
        emit->code->stack_cont.back()->set( start_index );
        emit->code->stack_cont.pop_back();
    }

    // stack of break
    while( emit->code->stack_break.size() && emit->code->stack_break.back() )
    {
        emit->code->stack_break.back()->set( emit->next_index() );
        emit->code->stack_break.pop_back();
    }

    // pop stack
    emit->pop_scope();
    // pop continue stack
    emit->code->stack_cont.pop_back();
    // pop break stack
    emit->code->stack_break.pop_back();
    
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
    emit->code->stack_cont.push_back( NULL );
    // mark the stack of break
    emit->code->stack_break.push_back( NULL );

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
    while( emit->code->stack_cont.size() && emit->code->stack_cont.back() )
    {
        emit->code->stack_cont.back()->set( start_index );
        emit->code->stack_cont.pop_back();
    }

    // stack of break
    while( emit->code->stack_break.size() && emit->code->stack_break.back() )
    {
        emit->code->stack_break.back()->set( emit->next_index() );
        emit->code->stack_break.pop_back();
    }

    // pop stack
    emit->pop_scope();
    // pop continue stack
    emit->code->stack_cont.pop_back();
    // pop break stack
    emit->code->stack_break.pop_back();
    
    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_break()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_break( Chuck_Emitter * emit, a_Stmt_Break br )
{
    // append
    Chuck_Instr_Goto * op = new Chuck_Instr_Goto( 0 );
    emit->append( op );
    // remember
    emit->code->stack_break.push_back( op );
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_continue()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_continue( Chuck_Emitter * emit, a_Stmt_Continue cont )
{
    // append
    Chuck_Instr_Goto * op = new Chuck_Instr_Goto( 0 );
    emit->append( op );
    // remember
    emit->code->stack_cont.push_back( op );

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

    // emit return
    // emit->append( new Chuck_Instr_Func_Return );

    // determine where later
    Chuck_Instr_Goto * instr = new Chuck_Instr_Goto( 0 );
    emit->append( instr );
    emit->code->stack_return.push_back( instr );

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
    // assert( exp->next == NULL );

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
    if( !emit_engine_emit_op( emit, binary->op, binary->lhs, binary->rhs, binary ) )
        return FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_op()
// desc: emit binary operator
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_op( Chuck_Emitter * emit, ae_Operator op, a_Exp lhs, a_Exp rhs, a_Exp_Binary binary )
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
            emit->append( instr = new Chuck_Instr_Mod_double );
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
            emit->append( instr = new Chuck_Instr_Mod_double_Reverse );
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

        if( !emit_engine_emit_op_chuck( emit, cl, cr, binary ) )
            return FALSE;

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
        if( isa( t_left, &t_object ) && isa( t_right, &t_object ) )
            emit->append( instr = new Chuck_Instr_Eq_int );
        else
        {
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
        }
        break;
    
    case ae_op_neq:
        if( isa( t_left, &t_object ) && isa( t_right, &t_object ) )
            emit->append( instr = new Chuck_Instr_Neq_int );
        else
        {
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
t_CKBOOL emit_engine_emit_op_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs, a_Exp_Binary binary )
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
    
    // time advance
    if( isa( left, &t_event ) && isa( right, &t_time ) && rhs->s_meta == ae_meta_var &&
		rhs->s_type == ae_exp_primary && !strcmp( "now", S_name(rhs->primary.var) ) )
    {
		// pop now
		emit->append( new Chuck_Instr_Reg_Pop_Word2 );
		emit->append( new Chuck_Instr_Event_Wait );

        return TRUE;
    }

    // func call
    if( isa( right, &t_function ) )
    {
        assert( binary->ck_func != NULL );
        
        // emit
        return emit_engine_emit_exp_func_call( emit, binary->ck_func, binary->self->type, binary->linepos );
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
			else if( isa( left, &t_string ) ) // string
			{
				// assign string
				emit->append( new Chuck_Instr_Assign_String );
			}
            else
            {
                // assign primitive
                if( right->size == 4 )
                    emit->append( new Chuck_Instr_Assign_Primitive );
                else if( right->size == 8 )
                    emit->append( new Chuck_Instr_Assign_Primitive2 );
                else
                {
                    EM_error2( rhs->linepos,
                        "(emit): internal error: assignment to type of size %i...",
                        right->size );
                }
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

    // get type
    Chuck_Type * t = unary->self->type;
    assert( t != NULL );

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

    case ae_op_new:

        // if this is an object
        if( isobj( t ) )
        {
            // instantiate object, including array
            if( !emit_engine_instantiate_object( emit, t, unary->type->array, unary->type->ref ) )
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
    Chuck_String * str = NULL;

    // find out exp
    switch( exp->s_type )
    {
    case ae_primary_var:
        if( exp->var == insert_symbol( "now" ) )
        {
            emit->append( new Chuck_Instr_Reg_Push_Now );
        }
        else if( exp->var == insert_symbol( "this" ) )
        {
            // TODO: verify this is in the right scope
            emit->append( new Chuck_Instr_Reg_Push_This );
        }
        else if( exp->var == insert_symbol( "me" ) )
        {
            emit->append( new Chuck_Instr_Reg_Push_Me );
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
        else if( exp->var == insert_symbol("null") ||
                 exp->var == insert_symbol("NULL") )
        {
            emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
        }
        else if( exp->var == insert_symbol( "pi" ) )
        {
            double pi = 3.14159265358979323846;
            emit->append( new Chuck_Instr_Reg_Push_Imm2( pi ) );
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
        else if( emit->find_dur( S_name(exp->var), &dur ) )
        {
            emit->append( new Chuck_Instr_Reg_Push_Imm2( dur ) );
        }
        else
        {
            // emit the symbol
            return emit_engine_emit_symbol( 
                emit, exp->var, exp->value, exp->self->emit_var, exp->linepos );
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
        str = new Chuck_String;
		initialize_object( str, &t_string );
        str->str = exp->str;
        temp = (t_CKUINT)str;
        emit->append( new Chuck_Instr_Reg_Push_Imm( temp ) );
        break;

    case ae_primary_array:
        if( !emit_engine_emit_array_lit( emit, exp->array ) )
            return FALSE;
        break;
        
    case ae_primary_exp:
        if( !emit_engine_emit_exp( emit, exp->exp ) )
            return FALSE;
        break;

    case ae_primary_hack:
        // emit the expression
        if( !emit_engine_emit_exp( emit, exp->exp ) )
            return FALSE;

        // emit hack
        emit->append( new Chuck_Instr_Hack( exp->self->type ) );
        break;

    }
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_array_lit()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_array_lit( Chuck_Emitter * emit, a_Array_Sub array )
{
    // go through and emit the expressions
    if( !emit_engine_emit_exp( emit, array->exp_list ) )
        return FALSE;

    // count the number
    a_Exp e = array->exp_list;
    t_CKUINT count = 0;
    // loop over
    while( e ) { count++; e = e->next; }

    // the type
    e = array->exp_list;
    Chuck_Type * type = e->cast_to ? e->cast_to : e->type;

    // construct array dynamically
    emit->append( new Chuck_Instr_Array_Init( type, count ) );

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

	// emit the exp
	if( !emit_engine_emit_exp( emit, cast->exp ) )
		return FALSE;

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
    Chuck_Type * type = NULL, * base_type = NULL;
    t_CKUINT depth = 0;
    a_Array_Sub sub = NULL;
    a_Exp exp = NULL;
    t_CKBOOL is_var = FALSE;
    t_CKBOOL is_str = FALSE;

    // get the type
    type = array->self->type;
    // get the base type
    base_type = array->base->type;
    // emit as addr?
    is_var = array->self->emit_var;
    // get the dimension
    depth = base_type->array_depth - type->array_depth;
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

    // emit the exp list
    if( !emit_engine_emit_exp( emit, exp ) )
        return FALSE;

    // find out first element
    if( isa( exp->type, &t_string ) )
        is_str = TRUE;

    // make sure
    if( type->size != 4 && type->size != 8 )
    {
        EM_error2( array->linepos,
            "(emit): internal error: array with datasize of %i...", type->size );
        return FALSE;
    }

    // check the depth
    if( depth == 1 )
    {
        // emit the array access
        if( is_str )
            emit->append( new Chuck_Instr_Array_Map_Access( type->size, is_var ) );
        else
            emit->append( new Chuck_Instr_Array_Access( type->size, is_var ) );
    }
    else
    {
        // emit the multi array access
        emit->append( new Chuck_Instr_Array_Access_Multi( depth, type->size, is_var ) );
    }

    // TODO: variable?

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_exp_func_call()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_func_call( Chuck_Emitter * emit,
                                         Chuck_Func * func,
                                         Chuck_Type * type,
                                         int linepos,
                                         t_CKBOOL spork )
{
    // is a member?
    t_CKBOOL is_member = func->is_member;

    // if member
    //if( is_member )
    //{
    //    // this
    //    emit->append( new Chuck_Instr_Reg_Push_This );
    //}

    // translate to code
    emit->append( new Chuck_Instr_Func_To_Code );
    // emit->append( new Chuck_Instr_Reg_Push_Imm( (t_CKUINT)func->code ) );
    // push the local stack depth - local variables
    emit->append( new Chuck_Instr_Reg_Push_Imm( emit->code->frame->curr_offset ) );

    // TODO: member functions and static functions
    // call the function
    t_CKUINT size = type->size;
    if( func->def->s_type == ae_func_builtin )
    {
        if( size == 0 || size == 4 || size == 8 )
        {
            // is member
            if( is_member )
                emit->append( new Chuck_Instr_Func_Call_Member( size ) );
            else
                emit->append( new Chuck_Instr_Func_Call_Static( size ) );
        }
        else
        {
            EM_error2( linepos,
                       "(emit): internal error: %i func call not handled",
                       size );
            return FALSE;
        }
    }
    else
    {
        emit->append( new Chuck_Instr_Func_Call );
    }

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
    // make sure there are args
    if( func_call->args )
    {
        // emit the args
        if( !emit_engine_emit_exp( emit, func_call->args ) )
        {
            EM_error2( func_call->linepos,
                       "(emit): internal error in emitting function call arguments..." );
            return FALSE;
        }
    }

    // emit func
    if( !emit_engine_emit_exp( emit, func_call->func ) )
    {
        EM_error2( func_call->linepos,
                   "(emit): internal error in evaluating function call..." );
        return FALSE;
    }
    
    // the rest
    return emit_engine_emit_exp_func_call( emit, func_call->ck_func, func_call->ret_type,
                                           func_call->linepos, spork );
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_dot_member()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_exp_dot_member( Chuck_Emitter * emit,
                                          a_Exp_Dot_Member member )
{
    // the type of the base
    Chuck_Type * t_base = NULL;
    // whether to emit addr or value
    t_CKBOOL emit_addr = member->self->emit_var;
    // is the base a class/namespace or a variable
    t_CKBOOL base_static = isa( member->t_base, &t_class );
    // a function
    Chuck_Func * func = NULL;
    // a non-function value
    Chuck_Value * value = NULL;
    // the offset
    t_CKUINT offset = 0;
    // actual type - if base is class name its type is actually 'class'
    //               to get the actual type use actual_type
    t_base = base_static ? member->t_base->actual_type : member->t_base;

    // make sure that the base type is object
    assert( t_base->info != NULL );

    // if base is static
    if( !base_static )
    {
        // if is a func
        if( isfunc( member->self->type ) )
        {
            // get the func
            value = type_engine_find_value( t_base, member->id );
            func = value->func_ref;
            // make sure it's there
            assert( func != NULL );

            // is the func static?
            if( func->is_member )
            {
                // emit the base
                emit_engine_emit_exp( emit, member->base );
                // dup the base pointer
                emit->append( new Chuck_Instr_Reg_Dup_Last );
                // find the offset for virtual table
                offset = func->vt_index;
                // emit the function
                emit->append( new Chuck_Instr_Dot_Member_Func( offset ) );
            }
            else
            {
                // emit the type
                emit->append( new Chuck_Instr_Reg_Push_Imm( (t_CKUINT)t_base ) );
                // emit the static function
                emit->append( new Chuck_Instr_Dot_Static_Func( func ) );
            }
        }
        else
        {
            // get the value
            // value = t_base->info->lookup_value( member->id, FALSE );
            value = type_engine_find_value( t_base, member->id );
            // make sure it's there
            assert( value != NULL );

            // find the offset for data
            offset = value->offset;

            // is the value static?
            if( value->is_member )
            {
                // emit the base
                emit_engine_emit_exp( emit, member->base );
                // lookup the member
                emit->append( new Chuck_Instr_Dot_Member_Data( 
                    offset, member->self->type->size, emit_addr ) );
            }
            else
            {
                // if builtin or not
                if( value->addr )
                {
                    // emit builtin
                    emit->append( new Chuck_Instr_Dot_Static_Import_Data(
                        value->addr, member->self->type->size, emit_addr ) );
                }
                else
                {
                    // emit the type
                    emit->append( new Chuck_Instr_Reg_Push_Imm( (t_CKUINT)t_base ) );
                    // emit the static value
                    emit->append( new Chuck_Instr_Dot_Static_Data(
                        offset, member->self->type->size, emit_addr ) );
                }
            }
        }
    }
    else // static
    {
        // emit the type
        emit->append( new Chuck_Instr_Reg_Push_Imm( (t_CKUINT)t_base ) );

        // if is a func
        if( isfunc( member->self->type ) )
        {
            // get the func
            func = t_base->info->lookup_func( member->id, FALSE );
            // make sure it's there
            assert( func != NULL );
            // emit the function
            emit->append( new Chuck_Instr_Dot_Static_Func( func ) );
        }
        else
        {
            // get the value
            value = t_base->info->lookup_value( member->id, FALSE );
            // make sure it's there
            assert( value != NULL );

            // if builtin
            if( value->addr )
            {
                // emit
                emit->append( new Chuck_Instr_Dot_Static_Import_Data(
                    value->addr, member->self->type->size, emit_addr ) );
            }
            else
            {
                // find the offset for data
                offset = value->offset;
                // emit the member
                emit->append( new Chuck_Instr_Dot_Static_Data(
                    offset, member->self->type->size, emit_addr ) );
            }
        }
    }
    
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
// name: emit_engine_pre_constructor()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_pre_constructor( Chuck_Emitter * emit, Chuck_Type * type )
{
    // parent first pre constructor
    if( type->parent != NULL )
        emit_engine_pre_constructor( emit, type->parent );

    // pre constructor
    if( type->has_constructor )
    {
        // make sure
        assert( type->info->pre_ctor != NULL );
        // append instruction
        emit->append( new Chuck_Instr_Pre_Constructor( type->info->pre_ctor,
            emit->code->frame->curr_offset ) );
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_pre_constructor_array()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_pre_constructor_array( Chuck_Emitter * emit, Chuck_Type * type )
{
    // alloc should have put all objects to made in linear list, on stack
    Chuck_Instr_Pre_Ctor_Array_Top * top = NULL;
    Chuck_Instr_Pre_Ctor_Array_Bottom * bottom = NULL;
    // get start index
    t_CKUINT start_index = emit->next_index();
    // append first part of pre ctor
    emit->append( top = new Chuck_Instr_Pre_Ctor_Array_Top( type ) );
    // call pre constructor
    emit_engine_pre_constructor( emit, type );
    // append second part of the pre ctor
    emit->append( bottom = new Chuck_Instr_Pre_Ctor_Array_Bottom );
    // set the goto of the first one
    top->set( emit->next_index() );
    // set the goto for the second one
    bottom->set( start_index );
    // clean up code
    emit->append( new Chuck_Instr_Pre_Ctor_Array_Post );

    return TRUE;
}



//-----------------------------------------------------------------------------
// name: emit_engine_instantiate_object()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_instantiate_object( Chuck_Emitter * emit, Chuck_Type * type,
                                         a_Array_Sub array, t_CKBOOL is_ref )
{
    // if array
    if( type->array_depth )
    {
        // emit indices
        emit_engine_emit_exp( emit, array->exp_list );
        // emit array allocation
        emit->append( new Chuck_Instr_Array_Alloc( 
            type->array_depth, type->array_type, emit->code->frame->curr_offset,
            is_ref ) );

        // handle constructor
        if( isobj( type->array_type ) && !is_ref )
        {
            // call pre constructor for array
            emit_engine_pre_constructor_array( emit, type->array_type );
        }
    }
    else if( !is_ref ) // not array
    {
        // emit object instantiation code, include pre constructor
        emit->append( new Chuck_Instr_Instantiate_Object( type ) );
        
        // call pre constructor
        emit_engine_pre_constructor( emit, type );
    }

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
    t_CKBOOL is_obj = FALSE;
    t_CKBOOL is_ref = FALSE;

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
        is_obj = isobj( type );
        // do alloc or not
        is_ref = decl->type->ref;

        // if this is an object
        if( is_obj )
        {
            // if array, then check to see if empty []
            if( !list->var_decl->array || list->var_decl->array->exp_list != NULL )
            {
                // instantiate object, including array
                if( !emit_engine_instantiate_object( emit, type, list->var_decl->array, is_ref ) )
                    return FALSE;
            }
        }

        // put in the value

        // member
        if( value->is_member )
        {
            // zero out location in object, and leave addr on operand stack
            if( type->size == 4 )
                emit->append( new Chuck_Instr_Alloc_Member_Word( value->offset ) );
            else if( type->size == 8 )
                emit->append( new Chuck_Instr_Alloc_Member_Word2( value->offset ) );
            else
            {
                EM_error2( decl->linepos,
                    "(emit): unhandle decl size of '%i'...",
                    type->size );
                return FALSE;
            }
        }
        else // not member
        {
			// not in class
			if( !emit->env->class_def || !decl->is_static )
			{
				// allocate a place on the local stack
				local = emit->alloc_local( type->size, value->name, is_ref );
				if( !local )
				{
					EM_error2( decl->linepos,
						"(emit): internal error: cannot allocate local '%s'...",
						value->name.c_str() );
					return FALSE;
				}

				// put in the value
				value->offset = local->offset;

				// zero out location in memory, and leave addr on operand stack
				// TODO: this is wrong for static
				// BAD:
				// FIX:
				if( type->size == 4 )
					emit->append( new Chuck_Instr_Alloc_Word( local->offset ) );
				else if( type->size == 8 )
					emit->append( new Chuck_Instr_Alloc_Word2( local->offset ) );
				else
				{
					EM_error2( decl->linepos,
						"(emit): unhandle decl size of '%i'...",
						type->size );
					return FALSE;
				}
			}
			else
			{
				// push something on the stack to pop...
			    // static
				// HACK
				// TODO
				emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );
			}
        }

        // if object, assign
        if( is_obj && !is_ref )
        {
            // assign the object
            emit->append( new Chuck_Instr_Assign_Object );
        }
        
        list = list->next;
    }

    return TRUE;
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
    // get the func
    Chuck_Func * func = func_def->ck_func;
    // get the value
    Chuck_Value * value = func->value_ref;
    // get the type
    Chuck_Type * type = value->type;
    // local
    Chuck_Local * local = NULL;

    // make sure it's the same one
    //Chuck_Func * func2 = emit->env->context->nspc.lookup_func( func_def->name, FALSE );
    //if( func != func2 )
    //{
    //    EM_error2( func_def->linepos,
    //        "(emit): ambiguous function resolution for %s...",
    //        S_name(func_def->name) );
    //    return FALSE;
    //}

    // make sure the code is empty
    if( func->code != NULL )
    {
        EM_error2( func_def->linepos,
            "(emit): function '%s' already emitted...",
            S_name(func_def->name) );
        return FALSE;
    }

    // make sure we are not in a function already
    if( emit->env->func != NULL )
    {
        EM_error2( func_def->linepos,
            "(emit): internal error: nested function definition..." );
        return FALSE;
    }

    // if not part of a class
    if( !emit->env->class_def )
    {
        // put function on stack
        local = emit->alloc_local( value->type->size, value->name, TRUE );
        // remember the offset
        value->offset = local->offset;
        // write to mem stack
        emit->append( new Chuck_Instr_Mem_Set_Imm( value->offset, (t_CKUINT)func ) );
    }

    // set the func
    emit->env->func = func;
    // push the current code
    emit->stack.push_back( emit->code );
    // make a new one
    emit->code = new Chuck_Code;
    // name the code
    emit->code->name = emit->env->class_def ? emit->env->class_def->name + "." : "";
    emit->code->name += func->name + "( ... )";
    // set whether need this
    emit->code->need_this = func->is_member;

    // go through the args
    a_Arg_List a = func_def->arg_list;
    t_CKBOOL is_ref = FALSE;

    // if member (non-static) function
    if( func->is_member )
    {
        // get the size
        emit->code->stack_depth += sizeof(t_CKUINT);
        // add this
        if( !emit->alloc_local( sizeof(t_CKUINT), "this", TRUE ) )
        {
            EM_error2( a->linepos,
                "(emit): internal error: cannot allocate local 'this'..." );
            return FALSE;
        }
    }
    // loop through args
    while( a )
    {
        // get the value
        value = a->var_decl->value;
        // get the type
        type = value->type;
        // get ref
        is_ref = a->type_decl->ref;
        // get the size
        emit->code->stack_depth += type->size;
        // allocate a place on the local stack
        local = emit->alloc_local( type->size, value->name, is_ref );
        if( !local )
        {
            EM_error2( a->linepos,
                "(emit): internal error: cannot allocate local '%s'...",
                value->name.c_str() );
            return FALSE;
        }
        // remember the offset
        value->offset = local->offset;

        // advance
        a = a->next;
    }

    // TODO: make sure the calculated stack depth is the same as func_def->stack depth
    // taking into account member function

    // emit the code
    if( !emit_engine_emit_stmt( emit, func_def->code, FALSE ) )
        return FALSE;

    // set the index for next instruction for return statements
    for( t_CKINT i = 0; i < emit->code->stack_return.size(); i++ )
        emit->code->stack_return[i]->set( emit->next_index() );
    // clear the return stack
    emit->code->stack_return.clear();
    // emit return statement
    emit->append( new Chuck_Instr_Func_Return );

    // vm code
    func->code = emit_to_code( emit->code, NULL, emit->dump );
    
    // unset the func
    emit->env->func = NULL;
    // delete the code
    SAFE_DELETE( emit->code );
    // pop the code
    assert( emit->stack.size() );
    emit->code = emit->stack.back();
    emit->stack.pop_back();

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_class_def()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_class_def( Chuck_Emitter * emit, a_Class_Def class_def )
{
    // get the type
    Chuck_Type * type = class_def->type;
    // the return type
    t_CKBOOL ret = TRUE;
    // the class body
    a_Class_Body body = class_def->body;
    
    // make sure the code is empty
    if( type->info->pre_ctor != NULL )
    {
        EM_error2( class_def->linepos,
            "(emit): class '%s' already emitted...",
            type->name.c_str() );
        return FALSE;
    }

    // make sure we are not in a class already
    //if( emit->env->class_def != NULL )
    //{
    //    EM_error2( class_def->linepos,
    //        "(emit): internal error: nested class definition..." );
    //    return FALSE;
    //}

    // set the class
    emit->env->class_stack.push_back( emit->env->class_def );
    emit->env->class_def = type;
    // push the current code
    emit->stack.push_back( emit->code );
    // make a new one
    emit->code = new Chuck_Code;
    // name the code
    emit->code->name = string("class ") + type->name;
    // whether code needs this
    emit->code->need_this = TRUE;
    // if has constructor
    if( type->has_constructor ) type->info->pre_ctor = new Chuck_VM_Code;

    // get the size
    emit->code->stack_depth += sizeof(t_CKUINT);
    // add this
    if( !emit->alloc_local( sizeof(t_CKUINT), "this", TRUE ) )
    {
        EM_error2( class_def->linepos,
            "(emit): internal error: cannot allocate local 'this'..." );
        return FALSE;
    }

    // emit the body
    while( body && ret )
    {
        // check the section
        switch( body->section->s_type )
        {
        case ae_section_stmt:
            ret = emit_engine_emit_stmt_list( emit, body->section->stmt_list );
            break;
        
        case ae_section_func:
            ret = emit_engine_emit_func_def( emit, body->section->func_def );
            break;
        
        case ae_section_class:
            ret = emit_engine_emit_class_def( emit, body->section->class_def );
            //EM_error2( body->section->class_def->linepos,
            //    "nested class definitions are not yet supported..." );
            //ret = FALSE;
            break;
        }
        
        // move to the next section
        body = body->next;
    }

    // if ok
    if( ret )
    {
        // emit return statement
        emit->append( new Chuck_Instr_Func_Return );
        // vm code
        type->info->pre_ctor = emit_to_code( emit->code, type->info->pre_ctor, emit->dump );
		// allocate static
		type->info->class_data = new t_CKBYTE[type->info->class_data_size];
    }
    else
    {
        // clean
        SAFE_DELETE( type->info->pre_ctor );
    }

    // unset the class
    emit->env->class_def = emit->env->class_stack.back();
    emit->env->class_stack.pop_back();
    // delete the code
    SAFE_DELETE( emit->code );
    // pop the code
    assert( emit->stack.size() );
    emit->code = emit->stack.back();
    emit->stack.pop_back();

    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_spork()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_spork( Chuck_Emitter * emit, a_Exp_Func_Call exp )
{
    // spork
    Chuck_Instr_Mem_Push_Imm * op = NULL;

    // push the current code
    emit->stack.push_back( emit->code );
    // make a new one
    emit->code = new Chuck_Code;
    // handle need this
    emit->code->need_this = exp->ck_func->is_member;
    // name it
    emit->code->name = "spork ~ exp";
    // push op
    op = new Chuck_Instr_Mem_Push_Imm( 0 );
    // emit the stack depth - we don't know this yet
    emit->append( op );

    // emit the function call, with special flag
    if( !emit_engine_emit_exp_func_call( emit, exp, TRUE ) )
        return FALSE;

    // done
    emit->append( new Chuck_Instr_EOC );
    // set the stack depth now that we know
    op->set( emit->code->stack_depth );

    // emit it
    Chuck_VM_Code * code = emit_to_code( emit->code, NULL, emit->dump );
    //code->name = string("spork ~ exp");

    // restore the code
    assert( emit->stack.size() > 0 );
    emit->code = emit->stack.back();
    // pop
    emit->stack.pop_back();

    a_Exp e = exp->args;
    t_CKUINT size = 0;
    while( e ) { size += e->type->size; e = e->next; }

    // emit instruction that will put the code on the stack
    emit->append( new Chuck_Instr_Reg_Push_Imm( (t_CKUINT)code ) );
    // emit spork instruction
    emit->append( new Chuck_Instr_Spork( size ) );

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: emit_engine_emit_symbol()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_symbol( Chuck_Emitter * emit, S_Symbol symbol, 
                                  Chuck_Value * v, t_CKBOOL emit_var,
                                  int linepos )
{
    // look up the value
    // Chuck_Value * v = emit->env->curr->lookup_value( symbol, TRUE );
    // it should be there
    if( !v )
    {
        // internal error
        EM_error2( linepos,
            "(emit): internal error: undefined symbol '%s'...",
            S_name(symbol) );
        return FALSE;
    }

    // if part of class - this only works because x.y is handled separately
    if( v->owner_class )
    {
        // emit as this.v
        a_Exp base = new_exp_from_id( "this", linepos );
        a_Exp dot = new_exp_from_member_dot( base, (char *)v->name.c_str(), linepos );
        base->type = v->owner_class;
        dot->type = v->type;
        dot->dot_member.t_base = v->owner_class;
        dot->emit_var = emit_var;
        // emit it
        if( !emit_engine_emit_exp_dot_member( emit, &dot->dot_member ) )
        {
            // internal error
            EM_error2( linepos,
                "(emit): internal error: symbol transformation failed..." );
            return FALSE;
        }

        return TRUE;
    }

    // var or value
    if( emit_var )
    {
        // emit as addr
        emit->append( new Chuck_Instr_Reg_Push_Mem_Addr( v->offset, v->is_context_global ) );
    }
    else
    {
        // check size
        if( v->type->size == 4 )
            emit->append( new Chuck_Instr_Reg_Push_Mem( v->offset, v->is_context_global ) );
        else if( v->type->size == 8 )
            emit->append( new Chuck_Instr_Reg_Push_Mem2( v->offset, v->is_context_global  ) );
        else
        {
            // internal error
            EM_error2( linepos,
                "(emit): internal error: symbol '%s' has size '%i'...",
                S_name(symbol), v->type->size );
            return FALSE;
        }
    }

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
    Chuck_Value * value = env->global()->lookup_value( name, FALSE );
    if( !value || !equals( value->type, &t_dur ) ) return FALSE;
    // copy
    *out = *( (t_CKDUR *)value->addr );
    
    return TRUE;
}




/*
//-----------------------------------------------------------------------------
// name: emit_engine_instantiate_object()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_instantiate_object( Chuck_Emitter * emit, Chuck_Type * type,
                                         a_Array_Sub array, t_CKBOOL is_ref )
{
    // if array
    if( type->array_depth )
    {
        // emit indices
        emit_engine_emit_exp( emit, array->exp_list );
        // emit array allocation
        emit->append( new Chuck_Instr_Array_Alloc( 
            type->array_depth, type->array_type, emit->code->frame->curr_offset,
            is_ref ) );

        // handle constructor
        //if( isobj( type->array_type ) && !is_ref )
        //{
        //    // TODO:
        //    EM_error2( array->linepos, "internal error: object array constructor not impl..." );
        //    return FALSE;
        //}
    }
    else if( !is_ref ) // not array
    {
        // if ugen
        //if( isa( type, &t_ugen ) )
        //{
        //    // get the ugen info
        //    Chuck_UGen_Info * info = decl->self->type->ugen;
        //    if( !info )
        //    {
        //        EM_error2( decl->linepos,
        //            "(emit): internal error: undefined ugen type '%s'",
        //            type->name.c_str() );
        //        return FALSE;
        //    }
        //    emit->append( new Chuck_Instr_Reg_Push_Imm( (t_CKUINT)info ) );
        //    emit->append( new Chuck_Instr_UGen_Alloc() );
        //}
        //else
        //{

        // emit object instantiation code, include pre constructor
        emit->append( new Chuck_Instr_Instantiate_Object( type ) );

        //}
        
        // call pre constructor
        emit_engine_pre_constructor( emit, type );

        // constructor
        //if( type->has_constructor )
        //{
        //    // make sure
        //    assert( type->info->pre_ctor != NULL );
        //    // push this
        //    emit->append( new Chuck_Instr_Reg_Dup_Last );
        //    // push pre-constructor
        //    emit->append( new Chuck_Instr_Reg_Push_Imm( (t_CKUINT)type->info->pre_ctor ) );
        //    // push frame offset
        //    emit->append( new Chuck_Instr_Reg_Push_Imm( emit->code->frame->curr_offset ) );
        //    // call the function
        //    if( type->info->pre_ctor->native_func != NULL )
        //        emit->append( new Chuck_Instr_Func_Call_Member( 0 ) );
        //    else
        //        emit->append( new Chuck_Instr_Func_Call );
        //}
    }

    return TRUE;
}*/
