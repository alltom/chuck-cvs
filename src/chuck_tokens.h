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
// file: chuck_tokens.h
// desc: chuck lexer tokens
//
// author: Ge Wang (gewang.cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Summer 2002
//-----------------------------------------------------------------------------

#ifndef __CHUCK_TOKENS_H__
#define __CHUCK_TOKENS_H__


// YYSTYPE
typedef union
{
    int pos;
    int ival;
    float fval;
    c_str sval;
}YYSTYPE;


# define    ID          257
# define    STRING_LIT  258
# define    NUM         259
# define    POUND       260
# define    COMMA       261
# define    COLON       262
# define    SEMICOLON   263
# define    LPAREN      264
# define    RPAREN      265
# define    LBRACK      266
# define    RBRACK      267
# define    LBRACE      268
# define    RBRACE      269
# define    DOT         270
# define    PLUS        271
# define    MINUS       272
# define    TIMES       273
# define    DIVIDE      274
# define    EQ          275
# define    NEQ         276
# define    LT          277
# define    LE          278
# define    GT          279
# define    GE          280
# define    AND         281
# define    OR          282
# define    ASSIGN      283
# define    IF          284
# define    THEN        285
# define    ELSE        286
# define    WHILE       287
# define    FOR         288
# define    DO          289
# define    LET         290
# define    IN_TOK      291
# define    END         292
# define    BREAK       293
# define    NULL_TOK    294
# define    FUNCTION    295
# define    RETURN      296
# define    QUESTION    297
# define    EXCLAMATION 298
# define    S_OR        299
# define    S_AND       300
# define    S_XOR       301
# define    PLUSPLUS    302
# define    MINUSMINUS  303
# define    RARROW      304
# define    LARROW      305
# define    FILTER      306
# define    OSC         307
# define    ENV         308
# define    SIMULT      309
# define    INT         310
# define    FLOAT       311
# define    STRING      312
# define    PATTERN     313
# define    CODE        314
# define    TRANSPORT   315
# define    HOST        316
# define    DUR         317
# define    SAMP        318
# define    MS          319
# define    SECOND      320
# define    MINUTE      321
# define    HOUR        322
# define    DAY         323
# define    WEEK        324
# define    TIME        325
# define    NOW         326
# define    WHENEVER    327
# define    NEXT        328
# define    UNTIL       329
# define    EVERY       330
# define    BEFORE      331
# define    AFTER       332
# define    AT          333
# define    AT_SYM      334
# define    ATAT_SYM    335
# define    FORK        336
# define    THREAD      337
# define    SPORK       338
# define    SHRED       339
# define    CHUCK       340


extern YYSTYPE yylval;


#endif
