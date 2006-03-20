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
// file: hidio_sdl.cpp
// desc: HID io over SDL
//
// author: Spencer Salazar (ssalazar@cs.princeton.edu)
//         Ge Wang (gewang@cs.princeton.edu)
//         Ananya Misra (amisra@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: spring 2006
//-----------------------------------------------------------------------------
#include "hidio_sdl.h"
#include "chuck_errmsg.h"
#include <vector>
#include <map>


struct PhyHidDevIn
{
    // lies
};

struct PhyHidDevOut
{
    // lies
};



//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
#define BUFFER_SIZE 8192

std::vector<PhyHidDevIn *> HidInManager::the_phins;
std::vector<CBufferAdvance *> HidInManager::the_bufs;
std::vector<PhyHidDevOut *> HidOutManager::the_phouts;



//-----------------------------------------------------------------------------
// name: HidOut()
// desc: constructor
//-----------------------------------------------------------------------------
HidOut::HidOut()
{
    // phout = new PhyHidDevOut;
    m_device_num = 0;
    m_valid = FALSE;
    m_suppress_output = FALSE;
}




//-----------------------------------------------------------------------------
// name: ~HidOut()
// desc: destructor
//-----------------------------------------------------------------------------
HidOut::~HidOut()
{
    if( phout ) this->close();
    SAFE_DELETE( phout );
}




//-----------------------------------------------------------------------------
// name: send()
// desc: send
//-----------------------------------------------------------------------------
t_CKUINT HidOut::send( const HidMsg * msg )
{
    if( !m_valid ) return 0;

    return 0;
}




//-----------------------------------------------------------------------------
// name: open
// desc: open HID output
//-----------------------------------------------------------------------------
t_CKBOOL HidOut::open( t_CKUINT device_num )
{
    // close if already opened
    if( m_valid )
        this->close();

    return m_valid = HidOutManager::open( this, (t_CKINT)device_num );
}




