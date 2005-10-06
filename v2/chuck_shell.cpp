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
// file: chuck_shell.cpp
// desc: ...
//
// author: Spencer Salazar (ssalazar@princeton.edu)
// date: Autumn 2005
//-----------------------------------------------------------------------------
#include "chuck_shell.h"
#include "chuck_shell_mode.h"

static const char CHUCK_SHELL_ESCAPE_CHAR='#';

extern Chuck_Shell * g_shell;

//-----------------------------------------------------------------------------
// name: shell_cb
// desc: thread routine
//-----------------------------------------------------------------------------
void * shell_cb( void * p )
{
    // log
    EM_log( CK_LOG_INFO, "starting thread routine for shell..." );
    EM_pushlog();
    
    if(g_shell == NULL)
        EM_log(CK_LOG_SYSTEM_ERROR, "chuck shell not instantiated" );
    else
        g_shell->run();
    
    //delete g_shell;
    //delete g_shell_ui;
    EM_poplog();
    EM_log( CK_LOG_INFO, "exiting thread routine for shell..." );
    return NULL;
}




//-----------------------------------------------------------------------------
// name: Chuck_Shell()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell::Chuck_Shell()
{
    vm = NULL;
    compiler = NULL;
    current_mode = NULL;
    ui = NULL;
    initialized=FALSE;
}




//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell::init( Chuck_VM * vm, Chuck_Compiler * compiler,
                            Chuck_Shell_UI * ui )
{
    // log
    EM_log( CK_LOG_SYSTEM, "initializing chuck shell..." );
    // push log
    EM_pushlog();
    
    if(initialized == TRUE)
        {
        EM_log( CK_LOG_WARNING, "chuck shell already initialized" );
        return TRUE;
        }
    
    if(vm == NULL)
        {
        EM_log( CK_LOG_SYSTEM_ERROR, "NULL VM passed to Chuck_Shell::init" );
        return FALSE;
        }
    this->vm = vm;
    
    if(compiler == NULL)
        {
        EM_log( CK_LOG_SYSTEM_ERROR, "NULL compiler passed to Chuck_Shell::init" );
        return FALSE;
        }
    this->compiler = compiler;

    if(ui == NULL)
        {
        EM_log( CK_LOG_SYSTEM_ERROR, "NULL ui passed to Chuck_Shell::init" );
        return FALSE;
        }
    this->ui = ui;

	current_mode=new Chuck_Shell_Mode_Command();
	if( !current_mode->init( vm, compiler ) )
		{
		EM_log( CK_LOG_SYSTEM_ERROR, "unable to initialize shell command mode");
		//delete current_mode;
		return FALSE;
		}

    initialized=TRUE;
    
    // pop log
    EM_poplog();

    return TRUE;
}

//-----------------------------------------------------------------------------
// name: run()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::run()
{
    EM_log( CK_LOG_SYSTEM, "running chuck shell" );
    EM_pushlog();
    
    if(initialized == FALSE)
        {
        EM_log( CK_LOG_SYSTEM_ERROR, "chuck shell not initialized" );
        }
    else
        {    
        std::string command;
        std::string result;
        
        for(;;)
            {
            if(ui->nextCommand(command) == TRUE)
                {
                if( command[0] == CHUCK_SHELL_ESCAPE_CHAR )
                	{
					//interpret the shell meta-command
					result="(meta-command)\n";
                	}
				else
					current_mode->execute(command,result);
				ui->nextResult(result);
                }
            else
                break;
            }
        }
    
    EM_poplog();
    EM_log( CK_LOG_SYSTEM, "exiting chuck shell run routine" );
}

//-----------------------------------------------------------------------------
// name: Chuck_Shell_UI()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_UI::Chuck_Shell_UI()
{

}

//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_UI::init()
{
    return TRUE;
}

