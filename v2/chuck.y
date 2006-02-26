%{

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
// file: chuck.tab.c
// desc: chuck parser
//
// author: Ge Wang (gewang@cs.princeton.edu) - generated by yacc
//         Perry R. Cook (prc@cs.princeton.edu)
//
// based in part on the ansi C grammar by Jeff Lee, maintained by Jutta Degener
//
// date: Summer 2002
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chuck_utils.h"
#include "chuck_errmsg.h"
#include "chuck_absyn.h"

// function
int yylex( void );

void yyerror( char *s )
{
    EM_error( EM_tokPos, "%s", s );
}

a_Program g_program = NULL;

%}


%union
{
    int pos;
    int ival;
    double fval;
    c_str sval;
    
    a_Program program;
    a_Section program_section;
    a_Stmt_List stmt_list;
    a_Class_Def class_def;
    a_Class_Ext class_ext;
    a_Class_Body class_body;
    a_Stmt stmt;
    a_Exp exp;
    a_Func_Def func_def;
    a_Var_Decl_List var_decl_list;
    a_Var_Decl var_decl;
    a_Type_Decl type_decl;
    a_Arg_List arg_list;
    a_Id_List id_list;
    a_Array_Sub array_sub;
};

// expect 35 shift/reduce conflicts
%expect 35

%token <sval> ID STRING_LIT
%token <ival> NUM
%token <fval> FLOAT

%token
  POUND COMMA COLON SEMICOLON LPAREN RPAREN
  LBRACK RBRACK LBRACE RBRACE DOT
  PLUS MINUS TIMES DIVIDE PERCENT
  EQ NEQ LT LE GT GE AND OR ASSIGN
  IF THEN ELSE WHILE FOR DO LOOP
  BREAK CONTINUE NULL_TOK FUNCTION RETURN
  QUESTION EXCLAMATION S_OR S_AND S_XOR
  PLUSPLUS MINUSMINUS DOLLAR
  SIMULT PATTERN CODE TRANSPORT HOST
  TIME WHENEVER NEXT UNTIL EVERY BEFORE
  AFTER AT AT_SYM ATAT_SYM NEW SIZEOF TYPEOF
  SAME PLUS_CHUCK MINUS_CHUCK TIMES_CHUCK
  DIVIDE_CHUCK S_AND_CHUCK S_OR_CHUCK
  S_XOR_CHUCK SHIFT_RIGHT_CHUCK
  SHIFT_LEFT_CHUCK PERCENT_CHUCK
  SHIFT_RIGHT SHIFT_LEFT TILDA CHUCK
  COLONCOLON S_CHUCK AT_CHUCK LEFT_S_CHUCK
  UNCHUCK CLASS INTERFACE EXTENDS IMPLEMENTS
  PUBLIC PROTECTED PRIVATE STATIC ABSTRACT CONST 
  SPORK L_HACK R_HACK


%type <program> program
%type <program_section> program_section
%type <stmt> code_segment
%type <func_def> function_definition
%type <class_def> class_definition
%type <class_body> class_body
%type <class_body> class_body2
%type <class_ext> class_ext
%type <ival> class_decl 
%type <class_ext> iface_ext
%type <program_section> class_section
%type <stmt_list> statement_list
%type <stmt> statement
//%type <stmt_label> label_statement
%type <stmt> loop_statement
%type <stmt> selection_statement
%type <stmt> jump_statement
%type <stmt> expression_statement
%type <exp> expression
%type <exp> chuck_expression
%type <exp> conditional_expression
%type <exp> logical_or_expression
%type <exp> logical_and_expression
%type <exp> inclusive_or_expression
%type <exp> exclusive_or_expression
%type <exp> and_expression
%type <exp> equality_expression
%type <exp> relational_expression
%type <exp> shift_expression
%type <exp> additive_expression
%type <exp> multiplicative_expression
%type <exp> tilda_expression
%type <exp> cast_expression
%type <exp> unary_expression
%type <exp> dur_expression
%type <exp> postfix_expression
%type <exp> primary_expression
%type <exp> decl_expression
%type <ival> unary_operator
%type <ival> chuck_operator
%type <var_decl_list> var_decl_list
%type <var_decl> var_decl
%type <type_decl> type_decl_a
%type <type_decl> type_decl_b
%type <type_decl> type_decl
%type <type_decl> type_decl2
%type <ival> function_decl
%type <ival> static_decl
%type <arg_list> arg_list
%type <id_list> id_list
%type <id_list> id_dot
%type <array_sub> array_exp
%type <array_sub> array_empty

