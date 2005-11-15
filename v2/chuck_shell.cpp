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
    string tmp = " ";
    string qq = "\"'";
    str.append( " " );

    int i = str.find_first_not_of( edge ), j = 0;
    if( i != 0 )
    {
        tmp[0] = str[i - 1];
    }
    
    for(;;)
    {
        j = str.find_first_of( tmp, i );
        if( j == string::npos )
            break;
        substrings.push_back( string( str, i, j - i ) );
        i = str.find_first_not_of( edge, j );
        tmp[0] = str[i - 1];
    }
}

//-----------------------------------------------------------------------------
// name: tokenize_string
// desc: divides a string into space separated tokens, respecting single and 
//      double quotes
//-----------------------------------------------------------------------------
void tokenize_string( string str, vector< string > & tokens)
{
    int space = 1;
    int end_space = 0;
    int squote = 0;
    int dquote = 0;
    int i = 0, j = 0, len = str.size();
    
    for( i = 0; i < len; i++ )
    {
        if( isspace( str[i] ) && space )
        {
            j++;
            continue;
        }
        
        if( isspace( str[i] ) && end_space )
        {
            tokens.push_back( string( str, j, i - j ) );
            j = i + 1;
            space = 1;
            end_space = 0;
            continue;
        }
        
        if( str[i] == '\'' )
        {
            if( dquote )
                continue;
            if( !squote )
            {
                str.erase( i, 1 );
                len--;
                i--;
                space = 0;
                end_space = 0;
                squote = 1;
                continue;
            }
            
            else if( str[i - 1] == '\\' )
            {
                str.erase( i - 1, 1 );
                len--;
                i--;
                continue;
            }
            
            else
            {
                str.erase( i, 1 );
                len--;
                i--;
                squote = 0;
                end_space = 1;
                space = 0;
                continue;
            }
        }
        
        if( str[i] == '"' ) //"
        {
            if( squote )
                continue;
            if( !dquote )
            {
                str.erase( i, 1 );
                i--;
                len--;
                space = 0;
                end_space = 0;
                dquote = 1;
                continue;
            }
            
            else if( str[i - 1] == '\\' )
            {
                str.erase( i - 1, 1 );
                len--;
                i--;
                continue;
            }
            
            else
            {
                str.erase( i, 1 );
                i--;
                len--;
                dquote = 0;
                end_space = 1;
                space = 0;
                continue;
            }
        }
        
        if( !squote && !dquote )
        {       
            end_space = 1;
            space = 0;
        }
    }
    
    if( i > j && end_space )
    {
        tokens.push_back( string( str, j, i - j ) );
    }
}

//-----------------------------------------------------------------------------
// name: shell_cb
// desc: thread routine
//-----------------------------------------------------------------------------
void * shell_cb( void * p )
{
	Chuck_Shell * shell;
    // log
    EM_log( CK_LOG_INFO, "starting thread routine for shell..." );
    EM_pushlog();

    // assuming this is absolutely necessary, an assert may be better
    assert( p != NULL );

	shell = ( Chuck_Shell * ) p;
	
    // run the shell
    shell->run();
    
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
    ui = NULL;
    // process_vm = NULL;
    current_vm = NULL;
    initialized = FALSE;
    stop = FALSE;
    code_entry_active = FALSE;
    save_current_vm = FALSE;
}

//-----------------------------------------------------------------------------
// name: ~Chuck_Shell()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell::~Chuck_Shell()
{
    int i, len = allocated_commands.size();    

    //iterate through commands, delete the associated heap data
    for( i = 0; i != len; i++ )
	    SAFE_DELETE( allocated_commands[i] );

    // delete ui
    SAFE_DELETE( ui );

    // flag
    initialized = FALSE;

}

