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
// file: midiio.h
// desc: midi io header
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//-----------------------------------------------------------------------------
#ifndef __MIDI_IO_H__
#define __MIDI_IO_H__

#include "chuck_def.h"
#include "rtmidi.h"
#include "util_buffers.h"




//-----------------------------------------------------------------------------
// stuff
//-----------------------------------------------------------------------------
#define MIDI_NOTEON         0x90
#define MIDI_NOTEOFF        0x80
#define MIDI_POLYPRESS      0xa0
#define MIDI_CTRLCHANGE     0xb0
#define MIDI_PROGCHANGE     0xc0
#define MIDI_CHANPRESS      0xd0
#define MIDI_PITCHBEND      0xe0
#define MIDI_ALLNOTESOFF    0x7b




//-----------------------------------------------------------------------------
// definitions
//-----------------------------------------------------------------------------
union MidiMsg
{
    t_CKBYTE data[4];
    t_CKUINT dw;
};




//-----------------------------------------------------------------------------
// name: class MidiOut
// desc: midi out
//-----------------------------------------------------------------------------
class MidiOut
{
public:
    MidiOut();
    ~MidiOut();

public:
    t_CKBOOL open( t_CKUINT device_num = 0 );
    t_CKBOOL close();

public:
    t_CKUINT  send( t_CKBYTE status );
    t_CKUINT  send( t_CKBYTE status, t_CKBYTE data1 );
    t_CKUINT  send( t_CKBYTE status, t_CKBYTE data1, t_CKBYTE data2 );
    t_CKUINT  send( const MidiMsg * msg );

public:
    t_CKUINT  noteon( t_CKUINT  channel, t_CKUINT  note, t_CKUINT  velocity );
    t_CKUINT  noteoff( t_CKUINT  channel, t_CKUINT  note, t_CKUINT  velocity );
    t_CKUINT  polypress( t_CKUINT  channel, t_CKUINT  note, t_CKUINT  pressure );
    t_CKUINT  ctrlchange( t_CKUINT  channel, t_CKUINT  ctrl_num, t_CKUINT  ctrl_val );
    t_CKUINT  progchange( t_CKUINT  channel, t_CKUINT  patch );
    t_CKUINT  chanpress( t_CKUINT  channel, t_CKUINT  pressure );
    t_CKUINT  pitchbend( t_CKUINT  channel, t_CKUINT  bend_val );
    t_CKUINT  allnotesoff( t_CKUINT  channel );

public:
	RtMidiOut * mout;
	std::vector<unsigned char> m_msg;
	t_CKUINT m_device_num;
	t_CKBOOL m_valid;
};




//-----------------------------------------------------------------------------
// name: class MidiIn
// desc: midi
//-----------------------------------------------------------------------------
class MidiIn : public Chuck_Event
{
public:
    MidiIn();
    ~MidiIn();

public:
    t_CKBOOL open( t_CKUINT device_num = 0 );
    t_CKBOOL close();

public:
    t_CKUINT  recv( MidiMsg * msg );

public:
    CBuffer * m_buffer;
	t_CKUINT m_read_index;
	RtMidiIn * min;
	t_CKBOOL m_valid;
	t_CKUINT m_device_num;
	t_CKUINT m_ref_count;
	Chuck_Object * SELF;
    // static void CALLBACK cb_midi_input( HMIDIIN hm_in, t_CKUINT  msg,
    //                  DWORD instance, DWORD param1, DWORD param2 );
};


void probeMidiIn();
void probeMidiOut();


class MidiInManager
{
public:
	static t_CKBOOL open( MidiIn * min, t_CKINT device_num );
	static t_CKBOOL close( MidiIn * min );

    static void cb_midi_input( double deltatime, std::vector<unsigned char> * msg,
		                       void *userData );
protected:
	MidiInManager();
	~MidiInManager();

    static std::vector<RtMidiIn *> the_mins;
    static std::vector<CBuffer *> the_bufs;
};


class MidiOutManager
{
public:
	static t_CKBOOL open( MidiOut * mout, t_CKINT device_num );
	static t_CKBOOL close( MidiOut * mout );

protected:
	MidiOutManager();
	~MidiOutManager();

	static std::vector<RtMidiOut *> the_mouts;
};




#endif
