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
     ABSTRACT = 350,
     CONST = 351,
     SPORK = 352,
     L_NSPC = 353,
     R_NSPC = 354
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
#define ABSTRACT 350
#define CONST 351
#define SPORK 352
#define L_NSPC 353
#define R_NSPC 354




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
#line 353 "chuck.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 365 "chuck.tab.c"

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
#define YYFINAL  97
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   823

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  48
/* YYNRULES -- Number of rules. */
#define YYNRULES  149
/* YYNRULES -- Number of states. */
#define YYNSTATES  272

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   354

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
      95,    96,    97,    98,    99
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    20,    27,
      33,    40,    43,    48,    51,    56,    58,    61,    63,    65,
      68,    70,    74,    76,    85,    93,   102,   110,   112,   114,
     116,   118,   120,   122,   123,   125,   128,   130,   133,   136,
     141,   143,   146,   148,   150,   152,   154,   156,   159,   163,
     169,   177,   183,   191,   198,   206,   212,   220,   223,   227,
     229,   232,   234,   238,   240,   244,   248,   253,   256,   260,
     262,   265,   268,   270,   274,   276,   279,   282,   284,   286,
     288,   290,   292,   294,   296,   298,   300,   302,   304,   310,
     312,   316,   318,   322,   324,   328,   330,   334,   336,   340,
     342,   346,   350,   352,   356,   360,   364,   368,   370,   374,
     378,   380,   384,   388,   390,   394,   398,   402,   404,   408,
     410,   415,   417,   420,   423,   426,   431,   436,   439,   444,
     446,   448,   450,   452,   454,   457,   459,   462,   466,   471,
     475,   478,   481,   485,   487,   489,   491,   493,   496,   500
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     101,     0,    -1,   102,    -1,   102,   101,    -1,   116,    -1,
     110,    -1,   103,    -1,    87,   108,    15,   105,    16,    -1,
      87,   108,   104,    15,   105,    16,    -1,    88,   108,    15,
     105,    16,    -1,    88,   108,   107,    15,   105,    16,    -1,
      90,   108,    -1,    90,   108,    89,     3,    -1,    89,     3,
      -1,    89,     3,    90,   108,    -1,   106,    -1,   106,   105,
      -1,   116,    -1,   110,    -1,    89,   108,    -1,     3,    -1,
       3,     8,   108,    -1,     3,    -1,   111,   112,   114,     3,
      11,   115,    12,   121,    -1,   111,   112,   114,     3,    11,
      12,   121,    -1,   111,   112,   114,     3,    11,   115,    12,
      10,    -1,   111,   112,   114,     3,    11,    12,    10,    -1,
      40,    -1,    91,    -1,    92,    -1,    93,    -1,    94,    -1,
      95,    -1,    -1,   109,    -1,   109,    62,    -1,   113,    -1,
     113,   126,    -1,   113,   129,    -1,   113,   129,     8,   115,
      -1,   117,    -1,   117,   116,    -1,   122,    -1,   120,    -1,
     119,    -1,   118,    -1,   121,    -1,    41,    10,    -1,    41,
     123,    10,    -1,    32,    11,   123,    12,   117,    -1,    32,
      11,   123,    12,   117,    34,   117,    -1,    35,    11,   123,
      12,   117,    -1,    37,   117,    35,    11,   123,    12,    10,
      -1,    36,    11,   122,   122,    12,   117,    -1,    36,    11,
     122,   122,   123,    12,   117,    -1,    57,    11,   123,    12,
     117,    -1,    37,   117,    57,    11,   123,    12,    10,    -1,
      15,    16,    -1,    15,   116,    16,    -1,    10,    -1,   123,
      10,    -1,   124,    -1,   124,     8,   123,    -1,   127,    -1,
     124,   130,   127,    -1,    13,   123,    14,    -1,    13,   123,
      14,   125,    -1,    13,    14,    -1,   126,    13,    14,    -1,
     131,    -1,   113,   128,    -1,    67,   128,    -1,   129,    -1,
     129,     8,   128,    -1,     3,    -1,     3,   125,    -1,     3,
     126,    -1,    81,    -1,    84,    -1,    68,    -1,    69,    -1,
      70,    -1,    71,    -1,    75,    -1,    76,    -1,    77,    -1,
      86,    -1,   132,    -1,   132,    42,   123,     9,   131,    -1,
     133,    -1,   132,    30,   133,    -1,   134,    -1,   133,    29,
     134,    -1,   135,    -1,   134,    44,   135,    -1,   136,    -1,
     135,    46,   136,    -1,   137,    -1,   136,    45,   137,    -1,
     138,    -1,   137,    23,   138,    -1,   137,    24,   138,    -1,
     139,    -1,   138,    25,   139,    -1,   138,    27,   139,    -1,
     138,    26,   139,    -1,   138,    28,   139,    -1,   140,    -1,
     139,    79,   140,    -1,   139,    78,   140,    -1,   141,    -1,
     140,    18,   141,    -1,   140,    19,   141,    -1,   142,    -1,
     141,    20,   142,    -1,   141,    21,   142,    -1,   141,    22,
     142,    -1,   143,    -1,   142,    80,   143,    -1,   144,    -1,
      25,   114,    27,   143,    -1,   146,    -1,    47,   144,    -1,
      48,   144,    -1,   145,   143,    -1,    66,    25,   123,    27,
      -1,    65,    25,   144,    27,    -1,    64,   113,    -1,    64,
      25,   113,    27,    -1,    18,    -1,    19,    -1,    80,    -1,
      43,    -1,    20,    -1,    97,    80,    -1,   147,    -1,   146,
     125,    -1,   146,    11,    12,    -1,   146,    11,   123,    12,
      -1,   146,    17,     3,    -1,   146,    47,    -1,   146,    48,
      -1,   146,    82,   147,    -1,     3,    -1,     5,    -1,     6,
      -1,     4,    -1,    98,    99,    -1,    98,     3,    99,    -1,
      11,   123,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   166,   166,   167,   171,   172,   173,   177,   179,   181,
     183,   188,   189,   190,   191,   195,   196,   200,   201,   205,
     209,   210,   214,   219,   221,   223,   225,   230,   231,   232,
     233,   237,   238,   239,   243,   244,   248,   249,   253,   254,
     258,   259,   263,   264,   265,   266,   268,   272,   273,   277,
     279,   284,   286,   288,   290,   292,   294,   299,   300,   304,
     305,   309,   310,   314,   315,   320,   321,   326,   327,   331,
     332,   333,   337,   338,   342,   343,   344,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   365,   366,   371,
     372,   377,   378,   383,   384,   389,   390,   395,   396,   401,
     402,   404,   409,   410,   412,   414,   416,   421,   422,   424,
     429,   430,   432,   437,   438,   440,   442,   447,   448,   453,
     454,   459,   460,   462,   464,   466,   468,   470,   472,   477,
     478,   479,   480,   481,   482,   487,   488,   490,   492,   494,
     496,   498,   500,   505,   506,   507,   508,   509,   510,   511
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
  "IF", "THEN", "ELSE", "WHILE", "FOR", "DO", "BREAK", "NULL_TOK", 
  "FUNCTION", "RETURN", "QUESTION", "EXCLAMATION", "S_OR", "S_AND", 
  "S_XOR", "PLUSPLUS", "MINUSMINUS", "SIMULT", "PATTERN", "CODE", 
  "TRANSPORT", "HOST", "TIME", "WHENEVER", "NEXT", "UNTIL", "EVERY", 
  "BEFORE", "AFTER", "AT", "AT_SYM", "ATAT_SYM", "NEW", "SIZEOF", 
  "TYPEOF", "SAME", "PLUS_CHUCK", "MINUS_CHUCK", "TIMES_CHUCK", 
  "DIVIDE_CHUCK", "S_AND_CHUCK", "S_OR_CHUCK", "S_XOR_CHUCK", 
  "SHIFT_RIGHT_CHUCK", "SHIFT_LEFT_CHUCK", "PERCENT_CHUCK", "SHIFT_RIGHT", 
  "SHIFT_LEFT", "TILDA", "CHUCK", "COLONCOLON", "S_CHUCK", "AT_CHUCK", 
  "LEFT_S_CHUCK", "UNCHUCK", "CLASS", "INTERFACE", "EXTENDS", 
  "IMPLEMENTS", "PUBLIC", "PROTECTED", "PRIVATE", "STATIC", "ABSTRACT", 
  "CONST", "SPORK", "L_NSPC", "R_NSPC", "$accept", "program", 
  "program_section", "class_definition", "class_ext", "class_body", 
  "class_section", "iface_ext", "id_list", "id_dot", 
  "function_definition", "function_decl", "static_decl", "type_decl", 
  "type_decl2", "arg_list", "statement_list", "statement", 
  "jump_statement", "selection_statement", "loop_statement", 
  "code_segment", "expression_statement", "expression", 
  "chuck_expression", "array_exp", "array_empty", "decl_expression", 
  "var_decl_list", "var_decl", "chuck_operator", "conditional_expression", 
  "logical_or_expression", "logical_and_expression", 
  "inclusive_or_expression", "exclusive_or_expression", "and_expression", 
  "equality_expression", "relational_expression", "shift_expression", 
  "additive_expression", "multiplicative_expression", "tilda_expression", 
  "cast_expression", "unary_expression", "unary_operator", 
  "postfix_expression", "primary_expression", 0
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
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   100,   101,   101,   102,   102,   102,   103,   103,   103,
     103,   104,   104,   104,   104,   105,   105,   106,   106,   107,
     108,   108,   109,   110,   110,   110,   110,   111,   111,   111,
     111,   112,   112,   112,   113,   113,   114,   114,   115,   115,
     116,   116,   117,   117,   117,   117,   117,   118,   118,   119,
     119,   120,   120,   120,   120,   120,   120,   121,   121,   122,
     122,   123,   123,   124,   124,   125,   125,   126,   126,   127,
     127,   127,   128,   128,   129,   129,   129,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   131,   131,   132,
     132,   133,   133,   134,   134,   135,   135,   136,   136,   137,
     137,   137,   138,   138,   138,   138,   138,   139,   139,   139,
     140,   140,   140,   141,   141,   141,   141,   142,   142,   143,
     143,   144,   144,   144,   144,   144,   144,   144,   144,   145,
     145,   145,   145,   145,   145,   146,   146,   146,   146,   146,
     146,   146,   146,   147,   147,   147,   147,   147,   147,   147
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     5,     6,     5,
       6,     2,     4,     2,     4,     1,     2,     1,     1,     2,
       1,     3,     1,     8,     7,     8,     7,     1,     1,     1,
       1,     1,     1,     0,     1,     2,     1,     2,     2,     4,
       1,     2,     1,     1,     1,     1,     1,     2,     3,     5,
       7,     5,     7,     6,     7,     5,     7,     2,     3,     1,
       2,     1,     3,     1,     3,     3,     4,     2,     3,     1,
       2,     2,     1,     3,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     5,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     3,     1,
       4,     1,     2,     2,     2,     4,     4,     2,     4,     1,
       1,     1,     1,     1,     2,     1,     2,     3,     4,     3,
       2,     2,     3,     1,     1,     1,     1,     2,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,   143,   146,   144,   145,    59,     0,     0,   129,   130,
     133,     0,     0,     0,     0,     0,    27,     0,   132,     0,
       0,     0,     0,     0,     0,     0,   131,     0,     0,    28,
      29,    30,     0,     0,     0,     2,     6,    34,     5,    33,
       0,     4,    40,    45,    44,    43,    46,    42,     0,    61,
      63,    69,    87,    89,    91,    93,    95,    97,    99,   102,
     107,   110,   113,   117,   119,     0,   121,   135,     0,    57,
       0,    22,    36,     0,     0,     0,     0,     0,    47,     0,
     143,   122,   123,     0,     0,   127,     0,     0,    74,    71,
      72,    20,     0,     0,   134,     0,   147,     1,     3,    35,
      31,    32,     0,    70,    41,    60,     0,    79,    80,    81,
      82,    83,    84,    85,    77,    78,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   124,     0,
       0,     0,   140,   141,     0,   136,   149,    58,     0,    37,
       0,     0,     0,     0,     0,     0,    48,     0,     0,     0,
       0,     0,    75,    76,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   148,     0,    62,    64,    90,     0,    92,
      94,    96,    98,   100,   101,   103,   105,   104,   106,   109,
     108,   111,   112,   114,   115,   116,   118,   137,     0,     0,
     139,   142,    67,     0,   120,     0,     0,     0,     0,     0,
       0,   128,   126,   125,    73,    21,     0,    15,    18,    17,
      13,    11,     0,     0,    19,     0,     0,     0,   138,    65,
      68,    49,    51,     0,     0,     0,     0,    55,     7,    16,
       0,     0,     0,     9,     0,     0,    88,    66,     0,    53,
       0,     0,     0,    14,    12,     8,    10,     0,     0,     0,
      50,    54,    52,    56,    26,    24,    38,     0,     0,    25,
      23,    39
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    34,    35,    36,   169,   216,   217,   172,    92,    37,
     218,    39,   102,    40,    73,   259,   219,    42,    43,    44,
      45,    46,    47,    48,    49,   145,   149,    50,    89,    90,
     117,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -224
