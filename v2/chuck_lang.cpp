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
#include "chuck_vm.h"
#include "midiio_rtmidi.h"
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

	//the type argument IS t_ugen here - there's no other use of this function
	//why are we using t_ugen here instead of type ( or vice versa ) ? PLD
    // add ugen info

    t_ugen.ugen_info = new Chuck_UGen_Info;
    t_ugen.ugen_info->add_ref();
    t_ugen.ugen_info->tick = __ugen_tick;

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

    // end
    type_engine_import_class_end( env );

    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );

    return FALSE;
}




static t_CKUINT event_offset_data = 0;
//-----------------------------------------------------------------------------
// name: init_class_event()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_event( Chuck_Env * env, Chuck_Type * type )
{
    Chuck_DL_Func * func = NULL;
    Chuck_Value * value = NULL;

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
    func->add_arg( "shred", "me" );
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




//-----------------------------------------------------------------------------
// name: init_class_shred()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL init_class_shred( Chuck_Env * env, Chuck_Type * type )
{
    // init as base class
    Chuck_DL_Func * func = NULL;

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
//    init_base_array( env, type, env->global(), (t_CKUINT)string_ctor );
    return TRUE;
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
    if( !type_engine_import_class_begin( env, "MidiIn", "event",
                                         env->global(), MidiIn_ctor ) )
        return FALSE;

    // add open()
    func = make_new_mfun( "int", "open", MidiIn_open );
    func->add_arg( "int", "port" );
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
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;

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
}

CK_DLL_MFUN( ugen_cget_gain )
{
    // get as ugen
    Chuck_UGen * ugen = (Chuck_UGen *)SELF;
    // set return
    RETURN->v_float = (t_CKFLOAT)ugen->m_gain;
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
    Chuck_Event * d = (Chuck_Event *)SELF;
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
    RETURN->v_int = derhs->id;
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
}

CK_DLL_MFUN( MidiIn_open )
{
    MidiIn * min = (MidiIn *)OBJ_MEMBER_INT(SELF, MidiIn_offset_data);
    t_CKINT port = GET_CK_INT(ARGS);
    RETURN->v_int = min->open( port );
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
}

CK_DLL_MFUN( MidiOut_open )
{
    MidiOut * mout = (MidiOut *)OBJ_MEMBER_INT(SELF, MidiOut_offset_data);
    t_CKINT port = GET_CK_INT(ARGS);
    RETURN->v_int = mout->open( port );
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
// MidiRW API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MidiRW_ctor )
{
    OBJ_MEMBER_INT(SELF, MidiRW_offset_data) = (t_CKUINT)new MidiRW;
}

CK_DLL_DTOR( MidiRW_dtor )
{
    delete (MidiRW *)OBJ_MEMBER_INT(SELF, MidiRW_offset_data);
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
