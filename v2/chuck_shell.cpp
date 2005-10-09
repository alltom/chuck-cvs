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


// escape char
static const char CHUCK_SHELL_ESCAPE_CHAR = '#';

// global shell pointer (lives in chuck_main)
extern Chuck_Shell * g_shell;

// global network socket
extern ck_socket g_sock;

// SIGPIPE mode
extern t_CKUINT g_sigpipe_mode;

// global OTF host
extern char g_host[256];

// global OTF port
extern int g_port;




//-----------------------------------------------------------------------------
// name: divide_string
// desc: divides string into substrings, each separated by an arbitrarily long 
//       sequence of any characters in edge.  
//-----------------------------------------------------------------------------
void divide_string( const std::string & stra, const std::string & edge, 
                    std::vector< std::string > & substrings)
{
    std::string str = stra + " ";
    int i = str.find_first_not_of( edge ), j = 0;
    for(;;)
    {
        j = str.find_first_of( edge, i );
        if( j == std::string::npos )
            break;
        substrings.push_back( std::string( str, i, j - i ) );
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
    
    std::vector< Chuck_Shell_Network_VM * > * nvmvec = 
        new std::vector< Chuck_Shell_Network_VM * > ();
    std::vector< Chuck_Shell_Shred * > * shrvec = 
        new std::vector< Chuck_Shell_Shred * > ();
    
    // make new mode
    current_mode = new Chuck_Shell_Mode_Command();
    // initialize it
    if( !current_mode->init( vm, compiler, nvmvec, shrvec, this ) )
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

    Chuck_Shell_Request command;
    Chuck_Shell_Response result;

    // loop
    for(;;)
    {
        // get command
        if( ui->next_command( command ) == TRUE )
        {
            if( command[0] == CHUCK_SHELL_ESCAPE_CHAR )
            {
                // interpret the shell meta-command
                result = "(meta-command)\n";
            }
            else
            {
                // result.clear();
                result = "";

                // execute the command
                current_mode->execute(command,result);

                // command.clear();
                command = "";
            }

            // pass the result to the shell ui
            ui->next_result( result );
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
t_CKBOOL Chuck_Shell_Mode::init( Chuck_VM * vm, 
                                 Chuck_Compiler * compiler, 
                                 std::vector< Chuck_Shell_Network_VM * > * nvms, 
                                 std::vector< Chuck_Shell_Shred * > * shreds,
                                 Chuck_Shell * host_shell ) 
{
    //TODO: input validation
    this->vm = vm;
    this->compiler = compiler;
    this->host_shell = host_shell;
    this->vms = nvms;
    this->shreds = shreds;
    initialized = TRUE;
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: add_VM
// desc: ...
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// name: remove_VM
// desc: ...
//-----------------------------------------------------------------------------


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
    std::vector< std::string > vec;
    
    out = "";

    //divide the string into white space separated substrings
    divide_string( in, " \t\n\v",  vec );
    if( vec.size() == 0) 
    {
        out = "";
        return TRUE;
    }
    
    //first find out what the command is
    if( vec[0] == "add" || vec[0] == "+" )
    {
        Chuck_VM_Code * code = NULL;
        Chuck_Shell_Shred * shred = new Chuck_Shell_Shred;
        std::vector< t_CKUINT > only_these_vms;
        char buf[10];
        int i;
                
        //first gather command line options
        for( i = 1; i < vec.size(); i++ )
        {
            if( vec[i][0] == '-' )
            {
                if( vec[i] == "-v" )
                {
            
                }
            }
            else
                break;
            }
        
        for(; i < vec.size(); i++ )
        {
            //first need to stat the file and make sure it exists/is readable
            
            // compile the file for local usage
            if( !compiler->go( vec[i], NULL ) )
            {
                out += "*** error: cannot compile '"+vec[i]+"' ***\n";
                continue;
            }
            
            //add to local VM
            code = compiler->output();
            code->name += vec[i];
            shred->shred = vm->spork( code, NULL );
            shred->vms.push_back( 0 );
            
            //add to network VMs
            Net_Msg msg;
            g_sigpipe_mode = 1;
            for( int j = 0; j < vms->size(); j++ )
            {
                if( (*vms)[j] == NULL )
                    continue;
                // snprintf( g_host, 255, (*vms)[j]->hostname.c_str() );
                sprintf( g_host, (*vms)[j]->hostname.c_str() );
                g_port = (*vms)[j]->port;
                msg.type=MSG_ADD;
                msg.param = 1;
                if( !otf_send_connect() )
                {
                    // snprintf( buf, 9, "%u", (*vms)[j]->port );
                    sprintf( buf, "%u", (*vms)[j]->port );
                    out += "unable to connect to " + 
                        (*vms)[j]->hostname + ":" + buf + "\n";
                    //snprintf( buf, 9, "%u", j );
                    sprintf( buf, "%u", j );
                    out += std::string("*** add failed for VM ") + buf + 
                            "***\n";
                    continue;
                }
                otf_send_file( vec[i].c_str(), msg, "add" );
                
                // end transaction
                msg.type = MSG_DONE;
                otf_hton( &msg );
                ck_send( g_sock, (char *)&msg, sizeof(msg) );
            
                // set timeout
                ck_recv_timeout( g_sock, 0, 2000000 );
                // get reply from server
                if( ck_recv( g_sock, (char *)&msg, sizeof(msg) ) )
                {
                    otf_ntoh( &msg );
                    if( !msg.param )
                        // error from server
                    {
                        out += std::string((char *)msg.buffer) + "\n";
                        // snprintf( buf, 9, "%u", j );
                        sprintf( buf, "%u", j );
                        out += std::string("*** add failed for VM ") + buf + 
                            "***\n";
                        continue;
                    }
                }
                else //timeout
                {
                    // snprintf( buf, 9, "%u", (*vms)[j]->port );
                    sprintf( buf, "%u", j );
                    out += "remote operation timed out for " + 
                        (*vms)[j]->hostname + ":" + buf + "\n";
                    // snprintf( buf, 9, "%u", j );
                    sprintf( buf, "%u", j );
                    out += std::string("*** add failed for VM ") + buf + 
                            "***\n";
                    continue;
                }
                
                shred->vms.push_back( j );
                
                // close the sock
                ck_close( g_sock );
            
            }
            
            // add to shred list
            shred->name = vec[i];
            shreds->push_back( shred );
            
            // print success message
            // snprintf(buf,9,"%u",shreds->size()-1);
            sprintf( buf, "%u", shreds->size()-1 );
            out += std::string( "shred " ) + buf + ": '"+vec[i]+"'\n";
        }
    }

    else if( vec[0] == "attach" )
    {
        //in theory this should send some sort of non-state changing command
        //to the VM in question to ensure that it exists
        
        char buf[10];
        
        if( vec.size() < 2 )
            vec.push_back("localhost:8888");

        for( int i = 1; i < vec.size(); i++ )
        {
            Chuck_Shell_Network_VM * net_vm = new Chuck_Shell_Network_VM;
            if( vec[i] == "local" || vec[i] == "default" )
                vec[i] = "localhost:8888";
            int j = vec[i].find( ':' );
            if( j == std::string::npos )
            {
                net_vm->hostname = vec[i];
                net_vm->port = 8888;
            }
            else
            {
                net_vm->hostname = std::string( vec[i], 0, j );
                std::string port = std::string( vec[i], j+1, vec[i].size());
                net_vm->port = strtol( port.c_str(), NULL, 10 );
                if( net_vm->port == 0 )
                {
                    out += "*** error: invalid port "+port+" ***\n";
                    continue;
                }
            }
            
            vms->push_back( net_vm );
            // snprintf( buf, 9, "%u", vms->size());
            sprintf( buf, "%u", vms->size() );
            out += std::string("VM ") + buf + ": ";
            // snprintf( buf, 9, "%u", net_vm->port );
            sprintf( buf, "%u", vms->size() );
            out += "attached to " + net_vm->hostname + " on port " + buf + "\n";
        }
    }
        
    else if( vec[0] == "detach" )
    {
        long vm_num = 0;
        for( int i = 1; i < vec.size(); i++ )
        {
            vm_num = strtol( vec[i].c_str(), NULL, 10 );
            if( ( vm_num == 0 && errno == EINVAL ) || vm_num > vms->size() 
                || (*vms)[vm_num] == NULL )
            {
                out += "invalid VM reference: " + vec[i] + "\n";
                continue;
            }
            vm_num +=1;
            SAFE_DELETE( (*vms)[vm_num] );
            out += "VM " + vec[i] + " detached\n";
        }
    }
        
    else
        out = "shell: unknown command '"+vec[0]+"'\n";
    
    return TRUE;
}
