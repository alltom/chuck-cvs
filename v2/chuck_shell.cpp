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
#include "chuck_otf.h"
#include "util_network.h"

#include <stdio.h>
#include <errno.h>

using namespace std;

// global shell pointer (lives in chuck_main)
extern Chuck_Shell * g_shell;

// SIGPIPE mode
extern t_CKUINT g_sigpipe_mode;


//-----------------------------------------------------------------------------
// name: divide_string
// desc: divides string into substrings, each separated by an arbitrarily long 
//       sequence of any characters in edge.  
//-----------------------------------------------------------------------------
void divide_string( const string & stra, const string & edge, 
                    vector< string > & substrings)
{
    string str = stra + " ";
    int i = str.find_first_not_of( edge ), j = 0;
    for(;;)
    {
        j = str.find_first_of( edge, i );
        if( j == string::npos )
            break;
        substrings.push_back( string( str, i, j - i ) );
        i = str.find_first_not_of( edge, j );
    }
}




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
  	//iterator i = modes
  
    // delete ui
    SAFE_DELETE( ui );

    // flag
    initialized = FALSE;

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
    
    // ui
    if( ui == NULL )
    {
        fprintf( stderr, "[chuck](via shell): NULL ui passed to Chuck_Shell::init\n" );
        return FALSE;
    }
    this->ui = ui;

    Chuck_Shell_Network_VM * cspv = new Chuck_Shell_Network_VM();
    if( !cspv->init( "localhost", 8888 ) )
    {
        fprintf( stderr, "[chuck](via shell): error initializing process VM\n" );
        SAFE_DELETE( cspv );
		return FALSE;
    }

    vms = vector< Chuck_Shell_VM * > ();
    vms.push_back( cspv );
    
    // make new mode
    current_mode = new Chuck_Shell_Mode_Command();
    // initialize it
    if( !current_mode->init( &vms, this ) )
    {
        fprintf( stderr, "[chuck](via shell): unable to initialize shell command mode\n");
        SAFE_DELETE( vms[0] );
        SAFE_DELETE( current_mode );
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

    Chuck_Shell_Request command;
    Chuck_Shell_Response result;

    // loop
    for(;;)
    {
        // get command
        if( ui->next_command( command ) == TRUE )
        {
			// result.clear();
			result = "";

			// execute the command
			current_mode->execute(command,result);

			// command.clear();
			command = "";

            // pass the result to the shell ui
            ui->next_result( result );
        }
        else
            // done
            break;
    }
    
    // log
    EM_poplog();
    EM_log( CK_LOG_SYSTEM, "exiting chuck shell..." );
}


//-----------------------------------------------------------------------------
// name: Chuck_Shell_VM()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_VM::Chuck_Shell_VM()
{
}

//-----------------------------------------------------------------------------
// name: ~Chuck_Shell_VM()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_VM::~Chuck_Shell_VM()
{
}

//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_VM::init()
{
}

//-----------------------------------------------------------------------------
// name: Chuck_Shell_Network_VM()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_Network_VM::Chuck_Shell_Network_VM()
{
}

//-----------------------------------------------------------------------------
// name: ~Chuck_Shell_Network_VM()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_Network_VM::~Chuck_Shell_Network_VM()
{
}

