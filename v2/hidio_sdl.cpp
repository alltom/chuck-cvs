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
#include "util_hid.h"
#include "chuck_errmsg.h"
#include <limits.h>
#include <vector>
#include <map>
using namespace std;


struct PhyHidDevIn
{
public:
    PhyHidDevIn();
    ~PhyHidDevIn();
    t_CKBOOL open( t_CKINT type, t_CKUINT number );
    t_CKBOOL close();
    
    t_CKBOOL register_element( t_CKINT type, t_CKUINT eid );
    t_CKBOOL unregister_element( t_CKINT type, t_CKUINT eid );
    
    t_CKBOOL query_element( HidMsg * query );

    t_CKBOOL register_client( HidIn * client );
    t_CKBOOL unregister_client( HidIn * client );

public:
    CBufferAdvance * cbuf;

protected:
    t_CKUINT refcount;
    
    t_CKUINT ** filter;
    t_CKINT device_type;
    t_CKUINT device_num;
    
    union 
    {
        SDL_Joystick * joystick;
        // kb and mouse data here
    };
    
    std::vector< HidIn * > clients;
};

struct PhyHidDevOut
{
    // lies
};



//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
#define BUFFER_SIZE 8192

std::vector< std::vector<PhyHidDevIn *> > HidInManager::the_matrix;
XThread * HidInManager::the_thread = NULL;
t_CKBOOL HidInManager::thread_going = FALSE;
t_CKBOOL HidInManager::has_init = FALSE;
std::vector<PhyHidDevOut *> HidOutManager::the_phouts;



//-----------------------------------------------------------------------------
// name: PhyHidDevIn()
// desc: constructor
//-----------------------------------------------------------------------------
PhyHidDevIn::PhyHidDevIn()
{
    filter = NULL;
    device_type = CK_HID_DEV_NONE;
    device_num = 0;
    joystick = NULL;
    cbuf = NULL;
}


//-----------------------------------------------------------------------------
// name: ~PhyHidDevIn()
// desc: destructor
//-----------------------------------------------------------------------------
PhyHidDevIn::~PhyHidDevIn()
{
    // check
    if( device_type != CK_HID_DEV_NONE )
        this->close();
}


//-----------------------------------------------------------------------------
// name: open()
// desc: opens the device of specified type and id
//-----------------------------------------------------------------------------
t_CKBOOL PhyHidDevIn::open( t_CKINT type, t_CKUINT number )
{
    // int temp;

    // check
    if( device_type != CK_HID_DEV_NONE )
    {
        // log
        EM_log( CK_LOG_WARNING, "PhyHidDevIn: open() failed -> already open!" );
        return FALSE;
    }

    switch( type )
    {
        case CK_HID_DEV_JOYSTICK:
            joystick = SDL_JoystickOpen( number );
            if( !joystick )
                return FALSE;
            
            filter = new t_CKUINT * [4];
            filter[0] = NULL;
            filter[1] = NULL;
            filter[2] = NULL;
            filter[3] = NULL;
            
            /*
            temp = SDL_JoystickNumAxes( joystick );
            if( temp > 0 )
                filter[CK_HID_JOYSTICK_AXIS] = new t_CKUINT[temp];
            else if( temp == 0 )
                filter[CK_HID_JOYSTICK_AXIS] = NULL;
            else
            {
                close();
                return FALSE;
            }
                
            temp = SDL_JoystickNumButtons( joystick );
            if( temp > 0 )
                filter[CK_HID_JOYSTICK_BUTTON] = new t_CKUINT[temp];
            else if( temp == 0 )
                filter[CK_HID_JOYSTICK_BUTTON] = NULL;
            else
            {
                close();
                return FALSE;
            }
                
            temp = SDL_JoystickNumHats( joystick );
            if( temp > 0 )
                filter[CK_HID_JOYSTICK_HAT] = new t_CKUINT[temp];
            else if( temp == 0 )
                filter[CK_HID_JOYSTICK_HAT] = NULL;
            else
            {
                close();
                return FALSE;
            }
                
            temp = SDL_JoystickNumBalls( joystick );
            if( temp > 0 )
                filter[CK_HID_JOYSTICK_BALL] = new t_CKUINT[temp];
            else if( temp == 0 )
                filter[CK_HID_JOYSTICK_BALL] = NULL;
            else
            {
                close();
                return FALSE;
            }
            */
                
            break;
            
        case CK_HID_DEV_MOUSE:
        case CK_HID_DEV_KEYBOARD:
            EM_log( CK_LOG_WARNING, "PhyHidDevIn: open operation failed; device-type support incomplete" );
            return FALSE;
            break;
                
        default:
            EM_log( CK_LOG_WARNING, "PhyHidDevIn: open operation failed; unknown device-type" );
            return FALSE;
    }

    // allocate the buffer
    cbuf = new CBufferAdvance;
    if( !cbuf->initialize( BUFFER_SIZE, sizeof(HidMsg) ) )
    {
        // log
        EM_log( CK_LOG_WARNING, "PhyHidDevIn: open operation failed: cannot initialize buffer" );
        this->close();
        return FALSE;
    }

    device_type = type;
    device_num = number;


    return TRUE;
}