%start program

%%

program
        : program_section                   { $$ = g_program = new_program( $1, EM_lineNum ); }
        | program_section program           { $$ = g_program = prepend_program( $1, $2, EM_lineNum ); }
        ;
        
program_section
        : statement_list                    { $$ = new_section_stmt( $1, EM_lineNum ); }
        | function_definition               { $$ = new_section_func_def( $1, EM_lineNum ); }
        | class_definition                  { $$ = new_section_class_def( $1, EM_lineNum ); }
        ;

class_definition
        : class_decl CLASS id_list LBRACE class_body RBRACE
            { $$ = new_class_def( $1, $3, NULL, $5, EM_lineNum ); }
        | class_decl CLASS id_list class_ext LBRACE class_body RBRACE 
            { $$ = new_class_def( $1, $3, $4, $6, EM_lineNum ); }
        | class_decl INTERFACE id_list LBRACE class_body RBRACE
            { $$ = new_iface_def( $1, $3, NULL, $5, EM_lineNum ); }
        | class_decl INTERFACE id_list iface_ext LBRACE class_body RBRACE
            { $$ = new_iface_def( $1, $3, $4, $6, EM_lineNum ); }
        ;

class_ext
        : IMPLEMENTS id_list                { $$ = new_class_ext( NULL, $2, EM_lineNum ); }
        | IMPLEMENTS id_list EXTENDS id_dot { $$ = new_class_ext( $4, $2, EM_lineNum ); }
        | EXTENDS id_dot                    { $$ = new_class_ext( $2, NULL, EM_lineNum ); }
        | EXTENDS id_dot IMPLEMENTS id_list { $$ = new_class_ext( $2, $4, EM_lineNum ); }
        ;

class_body
        : class_body2                       { $$ = $1; }
		|                                   { $$ = NULL; }
        ;

class_body2
        : class_section                     { $$ = new_class_body( $1, EM_lineNum ); }
        | class_section class_body2         { $$ = prepend_class_body( $1, $2, EM_lineNum ); }
        ;


class_section
        : statement_list                    { $$ = new_section_stmt( $1, EM_lineNum ); }
        | function_definition               { $$ = new_section_func_def( $1, EM_lineNum ); }
        | class_definition                  { $$ = new_section_class_def( $1, EM_lineNum ); }
        ;
        
iface_ext
        : EXTENDS id_list                   { $$ = new_class_ext( NULL, $2, EM_lineNum ); }
        ;

id_list
        : ID                                { $$ = new_id_list( $1, EM_lineNum ); }
        | ID COMMA id_list                  { $$ = prepend_id_list( $1, $3, EM_lineNum ); }
        ;

id_dot
        : ID                                { $$ = new_id_list( $1, EM_lineNum ); }
        | ID DOT id_dot                     { $$ = prepend_id_list( $1, $3, EM_lineNum ); }
        ;

function_definition
        : function_decl static_decl type_decl2 ID LPAREN arg_list RPAREN code_segment
            { $$ = new_func_def( $1, $2, $3, $4, $6, $8, EM_lineNum ); }
        | function_decl static_decl type_decl2 ID LPAREN RPAREN code_segment
            { $$ = new_func_def( $1, $2, $3, $4, NULL, $7, EM_lineNum ); }
        | function_decl static_decl type_decl2 ID LPAREN arg_list RPAREN SEMICOLON
            { $$ = new_func_def( $1, $2, $3, $4, $6, NULL, EM_lineNum ); }
        | function_decl static_decl type_decl2 ID LPAREN RPAREN SEMICOLON
            { $$ = new_func_def( $1, $2, $3, $4, NULL, NULL, EM_lineNum ); }
        ;