static const short yypact[] =
{
      21,    19,  -224,  -224,  -224,  -224,   655,   288,  -224,  -224,
    -224,    16,    34,    44,    66,   355,  -224,   420,  -224,   725,
     725,   100,    26,    25,    95,   131,  -224,   132,   132,  -224,
    -224,  -224,    62,     3,   143,    21,  -224,    84,  -224,   -20,
     131,  -224,   355,  -224,  -224,  -224,  -224,  -224,   137,    85,
    -224,  -224,    -9,   119,   105,   111,   114,    36,    79,    18,
      81,    45,    70,  -224,  -224,   690,   104,  -224,   151,  -224,
     148,  -224,   152,   140,   655,   655,   455,    -5,  -224,   158,
    -224,  -224,  -224,   655,    16,  -224,   725,   655,   157,  -224,
     164,   165,    -7,     5,  -224,    75,  -224,  -224,  -224,  -224,
    -224,  -224,    16,  -224,  -224,  -224,   655,  -224,  -224,  -224,
    -224,  -224,  -224,  -224,  -224,  -224,  -224,   655,   690,   655,
     690,   690,   690,   690,   690,   690,   690,   690,   690,   690,
     690,   690,   690,   690,   690,   690,   690,   690,  -224,   520,
     655,   172,  -224,  -224,    12,  -224,  -224,  -224,   162,   166,
     690,   168,   169,   455,   167,   171,  -224,   173,   150,   156,
     160,   555,  -224,   166,   131,   132,   204,   181,   132,   174,
     204,   132,   175,  -224,   185,  -224,  -224,   119,   182,   105,
     111,   114,    36,    79,    79,    18,    18,    18,    18,    81,
      81,    45,    45,    70,    70,    70,  -224,  -224,   183,   180,
    -224,  -224,  -224,   184,  -224,   355,   355,   620,   655,   655,
     355,  -224,  -224,  -224,  -224,  -224,   186,   204,  -224,  -224,
     106,   110,   204,   187,  -224,   204,   189,   690,  -224,   188,
    -224,   177,  -224,   355,   200,   205,   206,  -224,  -224,  -224,
     132,   213,   209,  -224,   210,    35,  -224,  -224,   355,  -224,
     355,   211,   217,  -224,  -224,  -224,  -224,    27,   131,   216,
    -224,  -224,  -224,  -224,  -224,  -224,   222,    39,    16,  -224,
    -224,  -224
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -224,   196,  -224,  -224,  -224,  -127,  -224,  -224,   -27,  -224,
      13,  -224,  -224,   -11,   130,   -30,    28,   -13,  -224,  -224,
    -224,  -223,   -64,    -3,  -224,   -84,   145,   125,   -35,   -15,
    -224,    22,  -224,   128,   133,   127,   134,   135,     6,    -4,
       2,     7,    -8,   -58,   -10,  -224,  -224,   115
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -23
static const short yytable[] =
{
      72,    93,    77,    68,   162,   103,    95,   138,   166,    81,
      82,    85,   153,    38,    79,    80,     2,     3,     4,    71,
     170,   118,   -22,     6,     1,     2,     3,     4,    41,    71,
     154,     5,     6,   119,   265,    70,     7,   264,    71,     8,
       9,    10,     7,   223,   270,    74,    11,   257,    38,   269,
      86,    84,   155,    12,     7,    75,    13,    14,    15,   124,
     125,    16,    17,    41,    18,   134,   135,   136,    19,    20,
     104,   151,   152,   158,   100,   101,   159,    76,    21,   196,
     157,   -22,   167,   168,   160,    22,    23,    24,    25,   207,
     239,    72,   204,   106,   171,   242,   130,   131,   244,   132,
     133,    26,    96,   175,   126,   127,   128,   129,    27,    28,
      33,    83,    29,    30,    31,   139,   178,   140,    32,    33,
      87,   141,   185,   186,   187,   188,   193,   194,   195,   214,
     183,   184,   189,   190,    88,    91,   198,   199,   215,   191,
     192,   221,    94,    97,   224,   247,    99,   105,   120,   121,
     137,   142,   143,   107,   108,   109,   110,   122,   199,   123,
     111,   112,   113,   146,   147,   148,   114,   150,   156,   115,
     161,   116,   164,   165,   173,   200,   202,   211,   208,   203,
     205,   206,   209,   212,   220,   210,   144,   213,   226,   222,
     225,   227,   231,   232,   229,   228,   240,   237,   230,   241,
     245,   140,   238,   243,   234,   235,   236,     1,     2,     3,
       4,   248,   250,   253,     5,     6,   254,   251,   252,     7,
     249,   262,     8,     9,    10,   255,   256,   263,   267,    11,
     268,    98,   174,   163,   258,   260,    12,   261,   271,    13,
      14,    15,   176,   266,    16,    17,   177,    18,   180,   246,
       0,    19,    20,   179,     0,     0,   181,   258,   182,   201,
       0,    21,     0,     0,     0,     0,     0,     0,    22,    23,
      24,    25,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
       0,     1,     2,     3,     4,    29,    30,    31,     5,     6,
       0,    32,    33,     7,    69,     0,     8,     9,    10,     0,
       0,     0,     0,    11,     0,     0,     0,     0,     0,     0,
      12,     0,     0,    13,    14,    15,     0,     0,     0,    17,
       0,    18,     0,     0,     0,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,     0,    22,    23,    24,    25,     0,     0,     1,     2,
       3,     4,     0,     0,     0,     5,     6,     0,    26,     0,
       7,     0,     0,     8,     9,    10,     0,     0,     0,     0,
      11,     0,     0,     0,     0,    32,    33,    12,     0,     0,
      13,    14,    15,     0,     0,     0,    17,     0,    18,     0,
       0,     0,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,     0,    22,
      23,    24,    25,     1,     2,     3,     4,     0,     0,     0,
      78,     6,     0,     0,     0,    26,     0,     0,     8,     9,
      10,     0,     0,     0,     0,    11,     0,     0,     0,     0,
       0,     0,    32,    33,     0,     0,     0,     0,     1,     2,
       3,     4,     0,    18,     0,     5,     6,    19,    20,     0,
       0,     0,     0,     8,     9,    10,     0,     0,     0,     0,
      11,     0,     0,     0,    22,    23,    24,    25,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
      26,     0,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    22,
      23,    24,    25,     1,     2,     3,     4,     0,     0,     0,
       0,     6,   197,     0,     0,    26,     0,     0,     8,     9,
      10,     0,     0,     0,     0,    11,     0,     0,     0,     0,
       0,     0,    32,    33,     0,     0,     0,     0,     1,     2,
       3,     4,     0,    18,     0,     0,     6,    19,    20,   202,
       0,     0,     0,     8,     9,    10,     0,     0,     0,     0,
      11,     0,     0,     0,    22,    23,    24,    25,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
      26,     0,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    22,
      23,    24,    25,     1,     2,     3,     4,     0,     0,     0,
       0,     6,   233,     0,     0,    26,     0,     0,     8,     9,
      10,     0,     0,     0,     0,    11,     0,     0,     0,     0,
       0,     0,    32,    33,     0,     0,     0,     0,     1,     2,
       3,     4,     0,    18,     0,     0,     6,    19,    20,     0,
       0,     0,     0,     8,     9,    10,     0,     0,     0,     0,
      11,     0,     0,     0,    22,    23,    24,    25,     0,     0,
       0,     0,     0,    80,     2,     3,     4,     0,    18,     0,
      26,     6,    19,    20,     0,     0,     0,     0,     8,     9,
      10,     0,     0,     0,     0,    11,     0,    32,    33,    22,
      23,    24,    25,     0,     0,     0,     0,     0,    80,     2,
       3,     4,     0,    18,     0,    26,     6,    19,    20,     0,
       0,     0,     0,     8,     9,    10,     0,     0,     0,     0,
       0,     0,    32,    33,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
      26,     0,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    22,
      23,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    33
};

static const short yycheck[] =
{
      11,    28,    15,     6,    88,    40,     3,    65,    15,    19,
      20,    22,    76,     0,    17,     3,     4,     5,     6,     3,
      15,    30,     3,    11,     3,     4,     5,     6,     0,     3,
      35,    10,    11,    42,   257,     7,    15,    10,     3,    18,
      19,    20,    15,   170,   267,    11,    25,    12,    35,    10,
      25,    25,    57,    32,    15,    11,    35,    36,    37,    23,
      24,    40,    41,    35,    43,    20,    21,    22,    47,    48,
      42,    74,    75,    84,    94,    95,    86,    11,    57,   137,
      83,    62,    89,    90,    87,    64,    65,    66,    67,   153,
     217,   102,   150,     8,    89,   222,    78,    79,   225,    18,
      19,    80,    99,   106,    25,    26,    27,    28,    87,    88,
      98,    11,    91,    92,    93,    11,   119,    13,    97,    98,
      25,    17,   126,   127,   128,   129,   134,   135,   136,   164,
     124,   125,   130,   131,     3,     3,   139,   140,   165,   132,
     133,   168,    80,     0,   171,   229,    62,    10,    29,    44,
      80,    47,    48,    68,    69,    70,    71,    46,   161,    45,
      75,    76,    77,    12,    16,    13,    81,    27,    10,    84,
      13,    86,     8,     8,    99,     3,    14,    27,    11,    13,
      12,    12,    11,    27,     3,    12,    82,    27,     3,    15,
      15,     9,   205,   206,    14,    12,    90,   210,    14,    89,
      11,    13,    16,    16,   207,   208,   209,     3,     4,     5,
       6,    34,    12,   240,    10,    11,     3,    12,    12,    15,
     233,    10,    18,    19,    20,    16,    16,    10,    12,    25,
       8,    35,   102,    88,   245,   248,    32,   250,   268,    35,
      36,    37,   117,   258,    40,    41,   118,    43,   121,   227,
      -1,    47,    48,   120,    -1,    -1,   122,   268,   123,   144,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    91,    92,    93,    10,    11,
      -1,    97,    98,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    35,    36,    37,    -1,    -1,    -1,    41,
      -1,    43,    -1,    -1,    -1,    47,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    65,    66,    67,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    -1,    80,    -1,
      15,    -1,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    97,    98,    32,    -1,    -1,
      35,    36,    37,    -1,    -1,    -1,    41,    -1,    43,    -1,
      -1,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      65,    66,    67,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    -1,    -1,    -1,    80,    -1,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    43,    -1,    10,    11,    47,    48,    -1,
      -1,    -1,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      80,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    64,
      65,    66,    67,     3,     4,     5,     6,    -1,    -1,    -1,
      -1,    11,    12,    -1,    -1,    80,    -1,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    43,    -1,    -1,    11,    47,    48,    14,
      -1,    -1,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      80,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    64,
      65,    66,    67,     3,     4,     5,     6,    -1,    -1,    -1,
      -1,    11,    12,    -1,    -1,    80,    -1,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    43,    -1,    -1,    11,    47,    48,    -1,
      -1,    -1,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    43,    -1,
      80,    11,    47,    48,    -1,    -1,    -1,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    97,    98,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    43,    -1,    80,    11,    47,    48,    -1,
      -1,    -1,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    64,    65,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      80,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     5,     6,    10,    11,    15,    18,    19,
      20,    25,    32,    35,    36,    37,    40,    41,    43,    47,
      48,    57,    64,    65,    66,    67,    80,    87,    88,    91,
      92,    93,    97,    98,   101,   102,   103,   109,   110,   111,
     113,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     127,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   123,    16,
     116,     3,   113,   114,    11,    11,    11,   117,    10,   123,
       3,   144,   144,    11,    25,   113,    25,    25,     3,   128,
     129,     3,   108,   108,    80,     3,    99,     0,   101,    62,
      94,    95,   112,   128,   116,    10,     8,    68,    69,    70,
      71,    75,    76,    77,    81,    84,    86,   130,    30,    42,
      29,    44,    46,    45,    23,    24,    25,    26,    27,    28,
      78,    79,    18,    19,    20,    21,    22,    80,   143,    11,
      13,    17,    47,    48,    82,   125,    12,    16,    13,   126,
      27,   123,   123,   122,    35,    57,    10,   123,   113,   144,
     123,    13,   125,   126,     8,     8,    15,    89,    90,   104,
      15,    89,   107,    99,   114,   123,   127,   133,   123,   134,
     135,   136,   137,   138,   138,   139,   139,   139,   139,   140,
     140,   141,   141,   142,   142,   142,   143,    12,   123,   123,
       3,   147,    14,    13,   143,    12,    12,   122,    11,    11,
      12,    27,    27,    27,   128,   108,   105,   106,   110,   116,
       3,   108,    15,   105,   108,    15,     3,     9,    12,    14,
      14,   117,   117,    12,   123,   123,   123,   117,    16,   105,
      90,    89,   105,    16,   105,    11,   131,   125,    34,   117,
      12,    12,    12,   108,     3,    16,    16,    12,   113,   115,
     117,   117,    10,    10,    10,   121,   129,    12,     8,    10,
     121,   115
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
#line 166 "chuck.y"
    { yyval.program = g_program = new_program( yyvsp[0].program_section, EM_lineNum ); ;}
    break;

  case 3:
#line 167 "chuck.y"
    { yyval.program = g_program = prepend_program( yyvsp[-1].program_section, yyvsp[0].program, EM_lineNum ); ;}
    break;

  case 4:
#line 171 "chuck.y"
    { yyval.program_section = new_section_stmt( yyvsp[0].stmt_list, EM_lineNum ); ;}
    break;

  case 5:
#line 172 "chuck.y"
    { yyval.program_section = new_section_func_def( yyvsp[0].func_def, EM_lineNum ); ;}
    break;

  case 6:
#line 173 "chuck.y"
    { yyval.program_section = new_section_class_def( yyvsp[0].class_def, EM_lineNum ); ;}
    break;

  case 7:
#line 178 "chuck.y"
    { yyval.class_def = new_class_def( yyvsp[-3].id_list, NULL, yyvsp[-1].class_body, EM_lineNum ); ;}
    break;

  case 8:
#line 180 "chuck.y"
    { yyval.class_def = new_class_def( yyvsp[-4].id_list, yyvsp[-3].class_ext, yyvsp[-1].class_body, EM_lineNum ); ;}
    break;

  case 9:
#line 182 "chuck.y"
    { yyval.class_def = new_iface_def( yyvsp[-3].id_list, NULL, yyvsp[-1].class_body, EM_lineNum ); ;}
    break;

  case 10:
#line 184 "chuck.y"
    { yyval.class_def = new_iface_def( yyvsp[-4].id_list, yyvsp[-3].class_ext, yyvsp[-1].class_body, EM_lineNum ); ;}
    break;

  case 11:
#line 188 "chuck.y"
    { yyval.class_ext = new_class_ext( NULL, yyvsp[0].id_list, EM_lineNum ); ;}
    break;

  case 12:
#line 189 "chuck.y"
    { yyval.class_ext = new_class_ext( yyvsp[0].sval, yyvsp[-2].id_list, EM_lineNum ); ;}
    break;

  case 13:
#line 190 "chuck.y"
    { yyval.class_ext = new_class_ext( yyvsp[0].sval, NULL, EM_lineNum ); ;}
    break;

  case 14:
#line 191 "chuck.y"
    { yyval.class_ext = new_class_ext( yyvsp[-2].sval, yyvsp[0].id_list, EM_lineNum ); ;}
    break;

  case 15:
#line 195 "chuck.y"
    { yyval.class_body = new_class_body( yyvsp[0].program_section, EM_lineNum ); ;}
    break;

  case 16:
#line 196 "chuck.y"
    { yyval.class_body = prepend_class_body( yyvsp[-1].program_section, yyvsp[0].class_body, EM_lineNum ); ;}
    break;

  case 17:
#line 200 "chuck.y"
    { yyval.program_section = new_section_stmt( yyvsp[0].stmt_list, EM_lineNum ); ;}
    break;

  case 18:
#line 201 "chuck.y"
    { yyval.program_section = new_section_func_def( yyvsp[0].func_def, EM_lineNum ); ;}
    break;

  case 19:
#line 205 "chuck.y"
    { yyval.class_ext = new_class_ext( NULL, yyvsp[0].id_list, EM_lineNum ); ;}
    break;

  case 20:
#line 209 "chuck.y"
    { yyval.id_list = new_id_list( yyvsp[0].sval, EM_lineNum ); ;}
    break;

  case 21:
#line 210 "chuck.y"
    { yyval.id_list = prepend_id_list( yyvsp[-2].sval, yyvsp[0].id_list, EM_lineNum ); ;}
    break;

  case 22:
#line 214 "chuck.y"
    { yyval.id_list = new_id_list( yyvsp[0].sval, EM_lineNum ); ;}
    break;

  case 23:
#line 220 "chuck.y"
    { yyval.func_def = new_func_def( yyvsp[-7].ival, yyvsp[-6].ival, yyvsp[-5].type_decl, yyvsp[-4].sval, yyvsp[-2].arg_list, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 24:
#line 222 "chuck.y"
    { yyval.func_def = new_func_def( yyvsp[-6].ival, yyvsp[-5].ival, yyvsp[-4].type_decl, yyvsp[-3].sval, NULL, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 25:
#line 224 "chuck.y"
    { yyval.func_def = new_func_def( yyvsp[-7].ival, yyvsp[-6].ival, yyvsp[-5].type_decl, yyvsp[-4].sval, yyvsp[-2].arg_list, NULL, EM_lineNum ); ;}
    break;

  case 26:
#line 226 "chuck.y"
    { yyval.func_def = new_func_def( yyvsp[-6].ival, yyvsp[-5].ival, yyvsp[-4].type_decl, yyvsp[-3].sval, NULL, NULL, EM_lineNum ); ;}
    break;

  case 27:
#line 230 "chuck.y"
    { yyval.ival = ae_key_func; ;}
    break;

  case 28:
#line 231 "chuck.y"
    { yyval.ival = ae_key_public; ;}
    break;

  case 29:
#line 232 "chuck.y"
    { yyval.ival = ae_key_protected; ;}
    break;

  case 30:
#line 233 "chuck.y"
    { yyval.ival = ae_key_private; ;}
    break;

  case 31:
#line 237 "chuck.y"
    { yyval.ival = ae_key_static; ;}
    break;

  case 32:
#line 238 "chuck.y"
    { yyval.ival = ae_key_abstract; ;}
    break;

  case 33:
#line 239 "chuck.y"
    { yyval.ival = 0; ;}
    break;

  case 34:
#line 243 "chuck.y"
    { yyval.type_decl = new_type_decl( yyvsp[0].id_list, 0, EM_lineNum ); ;}
    break;

  case 35:
#line 244 "chuck.y"
    { yyval.type_decl = new_type_decl( yyvsp[-1].id_list, 1, EM_lineNum ); ;}
    break;

  case 36:
#line 248 "chuck.y"
    { yyval.type_decl = yyvsp[0].type_decl; ;}
    break;

  case 37:
#line 249 "chuck.y"
    { yyval.type_decl = add_type_decl_array( yyvsp[-1].type_decl, yyvsp[0].array_sub, EM_lineNum ); ;}
    break;

  case 38:
#line 253 "chuck.y"
    { yyval.arg_list = new_arg_list( yyvsp[-1].type_decl, yyvsp[0].var_decl, EM_lineNum ); ;}
    break;

  case 39:
#line 254 "chuck.y"
    { yyval.arg_list = prepend_arg_list( yyvsp[-3].type_decl, yyvsp[-2].var_decl, yyvsp[0].arg_list, EM_lineNum ); ;}
    break;

  case 40:
#line 258 "chuck.y"
    { yyval.stmt_list = new_stmt_list( yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 41:
#line 259 "chuck.y"
    { yyval.stmt_list = prepend_stmt_list( yyvsp[-1].stmt, yyvsp[0].stmt_list, EM_lineNum ); ;}
    break;

  case 42:
#line 263 "chuck.y"
    { yyval.stmt = yyvsp[0].stmt; ;}
    break;

  case 43:
#line 264 "chuck.y"
    { yyval.stmt = yyvsp[0].stmt; ;}
    break;

  case 44:
#line 265 "chuck.y"
    { yyval.stmt = yyvsp[0].stmt; ;}
    break;

  case 45:
#line 266 "chuck.y"
    { yyval.stmt = yyvsp[0].stmt; ;}
    break;

  case 46:
#line 268 "chuck.y"
    { yyval.stmt = yyvsp[0].stmt; ;}
    break;

  case 47:
#line 272 "chuck.y"
    { yyval.stmt = new_stmt_from_return( NULL, EM_lineNum ); ;}
    break;

  case 48:
#line 273 "chuck.y"
    { yyval.stmt = new_stmt_from_return( yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 49:
#line 278 "chuck.y"
    { yyval.stmt = new_stmt_from_if( yyvsp[-2].exp, yyvsp[0].stmt, NULL, EM_lineNum ); ;}
    break;

  case 50:
#line 280 "chuck.y"
    { yyval.stmt = new_stmt_from_if( yyvsp[-4].exp, yyvsp[-2].stmt, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 51:
#line 285 "chuck.y"
    { yyval.stmt = new_stmt_from_while( yyvsp[-2].exp, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 52:
#line 287 "chuck.y"
    { yyval.stmt = new_stmt_from_do_while( yyvsp[-2].exp, yyvsp[-5].stmt, EM_lineNum ); ;}
    break;

  case 53:
#line 289 "chuck.y"
    { yyval.stmt = new_stmt_from_for( yyvsp[-3].stmt, yyvsp[-2].stmt, NULL, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 54:
#line 291 "chuck.y"
    { yyval.stmt = new_stmt_from_for( yyvsp[-4].stmt, yyvsp[-3].stmt, yyvsp[-2].exp, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 55:
#line 293 "chuck.y"
    { yyval.stmt = new_stmt_from_until( yyvsp[-2].exp, yyvsp[0].stmt, EM_lineNum ); ;}
    break;

  case 56:
#line 295 "chuck.y"
    { yyval.stmt = new_stmt_from_do_until( yyvsp[-2].exp, yyvsp[-5].stmt, EM_lineNum ); ;}
    break;

  case 57:
#line 299 "chuck.y"
    { yyval.stmt = new_stmt_from_code( NULL, EM_lineNum ); ;}
    break;

  case 58:
#line 300 "chuck.y"
    { yyval.stmt = new_stmt_from_code( yyvsp[-1].stmt_list, EM_lineNum ); ;}
    break;

  case 59:
#line 304 "chuck.y"
    { yyval.stmt = NULL; ;}
    break;

  case 60:
#line 305 "chuck.y"
    { yyval.stmt = new_stmt_from_expression( yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 61:
#line 309 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 62:
#line 310 "chuck.y"
    { yyval.exp = prepend_expression( yyvsp[-2].exp, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 63:
#line 314 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 64:
#line 316 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, yyvsp[-1].ival, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 65:
#line 320 "chuck.y"
    { yyval.array_sub = new_array_sub( yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 66:
#line 322 "chuck.y"
    { yyval.array_sub = prepend_array_sub( yyvsp[0].array_sub, yyvsp[-2].exp, EM_lineNum ); ;}
    break;

  case 67:
#line 326 "chuck.y"
    { yyval.array_sub = new_array_sub( NULL, EM_lineNum ); ;}
    break;

  case 68:
#line 327 "chuck.y"
    { yyval.array_sub = prepend_array_sub( yyvsp[-2].array_sub, NULL, EM_lineNum ); ;}
    break;

  case 69:
#line 331 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 70:
#line 332 "chuck.y"
    { yyval.exp = new_exp_decl( yyvsp[-1].type_decl, yyvsp[0].var_decl_list, EM_lineNum ); ;}
    break;

  case 71:
#line 333 "chuck.y"
    { yyval.exp = new_exp_decl( NULL, yyvsp[0].var_decl_list, EM_lineNum ); ;}
    break;

  case 72:
#line 337 "chuck.y"
    { yyval.var_decl_list = new_var_decl_list( yyvsp[0].var_decl, EM_lineNum ); ;}
    break;

  case 73:
#line 338 "chuck.y"
    { yyval.var_decl_list = prepend_var_decl_list( yyvsp[-2].var_decl, yyvsp[0].var_decl_list, EM_lineNum ); ;}
    break;

  case 74:
#line 342 "chuck.y"
    { yyval.var_decl = new_var_decl( yyvsp[0].sval, NULL, EM_lineNum ); ;}
    break;

  case 75:
#line 343 "chuck.y"
    { yyval.var_decl = new_var_decl( yyvsp[-1].sval, yyvsp[0].array_sub, EM_lineNum ); ;}
    break;

  case 76:
#line 344 "chuck.y"
    { yyval.var_decl = new_var_decl( yyvsp[-1].sval, yyvsp[0].array_sub, EM_lineNum ); ;}
    break;

  case 77:
#line 348 "chuck.y"
    { yyval.ival = ae_op_chuck; ;}
    break;

  case 78:
#line 349 "chuck.y"
    { yyval.ival = ae_op_at_chuck; ;}
    break;

  case 79:
#line 350 "chuck.y"
    { yyval.ival = ae_op_plus_chuck; ;}
    break;

  case 80:
#line 351 "chuck.y"
    { yyval.ival = ae_op_minus_chuck; ;}
    break;

  case 81:
#line 352 "chuck.y"
    { yyval.ival = ae_op_times_chuck; ;}
    break;

  case 82:
#line 353 "chuck.y"
    { yyval.ival = ae_op_divide_chuck; ;}
    break;

  case 83:
#line 354 "chuck.y"
    { yyval.ival = ae_op_shift_right_chuck; ;}
    break;

  case 84:
#line 355 "chuck.y"
    { yyval.ival = ae_op_shift_left_chuck; ;}
    break;

  case 85:
#line 356 "chuck.y"
    { yyval.ival = ae_op_percent_chuck; ;}
    break;

  case 86:
#line 357 "chuck.y"
    { yyval.ival = ae_op_unchuck; ;}
    break;

  case 87:
#line 365 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 88:
#line 367 "chuck.y"
    { yyval.exp = new_exp_from_if( yyvsp[-4].exp, yyvsp[-2].exp, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 89:
#line 371 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 90:
#line 373 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_or, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 91:
#line 377 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 92:
#line 379 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_and, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 93:
#line 383 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 94:
#line 385 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_s_or, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 95:
#line 389 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 96:
#line 391 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_s_xor, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 97:
#line 395 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 98:
#line 397 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_s_and, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 99:
#line 401 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 100:
#line 403 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_eq, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 101:
#line 405 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_neq, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 102:
#line 409 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 103:
#line 411 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_lt, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 104:
#line 413 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_gt, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 105:
#line 415 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_le, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 106:
#line 417 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_ge, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 107:
#line 421 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 108:
#line 423 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_shift_left, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 109:
#line 425 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_shift_right, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 110:
#line 429 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 111:
#line 431 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_plus, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 112:
#line 433 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_minus, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 113:
#line 437 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 114:
#line 439 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_times, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 115:
#line 441 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_divide, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 116:
#line 443 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_percent, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 117:
#line 447 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 118:
#line 449 "chuck.y"
    { yyval.exp = new_exp_from_binary( yyvsp[-2].exp, ae_op_tilda, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 119:
#line 453 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 120:
#line 455 "chuck.y"
    { yyval.exp = new_exp_from_cast( yyvsp[-2].type_decl, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 121:
#line 459 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 122:
#line 461 "chuck.y"
    { yyval.exp = new_exp_from_unary( ae_op_plusplus, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 123:
#line 463 "chuck.y"
    { yyval.exp = new_exp_from_unary( ae_op_minusminus, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 124:
#line 465 "chuck.y"
    { yyval.exp = new_exp_from_unary( yyvsp[-1].ival, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 125:
#line 467 "chuck.y"
    { yyval.exp = new_exp_from_unary( ae_op_typeof, yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 126:
#line 469 "chuck.y"
    { yyval.exp = new_exp_from_unary( ae_op_sizeof, yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 127:
#line 471 "chuck.y"
    { yyval.exp = new_exp_from_unary2( ae_op_new, yyvsp[0].type_decl, EM_lineNum ); ;}
    break;

  case 128:
#line 473 "chuck.y"
    { yyval.exp = new_exp_from_unary2( ae_op_new, yyvsp[-1].type_decl, EM_lineNum ); ;}
    break;

  case 129:
#line 477 "chuck.y"
    { yyval.ival = ae_op_plus; ;}
    break;

  case 130:
#line 478 "chuck.y"
    { yyval.ival = ae_op_minus; ;}
    break;

  case 131:
#line 479 "chuck.y"
    { yyval.ival = ae_op_tilda; ;}
    break;

  case 132:
#line 480 "chuck.y"
    { yyval.ival = ae_op_exclamation; ;}
    break;

  case 133:
#line 481 "chuck.y"
    { yyval.ival = ae_op_times; ;}
    break;

  case 134:
#line 482 "chuck.y"
    { yyval.ival = ae_op_spork; ;}
    break;

  case 135:
#line 487 "chuck.y"
    { yyval.exp = yyvsp[0].exp; ;}
    break;

  case 136:
#line 489 "chuck.y"
    { yyval.exp = new_exp_from_array( yyvsp[-1].exp, yyvsp[0].array_sub, EM_lineNum ); ;}
    break;

  case 137:
#line 491 "chuck.y"
    { yyval.exp = new_exp_from_func_call( yyvsp[-2].exp, NULL, EM_lineNum ); ;}
    break;

  case 138:
#line 493 "chuck.y"
    { yyval.exp = new_exp_from_func_call( yyvsp[-3].exp, yyvsp[-1].exp, EM_lineNum ); ;}
    break;

  case 139:
#line 495 "chuck.y"
    { yyval.exp = new_exp_from_member_dot( yyvsp[-2].exp, yyvsp[0].sval, EM_lineNum ); ;}
    break;

  case 140:
#line 497 "chuck.y"
    { yyval.exp = new_exp_from_postfix( yyvsp[-1].exp, ae_op_plusplus, EM_lineNum ); ;}
    break;

  case 141:
#line 499 "chuck.y"
    { yyval.exp = new_exp_from_postfix( yyvsp[-1].exp, ae_op_minusminus, EM_lineNum ); ;}
    break;

  case 142:
#line 501 "chuck.y"
    { yyval.exp = new_exp_from_dur( yyvsp[-2].exp, yyvsp[0].exp, EM_lineNum ); ;}
    break;

  case 143:
#line 505 "chuck.y"
    { yyval.exp = new_exp_from_id( yyvsp[0].sval, EM_lineNum ); ;}
    break;

  case 144:
#line 506 "chuck.y"
    { yyval.exp = new_exp_from_int( yyvsp[0].ival, EM_lineNum ); ;}
    break;

  case 145:
#line 507 "chuck.y"
    { yyval.exp = new_exp_from_float( yyvsp[0].fval, EM_lineNum ); ;}
    break;

  case 146:
#line 508 "chuck.y"
    { yyval.exp = new_exp_from_str( yyvsp[0].sval, EM_lineNum ); ;}
    break;

  case 147:
#line 509 "chuck.y"
    { yyval.exp = new_exp_from_namespace( "", EM_lineNum ); ;}
    break;

  case 148:
#line 510 "chuck.y"
    { yyval.exp = new_exp_from_namespace( yyvsp[-1].sval, EM_lineNum ); ;}
    break;

  case 149:
#line 511 "chuck.y"
    { yyval.exp = yyvsp[-1].exp; ;}
    break;


    }

/* Line 999 of yacc.c.  */
#line 2372 "chuck.tab.c"

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
 * $Header: /cvs/chuck_dev/v2/chuck_win32.c,v 1.14 2005/03/29 05:11:21 gw-chuck Exp $
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

#define YY_NUM_RULES 104
#define YY_END_OF_BUFFER 105
static yyconst short int yy_accept[236] =
    {   0,
        0,    0,  105,  103,    3,    5,  103,    2,   42,  103,
       16,   15,   28,   34,   35,   13,   11,    8,   12,   10,
       14,   99,   99,    9,   40,   22,   33,   23,   41,   95,
      101,  101,   36,   37,   30,  101,  101,  101,  101,  101,
      101,  101,  101,  101,  101,  101,  101,   38,   29,   39,
       43,    4,   21,    0,  102,    0,    0,    0,   26,    0,
        0,   17,    0,    0,    6,    0,    0,    7,    0,   72,
      100,    0,    1,    0,  100,   99,    0,   99,    0,   18,
       19,   73,   32,   24,   70,   20,   69,   25,   31,    0,
       96,  101,  101,    0,    0,  101,  101,  101,  101,  101,

      101,  101,   47,  101,  101,  101,  101,  101,  101,  101,
      101,  101,  101,  101,  101,  101,    0,    0,   27,   71,
       83,   94,   78,   89,   76,   87,   74,   85,   75,   86,
       77,   88,  100,   98,   97,    0,    0,    0,    0,   84,
      101,   80,   91,  101,  101,  101,  101,  101,   44,   55,
      101,  101,   56,  101,  101,  101,  101,  101,  101,  101,
      101,  101,  101,  101,  101,   79,   90,   98,   97,   82,
       93,   81,   92,   50,  101,  101,  101,   48,  101,  101,
      101,  101,   51,  101,  101,  101,   65,  101,  101,  101,
       49,  101,  101,  101,   46,   57,   66,  101,  101,  101,

      101,  101,  101,  101,  101,   67,  101,  101,   53,   45,
      101,  101,  101,  101,  101,  101,   61,   52,   64,   68,
       59,  101,  101,  101,   63,  101,   54,  101,  101,  101,
      101,   58,   62,   60,    0
    } ;

static yyconst int yy_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
        1,    1,    4,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    5,    6,    7,    8,    1,    9,   10,    1,   11,
       12,   13,   14,   15,   16,   17,   18,   19,   20,   20,
       20,   20,   20,   20,   20,   21,   21,   22,   23,   24,
       25,   26,   27,   28,   29,   29,   30,   29,   29,   29,
       31,   31,   31,   31,   31,   32,   31,   33,   31,   31,
       31,   31,   31,   31,   34,   31,   31,   35,   31,   31,
       36,   37,   38,   39,   31,    1,   40,   41,   42,   43,

       44,   45,   31,   46,   47,   31,   48,   49,   50,   51,
       52,   53,   31,   54,   55,   56,   57,   58,   59,   60,
       61,   31,   62,   63,   64,   65,    1,    1,    1,    1,
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
        1,    1,    1,    1,    1,    1,    1,    1,    2,    2,
        2,    1,    1,    1,    1,    1,    1,    1,    2,    2,
        3,    3,    3,    3,    3,    1,    1,    1,    1,    2,
        2,    2,    2,    2,    2,    3,    3,    3,    3,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
        3,    1,    1,    1,    1
    } ;

static yyconst short int yy_base[239] =
    {   0,
        0,    0,  309,  310,  310,  310,  305,  310,  282,   59,
      310,   51,   58,  284,  310,   53,   56,  310,   59,   67,
       55,   80,   74,   67,  310,   93,   78,   94,  310,   88,
        0,  271,  310,  310,   82,  250,   72,   76,   25,   82,
       48,   81,  259,   86,   65,  251,  255,  310,  118,  310,
      310,  310,  274,  121,  310,  296,  272,  271,  310,  270,
      269,  310,  268,  267,  310,  266,  265,  264,  263,  310,
      125,  262,  310,  261,  128,  133,  136,  127,    0,  310,
      310,  310,  135,  310,  310,  310,  310,  310,  141,  260,
      310,    0,  253,  258,  257,  238,  241,  229,  224,  222,

      223,  225,    0,  222,  218,  214,  223,  116,  121,  215,
      218,  229,  224,  214,  210,  218,  238,  237,  310,  310,
      310,  310,  310,  310,  310,  310,  310,  310,  310,  310,
      310,  310,  150,  153,  146,  236,  235,  234,  233,  310,
      226,  310,  310,  217,  201,  200,  210,  209,    0,  210,
      202,  206,    0,  200,  190,  191,  197,  201,  187,  189,
      186,  190,  196,  192,  189,  310,  310,  157,  160,  310,
      310,  310,  310,    0,  189,  181,  179,    0,  183,  177,
      188,  177,    0,  190,  185,  181,    0,  173,  178,  178,
        0,  172,  174,  172,    0,    0,    0,  172,  166,  162,

      166,  152,  165,  163,  153,    0,  159,  155,    0,    0,
      144,  146,  153,  156,  149,  132,    0,    0,    0,    0,
        0,  135,  132,  137,    0,  133,    0,  118,  120,   98,
       75,    0,    0,    0,  310,  217,  219,   88
    } ;

static yyconst short int yy_def[239] =
    {   0,
      235,    1,  235,  235,  235,  235,  235,  235,  235,  236,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      237,  237,  235,  235,  235,  237,  237,  237,  237,  237,
      237,  237,  237,  237,  237,  237,  237,  235,  235,  235,
      235,  235,  235,  236,  235,  236,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  238,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  237,  237,  235,  235,  237,  237,  237,  237,  237,

      237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
      237,  237,  237,  237,  237,  237,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  238,  235,  235,  235,  235,  235,
      237,  235,  235,  237,  237,  237,  237,  237,  237,  237,
      237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
      237,  237,  237,  237,  237,  235,  235,  235,  235,  235,
      235,  235,  235,  237,  237,  237,  237,  237,  237,  237,
      237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
      237,  237,  237,  237,  237,  237,  237,  237,  237,  237,

      237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
      237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
      237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
      237,  237,  237,  237,    0,  235,  235,  235
    } ;

static yyconst short int yy_nxt[376] =
    {   0,
        4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
       14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
       23,   24,   25,   26,   27,   28,   29,   30,   31,   31,
       31,   31,   32,   31,   31,   33,    4,   34,   35,   31,
       36,   37,   31,   38,   39,   31,   40,   31,   31,   31,
       41,   31,   42,   43,   44,   45,   46,   31,   47,   31,
       31,   48,   49,   50,   51,   55,   57,   59,   63,   65,
       72,   66,   73,   60,   68,   58,  101,   64,   80,   74,
       67,  102,   61,   69,   70,   71,   71,   71,   81,  135,
       75,  106,   76,   76,   76,   56,   75,   94,   76,   76,

       76,   85,   86,   87,  107,   78,   95,   78,   82,   77,
      113,   78,   90,   78,   79,   91,   83,   84,   88,   89,
       97,   77,   78,   98,   99,  114,  103,   55,   78,  234,
       78,  104,  105,  117,  108,  100,   78,  109,  111,   79,
      233,  112,  118,   71,   71,   71,  133,  133,  133,   75,
      136,   76,   76,   76,  134,  134,  138,   56,   78,  137,
       78,  157,  155,  232,   78,  139,   78,  156,  133,  133,
      133,  134,  134,  231,  158,   78,  230,  169,  229,  169,
      119,   78,  228,   78,  168,  227,  168,  226,  168,   78,
      168,  169,  225,  169,  169,  224,  223,  222,  221,  220,

      219,  168,  169,  218,  217,  168,  216,  215,  169,  168,
      214,  213,  212,  168,  211,  210,  169,   54,   54,   54,
       92,   92,  209,  208,  207,  206,  205,  204,  203,  202,
      201,  200,  199,  198,  197,  196,  195,  194,  193,  192,
      191,  190,  189,  188,  187,  186,  185,  184,  183,  182,
      181,  180,  179,  178,  177,  176,  175,  174,  173,  172,
      171,  170,  167,  166,  165,  164,  163,  162,  161,  160,
      159,  154,  153,  152,  151,  150,  149,  148,  147,  146,
      145,  144,  143,  142,  141,  140,  132,  131,  130,  129,
      128,  127,  126,  125,  124,  123,  122,  121,  235,  120,

      116,  115,  110,   96,   93,   62,   53,   52,  235,    3,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235
    } ;

static yyconst short int yy_chk[376] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,   10,   12,   13,   16,   17,
       21,   17,   21,   13,   19,   12,   39,   16,   24,   21,
       17,   39,   13,   19,   19,   20,   20,   20,   24,  238,
       23,   41,   23,   23,   23,   10,   22,   35,   22,   22,

       22,   27,   27,   27,   41,   23,   35,   23,   26,   22,
       45,   22,   30,   22,   22,   30,   26,   26,   28,   28,
       37,   22,   23,   37,   38,   45,   40,   54,   22,  231,
       23,   40,   40,   49,   42,   38,   22,   42,   44,   22,
      230,   44,   49,   71,   71,   71,   75,   75,   75,   76,
       83,   76,   76,   76,   77,   77,   89,   54,   78,   83,
       78,  109,  108,  229,   76,   89,   76,  108,  133,  133,
      133,  134,  134,  228,  109,   78,  226,  135,  224,  135,
       49,   76,  223,   78,  134,  222,  134,  216,  168,   76,
      168,  169,  215,  169,  135,  214,  213,  212,  211,  208,

      207,  134,  135,  205,  204,  168,  203,  202,  169,  134,
      201,  200,  199,  168,  198,  194,  169,  236,  236,  236,
      237,  237,  193,  192,  190,  189,  188,  186,  185,  184,
      182,  181,  180,  179,  177,  176,  175,  165,  164,  163,
      162,  161,  160,  159,  158,  157,  156,  155,  154,  152,
      151,  150,  148,  147,  146,  145,  144,  141,  139,  138,
      137,  136,  118,  117,  116,  115,  114,  113,  112,  111,
      110,  107,  106,  105,  104,  102,  101,  100,   99,   98,
       97,   96,   95,   94,   93,   90,   74,   72,   69,   68,
       67,   66,   64,   63,   61,   60,   58,   57,   56,   53,

       47,   46,   43,   36,   32,   14,    9,    7,    3,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235,  235,  235,  235,  235,  235,
      235,  235,  235,  235,  235
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
#line 10 "chuck.lex"
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

int comment();

#line 647 "chuck.yy.c"

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

#line 130 "chuck.lex"


#line 812 "chuck.yy.c"

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
				if ( yy_current_state >= 236 )
					yy_c = yy_meta[(unsigned int) yy_c];
				}
			yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
			++yy_cp;
			}
		while ( yy_base[yy_current_state] != 310 );

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
#line 132 "chuck.lex"
{ adjust(); comment(); continue; }
	YY_BREAK
case 2:
YY_RULE_SETUP
#line 133 "chuck.lex"
{ adjust(); continue; }
	YY_BREAK
case 3:
YY_RULE_SETUP
#line 134 "chuck.lex"
{ adjust(); continue; }
	YY_BREAK
case 4:
YY_RULE_SETUP
#line 135 "chuck.lex"
{ adjust(); EM_newline(); continue; }
	YY_BREAK
case 5:
YY_RULE_SETUP
#line 136 "chuck.lex"
{ adjust(); EM_newline(); continue; }
	YY_BREAK
case 6:
YY_RULE_SETUP
#line 138 "chuck.lex"
{ adjust(); return PLUSPLUS; }
	YY_BREAK
case 7:
YY_RULE_SETUP
#line 139 "chuck.lex"
{ adjust(); return MINUSMINUS; }
	YY_BREAK
case 8:
YY_RULE_SETUP
#line 141 "chuck.lex"
{ adjust(); return COMMA; }
	YY_BREAK
case 9:
YY_RULE_SETUP
#line 142 "chuck.lex"
{ adjust(); return COLON; }
	YY_BREAK
case 10:
YY_RULE_SETUP
#line 143 "chuck.lex"
{ adjust(); return DOT; }
	YY_BREAK
case 11:
YY_RULE_SETUP
#line 144 "chuck.lex"
{ adjust(); return PLUS; }
	YY_BREAK
case 12:
YY_RULE_SETUP
#line 145 "chuck.lex"
{ adjust(); return MINUS; }
	YY_BREAK
case 13:
YY_RULE_SETUP
#line 146 "chuck.lex"
{ adjust(); return TIMES; }
	YY_BREAK
case 14:
YY_RULE_SETUP
#line 147 "chuck.lex"
{ adjust(); return DIVIDE; }
	YY_BREAK
case 15:
YY_RULE_SETUP
#line 148 "chuck.lex"
{ adjust(); return PERCENT; }
	YY_BREAK
case 16:
YY_RULE_SETUP
#line 149 "chuck.lex"
{ adjust(); return POUND; }
	YY_BREAK
case 17:
YY_RULE_SETUP
#line 151 "chuck.lex"
{ adjust(); return L_NSPC; }
	YY_BREAK
case 18:
YY_RULE_SETUP
#line 152 "chuck.lex"
{ adjust(); return R_NSPC; }
	YY_BREAK
case 19:
YY_RULE_SETUP
#line 153 "chuck.lex"
{ adjust(); return COLONCOLON; }
	YY_BREAK
case 20:
YY_RULE_SETUP
#line 154 "chuck.lex"
{ adjust(); return EQ; }
	YY_BREAK
case 21:
YY_RULE_SETUP
#line 155 "chuck.lex"
{ adjust(); return NEQ; }
	YY_BREAK
case 22:
YY_RULE_SETUP
#line 156 "chuck.lex"
{ adjust(); return LT; }
	YY_BREAK
case 23:
YY_RULE_SETUP
#line 157 "chuck.lex"
{ adjust(); return GT; }
	YY_BREAK
case 24:
YY_RULE_SETUP
#line 158 "chuck.lex"
{ adjust(); return LE; }
	YY_BREAK
case 25:
YY_RULE_SETUP
#line 159 "chuck.lex"
{ adjust(); return GE; }
	YY_BREAK
case 26:
YY_RULE_SETUP
#line 160 "chuck.lex"
{ adjust(); return AND; }
	YY_BREAK
case 27:
YY_RULE_SETUP
#line 161 "chuck.lex"
{ adjust(); return OR; }
	YY_BREAK
case 28:
YY_RULE_SETUP
#line 162 "chuck.lex"
{ adjust(); return S_AND; }
	YY_BREAK
case 29:
YY_RULE_SETUP
#line 163 "chuck.lex"
{ adjust(); return S_OR; }
	YY_BREAK
case 30:
YY_RULE_SETUP
#line 164 "chuck.lex"
{ adjust(); return S_XOR; }
	YY_BREAK
case 31:
YY_RULE_SETUP
#line 165 "chuck.lex"
{ adjust(); return SHIFT_RIGHT; }
	YY_BREAK
case 32:
YY_RULE_SETUP
#line 166 "chuck.lex"
{ adjust(); return SHIFT_LEFT; }
	YY_BREAK
case 33:
YY_RULE_SETUP
#line 167 "chuck.lex"
{ adjust(); return ASSIGN; }
	YY_BREAK
case 34:
YY_RULE_SETUP
#line 168 "chuck.lex"
{ adjust(); return LPAREN; }
	YY_BREAK
case 35:
YY_RULE_SETUP
#line 169 "chuck.lex"
{ adjust(); return RPAREN; }
	YY_BREAK
case 36:
YY_RULE_SETUP
#line 170 "chuck.lex"
{ adjust(); return LBRACK; }
	YY_BREAK
case 37:
YY_RULE_SETUP
#line 171 "chuck.lex"
{ adjust(); return RBRACK; }
	YY_BREAK
case 38:
YY_RULE_SETUP
#line 172 "chuck.lex"
{ adjust(); return LBRACE; }
	YY_BREAK
case 39:
YY_RULE_SETUP
#line 173 "chuck.lex"
{ adjust(); return RBRACE; }
	YY_BREAK
case 40:
YY_RULE_SETUP
#line 174 "chuck.lex"
{ adjust(); return SEMICOLON; }
	YY_BREAK
case 41:
YY_RULE_SETUP
#line 175 "chuck.lex"
{ adjust(); return QUESTION; }
	YY_BREAK
case 42:
YY_RULE_SETUP
#line 176 "chuck.lex"
{ adjust(); return EXCLAMATION; }
	YY_BREAK
case 43:
YY_RULE_SETUP
#line 177 "chuck.lex"
{ adjust(); return TILDA; }
	YY_BREAK
case 44:
YY_RULE_SETUP
#line 178 "chuck.lex"
{ adjust(); return FOR; }
	YY_BREAK
case 45:
YY_RULE_SETUP
#line 179 "chuck.lex"
{ adjust(); return WHILE; }
	YY_BREAK
case 46:
YY_RULE_SETUP
#line 180 "chuck.lex"
{ adjust(); return BREAK; }
	YY_BREAK
case 47:
YY_RULE_SETUP
#line 181 "chuck.lex"
{ adjust(); return IF; }
	YY_BREAK
case 48:
YY_RULE_SETUP
#line 182 "chuck.lex"
{ adjust(); return ELSE; }
	YY_BREAK
case 49:
YY_RULE_SETUP
#line 183 "chuck.lex"
{ adjust(); return THEN; }
	YY_BREAK
case 50:
YY_RULE_SETUP
#line 185 "chuck.lex"
{ adjust(); return NULL_TOK; }
	YY_BREAK
case 51:
YY_RULE_SETUP
#line 186 "chuck.lex"
{ adjust(); return NULL_TOK; }
	YY_BREAK
case 52:
YY_RULE_SETUP
#line 187 "chuck.lex"
{ adjust(); return RETURN; }
	YY_BREAK
case 53:
YY_RULE_SETUP
#line 189 "chuck.lex"
{ adjust(); return UNTIL; }
	YY_BREAK
case 54:
YY_RULE_SETUP
#line 190 "chuck.lex"
{ adjust(); return FUNCTION; }
	YY_BREAK
case 55:
YY_RULE_SETUP
#line 191 "chuck.lex"
{ adjust(); return FUNCTION; }
	YY_BREAK
case 56:
YY_RULE_SETUP
#line 192 "chuck.lex"
{ adjust(); return NEW; }
	YY_BREAK
case 57:
YY_RULE_SETUP
#line 193 "chuck.lex"
{ adjust(); return CLASS; }
	YY_BREAK
case 58:
YY_RULE_SETUP
#line 194 "chuck.lex"
{ adjust(); return INTERFACE; }
	YY_BREAK
case 59:
YY_RULE_SETUP
#line 195 "chuck.lex"
{ adjust(); return EXTENDS; }
	YY_BREAK
case 60:
YY_RULE_SETUP
#line 196 "chuck.lex"
{ adjust(); return IMPLEMENTS; }
	YY_BREAK
case 61:
YY_RULE_SETUP
#line 197 "chuck.lex"
{ adjust(); return PUBLIC; }
	YY_BREAK
case 62:
YY_RULE_SETUP
#line 198 "chuck.lex"
{ adjust(); return PROTECTED; }
	YY_BREAK
case 63:
YY_RULE_SETUP
#line 199 "chuck.lex"
{ adjust(); return PRIVATE; }
	YY_BREAK
case 64:
YY_RULE_SETUP
#line 200 "chuck.lex"
{ adjust(); return STATIC; }
	YY_BREAK
case 65:
YY_RULE_SETUP
#line 201 "chuck.lex"
{ adjust(); return ABSTRACT; }
	YY_BREAK
case 66:
YY_RULE_SETUP
#line 202 "chuck.lex"
{ adjust(); return CONST; }
	YY_BREAK
case 67:
YY_RULE_SETUP
#line 203 "chuck.lex"
{ adjust(); return SPORK; }
	YY_BREAK
case 68:
YY_RULE_SETUP
#line 204 "chuck.lex"
{ adjust(); return TYPEOF; }
	YY_BREAK
case 69:
YY_RULE_SETUP
#line 206 "chuck.lex"
{ adjust(); return CHUCK; }
	YY_BREAK
case 70:
YY_RULE_SETUP
#line 207 "chuck.lex"
{ adjust(); return UNCHUCK; }
	YY_BREAK
case 71:
YY_RULE_SETUP
#line 208 "chuck.lex"
{ adjust(); return UNCHUCK; }
	YY_BREAK
case 72:
YY_RULE_SETUP
#line 209 "chuck.lex"
{ adjust(); return S_CHUCK; }
	YY_BREAK
case 73:
YY_RULE_SETUP
#line 210 "chuck.lex"
{ adjust(); return LEFT_S_CHUCK; }
	YY_BREAK
case 74:
YY_RULE_SETUP
#line 211 "chuck.lex"
{ adjust(); return PLUS_CHUCK; }
	YY_BREAK
case 75:
YY_RULE_SETUP
#line 212 "chuck.lex"
{ adjust(); return MINUS_CHUCK; }
	YY_BREAK
case 76:
YY_RULE_SETUP
#line 213 "chuck.lex"
{ adjust(); return TIMES_CHUCK; }
	YY_BREAK
case 77:
YY_RULE_SETUP
#line 214 "chuck.lex"
{ adjust(); return DIVIDE_CHUCK; }
	YY_BREAK
case 78:
YY_RULE_SETUP
#line 215 "chuck.lex"
{ adjust(); return S_AND_CHUCK; }
	YY_BREAK
case 79:
YY_RULE_SETUP
#line 216 "chuck.lex"
{ adjust(); return S_OR_CHUCK; }
	YY_BREAK
case 80:
YY_RULE_SETUP
#line 217 "chuck.lex"
{ adjust(); return S_XOR_CHUCK; }
	YY_BREAK
case 81:
YY_RULE_SETUP
#line 218 "chuck.lex"
{ adjust(); return SHIFT_RIGHT_CHUCK; }
	YY_BREAK
case 82:
YY_RULE_SETUP
#line 219 "chuck.lex"
{ adjust(); return SHIFT_LEFT_CHUCK; }
	YY_BREAK
case 83:
YY_RULE_SETUP
#line 220 "chuck.lex"
{ adjust(); return PERCENT_CHUCK; }
	YY_BREAK
case 84:
YY_RULE_SETUP
#line 221 "chuck.lex"
{ adjust(); return AT_CHUCK; }
	YY_BREAK
case 85:
YY_RULE_SETUP
#line 222 "chuck.lex"
{ adjust(); return PLUS_CHUCK; }
	YY_BREAK
case 86:
YY_RULE_SETUP
#line 223 "chuck.lex"
{ adjust(); return MINUS_CHUCK; }
	YY_BREAK
case 87:
YY_RULE_SETUP
#line 224 "chuck.lex"
{ adjust(); return TIMES_CHUCK; }
	YY_BREAK
case 88:
YY_RULE_SETUP
#line 225 "chuck.lex"
{ adjust(); return DIVIDE_CHUCK; }
	YY_BREAK
case 89:
YY_RULE_SETUP
#line 226 "chuck.lex"
{ adjust(); return S_AND_CHUCK; }
	YY_BREAK
case 90:
YY_RULE_SETUP
#line 227 "chuck.lex"
{ adjust(); return S_OR_CHUCK; }
	YY_BREAK
case 91:
YY_RULE_SETUP
#line 228 "chuck.lex"
{ adjust(); return S_XOR_CHUCK; }
	YY_BREAK
case 92:
YY_RULE_SETUP
#line 229 "chuck.lex"
{ adjust(); return SHIFT_RIGHT_CHUCK; }
	YY_BREAK
case 93:
YY_RULE_SETUP
#line 230 "chuck.lex"
{ adjust(); return SHIFT_LEFT_CHUCK; }
	YY_BREAK
case 94:
YY_RULE_SETUP
#line 231 "chuck.lex"
{ adjust(); return PERCENT_CHUCK; }
	YY_BREAK
case 95:
YY_RULE_SETUP
#line 232 "chuck.lex"
{ adjust(); return AT_SYM; }
	YY_BREAK
case 96:
YY_RULE_SETUP
#line 233 "chuck.lex"
{ adjust(); return ATAT_SYM; }
	YY_BREAK
case 97:
YY_RULE_SETUP
#line 235 "chuck.lex"
{ adjust(); yylval.ival=htol(yytext); return NUM; }
	YY_BREAK
case 98:
YY_RULE_SETUP
#line 236 "chuck.lex"
{ adjust(); yylval.ival=atoi(yytext); return NUM; }
	YY_BREAK
case 99:
YY_RULE_SETUP
#line 237 "chuck.lex"
{ adjust(); yylval.ival=atoi(yytext); return NUM; }
	YY_BREAK
case 100:
YY_RULE_SETUP
#line 238 "chuck.lex"
{ adjust(); yylval.fval=atof(yytext); return FLOAT; }
	YY_BREAK
case 101:
YY_RULE_SETUP
#line 239 "chuck.lex"
{ adjust(); yylval.sval=alloc_str(yytext); return ID; }
	YY_BREAK
case 102:
YY_RULE_SETUP
#line 240 "chuck.lex"
{ adjust(); yylval.sval=alloc_str(strip_lit(yytext)); return STRING_LIT; }
	YY_BREAK
case 103:
YY_RULE_SETUP
#line 242 "chuck.lex"
{ adjust(); EM_error( EM_tokPos, "illegal token" ); }
	YY_BREAK
case 104:
YY_RULE_SETUP
#line 244 "chuck.lex"
ECHO;
	YY_BREAK
#line 1415 "chuck.yy.c"
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
			if ( yy_current_state >= 236 )
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
		if ( yy_current_state >= 236 )
			yy_c = yy_meta[(unsigned int) yy_c];
		}
	yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
	yy_is_jam = (yy_current_state == 235);

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
#line 244 "chuck.lex"


// comment
int comment()
{
    char c;

    while ((c = input()) != '\n' && c != '\r' && c != 0);

    if (c != 0) { 
       adjust(); 
       if (c == '\n') EM_newline();
    }
    
    return 0;
}
