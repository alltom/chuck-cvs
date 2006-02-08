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
// file: chuck_globals.cpp
// desc: global variables slum
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: spring 2006
//-----------------------------------------------------------------------------
#include "chuck_globals.h"
#include "chuck_bbq.h"
#include "ugen_stk.h"
#include "ulib_std.h"
#include <stdlib.h>
#include <stdio.h>


// current version
const char CK_VERSION[] = "1.2.0.5-rc2 (dracula)";

// global virtual machine
Chuck_VM * g_vm = NULL;
// global compiler
Chuck_Compiler * g_compiler = NULL;
// the shell
Chuck_Shell * g_shell = NULL;
// global variables
t_CKUINT g_sigpipe_mode = 0;
// default socket
ck_socket g_sock = NULL;
// default port
t_CKINT g_port = 8888;


//-----------------------------------------------------------------------------
// name: all_detach()
// desc: called during cleanup to close all open file handles
//-----------------------------------------------------------------------------
extern "C" void all_detach()
{
    // close stk file handles
    stk_detach( 0, NULL );
    // close midi file handles
    midirw_detach();
    // shutdown kb loop
    KBHitManager::shutdown();
}


//-----------------------------------------------------------------------------
// name: signal_pipe()
// desc: ...
//-----------------------------------------------------------------------------
extern "C" void signal_pipe( int sig_num )
{
    fprintf( stderr, "[chuck]: sigpipe handled - broken pipe (no connection)...\n" );
    if( g_sigpipe_mode )
    {
        all_detach();
        // exit( 2 );
    }
}