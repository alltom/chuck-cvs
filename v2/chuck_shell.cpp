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
#ifndef __PLATFORM_WIN32__
 #include <unistd.h>
 #include <sys/param.h>
 #include <sys/types.h>
 #include <dirent.h>
#endif

using namespace std;

// global shell pointer (lives in chuck_main)
extern Chuck_Shell * g_shell;

//-----------------------------------------------------------------------------
// name: divide_string
// desc: divides string into substrings, each separated by an arbitrarily long 
//       sequence of any characters in edge.  
//-----------------------------------------------------------------------------
void divide_string( string str, const string & edge, 
                    vector< string > & substrings)
{
    //string space = " ";
	//string singleq = "'";
	//string doubleq = "\"";
    str.append( " " );

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
    process_vm = NULL;
    initialized = FALSE;
    stop = FALSE;
}




//-----------------------------------------------------------------------------
// name: ~Chuck_Shell()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell::~Chuck_Shell()
{
    // iterate through map, delete modes?
  	//iterator i = modes

	// iterate through map, delete vms

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
	
	process_vm = vm;

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
    for( ; !stop; )
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
// name: close()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::close()
{
	stop = TRUE;
}

//-----------------------------------------------------------------------------
// name: kill()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::kill()
{
	stop = TRUE;
	if( process_vm != NULL )
	{
		process_vm->stop();
	}
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
t_CKBOOL Chuck_Shell_Network_VM::add_shred( const vector< string > & files, 
											string & out )
{
	long int i = 0;
	t_CKBOOL return_val;
	int j, str_len, vec_len = files.size() + 1;
	char ** argv = new char * [ vec_len ];
	
	/* prepare an argument vector to submit to otf_send_cmd */
	/* first, specify an add command */
	argv[0] = new char [2];
	strncpy( argv[0], "+", 2 );
	
	/* copy file paths into argv */
	for( j = 1; j < vec_len; j++ )
	{
		str_len = files[j - 1].size() + 1;
		argv[j] = new char [str_len];
		strncpy( argv[j], files[j - 1].c_str(), str_len );
	}
	
	/* send the command */
	if( otf_send_cmd( vec_len, argv, i, hostname.c_str(), port ) )
		return_val = TRUE;
	else
	{
		out += "add command failed\n";
		return_val = FALSE;
	}

	/* clean up heap data */
	for( j = 0; j < vec_len; j++ )
		delete[] argv[j];
	delete[] argv;
	
	return return_val;
}

//-----------------------------------------------------------------------------
// name: remove_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Network_VM::remove_shred( const vector< string > & ids, 
											   string & out )
{
	long int i = 0;
	t_CKBOOL return_val;
	int j, str_len, vec_len = ids.size() + 1;
	char ** argv = new char * [ vec_len ];
	
	/* prepare an argument vector to submit to otf_send_cmd */
	/* first, specify an add command */
	argv[0] = new char [2];
	strncpy( argv[0], "-", 2 );
	
	/* copy ids into argv */
	for( j = 1; j < vec_len; j++ )
	{
		str_len = ids[j - 1].size() + 1;
		argv[j] = new char [str_len];
		strncpy( argv[j], ids[j - 1].c_str(), str_len );
	}
	
	/* send the command */
	if( otf_send_cmd( vec_len, argv, i, hostname.c_str(), port ) )
		return_val = TRUE;
	else
	{
		out += "remove command failed\n";
		return_val = FALSE;
	}

	/* clean up heap data */
	for( j = 0; j < vec_len; j++ )
		delete[] argv[j];
	delete[] argv;
	
	return return_val;
}

//-----------------------------------------------------------------------------
// name: remove_all()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Network_VM::remove_all( string & out )
{
	int return_val = TRUE;
	long int j = 0;
	char ** argv = new char * [1];
	argv[0] = "--removeall";
	if( !otf_send_cmd( 1, argv, j, hostname.c_str(), port ) )
	{
		out += "removeall command failed\n";
		return_val = FALSE;
	}

	return return_val;
}

//-----------------------------------------------------------------------------
// name: remove_last()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Network_VM::remove_last( string & out )
{
	int return_val = TRUE;
	long int j = 0;
	char ** argv = new char * [1];
	argv[0] = "--";
	if( !otf_send_cmd( 1, argv, j, hostname.c_str(), port ) )
	{
		out += "removelast command failed\n";
		return_val = FALSE;
	}
	
	return return_val;
}

//-----------------------------------------------------------------------------
// name: replace_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Network_VM::replace_shred( const vector< string > &vec,
										string & out )
{
	long int i = 0;
	t_CKBOOL return_val;
	int j, str_len, vec_len = vec.size() + 1;
	char ** argv = new char * [ vec_len ];
	
	/* prepare an argument vector to submit to otf_send_cmd */
	/* first, specify an add command */
	argv[0] = new char [sizeof( "--replace" )];
	strncpy( argv[0], "--replace", sizeof( "--replace" ) );
	
	/* copy ids/files into argv */
	for( j = 1; j < vec_len; j++ )
	{
		str_len = vec[j - 1].size() + 1;
		argv[j] = new char [str_len];
		strncpy( argv[j], vec[j - 1].c_str(), str_len );
	}
	
	/* send the command */
	if( otf_send_cmd( vec_len, argv, i, hostname.c_str(), port ) )
		return_val = TRUE;
	else
	{
		out += "replace command failed\n";
		return_val = FALSE;
	}
	
	if( vec.size() % 2 != 0 )
	{
		out += "ignoring excess arguments...\n";
		return FALSE;
	}
	
/* clean up heap data */
	for( j = 0; j < vec_len; j++ )
		delete[] argv[j];
	delete[] argv;
	
	return return_val;
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
// name: kill()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Network_VM::kill( string & out )
{
	char ** argv = new char * [1];
	long int j = 0;
	t_CKBOOL return_val;
	
	argv[0] = "--kill";
	
	if( otf_send_cmd( 1, argv, j, hostname.c_str(), port ) )
		return_val = TRUE;
	else
	{
		return_val = FALSE;
		out += "kill command failed";
	}
	
	return return_val;
}

//-----------------------------------------------------------------------------
// name: fullname()
// desc: returns a somewhat descriptive full name for this VM
//-----------------------------------------------------------------------------
string Chuck_Shell_Network_VM::fullname()
{
	char buf[16];
	sprintf( buf, ":%u", port );
	
	return hostname + buf;
}
/*
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
t_CKBOOL Chuck_Shell_Process_VM::add_shred( const vector< string > & files, 
											string & out )
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: remove_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Process_VM::remove_shred( const vector< string > & id, 
											   string & out )
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: remove_all()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Process_VM::remove_all( const vector< string > & id, 
											 string & out )
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: remove_last()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Process_VM::remove_last( const vector< string > & id, 
											  string & out )
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// name: replace_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_Process_VM::replace_shred( const vector< string > & id, 
												string & out )
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
// name: fullname()
// desc: returns a somewhat descriptive full name for this VM
//-----------------------------------------------------------------------------
string Chuck_Shell_Process_VM::fullname()
{
	return "local process VM";
}
*/
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
    save_current_vm = true;
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
    
    // first check if the first token matches an alias
    while( aliases.find( vec[0] ) != aliases.end() )
    {
    	vector< string > vec2;
    	divide_string( aliases[vec[0]], " \t\n\v", vec2 );
    	vec.erase( vec.begin() );
    	vec2.insert( vec2.end(), vec.begin(), vec.end() );
    	vec = vec2;
    }
    
    if( vec[0] == "vm" )
    {
    	vec.erase( vec.begin() );
    	
    	if( vec[0] == "attach" || vec[0] == "@" )
    	{
    		vec.erase( vec.begin() );
    		vm_attach( vec, out );
    	}
    	
    	else if( vec[0] == "add" || vec[0] == "+" )
    	{
    		vec.erase( vec.begin() );
    		vm_add( vec, out );
    	}
    	
    	else if( vec[0] == "remove" || vec[0] == "-" )
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
    	
    	else if( vec[0] == "=" || vec[0] == "swap" )
    	{
    	vec.erase( vec.begin() );
    	vm_swap( vec, out );
    	}
    	
    	else
    		out = "shell: unknown command 'vm " + vec[0] + "'\n";
    }
    
    else if( vec[0] == "vms" )
    {
    	vec.erase( vec.begin() );
    	vm_list( vec, out );	
    }
    
    else if( vec[0] == "add" || vec[0] == "+")
    {
    	vec.erase( vec.begin() );
    	add( vec, out );
    }
    
    else if( vec[0] == "remove" || vec[0] == "-" )
    {
    	vec.erase( vec.begin() );
    	remove( vec, out );
    }
    
    else if( vec[0] == "removeall" || vec[0] == "remall" )
    {
    	vec.erase( vec.begin() );
    	removeall( vec, out );
    }
    
    else if( vec[0] == "--" )
    {
    	vec.erase( vec.begin() );
    	removelast( vec, out );
    }
    
    else if( vec[0] == "replace" || vec[0] == "=" )
    {
    	vec.erase( vec.begin() );
    	replace( vec, out );
    }
    
    else if( vec[0] == "close" )
    {
    	vec.erase( vec.begin() );
    	close( vec, out );
    }
    
    else if( vec[0] == "kill" )
    {
    	vec.erase( vec.begin() );
    	kill( vec, out );
    }
    
    else if( vec[0] == "ls" )
    {
    	vec.erase( vec.begin() );
    	ls( vec, out );
    }
    
    else if( vec[0] == "pwd" )
    {
    	vec.erase( vec.begin() );
    	pwd( vec, out );
    }
    
    else if( vec[0] == "cd" )
    {
    	vec.erase( vec.begin() );
    	cd( vec, out );
    }
    
    else if( vec[0] == "alias" )
    {
    	vec.erase( vec.begin() );
    	alias( vec, out );
    }
    
    else if( vec[0] == "unalias" )
    {
    	vec.erase( vec.begin() );
    	unalias( vec, out );
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
	current_vm->add_shred( argv, out );
}

//-----------------------------------------------------------------------------
// name: remove()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::remove( const vector< string > & argv,
										  string & out )
{
	current_vm->remove_shred( argv, out );
}

