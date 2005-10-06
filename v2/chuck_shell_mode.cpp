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
// file: chuck_shell_mode.cpp
// desc: ...
//
// author: Spencer Salazar (ssalazar@princeton.edu)
// date: Autumn 2005
//-----------------------------------------------------------------------------
#include "chuck_shell_mode.h"


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
Chuck_Shell_Mode_Command::Chuck_Shell_Mode_Command() : Chuck_Shell_Mode()
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
