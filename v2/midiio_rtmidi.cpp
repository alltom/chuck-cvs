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
// file: midiio_win32.cpp
// desc: midi io
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <vector>
#include "midiio_rtmidi.h"
#include "chuck_errmsg.h"




//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
#define BUFFER_SIZE 8192




//-----------------------------------------------------------------------------
// name: MidiOut()
// desc: constructor
//-----------------------------------------------------------------------------
MidiOut::MidiOut()
{
	mout = new RtMidiOut;
    m_device_num = 0;
	m_valid = FALSE;
}




//-----------------------------------------------------------------------------
// name: ~MidiOut()
// desc: destructor
//-----------------------------------------------------------------------------
MidiOut::~MidiOut()
{
    if( mout ) this->close();
	SAFE_DELETE( mout );
}




//-----------------------------------------------------------------------------
// name: send()
// desc: send 1 byte midi message
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::send( t_CKBYTE status )
{
    return this->send( status, 0, 0 );
}




//-----------------------------------------------------------------------------
// name: send()
// desc: send 2 byte midi message
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::send( t_CKBYTE status, t_CKBYTE data1 )
{
    return this->send( status, data1, 0 );
}




//-----------------------------------------------------------------------------
// name: send()
// desc: send 3 byte midi message
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::send( t_CKBYTE status, t_CKBYTE data1, t_CKBYTE data2 )
{
	if( !m_valid ) return 0;

	// clear
	m_msg.clear();
	// add
	m_msg.push_back( status );
	m_msg.push_back( data1 );
	m_msg.push_back( data2 );

    mout->sendMessage( &m_msg );

	return 3;
}




//-----------------------------------------------------------------------------
// name: send()
// desc: send
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::send( const MidiMsg * msg )
{
	if( !m_valid ) return 0;

	// clear
	m_msg.clear();
	// add
	m_msg.push_back( msg->data[0] );
	m_msg.push_back( msg->data[1] );
	m_msg.push_back( msg->data[2] );

    mout->sendMessage( &m_msg );

	return 3;
}