//-----------------------------------------------------------------------------
// name: removeall()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::removeall( const vector< string > & argv,
										  string & out )
{
	current_vm->remove_all( out );
	if( argv.size() > 0 )
		out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: removelast()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::removelast( const vector< string > & argv,
										  string & out )
{
	current_vm->remove_last( out );
	if( argv.size() > 0 )
		out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: replace()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::replace( const vector< string > & argv,
										  string & out )
{
	current_vm->replace_shred( argv, out );
}

//-----------------------------------------------------------------------------
// name: kill()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::kill( const vector< string > & argv,
									  string & out )
{
	current_vm->kill( out );
	if( argv.size() > 0 )
		out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: close()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::close( const vector< string > & argv,
									  string & out )
{
	host_shell->close();
	if( argv.size() > 0 )
		out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: ls()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::ls( const vector< string > & argv,
									  string & out )
{
#ifndef __PLATFORM_WIN32__
	
	if( argv.size() == 0 )
		{
		DIR * dir_handle = opendir( "." );
		dirent * dir_entity = readdir( dir_handle );
		
		while( dir_entity != NULL )
		{
			out += string( dir_entity->d_name, dir_entity->d_namlen ) + "\n";
			dir_entity = readdir( dir_handle );
		}
		
		closedir( dir_handle );
		return;
		}
	
	int i, len = argv.size();
	t_CKBOOL print_parent_name = len > 1 ? TRUE : FALSE;
	
	for( i = 0; i < len; i++ )
	{
		DIR * dir_handle = opendir( argv[i].c_str() );
		dirent * dir_entity = readdir( dir_handle );
		
		if( print_parent_name )
			out += argv[i] + ":\n";
		
		while( dir_entity != NULL )
		{
			out += string( dir_entity->d_name, dir_entity->d_namlen ) + "\n";
			dir_entity = readdir( dir_handle );
		}
		
		if( print_parent_name )
			out += "\n";
		
		closedir( dir_handle );
	}
	
#else
	out += "command not yet supported on Win32!\n";
#endif
}

//-----------------------------------------------------------------------------
// name: cd()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::cd( const vector< string > & argv,
									string & out )
{
#ifndef __PLATFORM_WIN32__
	if( argv.size() < 1 )
	{
		if( chdir( getenv( "HOME" ) ) )
			out += "cd command failed\n";
	}
	
	else
	{
		if( chdir( argv[0].c_str() ) )
			out += "cd command failed\n";
	}
	
#else
	out += "command not yet supported on Win32!\n";
#endif
}

//-----------------------------------------------------------------------------
// name: pwd()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::pwd( const vector< string > & argv,
									string & out )
{
#ifndef __PLATFORM_WIN32__
	char * cwd = getcwd( NULL, 0 );
	out += string( cwd ) + "\n";
	free( cwd );
	if( argv.size() > 0 )
		out += "ignoring excess arguments...\n";
#else
	out += "command not yet supported on Win32!\n";
#endif
}

//-----------------------------------------------------------------------------
// name: alias()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::alias( const vector< string > & argv,
									  string & out )
{
	int i, j, len = argv.size();
	string a, b;
	
	for( i = 0; i < len; i++ )
	{
		j = argv[i].find( "=" );
		
		// no alias assignment specified; just print the alias value if exists
		if( j == string::npos )
		{
			// see if the alias exists in the map
			if( aliases.find( argv[i] ) == aliases.end() )
				out += "alias " + argv[i] + " not found\n";
			else
				out += "alias " + argv[i] + "='" + aliases[argv[i]] + "'\n";
		}
		// create the alias
		else
		{
			a = string( argv[i], 0, j );
			b = string( argv[i], j + 1, string::npos );
			aliases[a] = b;
		}
	}
}

