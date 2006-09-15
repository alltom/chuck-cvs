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
// file: hidio_sdl.h
// desc: HID io over SDL header
//
// author: Spencer Salazar (ssalazar@cs.princeton.edu)
//         Ge Wang (gewang@cs.princeton.edu)
//         Ananya Misra (amisra@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Summer 2005
//-----------------------------------------------------------------------------
#ifndef __HID_IO_H__
#define __HID_IO_H__

#ifdef __cplusplus
#include "chuck_def.h"
#include "util_buffers.h"
#include "util_thread.h"
#endif

// forward reference
struct PhyHidDevIn;
struct PhyHidDevOut;

//-----------------------------------------------------------------------------
// definitions
//-----------------------------------------------------------------------------
struct HidMsg
{
    t_CKINT device_type; // device type
    t_CKINT device_num;  // device number
    t_CKINT type;        // message type
    t_CKINT eid;         // element id
    t_CKINT idata[4];    // int data
    t_CKFLOAT fdata[4];  // float data
    
#ifdef __cplusplus
    HidMsg()
    { this->clear(); }

    void clear()
    {
        memset( this, 0, sizeof(HidMsg) );
    }
#endif
};


/* device types */
static const t_CKUINT CK_HID_DEV_NONE = 0;
static const t_CKUINT CK_HID_DEV_JOYSTICK = 1;
static const t_CKUINT CK_HID_DEV_MOUSE = 2;
static const t_CKUINT CK_HID_DEV_KEYBOARD = 3;
static const t_CKUINT CK_HID_DEV_COUNT = 4;

/* message types */
static const t_CKUINT CK_HID_JOYSTICK_AXIS = 0;
static const t_CKUINT CK_HID_BUTTON_DOWN = 1;
static const t_CKUINT CK_HID_BUTTON_UP = 2;
static const t_CKUINT CK_HID_JOYSTICK_HAT = 3;
static const t_CKUINT CK_HID_JOYSTICK_BALL = 4;
static const t_CKUINT CK_HID_MOUSE_MOTION = 5;
static const t_CKUINT CK_HID_MOUSE_WHEEL = 6;
static const t_CKUINT CK_HID_MSG_COUNT = 7;

/* constants */
#define CK_MAX_HID_DEVICES 1024

#ifdef __cplusplus

//-----------------------------------------------------------------------------
// name: struct HidOut
// desc: HID out
//-----------------------------------------------------------------------------
class HidOut
{
public:
    HidOut();
    ~HidOut();

public:
    t_CKBOOL open( t_CKUINT device_num = 0 );
    t_CKBOOL close();
    t_CKBOOL good() { return m_valid; }
    t_CKINT  num() { return m_valid ? (t_CKINT)m_device_num : -1; }
    
public:
    void     set_suppress( t_CKBOOL print_or_not )
    { m_suppress_output = print_or_not; }
    t_CKBOOL get_suppress()
    { return m_suppress_output; }

public:
    t_CKUINT send( const HidMsg * msg );

public:
    PhyHidDevOut * phout;
    std::vector<unsigned char> m_msg;
    t_CKUINT m_device_num;
    t_CKBOOL m_valid;
    t_CKBOOL m_suppress_output;
};




//-----------------------------------------------------------------------------
// name: class HidIn
// desc: HID input
//-----------------------------------------------------------------------------
struct HidIn : public Chuck_Event
{
public:
    HidIn();
    ~HidIn();

public:
    t_CKBOOL open( t_CKINT device_type, t_CKINT device_num );
    t_CKBOOL close();
    t_CKBOOL good() { return m_valid; }
    t_CKINT  num() { return m_valid ? (t_CKINT)m_device_num : -1; }

public:
    void     set_suppress( t_CKBOOL print_or_not )
    { m_suppress_output = print_or_not; }
    t_CKBOOL get_suppress()
    { return m_suppress_output; }

public:
    t_CKBOOL empty();
    t_CKUINT recv( HidMsg * msg );

public:
    PhyHidDevIn * phin;
    CBufferAdvance * m_buffer;
    t_CKUINT m_read_index;
    t_CKBOOL m_valid;
    t_CKINT m_device_num;
    Chuck_Object * SELF;
    t_CKBOOL m_suppress_output;
};


void probeHidIn();
void probeHidOut();


class HidInManager
{
public:
    static void init();
    static void cleanup();
    static t_CKBOOL open( HidIn * hin, t_CKINT device_type, t_CKINT device_num );
    static t_CKBOOL close( HidIn * hin );

#ifndef __PLATFORM_WIN32__
    static void * cb_hid_input( void * );
#else
    static unsigned __stdcall cb_hid_input( void * );
#endif

    static void push_message( HidMsg & msg );
        
protected:
    static std::vector< std::vector<PhyHidDevIn *> > the_matrix;
    static XThread * the_thread;
    static CBufferSimple * msg_buffer;
    static t_CKBOOL thread_going;
    static t_CKBOOL has_init;
};


class HidOutManager
{
public:
    static t_CKBOOL open( HidOut * mout, t_CKINT device_num );
    static t_CKBOOL close( HidOut * mout );

protected:
    HidOutManager();
    ~HidOutManager();

    static std::vector<PhyHidDevOut *> the_phouts;
};

#endif


#endif