//-----------------------------------------------------------------------------
// name: open
// desc: open midi output
//-----------------------------------------------------------------------------
t_CKBOOL MidiOut::open( t_CKUINT device_num )
{
    // close if already opened
    if( m_valid )
        this->close();

    // copy
    m_device_num = device_num;

	// open the port
	try {
		mout->openPort( m_device_num );
		m_valid = TRUE;
	} catch( RtError & err )
	{
        // print it
		EM_error2( 0, "MidiOut: couldn't open port %i", m_device_num );
		EM_error2( 0, "...(reason: %s)", err.getMessage().c_str() );
		return FALSE;
	}

    // open the midi out
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: close( )
// desc: close midi output
//-----------------------------------------------------------------------------
t_CKBOOL MidiOut::close( )
{
    if( !m_valid )
        return FALSE;

    // close
	mout->closePort();

	m_valid = FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: noteon()
// desc: note on message
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::noteon( t_CKUINT channel, t_CKUINT note, t_CKUINT velocity )
{ return this->send( (t_CKBYTE)(MIDI_NOTEON + channel), note, velocity ); }




//-----------------------------------------------------------------------------
// name: noteoff()
// desc: note off message
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::noteoff( t_CKUINT channel, t_CKUINT note, t_CKUINT velocity )
{ return this->send( (t_CKBYTE)(MIDI_NOTEOFF + channel), note, velocity ); }




//-----------------------------------------------------------------------------
// name: polypress()
// desc: polypress message
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::polypress( t_CKUINT channel, t_CKUINT note, t_CKUINT pressure )
{ return this->send( (t_CKBYTE)(MIDI_POLYPRESS + channel), note, pressure ); }




//-----------------------------------------------------------------------------
// name: ctrlchange()
// desc: ctrl change message
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::ctrlchange( t_CKUINT channel, t_CKUINT ctrl_num, t_CKUINT ctrl_val )
{ return this->send( (t_CKBYTE)(MIDI_CTRLCHANGE + channel), ctrl_num, ctrl_val ); }




//-----------------------------------------------------------------------------
// name: progchange()
// desc: prog change message
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::progchange( t_CKUINT channel, t_CKUINT patch )
{ return this->send( (t_CKBYTE)(MIDI_PROGCHANGE + channel), patch, 0 ); }




//-----------------------------------------------------------------------------
// name: chanpress()
// desc: chan press
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::chanpress( t_CKUINT channel, t_CKUINT pressure )
{ return this->send( (t_CKBYTE)(MIDI_CHANPRESS + channel), pressure, 0 ); }




//-----------------------------------------------------------------------------
// name: pitchbend()
// desc: pitch bend
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::pitchbend( t_CKUINT channel, t_CKUINT bend_val )
{
	assert( FALSE );
	return 0;
//    return this->send( (t_CKBYTE)(MIDI_PITCHBEND + channel),
//                       (t_CKBYTE)(HIBYTE( bend_val << 1 )),
//                       (t_CKBYTE)(LOBYTE( bend_val & 0x7f )) );
}




//-----------------------------------------------------------------------------
// name: allnotesoff()
// desc: allnotesoff
//-----------------------------------------------------------------------------
t_CKUINT MidiOut::allnotesoff( t_CKUINT channel )
{
    return this->send( (t_CKBYTE)(MIDI_CTRLCHANGE + channel), 
                       (t_CKBYTE)(MIDI_ALLNOTESOFF), 0 );
}




//-----------------------------------------------------------------------------
// name: MidiIn()
// desc: constructor
//-----------------------------------------------------------------------------
MidiIn::MidiIn()
{
	min = new RtMidiIn;
	m_device_num = 0;
	m_valid = FALSE;
}




//-----------------------------------------------------------------------------
// name: ~MidiIn()
// desc: destructor
//-----------------------------------------------------------------------------
MidiIn::~MidiIn( )
{
    this->close();
	SAFE_DELETE( min );
}




//-----------------------------------------------------------------------------
// name: open()
// desc: open
//-----------------------------------------------------------------------------
t_CKBOOL MidiIn::open( t_CKUINT device_num )
{
    // close if already opened
    if( m_valid )
        this->close();

    // copy
    m_device_num = device_num;

	// open the port
	try {
		min->openPort( m_device_num );
		min->setCallback( cb_midi_input, this );
		m_valid = TRUE;
	} catch( RtError & err ) {
        // print it
		EM_error2( 0, "MidiOut: couldn't open port %i", m_device_num );
		EM_error2( 0, "...(reason: %s)", err.getMessage().c_str() );
		return FALSE;
	}

    // allocate the buffer
    if( !m_buffer.initialize( BUFFER_SIZE, sizeof(MidiMsg) ) )
    {
        this->close();
        return FALSE;
    }

    // open the midi out
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: close()
// desc: close
//-----------------------------------------------------------------------------
t_CKBOOL MidiIn::close()
{
    if( !m_valid )
        return FALSE;

	// close
	min->closePort();

    // deallocate the buffer
    m_buffer.cleanup();

	m_valid = FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: get()
// desc: get message
//-----------------------------------------------------------------------------
t_CKUINT MidiIn::recv( MidiMsg * msg )
{
    return m_buffer.get( msg, 1 );
}




//-----------------------------------------------------------------------------
// name: cb_midi_output
// desc: call back
//-----------------------------------------------------------------------------
void MidiIn::cb_midi_input( double deltatime, std::vector<unsigned char> * msg,
		                    void * userData )
{
	unsigned int nBytes = msg->size();
	MidiIn * min = (MidiIn *)userData;
	MidiMsg m;
	if( nBytes >= 1 ) m.data[0] = msg->at(0);
	if( nBytes >= 2 ) m.data[1] = msg->at(1);
	if( nBytes >= 3 ) m.data[2] = msg->at(2);

    // put in the buffer, make sure not active sensing
    if( m.data[2] != 0xfe )
    {
        min->m_buffer.put( &m, 1 );
    }
}
