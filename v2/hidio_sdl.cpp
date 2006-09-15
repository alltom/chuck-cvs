
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

#ifndef __PLATFORM_WIN32__
#include <unistd.h>
#else
#include "chuck_def.h"
#endif

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
    
    t_CKBOOL query_element( HidMsg * query );

    t_CKBOOL register_client( HidIn * client );
    t_CKBOOL unregister_client( HidIn * client );

public:
    CBufferAdvance * cbuf;

protected:    
    t_CKINT device_type;
    t_CKUINT device_num;
    
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
CBufferSimple * HidInManager::msg_buffer = NULL;
std::vector<PhyHidDevOut *> HidOutManager::the_phouts;

//-----------------------------------------------------------------------------
// name: PhyHidDevIn()
// desc: constructor
//-----------------------------------------------------------------------------
PhyHidDevIn::PhyHidDevIn()
{
    device_type = CK_HID_DEV_NONE;
    device_num = 0;
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
            if( Joystick_open( (int) number ) )
            {
                EM_log( CK_LOG_WARNING, "PhyHidDevIn: open() failed -> invalid joystick number %d", number );
                return FALSE;
            }            
                
            break;
            
        case CK_HID_DEV_MOUSE:
            if( Mouse_open( (int) number ) )
            {
                EM_log( CK_LOG_WARNING, "PhyHidDevIn: open() failed -> invalid mouse number %d", number );
                return FALSE;
            }
            
            break;
            
        case CK_HID_DEV_KEYBOARD:
            if( Keyboard_open( (int) number ) )
            {
                EM_log( CK_LOG_WARNING, "PhyHidDevIn: open() failed -> invalid keyboard number %d", number );
                return FALSE;
            }
            
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
            Joystick_close( device_num );
                        
            break;
            
        case CK_HID_DEV_MOUSE:
            Mouse_close( device_num );
            break;
            
        case CK_HID_DEV_KEYBOARD:
            Keyboard_close( device_num );
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
    // log
    EM_log( CK_LOG_INFO, "initializing HID..." );
	EM_pushlog();
    if( has_init == FALSE )
    {
        // allocate the matrix
        the_matrix.resize( CK_HID_DEV_COUNT );
        // resize each vector
        for( vector<vector<PhyHidDevIn *> >::size_type i = 0; i < the_matrix.size(); i++ )
        {
            // allocate
            the_matrix[i].resize( CK_MAX_HID_DEVICES );
        }
        
        msg_buffer = new CBufferSimple;
        msg_buffer->initialize( 1000, sizeof( HidMsg ) );
        
        Hid_init();
        
        Joystick_init();
        Mouse_init();
        Keyboard_init();

        has_init = TRUE;
    }

	EM_poplog();
}


void HidInManager::cleanup()
{
    // log
    EM_log( CK_LOG_INFO, "shutting down HID..." );

    /*
    // flag
    thread_going = FALSE;
     
    // break Hid_wait();
    Hid_quit();

	// clean up
    if( the_thread != NULL )
        SAFE_DELETE( the_thread );

	// clean up subsystems
    Joystick_quit();
    Mouse_quit();
    Keyboard_quit();

    if( msg_buffer )
    {
        msg_buffer->cleanup();
        SAFE_DELETE( msg_buffer );
    }
     
    // init
    has_init = FALSE;
    */

    // loop
    for( vector<vector<PhyHidDevIn *> >::size_type i = 0; i < the_matrix.size(); i++ )
    {
        // loop
        for( vector<PhyHidDevIn *>::size_type j = 0; j < the_matrix[i].size(); j++ )
        {
            // deallocate devices
            SAFE_DELETE( the_matrix[i][j] );
        }
    }
}


t_CKBOOL HidInManager::open( HidIn * hin, t_CKINT device_type, t_CKINT device_num )
{
    // init?
    if( has_init == FALSE )
    {
        init();
    }

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
// desc: called by device implementations to push a message onto the buffer
//-----------------------------------------------------------------------------
void HidInManager::push_message( HidMsg & msg )
{
    // find the queue
    if( the_matrix[msg.device_type][msg.device_num] != NULL )
    {
        CBufferAdvance * cbuf = the_matrix[msg.device_type][msg.device_num]->cbuf;
        if( cbuf != NULL )
            // queue the thing
            cbuf->put( &msg, 1 );
    }
}

extern "C" void push_message( HidMsg msg )
{
    HidInManager::push_message( msg );
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
    HidMsg msg;

    // keep going
    while( thread_going )
    {
        Hid_poll();
    }
    
    // log
    EM_log( CK_LOG_INFO, "HID thread exiting..." );

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


