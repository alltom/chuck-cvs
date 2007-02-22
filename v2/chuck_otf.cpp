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
#include "chuck_globals.h"
#include "util_thread.h"
#include "util_string.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#ifndef __PLATFORM_WIN32__
#include <unistd.h>
#endif

using namespace std;

// extern "C" void signal_int( int );

// log level
t_CKUINT g_otf_log = CK_LOG_INFO;




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
    t_CKUINT ret = 0;
    
    // fprintf( stderr, "UDP message recv...\n" );
    if( msg->type == MSG_REPLACE || msg->type == MSG_ADD )
    {
        string filename;
        vector<string> args;

        // parse out command line arguments
        if( !extract_args( msg->buffer, filename, args ) )
        {
            // error
            fprintf( stderr, "[chuck]: malformed filename with argument list...\n" );
            fprintf( stderr, "    -->  '%s'", msg->buffer );
            goto cleanup;
        }
        
        // copy stuff
        if( args.size() > 0 )
        {
            strcpy( msg->buffer, filename.c_str() );
            cmd->args = new vector<string>;
            *(cmd->args) = args;
        }

        // see if entire file is on the way
        if( msg->param2 )
        {
            fd = recv_file( *msg, (ck_socket)data );
            if( !fd )
            {
                fprintf( stderr, "[chuck]: incoming source transfer '%s' failed...\n",
                    mini(msg->buffer) );
                goto cleanup;
            }
        }

        // parse, type-check, and emit
        if( !compiler->go( msg->buffer, fd ) )
            goto cleanup;

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
             || msg->type == MSG_KILL || msg->type == MSG_TIME || msg->type == MSG_RESET_ID )
    {
        cmd->type = msg->type;
        cmd->param = msg->param;
    }
    else
    {
        fprintf( stderr, "[chuck]: unrecognized incoming command from network: '%i'\n", cmd->type );
        SAFE_DELETE(cmd);
        goto cleanup;
    }
    
    // immediate
    if( immediate )
        ret = vm->process_msg( cmd );
    else
    {
        vm->queue_msg( cmd, 1 );
        ret = 1;
    }

cleanup:
    // close file handle
    if( fd ) fclose( fd );

    return ret;
}




