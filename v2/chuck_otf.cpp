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
// file: chuck_otf.cpp
// desc: on-the-fly programming utilities
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2004
//-----------------------------------------------------------------------------
#include "chuck_otf.h"
#include "chuck_compile.h"
#include "chuck_errmsg.h"
#include "util_network.h"




//-----------------------------------------------------------------------------
// name: otf_hton( )
// desc: ...
//-----------------------------------------------------------------------------
void otf_hton( Net_Msg * msg )
{
    msg->header = htonl( msg->header );
    msg->type = htonl( msg->type );
    msg->param = htonl( msg->param );
    msg->param2 = htonl( msg->param2 );
    msg->param3 = htonl( msg->param3 );
    msg->length = htonl( msg->length );
}




//-----------------------------------------------------------------------------
// name: otf_ntoh( )
// desc: ...
//-----------------------------------------------------------------------------
void otf_ntoh( Net_Msg * msg )
{
    msg->header = ntohl( msg->header );
    msg->type = ntohl( msg->type );
    msg->param = ntohl( msg->param );
    msg->param2 = ntohl( msg->param2 );
    msg->param3 = ntohl( msg->param3 );
    msg->length = ntohl( msg->length );
}




//-----------------------------------------------------------------------------
// name: recv_file()
// desc: ...
//-----------------------------------------------------------------------------
FILE * recv_file( const Net_Msg & msg, ck_socket sock )
{
    Net_Msg buf;
    
    // what is left
    // t_CKUINT left = msg.param2;
    // make a temp file
    FILE * fd = tmpfile();

    do {
        // msg
        if( !ck_recv( sock, (char *)&buf, sizeof(buf) ) )
            goto error;
        otf_ntoh( &buf );
        // write
        fwrite( buf.buffer, sizeof(char), buf.length, fd );
    }while( buf.param2 );
    
    return fd;

error:
    fclose( fd );
    fd = NULL;
    return NULL;
}




//-----------------------------------------------------------------------------
// name: process_msg()
// desc: ...
//-----------------------------------------------------------------------------
t_CKUINT process_msg( Chuck_VM * vm, Chuck_Compiler * compiler, 
                      Net_Msg * msg, t_CKBOOL immediate, void * data )
{
    Chuck_Msg * cmd = new Chuck_Msg;
    Chuck_VM_Code * code = NULL;
    FILE * fd = NULL;
    
    // fprintf( stderr, "UDP message recv...\n" );
    if( msg->type == MSG_REPLACE || msg->type == MSG_ADD )
    {
        // see if entire file is on the way
        if( msg->param2 )
        {
            fd = recv_file( *msg, (ck_socket)data );
            if( !fd )
            {
                fprintf( stderr, "[chuck]: incoming source transfer '%s' failed...\n",
                    mini(msg->buffer) );
                return 0;
            }
        }

        // parse, type-check, and emit
        if( !compiler->go( msg->buffer, fd ) )
            return 0;

        // get the code
        code = compiler->output();
        // name it
        code->name += string(msg->buffer);

        // set the flags for the command
        cmd->type = msg->type;
        cmd->code = code;
        if( msg->type == MSG_REPLACE )
            cmd->param = msg->param;
    }
    else if( msg->type == MSG_STATUS || msg->type == MSG_REMOVE || msg->type == MSG_REMOVEALL
             || msg->type == MSG_KILL || msg->type == MSG_TIME )
    {
        cmd->type = msg->type;
        cmd->param = msg->param;
    }
    else
    {
        fprintf( stderr, "[chuck]: unrecognized incoming command from network: '%i'\n", cmd->type );
        SAFE_DELETE(cmd);
        return 0;
    }
    
    // immediate
    if( immediate )
        return vm->process_msg( cmd );

    vm->queue_msg( cmd, 1 );

    return 1;
}




const char * poop[] = {
    "[chuck]: bus error...",
    "[chuck]: segmentation fault, core dumped...",
    "[chuck]: access violation, NULL pointer...",
    "[chuck]: Unhandled exception in chuck: 0xC0000005: Access violation",
    "[chuck]: malloc: damage after normal block...",
    "[chuck]: virtual machine panic!!!",
    "[chuck]: confused by earlier errors, bailing out...",
    "[chuck]: lack of destructors have led to the unrecoverable mass build-up of trash\n"
    "         the chuck garbage collector will now run, deleting all files (bye.)",
    "[chuck]: calling machine.crash()...",
    "[chuck]: an unknown fatal error has occured.  please restart your computer...",
    "[chuck]: an unknown fatal error has occured.  please reinstall your OS...",
    "[chuck]: an unknown fatal error has occured.  please update to chuck-3.0",
    "[chuck]: internal error: unknown error",
    "[chuck]: page fault!!!",
    "[chuck]: error printing error message.  cannot continue 2#%%HGAFf9a0x"
};

long poop_size = sizeof( poop ) / sizeof( char * );