//-----------------------------------------------------------------------------
// name: close( )
// desc: close HID output
//-----------------------------------------------------------------------------
t_CKBOOL HidOut::close( )
{
    if( !m_valid )
        return FALSE;

    // close
    // HidOutManager::close( this );

    m_valid = FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: HidIn()
// desc: constructor
//-----------------------------------------------------------------------------
HidIn::HidIn()
{
    phin = NULL;
    m_device_num = 0;
    m_valid = FALSE;
    m_read_index = 0;
    m_buffer = NULL;
    m_suppress_output = FALSE;
    SELF = NULL;
}




//-----------------------------------------------------------------------------
// name: ~HidIn()
// desc: destructor
//-----------------------------------------------------------------------------
HidIn::~HidIn( )
{
    this->close();
    // SAFE_DELETE( min );
}




//-----------------------------------------------------------------------------
// name: open()
// desc: open
//-----------------------------------------------------------------------------
t_CKBOOL HidIn::open( t_CKUINT device_num )
{
    // close if already opened
    if( m_valid )
        this->close();

    // open
    return m_valid = HidInManager::open( this, (t_CKINT)device_num );
}




HidInManager::HidInManager()
{
    the_phins.resize( 1024 );
    the_bufs.resize( 1024 );
}


HidInManager::~HidInManager()
{
    // yeah right
}


t_CKBOOL HidInManager::open( HidIn * hin, t_CKINT device_num )
{
    // see if port not already open
    if( device_num >= (t_CKINT)the_phins.capacity() || !the_phins[device_num] )
    {
        // allocate the buffer
        CBufferAdvance * cbuf = new CBufferAdvance;
        if( !cbuf->initialize( BUFFER_SIZE, sizeof(HidMsg) ) )
        {
            if( !hin->m_suppress_output )
                EM_error2( 0, "HidIn: couldn't allocate CBuffer for port %i...", device_num );
            delete cbuf;
            return FALSE;
        }

        // allocate
        PhyHidDevIn * phin = new PhyHidDevIn;
        /*
        try {
            rtmin->openPort( device_num );
            rtmin->setCallback( cb_midi_input, cbuf );
        } catch( RtError & err ) {
            if( !min->m_suppress_output )
            {
                // print it
                EM_error2( 0, "MidiIn: couldn't open MIDI port %i...", device_num );
                err.getMessage();
                // const char * e = err.getMessage().c_str();
                // EM_error2( 0, "...(%s)", err.getMessage().c_str() );
            }
            delete cbuf;
            return FALSE;
        }*/

        // resize?
        if( device_num >= (t_CKINT)the_phins.capacity() )
        {
            t_CKINT size = the_phins.capacity() * 2;
            if( device_num >= size ) size = device_num + 1;
            the_phins.resize( size );
            the_bufs.resize( size );
        }

        // put cbuf and rtmin in vector for future generations
        the_phins[device_num] = phin;
        the_bufs[device_num] = cbuf;
    }

    // set min
    hin->phin = the_phins[device_num];
    // found
    hin->m_buffer = the_bufs[device_num];
    // get an index into your (you are min here) own buffer, 
    // and a free ticket to your own workshop
    hin->m_read_index = hin->m_buffer->join( (Chuck_Event *)hin->SELF );
    hin->m_device_num = (t_CKUINT)device_num;

    // done
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: close()
// desc: close
//-----------------------------------------------------------------------------
t_CKBOOL HidIn::close()
{
    if( !m_valid )
        return FALSE;

    // close
    // HidInManager::close( this );

    m_valid = FALSE;

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: empty()
// desc: is empty?
//-----------------------------------------------------------------------------
t_CKBOOL HidIn::empty()
{
    if( !m_valid ) return TRUE;
    return m_buffer->empty( m_read_index );
}




//-----------------------------------------------------------------------------
// name: get()
// desc: get message
//-----------------------------------------------------------------------------
t_CKUINT HidIn::recv( HidMsg * msg )
{
    if( !m_valid ) return FALSE;
    return m_buffer->get( msg, 1, m_read_index );
}




//-----------------------------------------------------------------------------
// name: cb_hid_input
// desc: call back
//-----------------------------------------------------------------------------
void HidInManager::cb_hid_input( double deltatime, std::vector<unsigned char> * msg,
                                 void * userData )
{
}




//-----------------------------------------------------------------------------
// name: probeHidIn()
// desc: ...
//-----------------------------------------------------------------------------
void probeHidIn()
{
}




//-----------------------------------------------------------------------------
// name: probeHidOut()
// desc: ...
//-----------------------------------------------------------------------------
void probeHidOut()
{
}




HidOutManager::HidOutManager()
{
    the_phouts.resize( 1024 );
}


HidOutManager::~HidOutManager()
{
    // yeah right
}


t_CKBOOL HidOutManager::open( HidOut * hout, t_CKINT device_num )
{
    // see if port not already open
    if( device_num >= (t_CKINT)the_phouts.capacity() || !the_phouts[device_num] )
    {
        // allocate
        PhyHidDevOut * phout = new PhyHidDevOut;
        /*
        try {
            rtmout->openPort( device_num );
        } catch( RtError & err ) {
            if( !mout->m_suppress_output )
            {
                // print it
                EM_error2( 0, "MidiOut: couldn't open MIDI port %i...", device_num );
                err.getMessage();
                // const char * e = err.getMessage().c_str();
                // EM_error2( 0, "...(%s)", err.getMessage().c_str() );
            }
            return FALSE;
        }*/

        // resize?
        if( device_num >= (t_CKINT)the_phouts.capacity() )
        {
            t_CKINT size = the_phouts.capacity() * 2;
            if( device_num >= size ) size = device_num + 1;
            the_phouts.resize( size );
        }

        // put rtmout in vector for future generations
        the_phouts[device_num] = phout;
    }

    // found (always) (except when it doesn't get here)
    hout->phout = the_phouts[device_num];
    hout->m_device_num = (t_CKUINT)device_num;

    // done
    return TRUE;
}