//-----------------------------------------------------------------------------
// name: unalias()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::unalias( const vector< string > & argv,
										string & out )
{
	
}

//-----------------------------------------------------------------------------
// name: vm_attach()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::vm_attach( const vector< string > & argv,
										  string & out )
{
	string hostname;
	int port;
	
	/* parse the hostname:port */
	if( argv.size() == 0 )
		/* no hostname:port specified, use default */
	{
		hostname = "localhost";
		port = 8888;
	}
	else
	{
		int i = argv[0].find( ":" );

		if( i == string::npos )
		{
			hostname = argv[0];
			port = 8888;
		}
		
		else
		{
			port = strtol( string( argv[0], i + 1 ).c_str(), NULL, 10 );
			if( port == 0 /* && errno == EINVAL */ )
				out += "invalid port '" + string( argv[0], i + 1 ) + "'\n";
			else
				hostname = string( argv[0], 0, i );
		}
	}
		
	if( port != 0 )
	{
		Chuck_Shell_Network_VM * new_vm = new Chuck_Shell_Network_VM();
		new_vm->init( hostname, port );
		//if( !new_vm->test() )
		//	{
			
		//	}
		if( !save_current_vm )
			SAFE_DELETE( current_vm );
		current_vm = new_vm;
		save_current_vm = FALSE;
		out += argv[0] + " is now current VM\n";
	}
	else 
		out += "unable to attach to " + argv[0] + "\n";
	
	if( argv.size() > 1 )
		out += "ignoring excess arguments...";
	
}

