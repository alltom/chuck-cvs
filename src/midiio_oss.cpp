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
// file: midiio_oss.cpp
// desc: midi io oss implementation
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//-----------------------------------------------------------------------------
#include "midiio_oss.h"
#include <stdio.h>
#include <stdlib.h>




//-----------------------------------------------------------------------------
// name: MidiOut()
// desc: constructor
//-----------------------------------------------------------------------------
MidiOut::MidiOut()
{
    UINT__ m_device_num = 0;
    // m_midi_out = NULL;
}




//-----------------------------------------------------------------------------
// name: ~MidiOut()
// desc: destructor
//-----------------------------------------------------------------------------
MidiOut::~MidiOut()
{
    this->close();
}




//-----------------------------------------------------------------------------
// name: open()
// desc: open a device
//-----------------------------------------------------------------------------
BOOL__ MidiOut::open( int device_num )
{
    int err = 0;
    char buffer[256];

    m_device_num = device_num;

    // open the midi
    sprintf( buffer, "hw:0,%d", m_device_num );    
    
    return err == 0;
}




//-----------------------------------------------------------------------------
// name: close()
// desc: close the device
//-----------------------------------------------------------------------------
BOOL__ MidiOut::close( )
{
    // send everything in buffer
    
    // close midi out

    return true;
}




//-----------------------------------------------------------------------------
// name: drain()
// desc: ...
//-----------------------------------------------------------------------------
BOOL__ MidiOut::drain()
{
    return true;
}




//-----------------------------------------------------------------------------
// name: send()
// desc: send 1 BYTE__ midi message
//-----------------------------------------------------------------------------
UINT__ MidiOut::send( BYTE__ status )
{
    // send

    return true;
}




//-----------------------------------------------------------------------------
// name: send()
// desc: send 2 BYTE__ midi message
//-----------------------------------------------------------------------------
UINT__ MidiOut::send( BYTE__ status, BYTE__ data1 )
{
    // send
    
    return true;
}




//-----------------------------------------------------------------------------
// name: send()
// desc: send 3 BYTE__ midi message
//-----------------------------------------------------------------------------
UINT__ MidiOut::send( BYTE__ status, BYTE__ data1, BYTE__ data2 )
{
    // send the three BYTE

    return true;
}




//-----------------------------------------------------------------------------
// name: send()
// desc: send midi message
//-----------------------------------------------------------------------------
UINT__ MidiOut::send( const MidiMsg * msg )
{
    return this->send( msg->data[0], msg->data[1], msg->data[2] );
}




//-----------------------------------------------------------------------------
// name: MidiIn()
// desc: constructor
//-----------------------------------------------------------------------------
MidiIn::MidiIn()
{
    // m_midi_in = NULL;
    m_device_num = 0;
    pthread_mutex_init( &m_mutex, NULL );
}




//-----------------------------------------------------------------------------
// name: ~MidiIn()
// desc: destructor
//-----------------------------------------------------------------------------
MidiIn::~MidiIn( )
{
    this->close();
}




//-----------------------------------------------------------------------------
// name: open()
// desc: open
//-----------------------------------------------------------------------------
BOOL__ MidiIn::open( int device_num )
{
  int err = 0;
  char buffer[256];

  m_device_num = device_num;

  // open the raw midi
  sprintf( buffer, "hw:0,%d", m_device_num );  

  // initialize the buffer
  m_buffer.initialize( 1024, sizeof( MidiMsg ) );

  // thread
  pthread_create( &m_cb_thread_id, NULL, midi_in_cb, this );

  return err == 0;
}




//-----------------------------------------------------------------------------
// name: close()
// desc: close
//-----------------------------------------------------------------------------
BOOL__ MidiIn::close()
{
    pthread_cancel( m_cb_thread_id );
    pthread_mutex_destroy( &m_mutex );

    return true;
}




//-----------------------------------------------------------------------------
// name: get()
// desc: get message
//-----------------------------------------------------------------------------
UINT__ MidiIn::recv( MidiMsg * msg )
{
    UINT__ r = 0;
    // pthread_mutex_lock( &m_mutex );
    r = m_buffer.get( msg, 1 );
    // pthread_mutex_unlock( &m_mutex );

    return r;
}




//-----------------------------------------------------------------------------
// name: Cbuffer()
// desc: constructor
//-----------------------------------------------------------------------------
CBuffer::CBuffer()
{
    m_data = NULL;
    m_data_width = m_read_offset = m_write_offset = m_max_elem = 0;
}




