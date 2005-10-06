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
// file: chuck_console.cpp
// desc: ...
//
// author: Spencer Salazar (ssalazar@princeton.edu)
// date: Autumn 2005
//-----------------------------------------------------------------------------

#include "chuck_console.h"
#ifdef __USE_READLINE__
#include <readline/readline.h>
#else
#include "util_consoleio.h"
#endif

//-----------------------------------------------------------------------------
// name: Chuck_Console()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Console::Chuck_Console() : Chuck_Shell_UI()
{
}

//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Console::init()
{

    return TRUE;
}

//-----------------------------------------------------------------------------
// name: nextCommand()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Console::nextCommand(std::string &out)
{
    char * line_read=NULL;
    
    line_read = readline( "shell %> " );
    
    if( line_read == NULL)
        {
        out = "";
        return FALSE;
        }
    
    if( *line_read == 0 )
        add_history( line_read );        
    
    out = line_read;
    
    free( line_read );
    
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: nextResult()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Console::nextResult(const std::string &in)
{
    printf( in.c_str() );
}



