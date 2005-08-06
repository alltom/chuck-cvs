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
// file: errmsg.cpp
// desc: functions used in all phases of the compiler to give error messages 
//       about the Tiger program.  (now ChucK)
//
// author: Andrew Appel (appel@cs.princeton.edu)
// modified: Ge Wang (gewang@cs.princeton.edu)
//           Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "chuck_utils.h"
#include "chuck_errmsg.h"


// global
int EM_tokPos = 0;
int EM_lineNum = 1;
t_CKBOOL anyErrors= FALSE;

// local global
static const char * fileName = "";
static int lineNum = 1;
static char g_buffer[1024] = "";
static char g_lasterror[1024] = "[chuck]: (no error)";
static int g_loglevel = CK_LOG_SYSTEM_ERROR;

// name
static const char * g_str[] = {
    "ALL",          // 0
    "FINEST",       // 1
    "FINER",        // 2
    "FINE",         // 3
    "CONFIG",       // 4
    "INFO",         // 5
    "WARNING",      // 6
    "SEVERE",       // 7
    "SYSTEM",       // 8
    "SYSTEM_ERROR", // 9
    "NONE"          // 10
};


// external
extern "C" { 
    extern FILE *yyin;
}


// intList
typedef struct intList {int i; struct intList *rest;} *IntList;
static IntList linePos=NULL;


// constructor
static IntList intList( int i, IntList rest )
{
    IntList l = (IntList)checked_malloc(sizeof *l);
    l->i=i; l->rest=rest;
    return l;
}


// new line in lexer
void EM_newline(void)
{
    lineNum++;
    EM_lineNum++;
    linePos = intList(EM_tokPos, linePos);
}


// content after rightmost '/'
const char * mini( const char * str )
{
    int len = strlen( str );
    const char * p = str + len;
    while( p != str && *p != '/' && *p != '\\' ) p--;
    return ( p == str || strlen(p+1) == 0 ? p : p+1 );
}


// [%s]:line(%d).char(%d): 
void EM_error( int pos, const char * message, ... )
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

    fprintf( stderr, "[%s]:", *fileName ? mini(fileName) : "chuck" );
    sprintf( g_lasterror, "[%s]:", *fileName ? mini(fileName) : "chuck" );
    if(lines)
    {
        fprintf(stderr, "line(%d).char(%d):", num, pos-lines->i );
        sprintf( g_buffer, "line(%d).char(%d):", num, pos-lines->i );
        strcat( g_lasterror, g_buffer );
    }
    fprintf(stderr, " " );
    strcat( g_lasterror, " " );
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    vsprintf( g_buffer, message, ap );
    va_end(ap);
    fprintf(stderr, "\n");
    strcat( g_lasterror, g_buffer );
}


// [%s]:line(%d): 
void EM_error2( int line, const char * message, ... )
{
    va_list ap;

    fprintf( stderr, "[%s]:", *fileName ? mini(fileName) : "chuck" );
    sprintf( g_lasterror, "[%s]:", *fileName ? mini(fileName) : "chuck" );
    if(line)
    {
        fprintf( stderr, "line(%d):", line );
        sprintf( g_buffer, "line(%d):", line );
        strcat( g_lasterror, g_buffer );
    }
    fprintf( stderr, " " );
    strcat( g_lasterror, " " );

    va_start( ap, message );
    vfprintf( stderr, message, ap );
    vsprintf( g_buffer, message, ap );
    va_end( ap );

    strcat( g_lasterror, g_buffer );
    fprintf( stderr, "\n" );
}


// [%s]:line(%d):
void EM_error2b( int line, const char * message, ... )
{
    va_list ap;

    fprintf( stderr, "[%s]:", *fileName ? mini(fileName) : "chuck" );
    sprintf( g_lasterror, "[%s]:", *fileName ? mini(fileName) : "chuck" );
    if(line)
    {
        fprintf( stderr, "line(%d):", line );
        sprintf( g_buffer, "line(%d):", line );
        strcat( g_lasterror, g_buffer );
    }
    fprintf( stderr, " " );
    strcat( g_lasterror, " " );

    va_start( ap, message );
    vfprintf( stderr, message, ap );
    vsprintf( g_buffer, message, ap );
    va_end( ap );

    strcat( g_lasterror, g_buffer );
    fprintf( stdout, "\n" );
}


// 
void EM_error3( const char * message, ... )
{
    va_list ap;
    
    g_lasterror[0] = '\0';
    g_buffer[0] = '\0';

    va_start( ap, message );
    vfprintf( stderr, message, ap );
    vsprintf( g_buffer, message, ap );
    va_end( ap );

    strcat( g_lasterror, g_buffer );
    fprintf( stderr, "\n" );
}


// log
void EM_log( int level, const char * message, ... )
{
    va_list ap;

    if( level < CK_LOG_FINEST ) level = CK_LOG_FINEST;
    else if( level >= CK_LOG_NONE ) level = CK_LOG_NONE - 1;

    // check level
    if( level < g_loglevel ) return;

    fprintf( stderr, "[chuck]:" );
    fprintf( stderr, "(LOG-%i:%s): ", level, g_str[level] );

    va_start( ap, message );
    vfprintf( stderr, message, ap );
    va_end( ap );

    fprintf( stderr, "\n" );
}


// set log level
void EM_setlog( int level )
{
    if( level < CK_LOG_FINEST ) level = CK_LOG_FINEST;
    else if( level > CK_LOG_NONE ) level = CK_LOG_NONE;
    g_loglevel = level;

    // log this
    EM_log( CK_LOG_SYSTEM, "log level set to: %i (%s)...", level, g_str[level] );
}

// prepare new file
t_CKBOOL EM_reset( const char * fname, FILE * fd )
{
    anyErrors = FALSE;
    fileName = fname ? fname : (c_str)"";
    lineNum = 1;
    EM_lineNum = 1;

    // free the intList
    IntList curr = NULL;
    while( linePos )
    {
        curr = linePos;
        linePos = linePos->rest;
        // free
        free( curr );
    }

    // make new intList
    linePos = intList( 0, NULL );

    // TODO: if( yyin ) fclose( yyin );
    if( fd ) yyin = fd;
    else yyin = fopen( fname, "r" );
    if (!yyin)
        EM_error2( 0, "no such file or directory" );
    else
        fseek( yyin, 0, SEEK_SET );

    return (yyin != 0);
}


// change file
void EM_change_file( const char * fname )
{
    // set
    fileName = fname ? fname : (c_str)"";
    // more set
    lineNum = 0;
    EM_lineNum = 0;
}


// return last error
const char * EM_lasterror()
{
    return g_lasterror;
}
