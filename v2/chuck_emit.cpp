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
t_CKBOOL emit_engine_emit_prog( Chuck_Emitter * emit, a_Program prog );
t_CKBOOL emit_engine_emit_stmt_list( Chuck_Emitter * emit, a_Stmt_List list );
t_CKBOOL emit_engine_emit_stmt( Chuck_Emitter * emit, a_Stmt stmt );
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
t_CKTYPE emit_engine_emit_exp( Chuck_Emitter * emit, a_Exp exp );
t_CKTYPE emit_engine_emit_exp_binary( Chuck_Emitter * emit, a_Exp_Binary binary );
t_CKTYPE emit_engine_emit_op( Chuck_Emitter * emit, ae_Operator op, a_Exp lhs, a_Exp rhs );
t_CKTYPE emit_engine_emit_op_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );
t_CKTYPE emit_engine_emit_op_unchuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );
t_CKTYPE emit_engine_emit_exp_unary( Chuck_Emitter * emit, a_Exp_Unary unary );
t_CKTYPE emit_engine_emit_exp_primary( Chuck_Emitter * emit, a_Exp_Primary exp );
t_CKTYPE emit_engine_emit_exp_cast( Chuck_Emitter * emit, a_Exp_Cast cast );
t_CKTYPE emit_engine_emit_exp_postfix( Chuck_Emitter * emit, a_Exp_Postfix postfix );
t_CKTYPE emit_engine_emit_exp_dur( Chuck_Emitter * emit, a_Exp_Dur dur );
t_CKTYPE emit_engine_emit_exp_array( Chuck_Emitter * emit, a_Exp_Array array );
t_CKTYPE emit_engine_emit_exp_func_call( Chuck_Emitter * emit, a_Exp_Func_Call func_call );
t_CKTYPE emit_engine_emit_exp_dot_member( Chuck_Emitter * emit, a_Exp_Dot_Member member );
t_CKTYPE emit_engine_emit_exp_if( Chuck_Emitter * emit, a_Exp_If exp_if );
t_CKTYPE emit_engine_emit_exp_decl( Chuck_Emitter * emit, a_Exp_Decl decl );
t_CKTYPE emit_engine_emit_exp_namespace( Chuck_Emitter * emit, a_Exp_Namespace name_space );
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

    return ret;
}




//-----------------------------------------------------------------------------
// name: emit_to_code
// desc: ...
//-----------------------------------------------------------------------------
Chuck_VM_Code * emit_to_code( Chuck_Emitter * emit, t_CKBOOL dump )
{
    Chuck_VM_Code * code = new Chuck_VM_Code;
    code->num_instr = emit->code->size();
    code->instr = new Chuck_Instr *[code->num_instr];

    // copy
    for( t_CKUINT i = 0; i < code->num_instr; i++ )
        code->instr[i] = emit->code->code[i];

    // dump
    if( dump )
    {
        // name of what we are dumping
        EM_error2( 0, "dumping %s", emit->code->name.c_str() );
        EM_error2( 0, "\n" );

        // uh
        for( t_CKUINT i = 0; i < code2->num_instr; i++ )
            EM_error2( 0, 
                "'%i' %s( %s )\n",
                i, 
               code2->instr[i]->name(), code2->instr[i]->params() );

        fprintf( stdout, "...\n\n" );
    }

        
        // functions
    for( unsigned int j = 0; j < emit->functions.size(); j++ )
    {
        Chuck_VM_Code * code2 = new Chuck_VM_Code;
        Chuck_Code * c = emit->functions[j];
        code2->num_instr = c->code.size();
        code2->instr = new Chuck_Instr *[code2->num_instr];

        // copy
        for( unsigned int i = 0; i < code2->num_instr; i++ )
            code2->instr[i] = c->code[i];

        // set the parent
        code2->stack_depth = c->stack_depth;
        
        if( dump )
        {
            fprintf( stderr, "[chuck]: dumping function %s( ... )\n\n", c->name.c_str() );

            for( unsigned int i = 0; i < code2->num_instr; i++ )
                fprintf( stdout, "'%i' %s( %s )\n", i, 
                    code2->instr[i]->name(), code2->instr[i]->params() );

            fprintf( stdout, "...\n\n" );
        }

        // put in table
        S_enter( g_func2code, insert_symbol((char *)c->name.c_str()), code2 );
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
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_prog( Chuck_Emitter * emit, a_Program prog );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_stmt_list( Chuck_Emitter * emit, a_Stmt_List list );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_stmt( Chuck_Emitter * emit, a_Stmt stmt );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_if( Chuck_Emitter * emit, a_Stmt_If stmt );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_for( Chuck_Emitter * emit, a_Stmt_For stmt );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_while( Chuck_Emitter * emit, a_Stmt_While stmt );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_do_while( Chuck_Emitter * emit, a_Stmt_While stmt );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_until( Chuck_Emitter * emit, a_Stmt_Until stmt );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_do_until( Chuck_Emitter * emit, a_Stmt_Until stmt );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_break( Chuck_Emitter * emit, a_Stmt_Break br );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_continue( Chuck_Emitter * emit, a_Stmt_Continue cont );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_return( Chuck_Emitter * emit, a_Stmt_Return stmt );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_switch( Chuck_Emitter * emit, a_Stmt_Switch stmt );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp( Chuck_Emitter * emit, a_Exp exp );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_binary( Chuck_Emitter * emit, a_Exp_Binary binary );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_op( Chuck_Emitter * emit, ae_Operator op, a_Exp lhs, a_Exp rhs );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_op_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_op_unchuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_unary( Chuck_Emitter * emit, a_Exp_Unary unary );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_primary( Chuck_Emitter * emit, a_Exp_Primary exp );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_cast( Chuck_Emitter * emit, a_Exp_Cast cast );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_postfix( Chuck_Emitter * emit, a_Exp_Postfix postfix );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_dur( Chuck_Emitter * emit, a_Exp_Dur dur );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_array( Chuck_Emitter * emit, a_Exp_Array array );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_func_call( Chuck_Emitter * emit, a_Exp_Func_Call func_call );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_dot_member( Chuck_Emitter * emit, a_Exp_Dot_Member member );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_if( Chuck_Emitter * emit, a_Exp_If exp_if );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_decl( Chuck_Emitter * emit, a_Exp_Decl decl );




//-----------------------------------------------------------------------------
// name:
// desc: ...
//-----------------------------------------------------------------------------
t_CKTYPE emit_engine_emit_exp_namespace( Chuck_Emitter * emit, a_Exp_Namespace name_space );




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