//-----------------------------------------------------------------------------
// name: otf_send_file()
// desc: ...
//-----------------------------------------------------------------------------
int otf_send_file( const char * fname, Net_Msg & msg, const char * op,
                   ck_socket dest )
{
    FILE * fd = NULL;
    struct stat fs;
    string filename;
    vector<string> args;

    // parse out command line arguments
    if( !extract_args( fname, filename, args ) )
    {
        // error
        fprintf( stderr, "[chuck]: malformed filename + argument list...\n" );
        fprintf( stderr, "    -->  '%s'", fname );
        return FALSE;
    }

    // filename and any args
    strcpy( msg.buffer, fname );

    // test it
    fd = open_cat_ck( (char *)filename.c_str() );
    if( !fd )
    {
        fprintf( stderr, "[chuck]: cannot open file '%s' for [%s]...\n", filename.c_str(), op );
        return FALSE;
    }

    if( !chuck_parse( (char *)filename.c_str(), fd ) )
    {
        fprintf( stderr, "[chuck]: skipping file '%s' for [%s]...\n", filename.c_str(), op );
        fclose( fd );
        return FALSE;
    }

    // stat it
    memset( &fs, 0, sizeof(fs) );
    stat( filename.c_str(), &fs );
    fseek( fd, 0, SEEK_SET );

    // log
    EM_log( CK_LOG_INFO, "sending TCP file %s, size=%d", filename.c_str(), (t_CKUINT)fs.st_size );
    EM_pushlog();

    // send the first packet
    msg.param2 = (t_CKUINT)fs.st_size;
    msg.length = 0;
    otf_hton( &msg );
    // go
    ck_send( dest, (char *)&msg, sizeof(msg) );

    // send the whole thing
    t_CKUINT left = (t_CKUINT)fs.st_size;
    while( left )
    {
        // log
        EM_log( CK_LOG_FINER, "%03d bytes left ... ", left );
        // amount to send
        msg.length = left > NET_BUFFER_SIZE ? NET_BUFFER_SIZE : left;
        // read
        msg.param3 = fread( msg.buffer, sizeof(char), msg.length, fd );
        // check for error
        if( !msg.param3 )
        {
            // error encountered
            fprintf( stderr, "[chuck]: error while reading '%s'...\n", filename.c_str() );
            // done
            goto done;
        }

        // amount left
        left -= msg.param3 ? msg.param3 : left;
        // what's left
        msg.param2 = left;

        // log
        EM_log( CK_LOG_FINER, "sending buffer %03d length %03d...", msg.param3, msg.length );
        // send it
        otf_hton( &msg );
        ck_send( dest, (char *)&msg, sizeof(msg) );
    }

done:
    // pop log
    EM_poplog();

    // close
    fclose( fd );

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: otf_send_connect()
// desc: ...
//-----------------------------------------------------------------------------
ck_socket otf_send_connect( const char * host, int port )
{
    // log
    EM_log( CK_LOG_INFO, "otf connect: %s:%i", host, port );

    ck_socket sock = ck_tcp_create( 0 );
    if( !sock )
    {
        fprintf( stderr, "[chuck]: cannot open socket to send command...\n" );
        return NULL;
    }

    if( strcmp( host, "127.0.0.1" ) )
        EM_log( g_otf_log, "connecting to %s on port %i via TCP...", host, port );
    
    if( !ck_connect( sock, host, port ) )
    {
        fprintf( stderr, "cannot open TCP socket on %s:%i...\n", host, port );
        ck_close( sock );
        return NULL;
    }
    
    ck_send_timeout( sock, 0, 2000000 );

    return sock;
}




//-----------------------------------------------------------------------------
// name: otf_send_cmd()
// desc: ...
//-----------------------------------------------------------------------------
int otf_send_cmd( int argc, char ** argv, t_CKINT & i, const char * host, int port,
                  int * is_otf )
{
    Net_Msg msg;
    g_sigpipe_mode = 1;
    int tasks_total = 0, tasks_done = 0;
    ck_socket dest = NULL;
    if( is_otf ) *is_otf = TRUE;

    // log
    EM_log( CK_LOG_INFO, "examining otf command '%s'...", argv[i] );

    if( !strcmp( argv[i], "--add" ) || !strcmp( argv[i], "+" ) )
    {
        if( ++i >= argc )
        {
            fprintf( stderr, "[chuck]: not enough arguments following [add]...\n" );
            goto error;
        }

        if( !(dest = otf_send_connect( host, port )) ) return 0;
        EM_pushlog();
        do {
            // log
            EM_log( CK_LOG_INFO, "sending file:args '%s' for add...", mini(argv[i]) );
            msg.type = MSG_ADD;
            msg.param = 1;
            tasks_done += otf_send_file( argv[i], msg, "add", dest );
            tasks_total++;
        } while( ++i < argc );
        // log
        EM_poplog();

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

        if( !(dest = otf_send_connect( host, port )) ) return 0;
        EM_pushlog();
        do {
            // log
            EM_log( CK_LOG_INFO, "requesting removal of shred '%i'..." );
            msg.param = atoi( argv[i] );
            msg.type = MSG_REMOVE;
            otf_hton( &msg );
            ck_send( dest, (char *)&msg, sizeof(msg) );
        } while( ++i < argc );
        // log
        EM_poplog();
    }
    else if( !strcmp( argv[i], "--" ) )
    {
        if( !(dest = otf_send_connect( host, port )) ) return 0;
        EM_pushlog();
        // log
        EM_log( CK_LOG_INFO, "requesting removal of last shred..." );
        msg.param = 0xffffffff;
        msg.type = MSG_REMOVE;
        otf_hton( &msg );
        ck_send( dest, (char *)&msg, sizeof(msg) );
        // log
        EM_poplog();
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

        if( !(dest = otf_send_connect( host, port )) ) return 0;
        EM_pushlog();
        EM_log( CK_LOG_INFO, "requesting replace shred '%i' with '%s'...", msg.param, mini(argv[i]) );
        msg.type = MSG_REPLACE;
        if( !otf_send_file( argv[i], msg, "replace", dest ) )
            goto error;
        EM_poplog();
    }
    else if( !strcmp( argv[i], "--removeall" ) || !strcmp( argv[i], "--remall" ) )
    {
        if( !(dest = otf_send_connect( host, port )) ) return 0;
        EM_pushlog();
        EM_log( CK_LOG_INFO, "requesting removeall..." );
        msg.type = MSG_REMOVEALL;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( dest, (char *)&msg, sizeof(msg) );
        EM_poplog();
    }
    else if( !strcmp( argv[i], "--kill" ) )
    {
        if( !(dest = otf_send_connect( host, port )) ) return 0;
        msg.type = MSG_REMOVEALL;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( dest, (char *)&msg, sizeof(msg) );
        msg.type = MSG_KILL;
        msg.param = (i+1)<argc ? atoi(argv[++i]) : 0;
        otf_hton( &msg );
        ck_send( dest, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--time" ) )
    {
        if( !(dest = otf_send_connect( host, port )) ) return 0;
        msg.type = MSG_TIME;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( dest, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--rid" ) )
    {
        if( !(dest = otf_send_connect( host, port )) ) return 0;
        msg.type = MSG_RESET_ID;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( dest, (char *)&msg, sizeof(msg) );
    }
    else if( !strcmp( argv[i], "--status" ) || !strcmp( argv[i], "^" ) )
    {
        if( !(dest = otf_send_connect( host, port )) ) return 0;
        msg.type = MSG_STATUS;
        msg.param = 0;
        otf_hton( &msg );
        ck_send( dest, (char *)&msg, sizeof(msg) );
    }
    else
    {
        if( is_otf ) *is_otf = FALSE;
        return 0;
    }
        
    // send
    msg.type = MSG_DONE;
    otf_hton( &msg );
    // log
    EM_log( CK_LOG_INFO, "otf sending request..." );
    ck_send( dest, (char *)&msg, sizeof(msg) );

    // set timeout
    ck_recv_timeout( dest, 0, 2000000 );
    // log
    EM_log( CK_LOG_INFO, "otf awaiting reply..." );
    // reply
    if( ck_recv( dest, (char *)&msg, sizeof(msg) ) )
    {
        otf_ntoh( &msg );
        if( !msg.param )
        {
            fprintf( stderr, "[chuck(remote)]:operation failed (sorry)" );
            fprintf( stderr, "...(reason: %s)\n", 
                ( strstr( (char *)msg.buffer, ":" ) ? strstr( (char *)msg.buffer, ":" ) + 1 : (char *)msg.buffer ) );
        }
        else
        {
            EM_log( CK_LOG_INFO, "reply received..." );
        }
    }
    else
    {
        fprintf( stderr, "[chuck]: remote operation timed out...\n" );
    }
    // close the sock
    ck_close( dest );
    
    // exit
    // exit( msg.param );

    return 1;
    
error:
    
    // if sock was opened
    if( dest )
    {
        msg.type = MSG_DONE;
        otf_hton( &msg );
        ck_send( dest, (char *)&msg, sizeof(msg) );
        ck_close( dest );
    }
    
    // exit( 1 );

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

    // catch SIGINT
    // signal( SIGINT, signal_int );
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
            fprintf( stderr, "[chuck]: 0-length packet...\n" );
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
            else
            {
                usleep( 10000 );
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