class_decl
        : PUBLIC                            { $$ = ae_key_public; }
        | PRIVATE                           { $$ = ae_key_private; }
        |                                   { $$ = ae_key_private; }
        ;

function_decl
        : FUNCTION                          { $$ = ae_key_func; }
        | PUBLIC                            { $$ = ae_key_public; }
        | PROTECTED                         { $$ = ae_key_protected; }
        | PRIVATE                           { $$ = ae_key_private; }
        ;

static_decl
        : STATIC                            { $$ = ae_key_static; }
        | ABSTRACT                          { $$ = ae_key_abstract; }
        |                                   { $$ = ae_key_instance; }
        ;

type_decl_a
        : ID                                { $$ = new_type_decl( new_id_list( $1, EM_lineNum ), 0, EM_lineNum ); }
        | ID AT_SYM                         { $$ = new_type_decl( new_id_list( $1, EM_lineNum ), 1, EM_lineNum ); }
        ;

type_decl_b
        : LT id_dot GT                      { $$ = new_type_decl( $2, 0, EM_lineNum ); }
        | LT id_dot GT AT_SYM               { $$ = new_type_decl( $2, 1, EM_lineNum ); }
        ;

//type_decl_c
//        : LPAREN id_dot RPAREN              { $$ = new_type_decl( $3, 0, EM_lineNum ); }
//        // | LPAREN id_dot RPAREN AT_SYM       { $$ = new_type_decl( $1, 1, EM_lineNum ); }
//        ;

type_decl
        : type_decl_a                       { $$ = $1; }
        | type_decl_b                       { $$ = $1; }
        // | type_decl_c                       { $$ = $1; }
        ;

type_decl2
        : type_decl                         { $$ = $1; }
        | type_decl array_empty             { $$ = add_type_decl_array( $1, $2, EM_lineNum ); }
        ;

arg_list
        : type_decl var_decl                { $$ = new_arg_list( $1, $2, EM_lineNum ); }
        | type_decl var_decl COMMA arg_list { $$ = prepend_arg_list( $1, $2, $4, EM_lineNum ); }
        ;

statement_list
        : statement                         { $$ = new_stmt_list( $1, EM_lineNum ); }
        | statement statement_list          { $$ = prepend_stmt_list( $1, $2, EM_lineNum ); }
        ;
        
statement
        : expression_statement              { $$ = $1; }
        | loop_statement                    { $$ = $1; }
        | selection_statement               { $$ = $1; }
        | jump_statement                    { $$ = $1; }
        // | label_statement                   { }
        | code_segment                      { $$ = $1; }
        ;

jump_statement
        : RETURN SEMICOLON                  { $$ = new_stmt_from_return( NULL, EM_lineNum ); }
        | RETURN expression SEMICOLON       { $$ = new_stmt_from_return( $2, EM_lineNum ); }
        | BREAK SEMICOLON                   { $$ = new_stmt_from_break( EM_lineNum ); }
        | CONTINUE SEMICOLON                { $$ = new_stmt_from_continue( EM_lineNum ); }
        ;

selection_statement
        : IF LPAREN expression RPAREN statement
            { $$ = new_stmt_from_if( $3, $5, NULL, EM_lineNum ); }
        | IF LPAREN expression RPAREN statement ELSE statement
            { $$ = new_stmt_from_if( $3, $5, $7, EM_lineNum ); }
        ;
        