//-----------------------------------------------------------------------------
// name: close()
// desc: closes the device, deallocates all associated data
//-----------------------------------------------------------------------------
t_CKBOOL PhyHidDevIn::close()
{
    // check
    if( cbuf != NULL )
    {
        // delete
        SAFE_DELETE( cbuf );
        // TODO: release references from cbuf?
    }

    switch( device_type )
    {
        case CK_HID_DEV_JOYSTICK:
            if( filter != NULL )
            {
                if( filter[0] != NULL )
                    delete[] filter[0];
                if( filter[1] != NULL )
                    delete[] filter[1];
                if( filter[2] != NULL )
                    delete[] filter[2];
                if( filter[3] != NULL )
                    delete[] filter[3];
                delete[] filter;
             
                filter = NULL;
            }            
            
            if( joystick != NULL )
                SDL_JoystickClose( joystick );
            joystick = NULL;
                        
            break;
            
        default:
            EM_log( CK_LOG_WARNING, "PhyHidDevIn: close operation failed; device not opened" );
            return FALSE;
    }
    
    device_type = CK_HID_DEV_NONE;
    device_num = 0;

    return TRUE;
}



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
t_CKBOOL HidIn::open( t_CKINT device_type, t_CKINT device_num )
{
    // close if already opened
    if( m_valid )
        this->close();

    // open
    return m_valid = HidInManager::open( this, device_type, device_num );
}




void HidInManager::init()
{
    assert( has_init == FALSE );

    // allocate the matrix
    the_matrix.resize( CK_HID_DEV_COUNT );
    // resize each vector
    for( vector<vector<PhyHidDevIn *> >::size_type i = 0; i < the_matrix.size(); i++ )
    {
        // allocate
        the_matrix[i].resize( CK_MAX_HID_DEVICES );
    }

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ); // VIDEO is necessary...

    has_init = TRUE;
}


void HidInManager::cleanup()
{
    SDL_Quit();
    // yeah right
}


t_CKBOOL HidInManager::open( HidIn * hin, t_CKINT device_type, t_CKINT device_num )
{
    // check type
    if( device_type < 1 || device_type >= CK_HID_DEV_COUNT )
    {
        // log
        EM_log( CK_LOG_WARNING, "HidInManager: open() failed -> invalid type '%d'...", 
            device_type );
        return FALSE;
    }

    // get the vector
    vector<PhyHidDevIn *> & v = the_matrix[device_type];

    // see if port not already open
    if( device_num >= (t_CKINT)v.capacity() || !v[device_num] )
    {
        // allocate
        PhyHidDevIn * phin = new PhyHidDevIn;
        // open
        if( !phin->open( device_type, device_num ) )
        {
            // log
            EM_error2( 0, "HidIn: couldn't open HID device %d:%d...", device_type, device_num );
            SAFE_DELETE( phin );
            return FALSE;
        }

        // resize?
        if( device_num >= (t_CKINT)v.capacity() )
        {
            t_CKINT size = v.capacity() * 2;
            if( device_num >= size ) size = device_num + 1;
            v.resize( size );
        }

        // put cbuf and rtmin in vector for future generations
        v[device_num] = phin;
    }

    // set min
    hin->phin = v[device_num];
    // found
    hin->m_buffer = v[device_num]->cbuf;
    // get an index into your (you are min here) own buffer, 
    // and a free ticket to your own workshop
    hin->m_read_index = hin->m_buffer->join( (Chuck_Event *)hin->SELF );
    hin->m_device_num = (t_CKUINT)device_num;

    // start thread
    if( the_thread == NULL )
    {
        // allocate
        the_thread = new XThread;
        // flag
        thread_going = TRUE;
        // start
        the_thread->start( cb_hid_input, NULL );
   }

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
#ifndef __PLATFORM_WIN32__
void * HidInManager::cb_hid_input( void * stuff )
#else
unsigned __stdcall HidInManager::cb_hid_input( void * stuff )
#endif 
{
    SDL_Event event;
    HidMsg msg;
    t_CKINT type;
    t_CKINT num;

    // keep going
    while( thread_going )
    {
        // wait for SDL event
        SDL_WaitEvent( &event );
        // clear
        msg.clear();
        // make HID msg
        switch( event.type )
        {
            case SDL_JOYAXISMOTION:
                type = CK_HID_DEV_JOYSTICK;
                num = event.jaxis.which;
                // msg
                msg.type = CK_HID_JOYSTICK_AXIS;
                msg.eid = event.jaxis.axis;
                msg.idata[0] = event.jaxis.value;
                msg.fdata[0] = msg.idata[0] / -((t_CKFLOAT)SHRT_MIN);
                break;
            case SDL_JOYBUTTONUP:
                type = CK_HID_DEV_JOYSTICK;
                num = event.jbutton.which;
                // msg
                msg.type = CK_HID_JOYSTICK_BUTTON_UP;
                msg.eid = event.jbutton.button;
                break;
            case SDL_JOYBUTTONDOWN:
                type = CK_HID_DEV_JOYSTICK;
                num = event.jbutton.which;
                // msg
                msg.type = CK_HID_JOYSTICK_BUTTON_DOWN;
                msg.eid = event.jbutton.button;
                break;
            case SDL_JOYHATMOTION:
                type = CK_HID_DEV_JOYSTICK;
                num = event.jhat.which;
                // msg
                msg.type = CK_HID_JOYSTICK_HAT;
                msg.eid = event.jhat.hat;
                msg.idata[0] = event.jhat.value;
                break;
            case SDL_QUIT:
                thread_going = FALSE;
                break;
            default:
                // log
                EM_log( CK_LOG_WARNING, "HID: unsupported event type '%d'...", event.type );
                continue;
        }

        // find the queue
        CBufferAdvance * cbuf = the_matrix[type][num]->cbuf;
        assert( cbuf != NULL );
        // queue the thing
        cbuf->put( &msg, 1 );
    }

    return 0;
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


