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
// file: util_console.cpp
// desc: ...
//
// author: Spencer Salazar (ssalazar@princeton.edu)
// date: Autumn 2005
//-----------------------------------------------------------------------------
#include "util_console.h"
#include <stdio.h>
#ifdef __USE_READLINE__
 #include <readline/readline.h>
#else
 #include <stdlib.h>
 #define CONSOLE_INPUT_BUFFER_SIZE 255
#endif

char * io_readline( const char * prompt )
{
#ifdef __USE_READLINE__

    // call the real readline
    return readline( prompt );

#else

    // insert our hack
    char * buf=(char *)malloc( CONSOLE_INPUT_BUFFER_SIZE * sizeof(char) );
    char * result;
    
    fputs( prompt, stdout );
    
	result = fgets( buf, CONSOLE_INPUT_BUFFER_SIZE, stdin );
	
	if( result == NULL )
	{
		free( buf );
		return NULL;
	}
	
	for( int i=0; i < CONSOLE_INPUT_BUFFER_SIZE; i++ )
		if(buf[i] == '\n' )
		{
			buf[i] = 0;
			break;
		}
	
	return buf;
	
#endif
}

void io_addhistory( const char * addme )
{
#ifdef __USE_READLINE__
	
	add_history( addme );
	
#else
	
	//do nothing
	
#endif
}


// kb hit
#ifndef __PLATFORM_WIN32__
  #include <stdio.h>
  #include <string.h>
  #include <termios.h>

  // global
  static struct termios g_save;
#else
  #include <stdio.h>
  #include <conio.h>
#endif


// global
static t_CKINT g_c;


// on entering mode
t_CKBOOL initscr()
{
#ifndef __PLATFORM_WIN32__
    struct termios term;
 
    if( ioctl( 0, TIOCGETA, &term ) == -1 ) 
    { 
        EM_log( CK_LOG_SYSTEM,"(kbhit disabled): standard input not a tty!");
        return FALSE;
    }

    g_save = term;
                
    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;

    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME]=0;  
                
    ioctl( 0, TIOCSETA, &term );
#endif

    return TRUE;
}


// on exit
void kb_endwin()
{
#ifndef __PLATFORM_WIN32__
    ioctl( 0, TIOCSETA, &g_save );
#endif
}


// hit
t_CKINT kb_hit()
{
#ifndef __PLATFORM_WIN32__
    int ifkeyin;
    char c;
    ifkeyin = read( 0, &c, 1 );
    g_c = (t_CKINT)c;

    return(ifkeyin);
#else
    return (t_CKINT)kbhit();
#endif
}

// get
t_CKINT kb_getch()
{
#ifndef __PLATFORM_WIN32__
    return g_c;
#else
    return (t_CKINT)::getch();
#endif
}
