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





//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
t_CKBOOL emit_engine_emit_prog( Chuck_Emitter * emit, a_Program prog );
t_CKBOOL emit_engine_emit_stmt_list( Chuck_Emitter * emit, a_Stmt_List list );
t_CKBOOL emit_engine_emit_stmt( Chuck_Emitter * emit, a_Stmt stmt );
t_CKBOOL emit_engine_emit_if( Chuck_Emitter * emit, a_Stmt_If stmt );
t_CKBOOL emit_engine_emit_for( Chuck_Emitter * emit, a_Stmt_For stmt );
t_CKBOOL emit_engine_emit_while( Chuck_Emitter * emit, a_Stmt_While stmt );
t_CKBOOL emit_engine_emit_until( Chuck_Emitter * emit, a_Stmt_Until stmt );
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
t_CKTYPE emit_engine_emit_primary( Chuck_Emitter * emit, a_Exp_Primary exp );
t_CKTYPE emit_engine_emit_exp_cast( Chuck_Emitter * emit, a_Exp_Cast cast );
t_CKTYPE emit_engine_emit_exp_postfix( Chuck_Emitter * emit, a_Exp_Postfix postfix );
t_CKTYPE emit_engine_emit_exp_dur( Chuck_Emitter * emit, a_Exp_Dur dur );
t_CKTYPE emit_engine_emit_exp_array( Chuck_Emitter * emit, a_Exp_Array array );
t_CKTYPE emit_engine_emit_exp_func_call( Chuck_Emitter * emit, a_Exp_Func_Call func_call );
t_CKTYPE emit_engine_emit_exp_dot_member( Chuck_Emitter * emit, a_Exp_Dot_Member member );
t_CKTYPE emit_engine_emit_exp_if( Chuck_Emitter * emit, a_Exp_If exp_if );
t_CKTYPE emit_engine_emit_exp_decl( Chuck_Emitter * emit, a_Exp_Decl decl );
t_CKTYPE emit_engine_emit_exp_namespace( Chuck_Emitter * emit, a_Exp_Namespace name_space );
t_CKBOOL emit_engine_emit_cast_valid( Chuck_Emitter * emit, t_CKTYPE to, t_CKTYPE from );
t_CKBOOL emit_engine_emit_code_segment( Chuck_Emitter * emit, a_Stmt_Code stmt, t_CKBOOL push = TRUE );
t_CKBOOL emit_engine_emit_func_def( Chuck_Emitter * emit, a_Func_Def func_def );
t_CKBOOL emit_engine_emit_class_def( Chuck_Emitter * emit, a_Class_Def class_def );
t_CKBOOL emit_engine_emit_func_def_import( Chuck_Emitter * emit, a_Func_Def func_def );
t_CKBOOL emit_engine_emit_ugen_def_import( Chuck_Emitter * emit, Chuck_UGen_Info * ugen );
t_CKBOOL type_engine_add_dll( Chuck_Emitter * emit, Chuck_DLL * dll, const char * name );
t_CKBOOL emit_engine_emit_value_import( Chuck_Emitter * emit, const string & name, 
										 const string & type, void * addr );

t_CKBOOL emit_engine_emit_prog( Chuck_Emitter * emit, a_Program prog );
t_CKBOOL emit_engine_emit_stmt_list( Chuck_Emitter * emit, a_Stmt_List list );
t_CKBOOL emit_engine_emit_stmt( Chuck_Emitter * emit, a_Stmt stmt, t_CKBOOL pop = TRUE );
t_CKBOOL emit_engine_emit_code_segment( Chuck_Emitter * emit, a_Stmt_Code stmt );
t_CKBOOL emit_engine_emit_switch( Chuck_Emitter * emit, a_Stmt_Switch stmt );
t_CKBOOL emit_engine_emit_while( Chuck_Emitter * emit, a_Stmt_While stmt );
t_CKBOOL emit_engine_emit_do_while( Chuck_Emitter * emit, a_Stmt_While stmt );
t_CKBOOL emit_engine_emit_until( Chuck_Emitter * emit, a_Stmt_Until stmt );
t_CKBOOL emit_engine_emit_do_until( Chuck_Emitter * emit, a_Stmt_Until stmt );
t_CKBOOL emit_engine_emit_for( Chuck_Emitter * emit, a_Stmt_For stmt );
t_CKBOOL emit_engine_emit_if( Chuck_Emitter * emit, a_Stmt_If stmt );
t_CKBOOL emit_engine_emit_return( Chuck_Emitter * emit, a_Stmt_Return stmt );
t_CKBOOL emit_engine_emit_exp( Chuck_Emitter * emit, a_Exp exp );
t_CKBOOL emit_engine_emit_exp_binary( Chuck_Emitter * emit, a_Exp_Binary exp );
t_CKBOOL emit_engine_emit_exp_unary( Chuck_Emitter * emit, a_Exp_Unary exp );
t_CKBOOL emit_engine_emit_exp_cast( Chuck_Emitter * emit, a_Exp_Cast exp );
t_CKBOOL emit_engine_emit_exp_postfix( Chuck_Emitter * emit, a_Exp_Postfix exp );
t_CKBOOL emit_engine_emit_exp_dur( Chuck_Emitter * emit, a_Exp_Dur exp );
t_CKBOOL emit_engine_emit_exp_primary( Chuck_Emitter * emit, a_Exp_Primary exp );
t_CKBOOL emit_engine_emit_exp_array( Chuck_Emitter * emit, a_Exp_Array exp );
t_CKBOOL emit_engine_emit_exp_func_call( Chuck_Emitter * emit, a_Exp_Func_Call exp, t_CKBOOL spork = FALSE );
t_CKBOOL emit_engine_emit_exp_dot_member( Chuck_Emitter * emit, a_Exp_Dot_Member exp );
t_CKBOOL emit_engine_emit_exp_if( Chuck_Emitter * emit, a_Exp_If exp );
t_CKBOOL emit_engine_emit_exp_decl( Chuck_Emitter * emit, a_Exp_Decl exp );
t_CKBOOL emit_engine_emit_exp_namespace( Chuck_Emitter * emit, a_Exp_Namespace nspc );
t_CKBOOL emit_engine_emit_op( Chuck_Emitter * emit, ae_Operator op, a_Exp lhs, a_Exp rhs );
t_CKBOOL emit_engine_emit_chuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );
t_CKBOOL emit_engine_emit_unchuck( Chuck_Emitter * emit, a_Exp lhs, a_Exp rhs );
t_CKBOOL emit_engine_emit_func_def( Chuck_Emitter * emit, a_Func_Def func_def );
t_CKBOOL emit_engine_emit_spork( Chuck_Emitter * emit, a_Exp_Func_Call exp );
t_CKBOOL emit_engine_emit_symbol( Chuck_Emitter * emit, S_Symbol symbol, 
                                  t_CKBOOL offset, int linepos );