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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "chuck.y"


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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

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
/* Line 191 of yacc.c.  */
#line 359 "chuck.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 371 "chuck.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  107
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1128

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  103
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  53
/* YYNRULES -- Number of rules. */
#define YYNRULES  171
/* YYNRULES -- Number of states. */
#define YYNSTATES  295

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   357

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    21,    29,
      36,    44,    47,    52,    55,    60,    62,    63,    65,    68,
      70,    72,    74,    77,    79,    83,    85,    89,    98,   106,
     115,   123,   125,   127,   128,   130,   132,   134,   136,   138,
     140,   141,   143,   146,   150,   155,   157,   159,   161,   164,
     167,   172,   174,   177,   179,   181,   183,   185,   187,   190,
     194,   197,   200,   206,   214,   220,   228,   235,   243,   249,
     257,   263,   266,   270,   272,   275,   277,   281,   283,   287,
     291,   296,   299,   303,   305,   308,   312,   315,   319,   321,
     325,   327,   330,   333,   335,   337,   339,   341,   343,   345,
     347,   349,   351,   353,   355,   357,   359,   361,   367,   369,
     373,   375,   379,   381,   385,   387,   391,   393,   397,   399,
     403,   407,   409,   413,   417,   421,   425,   427,   431,   435,
     437,   441,   445,   447,   451,   455,   459,   461,   465,   467,
     471,   473,   476,   479,   482,   485,   488,   491,   495,   499,
     501,   503,   505,   507,   509,   512,   514,   518,   520,   523,
     527,   532,   536,   539,   542,   544,   546,   548,   550,   552,
     556,   560
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     104,     0,    -1,   105,    -1,   105,   104,    -1,   123,    -1,
     114,    -1,   106,    -1,   115,    90,   112,    15,   108,    16,
      -1,   115,    90,   112,   107,    15,   108,    16,    -1,   115,
      91,   112,    15,   108,    16,    -1,   115,    91,   112,   111,
      15,   108,    16,    -1,    93,   112,    -1,    93,   112,    92,
     113,    -1,    92,   113,    -1,    92,   113,    93,   112,    -1,
     109,    -1,    -1,   110,    -1,   110,   109,    -1,   123,    -1,
     114,    -1,   106,    -1,    92,   112,    -1,     3,    -1,     3,
       8,   112,    -1,     3,    -1,     3,    17,   113,    -1,   116,
     117,   121,     3,    11,   122,    12,   128,    -1,   116,   117,
     121,     3,    11,    12,   128,    -1,   116,   117,   121,     3,
      11,   122,    12,    10,    -1,   116,   117,   121,     3,    11,
      12,    10,    -1,    94,    -1,    96,    -1,    -1,    42,    -1,
      94,    -1,    95,    -1,    96,    -1,    97,    -1,    98,    -1,
      -1,     3,    -1,     3,    65,    -1,    25,   113,    27,    -1,
      25,   113,    27,    65,    -1,   118,    -1,   119,    -1,   120,
      -1,   120,   133,    -1,   120,   136,    -1,   120,   136,     8,
     122,    -1,   124,    -1,   124,   123,    -1,   129,    -1,   127,
      -1,   126,    -1,   125,    -1,   128,    -1,    43,    10,    -1,
      43,   130,    10,    -1,    39,    10,    -1,    40,    10,    -1,
      32,    11,   130,    12,   124,    -1,    32,    11,   130,    12,
     124,    34,   124,    -1,    35,    11,   130,    12,   124,    -1,
      37,   124,    35,    11,   130,    12,    10,    -1,    36,    11,
     129,   129,    12,   124,    -1,    36,    11,   129,   129,   130,
      12,   124,    -1,    60,    11,   130,    12,   124,    -1,    37,
     124,    60,    11,   130,    12,    10,    -1,    38,    11,   130,
      12,   124,    -1,    15,    16,    -1,    15,   123,    16,    -1,
      10,    -1,   130,    10,    -1,   131,    -1,   131,     8,   130,
      -1,   134,    -1,   131,   137,   134,    -1,    13,   130,    14,
      -1,    13,   130,    14,   132,    -1,    13,    14,    -1,   133,
      13,    14,    -1,   138,    -1,   120,   135,    -1,    97,   120,
     135,    -1,    70,   135,    -1,    97,    70,   135,    -1,   136,
      -1,   136,     8,   135,    -1,     3,    -1,     3,   132,    -1,
       3,   133,    -1,    84,    -1,    87,    -1,    71,    -1,    72,
      -1,    73,    -1,    74,    -1,    78,    -1,    79,    -1,    80,
      -1,    89,    -1,    75,    -1,    76,    -1,    77,    -1,   139,
      -1,   139,    44,   130,     9,   138,    -1,   140,    -1,   139,
      30,   140,    -1,   141,    -1,   140,    29,   141,    -1,   142,
      -1,   141,    46,   142,    -1,   143,    -1,   142,    48,   143,
      -1,   144,    -1,   143,    47,   144,    -1,   145,    -1,   144,
      23,   145,    -1,   144,    24,   145,    -1,   146,    -1,   145,
      25,   146,    -1,   145,    27,   146,    -1,   145,    26,   146,
      -1,   145,    28,   146,    -1,   147,    -1,   146,    82,   147,
      -1,   146,    81,   147,    -1,   148,    -1,   147,    18,   148,
      -1,   147,    19,   148,    -1,   149,    -1,   148,    20,   149,
      -1,   148,    21,   149,    -1,   148,    22,   149,    -1,   150,
      -1,   149,    83,   150,    -1,   151,    -1,   150,    51,   120,
      -1,   153,    -1,    49,   151,    -1,    50,   151,    -1,   152,
     151,    -1,    69,   151,    -1,    68,   151,    -1,    67,   120,
      -1,    67,   120,   132,    -1,   100,    83,   128,    -1,    18,
      -1,    19,    -1,    83,    -1,    45,    -1,    20,    -1,   100,
      83,    -1,   154,    -1,   153,    85,   154,    -1,   155,    -1,
     154,   132,    -1,   154,    11,    12,    -1,   154,    11,   130,
      12,    -1,   154,    17,     3,    -1,   154,    49,    -1,   154,
      50,    -1,     3,    -1,     5,    -1,     6,    -1,     4,    -1,
     132,    -1,   101,   130,   102,    -1,    11,   130,    12,    -1,
      11,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   171,   171,   172,   176,   177,   178,   182,   184,   186,
     188,   193,   194,   195,   196,   200,   201,   205,   206,   211,
     212,   213,   217,   221,   222,   226,   227,   231,   233,   235,
     237,   242,   243,   244,   248,   249,   250,   251,   255,   256,
     257,   261,   262,   266,   267,   276,   277,   282,   283,   287,
     288,   292,   293,   297,   298,   299,   300,   302,   306,   307,
     308,   309,   313,   315,   320,   322,   324,   326,   328,   330,
     332,   337,   338,   342,   343,   347,   348,   352,   353,   358,
     359,   364,   365,   369,   370,   371,   372,   373,   377,   378,
     382,   383,   384,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   404,   405,   410,   411,
     416,   417,   422,   423,   428,   429,   434,   435,   440,   441,
     443,   448,   449,   451,   453,   455,   460,   461,   463,   468,
     469,   471,   476,   477,   479,   481,   486,   487,   492,   493,
     498,   499,   501,   503,   505,   507,   509,   511,   513,   518,
     519,   520,   521,   522,   523,   528,   529,   534,   535,   537,
     539,   541,   543,   545,   550,   551,   552,   553,   554,   555,
     556,   557
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "STRING_LIT", "NUM", "FLOAT", 
  "POUND", "COMMA", "COLON", "SEMICOLON", "LPAREN", "RPAREN", "LBRACK", 
  "RBRACK", "LBRACE", "RBRACE", "DOT", "PLUS", "MINUS", "TIMES", "DIVIDE", 
  "PERCENT", "EQ", "NEQ", "LT", "LE", "GT", "GE", "AND", "OR", "ASSIGN", 
  "IF", "THEN", "ELSE", "WHILE", "FOR", "DO", "LOOP", "BREAK", "CONTINUE", 
  "NULL_TOK", "FUNCTION", "RETURN", "QUESTION", "EXCLAMATION", "S_OR", 
  "S_AND", "S_XOR", "PLUSPLUS", "MINUSMINUS", "DOLLAR", "SIMULT", 
  "PATTERN", "CODE", "TRANSPORT", "HOST", "TIME", "WHENEVER", "NEXT", 
  "UNTIL", "EVERY", "BEFORE", "AFTER", "AT", "AT_SYM", "ATAT_SYM", "NEW", 
  "SIZEOF", "TYPEOF", "SAME", "PLUS_CHUCK", "MINUS_CHUCK", "TIMES_CHUCK", 
  "DIVIDE_CHUCK", "S_AND_CHUCK", "S_OR_CHUCK", "S_XOR_CHUCK", 
  "SHIFT_RIGHT_CHUCK", "SHIFT_LEFT_CHUCK", "PERCENT_CHUCK", "SHIFT_RIGHT", 
  "SHIFT_LEFT", "TILDA", "CHUCK", "COLONCOLON", "S_CHUCK", "AT_CHUCK", 
  "LEFT_S_CHUCK", "UNCHUCK", "CLASS", "INTERFACE", "EXTENDS", 
  "IMPLEMENTS", "PUBLIC", "PROTECTED", "PRIVATE", "STATIC", "ABSTRACT", 
  "CONST", "SPORK", "L_HACK", "R_HACK", "$accept", "program", 
  "program_section", "class_definition", "class_ext", "class_body", 
  "class_body2", "class_section", "iface_ext", "id_list", "id_dot", 
  "function_definition", "class_decl", "function_decl", "static_decl", 
  "type_decl_a", "type_decl_b", "type_decl", "type_decl2", "arg_list", 
  "statement_list", "statement", "jump_statement", "selection_statement", 
  "loop_statement", "code_segment", "expression_statement", "expression", 
  "chuck_expression", "array_exp", "array_empty", "decl_expression", 
  "var_decl_list", "var_decl", "chuck_operator", "conditional_expression", 
  "logical_or_expression", "logical_and_expression", 
  "inclusive_or_expression", "exclusive_or_expression", "and_expression", 
  "equality_expression", "relational_expression", "shift_expression", 
  "additive_expression", "multiplicative_expression", "tilda_expression", 
  "cast_expression", "unary_expression", "unary_operator", 
  "dur_expression", "postfix_expression", "primary_expression", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   103,   104,   104,   105,   105,   105,   106,   106,   106,
     106,   107,   107,   107,   107,   108,   108,   109,   109,   110,
     110,   110,   111,   112,   112,   113,   113,   114,   114,   114,
     114,   115,   115,   115,   116,   116,   116,   116,   117,   117,
     117,   118,   118,   119,   119,   120,   120,   121,   121,   122,
     122,   123,   123,   124,   124,   124,   124,   124,   125,   125,
     125,   125,   126,   126,   127,   127,   127,   127,   127,   127,
     127,   128,   128,   129,   129,   130,   130,   131,   131,   132,
     132,   133,   133,   134,   134,   134,   134,   134,   135,   135,
     136,   136,   136,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   138,   138,   139,   139,
     140,   140,   141,   141,   142,   142,   143,   143,   144,   144,
     144,   145,   145,   145,   145,   145,   146,   146,   146,   147,
     147,   147,   148,   148,   148,   148,   149,   149,   150,   150,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   152,
     152,   152,   152,   152,   152,   153,   153,   154,   154,   154,
     154,   154,   154,   154,   155,   155,   155,   155,   155,   155,
     155,   155
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     7,     6,
       7,     2,     4,     2,     4,     1,     0,     1,     2,     1,
       1,     1,     2,     1,     3,     1,     3,     8,     7,     8,
       7,     1,     1,     0,     1,     1,     1,     1,     1,     1,
       0,     1,     2,     3,     4,     1,     1,     1,     2,     2,
       4,     1,     2,     1,     1,     1,     1,     1,     2,     3,
       2,     2,     5,     7,     5,     7,     6,     7,     5,     7,
       5,     2,     3,     1,     2,     1,     3,     1,     3,     3,
       4,     2,     3,     1,     2,     3,     2,     3,     1,     3,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     5,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     3,     1,     3,
       1,     2,     2,     2,     2,     2,     2,     3,     3,     1,
       1,     1,     1,     1,     2,     1,     3,     1,     2,     3,
       4,     3,     2,     2,     1,     1,     1,     1,     1,     3,
       3,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
      33,   164,   167,   165,   166,    73,     0,     0,     0,   149,
     150,   153,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,   152,     0,     0,     0,     0,     0,     0,     0,
     151,    35,    36,    37,     0,     0,     0,     0,    33,     6,
       5,     0,    40,    45,    46,     0,     4,    51,    56,    55,
      54,    57,    53,     0,    75,   168,    77,    83,   106,   108,
     110,   112,   114,   116,   118,   121,   126,   129,   132,   136,
     138,     0,   140,   155,   157,    42,   171,     0,     0,    71,
       0,    25,     0,     0,     0,     0,     0,     0,    60,    61,
      58,     0,   164,   141,   142,     0,    41,   146,   145,   144,
      90,    86,    88,     0,     0,   154,     0,     1,     3,     0,
       0,    38,    39,     0,    84,    52,    74,     0,    95,    96,
      97,    98,   103,   104,   105,    99,   100,   101,    93,    94,
     102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   143,     0,     0,     0,   162,   163,   158,
     170,    79,    72,     0,    43,     0,     0,     0,     0,     0,
       0,    59,     0,   147,     0,    91,    92,     0,    87,    85,
     148,   169,    23,     0,     0,    47,     0,    76,    78,   109,
       0,   111,   113,   115,   117,   119,   120,   122,   124,   123,
     125,   128,   127,   130,   131,   133,   134,   135,   137,   139,
     156,   159,     0,   161,    80,    26,    44,     0,     0,     0,
       0,     0,     0,     0,    81,     0,    89,     0,    33,     0,
       0,     0,    33,     0,     0,     0,    48,     0,     0,   160,
      62,    64,     0,     0,     0,     0,    70,    68,    82,    24,
      21,     0,    15,    33,    20,    19,    13,    11,    33,     0,
      22,    33,     0,   107,     0,    66,     0,     0,     0,     7,
      18,     0,     0,     0,     9,     0,     0,     0,     0,    63,
      67,    65,    69,    14,    12,     8,    10,    30,    28,    49,
       0,     0,    29,    27,    50
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    37,    38,   250,   231,   251,   252,   253,   234,   183,
      82,   254,    41,    42,   113,    43,    44,    45,   186,   278,
     255,    47,    48,    49,    50,    51,    52,    53,    54,    55,
     176,    56,   101,   102,   131,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -188
