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
// name: util_network.c
// desc: sockets
// 
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//
// originally randy_socket
// author: Peng Bi (pbi@cs.princeton.edu)
//         Ge Wang (gewang@cs.princeton.edu)
//-----------------------------------------------------------------------------
#include "util_network.h"
#include "chuck_utils.h"

#if defined(__PLATFORM_WIN32__)
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif




//-----------------------------------------------------------------------------
// name: struct ck_socket_
// desc: ...
//-----------------------------------------------------------------------------
struct ck_socket_
{
    int sock;
    int prot;
    struct sockaddr_in sock_in;
    int len;

#ifndef __PLATFORM_WIN32__
};
#else
    static WSADATA wsd;
    static int init;
};

WSADATA ck_socket_::wsd;
int ck_socket_::init = 0;
#endif



//-----------------------------------------------------------------------------
// name: ck_udp_create()
// desc: create a udp socket
//-----------------------------------------------------------------------------
ck_socket ck_udp_create( )
{
    ck_socket sock = (ck_socket)checked_malloc( sizeof( struct ck_socket_ ) );

#ifdef __PLATFORM_WIN32__
    // winsock init
    if( ck_socket_::init == 0 )
        WSAStartup( MAKEWORD(1,1), &(ck_socket_::wsd) );

    // count
    ck_socket_::init++;
#endif

    sock->sock = socket( AF_INET, SOCK_DGRAM, 0 );
    sock->prot = SOCK_DGRAM;

    return sock;
}




//-----------------------------------------------------------------------------
// name: ck_tcp_create()
// desc: create a tcp socket
//-----------------------------------------------------------------------------
ck_socket ck_tcp_create( )
{
    ck_socket sock = (ck_socket)checked_malloc( sizeof( struct ck_socket_ ) );

#ifdef __PLATFORM_WIN32__
    // winsock init
    if( ck_socket_::init == 0 )
        WSAStartup( MAKEWORD(1,1), &(ck_socket_::wsd) );

    // count
    ck_socket_::init++;
#endif

    sock->sock = socket( AF_INET, SOCK_STREAM, 0 );
    sock->prot = SOCK_STREAM;

    return sock;
}




//-----------------------------------------------------------------------------
// name: ck_connect2()
// desc: connect to a server
//-----------------------------------------------------------------------------
t_CKBOOL ck_connect2( ck_socket sock, const struct sockaddr * addr, int size ) 
{
    int ret = connect( sock->sock, addr, size );

    return ( ret >= 0 );
}




//-----------------------------------------------------------------------------
// name: ck_connect()
// desc: connect to a server
//-----------------------------------------------------------------------------
t_CKBOOL ck_connect( ck_socket sock, const char * hostname, int port )
{
    int ret;
    struct hostent * host;

#ifdef __PLATFORM_WIN32__
    memset( &sock->sock_in, 0, sizeof(struct sockaddr_in) );
#else
    bzero( &sock->sock_in, sizeof(struct sockaddr_in) );
#endif

    sock->sock_in.sin_family = AF_INET;
    sock->sock_in.sin_port = htons( (unsigned short)port );
    // lookup name
    host = gethostbyname( hostname );
    if( !host )
    {
        sock->sock_in.sin_addr.s_addr = inet_addr( hostname );
        if( sock->sock_in.sin_addr.s_addr == -1 )
            return FALSE;
    }
    else
    {
#ifdef __PLATFORM_WIN32__
        memcpy( (char *)&sock->sock_in.sin_addr, host->h_addr, host->h_length );
#else
        bcopy( host->h_addr, (char *)&sock->sock_in.sin_addr, host->h_length );
#endif
	}

    ret = ck_connect2( sock, (struct sockaddr *)&sock->sock_in, 
        sizeof( struct sockaddr_in ) );

    return ( ret >= 0 );
}




//-----------------------------------------------------------------------------
// name: ck_bind()
// desc: bind to a port
//-----------------------------------------------------------------------------
t_CKBOOL ck_bind( ck_socket sock, int port ) 
{
    int ret;

#ifdef __PLATFORM_WIN32__
    memset( &sock->sock_in, 0,  sizeof(struct sockaddr_in) );
#else
    bzero( &sock->sock_in, sizeof(struct sockaddr_in) );
#endif

    sock->sock_in.sin_family = AF_INET;
    sock->sock_in.sin_port = htons( (unsigned short)port );
    sock->sock_in.sin_addr.s_addr = htonl( INADDR_ANY );

    ret = bind( sock->sock, (struct sockaddr *)&sock->sock_in, 
        sizeof(struct sockaddr_in));

    return ( ret >= 0 );
}




//-----------------------------------------------------------------------------
// name: ck_listen()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL ck_listen( ck_socket sock, int backlog )
{
    int ret;
    
    if( sock->prot != SOCK_STREAM ) return FALSE;
    ret = listen( sock->sock, backlog );
    
    return ( ret >= 0 );
}




//-----------------------------------------------------------------------------
// name: ck_accept()
// desc: ...
//-----------------------------------------------------------------------------
ck_socket ck_accept( ck_socket sock )
{
    ck_socket client;
    
    if( sock->prot != SOCK_STREAM ) return FALSE;
    client = (ck_socket)checked_malloc( sizeof( struct ck_socket_ ) );
    client->len = sizeof( client->sock_in );
    client->sock = accept( sock->sock, (struct sockaddr *)&client->sock_in,
        &client->len );
    if( !client->sock ) goto error;
    
    return client;

error:
    free( client );
    return NULL;
}




//-----------------------------------------------------------------------------
// name: ck_send()
// desc: send a datagram
//-----------------------------------------------------------------------------
int ck_send( ck_socket sock, const char * buffer, int len ) 
{
    return send( sock->sock, buffer, len, 0 );
}




//-----------------------------------------------------------------------------
// name: ck_sendto()
// desc: send a datagram
//-----------------------------------------------------------------------------
int ck_sendto( ck_socket sock, const char * buffer, int len,
               const struct sockaddr * to, int tolen ) 
{
    return sendto( sock->sock, buffer, len, 0, to, tolen );
}




//-----------------------------------------------------------------------------
// name: ck_recvfrom()
// desc: recv a datagram
//-----------------------------------------------------------------------------
int ck_recvfrom( ck_socket sock, char * buffer, int len,
                 struct sockaddr * from, int * fromlen ) 
{
    return recvfrom( sock->sock, buffer, len, 0, from, fromlen );
}




//-----------------------------------------------------------------------------
// name: ck_recv
// desc: recv a datagram
//-----------------------------------------------------------------------------
int ck_recv( ck_socket sock, char * buffer, int len ) 
{
    return recv( sock->sock, buffer, len, 0);
}




//-----------------------------------------------------------------------------
// name: ck_close()
// close the socket
//-----------------------------------------------------------------------------
void ck_close( ck_socket sock ) 
{
    close( sock->sock );
    free( sock );

#ifdef __PLATFORM_WIN32__
    // uncount
    ck_socket_::init--;

    // close
    if( ck_socket_::init == 0 ) WSACleanup();
#endif
}
