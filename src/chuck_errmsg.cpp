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

/*
 * errmsg.cpp - functions used in all phases of the compiler to give
 *              error messages about the Tiger program.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "chuck_utils.h"
#include "chuck_errmsg.h"


t_CKBOOL anyErrors= FALSE;
static c_str fileName = "";
static int lineNum = 1;
int EM_tokPos = 0;
int EM_lineNum = 1;
extern FILE *yyin;

typedef struct intList {int i; struct intList *rest;} *IntList;
static IntList linePos=NULL;


static IntList intList( int i, IntList rest )
{
    IntList l = (IntList)checked_malloc(sizeof *l);
    l->i=i; l->rest=rest;
    return l;
}


void EM_newline(void)
{
    lineNum++;
    EM_lineNum++;
    linePos = intList(EM_tokPos, linePos);
}


void EM_error( int pos, char *message, ... )
{
    va_list ap;
    IntList lines = linePos; 
    int num = lineNum;

    anyErrors = TRUE;
    while( lines && lines->i >= pos ) 
    {
        lines = lines->rest;
        num--;
    }

    fprintf( stderr, "('%s'):", *fileName ? fileName : "chuck" );
    if (lines) fprintf(stderr, "line(%d).char(%d):", num, pos-lines->i );
    fprintf(stderr, " " );
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}


void EM_error2( int line, char *message, ... )
{
    va_list ap;

    fprintf( stderr, "[%s]:", *fileName ? fileName : "chuck" );
    if (line) fprintf(stderr, "line(%d):", line );
    fprintf(stderr, " " );
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}


void EM_reset( c_str fname )
{
    anyErrors = FALSE; fileName = fname ? fname : (c_str)""; lineNum = 1;  EM_lineNum = 1;
    linePos=intList(0, NULL);

    yyin = fopen(fname, "r");
    if (!yyin) 
    {
        EM_error( 0, "no such file or directory" );
        exit(1);
    }
}
