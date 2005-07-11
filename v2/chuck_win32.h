/* A Bison parser, made by GNU Bison 1.875c.  */

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
     CONTINUE = 294,
     NULL_TOK = 295,
     FUNCTION = 296,
     RETURN = 297,
     QUESTION = 298,
     EXCLAMATION = 299,
     S_OR = 300,
     S_AND = 301,
     S_XOR = 302,
     PLUSPLUS = 303,
     MINUSMINUS = 304,
     DOLLAR = 305,
     SIMULT = 306,
     PATTERN = 307,
     CODE = 308,
     TRANSPORT = 309,
     HOST = 310,
     TIME = 311,
     WHENEVER = 312,
     NEXT = 313,
     UNTIL = 314,
     EVERY = 315,
     BEFORE = 316,
     AFTER = 317,
     AT = 318,
     AT_SYM = 319,
     ATAT_SYM = 320,
     NEW = 321,
     SIZEOF = 322,
     TYPEOF = 323,
     SAME = 324,
     PLUS_CHUCK = 325,
     MINUS_CHUCK = 326,
     TIMES_CHUCK = 327,
     DIVIDE_CHUCK = 328,
     S_AND_CHUCK = 329,
     S_OR_CHUCK = 330,
     S_XOR_CHUCK = 331,
     SHIFT_RIGHT_CHUCK = 332,
     SHIFT_LEFT_CHUCK = 333,
     PERCENT_CHUCK = 334,
     SHIFT_RIGHT = 335,
     SHIFT_LEFT = 336,
     TILDA = 337,
     CHUCK = 338,
     COLONCOLON = 339,
     S_CHUCK = 340,
     AT_CHUCK = 341,
     LEFT_S_CHUCK = 342,
     UNCHUCK = 343,
     CLASS = 344,
     INTERFACE = 345,
     EXTENDS = 346,
     IMPLEMENTS = 347,
     PUBLIC = 348,
     PROTECTED = 349,
     PRIVATE = 350,
     STATIC = 351,
     ABSTRACT = 352,
     CONST = 353,
     SPORK = 354,
     L_HACK = 355,
     R_HACK = 356
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
#define CONTINUE 294
#define NULL_TOK 295
#define FUNCTION 296
#define RETURN 297
#define QUESTION 298
#define EXCLAMATION 299
#define S_OR 300
#define S_AND 301
#define S_XOR 302
#define PLUSPLUS 303
#define MINUSMINUS 304
#define DOLLAR 305
#define SIMULT 306
#define PATTERN 307
#define CODE 308
#define TRANSPORT 309
#define HOST 310
#define TIME 311
#define WHENEVER 312
#define NEXT 313
#define UNTIL 314
#define EVERY 315
#define BEFORE 316
#define AFTER 317
#define AT 318
#define AT_SYM 319
#define ATAT_SYM 320
#define NEW 321
#define SIZEOF 322
#define TYPEOF 323
#define SAME 324
#define PLUS_CHUCK 325
#define MINUS_CHUCK 326
#define TIMES_CHUCK 327
#define DIVIDE_CHUCK 328
#define S_AND_CHUCK 329
#define S_OR_CHUCK 330
#define S_XOR_CHUCK 331
#define SHIFT_RIGHT_CHUCK 332
#define SHIFT_LEFT_CHUCK 333
#define PERCENT_CHUCK 334
#define SHIFT_RIGHT 335
#define SHIFT_LEFT 336
#define TILDA 337
#define CHUCK 338
#define COLONCOLON 339
#define S_CHUCK 340
#define AT_CHUCK 341
#define LEFT_S_CHUCK 342
#define UNCHUCK 343
#define CLASS 344
#define INTERFACE 345
#define EXTENDS 346
#define IMPLEMENTS 347
#define PUBLIC 348
#define PROTECTED 349
#define PRIVATE 350
#define STATIC 351
#define ABSTRACT 352
#define CONST 353
#define SPORK 354
#define L_HACK 355
#define R_HACK 356




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
/* Line 1268 of yacc.c.  */
#line 262 "chuck.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



