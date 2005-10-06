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


// escape char
static const char CHUCK_SHELL_ESCAPE_CHAR = '#';

// global shell pointer (lives in chuck_main)
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

    // assuming this is absolutely necessary, an assert may be better
    assert( g_shell != NULL );

    // run the shell
    g_shell->run();
    
    // delete and set to NULL
    SAFE_DELETE( g_shell );
    // perhaps let shell destructor clean up mode and ui?
    
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
    initialized = FALSE;
}




//-----------------------------------------------------------------------------
// name: ~Chuck_Shell()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell::~Chuck_Shell()
{
    // iterate through map, delete modes?

    // delete ui?
    SAFE_DELETE( ui );

    // flag
    initialized = FALSE;

    // zero out
    vm = NULL;
    compiler = NULL;
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
    
    // check
    if( initialized == TRUE )
    {
        EM_log( CK_LOG_WARNING, "chuck shell already initialized" );
        return TRUE;
    }
    
    // vm
    if( vm == NULL )
    {
        fprintf( stderr, "[chuck](via shell): NULL VM passed to Chuck_Shell::init\n" );
        return FALSE;
    }
    this->vm = vm;

    // compiler
    if(compiler == NULL)
    {
        fprintf( stderr, "[chuck](via shell): NULL compiler passed to Chuck_Shell::init\n" );
        return FALSE;
    }
    this->compiler = compiler;

    // ui
    if( ui == NULL )
    {
        fprintf( stderr, "[chuck](via shell): NULL ui passed to Chuck_Shell::init\n" );
        return FALSE;
    }
    this->ui = ui;

    // make new mode
    current_mode = new Chuck_Shell_Mode_Command();
    // initialize it
    if( !current_mode->init( vm, compiler ) )
    {
        fprintf( stderr, "[chuck](via shell): unable to initialize shell command mode\n");
        //delete current_mode;
        return FALSE;
    }

    // flag
    initialized = TRUE;
    
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
    // log
    EM_log( CK_LOG_SYSTEM, "running chuck shell..." );
    EM_pushlog();
    
    // make sure
    if(initialized == FALSE)
    {
        fprintf( stderr, "[chuck](via shell): shell not initialized...\n" );
        return;
    }

    std::string command;
    std::string result;

    // loop
    for(;;)
    {
        // get command
        if( ui->nextCommand( command ) == TRUE )
        {
            if( command[0] == CHUCK_SHELL_ESCAPE_CHAR )
            {
                // interpret the shell meta-command
                result = "(meta-command)\n";
            }
            else
            {
                // execute the command
                current_mode->execute(command,result);
            }

            // pass the result to the shell ui
            ui->nextResult( result );
        }
        else
            // done
            break;
    }
    
    // log
    EM_poplog();
    EM_log( CK_LOG_SYSTEM, "exiting chuck shell run routine..." );
}




//-----------------------------------------------------------------------------
// name: Chuck_Shell_UI()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_UI::Chuck_Shell_UI()
{
}




//-----------------------------------------------------------------------------
// name: ~Chuck_Shell_UI()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_UI::~Chuck_Shell_UI()
{
    // this being chuck, there isn't much need to anything here =P
}




//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_UI::init()
{
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: Chuck_Shell_Mode
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_Mode::Chuck_Shell_Mode()
{
	vm = NULL;
	compiler = NULL;
	initialized = FALSE;
}




//-----------------------------------------------------------------------------
// name: init
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Mode::init( Chuck_VM * vm, Chuck_Compiler * compiler )
{
	this->vm = vm;
	this->compiler = compiler;
	initialized = TRUE;
	return TRUE;
}



//-----------------------------------------------------------------------------
// name: switch_vm
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Mode::switch_vm( Chuck_VM * new_vm, Chuck_VM ** old_vm )
{
	if( old_vm != NULL )
		*old_vm = this->vm;
	if( new_vm != NULL )
		this->vm = new_vm;
	else
		{
		EM_log( CK_LOG_SYSTEM_ERROR, "NULL VM passed to switch_vm" );
		return FALSE;
		}
	return TRUE;
}



//-----------------------------------------------------------------------------
// name: switch_compiler
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Mode::switch_compiler( Chuck_Compiler * new_compiler, 
                                            Chuck_Compiler ** old_compiler )
{
	if( old_compiler != NULL )
		*old_compiler = this->compiler;
	if( new_compiler != NULL )
		this->compiler = new_compiler;
	else
		{
		EM_log( CK_LOG_SYSTEM_ERROR, 
				"NULL compiler passed to switch_compiler" );
		return FALSE;
		}
	return TRUE;
}




//-----------------------------------------------------------------------------
// name: Chuck_Shell_Mode_Command
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_Mode_Command::Chuck_Shell_Mode_Command()
{
}




//-----------------------------------------------------------------------------
// name: execute
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Mode_Command::execute( const std::string & in, std::string & out )
{
	out = in + "\n";
	return TRUE;
}