//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell::init( Chuck_Shell_UI * ui )
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
    
    //process_vm = vm;

    Chuck_Shell_Network_VM * cspv = new Chuck_Shell_Network_VM();
    if( !cspv->init( "localhost", 8888 ) )
    {
        fprintf( stderr, "[chuck](via shell): error initializing process VM\n" );
        SAFE_DELETE( cspv );
        return FALSE;
    }

    vms = vector< Chuck_Shell_VM * > ();
    vms.push_back( cspv );

    current_vm = cspv;
    save_current_vm = true;
    
    code_entry_active = false;
    code = "";    
    
    // init default variables
    variables["COMMAND_PROMPT"] = "chuck %> ";
    variables["DEFAULT_CONTEXT"] = "shell_global";

    // initialize prompt
    prompt = variables["COMMAND_PROMPT"];   
    
    Command * temp;

    // initialize the string -> Chuck_Shell_Command map
    temp = new Command_VM();
    temp->init( this );
    commands["vm"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_VMList();
    temp->init( this );
    commands["vms"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Add();
    temp->init( this );
    commands["+"] = temp;
    commands["add"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Remove();
    temp->init( this );
    commands["-"] = temp;
    commands["remove"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Removeall();
    temp->init( this );
    commands["removeall"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Removelast();
    temp->init( this );
    commands["--"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Replace();
    temp->init( this );
    commands["replace"] = temp;
    commands["="] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Close();
    temp->init( this );
    commands["close"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Kill();
    temp->init( this );
    commands["kill"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Exit();
    temp->init( this );
    commands["exit"] = temp;
    commands["quit"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Ls();
    temp->init( this );
    commands["ls"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Pwd();
    temp->init( this );
    commands["pwd"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Cd();
    temp->init( this );
    commands["cd"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Alias();
    temp->init( this );
    commands["alias"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Unalias();
    temp->init( this );
    commands["unalias"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_Source();
    temp->init( this );
    commands["source"] = temp;
    commands["."] = temp;
    allocated_commands.push_back( temp );
    
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

    string command;
    string result;

    // loop
    for( ; !stop; )
    {
        // get command
        if( ui->next_command( prompt, command ) == TRUE )
        {
            // result.clear();
            result = "";
            
            // execute the command
            execute( command, result );

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
// name: execute
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell::execute( string & in, string & out )
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

    // first find out if this is code to run
    if( !code_entry_active && in[in.find_first_not_of( " \t\v\n" )] == '{' )
    	start_code();
    
    if( code_entry_active )
    {
    continue_code( in );
    
    if( code_entry_active == FALSE )
	    end_code( in, out );
	else
	// collect the next line of code
		return TRUE;
    }
    
    // divide the string into white space separated substrings
    tokenize_string( in, vec );
    
    // if no tokens
    if( vec.size() == 0) 
        return TRUE;
    
    // first check if the first token matches an alias
    while( aliases.find( vec[0] ) != aliases.end() )
    // use a while loop to handle nested aliases
    {
        vector< string > vec2;
        tokenize_string( aliases[vec[0]], vec2 );
        vec.erase( vec.begin() );
        vec2.insert( vec2.end(), vec.begin(), vec.end() );
        vec = vec2;
    }
    
    // locate the command
    if( commands.find( vec[0] ) == commands.end() )
    // command doesn't exist!
    {
        out += vec[0] + ": command not found\n";
        return FALSE;
    }
    else
    // execute the command
    {
        Command * command = commands[vec[0]];
        vec.erase( vec.begin() );
        command->execute( vec, out );
    }
        
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: start_code()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::start_code()
{
	code = "";
	code_entry_active = TRUE;
	scope = 0;
}

//-----------------------------------------------------------------------------
// name: continue_code()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::continue_code( string & in )
{
	if( in.find( "{" ) != string::npos )
	// increase the scope one level and change the prompt
	{
		char buf[16];
		scope++;
#ifndef __PLATFORM_WIN32__
		snprintf( buf, 16, "code %2d> ", scope );
#else
		sprintf( buf, "code %2d> ", scope);
#endif
		prompt = buf;
	}
	
	if( in.find( "}" ) != string::npos )
	// decrease the scope one level and change the prompt
	{
		char buf[16];
		scope--;
#ifndef __PLATFORM_WIN32__
		snprintf( buf, 16, "code %2d> ", scope );
#else
		sprintf( buf, "code %2d> ", scope);
#endif
		prompt = buf;
	}
	
	// collect this line as ChucK code
	code += in + "\n";

	if( scope == 0 )
	// the end of this code block -- lets execute it on the current_vm
		code_entry_active = FALSE;
}

//-----------------------------------------------------------------------------
// name: end_code()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::end_code( string & in, string & out )
{
	// open a temporary file
	char * tmp_filepath = tmpnam( NULL );
	FILE * tmp_file = fopen( tmp_filepath, "w" );
	
	//strip opening and closing braces
	int k = code.find( "{" );
	string head = string( code, 0, k );
	code = string( code, k + 1, code.size() - k - 1 );
	
	k = code.rfind( "}" );
	string tail = string( code, k + 1, code.size() - k - 1 );
	code = string( code, 0, k );
	
	// print out the code (for debugging)
	printf( "head: %s\ntail: %s\n", head.c_str(), tail.c_str() );
	printf( "--start code--\n%s\n--end code--\n", code.c_str() );
	
	// write the code to the temp file
	fprintf( tmp_file, "%s", code.c_str() );
	fclose( tmp_file );

	string argv = string( "+ " ) + tmp_filepath;
	
	if( this->execute( argv, out ) )
		;
	
	// delete the file
#ifndef __PLATFORM_WIN32__
	unlink( tmp_filepath );
#else
	// delete the file...
#endif // __PLATFORM_WIN32__
	
	prompt = variables["COMMAND_PROMPT"];
	in = string( tail );
}

//-----------------------------------------------------------------------------
// name: do_context()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::do_code_context( string & )
{
	
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
    /*
    if( process_vm != NULL )
    {
        process_vm->stop();
    }
    */
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
//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell_UI::init()
{
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell::Command::init( Chuck_Shell * caller )
{
    this->caller = caller;
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Add::execute( vector< string > & argv,
                                        string & out )
{
    if( caller->current_vm == NULL)
        out += "not attached to a VM\n";
    else
        caller->current_vm->add_shred( argv, out );
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Remove::execute( vector< string > & argv,
                                           string & out )
{
    if( caller->current_vm == NULL)
        out += "not attached to a VM\n";
    else
        caller->current_vm->remove_shred( argv, out );
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Removeall::execute( vector< string > & argv, 
                                              string & out )
{
    if( caller->current_vm == NULL)
        out += "not attached to a VM\n";
    else
        caller->current_vm->remove_all( out );

    if( argv.size() > 0 )
        out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Removelast::execute( vector< string > & argv,
                                               string & out )
{
    if( caller->current_vm == NULL)
        out += "not attached to a VM\n";
    else
        caller->current_vm->remove_last( out );
    
    if( argv.size() > 0 )
        out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Replace::execute( vector< string > & argv,
                                            string & out )
{
    if( caller->current_vm == NULL)
        out += "not attached to a VM\n";
    else
        caller->current_vm->replace_shred( argv, out );
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Kill::execute( vector< string > & argv,
                                         string & out )
{
    caller->current_vm->kill( out );
    if( argv.size() > 0 )
        out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Close::execute( vector< string > & argv,
                                          string & out )
{
    caller->close();
    out += "closing chuck shell.  Bye!\n";
    if( argv.size() > 0 )
        out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Exit::execute( vector< string > & argv,
                                         string & out )
{
    caller->kill();
    if( argv.size() > 0 )
        out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Ls::execute( vector< string > & argv,
                                       string & out )
{
#ifndef __PLATFORM_WIN32__
    
    if( argv.size() == 0 )
        {
        DIR * dir_handle = opendir( "." );
        dirent * dir_entity = readdir( dir_handle );
        
        while( dir_entity != NULL )
        {
            out += string( dir_entity->d_name ) + "\n";
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
            out += string( dir_entity->d_name ) + "\n";
            dir_entity = readdir( dir_handle );
        }
        
        if( print_parent_name )
            out += "\n";
        
        closedir( dir_handle );
    }
    
#else
    out += "command not yet supported on Win32!\n";
#endif // __PLATFORM_WIN32__
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Cd::execute( vector< string > & argv,
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
#endif //__PLATFORM_WIN32__
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Pwd::execute( vector< string > & argv,
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
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Alias::execute( vector< string > & argv,
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
            if( caller->aliases.find( argv[i] ) == caller->aliases.end() )
                out += "alias " + argv[i] + " not found\n";
            else
                out += "alias " + argv[i] + "='" + 
                       caller->aliases[argv[i]] + "'\n";
        }
        // create the alias
        else
        {
            a = string( argv[i], 0, j );
            b = string( argv[i], j + 1, string::npos );
            caller->aliases[a] = b;
        }
    }
    
    if( len == 0 )
    // no arguments specified; print the entire alias map
    {   
        map< string, string>::iterator i = caller->aliases.begin(), 
                                       end = caller->aliases.end();
        for( ; i != end; i++ )
            out += "alias " + i->first + "='" + i->second + "'\n";
    }
    

}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Unalias::execute( vector< string > & argv,
                                            string & out )
{
    int i, len = argv.size();
    
    for( i = 0; i < len; i++ )
    {
        if( caller->aliases.find( argv[i] ) == caller->aliases.end() )
        {
            out += "alias " + argv[i] + " not found\n";
        }
        
        else
            caller->aliases.erase( argv[i] );
    }
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Source::execute( vector< string > & argv,
                                           string & out )
{
    int i, len = argv.size();
    char line_buf[255];
    string line, temp;

    for( i = 0; i < len; i++ )
    {
        FILE * source_file = fopen( argv[i].c_str(), "r" );
        while( fgets( line_buf, 255, source_file ) != NULL )
        {
            line = string( line_buf );
            caller->execute( line, temp );
            out += temp;
        }
    }
}

//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell::Command_VM::init( Chuck_Shell * caller )
{
    Command * temp;
    
    Command::init( caller );
    
    temp = new Command_VMAttach();
    temp->init( caller );
    commands["attach"] = temp;
    commands["@"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_VMAdd();
    temp->init( caller );
    commands["add"] = temp;
    commands["+"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_VMRemove();
    temp->init( caller );
    commands["remove"] = temp;
    commands["-"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_VMList();
    temp->init( caller );
    commands["list"] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_VMSwap();
    temp->init( caller );
    commands["swap"] = temp;
    commands["="] = temp;
    allocated_commands.push_back( temp );
    
    temp = new Command_VMAttachAdd();
    temp->init( caller );
    commands["@+"] = temp;  
    allocated_commands.push_back( temp );
    
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: ~Command_VM()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell::Command_VM::~Command_VM()
{
	int i, len = allocated_commands.size();
	 
    //iterate through commands, delete the associated heap data
    for( i = 0; i != len; i++ )
	    SAFE_DELETE( allocated_commands[i] );
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_VM::execute( vector< string > & argv,
                                       string & out )
{
    if( commands.find( argv[0] ) == commands.end() )
    // command doesn't exist
        out += "vm " + argv[0] + ": command not found\n";
    else
    // call the mapped command
    {
        Command * command = commands[argv[0]];
        argv.erase( argv.begin() );
        command->execute( argv, out );
    }
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_VMAttach::execute( vector< string > & argv,
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
            port = strtol( string( argv[0].c_str(), i + 1 ).c_str(), NULL, 10 );
            if( port == 0 /* && errno == EINVAL */ )
                out += "invalid port '" + string( argv[0].c_str(), i + 1 ) + "'\n";
            else
                hostname = string( argv[0], 0, i );
        }
    }
        
    if( port != 0 )
    {
        Chuck_Shell_Network_VM * new_vm = new Chuck_Shell_Network_VM();
        new_vm->init( hostname, port );
        //if( !new_vm->test() )
        //  {
            
        //  }
        if( !caller->save_current_vm )
            SAFE_DELETE( caller->current_vm );
        caller->current_vm = new_vm;
        caller->save_current_vm = FALSE;
        out += argv[0] + " is now current VM\n";
    }
    else 
        out += "unable to attach to " + argv[0] + "\n";
    
    if( argv.size() > 1 )
        out += "ignoring excess arguments...";
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_VMAdd::execute( vector< string > & argv,
                                          string & out )
{
    char buf[16];
    
    caller->vms.push_back( caller->current_vm );
    caller->save_current_vm = TRUE;
    
#ifndef __PLATFORM_WIN32__
    snprintf( buf, 16, "%u", caller->vms.size() - 1 );
#else
    sprintf( buf, "%u", caller->vms.size() - 1 );   
#endif // __PLATFORM_WIN32__

    out += caller->current_vm->fullname() + " saved as VM " + buf + "\n";
    
    if( argv.size() > 0 )
        out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
// warning: this function will do a multiple free if the vm numbers supplied
// are all associated with the same VM
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_VMRemove::execute( vector< string > & argv,
                                             string & out )
{
    int i = 0, vm_no, len = argv.size();
    
    for( ; i < len; i++ )
    {
        vm_no = strtoul( argv[i].c_str(), NULL, 10 );
        if( vm_no == 0 && errno == EINVAL || caller->vms.size() <= vm_no || 
            caller->vms[vm_no] == NULL )
            out += "invalid VM id: " + argv[i] + "\n";
        else
        {
            SAFE_DELETE( caller->vms[vm_no] );
            caller->vms[vm_no] = NULL;
        }
    }
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_VMSwap::execute( vector< string > & argv,
                                           string & out )
{
    int new_vm;
    
    if( argv.size() < 1 )
    {
        out += string( "too few arguments...\n" );
        return;
    }
    
    new_vm = strtol( argv[0].c_str(), NULL, 10 );
    if( new_vm >= caller->vms.size() || new_vm < 0 || 
        caller->vms[new_vm] == NULL )
    {
        out += string( "invalid VM: " ) + argv[0];
        return;
    }
    
    if( !caller->save_current_vm )
        SAFE_DELETE( caller->current_vm );
    caller->save_current_vm = TRUE;
    caller->current_vm = caller->vms[new_vm];
    out += "current VM is now " + caller->current_vm->fullname() + "\n";
    
    if( argv.size() > 1 )
        out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_VMList::execute( vector< string > & argv,
                                           string & out )
{
    char buf[16];
    int i, len = caller->vms.size();
    
    out += string("current VM: ") + caller->current_vm->fullname() + "\n";
    
    for( i = 0; i < len; i++ )
    {
#ifndef __PLATFORM_WIN32__
        snprintf( buf, 16, "%u", i );
#else
        sprintf( buf, "%u", i );
#endif // __PLATFORM_WIN32__
        out += string( "VM " ) + buf + ": " + 
               caller->vms[i]->fullname() + "\n";
    }
    
    if( argv.size() > 0 )
        out += "ignoring excess arguments...\n";
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_VMAttachAdd::execute( vector< string > & argv,
                                                string & out )
{
//  caller->commands["vm"]->execute( argv, out );
//  vm_add( vector< string >(), out );
}

//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Shell::Command_Code::init( Chuck_Shell * caller )
{
    Command * temp;
    
    Command::init( caller );
    
    return TRUE;
}

//-----------------------------------------------------------------------------
// name: ~Command_Code()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Shell::Command_Code::~Command_Code()
{
	int i, len = allocated_commands.size();
	
    //iterate through commands, delete the associated heap data
    for( i = 0; i != len; i++ )
	    SAFE_DELETE( allocated_commands[i] );
}

//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Shell::Command_Code::execute( vector< string > & argv,
                                         string & out )
{
    if( commands.find( argv[0] ) == commands.end() )
    // command doesn't exist
        out += "code " + argv[0] + ": command not found\n";
    else
    // call the mapped command
    {
        Command * command = commands[argv[0]];
        argv.erase( argv.begin() );
        command->execute( argv, out );
    }
}

