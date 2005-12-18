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
     LOOP = 293,
     BREAK = 294,
     CONTINUE = 295,
     NULL_TOK = 296,
     FUNCTION = 297,
     RETURN = 298,
     QUESTION = 299,
     EXCLAMATION = 300,
     S_OR = 301,
     S_AND = 302,
     S_XOR = 303,
     PLUSPLUS = 304,
     MINUSMINUS = 305,
     DOLLAR = 306,
     SIMULT = 307,
     PATTERN = 308,
     CODE = 309,
     TRANSPORT = 310,
     HOST = 311,
     TIME = 312,
     WHENEVER = 313,
     NEXT = 314,
     UNTIL = 315,
     EVERY = 316,
     BEFORE = 317,
     AFTER = 318,
     AT = 319,
     AT_SYM = 320,
     ATAT_SYM = 321,
     NEW = 322,
     SIZEOF = 323,
     TYPEOF = 324,
     SAME = 325,
     PLUS_CHUCK = 326,
     MINUS_CHUCK = 327,
     TIMES_CHUCK = 328,
     DIVIDE_CHUCK = 329,
     S_AND_CHUCK = 330,
     S_OR_CHUCK = 331,
     S_XOR_CHUCK = 332,
     SHIFT_RIGHT_CHUCK = 333,
     SHIFT_LEFT_CHUCK = 334,
     PERCENT_CHUCK = 335,
     SHIFT_RIGHT = 336,
     SHIFT_LEFT = 337,
     TILDA = 338,
     CHUCK = 339,
     COLONCOLON = 340,
     S_CHUCK = 341,
     AT_CHUCK = 342,
     LEFT_S_CHUCK = 343,
     UNCHUCK = 344,
     CLASS = 345,
     INTERFACE = 346,
     EXTENDS = 347,
     IMPLEMENTS = 348,
     PUBLIC = 349,
     PROTECTED = 350,
     PRIVATE = 351,
     STATIC = 352,
     ABSTRACT = 353,
     CONST = 354,
     SPORK = 355,
     L_HACK = 356,
     R_HACK = 357
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
#define LOOP 293
#define BREAK 294
#define CONTINUE 295
#define NULL_TOK 296
#define FUNCTION 297
#define RETURN 298
#define QUESTION 299
#define EXCLAMATION 300
#define S_OR 301
#define S_AND 302
#define S_XOR 303
#define PLUSPLUS 304
#define MINUSMINUS 305
#define DOLLAR 306
#define SIMULT 307
#define PATTERN 308
#define CODE 309
#define TRANSPORT 310
#define HOST 311
#define TIME 312
#define WHENEVER 313
#define NEXT 314
#define UNTIL 315
#define EVERY 316
#define BEFORE 317
#define AFTER 318
#define AT 319
#define AT_SYM 320
#define ATAT_SYM 321
#define NEW 322
#define SIZEOF 323
#define TYPEOF 324
#define SAME 325
#define PLUS_CHUCK 326
#define MINUS_CHUCK 327
#define TIMES_CHUCK 328
#define DIVIDE_CHUCK 329
#define S_AND_CHUCK 330
#define S_OR_CHUCK 331
#define S_XOR_CHUCK 332
#define SHIFT_RIGHT_CHUCK 333
#define SHIFT_LEFT_CHUCK 334
#define PERCENT_CHUCK 335
#define SHIFT_RIGHT 336
#define SHIFT_LEFT 337
#define TILDA 338
#define CHUCK 339
#define COLONCOLON 340
#define S_CHUCK 341
#define AT_CHUCK 342
#define LEFT_S_CHUCK 343
#define UNCHUCK 344
#define CLASS 345
#define INTERFACE 346
#define EXTENDS 347
#define IMPLEMENTS 348
#define PUBLIC 349
#define PROTECTED 350
#define PRIVATE 351
#define STATIC 352
#define ABSTRACT 353
#define CONST 354
#define SPORK 355
#define L_HACK 356
#define R_HACK 357




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
#line 264 "chuck.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



