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
// name: chuck_lang.h
// desc: chuck class library base
//
// authors: Ge Wang (gewang@cs.princeton.edu)
//          Perry R. Cook (prc@cs.princeton.edu)
//          Ananya Misra (amisra@cs.princeton.edu)
//    date: spring 2005
//-----------------------------------------------------------------------------
#ifndef __CHUCK_LANG_H__
#define __CHUCK_LANG_H__

#include "chuck_def.h"
#include "chuck_oo.h"
#include "chuck_dl.h"
#include <queue>


// query
DLL_QUERY lang_query( Chuck_DL_Query * QUERY );

// base class initialization
t_CKBOOL init_class_object( Chuck_Env * env, Chuck_Type * type );
t_CKBOOL init_class_ugen( Chuck_Env * env, Chuck_Type * type );
t_CKBOOL init_class_event( Chuck_Env * env, Chuck_Type * type );
t_CKBOOL init_class_shred( Chuck_Env * env, Chuck_Type * type );
t_CKBOOL init_class_string( Chuck_Env * env, Chuck_Type * type );
t_CKBOOL init_class_array( Chuck_Env * env, Chuck_Type * type );
t_CKBOOL init_class_Midi( Chuck_Env * env );
t_CKBOOL init_class_MidiRW( Chuck_Env * env );



//-----------------------------------------------------------------------------
// object API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( object_ctor );
CK_DLL_DTOR( object_dtor );
CK_DLL_MFUN( object_equals );
CK_DLL_MFUN( object_getType );
CK_DLL_MFUN( object_hashCode );
CK_DLL_MFUN( object_toString );
CK_DLL_MFUN( object_setTestID );
CK_DLL_MFUN( object_getTestID );
CK_DLL_SFUN( object_testStatic );




//-----------------------------------------------------------------------------
// ugen API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( ugen_ctor );
CK_DLL_DTOR( ugen_dtor );
CK_DLL_MFUN( ugen_op );
CK_DLL_MFUN( ugen_cget_op );
CK_DLL_MFUN( ugen_last );
CK_DLL_MFUN( ugen_cget_last );
CK_DLL_MFUN( ugen_next );
CK_DLL_MFUN( ugen_cget_next );
CK_DLL_MFUN( ugen_gain );
CK_DLL_MFUN( ugen_cget_gain );


//-----------------------------------------------------------------------------
// shred API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( shred_ctor );
CK_DLL_DTOR( shred_dtor ); // 
CK_DLL_MFUN( shred_exit );
CK_DLL_MFUN( shred_clone );


//-----------------------------------------------------------------------------
// array API
//-----------------------------------------------------------------------------
CK_DLL_MFUN( array_capacity );
CK_DLL_MFUN( array_size );
CK_DLL_MFUN( array_push_back );
CK_DLL_MFUN( array_pop_back );
CK_DLL_MFUN( array_push_front );
CK_DLL_MFUN( array_pop_font );
CK_DLL_MFUN( array_clear );
CK_DLL_MFUN( array_set_capacity );
CK_DLL_MFUN( array_set_size );




//-----------------------------------------------------------------------------
// event API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( event_ctor );
CK_DLL_DTOR( event_dtor );
CK_DLL_MFUN( event_signal );
CK_DLL_MFUN( event_broadcast );
CK_DLL_MFUN( event_wait );
CK_DLL_MFUN( event_can_wait );



//-----------------------------------------------------------------------------
// string API
//-----------------------------------------------------------------------------
CK_DLL_MFUN( string_length );
CK_DLL_MFUN( string_set_at );
CK_DLL_MFUN( string_get_at );


/*
struct Chuck_VM;
struct Chuck_VM_Shred;
//-----------------------------------------------------------------------------
// name: Data_Event
// desc: data for base Chuck Event class
//-----------------------------------------------------------------------------
struct Data_Event
{
public:
    Data_Event();
    ~Data_Event();

public:
    t_CKUINT signal();
    t_CKUINT broadcast();
    t_CKUINT wait( Chuck_VM_Shred * shred, Chuck_VM * vm );

	std::queue<Chuck_VM_Shred *> m_queue;
};
//-----------------------------------------------------------------------------
// event API
//-----------------------------------------------------------------------------
*/


//-----------------------------------------------------------------------------
// name: Data_Exception
// desc: data for base Chuck Exception class
//-----------------------------------------------------------------------------
struct Data_Exception
{
public:
    Data_Exception();
    ~Data_Exception();

public:
};
//-----------------------------------------------------------------------------
// exception API
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// MidiMsg API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MidiMsg_ctor );



//-----------------------------------------------------------------------------
// MidiRW API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MidiRW_ctor );
CK_DLL_DTOR( MidiRW_dtor );
CK_DLL_MFUN( MidiRW_open );
CK_DLL_MFUN( MidiRW_close );
CK_DLL_MFUN( MidiRW_read );
CK_DLL_MFUN( MidiRW_write );


//-----------------------------------------------------------------------------
// MidiIn API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MidiIn_ctor );
CK_DLL_DTOR( MidiIn_dtor );
CK_DLL_MFUN( MidiIn_open );
CK_DLL_MFUN( MidiIn_recv );
CK_DLL_MFUN( MidiIn_can_wait );




//-----------------------------------------------------------------------------
// MidiOut API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( MidiOut_ctor );
CK_DLL_DTOR( MidiOut_dtor );
CK_DLL_MFUN( MidiOut_open );
CK_DLL_MFUN( MidiOut_send );



//-----------------------------------------------------------------------------
// SkiniMMsg API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( SkiniMsg_ctor );




//-----------------------------------------------------------------------------
// SkiniIn API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( SkiniIn_ctor );
CK_DLL_DTOR( SkiniIn_dtor );
CK_DLL_MFUN( SkiniIn_open );
CK_DLL_MFUN( SkiniIn_recv );




//-----------------------------------------------------------------------------
// SkiniOut API
//-----------------------------------------------------------------------------
CK_DLL_CTOR( SkiniOut_ctor );
CK_DLL_DTOR( SkiniOut_dtor );
CK_DLL_MFUN( SkiniOut_open );
CK_DLL_MFUN( SkiniOut_send );


#endif