loop_statement
        : WHILE LPAREN expression RPAREN statement
            { $$ = new_stmt_from_while( $3, $5, EM_lineNum ); }
        | DO statement WHILE LPAREN expression RPAREN SEMICOLON
            { $$ = new_stmt_from_do_while( $5, $2, EM_lineNum ); }
        | FOR LPAREN expression_statement expression_statement RPAREN statement
            { $$ = new_stmt_from_for( $3, $4, NULL, $6, EM_lineNum ); }
        | FOR LPAREN expression_statement expression_statement expression RPAREN statement
            { $$ = new_stmt_from_for( $3, $4, $5, $7, EM_lineNum ); }
        | UNTIL LPAREN expression RPAREN statement
            { $$ = new_stmt_from_until( $3, $5, EM_lineNum ); }
        | DO statement UNTIL LPAREN expression RPAREN SEMICOLON
            { $$ = new_stmt_from_do_until( $5, $2, EM_lineNum ); }
        | LOOP LPAREN expression RPAREN statement
            { $$ = new_stmt_from_loop( $3, $5, EM_lineNum ); }
        ;

code_segment
        : LBRACE RBRACE                     { $$ = new_stmt_from_code( NULL, EM_lineNum ); }
        | LBRACE statement_list RBRACE      { $$ = new_stmt_from_code( $2, EM_lineNum ); }
        ;
        
expression_statement
        : SEMICOLON                         { $$ = NULL; }
        | expression SEMICOLON              { $$ = new_stmt_from_expression( $1, EM_lineNum ); }
        ;
        
expression
        : chuck_expression                  { $$ = $1; }
        | chuck_expression COMMA expression { $$ = prepend_expression( $1, $3, EM_lineNum ); }
        ;

chuck_expression
        : decl_expression                   { $$ = $1; }
        | chuck_expression chuck_operator decl_expression
            { $$ = new_exp_from_binary( $1, $2, $3, EM_lineNum ); }
        ;

array_exp
        : LBRACK expression RBRACK          { $$ = new_array_sub( $2, EM_lineNum ); }
        | LBRACK expression RBRACK array_exp
            { $$ = prepend_array_sub( $4, $2, EM_lineNum ); }
        ;

array_empty
        : LBRACK RBRACK                     { $$ = new_array_sub( NULL, EM_lineNum ); }
        | array_empty LBRACK RBRACK         { $$ = prepend_array_sub( $1, NULL, EM_lineNum ); }
        ;

decl_expression
        : conditional_expression            { $$ = $1; }
        | type_decl var_decl_list           { $$ = new_exp_decl( $1, $2, 0, EM_lineNum ); }
        | STATIC type_decl var_decl_list    { $$ = new_exp_decl( $2, $3, 1, EM_lineNum ); }
        | SAME var_decl_list                { $$ = new_exp_decl( NULL, $2, 0, EM_lineNum ); }
        | STATIC SAME var_decl_list         { $$ = new_exp_decl( NULL, $3, 1, EM_lineNum ); }
        ;

var_decl_list
        : var_decl                          { $$ = new_var_decl_list( $1, EM_lineNum ); }
        | var_decl COMMA var_decl_list      { $$ = prepend_var_decl_list( $1, $3, EM_lineNum ); }
        ;

var_decl
        : ID                                { $$ = new_var_decl( $1, NULL, EM_lineNum ); }
        | ID array_exp                      { $$ = new_var_decl( $1, $2, EM_lineNum ); }
        | ID array_empty                    { $$ = new_var_decl( $1, $2, EM_lineNum ); }
        ;

chuck_operator
        : CHUCK                             { $$ = ae_op_chuck; }
        | AT_CHUCK                          { $$ = ae_op_at_chuck; }
        | PLUS_CHUCK                        { $$ = ae_op_plus_chuck; }
        | MINUS_CHUCK                       { $$ = ae_op_minus_chuck; }
        | TIMES_CHUCK                       { $$ = ae_op_times_chuck; }
        | DIVIDE_CHUCK                      { $$ = ae_op_divide_chuck; }
        | SHIFT_RIGHT_CHUCK                 { $$ = ae_op_shift_right_chuck; }
        | SHIFT_LEFT_CHUCK                  { $$ = ae_op_shift_left_chuck; }
        | PERCENT_CHUCK                     { $$ = ae_op_percent_chuck; }
        | UNCHUCK                           { $$ = ae_op_unchuck; }
        | S_AND_CHUCK                       { $$ = ae_op_s_and_chuck; }
        | S_OR_CHUCK                        { $$ = ae_op_s_or_chuck; }
        | S_XOR_CHUCK                       { $$ = ae_op_s_xor_chuck; }
        ;

