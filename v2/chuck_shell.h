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
// file: chuck_shell.h
// desc: ...
//
// author: Spencer Salazar (ssalazar@princeton.edu)
// date: Autumn 2005
//-----------------------------------------------------------------------------
#ifndef __CHUCK_SHELL_H__
#define __CHUCK_SHELL_H__

#include "chuck_def.h"
#include "chuck_vm.h"
#include "chuck_compile.h"

#include <map>
#include <string>

using namespace std;

// forward references
class Chuck_Shell_VM;
class Chuck_Shell_Mode;
class Chuck_Shell_UI;

typedef string Chuck_Shell_Request;
typedef string Chuck_Shell_Response;

//-----------------------------------------------------------------------------
// name: class Chuck_Shell
// desc: controller class for facilitating interaction between a shell UI and a 
//      shell mode.
//-----------------------------------------------------------------------------
class Chuck_Shell
{
friend class Chuck_Shell_Mode;
public:
    Chuck_Shell();
    ~Chuck_Shell();
    
    t_CKBOOL init( Chuck_VM *, Chuck_Compiler *, Chuck_Shell_UI * );
    void run();
    void close();
    void kill();

protected: /* these functions are usually called by a Chuck_Shell_Mode */
    //t_CKBOOL swap_mode( const string & );
    //t_CKBOOL swap_last_mode();
    //t_CKBOOL set_prompt( const string & );
	//t_CKBOOL meta_command( t_CKUINT, const string & );
	

private:
    map< string, Chuck_Shell_Mode * > modes;
    Chuck_Shell_Mode * current_mode;
    Chuck_Shell_UI * ui;
    vector< Chuck_Shell_VM * > vms;
    Chuck_VM * process_vm;
    t_CKBOOL initialized;
    t_CKBOOL stop;
};


//-----------------------------------------------------------------------------
// name: class Chuck_Shell_VM
// desc: encapsulates local and network VMs into a single class
//-----------------------------------------------------------------------------
class Chuck_Shell_VM
{
public:
	virtual t_CKBOOL add_shred( const vector< string > &, string & )=0;
	virtual t_CKBOOL remove_shred( const vector< string > &, string & )=0;
	virtual t_CKBOOL remove_all( string & )=0;
	virtual t_CKBOOL remove_last( string & )=0;
	virtual t_CKBOOL replace_shred( const vector< string > &, string & )=0;
	virtual t_CKBOOL status( string & )=0;
	virtual t_CKBOOL kill( string & )=0;
	virtual string fullname()=0;
};

//-----------------------------------------------------------------------------
// name: class Chuck_Shell_Network_VM
// desc: VM across the network
//-----------------------------------------------------------------------------
class Chuck_Shell_Network_VM : public Chuck_Shell_VM
{
public:
	t_CKBOOL init( const string &, t_CKINT );
	t_CKBOOL add_shred( const vector< string > &, string & );
	t_CKBOOL remove_shred( const vector< string > &, string & );
	t_CKBOOL remove_all( string & );
	t_CKBOOL remove_last( string & );
	t_CKBOOL replace_shred( const vector< string > &, string & );
	t_CKBOOL status( string & );
	t_CKBOOL kill( string & );
	string fullname();

private: 
	string hostname;
	t_CKINT port;
};
/* 
//-----------------------------------------------------------------------------
// name: class Chuck_Shell_Process_VM
// desc: VM in process
//-----------------------------------------------------------------------------
class Chuck_Shell_Process_VM : public Chuck_Shell_VM
{
public:
	t_CKBOOL init( Chuck_VM *, Chuck_Compiler * );
	t_CKBOOL add_shred( const vector< string > &, string & );
	t_CKBOOL remove_shred( const vector< string > &, string & );
	t_CKBOOL remove_all( const vector< string > &, string & );
	t_CKBOOL remove_last( const vector< string > &, string & );
	t_CKBOOL replace_shred( const vector< string > &, string & );
	t_CKBOOL status( string & );
	void stop();
	string fullname();

private:
	Chuck_VM * vm;
	Chuck_Compiler * compiler;
};
*/
//-----------------------------------------------------------------------------
// name: class Chuck_Shell_Mode
// desc: superclass to various types of Chuck shell modes
//-----------------------------------------------------------------------------
class Chuck_Shell_Mode
{
public:
    Chuck_Shell_Mode();
    ~Chuck_Shell_Mode();
    
    virtual t_CKBOOL init( vector< Chuck_Shell_VM * > *, 
    					   Chuck_Shell * );
    virtual t_CKBOOL execute( const Chuck_Shell_Request &,
    						  Chuck_Shell_Response & ) = 0;
    
protected:
    vector< Chuck_Shell_VM * > * vms;  //shared by all modes
    t_CKBOOL save_current_vm;  //if true, do not delete the current vm 
   	Chuck_Shell_VM * current_vm;
    Chuck_Shell * host_shell;
    t_CKBOOL initialized;
};

//-----------------------------------------------------------------------------
// name: class Chuck_Shell_UI
// desc: superclass to various types of Chuck UIs
//-----------------------------------------------------------------------------
class Chuck_Shell_UI
{
public:
    Chuck_Shell_UI();
    ~Chuck_Shell_UI();
    
    virtual t_CKBOOL init();
    virtual t_CKBOOL next_command( Chuck_Shell_Request & ) = 0;
    virtual void next_result( const Chuck_Shell_Response & ) = 0;
};


//-----------------------------------------------------------------------------
// name: class Chuck_Shell_Mode_Command
// desc: command mode for chuck shell
//-----------------------------------------------------------------------------
class Chuck_Shell_Mode_Command : public Chuck_Shell_Mode
{
public:
	Chuck_Shell_Mode_Command();
	~Chuck_Shell_Mode_Command();

    t_CKBOOL execute( const Chuck_Shell_Request &, Chuck_Shell_Response & );

protected:
	void add( const vector< string > &, string & );
	void remove( const vector< string > &, string & );
	void removeall( const vector< string > &, string & );
	void removelast( const vector< string > &, string & );
	void replace( const vector< string > &, string & );
	void kill( const vector< string > &, string & );
	void close( const vector< string > &, string & );
	void exit( const vector< string > &, string & );
	void ls( const vector< string > &, string & );
	void cd( const vector< string > &, string & );
	void pwd( const vector< string > &, string & );
	void alias( const vector< string > &, string & );
	void unalias( const vector< string > &, string & );
	void source( const vector< string > &, string & );
	void vm_attach( const vector< string > &, string & );
	void vm_add( const vector< string > &, string & );
	void vm_remove( const vector< string > &, string & );
	void vm_list( const vector< string > &, string & );
	void vm_attach_add( const vector< string > &, string & );
	void vm_swap( const vector< string > &, string & );

private:
	map < string, string > aliases;
	map < string, string > variables;

};

// prototype for shell thread routine
void * shell_cb( void * p );

#endif //__CHUCK_SHELL_H__
