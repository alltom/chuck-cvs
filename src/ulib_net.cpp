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
// file: ulib_net.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#include "ulib_net.h"
#include "util_network.h"




//-----------------------------------------------------------------------------
// name: net_query()
// desc: ...
//-----------------------------------------------------------------------------
DLL_QUERY net_query( Chuck_DL_Query * QUERY )
{
/*    // add netout
    QUERY->ugen_add( QUERY, "netout", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, netout_ctor, netout_dtor, netout_tick, NULL );
    // ctrl funcs
    QUERY->ugen_ctrl( QUERY, netout_ctrl_addr, netout_cget_addr, "string", "addr" );
    QUERY->ugen_ctrl( QUERY, netout_ctrl_port, netout_cget_port, "int", "port" );
    QUERY->ugen_ctrl( QUERY, netout_ctrl_size, netout_cget_size, "dur", "size" );
    QUERY->ugen_ctrl( QUERY, netout_ctrl_name, netout_cget_name, "string", "name" );
    
    // add netin
    QUERY->ugen_add( QUERY, "netin", NULL );
    // set funcs
    QUERY->ugen_func( QUERY, netin_ctor, netin_dtor, netin_tick, NULL );
    // ctrl funcs
    QUERY->ugen_ctrl( QUERY, netin_ctrl_port, netin_cget_port, "int", "port" );
    QUERY->ugen_ctrl( QUERY, netin_ctrl_name, netin_cget_name, "string", "name" );
*/    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: struct GigaMsg
// desc: ...
//-----------------------------------------------------------------------------
struct GigaMsg
{
    unsigned int type;
    unsigned int len;
    unsigned int seq_num;
  
    unsigned char * payload;

    // constructor
    GigaMsg() {
        type = len = seq_num = 0;
        payload = NULL;
    }

    // init
    void init( unsigned int _type, unsigned int _len )
    {
        type = _type;
        len = _len;
        payload = new unsigned char[len];
    }

    // destructor
    ~GigaMsg( )
    {
        if( payload )
        {
            delete [] payload;
	        payload = NULL;
        }
        type = 0;
        len = 0;
        seq_num = 0;
    }
};




//-----------------------------------------------------------------------------
// name: class GigaSend
// desc: ...
//-----------------------------------------------------------------------------
class GigaSend
{
public:
    GigaSend( );
    ~GigaSend( );

    t_CKBOOL connect( const char * hostname, int port, t_CKUINT buffer_size );
    t_CKBOOL disconnect( );

    void set_redundancy( t_CKUINT n );
    t_CKUINT get_redundancy( );

    t_CKBOOL send( const t_CKBYTE * buffer );

protected:
    ck_socket m_sock;
    t_CKUINT m_red;
    t_CKUINT m_buffer_size;
    GigaMsg m_msg;
    t_CKUINT m_len;
    t_CKBYTE m_buffer[0x8000];
};




//-----------------------------------------------------------------------------
// name: class GigaRecv
// desc: ...
//-----------------------------------------------------------------------------
class GigaRecv
{
public:
    GigaRecv( );
    ~GigaRecv( );

    t_CKBOOL listen( int port, t_CKUINT buffer_size );

    t_CKBOOL recv( t_CKBYTE * buffer );
    t_CKBOOL expire();

protected:
    ck_socket m_sock;
    t_CKUINT m_buffer_size;
    GigaMsg m_msg;
    t_CKUINT m_len;
    t_CKBYTE m_buffer[0x8000];
};




//-----------------------------------------------------------------------------
// name: netout
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTOR netout_ctor( t_CKTIME now );
UGEN_DTOR netout_dtor( t_CKTIME now, void * data );
UGEN_TICK netout_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );
UGEN_CTRL netout_ctrl_addr( t_CKTIME now, void * data, void * value );
UGEN_CGET netout_cget_addr( t_CKTIME now, void * data, void * out );
UGEN_CTRL netout_ctrl_port( t_CKTIME now, void * data, void * value );
UGEN_CGET netout_cget_port( t_CKTIME now, void * data, void * out );
UGEN_CTRL netout_ctrl_size( t_CKTIME now, void * data, void * value );
UGEN_CGET netout_cget_size( t_CKTIME now, void * data, void * out );
UGEN_CTRL netout_ctrl_name( t_CKTIME now, void * data, void * value );
UGEN_CGET netout_cget_name( t_CKTIME now, void * data, void * out );




//-----------------------------------------------------------------------------
// name: netin
// desc: ...
//-----------------------------------------------------------------------------
UGEN_CTOR netin_ctor( t_CKTIME now );
UGEN_DTOR netin_dtor( t_CKTIME now, void * data );
UGEN_TICK netin_tick( t_CKTIME now, void * data, SAMPLE in, SAMPLE * out );
UGEN_CTRL netin_ctrl_port( t_CKTIME now, void * data, void * value );
UGEN_CGET netin_cget_port( t_CKTIME now, void * data, void * out );
UGEN_CTRL netin_ctrl_name( t_CKTIME now, void * data, void * value );
UGEN_CGET netin_cget_name( t_CKTIME now, void * data, void * out );
