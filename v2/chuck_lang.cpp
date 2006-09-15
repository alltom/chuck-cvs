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
// name: chuck_lang.cpp
// desc: chuck class library base
//
// authors: Ge Wang (gewang@cs.princeton.edu)
//          Perry R. Cook (prc@cs.princeton.edu)
//    date: spring 2005
//-----------------------------------------------------------------------------
#include "chuck_lang.h"
#include "chuck_type.h"
#include "chuck_instr.h"
#include "chuck_vm.h"
#include "chuck_errmsg.h"
#include "midiio_rtmidi.h"
#include "hidio_sdl.h"
// #include "skiniio_skini.h"


// offset for member variable
static t_CKUINT object_offset_m_testID = CK_INVALID_OFFSET;

// storage for static variable
static t_CKINT object_our_testID = 0;

// dac tick
UGEN_TICK __ugen_tick( Chuck_Object * SELF, SAMPLE in, SAMPLE * out ) 
{ *out = in; return TRUE; }


//-----------------------------------------------------------------------------
// name: init_class_object()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_object( Chuck_Env * env, Chuck_Type * type )
{
    Chuck_DL_Func * func = NULL;

    // log
    EM_log( CK_LOG_SEVERE, "class 'object'" );

    // init as base class
    if( !type_engine_import_class_begin( env, type, env->global(), object_ctor ) )
        return FALSE;

    // add setTestID()
    func = make_new_mfun( "int", "testID", object_setTestID );
    func->add_arg( "int", "id" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add getTestID()
    func = make_new_mfun( "int", "testID", object_getTestID );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add getTest()
    func = make_new_sfun( "int", "testStatic", object_testStatic );
    func->add_arg( "float", "i" );
    if( !type_engine_import_sfun( env, func ) ) goto error;

    // add member variable
    object_offset_m_testID = type_engine_import_mvar( env, "int", "m_testID", FALSE );
    if( object_offset_m_testID == CK_INVALID_OFFSET ) goto error;

    // add static variable
    if( !type_engine_import_svar( env, "int", "our_testID", FALSE, (t_CKUINT)&object_our_testID ) )
        goto error;

    // end the class import
    type_engine_import_class_end( env );
    
    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}




//-----------------------------------------------------------------------------
// name: init_class_ugen()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_ugen( Chuck_Env * env, Chuck_Type * type )
{
    Chuck_DL_Func * func = NULL;

    EM_log( CK_LOG_SEVERE, "class 'ugen'" );

    //the type argument IS t_ugen here - there's no other use of this function
    //why are we using t_ugen here instead of type ( or vice versa ) ? PLD
    // add ugen info

    t_ugen.ugen_info = new Chuck_UGen_Info;
    t_ugen.ugen_info->add_ref();
    t_ugen.ugen_info->tick = __ugen_tick;
    t_ugen.ugen_info->num_ins = 1;
    t_ugen.ugen_info->num_outs = 1;

    // init as base class
    if( !type_engine_import_class_begin( env, type, env->global(), NULL ) )
        return FALSE;

    // add setTestID()
    //func = make_new_mfun( "int", "testID", ugen_setTestID );
    //func->add_arg( "int", "id" );
    //if( !type_engine_import_mfun( env, func ) ) goto error;

    // add getTestID()
    func = make_new_mfun( "int", "testID", ugen_getTestID );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // gain
    func = make_new_mfun( "float", "gain", ugen_gain );
    func->add_arg( "float", "val" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    func = make_new_mfun( "float", "gain", ugen_cget_gain );
    if( !type_engine_import_mfun( env, func ) ) goto error;

/*    // next
    func = make_new_mfun( "float", "next", ugen_next );
    func->add_arg( "float", "val" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    func = make_new_mfun( "float", "next", ugen_cget_next );
    if( !type_engine_import_mfun( env, func ) ) goto error;
*/
    // last
    func = make_new_mfun( "float", "last", ugen_last );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // op
    func = make_new_mfun( "int", "op", ugen_op );
    func->add_arg( "int", "val" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    func = make_new_mfun( "int", "op", ugen_cget_op );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add numChannels
    func = make_new_mfun( "int", "channels", ugen_numChannels );
    func->add_arg( "int", "num" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    func = make_new_mfun( "int", "channels", ugen_cget_numChannels );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add chan
    func = make_new_mfun( "UGen", "chan", ugen_chan );
    func->add_arg( "int", "num" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end
    type_engine_import_class_end( env );

    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );

    return FALSE;
}




// static t_CKUINT event_offset_data = 0;
//-----------------------------------------------------------------------------
// name: init_class_event()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_event( Chuck_Env * env, Chuck_Type * type )
{
    Chuck_DL_Func * func = NULL;
    Chuck_Value * value = NULL;

    // log
    EM_log( CK_LOG_SEVERE, "class 'event'" );

    // init as base class
    if( !type_engine_import_class_begin( env, type, env->global(), event_ctor ) )
        return FALSE;

    // add signal()
    func = make_new_mfun( "void", "signal", event_signal );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add broadcast()
    func = make_new_mfun( "void", "broadcast", event_broadcast );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add wait()
    func = make_new_mfun( "void", "wait", event_wait );
    func->add_arg( "Shred", "me" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add can_wait()
    func = make_new_mfun( "int", "can_wait", event_can_wait );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );

    // find the offset for can_wait
    value = type_engine_find_value( type, "can_wait" );
    assert( value != NULL );
    assert( value->func_ref != NULL );
    // remember it
    Chuck_Event::our_can_wait = value->func_ref->vt_index;
    
    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}




static t_CKUINT shred_offset_args = 0;
//-----------------------------------------------------------------------------
// name: init_class_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_shred( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
    Chuck_DL_Func * func = NULL;

    // log
    EM_log( CK_LOG_SEVERE, "class 'shred'" );

    // init as base class
    if( !type_engine_import_class_begin( env, type, env->global(), NULL ) )
        return FALSE;
    
    // add dtor
    // not

    // add clone()
    func = make_new_mfun( "void", "clone", shred_clone );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add exit()
    func = make_new_mfun( "void", "exit", shred_exit );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add id()
    func = make_new_mfun( "int", "id", shred_id );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add yield()
    func = make_new_mfun( "void", "yield", shred_yield );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add args
    shred_offset_args = type_engine_import_mvar( env, "string[]", "args", TRUE );
    if( shred_offset_args == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );
    
    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}




//-----------------------------------------------------------------------------
// name: init_class_string()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_string( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
    Chuck_DL_Func * func = NULL;

    // log
    EM_log( CK_LOG_SEVERE, "class 'string'" );

    // init as base class
    if( !type_engine_import_class_begin( env, type, env->global(), NULL ) )
        return FALSE;
    
    // add dtor
    // not

    // add length()
    func = make_new_mfun( "int", "length", string_length );
    if( !type_engine_import_mfun( env, func ) ) goto error;

/*    // add at()
    func = make_new_mfun( "int", "at", string_set_at );
    func->add_arg( "int", "ch" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    func = make_new_mfun( "int", "at", string_get_at );
    if( !type_engine_import_mfun( env, func ) ) goto error;
*/
    // end the class import
    type_engine_import_class_end( env );
    
    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}




//-----------------------------------------------------------------------------
// name: init_class_array()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_array( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
    Chuck_DL_Func * func = NULL;

    // log
    EM_log( CK_LOG_SEVERE, "class 'array'" );

    // init as base class
    if( !type_engine_import_class_begin( env, type, env->global(), NULL ) )
        return FALSE;

    // add size()
    func = make_new_mfun( "int", "size", array_size );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add size()
    func = make_new_mfun( "int", "cap", array_capacity );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add find()
    func = make_new_mfun( "int", "find", array_find );
    func->add_arg( "string", "key" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add erase()
    func = make_new_mfun( "int", "erase", array_erase );
    func->add_arg( "string", "key" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    type_engine_import_class_end( env );

    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}




// static
static t_CKUINT MidiIn_offset_data = 0;
static t_CKUINT MidiMsg_offset_data1 = 0;
static t_CKUINT MidiMsg_offset_data2 = 0;
static t_CKUINT MidiMsg_offset_data3 = 0;
static t_CKUINT MidiMsg_offset_when = 0;
static t_CKUINT MidiOut_offset_data = 0;

//-----------------------------------------------------------------------------
// name: init_class_Midi()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_Midi( Chuck_Env * env )
{
    Chuck_DL_Func * func = NULL;

    // init base class
    if( !type_engine_import_class_begin( env, "MidiMsg", "Object",
                                         env->global(), NULL ) )
        return FALSE;

    // add member variable
    MidiMsg_offset_data1 = type_engine_import_mvar( env, "int", "data1", FALSE );
    if( MidiMsg_offset_data1 == CK_INVALID_OFFSET ) goto error;

    // add member variable
    MidiMsg_offset_data2 = type_engine_import_mvar( env, "int", "data2", FALSE );
    if( MidiMsg_offset_data2 == CK_INVALID_OFFSET ) goto error;

    // add member variable
    MidiMsg_offset_data3 = type_engine_import_mvar( env, "int", "data3", FALSE );
    if( MidiMsg_offset_data3 == CK_INVALID_OFFSET ) goto error;

    // add member variable
    MidiMsg_offset_when = type_engine_import_mvar( env, "time", "when", FALSE );
    if( MidiMsg_offset_when == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );


    // init base class
    if( !type_engine_import_class_begin( env, "MidiIn", "Event",
                                         env->global(), MidiIn_ctor ) )
        return FALSE;

    // add open()
    func = make_new_mfun( "int", "open", MidiIn_open );
    func->add_arg( "int", "port" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add good()
    func = make_new_mfun( "int", "good", MidiIn_good );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add num()
    func = make_new_mfun( "int", "num", MidiIn_num );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add name()
    func = make_new_mfun( "string", "name", MidiIn_name );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add printerr()
    func = make_new_mfun( "void", "printerr", MidiIn_printerr );
    func->add_arg( "int", "print_or_not" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add recv()
    func = make_new_mfun( "int", "recv", MidiIn_recv );
    func->add_arg( "MidiMsg", "msg" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add can_wait()
    func = make_new_mfun( "int", "can_wait", MidiIn_can_wait );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    MidiIn_offset_data = type_engine_import_mvar( env, "int", "@MidiIn_data", FALSE );
    if( MidiIn_offset_data == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );
    
    // init base class
    if( !type_engine_import_class_begin( env, "MidiOut", "Object",
                                         env->global(), MidiOut_ctor ) )
        return FALSE;

    // add open()
    func = make_new_mfun( "int", "open", MidiOut_open );
    func->add_arg( "int", "port" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add good()
    func = make_new_mfun( "int", "good", MidiOut_good );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add num()
    func = make_new_mfun( "int", "num", MidiOut_num );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add name()
    func = make_new_mfun( "string", "name", MidiOut_name );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add printerr()
    func = make_new_mfun( "void", "printerr", MidiOut_printerr );
    func->add_arg( "int", "print_or_not" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add send()
    func = make_new_mfun( "int", "send", MidiOut_send );
    func->add_arg( "MidiMsg", "msg" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    MidiOut_offset_data = type_engine_import_mvar( env, "int", "@MidiOut_data", FALSE );
    if( MidiOut_offset_data == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );
    
    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}




// static
static t_CKUINT HidIn_offset_data = 0;
static t_CKUINT HidIn_offset_joystick = 0;
static t_CKUINT HidIn_offset_mouse = 0;
static t_CKUINT HidIn_offset_keyboard = 0;
static t_CKUINT HidIn_offset_axis_motion = 0;
static t_CKUINT HidIn_offset_button_down = 0;
static t_CKUINT HidIn_offset_button_up = 0;
static t_CKUINT HidIn_offset_joystick_hat = 0;
static t_CKUINT HidIn_offset_joystick_ball = 0;
static t_CKUINT HidIn_offset_mouse_motion = 0;
static t_CKUINT HidIn_offset_mouse_wheel = 0;

static t_CKUINT HidMsg_offset_device_type = 0;
static t_CKUINT HidMsg_offset_device_num = 0;
static t_CKUINT HidMsg_offset_type = 0;
static t_CKUINT HidMsg_offset_which = 0;
static t_CKUINT HidMsg_offset_idata = 0;
static t_CKUINT HidMsg_offset_fdata = 0;
static t_CKUINT HidMsg_offset_when = 0;
static t_CKUINT HidMsg_offset_deltax = 0;
static t_CKUINT HidMsg_offset_deltay = 0;
static t_CKUINT HidMsg_offset_axis_position = 0; // deprecated
static t_CKUINT HidMsg_offset_axis_position2 = 0;
static t_CKUINT HidMsg_offset_scaled_axis_position = 0; // deprecated
static t_CKUINT HidMsg_offset_hat_position = 0;
static t_CKUINT HidMsg_offset_x = 0;
static t_CKUINT HidMsg_offset_y = 0;
static t_CKUINT HidMsg_offset_scaled_x = 0;
static t_CKUINT HidMsg_offset_scaled_y = 0;

static t_CKUINT HidOut_offset_data = 0;

//-----------------------------------------------------------------------------
// name: init_class_HID()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_HID( Chuck_Env * env )
{
    Chuck_DL_Func * func = NULL;

    // init base class
    if( !type_engine_import_class_begin( env, "HidMsg", "Object",
                                         env->global(), NULL ) )
        return FALSE;

    // add member variable
    HidMsg_offset_device_type = type_engine_import_mvar( env, "int", "deviceType", FALSE );
    if( HidMsg_offset_device_type == CK_INVALID_OFFSET ) goto error;
    
    // add member variable
    HidMsg_offset_device_num = type_engine_import_mvar( env, "int", "deviceNum", FALSE );
    if( HidMsg_offset_device_num == CK_INVALID_OFFSET ) goto error;
    
    // add member variable
    HidMsg_offset_type = type_engine_import_mvar( env, "int", "type", FALSE );
    if( HidMsg_offset_type == CK_INVALID_OFFSET ) goto error;
    
    // add member variable
    HidMsg_offset_which = type_engine_import_mvar( env, "int", "which", FALSE );
    if( HidMsg_offset_which == CK_INVALID_OFFSET ) goto error;
    
    // add member variable
    HidMsg_offset_idata = type_engine_import_mvar( env, "int", "idata", FALSE );
    if( HidMsg_offset_idata == CK_INVALID_OFFSET ) goto error;

    // add member variable
    HidMsg_offset_fdata = type_engine_import_mvar( env, "float", "fdata", FALSE );
    if( HidMsg_offset_fdata == CK_INVALID_OFFSET ) goto error;

    // add member variable
    HidMsg_offset_when = type_engine_import_mvar( env, "time", "when", FALSE );
    if( HidMsg_offset_when == CK_INVALID_OFFSET ) goto error;

    // add member variable
    HidMsg_offset_deltax = type_engine_import_mvar( env, "int", "deltaX", FALSE );
    if( HidMsg_offset_deltax == CK_INVALID_OFFSET ) goto error;
    
    // add member variable
    HidMsg_offset_deltay = type_engine_import_mvar( env, "int", "deltaY", FALSE );
    if( HidMsg_offset_deltay == CK_INVALID_OFFSET ) goto error;
    
    // add member variable
    HidMsg_offset_axis_position = type_engine_import_mvar( env, "int", "axis_position", FALSE );
    if( HidMsg_offset_axis_position == CK_INVALID_OFFSET ) goto error;
    
    // add member variable
    HidMsg_offset_axis_position2 = type_engine_import_mvar( env, "float", "axisPosition", FALSE );
    if( HidMsg_offset_axis_position2 == CK_INVALID_OFFSET ) goto error;
    
    // add member variable
    HidMsg_offset_scaled_axis_position = type_engine_import_mvar( env, "float", "scaled_axis_position", FALSE );
    if( HidMsg_offset_scaled_axis_position == CK_INVALID_OFFSET ) goto error;
        
    // add is_axis_motion()
    func = make_new_mfun( "int", "is_axis_motion", HidMsg_is_axis_motion ); // deprecated
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add isAxisMotion()
    func = make_new_mfun( "int", "isAxisMotion", HidMsg_is_axis_motion );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add is_button_down()
    func = make_new_mfun( "int", "is_button_down", HidMsg_is_button_down ); // deprecated
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add isButtonDown()
    func = make_new_mfun( "int", "isButtonDown", HidMsg_is_button_down );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add is_button_up()
    func = make_new_mfun( "int", "is_button_up", HidMsg_is_button_up ); // deprecated
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add isButtonUp()
    func = make_new_mfun( "int", "isButtonUp", HidMsg_is_button_up );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add is_mouse_motion()
    func = make_new_mfun( "int", "is_mouse_motion", HidMsg_is_mouse_motion ); // deprecated
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add isMouseMotion()
    func = make_new_mfun( "int", "isMouseMotion", HidMsg_is_mouse_motion );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add is_hat_motion()
    func = make_new_mfun( "int", "is_hat_motion", HidMsg_is_hat_motion ); // deprecated
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add isHatMotion()
    func = make_new_mfun( "int", "isHatMotion", HidMsg_is_hat_motion );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );


    // init base class
    if( !type_engine_import_class_begin( env, "HidIn", "Event",
                                         env->global(), HidIn_ctor ) )
        return FALSE;

    // add open()
    func = make_new_mfun( "int", "open", HidIn_open );
    func->add_arg( "int", "type" );
    func->add_arg( "int", "num" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add openJoystick()
    func = make_new_mfun( "int", "openJoystick", HidIn_open_joystick );
    func->add_arg( "int", "num" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add openMouse()
    func = make_new_mfun( "int", "openMouse", HidIn_open_mouse );
    func->add_arg( "int", "num" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add openKeyboard()
    func = make_new_mfun( "int", "openKeyboard", HidIn_open_keyboard );
    func->add_arg( "int", "num" );
    if( !type_engine_import_mfun( env, func ) ) goto error;
    
    // add good()
    func = make_new_mfun( "int", "good", HidIn_good );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add num()
    func = make_new_mfun( "int", "num", HidIn_num );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add name()
    func = make_new_mfun( "string", "name", HidIn_name );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add printerr()
    func = make_new_mfun( "void", "printerr", HidIn_printerr );
    func->add_arg( "int", "print_or_not" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add recv()
    func = make_new_mfun( "int", "recv", HidIn_recv );
    func->add_arg( "HidMsg", "msg" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add can_wait()
    func = make_new_mfun( "int", "can_wait", HidIn_can_wait );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    HidIn_offset_data = type_engine_import_mvar( env, "int", "@HidIn_data", FALSE );
    if( HidIn_offset_data == CK_INVALID_OFFSET ) goto error;
    
    // add member variable JOYSTICK
    HidIn_offset_joystick = type_engine_import_mvar( env, "int", "JOYSTICK", FALSE ); 
    if( HidIn_offset_joystick == CK_INVALID_OFFSET ) goto error;
    
    // add member variable KEYBOARD
    HidIn_offset_keyboard = type_engine_import_mvar( env, "int", "KEYBOARD", FALSE ); 
    if( HidIn_offset_keyboard == CK_INVALID_OFFSET ) goto error;
    
    // add member variable MOUSE
    HidIn_offset_mouse = type_engine_import_mvar( env, "int", "MOUSE", FALSE ); 
    if( HidIn_offset_mouse == CK_INVALID_OFFSET ) goto error;
    
    // add member variable AXIS_MOTION
    HidIn_offset_axis_motion = type_engine_import_mvar( env, "int", "AXIS_MOTION", FALSE ); 
    if( HidIn_offset_axis_motion == CK_INVALID_OFFSET ) goto error;
    
    // add member variable BUTTON_DOWN
    HidIn_offset_button_down = type_engine_import_mvar( env, "int", "BUTTON_DOWN", FALSE ); 
    if( HidIn_offset_button_down == CK_INVALID_OFFSET ) goto error;
    
    // add member variable BUTTON_UP
    HidIn_offset_button_up = type_engine_import_mvar( env, "int", "BUTTON_UP", FALSE ); 
    if( HidIn_offset_button_up == CK_INVALID_OFFSET ) goto error;
    
    // add member variable JOYSTICK_HAT
    HidIn_offset_joystick_hat = type_engine_import_mvar( env, "int", "JOYSTICK_HAT", FALSE ); 
    if( HidIn_offset_joystick_hat == CK_INVALID_OFFSET ) goto error;
    
    // add member variable JOYSTICK_BALL
    HidIn_offset_joystick_ball = type_engine_import_mvar( env, "int", "JOYSTICK_BALL", FALSE ); 
    if( HidIn_offset_joystick_ball == CK_INVALID_OFFSET ) goto error;
    
    // add member variable MOUSE_MOTION
    HidIn_offset_mouse_motion = type_engine_import_mvar( env, "int", "MOUSE_MOTION", FALSE ); 
    if( HidIn_offset_mouse_motion == CK_INVALID_OFFSET ) goto error;
    
    // add member variable MOUSE_WHEEL
    HidIn_offset_mouse_wheel = type_engine_import_mvar( env, "int", "MOUSE_WHEEL", FALSE ); 
    if( HidIn_offset_mouse_wheel == CK_INVALID_OFFSET ) goto error;
    
    // end the class import
    type_engine_import_class_end( env );
    
    // init base class
    if( !type_engine_import_class_begin( env, "HidOut", "Object",
                                         env->global(), HidOut_ctor ) )
        return FALSE;

    // add open()
    func = make_new_mfun( "int", "open", HidOut_open );
    func->add_arg( "int", "port" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add good()
    func = make_new_mfun( "int", "good", HidOut_good );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add num()
    func = make_new_mfun( "int", "num", HidOut_num );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add name()
    func = make_new_mfun( "string", "name", HidOut_name );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add printerr()
    func = make_new_mfun( "void", "printerr", HidOut_printerr );
    func->add_arg( "int", "print_or_not" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add send()
    func = make_new_mfun( "int", "send", HidOut_send );
    func->add_arg( "HidMsg", "msg" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    HidOut_offset_data = type_engine_import_mvar( env, "int", "@HidOut_data", FALSE );
    if( HidOut_offset_data == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );
    
    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}




// static
static t_CKUINT MidiRW_offset_data = 0;
static t_CKUINT MidiMsgOut_offset_data = 0;
static t_CKUINT MidiMsgIn_offset_data = 0;
//-----------------------------------------------------------------------------
// name: init_class_MidiRW()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_MidiRW( Chuck_Env * env )
{
    Chuck_DL_Func * func = NULL;

    // init base class
    if( !type_engine_import_class_begin( env, "MidiRW", "Object",
                                         env->global(), MidiRW_ctor ) )
        return FALSE;

    // add open()
    func = make_new_mfun( "int", "open", MidiRW_open );
    func->add_arg( "string", "filename" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add close()
    func = make_new_mfun( "int", "close", MidiRW_close );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add write()
    func = make_new_mfun( "int", "write", MidiRW_write );
    func->add_arg( "MidiMsg", "msg" );
    func->add_arg( "time", "t" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add read()
    func = make_new_mfun( "int", "read", MidiRW_read );
    func->add_arg( "MidiMsg", "msg" );
    //func->add_arg( "time", "t" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    MidiRW_offset_data = type_engine_import_mvar( env, "int", "@MidiRW_data", FALSE );
    if( MidiRW_offset_data == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );
    
        // init base class
    if( !type_engine_import_class_begin( env, "MidiMsgOut", "Object",
                                         env->global(), MidiMsgOut_ctor ) )
        return FALSE;

    // add open()
    func = make_new_mfun( "int", "open", MidiMsgOut_open );
    func->add_arg( "string", "filename" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add close()
    func = make_new_mfun( "int", "close", MidiMsgOut_close );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add write()
    func = make_new_mfun( "int", "write", MidiMsgOut_write );
    func->add_arg( "MidiMsg", "msg" );
    func->add_arg( "time", "t" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    MidiMsgOut_offset_data = type_engine_import_mvar( env, "int", "@MidiMsgOut_data", FALSE );
    if( MidiMsgOut_offset_data == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );

        // init base class
    if( !type_engine_import_class_begin( env, "MidiMsgIn", "Object",
                                         env->global(), MidiMsgIn_ctor ) )
        return FALSE;

    // add open()
    func = make_new_mfun( "int", "open", MidiMsgIn_open );
    func->add_arg( "string", "filename" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add close()
    func = make_new_mfun( "int", "close", MidiMsgIn_close );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add read()
    func = make_new_mfun( "int", "read", MidiMsgIn_read );
    func->add_arg( "MidiMsg", "msg" );
    //func->add_arg( "time", "t" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    MidiMsgIn_offset_data = type_engine_import_mvar( env, "int", "@MidiMsgIn_data", FALSE );
    if( MidiMsgIn_offset_data == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );

    // initialize
    // HidInManager::init();

    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}



/*// static
static t_CKUINT SkiniIn_offset_data = 0;
static t_CKUINT SkiniOut_offset_data = 0;
static t_CKUINT SkiniMsg_offset_type = 0;
static t_CKUINT SkiniMsg_offset_time = 0;
static t_CKUINT SkiniMsg_offset_channel = 0;
static t_CKUINT SkiniMsg_offset_data1 = 0;
static t_CKUINT SkiniMsg_offset_data2 = 0;

//-----------------------------------------------------------------------------
// name: init_class_Skini()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_Skini( Chuck_Env * env )
{
    Chuck_DL_Func * func = NULL;

    // init base class
    if( !type_engine_import_class_begin( env, "SkiniMsg", "Object",
                                         env->global(), NULL ) )
        return FALSE;

    // add member variable
    SkiniMsg_offset_type = type_engine_import_mvar( env, "int", "type", FALSE );
    if( SkiniMsg_offset_type == CK_INVALID_OFFSET ) goto error;

    // add member variable
    SkiniMsg_offset_time = type_engine_import_mvar( env, "float", "time", FALSE );
    if( SkiniMsg_offset_time == CK_INVALID_OFFSET ) goto error;

    // add member variable
    SkiniMsg_offset_channel = type_engine_import_mvar( env, "int", "channel", FALSE );
    if( SkiniMsg_offset_channel == CK_INVALID_OFFSET ) goto error;

    // add member variable
    SkiniMsg_offset_data1 = type_engine_import_mvar( env, "float", "data1", FALSE );
    if( SkiniMsg_offset_data1 == CK_INVALID_OFFSET ) goto error;

    // add member variable
    SkiniMsg_offset_data2 = type_engine_import_mvar( env, "float", "data2", FALSE );
    if( SkiniMsg_offset_data2 == CK_INVALID_OFFSET ) goto error;


    // end the class import
    type_engine_import_class_end( env );

    
    // init base class
    if( !type_engine_import_class_begin( env, "SkiniIn", "event",
                                         env->global(), SkiniIn_ctor ) )
        return FALSE;

    // add open()
    func = make_new_mfun( "int", "open", SkiniIn_open );
    func->add_arg( "const char *", "filename" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add recv()
    func = make_new_mfun( "int", "recv", SkiniIn_recv );
    func->add_arg( "SkiniMsg", "msg" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    SkiniIn_offset_data = type_engine_import_mvar( env, "int", "@SkiniIn_data", FALSE );
    if( SkiniIn_offset_data == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );
    
    // init base class
    if( !type_engine_import_class_begin( env, "SkiniOut", "Object",
                                         env->global(), SkiniOut_ctor ) )
        return FALSE;

    // add open()
    func = make_new_mfun( "int", "open", MidiOut_open );
    func->add_arg( "const char *", "filename" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add send()
    func = make_new_mfun( "int", "send", SkiniOut_send );
    func->add_arg( "SkiniMsg", "msg" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add member variable
    SkiniOut_offset_data = type_engine_import_mvar( env, "int", "@SkiniOut_data", FALSE );
    if( SkiniOut_offset_data == CK_INVALID_OFFSET ) goto error;

    // end the class import
    type_engine_import_class_end( env );
    
    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;
}
*/


// Object ctor
CK_DLL_CTOR( object_ctor )
{
//    fprintf( stderr, "object ctor\n" );
}


// Object dtor
CK_DLL_DTOR( object_dtor )
{
//    fprintf( stderr, "object dtor\n" );
}


// setTestID
CK_DLL_MFUN( object_setTestID )
{
    fprintf( stderr, "object set test id\n" );
    t_CKINT v = GET_NEXT_INT(ARGS);
    OBJ_MEMBER_INT(SELF, object_offset_m_testID) = v;
    RETURN->v_int = v;
}


// getTestID
CK_DLL_MFUN( object_getTestID )
{
    fprintf( stderr, "object get test id\n" );
    RETURN->v_int = OBJ_MEMBER_INT(SELF, object_offset_m_testID);
}


// setTestID
CK_DLL_MFUN( ugen_setTestID )
{
    fprintf( stderr, "ugen set test id\n" );
    t_CKINT v = GET_NEXT_INT(ARGS);
    OBJ_MEMBER_INT(SELF, object_offset_m_testID) = v;
    RETURN->v_int = v;
}


// getTestID
CK_DLL_MFUN( ugen_getTestID )
{
    fprintf( stderr, "ugen get test id\n" );
    RETURN->v_int = OBJ_MEMBER_INT(SELF, object_offset_m_testID);
}


// getTest
CK_DLL_SFUN( object_testStatic )
{
    t_CKFLOAT v = GET_NEXT_FLOAT(ARGS);
    fprintf( stderr, "testStatic %f\n", v );
}




CK_DLL_CTOR( ugen_ctor )
{
}

CK_DLL_DTOR( ugen_dtor )
{
    // get as ugen
    // Chuck_UGen * ugen = (Chuck_UGen *)SELF;
}

CK_DLL_MFUN( ugen_op )
{
    // get as ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // get arg
    t_CKINT op = GET_CK_INT( ARGS );
    // set op
    ugen->m_op = op;
    // set return
    RETURN->v_int = ugen->m_op;

    // for multiple channels
    Chuck_DL_Return ret;
    for( t_CKUINT i = 0; i < ugen->m_multi_chan_size; i++ )
        ugen_op( ugen->m_multi_chan[i], ARGS, &ret );
}

CK_DLL_MFUN( ugen_cget_op )
{
    // get as ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // set return
    RETURN->v_int = ugen->m_op;
}

CK_DLL_MFUN( ugen_last )
{
    // get as ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // set return
    RETURN->v_float = (t_CKFLOAT)ugen->m_last;
}

CK_DLL_MFUN( ugen_next )
{
    // get as ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // get arg
    t_CKFLOAT next = GET_CK_FLOAT( ARGS );
    // set op
    ugen->m_next = (SAMPLE)next;
    ugen->m_use_next = TRUE;
    // set return
    RETURN->v_float = (t_CKFLOAT)ugen->m_next;

    // for multiple channels
    Chuck_DL_Return ret;
    for( t_CKUINT i = 0; i < ugen->m_multi_chan_size; i++ )
        ugen_next( ugen->m_multi_chan[i], ARGS, &ret );
}

CK_DLL_MFUN( ugen_cget_next )
{
    // get as ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // set return
    RETURN->v_float = ugen->m_use_next ? (t_CKFLOAT)ugen->m_next : 0.0f;
}

CK_DLL_MFUN( ugen_gain )
{
    // get as ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // get arg
    t_CKFLOAT gain = GET_CK_FLOAT( ARGS );
    // set op
    ugen->m_gain = (SAMPLE)gain;
    // set return
    RETURN->v_float = (t_CKFLOAT)ugen->m_gain;

    // for multiple channels
    Chuck_DL_Return ret;
    for( t_CKUINT i = 0; i < ugen->m_multi_chan_size; i++ )
        ugen_gain( ugen->m_multi_chan[i], ARGS, &ret );
}

CK_DLL_MFUN( ugen_cget_gain )
{
    // get as ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // set return
    RETURN->v_float = (t_CKFLOAT)ugen->m_gain;
}

CK_DLL_CTRL( ugen_numChannels )
{
    // get ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // error
    EM_error3( "setting .numChannels is not yet supported (use the command line)..." );
}

CK_DLL_CGET( ugen_cget_numChannels )
{
    // get ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // return
    RETURN->v_int = ugen->m_multi_chan_size == 0 ? 1 : ugen->m_multi_chan_size;
}

CK_DLL_CTRL( ugen_chan )
{
    // get ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    t_CKINT num = GET_NEXT_INT(ARGS);
    // check
    if( !ugen->m_multi_chan_size && num == 0 )
        RETURN->v_object = ugen;
    else if( num >= 0 && num < ugen->m_multi_chan_size )
        RETURN->v_object = ugen->m_multi_chan[num];
    else
        RETURN->v_object = NULL;
}


CK_DLL_CTOR( event_ctor )
{
//  OBJ_MEMBER_INT(SELF, event_offset_data) = (t_CKUINT)new Data_Event;
//  Chuck_Event * event = (Chuck_Event *)SELF;
}


CK_DLL_DTOR( event_dtor )
{
//  delete (Data_Event *)OBJ_MEMBER_INT(SELF, event_offset_data);
}

CK_DLL_MFUN( event_signal )
{
    Chuck_Event * d = (Chuck_Event *)SELF;
    d->signal();
}

CK_DLL_MFUN( event_broadcast )
{
    Chuck_Event * d = (Chuck_Event *)SELF;
    d->broadcast();
}

CK_DLL_MFUN( event_wait )
{
    // Chuck_Event * d = (Chuck_Event *)SELF;
    assert( FALSE );
}

CK_DLL_MFUN( event_can_wait )
{
    RETURN->v_int = TRUE;
}


//-----------------------------------------------------------------------------
// Shred API
//-----------------------------------------------------------------------------

//CK_DLL_CTOR( shred_ctor );
//CK_DLL_DTOR( shred_dtor ); //

CK_DLL_MFUN( shred_exit )
{
    Chuck_VM_Shred * derhs = (Chuck_VM_Shred *)SELF;
    // end the shred
    derhs->is_done = TRUE;
    derhs->is_running = FALSE;
// return; // thanks
}

CK_DLL_MFUN( shred_clone )
{
}

CK_DLL_MFUN( shred_id )
{
    Chuck_VM_Shred * derhs = (Chuck_VM_Shred *)SELF;
    // return the id
    RETURN->v_int = derhs->xid;
}

CK_DLL_MFUN( shred_yield )
{
    Chuck_VM_Shred * derhs = (Chuck_VM_Shred *)SELF;
    Chuck_VM * vm = derhs->vm_ref;

    // suspend
    derhs->is_running = FALSE;
    // reshredule
    vm->shreduler()->shredule( derhs, derhs->now );
}

CK_DLL_MFUN( string_length )
{
    Chuck_String * s = (Chuck_String *)SELF;
    RETURN->v_int = s->str.length();
}

/*
CK_DLL_MFUN( string_set_at )
{
    Chuck_String * s = (Chuck_String *)SELF;
    t_CKINT c = GET_CK_INT(
    RETURN->v_int = s->str.length();
}

CK_DLL_MFUN( string_get_at )
{
    Chuck_String * s = (Chuck_String *)SELF;
    RETURN->v_int = s->str.length();
}
*/


// array.size()
CK_DLL_MFUN( array_size )
{
    Chuck_Array * array = (Chuck_Array *)SELF;
    RETURN->v_int = array->size();
}

// array.cap()
CK_DLL_MFUN( array_capacity )
{
    Chuck_Array * array = (Chuck_Array *)SELF;
    RETURN->v_int = array->capacity();
}

// array.find()
CK_DLL_MFUN( array_find )
{
    Chuck_Array * array = (Chuck_Array *)SELF;
    Chuck_String * name = GET_NEXT_STRING( ARGS );
    RETURN->v_int = array->find( name->str );
}

// array.erase()
CK_DLL_MFUN( array_erase )
{
    Chuck_Array * array = (Chuck_Array *)SELF;
    Chuck_String * name = GET_NEXT_STRING( ARGS );
    RETURN->v_int = array->erase( name->str );
}

// array.push_back()
CK_DLL_MFUN( array_push_back )
{
    Chuck_Array * array = (Chuck_Array *)SELF;
    if( array->data_type_size() == CHUCK_ARRAY4_DATASIZE )
        RETURN->v_int = ((Chuck_Array4 *)array)->push_back( GET_NEXT_UINT( ARGS ) );
    else 
        RETURN->v_int = ((Chuck_Array8 *)array)->push_back( GET_NEXT_FLOAT( ARGS ) );
}

// array.pop_back()
CK_DLL_MFUN( array_pop_back )
{
    Chuck_Array * array = (Chuck_Array *)SELF;
    if( array->data_type_size() == CHUCK_ARRAY4_DATASIZE )
        RETURN->v_int = ((Chuck_Array4 *)array)->pop_back( );
    else 
        RETURN->v_int = ((Chuck_Array8 *)array)->pop_back( );
}


//-----------------------------------------------------------------------------
// MidiIn API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MidiIn_ctor )
{
    MidiIn * min = new MidiIn;
    min->SELF = SELF;
    OBJ_MEMBER_INT(SELF, MidiIn_offset_data) = (t_CKINT)min;
}

CK_DLL_DTOR( MidiIn_dtor )
{
    delete (MidiIn *)OBJ_MEMBER_INT(SELF, MidiIn_offset_data);
    OBJ_MEMBER_INT(SELF, MidiIn_offset_data) = 0;
}

CK_DLL_MFUN( MidiIn_open )
{
    MidiIn * min = (MidiIn *)OBJ_MEMBER_INT(SELF, MidiIn_offset_data);
    t_CKINT port = GET_CK_INT(ARGS);
    RETURN->v_int = min->open( port );
}

CK_DLL_MFUN( MidiIn_good )
{
    MidiIn * min = (MidiIn *)OBJ_MEMBER_INT(SELF, MidiIn_offset_data);
    RETURN->v_int = (t_CKINT)min->good();
}

CK_DLL_MFUN( MidiIn_num )
{
    MidiIn * min = (MidiIn *)OBJ_MEMBER_INT(SELF, MidiIn_offset_data);
    RETURN->v_int = min->num();
}

CK_DLL_MFUN( MidiIn_printerr )
{
    MidiIn * min = (MidiIn *)OBJ_MEMBER_INT(SELF, MidiIn_offset_data);
    t_CKINT print_or_not = GET_CK_INT(ARGS);
    min->set_suppress( !print_or_not );
}

CK_DLL_MFUN( MidiIn_name )
{
    MidiIn * min = (MidiIn *)OBJ_MEMBER_INT(SELF, MidiIn_offset_data);
    // TODO: memory leak, please fix, Thanks.
    Chuck_String * a = (Chuck_String *)instantiate_and_initialize_object( &t_string, NULL );
    // only if valid
    if( min->good() )
        a->str = min->min->getPortName( min->num() );
    RETURN->v_string = a;
}

CK_DLL_MFUN( MidiIn_recv )
{
    MidiIn * min = (MidiIn *)OBJ_MEMBER_INT(SELF, MidiIn_offset_data);
    Chuck_Object * fake_msg = GET_CK_OBJECT(ARGS);
    MidiMsg the_msg;
    RETURN->v_int = min->recv( &the_msg );
    if( RETURN->v_int )
    {
        OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data1) = the_msg.data[0];
        OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data2) = the_msg.data[1];
        OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data3) = the_msg.data[2];
    }
}


CK_DLL_MFUN( MidiIn_can_wait )
{
    MidiIn * min = (MidiIn *)OBJ_MEMBER_INT(SELF, MidiIn_offset_data);
    RETURN->v_int = min->empty();
}


//-----------------------------------------------------------------------------
// MidiOut API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MidiOut_ctor )
{
    OBJ_MEMBER_INT(SELF, MidiOut_offset_data) = (t_CKUINT)new MidiOut;
}

CK_DLL_DTOR( MidiOut_dtor )
{
    delete (MidiOut *)OBJ_MEMBER_INT(SELF, MidiOut_offset_data);
    OBJ_MEMBER_INT(SELF, MidiOut_offset_data) = 0;
}

CK_DLL_MFUN( MidiOut_open )
{
    MidiOut * mout = (MidiOut *)OBJ_MEMBER_INT(SELF, MidiOut_offset_data);
    t_CKINT port = GET_CK_INT(ARGS);
    RETURN->v_int = mout->open( port );
}

CK_DLL_MFUN( MidiOut_good )
{
    MidiOut * mout = (MidiOut *)OBJ_MEMBER_INT(SELF, MidiOut_offset_data);
    RETURN->v_int = (t_CKINT)mout->good();
}

CK_DLL_MFUN( MidiOut_num )
{
    MidiOut * mout = (MidiOut *)OBJ_MEMBER_INT(SELF, MidiOut_offset_data);
    RETURN->v_int = mout->num();
}

CK_DLL_MFUN( MidiOut_name )
{
    MidiOut * mout = (MidiOut *)OBJ_MEMBER_INT(SELF, MidiOut_offset_data);
    // TODO: memory leak, please fix, Thanks.
    Chuck_String * a = (Chuck_String *)instantiate_and_initialize_object( &t_string, NULL );
    // only if valid
    if( mout->good() )
        a->str = mout->mout->getPortName( mout->num() );
    RETURN->v_string = a;
}

CK_DLL_MFUN( MidiOut_printerr )
{
    MidiOut * mout = (MidiOut *)OBJ_MEMBER_INT(SELF, MidiOut_offset_data);
    t_CKINT print_or_not = GET_CK_INT(ARGS);
    mout->set_suppress( !print_or_not );
}

CK_DLL_MFUN( MidiOut_send )
{
    MidiOut * mout = (MidiOut *)OBJ_MEMBER_INT(SELF, MidiOut_offset_data);
    Chuck_Object * fake_msg = GET_CK_OBJECT(ARGS);
    MidiMsg the_msg;
    the_msg.data[0] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data1);
    the_msg.data[1] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data2);
    the_msg.data[2] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data3);
    RETURN->v_int = mout->send( &the_msg );
}



//-----------------------------------------------------------------------------
// HidMsg API
//-----------------------------------------------------------------------------
CK_DLL_MFUN( HidMsg_is_axis_motion )
{
    RETURN->v_int = ( ( t_CKINT ) OBJ_MEMBER_INT( SELF, HidMsg_offset_type ) == 
                      CK_HID_JOYSTICK_AXIS ? 1 : 0 );
}

CK_DLL_MFUN( HidMsg_is_button_down )
{
    RETURN->v_int = ( ( t_CKINT ) OBJ_MEMBER_INT( SELF, HidMsg_offset_type ) == 
                      CK_HID_BUTTON_DOWN ? 1 : 0 );
}

CK_DLL_MFUN( HidMsg_is_button_up )
{
    RETURN->v_int = ( ( t_CKINT ) OBJ_MEMBER_INT( SELF, HidMsg_offset_type ) == 
                      CK_HID_BUTTON_UP ? 1 : 0 );
}

CK_DLL_MFUN( HidMsg_is_mouse_motion )
{
    RETURN->v_int = ( ( t_CKINT ) OBJ_MEMBER_INT( SELF, HidMsg_offset_type ) == 
                      CK_HID_MOUSE_MOTION ? 1 : 0 );
}

CK_DLL_MFUN( HidMsg_is_hat_motion )
{
    RETURN->v_int = ( ( t_CKINT ) OBJ_MEMBER_INT( SELF, HidMsg_offset_type ) == 
                      CK_HID_JOYSTICK_HAT ? 1 : 0 );
}

//-----------------------------------------------------------------------------
// HidIn API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( HidIn_ctor )
{
    HidIn * min = new HidIn;
    min->SELF = SELF;
    OBJ_MEMBER_INT(SELF, HidIn_offset_data) = (t_CKINT)min;
    
    // initialize hacked "static" "constants"
    OBJ_MEMBER_INT(SELF, HidIn_offset_joystick) = (t_CKINT)CK_HID_DEV_JOYSTICK;
    OBJ_MEMBER_INT(SELF, HidIn_offset_mouse) = (t_CKINT)CK_HID_DEV_MOUSE;
    OBJ_MEMBER_INT(SELF, HidIn_offset_keyboard) = (t_CKINT)CK_HID_DEV_KEYBOARD;
    OBJ_MEMBER_INT(SELF, HidIn_offset_axis_motion) = (t_CKINT)CK_HID_JOYSTICK_AXIS;
    OBJ_MEMBER_INT(SELF, HidIn_offset_button_down) = (t_CKINT)CK_HID_BUTTON_DOWN;
    OBJ_MEMBER_INT(SELF, HidIn_offset_button_up) = (t_CKINT)CK_HID_BUTTON_UP;
    OBJ_MEMBER_INT(SELF, HidIn_offset_joystick_hat) = (t_CKINT)CK_HID_JOYSTICK_HAT;
    OBJ_MEMBER_INT(SELF, HidIn_offset_joystick_ball) = (t_CKINT)CK_HID_JOYSTICK_BALL;
    OBJ_MEMBER_INT(SELF, HidIn_offset_mouse_motion) = (t_CKINT)CK_HID_MOUSE_MOTION;
    OBJ_MEMBER_INT(SELF, HidIn_offset_mouse_wheel) = (t_CKINT)CK_HID_MOUSE_WHEEL;
}

CK_DLL_DTOR( HidIn_dtor )
{
    delete (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    OBJ_MEMBER_INT(SELF, HidIn_offset_data) = 0;
}

CK_DLL_MFUN( HidIn_open )
{
    HidIn * min = (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    t_CKINT type = GET_NEXT_INT(ARGS);
    t_CKINT num = GET_NEXT_INT(ARGS);
    RETURN->v_int = min->open( type, num );
}

CK_DLL_MFUN( HidIn_open_joystick )
{
    HidIn * min = (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    t_CKINT num = GET_NEXT_INT(ARGS);
    RETURN->v_int = min->open( CK_HID_DEV_JOYSTICK, num );
}

CK_DLL_MFUN( HidIn_open_mouse )
{
    HidIn * min = (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    t_CKINT num = GET_NEXT_INT(ARGS);
    RETURN->v_int = min->open( CK_HID_DEV_MOUSE, num );
}

CK_DLL_MFUN( HidIn_open_keyboard )
{
    HidIn * min = (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    t_CKINT num = GET_NEXT_INT(ARGS);
    RETURN->v_int = min->open( CK_HID_DEV_KEYBOARD, num );
}

CK_DLL_MFUN( HidIn_good )
{
    HidIn * min = (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    RETURN->v_int = (t_CKINT)min->good();
}

CK_DLL_MFUN( HidIn_num )
{
    HidIn * min = (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    RETURN->v_int = min->num();
}

CK_DLL_MFUN( HidIn_printerr )
{
    HidIn * min = (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    t_CKINT print_or_not = GET_CK_INT(ARGS);
    min->set_suppress( !print_or_not );
}

CK_DLL_MFUN( HidIn_name )
{
    HidIn * min = (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    // TODO: memory leak, please fix, Thanks.
    Chuck_String * a = (Chuck_String *)instantiate_and_initialize_object( &t_string, NULL );
    // only if valid
    // if( min->good() )
    //     a->str = min->phin->getPortName( min->num() );
    RETURN->v_string = a;
}

CK_DLL_MFUN( HidIn_recv )
{
    HidIn * min = (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    Chuck_Object * fake_msg = GET_CK_OBJECT(ARGS);
    HidMsg the_msg;
    RETURN->v_int = min->recv( &the_msg );
    if( RETURN->v_int )
    {
        OBJ_MEMBER_INT(fake_msg, HidMsg_offset_device_type) = the_msg.device_type;
        OBJ_MEMBER_INT(fake_msg, HidMsg_offset_device_num) = the_msg.device_num;
        OBJ_MEMBER_INT(fake_msg, HidMsg_offset_type) = the_msg.type;
        OBJ_MEMBER_INT(fake_msg, HidMsg_offset_which) = the_msg.eid;
        OBJ_MEMBER_INT(fake_msg, HidMsg_offset_idata) = the_msg.idata[0];
        OBJ_MEMBER_FLOAT(fake_msg, HidMsg_offset_fdata) = the_msg.fdata[0];
        
        // mouse motion specific member variables
        OBJ_MEMBER_INT(fake_msg, HidMsg_offset_deltax) = the_msg.idata[0];
        OBJ_MEMBER_INT(fake_msg, HidMsg_offset_deltay) = the_msg.idata[1];
        
        // axis motion specific member variables
        OBJ_MEMBER_INT(fake_msg, HidMsg_offset_axis_position) = the_msg.idata[0];
        OBJ_MEMBER_FLOAT(fake_msg, HidMsg_offset_scaled_axis_position) = the_msg.fdata[0];
        OBJ_MEMBER_FLOAT(fake_msg, HidMsg_offset_axis_position2) = the_msg.fdata[0];
    }
}


CK_DLL_MFUN( HidIn_can_wait )
{
    HidIn * min = (HidIn *)OBJ_MEMBER_INT(SELF, HidIn_offset_data);
    RETURN->v_int = min->empty();
}


//-----------------------------------------------------------------------------
// HidOut API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( HidOut_ctor )
{
    OBJ_MEMBER_INT(SELF, HidOut_offset_data) = (t_CKUINT)new HidOut;
}

CK_DLL_DTOR( HidOut_dtor )
{
    delete (HidOut *)OBJ_MEMBER_INT(SELF, HidOut_offset_data);
    OBJ_MEMBER_INT(SELF, HidOut_offset_data) = 0;
}

CK_DLL_MFUN( HidOut_open )
{
    HidOut * mout = (HidOut *)OBJ_MEMBER_INT(SELF, HidOut_offset_data);
    t_CKINT num = GET_CK_INT(ARGS);
    RETURN->v_int = mout->open( num );
}

CK_DLL_MFUN( HidOut_good )
{
    HidOut * mout = (HidOut *)OBJ_MEMBER_INT(SELF, HidOut_offset_data);
    RETURN->v_int = (t_CKINT)mout->good();
}

CK_DLL_MFUN( HidOut_num )
{
    HidOut * mout = (HidOut *)OBJ_MEMBER_INT(SELF, HidOut_offset_data);
    RETURN->v_int = mout->num();
}

CK_DLL_MFUN( HidOut_name )
{
    HidOut * mout = (HidOut *)OBJ_MEMBER_INT(SELF, HidOut_offset_data);
    // TODO: memory leak, please fix, Thanks.
    Chuck_String * a = (Chuck_String *)instantiate_and_initialize_object( &t_string, NULL );
    // only if valid
    // if( mout->good() )
    //     a->str = mout->mout->getPortName( mout->num() );
    RETURN->v_string = a;
}

CK_DLL_MFUN( HidOut_printerr )
{
    HidOut * mout = (HidOut *)OBJ_MEMBER_INT(SELF, HidOut_offset_data);
    t_CKINT print_or_not = GET_CK_INT(ARGS);
    mout->set_suppress( !print_or_not );
}

CK_DLL_MFUN( HidOut_send )
{
    HidOut * mout = (HidOut *)OBJ_MEMBER_INT(SELF, HidOut_offset_data);
    Chuck_Object * fake_msg = GET_CK_OBJECT(ARGS);
    HidMsg the_msg;
/*    the_msg.data[0] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, HidMsg_offset_data1);
    the_msg.data[1] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, HidMsg_offset_data2);
    the_msg.data[2] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, HidMsg_offset_data3);
*/    RETURN->v_int = mout->send( &the_msg );
}



//-----------------------------------------------------------------------------
// MidiRW API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MidiRW_ctor )
{
    OBJ_MEMBER_INT(SELF, MidiRW_offset_data) = (t_CKUINT)new MidiRW;
}

CK_DLL_DTOR( MidiRW_dtor )
{
    delete (MidiRW *)OBJ_MEMBER_INT(SELF, MidiRW_offset_data);
    OBJ_MEMBER_INT(SELF, MidiRW_offset_data) = 0;
}

CK_DLL_MFUN( MidiRW_open )
{
    MidiRW * mrw = (MidiRW *)OBJ_MEMBER_INT(SELF, MidiRW_offset_data);
    const char * filename = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_int = mrw->open( filename );
}

CK_DLL_MFUN( MidiRW_close )
{
    MidiRW * mrw = (MidiRW *)OBJ_MEMBER_INT(SELF, MidiRW_offset_data);
    RETURN->v_int = mrw->close();
}

CK_DLL_MFUN( MidiRW_read )
{
    MidiRW * mrw = (MidiRW *)OBJ_MEMBER_INT(SELF, MidiRW_offset_data);
    Chuck_Object * fake_msg = GET_NEXT_OBJECT(ARGS);
    MidiMsg the_msg;
    t_CKTIME time = 0.0;
    RETURN->v_int = mrw->read( &the_msg, &time );
    OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data1) = the_msg.data[0];
    OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data2) = the_msg.data[1];
    OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data3) = the_msg.data[2];
    OBJ_MEMBER_TIME(fake_msg, MidiMsg_offset_when) = time;
}

CK_DLL_MFUN( MidiRW_write )
{
    MidiRW * mrw = (MidiRW *)OBJ_MEMBER_INT(SELF, MidiRW_offset_data);
    Chuck_Object * fake_msg = GET_NEXT_OBJECT(ARGS);
    t_CKTIME time = GET_NEXT_TIME(ARGS);
    MidiMsg the_msg;
    the_msg.data[0] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data1);
    the_msg.data[1] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data2);
    the_msg.data[2] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data3);
    RETURN->v_int = mrw->write( &the_msg, &time );
}


//-----------------------------------------------------------------------------
// MidiMsgOut API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MidiMsgOut_ctor )
{
    OBJ_MEMBER_INT(SELF, MidiMsgOut_offset_data) = (t_CKUINT)new MidiMsgOut;
}

CK_DLL_DTOR( MidiMsgOut_dtor )
{
    delete (MidiMsgOut *)OBJ_MEMBER_INT(SELF, MidiMsgOut_offset_data);
    OBJ_MEMBER_INT(SELF, MidiMsgOut_offset_data) = 0;
}

CK_DLL_MFUN( MidiMsgOut_open )
{
    MidiMsgOut * mrw = (MidiMsgOut *)OBJ_MEMBER_INT(SELF, MidiMsgOut_offset_data);
    const char * filename = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_int = mrw->open( filename );
}

CK_DLL_MFUN( MidiMsgOut_close )
{
    MidiMsgOut * mrw = (MidiMsgOut *)OBJ_MEMBER_INT(SELF, MidiMsgOut_offset_data);
    RETURN->v_int = mrw->close();
}

CK_DLL_MFUN( MidiMsgOut_write )
{
    MidiMsgOut * mrw = (MidiMsgOut *)OBJ_MEMBER_INT(SELF, MidiMsgOut_offset_data);
    Chuck_Object * fake_msg = GET_NEXT_OBJECT(ARGS);
    t_CKTIME time = GET_NEXT_TIME(ARGS);
    MidiMsg the_msg;
    the_msg.data[0] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data1);
    the_msg.data[1] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data2);
    the_msg.data[2] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data3);
    RETURN->v_int = mrw->write( &the_msg, &time );
}


//-----------------------------------------------------------------------------
// MidiMsgIn API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MidiMsgIn_ctor )
{
    OBJ_MEMBER_INT(SELF, MidiMsgIn_offset_data) = (t_CKUINT)new MidiMsgIn;
}

CK_DLL_DTOR( MidiMsgIn_dtor )
{
    delete (MidiMsgIn *)OBJ_MEMBER_INT(SELF, MidiMsgIn_offset_data);
    OBJ_MEMBER_INT(SELF, MidiMsgIn_offset_data) = 0;
}

CK_DLL_MFUN( MidiMsgIn_open )
{
    MidiMsgIn * mrw = (MidiMsgIn *)OBJ_MEMBER_INT(SELF, MidiMsgIn_offset_data);
    const char * filename = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_int = mrw->open( filename );
}

CK_DLL_MFUN( MidiMsgIn_close )
{
    MidiMsgIn * mrw = (MidiMsgIn *)OBJ_MEMBER_INT(SELF, MidiMsgIn_offset_data);
    RETURN->v_int = mrw->close();
}

CK_DLL_MFUN( MidiMsgIn_read )
{
    MidiMsgIn * mrw = (MidiMsgIn *)OBJ_MEMBER_INT(SELF, MidiMsgIn_offset_data);
    Chuck_Object * fake_msg = GET_NEXT_OBJECT(ARGS);
    MidiMsg the_msg;
    t_CKTIME time = 0.0;
    RETURN->v_int = mrw->read( &the_msg, &time );
    OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data1) = the_msg.data[0];
    OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data2) = the_msg.data[1];
    OBJ_MEMBER_INT(fake_msg, MidiMsg_offset_data3) = the_msg.data[2];
    OBJ_MEMBER_TIME(fake_msg, MidiMsg_offset_when) = time;
}


/*//-----------------------------------------------------------------------------
// SkiniIn API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( SkiniIn_ctor )
{
    SkiniIn * skin = new SkiniIn;
//  skin->SELF = SELF;
    OBJ_MEMBER_INT(SELF, SkiniIn_offset_data) = (t_CKINT)skin;
}

CK_DLL_DTOR( SkiniIn_dtor )
{
    delete (SkiniIn *)OBJ_MEMBER_INT(SELF, SkiniIn_offset_data);
}

CK_DLL_MFUN( SkiniIn_open )
{
    SkiniIn * skin = (SkiniIn *)OBJ_MEMBER_INT(SELF, SkiniIn_offset_data);
    const char * filename = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_int = skin->open( filename );
}

CK_DLL_MFUN( SkiniIn_recv )
{
    SkiniIn * skin = (SkiniIn *)OBJ_MEMBER_INT(SELF, SkiniIn_offset_data);
    Chuck_Object * fake_msg = GET_CK_OBJECT(ARGS);
    SkiniMsg the_msg;
    RETURN->v_int = skin->recv( &the_msg );
    if( RETURN->v_int )
    {
        OBJ_MEMBER_INT( fake_msg, SkiniMsg_offset_type ) = the_msg.type;
        OBJ_MEMBER_FLOAT( fake_msg, SkiniMsg_offset_time ) = the_msg.time;
        OBJ_MEMBER_INT( fake_msg, SkiniMsg_offset_channel ) = the_msg.channel;
        OBJ_MEMBER_FLOAT( fake_msg, SkiniMsg_offset_data1 ) = the_msg.data1;
        OBJ_MEMBER_FLOAT( fake_msg, SkiniMsg_offset_data2 ) = the_msg.data2;
    }
}


//-----------------------------------------------------------------------------
// SkiniOut API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( SkiniOut_ctor )
{
    OBJ_MEMBER_INT(SELF, SkiniOut_offset_data) = (t_CKUINT)new SkiniOut;
}

CK_DLL_DTOR( SkiniOut_dtor )
{
    delete (SkiniOut *)OBJ_MEMBER_INT(SELF, SkiniOut_offset_data);
}

CK_DLL_MFUN( SkiniOut_open )
{
    SkiniOut * sout = (SkiniOut *)OBJ_MEMBER_INT(SELF, SkiniOut_offset_data);
    const char * filename = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_int = sout->open( filename );
}

CK_DLL_MFUN( SkiniOut_send )
{
    SkiniOut * sout = (SkiniOut *)OBJ_MEMBER_INT(SELF, SkiniOut_offset_data);
    Chuck_Object * fake_msg = GET_CK_OBJECT(ARGS);
    SkiniMsg the_msg;
    
    the_msg.type = OBJ_MEMBER_INT( fake_msg, SkiniMsg_offset_type );
    the_msg.time = OBJ_MEMBER_FLOAT( fake_msg, SkiniMsg_offset_time );
    the_msg.channel = OBJ_MEMBER_INT( fake_msg, SkiniMsg_offset_channel );
    the_msg.data1 = OBJ_MEMBER_FLOAT( fake_msg, SkiniMsg_offset_data1 );
    the_msg.data2 = OBJ_MEMBER_FLOAT( fake_msg, SkiniMsg_offset_data2 );

    //the_msg.data[0] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, SkiniMsg_offset_data1);
    //the_msg.data[1] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, SkiniMsg_offset_data2);
    //the_msg.data[2] = (t_CKBYTE)OBJ_MEMBER_INT(fake_msg, SkiniMsg_offset_data3);
    
    RETURN->v_int = sout->send( &the_msg );
}
*/

/*
//-----------------------------------------------------------------------------
// name: lang_query()
// desc: query entry point
//-----------------------------------------------------------------------------
DLL_QUERY lang_query( Chuck_DL_Query * QUERY )
{
    QUERY->setname( QUERY, "Lang" );

    // class
    QUERY->begin_class( QUERY, "Object", "" );
    
    // add ctor
    QUERY->add_ctor( QUERY, object_ctor );

    // add dtor
    QUERY->add_dtor( QUERY, object_dtor );

    // add setTestID
    QUERY->add_mfun( QUERY, object_setTestID, "void", "setTestID" );
    QUERY->add_arg( QUERY, "int", "value" );

    // add getTestID
    QUERY->add_mfun( QUERY, object_getTestID, "int", "getTestID" );

    // add toString
    //! return string that represents the value of the object
    // QUERY->add_mfun( QUERY, object_toString, "string", "toString" );
    
    // end class
    QUERY->end_class( QUERY );

    return TRUE;
}
*/