//-----------------------------------------------------------------------------
// name: ~CBuffer()
// desc: destructor
//-----------------------------------------------------------------------------
CBuffer::~CBuffer()
{
    this->cleanup();
}




//-----------------------------------------------------------------------------
// name: initialize()
// desc: initialize
//-----------------------------------------------------------------------------
BOOL__ CBuffer::initialize( UINT__ num_elem, UINT__ width )
{
    // cleanup
    cleanup();

    // allocate
    m_data = (BYTE__ *)malloc( num_elem * width );
    if( !m_data )
        return false;

    m_data_width = width;
    m_read_offset = 0;
    m_write_offset = 0;
    m_max_elem = num_elem;

    return true;
}




//-----------------------------------------------------------------------------
// name: cleanup()
// desc: cleanup
//-----------------------------------------------------------------------------
void CBuffer::cleanup()
{
    if( !m_data )
        return;

    free( m_data );

    m_data = NULL;
    m_data_width = m_read_offset = m_write_offset = m_max_elem = 0;
}




//-----------------------------------------------------------------------------
// name: put()
// desc: put
//-----------------------------------------------------------------------------
void CBuffer::put( void * data, UINT__ num_elem )
{
    UINT__ i, j;
    BYTE__ * d = (BYTE__ *)data;

    // copy
    for( i = 0; i < num_elem; i++ )
    {
        for( j = 0; j < m_data_width; j++ )
        {
            m_data[m_write_offset*m_data_width+j] = d[i*m_data_width+j];
        }

        // move the write
        m_write_offset++;

        // wrap
        if( m_write_offset >= m_max_elem )
            m_write_offset = 0;
    }
}




//-----------------------------------------------------------------------------
// name: get()
// desc: get
//-----------------------------------------------------------------------------
UINT__ CBuffer::get( void * data, UINT__ num_elem )
{
    UINT__ i, j;
    BYTE__ * d = (BYTE__ *)data;

    // read catch up with write
    if( m_read_offset == m_write_offset )
        return 0;

    // copy
    for( i = 0; i < num_elem; i++ )
    {
        for( j = 0; j < m_data_width; j++ )
        {
            d[i*m_data_width+j] = m_data[m_read_offset*m_data_width+j];
        }

        // move read
        m_read_offset++;
        
        // catch up
        if( m_read_offset == m_write_offset )
        {
            i++;
            break;
        }

        // wrap
        if( m_read_offset >= m_max_elem )
            m_read_offset = 0;
    }

    // return number of elems
    return 1;
}




//-----------------------------------------------------------------------------
// name: midi_in_cb()
// desc: ...
//-----------------------------------------------------------------------------
void * MidiIn::midi_in_cb( void * arg )
{
    MidiIn * min = (MidiIn *)arg;
    BYTE__ byte = 0;
    int n = 0, num_args = 0, num_left = 0;
    MidiMsg msg;

    while( true )
    {
        // get the next BYTE

        if( n < 0 )
        {
            // encounter error
            fprintf( stdout, "error: rawmidi_read...\n" );
            continue;
        }

        while( n > 0 )
        {
            if( byte & 0x80 ) // status byte
            {
                if( (byte & 0xf0) == 0xf0 ) // system msg
                {
                    n--;
                    continue;
                }
            
                if( ( (byte & 0xf0) == 0xc0 ) || ( (byte & 0xf0) == 0xd0 ) )
                    num_args = 1;
                else
                    num_args = 2;

                msg.data[3] = 0;
                msg.data[2] = byte;
                msg.data[1] = 0;
                msg.data[0] = 0;
                num_left = num_args;
            }
            else // data byte
            {
                if( num_left == num_args )
                    msg.data[1] = byte;
                else
                    msg.data[0] = byte;

                num_left--;

                if( !num_left )
                {
                    if( ((msg.data[2] & 0xf0) == 0xc0) || ((msg.data[2] & 0xf0) == 0xd0) )
                        num_left = 1;
                    else
                        num_left = 2;

                    //pthread_mutex_lock( &min->m_mutex );
                    min->m_buffer.put( &msg, 1 );
                    //pthread_mutex_unlock( &min->m_mutex );
                }
            }
        n--;
        }
    }

    return NULL;
}
