/* A Bison parser, made by GNU Bison 1.875b.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     STRING_LIT = 259,
     NUM = 260,
     FLOAT = 261,
     POUND = 262,
     COMMA = 263,
     COLON = 264,
     SEMICOLON = 265,
     LPAREN = 266,
     RPAREN = 267,
     LBRACK = 268,
     RBRACK = 269,
     LBRACE = 270,
     RBRACE = 271,
     DOT = 272,
     PLUS = 273,
     MINUS = 274,
     TIMES = 275,
     DIVIDE = 276,
     PERCENT = 277,
     EQ = 278,
     NEQ = 279,
     LT = 280,
     LE = 281,
     GT = 282,
     GE = 283,
     AND = 284,
     OR = 285,
     ASSIGN = 286,
     IF = 287,
     THEN = 288,
     ELSE = 289,
     WHILE = 290,
     FOR = 291,
     DO = 292,
     BREAK = 293,
     NULL_TOK = 294,
     FUNCTION = 295,
     RETURN = 296,
     QUESTION = 297,
     EXCLAMATION = 298,
     S_OR = 299,
     S_AND = 300,
     S_XOR = 301,
     PLUSPLUS = 302,
     MINUSMINUS = 303,
     SIMULT = 304,
     PATTERN = 305,
     CODE = 306,
     TRANSPORT = 307,
     HOST = 308,
     TIME = 309,
     WHENEVER = 310,
     NEXT = 311,
     UNTIL = 312,
     EVERY = 313,
     BEFORE = 314,
     AFTER = 315,
     AT = 316,
     AT_SYM = 317,
     ATAT_SYM = 318,
     NEW = 319,
     SIZEOF = 320,
     TYPEOF = 321,
     SAME = 322,
     PLUS_CHUCK = 323,
     MINUS_CHUCK = 324,
     TIMES_CHUCK = 325,
     DIVIDE_CHUCK = 326,
     S_AND_CHUCK = 327,
     S_OR_CHUCK = 328,
     S_XOR_CHUCK = 329,
     SHIFT_RIGHT_CHUCK = 330,
     SHIFT_LEFT_CHUCK = 331,
     PERCENT_CHUCK = 332,
     SHIFT_RIGHT = 333,
     SHIFT_LEFT = 334,
     TILDA = 335,
     CHUCK = 336,
     COLONCOLON = 337,
     S_CHUCK = 338,
     AT_CHUCK = 339,
     LEFT_S_CHUCK = 340,
     UNCHUCK = 341,
     CLASS = 342,
     INTERFACE = 343,
     EXTENDS = 344,
     IMPLEMENTS = 345,
     PUBLIC = 346,
     PROTECTED = 347,
     PRIVATE = 348,
     STATIC = 349,
     CONST = 350,
     SPORK = 351,
     L_NSPC = 352,
     R_NSPC = 353
   };
#endif
#define ID 258
#define STRING_LIT 259
#define NUM 260
#define FLOAT 261
#define POUND 262
#define COMMA 263
#define COLON 264
#define SEMICOLON 265
#define LPAREN 266
#define RPAREN 267
#define LBRACK 268
#define RBRACK 269
#define LBRACE 270
#define RBRACE 271
#define DOT 272
#define PLUS 273
#define MINUS 274
#define TIMES 275
#define DIVIDE 276
#define PERCENT 277
#define EQ 278
#define NEQ 279
#define LT 280
#define LE 281
#define GT 282
#define GE 283
#define AND 284
#define OR 285
#define ASSIGN 286
#define IF 287
#define THEN 288
#define ELSE 289
#define WHILE 290
#define FOR 291
#define DO 292
#define BREAK 293
#define NULL_TOK 294
#define FUNCTION 295
#define RETURN 296
#define QUESTION 297
#define EXCLAMATION 298
#define S_OR 299
#define S_AND 300
#define S_XOR 301
#define PLUSPLUS 302
#define MINUSMINUS 303
#define SIMULT 304
#define PATTERN 305
#define CODE 306
#define TRANSPORT 307
#define HOST 308
#define TIME 309
#define WHENEVER 310
#define NEXT 311
#define UNTIL 312
#define EVERY 313
#define BEFORE 314
#define AFTER 315
#define AT 316
#define AT_SYM 317
#define ATAT_SYM 318
#define NEW 319
#define SIZEOF 320
#define TYPEOF 321
#define SAME 322
#define PLUS_CHUCK 323
#define MINUS_CHUCK 324
#define TIMES_CHUCK 325
#define DIVIDE_CHUCK 326
#define S_AND_CHUCK 327
#define S_OR_CHUCK 328
#define S_XOR_CHUCK 329
#define SHIFT_RIGHT_CHUCK 330
#define SHIFT_LEFT_CHUCK 331
#define PERCENT_CHUCK 332
#define SHIFT_RIGHT 333
#define SHIFT_LEFT 334
#define TILDA 335
#define CHUCK 336
#define COLONCOLON 337
#define S_CHUCK 338
#define AT_CHUCK 339
#define LEFT_S_CHUCK 340
#define UNCHUCK 341
#define CLASS 342
#define INTERFACE 343
#define EXTENDS 344
#define IMPLEMENTS 345
#define PUBLIC 346
#define PROTECTED 347
#define PRIVATE 348
#define STATIC 349
#define CONST 350
#define SPORK 351
#define L_NSPC 352
#define R_NSPC 353




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 59 "chuck.y"
typedef union YYSTYPE {
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
} YYSTYPE;
/* Line 1252 of yacc.c.  */
#line 256 "chuck.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