//-----------------------------------------------------------------------------
// name: vm_add()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::vm_add( const vector< string > & argv,
									   string & out )
{
	char buf[16];
	
	vms->push_back( current_vm );
	save_current_vm = TRUE;
	
	sprintf( buf, "%u", vms->size() - 1 );
	out += current_vm->fullname() + " saved as VM " + buf + "\n";
	
	if( argv.size() > 0 )
		out += "ignoring excess arguments...\n";
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
	char buf[16];
	int i, len = vms->size();
	
	out += string("current VM: ") + current_vm->fullname() + "\n";
	
	for( i = 0; i < len; i++ )
	{
		sprintf( buf, "%u", i );
		out += string( "VM " ) + buf + ": " + (*vms)[i]->fullname() + "\n";
	}
	
	if( argv.size() > 0 )
		out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: vm_swap()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::vm_swap( const vector< string > & argv,
										string & out )
{
	int new_vm;
	
	if( argv.size() < 1 )
	{
		out += string( "too few arguments...\n" );
		return;
	}
	
	new_vm = strtol( argv[0].c_str(), NULL, 10 );
	if( new_vm >= vms->size() || new_vm < 0 || (*vms)[new_vm] == NULL )
	{
		out += string( "invalid VM: " ) + argv[0];
		return;
	}
	
	if( !save_current_vm )
		SAFE_DELETE( current_vm );
	save_current_vm = TRUE;
	current_vm = (*vms)[new_vm];
	out += "current VM is now " + current_vm->fullname() + "\n";
}

//-----------------------------------------------------------------------------
// name: vm_attach_add()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell_Mode_Command::vm_attach_add( const vector< string > & argv,
										  	  string & out )
{
	vm_attach( argv, out );
	vm_add( vector< string >(), out );
}


