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
#include "util_thread.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>


// global
extern Chuck_VM * g_vm;
extern Chuck_Compiler * g_compiler;
extern char g_host[256];
extern int g_port;
extern ck_socket g_sock;
extern t_CKUINT g_sigpipe_mode;
extern "C" void signal_pipe( int );


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
// name: otf_process_msg()
// desc: ...
//-----------------------------------------------------------------------------
t_CKUINT otf_process_msg( Chuck_VM * vm, Chuck_Compiler * compiler, 
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




//-----------------------------------------------------------------------------
// name: otf_send_file()
// desc: ...
//-----------------------------------------------------------------------------
int otf_send_file( const char * filename, Net_Msg & msg, const char * op )
{
    FILE * fd = NULL;
    struct stat fs;
    
    strcpy( msg.buffer, "" );
    //if( filename[0] != '/' )
    //{ 
    //    strcpy( msg.buffer, getenv("PWD") ? getenv("PWD") : "" );
    //    strcat( msg.buffer, getenv("PWD") ? "/" : "" );
    //}
    strcat( msg.buffer, filename );

    // test it
    fd = open_cat_ck( (char *)msg.buffer );
    if( !fd )
    {
        fprintf( stderr, "[chuck]: cannot open file '%s' for [%s]...\n", filename, op );
        return FALSE;
    }

    if( !chuck_parse( (char *)msg.buffer, fd ) )
    {
        fprintf( stderr, "[chuck]: skipping file '%s' for [%s]...\n", filename, op );
        fclose( fd );
        return FALSE;
    }
            
    // stat it
    stat( msg.buffer, &fs );
    fseek( fd, 0, SEEK_SET );

    //fprintf(stderr, "sending TCP file %s\n", msg.buffer );
    // send the first packet
    msg.param2 = (t_CKUINT)fs.st_size;
    msg.length = 0;
    otf_hton( &msg );
    ck_send( g_sock, (char *)&msg, sizeof(msg) );

    // send the whole thing
    t_CKUINT left = (t_CKUINT)fs.st_size;
    while( left )
    {
        //fprintf(stderr,"file %03d bytes left ... ", left);
        // amount to send
        msg.length = left > NET_BUFFER_SIZE ? NET_BUFFER_SIZE : left;
        // read
        msg.param3 = fread( msg.buffer, sizeof(char), msg.length, fd );
        // amount left
        left -= msg.param3 ? msg.param3 : 0;
        msg.param2 = left;
        //fprintf(stderr, "sending fread %03d length %03d...\n", msg.param3, msg.length );
        // send it
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    
    // close
    fclose( fd );
    //fprintf(stderr, "done.\n", msg.buffer );
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: otf_send_connect()
// desc: ...
//-----------------------------------------------------------------------------
int otf_send_connect()
{
    g_sock = ck_tcp_create( 0 );
    if( !g_sock )
    {
        fprintf( stderr, "[chuck]: cannot open socket to send command...\n" );
        return FALSE;
    }

    if( strcmp( g_host, "127.0.0.1" ) )
        fprintf( stderr, "[chuck]: connecting to %s on port %i via TCP...\n", g_host, g_port );
    
    if( !ck_connect( g_sock, g_host, g_port ) )
    {
        fprintf( stderr, "[chuck]: cannot open TCP socket on %s:%i...\n", g_host, g_port );
        return FALSE;
    }
    
    ck_send_timeout( g_sock, 0, 2000000 );

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: otf_send_cmd()
// desc: ...
//-----------------------------------------------------------------------------
int otf_send_cmd( int argc, char ** argv, t_CKINT & i )
{
    Net_Msg msg;
    g_sigpipe_mode = 1;
    int tasks_total = 0, tasks_done = 0;
    
    if( !strcmp( argv[i], "--add" ) || !strcmp( argv[i], "+" ) )
    {
        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [add]...\n" );
            goto error;
        }

        if( !otf_send_connect() ) return 0;
        do {
            msg.type = MSG_ADD;
            msg.param = 1;
            tasks_done += otf_send_file( argv[i], msg, "add" );
            tasks_total++;
        } while( ++i < argc );
        
        if( !tasks_done )
            goto error;
    }
    else if( !strcmp( argv[i], "--remove" ) || !strcmp( argv[i], "-" ) )
    {
        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [remove]...\n" );
            goto error;
        }

        if( !otf_send_connect() ) return 0;
        do {
            msg.param = atoi( argv[i] );
            msg.type = MSG_REMOVE;
            otf_hton( &msg );
            ck_send( g_sock, (char *)&msg, sizeof(msg) );
        } while( ++i < argc );
    }
    else if( !strcmp( argv[i], "--" ) )
    {
        if( !otf_send_connect() ) return 0;
        msg.param = 0xffffffff;
        msg.type = MSG_REMOVE;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--replace" ) || !strcmp( argv[i], "=" ) )
    {
        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [replace]...\n" );
            goto error;
        }

        if( i <= 0 )
            msg.param = 0xffffffff;
        else
            msg.param = atoi( argv[i] );

        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [replace]...\n" );
            goto error;
        }

        if( !otf_send_connect() ) return 0;
        msg.type = MSG_REPLACE;
        if( !otf_send_file( argv[i], msg, "replace" ) )
            goto error;
    }
    else if( !strcmp( argv[i], "--removeall" ) || !strcmp( argv[i], "--remall" ) )
    {
        if( !otf_send_connect() ) return 0;
        msg.type = MSG_REMOVEALL;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--kill" ) )
    {
        if( !otf_send_connect() ) return 0;
        msg.type = MSG_REMOVEALL;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
        msg.type = MSG_KILL;
        msg.param = (i+1)<argc ? atoi(argv[++i]) : 0;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--time" ) )
    {
        if( !otf_send_connect() ) return 0;
        msg.type = MSG_TIME;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--status" ) || !strcmp( argv[i], "^" ) )
    {
        if( !otf_send_connect() ) return 0;
        msg.type = MSG_STATUS;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
    }
    else
        return 0;
        
    // send
    msg.type = MSG_DONE;
    otf_hton( &msg );
    ck_send( g_sock, (char *)&msg, sizeof(msg) );

    // set timeout
    ck_recv_timeout( g_sock, 0, 2000000 );
    // reply
    if( ck_recv( g_sock, (char *)&msg, sizeof(msg) ) )
    {
        otf_ntoh( &msg );
        fprintf( stderr, "[chuck(remote)]: operation %s\n", ( msg.param ? "successful" : "failed (sorry)" ) );
        if( !msg.param )
            fprintf( stderr, "(reason): %s\n", 
                ( strstr( (char *)msg.buffer, ":" ) ? strstr( (char *)msg.buffer, ":" ) + 1 : (char *)msg.buffer ) ) ;
    }
    else
    {
        fprintf( stderr, "[chuck]: remote operation timed out...\n" );
    }
    // close the sock
    ck_close( g_sock );
    
    // exit
    exit( msg.param );

    return 1;
    
error:
    
    // if sock was opened
    if( g_sock )
    {
        msg.type = MSG_DONE;
        otf_hton( &msg );
        ck_send( g_sock, (char *)&msg, sizeof(msg) );
        ck_close( g_sock );
    }
    
    exit( 1 );

    return 0;
}




//-----------------------------------------------------------------------------
// name: otf_cb()
// desc: ...
//-----------------------------------------------------------------------------
void * otf_cb( void * p )
{
    Net_Msg msg;
    Net_Msg ret;
    ck_socket client;
    int n;

#ifndef __PLATFORM_WIN32__
    // catch SIGPIPE
    signal( SIGPIPE, signal_pipe );
#endif

    while( true )
    {
        client = ck_accept( g_sock );
        if( !client )
        {
            if( g_vm ) fprintf( stderr, "[chuck]: socket error during accept()...\n" );
            usleep( 40000 );
            ck_close( client );
            continue;
        }
        msg.clear();
        // set time out
        ck_recv_timeout( client, 0, 5000000 );
        n = ck_recv( client, (char *)&msg, sizeof(msg) );
        otf_ntoh( &msg );
        if( n != sizeof(msg) )
        {
            fprintf( stderr, "[chuck]: 0-length packet...\n", (int)client );
            usleep( 40000 );
            ck_close( client );
            continue;
        }
        
        if( msg.header != NET_HEADER )
        {
            fprintf( stderr, "[chuck]: header mismatch - possible endian lunacy...\n" );
            ck_close( client );
            continue;
        }

        while( msg.type != MSG_DONE )
        {
            if( g_vm )
            {
                if( !otf_process_msg( g_vm, g_compiler, &msg, FALSE, client ) )
                {
                    ret.param = FALSE;
                    strcpy( (char *)ret.buffer, EM_lasterror() );
                    while( msg.type != MSG_DONE && n )
                    {
                        n = ck_recv( client, (char *)&msg, sizeof(msg) );
                        otf_ntoh( &msg );
                    }
                    break;
                }
                else
                {
                    ret.param = TRUE;
                    strcpy( (char *)ret.buffer, "success" );
                    n = ck_recv( client, (char *)&msg, sizeof(msg) );
                    otf_ntoh( &msg );
                }
            }
        }
        
        otf_hton( &ret );
        ck_send( client, (char *)&ret, sizeof(ret) );
        ck_close( client );
    }
    
    return NULL;
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