conditional_expression
        : logical_or_expression             { $$ = $1; }
        | logical_or_expression QUESTION expression COLON conditional_expression
            { $$ = new_exp_from_if( $1, $3, $5, EM_lineNum ); }
        ;

logical_or_expression
        : logical_and_expression            { $$ = $1; }
        | logical_or_expression OR logical_and_expression
            { $$ = new_exp_from_binary( $1, ae_op_or, $3, EM_lineNum ); }
        ;

logical_and_expression
        : inclusive_or_expression           { $$ = $1; }
        | logical_and_expression AND inclusive_or_expression
            { $$ = new_exp_from_binary( $1, ae_op_and, $3, EM_lineNum ); }
        ;
        
inclusive_or_expression
        : exclusive_or_expression           { $$ = $1; }
        | inclusive_or_expression S_OR exclusive_or_expression
            { $$ = new_exp_from_binary( $1, ae_op_s_or, $3, EM_lineNum ); }
        ;
        
exclusive_or_expression
        : and_expression                    { $$ = $1; }
        | exclusive_or_expression S_XOR and_expression
            { $$ = new_exp_from_binary( $1, ae_op_s_xor, $3, EM_lineNum ); }
        ;
        
and_expression
        : equality_expression               { $$ = $1; }
        | and_expression S_AND equality_expression
            { $$ = new_exp_from_binary( $1, ae_op_s_and, $3, EM_lineNum ); }
        ;
        
equality_expression
        : relational_expression             { $$ = $1; }
        | equality_expression EQ relational_expression
            { $$ = new_exp_from_binary( $1, ae_op_eq, $3, EM_lineNum ); }
        | equality_expression NEQ relational_expression
            { $$ = new_exp_from_binary( $1, ae_op_neq, $3, EM_lineNum ); }
        ;

relational_expression
        : shift_expression                  { $$ = $1; }
        | relational_expression LT shift_expression
            { $$ = new_exp_from_binary( $1, ae_op_lt, $3, EM_lineNum ); }
        | relational_expression GT shift_expression
            { $$ = new_exp_from_binary( $1, ae_op_gt, $3, EM_lineNum ); }
        | relational_expression LE shift_expression
            { $$ = new_exp_from_binary( $1, ae_op_le, $3, EM_lineNum ); }
        | relational_expression GE shift_expression
            { $$ = new_exp_from_binary( $1, ae_op_ge, $3, EM_lineNum ); }
        ;

shift_expression
        : additive_expression               { $$ = $1; }
        | shift_expression SHIFT_LEFT additive_expression
            { $$ = new_exp_from_binary( $1, ae_op_shift_left, $3, EM_lineNum ); }
        | shift_expression SHIFT_RIGHT additive_expression
            { $$ = new_exp_from_binary( $1, ae_op_shift_right, $3, EM_lineNum ); }
        ;

additive_expression
        : multiplicative_expression          { $$ = $1; }
        | additive_expression PLUS multiplicative_expression
            { $$ = new_exp_from_binary( $1, ae_op_plus, $3, EM_lineNum ); }
        | additive_expression MINUS multiplicative_expression
            { $$ = new_exp_from_binary( $1, ae_op_minus, $3, EM_lineNum ); }
        ;

multiplicative_expression
        : tilda_expression                   { $$ = $1; }
        | multiplicative_expression TIMES tilda_expression
            { $$ = new_exp_from_binary( $1, ae_op_times, $3, EM_lineNum ); }
        | multiplicative_expression DIVIDE tilda_expression
            { $$ = new_exp_from_binary( $1, ae_op_divide, $3, EM_lineNum ); }
        | multiplicative_expression PERCENT tilda_expression
            { $$ = new_exp_from_binary( $1, ae_op_percent, $3, EM_lineNum ); }
        ;