//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Network_VM::init( const string & hostname, t_CKINT port )
{
	this->hostname = hostname;
	this->port = port;
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: add_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Network_VM::add_shred( const string & file, string & out )
{
	long int i = 0;
	char str[1024];
	strncpy( str, file.c_str(), 1024 );
	char * argv[] = { "+", str };
	otf_send_cmd( 2, argv, i, hostname.c_str(), port );
	/**** it would awesome if arg2 of otf_send_cmd was const char ** ****/
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: remove_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Network_VM::remove_shred( t_CKUINT id, string & out )
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: status()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Network_VM::status( string & out )
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: Chuck_Shell_Process_VM()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_Process_VM::Chuck_Shell_Process_VM()
{
}

//-----------------------------------------------------------------------------
// name: ~Chuck_Shell_Process_VM()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_Process_VM::~Chuck_Shell_Process_VM()
{
}

//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Process_VM::init( Chuck_VM * vm, 
									   Chuck_Compiler * compiler )
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: add_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Process_VM::add_shred( const string & file, string & out )
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: remove_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Process_VM::remove_shred( t_CKUINT id, string & out )
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: status()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Process_VM::status( string & out )
{
	return TRUE;
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
    vms = NULL;
    current_vm = NULL;
    host_shell = NULL;
    initialized = FALSE;
}




//-----------------------------------------------------------------------------
// name: ~Chuck_Shell_Mode
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_Mode::~Chuck_Shell_Mode()
{
}




//-----------------------------------------------------------------------------
// name: init
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Mode::init( vector< Chuck_Shell_VM * > * vms, 
                                 Chuck_Shell * host_shell ) 
{
    // TODO: input validation
    this->host_shell = host_shell;
    this->vms = vms;
    this->current_vm = (*vms)[0];
    initialized = TRUE;
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
// name: ~Chuck_Shell_Mode_Command
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell_Mode_Command::~Chuck_Shell_Mode_Command()
{
}




//-----------------------------------------------------------------------------
// name: execute
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Mode_Command::execute( const Chuck_Shell_Request & in, 
                                            Chuck_Shell_Response & out )
{
	if( !initialized )
	{
		//fprintf();
		return FALSE;
	}
	
    // vector of string tokens
    vector< string > vec;
        
    // clear the response
    out = "";

    // divide the string into white space separated substrings
    divide_string( in, " \t\n\v",  vec );
    // if no tokens
    if( vec.size() == 0) 
    {
        out = "";
        return TRUE;
    }    
    
    if( vec[0] == "vm" )
    {
    	vec.erase( vec.begin() );
    	
    	if( vec[0] == "attach" || vec[1] == "@" )
    	{
    		vec.erase( vec.begin() );
    		vm_attach( vec, out );
    	}
    	
    	else if( vec[0] == "add" || vec[1] == "+" )
    	{
    		vec.erase( vec.begin() );
    		vm_add( vec, out );
    	}
    	
    	else if( vec[0] == "remove" || vec[1] == "-" )
    	{
    		vec.erase( vec.begin() );
    		vm_remove( vec, out );
    	}
    	
    	else if( vec[0] == "list" )
    	{
    		vec.erase( vec.begin() );
    		vm_list( vec, out );
    	}
    	
    	else if( vec[0] == "@+" )
    	{
    		vec.erase( vec.begin() );
    		vm_attach_add( vec, out );
    	}
    	else
    		out = "shell: unknown command 'vm " + vec[0] + "'\n";
    }
    
    else if( vec[0] == "vms" )
    {
    	vec.erase( vec.begin() );
    	vm_list( vec, out );	
    }
    
    else if( vec[0] == "add" )
    {
    	vec.erase( vec.begin() );
    	add( vec, out );
    }
    
    else
    	out = "shell: unknown command '" + vec[0] + "'\n";
    
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: add()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::add( const vector< string > & argv,
										  string & out )
{
	if( current_vm->add_shred(argv[0], out) )
		out += argv[0] + " added\n";
}

//-----------------------------------------------------------------------------
// name: vm_attach()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::vm_attach( const vector< string > & argv,
										  string & out )
{
	string hostname = argv[0];
	
	out += argv[0] + " attached\n";
}

//-----------------------------------------------------------------------------
// name: vm_add()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::vm_add( const vector< string > & argv,
									   string & out )
{
	
}

//-----------------------------------------------------------------------------
// name: vm_remove()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::vm_remove( const vector< string > & argv,
										  string & out )
{
	
}

//-----------------------------------------------------------------------------
// name: vm_list()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::vm_list( const vector< string > & argv,
										string & out )
{
	
}

//-----------------------------------------------------------------------------
// name: vm_attach_add()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::vm_attach_add( const vector< string > & argv,
										  	  string & out )
{
	
}