static const short yypact[] =
{
     532,    13,  -188,  -188,  -188,  -188,    36,  1027,   631,  -188,
    -188,  -188,    16,    25,    49,    51,   702,    57,    60,    67,
    -188,   755,  -188,    96,    96,    86,    32,    96,    96,    20,
    -188,   -40,  -188,    33,    19,    55,  1027,    43,   235,  -188,
    -188,    44,    59,  -188,  -188,    20,  -188,   702,  -188,  -188,
    -188,  -188,  -188,   107,   236,  -188,  -188,  -188,    23,    98,
      94,   114,    95,   135,   122,    79,   149,    90,    61,   118,
    -188,    96,    93,    71,  -188,  -188,  -188,   168,   167,  -188,
     166,   169,   156,  1027,  1027,   808,    -8,  1027,  -188,  -188,
    -188,   174,  -188,  -188,  -188,  1027,   120,   175,  -188,  -188,
     177,  -188,   179,    20,    20,   176,    91,  -188,  -188,   189,
     189,  -188,  -188,    32,  -188,  -188,  -188,  1027,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  1027,    96,  1027,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    32,  -188,     7,   861,   191,  -188,  -188,  -188,
    -188,   175,  -188,    16,   130,   186,   187,   808,   190,   193,
     188,  -188,   194,  -188,   921,  -188,   192,    20,  -188,  -188,
    -188,  -188,   201,     0,     6,   197,   208,  -188,  -188,    98,
     203,    94,   114,    95,   135,   122,   122,    79,    79,    79,
      79,   149,   149,    90,    90,    61,    61,    61,   118,  -188,
      71,  -188,   202,  -188,  -188,  -188,  -188,   702,   702,   974,
    1027,  1027,   702,   702,  -188,   199,  -188,   189,   334,    16,
     189,   204,   334,   189,   205,   207,   192,   211,    96,  -188,
     184,  -188,   702,   212,   213,   214,  -188,  -188,  -188,  -188,
    -188,   215,  -188,   433,  -188,  -188,   136,   131,   334,   216,
    -188,   334,    34,  -188,   702,  -188,   702,   218,   220,  -188,
    -188,   189,    16,   217,  -188,   221,    48,    20,   222,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,   234,
      80,    32,  -188,  -188,  -188
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -188,   209,  -188,    31,  -188,  -187,   -10,  -188,  -188,  -105,
    -154,    38,  -188,  -188,  -188,  -188,  -188,   -26,  -188,   -39,
      26,   -15,  -188,  -188,  -188,  -101,   -71,    -4,  -188,   -67,
      72,   125,   -38,   -19,  -188,    21,  -188,   129,   128,   133,
     127,   132,    35,    12,    27,    30,   -18,   113,     1,  -188,
    -188,   112,  -188
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -42
static const short yytable[] =
{
      97,    86,    77,    78,   180,   184,   159,   114,   104,   215,
      92,     2,     3,     4,   167,   228,   -41,    91,     6,    81,
       7,   232,    96,   100,    93,    94,    46,   168,    98,    99,
     173,    39,   106,   175,    80,    96,    83,    96,    40,     1,
       2,     3,     4,   107,    12,   259,   276,     6,    76,     7,
     -31,   -31,   169,   132,     9,    10,    11,    12,   287,    12,
      84,    12,    85,     8,    46,   178,   179,   133,    87,    39,
      88,   273,   153,   115,   275,   256,    40,    89,    75,   165,
     166,    22,   155,   170,     7,    23,    24,   185,   156,   103,
     292,   172,   229,   230,   214,     8,   219,    95,   233,    92,
       2,     3,     4,    26,    27,    28,    29,     6,    36,     7,
     148,   149,   150,   187,     9,    10,    11,   116,   284,    30,
     157,   158,   249,   -32,   -32,   257,   209,   134,   260,   190,
     205,   206,   207,    34,   109,   110,    35,    36,   105,   226,
     135,    22,   137,   159,   151,    23,    24,   140,   141,   142,
     143,   212,   197,   198,   199,   200,   111,   112,   138,   139,
     144,   145,   136,    26,    27,    28,   283,   146,   147,   152,
      78,   201,   202,   195,   196,   288,   203,   204,   154,    30,
     160,   161,   162,   164,   171,    75,   163,   177,     7,   293,
     174,     8,   182,   181,   213,   216,    35,    36,   217,   218,
     222,   220,   240,   241,   221,   225,   223,   246,   247,   227,
     235,   237,   238,   248,   239,   243,   244,   245,   264,   258,
     261,   224,   262,   272,   266,   267,   268,   265,   281,   271,
     282,   269,   274,   285,   290,    -2,   277,   286,     1,     2,
       3,     4,   291,   270,   117,     5,     6,   108,     7,   279,
       8,   280,   294,     9,    10,    11,   188,   236,   289,   263,
      12,   189,   191,   193,   208,   277,   210,    13,   192,   194,
      14,    15,    16,    17,    18,    19,     0,    20,    21,     0,
      22,     0,     0,     0,    23,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,     0,    26,    27,    28,    29,     0,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,     0,    30,     0,
     128,     0,     0,   129,     0,   130,     0,     0,     0,    31,
      32,    33,    34,     0,     0,    35,    36,     1,     2,     3,
       4,     0,     0,     0,     5,     6,     0,     7,     0,     8,
     -16,     0,     9,    10,    11,     0,     0,     0,     0,    12,
       0,     0,     0,     0,     0,     0,    13,     0,     0,    14,
      15,    16,    17,    18,    19,     0,    20,    21,     0,    22,
       0,     0,     0,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
       0,    26,    27,    28,    29,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    30,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,    34,     0,     0,    35,    36,     1,     2,     3,     4,
       0,     0,     0,     5,     6,     0,     7,     0,     8,   -17,
       0,     9,    10,    11,     0,     0,     0,     0,    12,     0,
       0,     0,     0,     0,     0,    13,     0,     0,    14,    15,
      16,    17,    18,    19,     0,    20,    21,     0,    22,     0,
       0,     0,    23,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,     0,
      26,    27,    28,    29,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    30,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
      34,     0,     0,    35,    36,     1,     2,     3,     4,     0,
       0,     0,     5,     6,     0,     7,     0,     8,     0,     0,
       9,    10,    11,     0,     0,     0,     0,    12,     0,     0,
       0,     0,     0,     0,    13,     0,     0,    14,    15,    16,
      17,    18,    19,     0,    20,    21,     0,    22,     0,     0,
       0,    23,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,     0,    26,
      27,    28,    29,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    32,    33,    34,
       0,     0,    35,    36,     1,     2,     3,     4,     0,     0,
       0,     5,     6,     0,     7,     0,     8,    79,     0,     9,
      10,    11,     0,     0,     0,     0,    12,     0,     0,     0,
       0,     0,     0,    13,     0,     0,    14,    15,    16,    17,
      18,    19,     0,     0,    21,     0,    22,     0,     0,     0,
      23,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,     0,    26,    27,
      28,    29,     0,     0,     0,     1,     2,     3,     4,     0,
       0,     0,     5,     6,    30,     7,     0,     8,     0,     0,
       9,    10,    11,     0,     0,     0,     0,    12,    34,     0,
       0,    35,    36,     0,    13,     0,     0,    14,    15,    16,
      17,    18,    19,     0,     0,    21,     0,    22,     0,     0,
       0,    23,    24,     0,     0,     0,     0,     0,     1,     2,
       3,     4,    25,     0,     0,    90,     6,     0,     7,    26,
      27,    28,    29,     9,    10,    11,     0,     0,     0,     0,
      12,     0,     0,     0,     0,    30,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    34,
      22,     0,    35,    36,    23,    24,     0,     0,     0,     0,
       0,     1,     2,     3,     4,     0,     0,     0,     5,     6,
       0,     7,    26,    27,    28,    29,     9,    10,    11,     0,
       0,     0,     0,    12,     0,     0,     0,     0,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,    22,     0,    35,    36,    23,    24,     0,
       0,     0,     0,     0,     1,     2,     3,     4,     0,     0,
       0,     0,     6,   211,     7,    26,    27,    28,    29,     9,
      10,    11,     0,     0,     0,     0,    12,     0,     0,     0,
       0,    30,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,    22,     0,    35,    36,
      23,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     1,     2,     3,     4,    26,    27,
      28,    29,     6,     0,     7,   224,     0,     0,     0,     9,
      10,    11,     0,     0,    30,     0,    12,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
       0,    35,    36,     0,     0,     0,    22,     0,     0,     0,
      23,    24,     0,     0,     0,     0,     0,     1,     2,     3,
       4,     0,     0,     0,     0,     6,   242,     7,    26,    27,
      28,    29,     9,    10,    11,     0,     0,     0,     0,    12,
       0,     0,     0,     0,    30,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,    22,
       0,    35,    36,    23,    24,     0,     0,     0,     0,     0,
       1,     2,     3,     4,     0,     0,     0,     0,     6,     0,
       7,    26,    27,    28,    29,     9,    10,    11,     0,     0,
       0,     0,    12,     0,     0,     0,     0,    30,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,    22,     0,    35,    36,    23,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,    27,    28,    29,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,     0,    35,    36
};

static const short yycheck[] =
{
      26,    16,     6,     7,   105,   110,    73,    45,    34,   163,
       3,     4,     5,     6,    85,    15,     3,    21,    11,     3,
      13,    15,     3,     3,    23,    24,     0,    35,    27,    28,
      97,     0,    36,   100,     8,     3,    11,     3,     0,     3,
       4,     5,     6,     0,    25,   232,    12,    11,    12,    13,
      90,    91,    60,    30,    18,    19,    20,    25,    10,    25,
      11,    25,    11,    15,    38,   103,   104,    44,    11,    38,
      10,   258,    71,    47,   261,   229,    38,    10,    65,    83,
      84,    45,    11,    87,    13,    49,    50,   113,    17,    70,
      10,    95,    92,    93,   161,    15,   167,    11,    92,     3,
       4,     5,     6,    67,    68,    69,    70,    11,   101,    13,
      20,    21,    22,   117,    18,    19,    20,    10,   272,    83,
      49,    50,   227,    90,    91,   230,   152,    29,   233,   133,
     148,   149,   150,    97,    90,    91,   100,   101,    83,   177,
      46,    45,    47,   210,    83,    49,    50,    25,    26,    27,
      28,   155,   140,   141,   142,   143,    97,    98,    23,    24,
      81,    82,    48,    67,    68,    69,   271,    18,    19,    51,
     174,   144,   145,   138,   139,   276,   146,   147,    85,    83,
      12,    14,    16,    27,    10,    65,    17,     8,    13,   290,
      13,    15,     3,   102,     3,    65,   100,   101,    12,    12,
      12,    11,   217,   218,    11,    13,    12,   222,   223,     8,
      13,     3,     9,    14,    12,   219,   220,   221,    34,    15,
      15,    14,    11,    92,    12,    12,    12,   242,    10,    93,
      10,    16,    16,    16,    12,     0,   262,    16,     3,     4,
       5,     6,     8,   253,     8,    10,    11,    38,    13,   264,
      15,   266,   291,    18,    19,    20,   131,   185,   277,   238,
      25,   132,   134,   136,   151,   291,   154,    32,   135,   137,
      35,    36,    37,    38,    39,    40,    -1,    42,    43,    -1,
      45,    -1,    -1,    -1,    49,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    83,    -1,
      84,    -1,    -1,    87,    -1,    89,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,   100,   101,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    -1,    13,    -1,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    -1,    45,
      -1,    -1,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    -1,    -1,   100,   101,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    -1,    13,    -1,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    -1,    45,    -1,
      -1,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,   100,   101,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    -1,    13,    -1,    15,    -1,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    -1,    45,    -1,    -1,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      -1,    -1,   100,   101,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    -1,    13,    -1,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    45,    -1,    -1,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    83,    13,    -1,    15,    -1,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    97,    -1,
      -1,   100,   101,    -1,    32,    -1,    -1,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    45,    -1,    -1,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    60,    -1,    -1,    10,    11,    -1,    13,    67,
      68,    69,    70,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      45,    -1,   100,   101,    49,    50,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      -1,    13,    67,    68,    69,    70,    18,    19,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    45,    -1,   100,   101,    49,    50,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    -1,    11,    12,    13,    67,    68,    69,    70,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    45,    -1,   100,   101,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    67,    68,
      69,    70,    11,    -1,    13,    14,    -1,    -1,    -1,    18,
      19,    20,    -1,    -1,    83,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,   100,   101,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    -1,    11,    12,    13,    67,    68,
      69,    70,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    45,
      -1,   100,   101,    49,    50,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    -1,    11,    -1,
      13,    67,    68,    69,    70,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    45,    -1,   100,   101,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,   100,   101
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     5,     6,    10,    11,    13,    15,    18,
      19,    20,    25,    32,    35,    36,    37,    38,    39,    40,
      42,    43,    45,    49,    50,    60,    67,    68,    69,    70,
      83,    94,    95,    96,    97,   100,   101,   104,   105,   106,
     114,   115,   116,   118,   119,   120,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   134,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,    65,    12,   130,   130,    16,
     123,     3,   113,    11,    11,    11,   124,    11,    10,    10,
      10,   130,     3,   151,   151,    11,     3,   120,   151,   151,
       3,   135,   136,    70,   120,    83,   130,     0,   104,    90,
      91,    97,    98,   117,   135,   123,    10,     8,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    84,    87,
      89,   137,    30,    44,    29,    46,    48,    47,    23,    24,
      25,    26,    27,    28,    81,    82,    18,    19,    20,    21,
      22,    83,    51,   151,    85,    11,    17,    49,    50,   132,
      12,    14,    16,    17,    27,   130,   130,   129,    35,    60,
     130,    10,   130,   132,    13,   132,   133,     8,   135,   135,
     128,   102,     3,   112,   112,   120,   121,   130,   134,   140,
     130,   141,   142,   143,   144,   145,   145,   146,   146,   146,
     146,   147,   147,   148,   148,   149,   149,   149,   150,   120,
     154,    12,   130,     3,   132,   113,    65,    12,    12,   129,
      11,    11,    12,    12,    14,    13,   135,     8,    15,    92,
      93,   107,    15,    92,   111,    13,   133,     3,     9,    12,
     124,   124,    12,   130,   130,   130,   124,   124,    14,   112,
     106,   108,   109,   110,   114,   123,   113,   112,    15,   108,
     112,    15,    11,   138,    34,   124,    12,    12,    12,    16,
     109,    93,    92,   108,    16,   108,    12,   120,   122,   124,
     124,    10,    10,   112,   113,    16,    16,    10,   128,   136,
      12,     8,    10,   128,   122
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 171 "chuck.y"
    { yyval.program = g_program = new_program( yyvsp[0].program_section, EM_lineNum ); ;}
    break;

  case 3:
#line 172 "chuck.y"
    { yyval.program = g_program = prepend_program( yyvsp[-1].program_section, yyvsp[0].program, EM_lineNum ); ;}
    break;

  case 4:
#line 176 "chuck.y"
    { yyval.program_section = new_section_stmt( yyvsp[0].stmt_list, EM_lineNum ); ;}
    break;

  case 5:
#line 177 "chuck.y"
    { yyval.program_section = new_section_func_def( yyvsp[0].func_def, EM_lineNum ); ;}
    break;

  case 6:
#line 178 "chuck.y"
    { yyval.program_section = new_section_class_def( yyvsp[0].class_def, EM_lineNum ); ;}
    break;

  case 7:
#line 183 "chuck.y"
    { yyval.class_def = new_class_def( yyvsp[-5].ival, yyvsp[-3].id_list, NULL, yyvsp[-1].class_body, EM_lineNum ); ;}
    break;

  case 8:
#line 185 "chuck.y"
    { yyval.class_def = new_class_def( yyvsp[-6].ival, yyvsp[-4].id_list, yyvsp[-3].class_ext, yyvsp[-1].class_body, EM_lineNum ); ;}
    break;

  case 9:
#line 187 "chuck.y"
    { yyval.class_def = new_iface_def( yyvsp[-5].ival, yyvsp[-3].id_list, NULL, yyvsp[-1].class_body, EM_lineNum ); ;}
    break;

  case 10:
#line 189 "chuck.y"
    { yyval.class_def = new_iface_def( yyvsp[-6].ival, yyvsp[-4].id_list, yyvsp[-3].class_ext, yyvsp[-1].class_body, EM_lineNum ); ;}
    break;

  case 11:
#line 193 "chuck.y"
    { yyval.class_ext = new_class_ext( NULL, yyvsp[0].id_list, EM_lineNum ); ;}
    break;

  case 12:
#line 194 "chuck.y"
    { yyval.class_ext = new_class_ext( yyvsp[0].id_list, yyvsp[-2].id_list, EM_lineNum ); ;}
    break;

  case 13:
#line 195 "chuck.y"
    { yyval.class_ext = new_class_ext( yyvsp[0].id_list, NULL, EM_lineNum ); ;}
    break;

  case 14:
#line 196 "chuck.y"
    { yyval.class_ext = new_class_ext( yyvsp[-2].id_list, yyvsp[0].id_list, EM_lineNum ); ;}
    break;

  case 15:
#line 200 "chuck.y"
    { yyval.class_body = yyvsp[0].class_body; ;}
    break;

  case 16:
#line 201 "chuck.y"
    { yyval.class_body = NULL; ;}
    break;

  case 17:
#line 205 "chuck.y"
    { yyval.class_body = new_class_body( yyvsp[0].program_section, EM_lineNum ); ;}
    break;

  case 18:
#line 206 "chuck.y"
    { yyval.class_body = prepend_class_body( yyvsp[-1].program_section, yyvsp[0].class_body, EM_lineNum ); ;}
    break;

  case 19:
#line 211 "chuck.y"
    { yyval.program_section = new_section_stmt( yyvsp[0].stmt_list, EM_lineNum ); ;}
    break;

  case 20:
#line 212 "chuck.y"
    { yyval.program_section = new_section_func_def( yyvsp[0].func_def, EM_lineNum ); ;}
    break;

  case 21:
#line 213 "chuck.y"
    { yyval.program_section = new_section_class_def( yyvsp[0].class_def, EM_lineNum ); ;}
    break;

  case 22:
#line 217 "chuck.y"
    { yyval.class_ext = new_class_ext( NULL, yyvsp[0].id_list, EM_lineNum ); ;}
    break;

  case 23:
#line 221 "chuck.y"
    { yyval.id_list = new_id_list( yyvsp[0].sval, EM_lineNum ); ;}
    break;

  case 24:
#line 222 "chuck.y"
    { yyval.id_list = prepend_id_list( yyvsp[-2].sval, yyvsp[0].id_list, EM_lineNum ); ;}
    break;

  case 25:
#line 226 "chuck.y"
    { yyval.id_list = new_id_list( yyvsp[0].sval, EM_lineNum ); ;}
    break;

  case 26:
#line 227 "chuck.y"
    { yyval.id_list = prepend_id_list( yyvsp[-2].sval, yyvsp[0].id_list, EM_lineNum ); ;}
    break;

  case 27:
#line 232 "chuck.y"
    { yyval.func_def = new_func_def( yyvsp[-7].ival, yyvsp[-6].ival, yyvsp[-5].type_decl, yyvsp[-4].sval, yyvsp[-2].arg_list, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 28:
#line 234 "chuck.y"
    { yyval.func_def = new_func_def( yyvsp[-6].ival, yyvsp[-5].ival, yyvsp[-4].type_decl, yyvsp[-3].sval, NULL, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 29:
#line 236 "chuck.y"
    { yyval.func_def = new_func_def( yyvsp[-7].ival, yyvsp[-6].ival, yyvsp[-5].type_decl, yyvsp[-4].sval, yyvsp[-2].arg_list, NULL, EM_lineNum ); ;}
    break;

  case 30:
#line 238 "chuck.y"
    { yyval.func_def = new_func_def( yyvsp[-6].ival, yyvsp[-5].ival, yyvsp[-4].type_decl, yyvsp[-3].sval, NULL, NULL, EM_lineNum ); ;}
    break;

  case 31:
#line 242 "chuck.y"
    { yyval.ival = ae_key_public; ;}
    break;

  case 32:
#line 243 "chuck.y"
    { yyval.ival = ae_key_private; ;}
    break;

  case 33:
#line 244 "chuck.y"
    { yyval.ival = ae_key_private; ;}
    break;

  case 34:
#line 248 "chuck.y"
    { yyval.ival = ae_key_func; ;}
    break;

  case 35:
#line 249 "chuck.y"
    { yyval.ival = ae_key_public; ;}
    break;

  case 36:
#line 250 "chuck.y"
    { yyval.ival = ae_key_protected; ;}
    break;

  case 37:
#line 251 "chuck.y"
    { yyval.ival = ae_key_private; ;}
    break;

  case 38:
#line 255 "chuck.y"
    { yyval.ival = ae_key_static; ;}
    break;

  case 39:
#line 256 "chuck.y"
    { yyval.ival = ae_key_abstract; ;}
    break;

  case 40:
#line 257 "chuck.y"
    { yyval.ival = ae_key_instance; ;}
    break;

  case 41:
#line 261 "chuck.y"
    { yyval.type_decl = new_type_decl( new_id_list( yyvsp[0].sval, EM_lineNum ), 0, EM_lineNum ); ;}
    break;

  case 42:
#line 262 "chuck.y"
    { yyval.type_decl = new_type_decl( new_id_list( yyvsp[-1].sval, EM_lineNum ), 1, EM_lineNum ); ;}
    break;

  case 43:
#line 266 "chuck.y"
    { yyval.type_decl = new_type_decl( yyvsp[-1].id_list, 0, EM_lineNum ); ;}
    break;

  case 44:
#line 267 "chuck.y"
    { yyval.type_decl = new_type_decl( yyvsp[-2].id_list, 1, EM_lineNum ); ;}
    break;

  case 45:
#line 276 "chuck.y"
    { yyval.type_decl = yyvsp[0].type_decl; ;}
    break;

  case 46:
#line 277 "chuck.y"
    { yyval.type_decl = yyvsp[0].type_decl; ;}
    break;

  case 47:
#line 282 "chuck.y"
    { yyval.type_decl = yyvsp[0].type_decl; ;}
    break;

  case 48:
#line 283 "chuck.y"
    { yyval.type_decl = add_type_decl_array( yyvsp[-1].type_decl, yyvsp[0].array_sub, EM_lineNum ); ;}
    break;

  case 49:
#line 287 "chuck.y"
    { yyval.arg_list = new_arg_list( yyvsp[-1].type_decl, yyvsp[0].var_decl, EM_lineNum ); ;}
    break;

  case 50:
#line 288 "chuck.y"
    { yyval.arg_list = prepend_arg_list( yyvsp[-3].type_decl, yyvsp[-2].var_decl, yyvsp[0].arg_list, EM_lineNum ); ;}
    break;

  case 51:
#line 292 "chuck.y"
    { yyval.stmt_list = new_stmt_list( yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 52:
#line 293 "chuck.y"
    { yyval.stmt_list = prepend_stmt_list( yyvsp[-1].stmt, yyvsp[0].stmt_list, EM_lineNum ); ;}
    break;

  case 53:
#line 297 "chuck.y"
    { yyval.stmt = yyvsp[0].stmt; ;}
    break;

  case 54:
#line 298 "chuck.y"
    { yyval.stmt = yyvsp[0].stmt; ;}
    break;

  case 55:
#line 299 "chuck.y"
    { yyval.stmt = yyvsp[0].stmt; ;}
    break;

  case 56:
#line 300 "chuck.y"
    { yyval.stmt = yyvsp[0].stmt; ;}
    break;

  case 57:
#line 302 "chuck.y"
    { yyval.stmt = yyvsp[0].stmt; ;}
    break;

  case 58:
#line 306 "chuck.y"
    { yyval.stmt = new_stmt_from_return( NULL, EM_lineNum ); ;}
    break;

  case 59:
#line 307 "chuck.y"
    { yyval.stmt = new_stmt_from_return( yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 60:
#line 308 "chuck.y"
    { yyval.stmt = new_stmt_from_break( EM_lineNum ); ;}
    break;

  case 61:
#line 309 "chuck.y"
    { yyval.stmt = new_stmt_from_continue( EM_lineNum ); ;}
    break;

  case 62:
#line 314 "chuck.y"
    { yyval.stmt = new_stmt_from_if( yyvsp[-2].exp, yyvsp[0].stmt, NULL, EM_lineNum ); ;}
    break;

  case 63:
#line 316 "chuck.y"
    { yyval.stmt = new_stmt_from_if( yyvsp[-4].exp, yyvsp[-2].stmt, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 64:
#line 321 "chuck.y"
    { yyval.stmt = new_stmt_from_while( yyvsp[-2].exp, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 65:
#line 323 "chuck.y"
    { yyval.stmt = new_stmt_from_do_while( yyvsp[-2].exp, yyvsp[-5].stmt, EM_lineNum ); ;}
    break;

  case 66:
#line 325 "chuck.y"
    { yyval.stmt = new_stmt_from_for( yyvsp[-3].stmt, yyvsp[-2].stmt, NULL, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 67:
#line 327 "chuck.y"
    { yyval.stmt = new_stmt_from_for( yyvsp[-4].stmt, yyvsp[-3].stmt, yyvsp[-2].exp, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 68:
#line 329 "chuck.y"
    { yyval.stmt = new_stmt_from_until( yyvsp[-2].exp, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 69:
#line 331 "chuck.y"
    { yyval.stmt = new_stmt_from_do_until( yyvsp[-2].exp, yyvsp[-5].stmt, EM_lineNum ); ;}
    break;

  case 70:
#line 333 "chuck.y"
    { yyval.stmt = new_stmt_from_loop( yyvsp[-2].exp, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 71:
#line 337 "chuck.y"
    { yyval.stmt = new_stmt_from_code( NULL, EM_lineNum ); ;}
    break;

  case 72:
#line 338 "chuck.y"
    { yyval.stmt = new_stmt_from_code( yyvsp[-1].stmt_list, EM_lineNum ); ;}
    break;

  case 73:
#line 342 "chuck.y"
    { yyval.stmt = NULL; ;}
    break;

  case 74:
#line 343 "chuck.y"
    { yyval.stmt = new_stmt_from_expression( yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 75:
#line 347 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 76:
#line 348 "chuck.y"
    { yyval.exp = prepend_expression( yyvsp[-2].exp, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 77:
#line 352 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 78:
#line 354 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, yyvsp[-1].ival, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 79:
#line 358 "chuck.y"
    { yyval.array_sub = new_array_sub( yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 80:
#line 360 "chuck.y"
    { yyval.array_sub = prepend_array_sub( yyvsp[0].array_sub, yyvsp[-2].exp, EM_lineNum ); ;}
    break;

  case 81:
#line 364 "chuck.y"
    { yyval.array_sub = new_array_sub( NULL, EM_lineNum ); ;}
    break;

  case 82:
#line 365 "chuck.y"
    { yyval.array_sub = prepend_array_sub( yyvsp[-2].array_sub, NULL, EM_lineNum ); ;}
    break;

  case 83:
#line 369 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 84:
#line 370 "chuck.y"
    { yyval.exp = new_exp_decl( yyvsp[-1].type_decl, yyvsp[0].var_decl_list, 0, EM_lineNum ); ;}
    break;

  case 85:
#line 371 "chuck.y"
    { yyval.exp = new_exp_decl( yyvsp[-1].type_decl, yyvsp[0].var_decl_list, 1, EM_lineNum ); ;}
    break;

  case 86:
#line 372 "chuck.y"
    { yyval.exp = new_exp_decl( NULL, yyvsp[0].var_decl_list, 0, EM_lineNum ); ;}
    break;

  case 87:
#line 373 "chuck.y"
    { yyval.exp = new_exp_decl( NULL, yyvsp[0].var_decl_list, 1, EM_lineNum ); ;}
    break;

  case 88:
#line 377 "chuck.y"
    { yyval.var_decl_list = new_var_decl_list( yyvsp[0].var_decl, EM_lineNum ); ;}
    break;

  case 89:
#line 378 "chuck.y"
    { yyval.var_decl_list = prepend_var_decl_list( yyvsp[-2].var_decl, yyvsp[0].var_decl_list, EM_lineNum ); ;}
    break;

  case 90:
#line 382 "chuck.y"
    { yyval.var_decl = new_var_decl( yyvsp[0].sval, NULL, EM_lineNum ); ;}
    break;

  case 91:
#line 383 "chuck.y"
    { yyval.var_decl = new_var_decl( yyvsp[-1].sval, yyvsp[0].array_sub, EM_lineNum ); ;}
    break;

  case 92:
#line 384 "chuck.y"
    { yyval.var_decl = new_var_decl( yyvsp[-1].sval, yyvsp[0].array_sub, EM_lineNum ); ;}
    break;

  case 93:
#line 388 "chuck.y"
    { yyval.ival = ae_op_chuck; ;}
    break;

  case 94:
#line 389 "chuck.y"
    { yyval.ival = ae_op_at_chuck; ;}
    break;

  case 95:
#line 390 "chuck.y"
    { yyval.ival = ae_op_plus_chuck; ;}
    break;

  case 96:
#line 391 "chuck.y"
    { yyval.ival = ae_op_minus_chuck; ;}
    break;

  case 97:
#line 392 "chuck.y"
    { yyval.ival = ae_op_times_chuck; ;}
    break;

  case 98:
#line 393 "chuck.y"
    { yyval.ival = ae_op_divide_chuck; ;}
    break;

  case 99:
#line 394 "chuck.y"
    { yyval.ival = ae_op_shift_right_chuck; ;}
    break;

  case 100:
#line 395 "chuck.y"
    { yyval.ival = ae_op_shift_left_chuck; ;}
    break;

  case 101:
#line 396 "chuck.y"
    { yyval.ival = ae_op_percent_chuck; ;}
    break;

  case 102:
#line 397 "chuck.y"
    { yyval.ival = ae_op_unchuck; ;}
    break;

  case 103:
#line 398 "chuck.y"
    { yyval.ival = ae_op_s_and_chuck; ;}
    break;

  case 104:
#line 399 "chuck.y"
    { yyval.ival = ae_op_s_or_chuck; ;}
    break;

  case 105:
#line 400 "chuck.y"
    { yyval.ival = ae_op_s_xor_chuck; ;}
    break;

  case 106:
#line 404 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 107:
#line 406 "chuck.y"
    { yyval.exp = new_exp_from_if( yyvsp[-4].exp, yyvsp[-2].exp, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 108:
#line 410 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 109:
#line 412 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_or, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 110:
#line 416 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 111:
#line 418 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_and, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 112:
#line 422 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 113:
#line 424 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_s_or, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 114:
#line 428 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 115:
#line 430 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_s_xor, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 116:
#line 434 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 117:
#line 436 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_s_and, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 118:
#line 440 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 119:
#line 442 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_eq, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 120:
#line 444 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_neq, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 121:
#line 448 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 122:
#line 450 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_lt, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 123:
#line 452 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_gt, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 124:
#line 454 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_le, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 125:
#line 456 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_ge, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 126:
#line 460 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 127:
#line 462 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_shift_left, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 128:
#line 464 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_shift_right, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 129:
#line 468 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 130:
#line 470 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_plus, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 131:
#line 472 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_minus, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 132:
#line 476 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 133:
#line 478 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_times, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 134:
#line 480 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_divide, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 135:
#line 482 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_percent, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 136:
#line 486 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 137:
#line 488 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_tilda, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 138:
#line 492 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 139:
#line 494 "chuck.y"
    { yyval.exp = new_exp_from_cast( yyvsp[0].type_decl, yyvsp[-2].exp, EM_lineNum ); ;}
    break;

  case 140:
#line 498 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 141:
#line 500 "chuck.y"
    { yyval.exp = new_exp_from_unary( ae_op_plusplus, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 142:
#line 502 "chuck.y"
    { yyval.exp = new_exp_from_unary( ae_op_minusminus, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 143:
#line 504 "chuck.y"
    { yyval.exp = new_exp_from_unary( yyvsp[-1].ival, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 144:
#line 506 "chuck.y"
    { yyval.exp = new_exp_from_unary( ae_op_typeof, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 145:
#line 508 "chuck.y"
    { yyval.exp = new_exp_from_unary( ae_op_sizeof, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 146:
#line 510 "chuck.y"
    { yyval.exp = new_exp_from_unary2( ae_op_new, yyvsp[0].type_decl, NULL, EM_lineNum ); ;}
    break;

  case 147:
#line 512 "chuck.y"
    { yyval.exp = new_exp_from_unary2( ae_op_new, yyvsp[-1].type_decl, yyvsp[0].array_sub, EM_lineNum ); ;}
    break;

  case 148:
#line 514 "chuck.y"
    { yyval.exp = new_exp_from_unary3( ae_op_spork, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 149:
#line 518 "chuck.y"
    { yyval.ival = ae_op_plus; ;}
    break;

  case 150:
#line 519 "chuck.y"
    { yyval.ival = ae_op_minus; ;}
    break;

  case 151:
#line 520 "chuck.y"
    { yyval.ival = ae_op_tilda; ;}
    break;

  case 152:
#line 521 "chuck.y"
    { yyval.ival = ae_op_exclamation; ;}
    break;

  case 153:
#line 522 "chuck.y"
    { yyval.ival = ae_op_times; ;}
    break;

  case 154:
#line 523 "chuck.y"
    { yyval.ival = ae_op_spork; ;}
    break;

  case 156:
#line 530 "chuck.y"
    { yyval.exp = new_exp_from_dur( yyvsp[-2].exp, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 157:
#line 534 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 158:
#line 536 "chuck.y"
    { yyval.exp = new_exp_from_array( yyvsp[-1].exp, yyvsp[0].array_sub, EM_lineNum ); ;}
    break;

  case 159:
#line 538 "chuck.y"
    { yyval.exp = new_exp_from_func_call( yyvsp[-2].exp, NULL, EM_lineNum ); ;}
    break;

  case 160:
#line 540 "chuck.y"
    { yyval.exp = new_exp_from_func_call( yyvsp[-3].exp, yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 161:
#line 542 "chuck.y"
    { yyval.exp = new_exp_from_member_dot( yyvsp[-2].exp, yyvsp[0].sval, EM_lineNum ); ;}
    break;

  case 162:
#line 544 "chuck.y"
    { yyval.exp = new_exp_from_postfix( yyvsp[-1].exp, ae_op_plusplus, EM_lineNum ); ;}
    break;

  case 163:
#line 546 "chuck.y"
    { yyval.exp = new_exp_from_postfix( yyvsp[-1].exp, ae_op_minusminus, EM_lineNum ); ;}
    break;

  case 164:
#line 550 "chuck.y"
    { yyval.exp = new_exp_from_id( yyvsp[0].sval, EM_lineNum ); ;}
    break;

  case 165:
#line 551 "chuck.y"
    { yyval.exp = new_exp_from_int( yyvsp[0].ival, EM_lineNum ); ;}
    break;

  case 166:
#line 552 "chuck.y"
    { yyval.exp = new_exp_from_float( yyvsp[0].fval, EM_lineNum ); ;}
    break;

  case 167:
#line 553 "chuck.y"
    { yyval.exp = new_exp_from_str( yyvsp[0].sval, EM_lineNum ); ;}
    break;

  case 168:
#line 554 "chuck.y"
    { yyval.exp = new_exp_from_array_lit( yyvsp[0].array_sub, EM_lineNum ); ;}
    break;

  case 169:
#line 555 "chuck.y"
    { yyval.exp = new_exp_from_hack( yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 170:
#line 556 "chuck.y"
    { yyval.exp = yyvsp[-1].exp; ;}
    break;

  case 171:
#line 557 "chuck.y"
    { yyval.exp = new_exp_from_nil( EM_lineNum ); ;}
    break;


    }

/* Line 999 of yacc.c.  */
#line 2570 "chuck.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}



#line 2 "chuck.yy.c"
/* A lexical scanner generated by flex */

/* Scanner skeleton version:
 * $Header: /cvs/chuck_dev/v2/chuck_win32.c,v 1.50 2007/04/09 21:29:14 gw-chuck Exp $
 */

#define FLEX_SCANNER
#define YY_FLEX_MAJOR_VERSION 2
#define YY_FLEX_MINOR_VERSION 5

#include <stdio.h>
#include <errno.h>

/* cfront 1.2 defines "c_plusplus" instead of "__cplusplus" */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif


#ifdef __cplusplus

#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif

/* Use prototypes in function declarations. */
#define YY_USE_PROTOS

/* The "const" storage-class-modifier is valid. */
#define YY_USE_CONST

#else	/* ! __cplusplus */

#if __STDC__

#define YY_USE_PROTOS
#define YY_USE_CONST

#endif	/* __STDC__ */
#endif	/* ! __cplusplus */

#ifdef __TURBOC__
 #pragma warn -rch
 #pragma warn -use
#include <io.h>
#include <stdlib.h>
#define YY_USE_CONST
#define YY_USE_PROTOS
#endif

#ifdef YY_USE_CONST
#define yyconst const
#else
#define yyconst
#endif


#ifdef YY_USE_PROTOS
#define YY_PROTO(proto) proto
#else
#define YY_PROTO(proto) ()
#endif


/* Returned upon end-of-file. */
#define YY_NULL 0

/* Promotes a possibly negative, possibly signed char to an unsigned
 * integer for use as an array index.  If the signed char is negative,
 * we want to instead treat it as an 8-bit unsigned char, hence the
 * double cast.
 */
#define YY_SC_TO_UI(c) ((unsigned int) (unsigned char) c)

/* Enter a start condition.  This macro really ought to take a parameter,
 * but we do it the disgusting crufty way forced on us by the ()-less
 * definition of BEGIN.
 */
#define BEGIN yy_start = 1 + 2 *

/* Translate the current start state into a value that can be later handed
 * to BEGIN to return to the state.  The YYSTATE alias is for lex
 * compatibility.
 */
#define YY_START ((yy_start - 1) / 2)
#define YYSTATE YY_START

/* Action number for EOF rule of a given start state. */
#define YY_STATE_EOF(state) (YY_END_OF_BUFFER + state + 1)

/* Special action meaning "start processing a new file". */
#define YY_NEW_FILE yyrestart( yyin )

#define YY_END_OF_BUFFER_CHAR 0

/* Size of default input buffer. */
#define YY_BUF_SIZE 16384

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern int yyleng;
extern FILE *yyin, *yyout;

#define EOB_ACT_CONTINUE_SCAN 0
#define EOB_ACT_END_OF_FILE 1
#define EOB_ACT_LAST_MATCH 2

/* The funky do-while in the following #define is used to turn the definition
 * int a single C statement (which needs a semi-colon terminator).  This
 * avoids problems with code like:
 *
 * 	if ( condition_holds )
 *		yyless( 5 );
 *	else
 *		do_something_else();
 *
 * Prior to using the do-while the compiler would get upset at the
 * "else" because it interpreted the "if" statement as being all
 * done when it reached the ';' after the yyless() call.
 */

/* Return all but the first 'n' matched characters back to the input stream. */

#define yyless(n) \
	do \
		{ \
		/* Undo effects of setting up yytext. */ \
		*yy_cp = yy_hold_char; \
		YY_RESTORE_YY_MORE_OFFSET \
		yy_c_buf_p = yy_cp = yy_bp + n - YY_MORE_ADJ; \
		YY_DO_BEFORE_ACTION; /* set up yytext again */ \
		} \
	while ( 0 )

#define unput(c) yyunput( c, yytext_ptr )

/* The following is because we cannot portably get our hands on size_t
 * (without autoconf's help, which isn't available because we want
 * flex-generated scanners to compile on their own).
 */
typedef unsigned int yy_size_t;


struct yy_buffer_state
	{
	FILE *yy_input_file;

	char *yy_ch_buf;		/* input buffer */
	char *yy_buf_pos;		/* current position in input buffer */

	/* Size of input buffer in bytes, not including room for EOB
	 * characters.
	 */
	yy_size_t yy_buf_size;

	/* Number of characters read into yy_ch_buf, not including EOB
	 * characters.
	 */
	int yy_n_chars;

	/* Whether we "own" the buffer - i.e., we know we created it,
	 * and can realloc() it to grow it, and should free() it to
	 * delete it.
	 */
	int yy_is_our_buffer;

	/* Whether this is an "interactive" input source; if so, and
	 * if we're using stdio for input, then we want to use getc()
	 * instead of fread(), to make sure we stop fetching input after
	 * each newline.
	 */
	int yy_is_interactive;

	/* Whether we're considered to be at the beginning of a line.
	 * If so, '^' rules will be active on the next match, otherwise
	 * not.
	 */
	int yy_at_bol;

	/* Whether to try to fill the input buffer when we reach the
	 * end of it.
	 */
	int yy_fill_buffer;

	int yy_buffer_status;
#define YY_BUFFER_NEW 0
#define YY_BUFFER_NORMAL 1
	/* When an EOF's been seen but there's still some text to process
	 * then we mark the buffer as YY_EOF_PENDING, to indicate that we
	 * shouldn't try reading from the input source any more.  We might
	 * still have a bunch of tokens to match, though, because of
	 * possible backing-up.
	 *
	 * When we actually see the EOF, we change the status to "new"
	 * (via yyrestart()), so that the user can continue scanning by
	 * just pointing yyin at a new input file.
	 */
#define YY_BUFFER_EOF_PENDING 2
	};

static YY_BUFFER_STATE yy_current_buffer = 0;

/* We provide macros for accessing buffer states in case in the
 * future we want to put the buffer states in a more general
 * "scanner state".
 */
#define YY_CURRENT_BUFFER yy_current_buffer


/* yy_hold_char holds the character lost when yytext is formed. */
static char yy_hold_char;

static int yy_n_chars;		/* number of characters read into yy_ch_buf */


int yyleng;

/* Points to current character in buffer. */
static char *yy_c_buf_p = (char *) 0;
static int yy_init = 1;		/* whether we need to initialize */
static int yy_start = 0;	/* start state number */

/* Flag which is used to allow yywrap()'s to do buffer switches
 * instead of setting up a fresh yyin.  A bit of a hack ...
 */
static int yy_did_buffer_switch_on_eof;

void yyrestart YY_PROTO(( FILE *input_file ));

void yy_switch_to_buffer YY_PROTO(( YY_BUFFER_STATE new_buffer ));
void yy_load_buffer_state YY_PROTO(( void ));
YY_BUFFER_STATE yy_create_buffer YY_PROTO(( FILE *file, int size ));
void yy_delete_buffer YY_PROTO(( YY_BUFFER_STATE b ));
void yy_init_buffer YY_PROTO(( YY_BUFFER_STATE b, FILE *file ));
void yy_flush_buffer YY_PROTO(( YY_BUFFER_STATE b ));
#define YY_FLUSH_BUFFER yy_flush_buffer( yy_current_buffer )

YY_BUFFER_STATE yy_scan_buffer YY_PROTO(( char *base, yy_size_t size ));
YY_BUFFER_STATE yy_scan_string YY_PROTO(( yyconst char *yy_str ));
YY_BUFFER_STATE yy_scan_bytes YY_PROTO(( yyconst char *bytes, int len ));

static void *yy_flex_alloc YY_PROTO(( yy_size_t ));
static void *yy_flex_realloc YY_PROTO(( void *, yy_size_t ));
static void yy_flex_free YY_PROTO(( void * ));

#define yy_new_buffer yy_create_buffer

#define yy_set_interactive(is_interactive) \
	{ \
	if ( ! yy_current_buffer ) \
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE ); \
	yy_current_buffer->yy_is_interactive = is_interactive; \
	}

#define yy_set_bol(at_bol) \
	{ \
	if ( ! yy_current_buffer ) \
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE ); \
	yy_current_buffer->yy_at_bol = at_bol; \
	}

#define YY_AT_BOL() (yy_current_buffer->yy_at_bol)

typedef unsigned char YY_CHAR;
FILE *yyin = (FILE *) 0, *yyout = (FILE *) 0;
typedef int yy_state_type;
extern char *yytext;
#define yytext_ptr yytext

static yy_state_type yy_get_previous_state YY_PROTO(( void ));
static yy_state_type yy_try_NUL_trans YY_PROTO(( yy_state_type current_state ));
static int yy_get_next_buffer YY_PROTO(( void ));
static void yy_fatal_error YY_PROTO(( yyconst char msg[] ));

/* Done after the current pattern has been matched and before the
 * corresponding action - sets up yytext.
 */
#define YY_DO_BEFORE_ACTION \
	yytext_ptr = yy_bp; \
	yyleng = (int) (yy_cp - yy_bp); \
	yy_hold_char = *yy_cp; \
	*yy_cp = '\0'; \
	yy_c_buf_p = yy_cp;

#define YY_NUM_RULES 95
#define YY_END_OF_BUFFER 96
static yyconst short int yy_accept[220] =
    {   0,
        0,    0,   96,   94,    5,    7,   94,    4,   43,   94,
       18,   19,   17,   29,   35,   36,   15,   13,   10,   14,
       12,   16,   90,   90,   11,   41,   23,   34,   24,   42,
       86,   92,   37,   38,   31,   92,   92,   92,   92,   92,
       92,   92,   92,   92,   92,   92,   92,   92,   39,   30,
       40,   44,    6,   22,    0,   93,    0,    0,   27,    0,
        0,    8,    0,    9,    0,   91,    3,    1,    0,   91,
       90,    0,   90,    0,   20,    0,   33,   25,   73,   21,
       72,   26,   32,    0,   87,   92,    0,   92,   92,   92,
       53,   92,   92,   92,   92,   51,   92,   92,   92,   92,

       92,   92,   92,   92,   92,   92,   92,    0,   28,   74,
       85,   80,   78,   76,   77,   79,   91,   89,   88,    2,
       54,    0,    0,   55,   75,   82,   92,   92,   92,   92,
       92,   45,   58,   92,   92,   59,   92,   92,   92,   92,
       92,   92,   92,   92,   92,   92,   92,   81,   89,   88,
       84,   83,   92,   92,   92,   92,   52,   92,   92,   92,
       92,   92,   92,   92,   68,   92,   92,   92,   92,   92,
       92,   92,   50,   60,   69,   92,   92,   92,   92,   92,
       92,   92,   92,   92,   92,   70,   92,   92,   47,   46,
       92,   92,   92,   92,   92,   92,   92,   64,   48,   56,

       67,   71,   92,   62,   92,   92,   92,   66,   92,   49,
       57,   92,   92,   92,   92,   61,   65,   63,    0
    } ;

static yyconst int yy_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
        1,    1,    4,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    5,    6,    7,    8,    9,   10,   11,    1,   12,
       13,   14,   15,   16,   17,   18,   19,   20,   21,   21,
       21,   21,   21,   21,   21,   22,   22,   23,   24,   25,
       26,   27,   28,   29,   30,   30,   31,   30,   30,   30,
       32,   32,   32,   32,   32,   33,   32,   32,   32,   32,
       32,   32,   32,   32,   34,   32,   32,   35,   32,   32,
       36,   37,   38,   39,   32,    1,   40,   41,   42,   43,

       44,   45,   32,   46,   47,   32,   48,   49,   50,   51,
       52,   53,   32,   54,   55,   56,   57,   58,   59,   60,
       61,   32,   62,   63,   64,   65,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,

        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1
    } ;

static yyconst int yy_meta[66] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
        2,    2,    1,    1,    1,    1,    1,    1,    1,    2,
        2,    3,    3,    3,    3,    1,    1,    1,    1,    2,
        2,    2,    2,    2,    2,    3,    3,    3,    3,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
        3,    1,    1,    1,    1
    } ;

static yyconst short int yy_base[223] =
    {   0,
        0,    0,  292,  293,  293,  293,  288,  293,  264,   59,
      293,  293,  263,   56,  293,  293,  262,   53,  293,   52,
       50,   61,   79,   70,  264,  293,   59,   68,   47,  293,
       57,    0,  293,  293,  260,  231,   53,  232,   49,   54,
       72,  239,   61,  238,   73,  220,  229,  233,  293,   81,
      293,  293,  293,  251,  101,  293,  274,  249,  293,  248,
      247,  293,  246,  293,  245,  110,  293,  293,  244,  113,
      125,  104,  107,    0,  293,  253,  123,  293,  293,  293,
      293,  293,  124,  242,  293,    0,  241,  223,  226,  214,
        0,  209,  207,  208,  210,    0,  207,  203,  199,   90,

      111,  101,  205,  216,  202,  198,  206,  225,  293,  293,
      293,  293,  293,  293,  293,  293,  140,  146,  135,  293,
      293,  224,  223,  293,  293,  293,  209,  193,  115,  203,
      202,    0,  203,  195,  199,    0,  184,  185,  191,  195,
      194,  180,  182,  179,  190,  186,  183,  293,  139,  142,
      293,  293,  183,  175,  173,  181,    0,  176,  170,  181,
      170,  183,  178,  174,    0,  180,  165,  170,  170,  164,
      166,  170,    0,    0,    0,  162,  169,  164,  160,  164,
      146,  159,  158,  142,  146,    0,  152,  148,    0,    0,
      133,  134,  135,  142,  145,  139,  125,    0,    0,    0,

        0,    0,  134,    0,  126,  112,  113,    0,  109,    0,
        0,   64,   72,   46,   26,    0,    0,    0,  293,  203,
      205,   75
    } ;

static yyconst short int yy_def[223] =
    {   0,
      219,    1,  219,  219,  219,  219,  219,  219,  219,  220,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  221,  219,  219,  219,  221,  221,  221,  221,  221,
      221,  221,  221,  221,  221,  221,  221,  221,  219,  219,
      219,  219,  219,  219,  220,  219,  220,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  222,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  221,  219,  221,  221,  221,
      221,  221,  221,  221,  221,  221,  221,  221,  221,  221,

      221,  221,  221,  221,  221,  221,  221,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  222,  219,
      219,  219,  219,  219,  219,  219,  221,  221,  221,  221,
      221,  221,  221,  221,  221,  221,  221,  221,  221,  221,
      221,  221,  221,  221,  221,  221,  221,  219,  219,  219,
      219,  219,  221,  221,  221,  221,  221,  221,  221,  221,
      221,  221,  221,  221,  221,  221,  221,  221,  221,  221,
      221,  221,  221,  221,  221,  221,  221,  221,  221,  221,
      221,  221,  221,  221,  221,  221,  221,  221,  221,  221,
      221,  221,  221,  221,  221,  221,  221,  221,  221,  221,

      221,  221,  221,  221,  221,  221,  221,  221,  221,  221,
      221,  221,  221,  221,  221,  221,  221,  221,    0,  219,
      219,  219
    } ;

static yyconst short int yy_nxt[359] =
    {   0,
        4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
       14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
       24,   24,   25,   26,   27,   28,   29,   30,   31,   32,
       32,   32,   32,   32,   32,   33,    4,   34,   35,   32,
       36,   37,   38,   39,   40,   32,   41,   32,   32,   32,
       42,   32,   43,   44,   45,   46,   47,   32,   48,   32,
       32,   49,   50,   51,   52,   56,   59,   62,   64,   66,
       66,   66,   82,   83,   67,   76,  119,   65,   63,   68,
      218,   60,   84,   77,   78,   85,   69,   70,  217,   71,
       71,   71,   79,   80,   81,   57,   70,   92,   71,   71,

       71,   89,   73,   73,   90,   94,  108,   56,   93,   72,
       95,   73,   73,   74,  100,  216,   96,  101,   73,  215,
       72,   97,   98,  118,  118,  103,   73,   73,  104,   66,
       66,   66,  117,  117,  117,   73,  137,   57,   74,   73,
       73,  138,   70,  109,   71,   71,   71,  121,  122,  123,
      124,  139,  214,  141,  213,   73,  142,   73,   73,  117,
      117,  117,  212,   73,  140,  118,  118,  150,  150,  155,
      156,  149,  149,   73,  150,  150,  211,  210,  149,  149,
      209,   73,  208,  150,  207,  206,  205,  149,  204,  203,
      150,  150,  202,  201,  149,  149,  200,  199,  150,  198,

      197,  196,  149,   55,   55,   55,   86,   86,  195,  194,
      193,  192,  191,  190,  189,  188,  187,  186,  185,  184,
      183,  182,  181,  180,  179,  178,  177,  176,  175,  174,
      173,  172,  171,  170,  169,  168,  167,  166,  165,  164,
      163,  162,  161,  160,  159,  158,  157,  154,  153,  152,
      151,  148,  147,  146,  145,  144,  143,  136,  135,  134,
      133,  132,  131,  130,  129,  128,  127,  126,  125,  120,
      116,  115,  114,  113,  112,  111,  219,  110,  107,  106,
      105,  102,   99,   91,   88,   87,   75,   61,   58,   54,
       53,  219,    3,  219,  219,  219,  219,  219,  219,  219,

      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219
    } ;

static yyconst short int yy_chk[359] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,   10,   14,   18,   20,   21,
       21,   21,   29,   29,   22,   27,  222,   20,   18,   22,
      215,   14,   31,   27,   27,   31,   22,   24,  214,   24,
       24,   24,   28,   28,   28,   10,   23,   39,   23,   23,

       23,   37,   24,   24,   37,   40,   50,   55,   39,   23,
       40,   23,   23,   23,   43,  213,   41,   43,   24,  212,
       23,   41,   41,   72,   72,   45,   24,   23,   45,   66,
       66,   66,   70,   70,   70,   23,  100,   55,   23,   73,
       73,  100,   71,   50,   71,   71,   71,   77,   77,   83,
       83,  101,  209,  102,  207,   73,  102,   71,   71,  117,
      117,  117,  206,   73,  101,  118,  118,  119,  119,  129,
      129,  149,  149,   71,  150,  150,  205,  203,  118,  118,
      197,   71,  196,  119,  195,  194,  193,  149,  192,  191,
      150,  119,  188,  187,  118,  149,  185,  184,  150,  183,

      182,  181,  118,  220,  220,  220,  221,  221,  180,  179,
      178,  177,  176,  172,  171,  170,  169,  168,  167,  166,
      164,  163,  162,  161,  160,  159,  158,  156,  155,  154,
      153,  147,  146,  145,  144,  143,  142,  141,  140,  139,
      138,  137,  135,  134,  133,  131,  130,  128,  127,  123,
      122,  108,  107,  106,  105,  104,  103,   99,   98,   97,
       95,   94,   93,   92,   90,   89,   88,   87,   84,   76,
       69,   65,   63,   61,   60,   58,   57,   54,   48,   47,
       46,   44,   42,   38,   36,   35,   25,   17,   13,    9,
        7,    3,  219,  219,  219,  219,  219,  219,  219,  219,

      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219,  219,  219,
      219,  219,  219,  219,  219,  219,  219,  219
    } ;

static yy_state_type yy_last_accepting_state;
static char *yy_last_accepting_cpos;

/* The intent behind this definition is that it'll catch
 * any uses of REJECT which flex missed.
 */
#define REJECT reject_used_but_not_detected
#define yymore() yymore_used_but_not_detected
#define YY_MORE_ADJ 0
#define YY_RESTORE_YY_MORE_OFFSET
char *yytext;
#line 1 "chuck.lex"
#define INITIAL 0
#line 11 "chuck.lex"
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
// file: chuck.yy.c
// desc: chuck lexer
//
// author: Ge Wang (gewang.cs.princeton.edu) - generated by lex
//         Perry R. Cook (prc@cs.princeton.edu)
//
// based on the ansi C grammar by Jeff Lee, maintained by Jutta Degener 
// 
// date: Summer 2002
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include "chuck_utils.h"
#include "chuck_absyn.h"
#include "chuck_errmsg.h"

#ifndef __PLATFORM_WIN32__
  #include "chuck.tab.h"
#else
  #include "chuck_win32.h"
#endif



// globals
extern YYSTYPE yylval;
int char_pos = 1;

// define error handling
#define YY_FATAL_ERROR(msg) EM_error2( 0, msg )

#if defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif

  int yywrap(void);
  void adjust();
  c_str strip_lit( c_str str );
  c_str alloc_str( c_str str );
  long htol( c_str str );
  int comment();
  int block_comment();

#if defined(_cplusplus) || defined(__cplusplus)
}
#endif

// yywrap()
int yywrap( void )
{
    char_pos = 1;
    return 1;
}

// adjust()
void adjust()
{
    EM_tokPos = char_pos;
    char_pos += yyleng;
}

// strip
c_str strip_lit( c_str str )
{
    str[strlen(str)-1] = '\0';
    return str+1;
}

// alloc_str()
c_str alloc_str( c_str str )
{
    c_str s = (c_str)malloc( strlen(str) + 1 );
    strcpy( s, str );

    return s;
}

// to long
long htol( c_str str )
{
    char * c = str;
    unsigned long n = 0;

    // skip over 0x
    c += 2;
    while( *c )
    {
        n <<= 4; 
        switch( *c )
        {
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': case '0':
            n += *c - '0';
            break;

        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            n += *c - 'a' + 10;
            break;

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            n += *c - 'A' + 10;
            break;
        }    
        c++;
    }

    return n;
}

// block comment hack (thanks to unput/yytext_ptr inconsistency)
#define block_comment_hack loop: \
    while ((c = input()) != '*' && c != 0 && c != EOF ) \
        if( c == '\n' ) EM_newline(); \
    if( c == EOF ) adjust(); \
    else if( (c1 = input()) != '/' && c != 0 ) \
    { \
        unput(c1); \
        goto loop; \
    } \
    if( c != 0 ) adjust();

// comment hack
#define comment_hack \
    while ((c = input()) != '\n' && c != '\r' && c != 0 && c != EOF ); \
    if (c != 0) { \
       adjust(); \
       if (c == '\n') EM_newline(); \
    }

#line 676 "chuck.yy.c"

/* Macros after this point can all be overridden by user definitions in
 * section 1.
 */

#ifndef YY_SKIP_YYWRAP
#ifdef __cplusplus
extern "C" int yywrap YY_PROTO(( void ));
#else
extern int yywrap YY_PROTO(( void ));
#endif
#endif

#ifndef YY_NO_UNPUT
static void yyunput YY_PROTO(( int c, char *buf_ptr ));
#endif

#ifndef yytext_ptr
static void yy_flex_strncpy YY_PROTO(( char *, yyconst char *, int ));
#endif

#ifdef YY_NEED_STRLEN
static int yy_flex_strlen YY_PROTO(( yyconst char * ));
#endif

#ifndef YY_NO_INPUT
#ifdef __cplusplus
static int yyinput YY_PROTO(( void ));
#else
static int input YY_PROTO(( void ));
#endif
#endif

#if YY_STACK_USED
static int yy_start_stack_ptr = 0;
static int yy_start_stack_depth = 0;
static int *yy_start_stack = 0;
#ifndef YY_NO_PUSH_STATE
static void yy_push_state YY_PROTO(( int new_state ));
#endif
#ifndef YY_NO_POP_STATE
static void yy_pop_state YY_PROTO(( void ));
#endif
#ifndef YY_NO_TOP_STATE
static int yy_top_state YY_PROTO(( void ));
#endif

#else
#define YY_NO_PUSH_STATE 1
#define YY_NO_POP_STATE 1
#define YY_NO_TOP_STATE 1
#endif

#ifdef YY_MALLOC_DECL
YY_MALLOC_DECL
#else
#if __STDC__
#ifndef __cplusplus
#include <stdlib.h>
#endif
#else
/* Just try to get by without declaring the routines.  This will fail
 * miserably on non-ANSI systems for which sizeof(size_t) != sizeof(int)
 * or sizeof(void*) != sizeof(int).
 */
#endif
#endif

/* Amount of stuff to slurp up with each read. */
#ifndef YY_READ_BUF_SIZE
#define YY_READ_BUF_SIZE 8192
#endif

/* Copy whatever the last rule matched to the standard output. */

#ifndef ECHO
/* This used to be an fputs(), but since the string might contain NUL's,
 * we now use fwrite().
 */
#define ECHO (void) fwrite( yytext, yyleng, 1, yyout )
#endif

/* Gets input and stuffs it into "buf".  number of characters read, or YY_NULL,
 * is returned in "result".
 */
#ifndef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( yy_current_buffer->yy_is_interactive ) \
		{ \
		int c = '*', n; \
		for ( n = 0; n < max_size && \
			     (c = getc( yyin )) != EOF && c != '\n'; ++n ) \
			buf[n] = (char) c; \
		if ( c == '\n' ) \
			buf[n++] = (char) c; \
		if ( c == EOF && ferror( yyin ) ) \
			YY_FATAL_ERROR( "input in flex scanner failed" ); \
		result = n; \
		} \
	else \
		{ \
		errno=0; \
		while ( (result = fread(buf, 1, max_size, yyin))==0 && ferror(yyin)) \
			{ \
			if( errno != EINTR) \
				{ \
				YY_FATAL_ERROR( "input in flex scanner failed" ); \
				break; \
				} \
			errno=0; \
			clearerr(yyin); \
			} \
		}
#endif

/* No semi-colon after return; correct usage is to write "yyterminate();" -
 * we don't want an extra ';' after the "return" because that will cause
 * some compilers to complain about unreachable statements.
 */
#ifndef yyterminate
#define yyterminate() return YY_NULL
#endif

/* Number of entries by which start-condition stack grows. */
#ifndef YY_START_STACK_INCR
#define YY_START_STACK_INCR 25
#endif

/* Report a fatal error. */
#ifndef YY_FATAL_ERROR
#define YY_FATAL_ERROR(msg) yy_fatal_error( msg )
#endif

/* Default declaration of generated scanner - a define so the user can
 * easily add parameters.
 */
#ifndef YY_DECL
#define YY_DECL int yylex YY_PROTO(( void ))
#endif

/* Code executed at the beginning of each rule, after yytext and yyleng
 * have been set up.
 */
#ifndef YY_USER_ACTION
#define YY_USER_ACTION
#endif

/* Code executed at the end of each rule. */
#ifndef YY_BREAK
#define YY_BREAK break;
#endif

#define YY_RULE_SETUP \
	YY_USER_ACTION

YY_DECL
	{
	register yy_state_type yy_current_state;
	register char *yy_cp, *yy_bp;
	register int yy_act;

#line 168 "chuck.lex"


#line 841 "chuck.yy.c"

	if ( yy_init )
		{
		yy_init = 0;

#ifdef YY_USER_INIT
		YY_USER_INIT;
#endif

		if ( ! yy_start )
			yy_start = 1;	/* first start state */

		if ( ! yyin )
			yyin = stdin;

		if ( ! yyout )
			yyout = stdout;

		if ( ! yy_current_buffer )
			yy_current_buffer =
				yy_create_buffer( yyin, YY_BUF_SIZE );

		yy_load_buffer_state();
		}

	while ( 1 )		/* loops until end-of-file is reached */
		{
		yy_cp = yy_c_buf_p;

		/* Support of yytext. */
		*yy_cp = yy_hold_char;

		/* yy_bp points to the position in yy_ch_buf of the start of
		 * the current run.
		 */
		yy_bp = yy_cp;

		yy_current_state = yy_start;
yy_match:
		do
			{
			register YY_CHAR yy_c = yy_ec[YY_SC_TO_UI(*yy_cp)];
			if ( yy_accept[yy_current_state] )
				{
				yy_last_accepting_state = yy_current_state;
				yy_last_accepting_cpos = yy_cp;
				}
			while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
				{
				yy_current_state = (int) yy_def[yy_current_state];
				if ( yy_current_state >= 220 )
					yy_c = yy_meta[(unsigned int) yy_c];
				}
			yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
			++yy_cp;
			}
		while ( yy_base[yy_current_state] != 293 );

yy_find_action:
		yy_act = yy_accept[yy_current_state];
		if ( yy_act == 0 )
			{ /* have to back up */
			yy_cp = yy_last_accepting_cpos;
			yy_current_state = yy_last_accepting_state;
			yy_act = yy_accept[yy_current_state];
			}

		YY_DO_BEFORE_ACTION;


do_action:	/* This label is used only to access EOF actions. */


		switch ( yy_act )
	{ /* beginning of action switch */
			case 0: /* must back up */
			/* undo the effects of YY_DO_BEFORE_ACTION */
			*yy_cp = yy_hold_char;
			yy_cp = yy_last_accepting_cpos;
			yy_current_state = yy_last_accepting_state;
			goto yy_find_action;

case 1:
YY_RULE_SETUP
#line 170 "chuck.lex"
{ char c; adjust(); comment_hack; continue; }
	YY_BREAK
case 2:
YY_RULE_SETUP
#line 171 "chuck.lex"
{ char c; adjust(); comment_hack; continue; }
	YY_BREAK
case 3:
YY_RULE_SETUP
#line 172 "chuck.lex"
{ char c, c1; adjust(); block_comment_hack; continue; }
	YY_BREAK
case 4:
YY_RULE_SETUP
#line 173 "chuck.lex"
{ adjust(); continue; }
	YY_BREAK
case 5:
YY_RULE_SETUP
#line 174 "chuck.lex"
{ adjust(); continue; }
	YY_BREAK
case 6:
YY_RULE_SETUP
#line 175 "chuck.lex"
{ adjust(); EM_newline(); continue; }
	YY_BREAK
case 7:
YY_RULE_SETUP
#line 176 "chuck.lex"
{ adjust(); EM_newline(); continue; }
	YY_BREAK
case 8:
YY_RULE_SETUP
#line 178 "chuck.lex"
{ adjust(); return PLUSPLUS; }
	YY_BREAK
case 9:
YY_RULE_SETUP
#line 179 "chuck.lex"
{ adjust(); return MINUSMINUS; }
	YY_BREAK
case 10:
YY_RULE_SETUP
#line 181 "chuck.lex"
{ adjust(); return COMMA; }
	YY_BREAK
case 11:
YY_RULE_SETUP
#line 182 "chuck.lex"
{ adjust(); return COLON; }
	YY_BREAK
case 12:
YY_RULE_SETUP
#line 183 "chuck.lex"
{ adjust(); return DOT; }
	YY_BREAK
case 13:
YY_RULE_SETUP
#line 184 "chuck.lex"
{ adjust(); return PLUS; }
	YY_BREAK
case 14:
YY_RULE_SETUP
#line 185 "chuck.lex"
{ adjust(); return MINUS; }
	YY_BREAK
case 15:
YY_RULE_SETUP
#line 186 "chuck.lex"
{ adjust(); return TIMES; }
	YY_BREAK
case 16:
YY_RULE_SETUP
#line 187 "chuck.lex"
{ adjust(); return DIVIDE; }
	YY_BREAK
case 17:
YY_RULE_SETUP
#line 188 "chuck.lex"
{ adjust(); return PERCENT; }
	YY_BREAK
case 18:
YY_RULE_SETUP
#line 189 "chuck.lex"
{ adjust(); return POUND; }
	YY_BREAK
case 19:
YY_RULE_SETUP
#line 190 "chuck.lex"
{ adjust(); return DOLLAR; }
	YY_BREAK
case 20:
YY_RULE_SETUP
#line 192 "chuck.lex"
{ adjust(); return COLONCOLON; }
	YY_BREAK
case 21:
YY_RULE_SETUP
#line 193 "chuck.lex"
{ adjust(); return EQ; }
	YY_BREAK
case 22:
YY_RULE_SETUP
#line 194 "chuck.lex"
{ adjust(); return NEQ; }
	YY_BREAK
case 23:
YY_RULE_SETUP
#line 195 "chuck.lex"
{ adjust(); return LT; }
	YY_BREAK
case 24:
YY_RULE_SETUP
#line 196 "chuck.lex"
{ adjust(); return GT; }
	YY_BREAK
case 25:
YY_RULE_SETUP
#line 197 "chuck.lex"
{ adjust(); return LE; }
	YY_BREAK
case 26:
YY_RULE_SETUP
#line 198 "chuck.lex"
{ adjust(); return GE; }
	YY_BREAK
case 27:
YY_RULE_SETUP
#line 199 "chuck.lex"
{ adjust(); return AND; }
	YY_BREAK
case 28:
YY_RULE_SETUP
#line 200 "chuck.lex"
{ adjust(); return OR; }
	YY_BREAK
case 29:
YY_RULE_SETUP
#line 201 "chuck.lex"
{ adjust(); return S_AND; }
	YY_BREAK
case 30:
YY_RULE_SETUP
#line 202 "chuck.lex"
{ adjust(); return S_OR; }
	YY_BREAK
case 31:
YY_RULE_SETUP
#line 203 "chuck.lex"
{ adjust(); return S_XOR; }
	YY_BREAK
case 32:
YY_RULE_SETUP
#line 204 "chuck.lex"
{ adjust(); return SHIFT_RIGHT; }
	YY_BREAK
case 33:
YY_RULE_SETUP
#line 205 "chuck.lex"
{ adjust(); return SHIFT_LEFT; }
	YY_BREAK
case 34:
YY_RULE_SETUP
#line 206 "chuck.lex"
{ adjust(); return ASSIGN; }
	YY_BREAK
case 35:
YY_RULE_SETUP
#line 207 "chuck.lex"
{ adjust(); return LPAREN; }
	YY_BREAK
case 36:
YY_RULE_SETUP
#line 208 "chuck.lex"
{ adjust(); return RPAREN; }
	YY_BREAK
case 37:
YY_RULE_SETUP
#line 209 "chuck.lex"
{ adjust(); return LBRACK; }
	YY_BREAK
case 38:
YY_RULE_SETUP
#line 210 "chuck.lex"
{ adjust(); return RBRACK; }
	YY_BREAK
case 39:
YY_RULE_SETUP
#line 211 "chuck.lex"
{ adjust(); return LBRACE; }
	YY_BREAK
case 40:
YY_RULE_SETUP
#line 212 "chuck.lex"
{ adjust(); return RBRACE; }
	YY_BREAK
case 41:
YY_RULE_SETUP
#line 213 "chuck.lex"
{ adjust(); return SEMICOLON; }
	YY_BREAK
case 42:
YY_RULE_SETUP
#line 214 "chuck.lex"
{ adjust(); return QUESTION; }
	YY_BREAK
case 43:
YY_RULE_SETUP
#line 215 "chuck.lex"
{ adjust(); return EXCLAMATION; }
	YY_BREAK
case 44:
YY_RULE_SETUP
#line 216 "chuck.lex"
{ adjust(); return TILDA; }
	YY_BREAK
case 45:
YY_RULE_SETUP
#line 217 "chuck.lex"
{ adjust(); return FOR; }
	YY_BREAK
case 46:
YY_RULE_SETUP
#line 218 "chuck.lex"
{ adjust(); return WHILE; }
	YY_BREAK
case 47:
YY_RULE_SETUP
#line 219 "chuck.lex"
{ adjust(); return UNTIL; }
	YY_BREAK
case 48:
YY_RULE_SETUP
#line 220 "chuck.lex"
{ adjust(); return LOOP; }
	YY_BREAK
case 49:
YY_RULE_SETUP
#line 221 "chuck.lex"
{ adjust(); return CONTINUE; }
	YY_BREAK
case 50:
YY_RULE_SETUP
#line 222 "chuck.lex"
{ adjust(); return BREAK; }
	YY_BREAK
case 51:
YY_RULE_SETUP
#line 223 "chuck.lex"
{ adjust(); return IF; }
	YY_BREAK
case 52:
YY_RULE_SETUP
#line 224 "chuck.lex"
{ adjust(); return ELSE; }
	YY_BREAK
case 53:
YY_RULE_SETUP
#line 225 "chuck.lex"
{ adjust(); return DO; }
	YY_BREAK
case 54:
YY_RULE_SETUP
#line 226 "chuck.lex"
{ adjust(); return L_HACK; }
	YY_BREAK
case 55:
YY_RULE_SETUP
#line 227 "chuck.lex"
{ adjust(); return R_HACK; }
	YY_BREAK
case 56:
YY_RULE_SETUP
#line 229 "chuck.lex"
{ adjust(); return RETURN; }
	YY_BREAK
case 57:
YY_RULE_SETUP
#line 231 "chuck.lex"
{ adjust(); return FUNCTION; }
	YY_BREAK
case 58:
YY_RULE_SETUP
#line 232 "chuck.lex"
{ adjust(); return FUNCTION; }
	YY_BREAK
case 59:
YY_RULE_SETUP
#line 233 "chuck.lex"
{ adjust(); return NEW; }
	YY_BREAK
case 60:
YY_RULE_SETUP
#line 234 "chuck.lex"
{ adjust(); return CLASS; }
	YY_BREAK
case 61:
YY_RULE_SETUP
#line 235 "chuck.lex"
{ adjust(); return INTERFACE; }
	YY_BREAK
case 62:
YY_RULE_SETUP
#line 236 "chuck.lex"
{ adjust(); return EXTENDS; }
	YY_BREAK
case 63:
YY_RULE_SETUP
#line 237 "chuck.lex"
{ adjust(); return IMPLEMENTS; }
	YY_BREAK
case 64:
YY_RULE_SETUP
#line 238 "chuck.lex"
{ adjust(); return PUBLIC; }
	YY_BREAK
case 65:
YY_RULE_SETUP
#line 239 "chuck.lex"
{ adjust(); return PROTECTED; }
	YY_BREAK
case 66:
YY_RULE_SETUP
#line 240 "chuck.lex"
{ adjust(); return PRIVATE; }
	YY_BREAK
case 67:
YY_RULE_SETUP
#line 241 "chuck.lex"
{ adjust(); return STATIC; }
	YY_BREAK
case 68:
YY_RULE_SETUP
#line 242 "chuck.lex"
{ adjust(); return ABSTRACT; }
	YY_BREAK
case 69:
YY_RULE_SETUP
#line 243 "chuck.lex"
{ adjust(); return CONST; }
	YY_BREAK
case 70:
YY_RULE_SETUP
#line 244 "chuck.lex"
{ adjust(); return SPORK; }
	YY_BREAK
case 71:
YY_RULE_SETUP
#line 245 "chuck.lex"
{ adjust(); return TYPEOF; }
	YY_BREAK
case 72:
YY_RULE_SETUP
#line 247 "chuck.lex"
{ adjust(); return CHUCK; }
	YY_BREAK
case 73:
YY_RULE_SETUP
#line 248 "chuck.lex"
{ adjust(); return UNCHUCK; }
	YY_BREAK
case 74:
YY_RULE_SETUP
#line 249 "chuck.lex"
{ adjust(); return UNCHUCK; }
	YY_BREAK
case 75:
YY_RULE_SETUP
#line 250 "chuck.lex"
{ adjust(); return AT_CHUCK; }
	YY_BREAK
case 76:
YY_RULE_SETUP
#line 251 "chuck.lex"
{ adjust(); return PLUS_CHUCK; }
	YY_BREAK
case 77:
YY_RULE_SETUP
#line 252 "chuck.lex"
{ adjust(); return MINUS_CHUCK; }
	YY_BREAK
case 78:
YY_RULE_SETUP
#line 253 "chuck.lex"
{ adjust(); return TIMES_CHUCK; }
	YY_BREAK
case 79:
YY_RULE_SETUP
#line 254 "chuck.lex"
{ adjust(); return DIVIDE_CHUCK; }
	YY_BREAK
case 80:
YY_RULE_SETUP
#line 255 "chuck.lex"
{ adjust(); return S_AND_CHUCK; }
	YY_BREAK
case 81:
YY_RULE_SETUP
#line 256 "chuck.lex"
{ adjust(); return S_OR_CHUCK; }
	YY_BREAK
case 82:
YY_RULE_SETUP
#line 257 "chuck.lex"
{ adjust(); return S_XOR_CHUCK; }
	YY_BREAK
case 83:
YY_RULE_SETUP
#line 258 "chuck.lex"
{ adjust(); return SHIFT_RIGHT_CHUCK; }
	YY_BREAK
case 84:
YY_RULE_SETUP
#line 259 "chuck.lex"
{ adjust(); return SHIFT_LEFT_CHUCK; }
	YY_BREAK
case 85:
YY_RULE_SETUP
#line 260 "chuck.lex"
{ adjust(); return PERCENT_CHUCK; }
	YY_BREAK
case 86:
YY_RULE_SETUP
#line 261 "chuck.lex"
{ adjust(); return AT_SYM; }
	YY_BREAK
case 87:
YY_RULE_SETUP
#line 262 "chuck.lex"
{ adjust(); return ATAT_SYM; }
	YY_BREAK
case 88:
YY_RULE_SETUP
#line 264 "chuck.lex"
{ adjust(); yylval.ival=htol(yytext); return NUM; }
	YY_BREAK
case 89:
YY_RULE_SETUP
#line 265 "chuck.lex"
{ adjust(); yylval.ival=atoi(yytext); return NUM; }
	YY_BREAK
case 90:
YY_RULE_SETUP
#line 266 "chuck.lex"
{ adjust(); yylval.ival=atoi(yytext); return NUM; }
	YY_BREAK
case 91:
YY_RULE_SETUP
#line 267 "chuck.lex"
{ adjust(); yylval.fval=atof(yytext); return FLOAT; }
	YY_BREAK
case 92:
YY_RULE_SETUP
#line 268 "chuck.lex"
{ adjust(); yylval.sval=alloc_str(yytext); return ID; }
	YY_BREAK
case 93:
YY_RULE_SETUP
#line 269 "chuck.lex"
{ adjust(); yylval.sval=alloc_str(strip_lit(yytext)); return STRING_LIT; }
	YY_BREAK
case 94:
YY_RULE_SETUP
#line 271 "chuck.lex"
{ adjust(); EM_error( EM_tokPos, "illegal token" ); }
	YY_BREAK
case 95:
YY_RULE_SETUP
#line 273 "chuck.lex"
ECHO;
	YY_BREAK
#line 1399 "chuck.yy.c"
case YY_STATE_EOF(INITIAL):
	yyterminate();

	case YY_END_OF_BUFFER:
		{
		/* Amount of text matched not including the EOB char. */
		int yy_amount_of_matched_text = (int) (yy_cp - yytext_ptr) - 1;

		/* Undo the effects of YY_DO_BEFORE_ACTION. */
		*yy_cp = yy_hold_char;
		YY_RESTORE_YY_MORE_OFFSET

		if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_NEW )
			{
			/* We're scanning a new file or input source.  It's
			 * possible that this happened because the user
			 * just pointed yyin at a new source and called
			 * yylex().  If so, then we have to assure
			 * consistency between yy_current_buffer and our
			 * globals.  Here is the right place to do so, because
			 * this is the first action (other than possibly a
			 * back-up) that will match for the new input source.
			 */
			yy_n_chars = yy_current_buffer->yy_n_chars;
			yy_current_buffer->yy_input_file = yyin;
			yy_current_buffer->yy_buffer_status = YY_BUFFER_NORMAL;
			}

		/* Note that here we test for yy_c_buf_p "<=" to the position
		 * of the first EOB in the buffer, since yy_c_buf_p will
		 * already have been incremented past the NUL character
		 * (since all states make transitions on EOB to the
		 * end-of-buffer state).  Contrast this with the test
		 * in input().
		 */
		if ( yy_c_buf_p <= &yy_current_buffer->yy_ch_buf[yy_n_chars] )
			{ /* This was really a NUL. */
			yy_state_type yy_next_state;

			yy_c_buf_p = yytext_ptr + yy_amount_of_matched_text;

			yy_current_state = yy_get_previous_state();

			/* Okay, we're now positioned to make the NUL
			 * transition.  We couldn't have
			 * yy_get_previous_state() go ahead and do it
			 * for us because it doesn't know how to deal
			 * with the possibility of jamming (and we don't
			 * want to build jamming into it because then it
			 * will run more slowly).
			 */

			yy_next_state = yy_try_NUL_trans( yy_current_state );

			yy_bp = yytext_ptr + YY_MORE_ADJ;

			if ( yy_next_state )
				{
				/* Consume the NUL. */
				yy_cp = ++yy_c_buf_p;
				yy_current_state = yy_next_state;
				goto yy_match;
				}

			else
				{
				yy_cp = yy_c_buf_p;
				goto yy_find_action;
				}
			}

		else switch ( yy_get_next_buffer() )
			{
			case EOB_ACT_END_OF_FILE:
				{
				yy_did_buffer_switch_on_eof = 0;

				if ( yywrap() )
					{
					/* Note: because we've taken care in
					 * yy_get_next_buffer() to have set up
					 * yytext, we can now set up
					 * yy_c_buf_p so that if some total
					 * hoser (like flex itself) wants to
					 * call the scanner after we return the
					 * YY_NULL, it'll still work - another
					 * YY_NULL will get returned.
					 */
					yy_c_buf_p = yytext_ptr + YY_MORE_ADJ;

					yy_act = YY_STATE_EOF(YY_START);
					goto do_action;
					}

				else
					{
					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
					}
				break;
				}

			case EOB_ACT_CONTINUE_SCAN:
				yy_c_buf_p =
					yytext_ptr + yy_amount_of_matched_text;

				yy_current_state = yy_get_previous_state();

				yy_cp = yy_c_buf_p;
				yy_bp = yytext_ptr + YY_MORE_ADJ;
				goto yy_match;

			case EOB_ACT_LAST_MATCH:
				yy_c_buf_p =
				&yy_current_buffer->yy_ch_buf[yy_n_chars];

				yy_current_state = yy_get_previous_state();

				yy_cp = yy_c_buf_p;
				yy_bp = yytext_ptr + YY_MORE_ADJ;
				goto yy_find_action;
			}
		break;
		}

	default:
		YY_FATAL_ERROR(
			"fatal flex scanner internal error--no action found" );
	} /* end of action switch */
		} /* end of scanning one token */
	} /* end of yylex */


/* yy_get_next_buffer - try to read in a new buffer
 *
 * Returns a code representing an action:
 *	EOB_ACT_LAST_MATCH -
 *	EOB_ACT_CONTINUE_SCAN - continue scanning from current position
 *	EOB_ACT_END_OF_FILE - end of file
 */

static int yy_get_next_buffer()
	{
	register char *dest = yy_current_buffer->yy_ch_buf;
	register char *source = yytext_ptr;
	register int number_to_move, i;
	int ret_val;

	if ( yy_c_buf_p > &yy_current_buffer->yy_ch_buf[yy_n_chars + 1] )
		YY_FATAL_ERROR(
		"fatal flex scanner internal error--end of buffer missed" );

	if ( yy_current_buffer->yy_fill_buffer == 0 )
		{ /* Don't try to fill the buffer, so this is an EOF. */
		if ( yy_c_buf_p - yytext_ptr - YY_MORE_ADJ == 1 )
			{
			/* We matched a single character, the EOB, so
			 * treat this as a final EOF.
			 */
			return EOB_ACT_END_OF_FILE;
			}

		else
			{
			/* We matched some text prior to the EOB, first
			 * process it.
			 */
			return EOB_ACT_LAST_MATCH;
			}
		}

	/* Try to read more data. */

	/* First move last chars to start of buffer. */
	number_to_move = (int) (yy_c_buf_p - yytext_ptr) - 1;

	for ( i = 0; i < number_to_move; ++i )
		*(dest++) = *(source++);

	if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_EOF_PENDING )
		/* don't do the read, it's not guaranteed to return an EOF,
		 * just force an EOF
		 */
		yy_current_buffer->yy_n_chars = yy_n_chars = 0;

	else
		{
		int num_to_read =
			yy_current_buffer->yy_buf_size - number_to_move - 1;

		while ( num_to_read <= 0 )
			{ /* Not enough room in the buffer - grow it. */
#ifdef YY_USES_REJECT
			YY_FATAL_ERROR(
"input buffer overflow, can't enlarge buffer because scanner uses REJECT" );
#else

			/* just a shorter name for the current buffer */
			YY_BUFFER_STATE b = yy_current_buffer;

			int yy_c_buf_p_offset =
				(int) (yy_c_buf_p - b->yy_ch_buf);

			if ( b->yy_is_our_buffer )
				{
				int new_size = b->yy_buf_size * 2;

				if ( new_size <= 0 )
					b->yy_buf_size += b->yy_buf_size / 8;
				else
					b->yy_buf_size *= 2;

				b->yy_ch_buf = (char *)
					/* Include room in for 2 EOB chars. */
					yy_flex_realloc( (void *) b->yy_ch_buf,
							 b->yy_buf_size + 2 );
				}
			else
				/* Can't grow it, we don't own it. */
				b->yy_ch_buf = 0;

			if ( ! b->yy_ch_buf )
				YY_FATAL_ERROR(
				"fatal error - scanner input buffer overflow" );

			yy_c_buf_p = &b->yy_ch_buf[yy_c_buf_p_offset];

			num_to_read = yy_current_buffer->yy_buf_size -
						number_to_move - 1;
#endif
			}

		if ( num_to_read > YY_READ_BUF_SIZE )
			num_to_read = YY_READ_BUF_SIZE;

		/* Read in more data. */
		YY_INPUT( (&yy_current_buffer->yy_ch_buf[number_to_move]),
			yy_n_chars, num_to_read );

		yy_current_buffer->yy_n_chars = yy_n_chars;
		}

	if ( yy_n_chars == 0 )
		{
		if ( number_to_move == YY_MORE_ADJ )
			{
			ret_val = EOB_ACT_END_OF_FILE;
			yyrestart( yyin );
			}

		else
			{
			ret_val = EOB_ACT_LAST_MATCH;
			yy_current_buffer->yy_buffer_status =
				YY_BUFFER_EOF_PENDING;
			}
		}

	else
		ret_val = EOB_ACT_CONTINUE_SCAN;

	yy_n_chars += number_to_move;
	yy_current_buffer->yy_ch_buf[yy_n_chars] = YY_END_OF_BUFFER_CHAR;
	yy_current_buffer->yy_ch_buf[yy_n_chars + 1] = YY_END_OF_BUFFER_CHAR;

	yytext_ptr = &yy_current_buffer->yy_ch_buf[0];

	return ret_val;
	}


/* yy_get_previous_state - get the state just before the EOB char was reached */

static yy_state_type yy_get_previous_state()
	{
	register yy_state_type yy_current_state;
	register char *yy_cp;

	yy_current_state = yy_start;

	for ( yy_cp = yytext_ptr + YY_MORE_ADJ; yy_cp < yy_c_buf_p; ++yy_cp )
		{
		register YY_CHAR yy_c = (*yy_cp ? yy_ec[YY_SC_TO_UI(*yy_cp)] : 1);
		if ( yy_accept[yy_current_state] )
			{
			yy_last_accepting_state = yy_current_state;
			yy_last_accepting_cpos = yy_cp;
			}
		while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
			{
			yy_current_state = (int) yy_def[yy_current_state];
			if ( yy_current_state >= 220 )
				yy_c = yy_meta[(unsigned int) yy_c];
			}
		yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
		}

	return yy_current_state;
	}


/* yy_try_NUL_trans - try to make a transition on the NUL character
 *
 * synopsis
 *	next_state = yy_try_NUL_trans( current_state );
 */

#ifdef YY_USE_PROTOS
static yy_state_type yy_try_NUL_trans( yy_state_type yy_current_state )
#else
static yy_state_type yy_try_NUL_trans( yy_current_state )
yy_state_type yy_current_state;
#endif
	{
	register int yy_is_jam;
	register char *yy_cp = yy_c_buf_p;

	register YY_CHAR yy_c = 1;
	if ( yy_accept[yy_current_state] )
		{
		yy_last_accepting_state = yy_current_state;
		yy_last_accepting_cpos = yy_cp;
		}
	while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
		{
		yy_current_state = (int) yy_def[yy_current_state];
		if ( yy_current_state >= 220 )
			yy_c = yy_meta[(unsigned int) yy_c];
		}
	yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
	yy_is_jam = (yy_current_state == 219);

	return yy_is_jam ? 0 : yy_current_state;
	}


#ifndef YY_NO_UNPUT
#ifdef YY_USE_PROTOS
static void yyunput( int c, register char *yy_bp )
#else
static void yyunput( c, yy_bp )
int c;
register char *yy_bp;
#endif
	{
	register char *yy_cp = yy_c_buf_p;

	/* undo effects of setting up yytext */
	*yy_cp = yy_hold_char;

	if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
		{ /* need to shift things up to make room */
		/* +2 for EOB chars. */
		register int number_to_move = yy_n_chars + 2;
		register char *dest = &yy_current_buffer->yy_ch_buf[
					yy_current_buffer->yy_buf_size + 2];
		register char *source =
				&yy_current_buffer->yy_ch_buf[number_to_move];

		while ( source > yy_current_buffer->yy_ch_buf )
			*--dest = *--source;

		yy_cp += (int) (dest - source);
		yy_bp += (int) (dest - source);
		yy_current_buffer->yy_n_chars =
			yy_n_chars = yy_current_buffer->yy_buf_size;

		if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
			YY_FATAL_ERROR( "flex scanner push-back overflow" );
		}

	*--yy_cp = (char) c;


	yytext_ptr = yy_bp;
	yy_hold_char = *yy_cp;
	yy_c_buf_p = yy_cp;
	}
#endif	/* ifndef YY_NO_UNPUT */


#ifdef __cplusplus
static int yyinput()
#else
static int input()
#endif
	{
	int c;

	*yy_c_buf_p = yy_hold_char;

	if ( *yy_c_buf_p == YY_END_OF_BUFFER_CHAR )
		{
		/* yy_c_buf_p now points to the character we want to return.
		 * If this occurs *before* the EOB characters, then it's a
		 * valid NUL; if not, then we've hit the end of the buffer.
		 */
		if ( yy_c_buf_p < &yy_current_buffer->yy_ch_buf[yy_n_chars] )
			/* This was really a NUL. */
			*yy_c_buf_p = '\0';

		else
			{ /* need more input */
			int offset = yy_c_buf_p - yytext_ptr;
			++yy_c_buf_p;

			switch ( yy_get_next_buffer() )
				{
				case EOB_ACT_LAST_MATCH:
					/* This happens because yy_g_n_b()
					 * sees that we've accumulated a
					 * token and flags that we need to
					 * try matching the token before
					 * proceeding.  But for input(),
					 * there's no matching to consider.
					 * So convert the EOB_ACT_LAST_MATCH
					 * to EOB_ACT_END_OF_FILE.
					 */

					/* Reset buffer status. */
					yyrestart( yyin );

					/* fall through */

				case EOB_ACT_END_OF_FILE:
					{
					if ( yywrap() )
						return EOF;

					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
#ifdef __cplusplus
					return yyinput();
#else
					return input();
#endif
					}

				case EOB_ACT_CONTINUE_SCAN:
					yy_c_buf_p = yytext_ptr + offset;
					break;
				}
			}
		}

	c = *(unsigned char *) yy_c_buf_p;	/* cast for 8-bit char's */
	*yy_c_buf_p = '\0';	/* preserve yytext */
	yy_hold_char = *++yy_c_buf_p;


	return c;
	}


#ifdef YY_USE_PROTOS
void yyrestart( FILE *input_file )
#else
void yyrestart( input_file )
FILE *input_file;
#endif
	{
	if ( ! yy_current_buffer )
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE );

	yy_init_buffer( yy_current_buffer, input_file );
	yy_load_buffer_state();
	}


#ifdef YY_USE_PROTOS
void yy_switch_to_buffer( YY_BUFFER_STATE new_buffer )
#else
void yy_switch_to_buffer( new_buffer )
YY_BUFFER_STATE new_buffer;
#endif
	{
	if ( yy_current_buffer == new_buffer )
		return;

	if ( yy_current_buffer )
		{
		/* Flush out information for old buffer. */
		*yy_c_buf_p = yy_hold_char;
		yy_current_buffer->yy_buf_pos = yy_c_buf_p;
		yy_current_buffer->yy_n_chars = yy_n_chars;
		}

	yy_current_buffer = new_buffer;
	yy_load_buffer_state();

	/* We don't actually know whether we did this switch during
	 * EOF (yywrap()) processing, but the only time this flag
	 * is looked at is after yywrap() is called, so it's safe
	 * to go ahead and always set it.
	 */
	yy_did_buffer_switch_on_eof = 1;
	}


#ifdef YY_USE_PROTOS
void yy_load_buffer_state( void )
#else
void yy_load_buffer_state()
#endif
	{
	yy_n_chars = yy_current_buffer->yy_n_chars;
	yytext_ptr = yy_c_buf_p = yy_current_buffer->yy_buf_pos;
	yyin = yy_current_buffer->yy_input_file;
	yy_hold_char = *yy_c_buf_p;
	}


#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_create_buffer( FILE *file, int size )
#else
YY_BUFFER_STATE yy_create_buffer( file, size )
FILE *file;
int size;
#endif
	{
	YY_BUFFER_STATE b;

	b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
	if ( ! b )
		YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

	b->yy_buf_size = size;

	/* yy_ch_buf has to be 2 characters longer than the size given because
	 * we need to put in 2 end-of-buffer characters.
	 */
	b->yy_ch_buf = (char *) yy_flex_alloc( b->yy_buf_size + 2 );
	if ( ! b->yy_ch_buf )
		YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

	b->yy_is_our_buffer = 1;

	yy_init_buffer( b, file );

	return b;
	}


#ifdef YY_USE_PROTOS
void yy_delete_buffer( YY_BUFFER_STATE b )
#else
void yy_delete_buffer( b )
YY_BUFFER_STATE b;
#endif
	{
	if ( ! b )
		return;

	if ( b == yy_current_buffer )
		yy_current_buffer = (YY_BUFFER_STATE) 0;

	if ( b->yy_is_our_buffer )
		yy_flex_free( (void *) b->yy_ch_buf );

	yy_flex_free( (void *) b );
	}


#ifndef _WIN32
#include <unistd.h>
#else
#ifndef YY_ALWAYS_INTERACTIVE
#ifndef YY_NEVER_INTERACTIVE
extern int isatty YY_PROTO(( int ));
#endif
#endif
#endif

#ifdef YY_USE_PROTOS
void yy_init_buffer( YY_BUFFER_STATE b, FILE *file )
#else
void yy_init_buffer( b, file )
YY_BUFFER_STATE b;
FILE *file;
#endif


	{
	yy_flush_buffer( b );

	b->yy_input_file = file;
	b->yy_fill_buffer = 1;

#if YY_ALWAYS_INTERACTIVE
	b->yy_is_interactive = 1;
#else
#if YY_NEVER_INTERACTIVE
	b->yy_is_interactive = 0;
#else
	b->yy_is_interactive = file ? (isatty( fileno(file) ) > 0) : 0;
#endif
#endif
	}


#ifdef YY_USE_PROTOS
void yy_flush_buffer( YY_BUFFER_STATE b )
#else
void yy_flush_buffer( b )
YY_BUFFER_STATE b;
#endif

	{
	if ( ! b )
		return;

	b->yy_n_chars = 0;

	/* We always need two end-of-buffer characters.  The first causes
	 * a transition to the end-of-buffer state.  The second causes
	 * a jam in that state.
	 */
	b->yy_ch_buf[0] = YY_END_OF_BUFFER_CHAR;
	b->yy_ch_buf[1] = YY_END_OF_BUFFER_CHAR;

	b->yy_buf_pos = &b->yy_ch_buf[0];

	b->yy_at_bol = 1;
	b->yy_buffer_status = YY_BUFFER_NEW;

	if ( b == yy_current_buffer )
		yy_load_buffer_state();
	}


#ifndef YY_NO_SCAN_BUFFER
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_buffer( char *base, yy_size_t size )
#else
YY_BUFFER_STATE yy_scan_buffer( base, size )
char *base;
yy_size_t size;
#endif
	{
	YY_BUFFER_STATE b;

	if ( size < 2 ||
	     base[size-2] != YY_END_OF_BUFFER_CHAR ||
	     base[size-1] != YY_END_OF_BUFFER_CHAR )
		/* They forgot to leave room for the EOB's. */
		return 0;

	b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
	if ( ! b )
		YY_FATAL_ERROR( "out of dynamic memory in yy_scan_buffer()" );

	b->yy_buf_size = size - 2;	/* "- 2" to take care of EOB's */
	b->yy_buf_pos = b->yy_ch_buf = base;
	b->yy_is_our_buffer = 0;
	b->yy_input_file = 0;
	b->yy_n_chars = b->yy_buf_size;
	b->yy_is_interactive = 0;
	b->yy_at_bol = 1;
	b->yy_fill_buffer = 0;
	b->yy_buffer_status = YY_BUFFER_NEW;

	yy_switch_to_buffer( b );

	return b;
	}
#endif


#ifndef YY_NO_SCAN_STRING
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_string( yyconst char *yy_str )
#else
YY_BUFFER_STATE yy_scan_string( yy_str )
yyconst char *yy_str;
#endif
	{
	int len;
	for ( len = 0; yy_str[len]; ++len )
		;

	return yy_scan_bytes( yy_str, len );
	}
#endif


#ifndef YY_NO_SCAN_BYTES
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_bytes( yyconst char *bytes, int len )
#else
YY_BUFFER_STATE yy_scan_bytes( bytes, len )
yyconst char *bytes;
int len;
#endif
	{
	YY_BUFFER_STATE b;
	char *buf;
	yy_size_t n;
	int i;

	/* Get memory for full buffer, including space for trailing EOB's. */
	n = len + 2;
	buf = (char *) yy_flex_alloc( n );
	if ( ! buf )
		YY_FATAL_ERROR( "out of dynamic memory in yy_scan_bytes()" );

	for ( i = 0; i < len; ++i )
		buf[i] = bytes[i];

	buf[len] = buf[len+1] = YY_END_OF_BUFFER_CHAR;

	b = yy_scan_buffer( buf, n );
	if ( ! b )
		YY_FATAL_ERROR( "bad buffer in yy_scan_bytes()" );

	/* It's okay to grow etc. this buffer, and we should throw it
	 * away when we're done.
	 */
	b->yy_is_our_buffer = 1;

	return b;
	}
#endif


#ifndef YY_NO_PUSH_STATE
#ifdef YY_USE_PROTOS
static void yy_push_state( int new_state )
#else
static void yy_push_state( new_state )
int new_state;
#endif
	{
	if ( yy_start_stack_ptr >= yy_start_stack_depth )
		{
		yy_size_t new_size;

		yy_start_stack_depth += YY_START_STACK_INCR;
		new_size = yy_start_stack_depth * sizeof( int );

		if ( ! yy_start_stack )
			yy_start_stack = (int *) yy_flex_alloc( new_size );

		else
			yy_start_stack = (int *) yy_flex_realloc(
					(void *) yy_start_stack, new_size );

		if ( ! yy_start_stack )
			YY_FATAL_ERROR(
			"out of memory expanding start-condition stack" );
		}

	yy_start_stack[yy_start_stack_ptr++] = YY_START;

	BEGIN(new_state);
	}
#endif


#ifndef YY_NO_POP_STATE
static void yy_pop_state()
	{
	if ( --yy_start_stack_ptr < 0 )
		YY_FATAL_ERROR( "start-condition stack underflow" );

	BEGIN(yy_start_stack[yy_start_stack_ptr]);
	}
#endif


#ifndef YY_NO_TOP_STATE
static int yy_top_state()
	{
	return yy_start_stack[yy_start_stack_ptr - 1];
	}
#endif

#ifndef YY_EXIT_FAILURE
#define YY_EXIT_FAILURE 2
#endif

#ifdef YY_USE_PROTOS
static void yy_fatal_error( yyconst char msg[] )
#else
static void yy_fatal_error( msg )
char msg[];
#endif
	{
	(void) fprintf( stderr, "%s\n", msg );
	exit( YY_EXIT_FAILURE );
	}



/* Redefine yyless() so it works in section 3 code. */

#undef yyless
#define yyless(n) \
	do \
		{ \
		/* Undo effects of setting up yytext. */ \
		yytext[yyleng] = yy_hold_char; \
		yy_c_buf_p = yytext + n; \
		yy_hold_char = *yy_c_buf_p; \
		*yy_c_buf_p = '\0'; \
		yyleng = n; \
		} \
	while ( 0 )


/* Internal utility routines. */

#ifndef yytext_ptr
#ifdef YY_USE_PROTOS
static void yy_flex_strncpy( char *s1, yyconst char *s2, int n )
#else
static void yy_flex_strncpy( s1, s2, n )
char *s1;
yyconst char *s2;
int n;
#endif
	{
	register int i;
	for ( i = 0; i < n; ++i )
		s1[i] = s2[i];
	}
#endif

#ifdef YY_NEED_STRLEN
#ifdef YY_USE_PROTOS
static int yy_flex_strlen( yyconst char *s )
#else
static int yy_flex_strlen( s )
yyconst char *s;
#endif
	{
	register int n;
	for ( n = 0; s[n]; ++n )
		;

	return n;
	}
#endif


#ifdef YY_USE_PROTOS
static void *yy_flex_alloc( yy_size_t size )
#else
static void *yy_flex_alloc( size )
yy_size_t size;
#endif
	{
	return (void *) malloc( size );
	}

#ifdef YY_USE_PROTOS
static void *yy_flex_realloc( void *ptr, yy_size_t size )
#else
static void *yy_flex_realloc( ptr, size )
void *ptr;
yy_size_t size;
#endif
	{
	/* The cast to (char *) in the following accommodates both
	 * implementations that use char* generic pointers, and those
	 * that use void* generic pointers.  It works with the latter
	 * because both ANSI C and C++ allow castless assignment from
	 * any pointer type to void*, and deal with argument conversions
	 * as though doing an assignment.
	 */
	return (void *) realloc( (char *) ptr, size );
	}

#ifdef YY_USE_PROTOS
static void yy_flex_free( void *ptr )
#else
static void yy_flex_free( ptr )
void *ptr;
#endif
	{
	free( ptr );
	}

#if YY_MAIN
int main()
	{
	yylex();
	return 0;
	}
#endif
#line 273 "chuck.lex"


/*
// comment
int comment()
{
    char c;

    while ((c = yyinput()) != '\n' && c != '\r' && c != 0 && c != EOF );

    if (c != 0) { 
       adjust(); 
       if (c == '\n') EM_newline();
    }
    
    return 0;
}

// block comment
int block_comment()
{
    char c, c1;

loop:
    while ((c = yyinput()) != '*' && c != 0 && c != EOF )
        if( c == '\n' ) EM_newline();

    if( c == EOF )
    {
        adjust();
        return 1;
    }
    
    if( (c1 = yyinput()) != '/' && c != 0 )
    {
        unput(c1);
        goto loop;
    }
    
    if( c != 0 ) adjust();

    return 0;
}
*/