tilda_expression
        : cast_expression                   { $$ = $1; }
        | tilda_expression TILDA cast_expression
            { $$ = new_exp_from_binary( $1, ae_op_tilda, $3, EM_lineNum ); }
        ;
        
cast_expression
        : unary_expression                  { $$ = $1; }
        | cast_expression DOLLAR type_decl
            { $$ = new_exp_from_cast( $3, $1, EM_lineNum ); }
        ;
        
unary_expression
        : dur_expression                    { $$ = $1; }
        | PLUSPLUS unary_expression
            { $$ = new_exp_from_unary( ae_op_plusplus, $2, EM_lineNum ); }
        | MINUSMINUS unary_expression
            { $$ = new_exp_from_unary( ae_op_minusminus, $2, EM_lineNum ); }
        | unary_operator unary_expression
            { $$ = new_exp_from_unary( $1, $2, EM_lineNum ); }
        | TYPEOF unary_expression
            { $$ = new_exp_from_unary( ae_op_typeof, $2, EM_lineNum ); }
        | SIZEOF unary_expression
            { $$ = new_exp_from_unary( ae_op_sizeof, $2, EM_lineNum ); }
        | NEW type_decl
            { $$ = new_exp_from_unary2( ae_op_new, $2, NULL, EM_lineNum ); }
        | NEW type_decl array_exp
            { $$ = new_exp_from_unary2( ae_op_new, $2, $3, EM_lineNum ); }
		| SPORK TILDA code_segment
		    { $$ = new_exp_from_unary3( ae_op_spork, $3, EM_lineNum ); }
        ;

unary_operator
        : PLUS                              { $$ = ae_op_plus; }
        | MINUS                             { $$ = ae_op_minus; }
        | TILDA                             { $$ = ae_op_tilda; }
        | EXCLAMATION                       { $$ = ae_op_exclamation; }
        | TIMES                             { $$ = ae_op_times; }
        | SPORK TILDA                       { $$ = ae_op_spork; }
        // | S_AND                             { $$ = ae_op_s_and; }
        ;

dur_expression
        : postfix_expression
        | dur_expression COLONCOLON postfix_expression
            { $$ = new_exp_from_dur( $1, $3, EM_lineNum ); }
        ;
            
postfix_expression
        : primary_expression                { $$ = $1; }
        | postfix_expression array_exp
            { $$ = new_exp_from_array( $1, $2, EM_lineNum ); }
        | postfix_expression LPAREN RPAREN
            { $$ = new_exp_from_func_call( $1, NULL, EM_lineNum ); }
        | postfix_expression LPAREN expression RPAREN
            { $$ = new_exp_from_func_call( $1, $3, EM_lineNum ); }
        | postfix_expression DOT ID
            { $$ = new_exp_from_member_dot( $1, $3, EM_lineNum ); }
        | postfix_expression PLUSPLUS
            { $$ = new_exp_from_postfix( $1, ae_op_plusplus, EM_lineNum ); }
        | postfix_expression MINUSMINUS
            { $$ = new_exp_from_postfix( $1, ae_op_minusminus, EM_lineNum ); }
        ;

primary_expression
        : ID                                { $$ = new_exp_from_id( $1, EM_lineNum ); }
        | NUM                               { $$ = new_exp_from_int( $1, EM_lineNum ); }
        | FLOAT                             { $$ = new_exp_from_float( $1, EM_lineNum ); }
        | STRING_LIT                        { $$ = new_exp_from_str( $1, EM_lineNum ); }
        | array_exp                         { $$ = new_exp_from_array_lit( $1, EM_lineNum ); }
        | L_HACK expression R_HACK          { $$ = new_exp_from_hack( $2, EM_lineNum ); }
        | LPAREN expression RPAREN          { $$ = $2; }
		| LPAREN RPAREN                     { $$ = new_exp_from_nil( EM_lineNum ); }
        ;
