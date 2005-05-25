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
// file: ulib_opensoundcontrol.cpp
// desc: ...
//
// author: Philip L. Davidson (philipd@alumni.princeton.edu)
//         Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2005
//-----------------------------------------------------------------------------


#include "ulib_opensoundcontrol.h"
#include "chuck_type.h"

#include "util_opensoundcontrol.h"

static t_CKUINT osc_send_offset_data = 0;


DLL_QUERY opensoundcontrol_query ( Chuck_DL_Query * query ) { 

    // get the env
    Chuck_Env * env = Chuck_Env::instance();
    Chuck_DL_Func * func = NULL;

    // init base class
    if( !type_engine_import_class_begin( env, "OSC_Send", "Object",
                                         env->global(), osc_send_ctor ) )
		return FALSE;

    // add member variable  - OSCTransmitter object
    osc_send_offset_data = type_engine_import_mvar( env, "int", "@OSC_Send_data", FALSE );
    if( osc_send_offset_data == CK_INVALID_OFFSET ) goto error;

    func = make_new_mfun( "int", "setHost", osc_send_setHost );
    func->add_arg( "string", "address" );
    func->add_arg( "int", "port" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "startMesg", osc_send_startMesg );
    func->add_arg( "string", "address" );
    func->add_arg( "string", "args" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "addInt", osc_send_addInt );
    func->add_arg( "int", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "float", "addFloat", osc_send_addFloat );
    func->add_arg( "float", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "string", "addString", osc_send_addString );
    func->add_arg( "string", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "openBundle", osc_send_openBundle );
    func->add_arg( "time", "value" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "closeBundle", osc_send_closeBundle );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "kick", osc_send_holdMesg );
    func->add_arg( "int", "on" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "kick", osc_send_kickMesg );
    if( !type_engine_import_mfun( env, func ) ) goto error;

error:

    // end the class import
    type_engine_import_class_end( env );
}

//----------------------------------------------
// name : osc_send_ctor
// desc : CTOR function 
//-----------------------------------------------
CK_DLL_CTOR( osc_send_ctor ) { 
    OSC_Transmitter* xmit = new OSC_Transmitter();
    OBJ_MEMBER_INT( SELF, osc_send_offset_data ) = (t_CKINT)xmit;
}
 
//----------------------------------------------
// name :  osc_send_dtor 
// desc : DTOR function 
//-----------------------------------------------
CK_DLL_DTOR( osc_send_dtor ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    delete xmit;
}

//----------------------------------------------
// name :  osc_send_setHost 
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_send_setHost ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    Chuck_String* host = GET_NEXT_STRING(ARGS);
    t_CKINT port = GET_NEXT_INT(ARGS);
    xmit->setHost( (char*) host->str.c_str(), port );
}

//----------------------------------------------
// name :  osc_send_startMesg 
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_send_startMesg ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    Chuck_String* address = GET_NEXT_STRING(ARGS);
    Chuck_String* args = GET_NEXT_STRING(ARGS);
    xmit->startMessage( (char*) address->str.c_str(), (char*) args->str.c_str() );
}

//----------------------------------------------
// name :  osc_send_addInt 
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_send_addInt ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    xmit->addInt( GET_NEXT_INT(ARGS) );
}

//----------------------------------------------
// name :  osc_send_addFloat 
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_send_addFloat ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    xmit->addFloat( (float)(GET_NEXT_FLOAT(ARGS)) );
}

//----------------------------------------------
// name :  osc_send_addString 
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_send_addString ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    xmit->addString( (char*)(GET_NEXT_STRING(ARGS))->str.c_str() );
}

//----------------------------------------------
// name :  osc_send_openBundle 
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_send_openBundle ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    // we should add in an option to translate from chuck-time to current time to timetag.
    // but for now let's just spec. immediately.
    xmit->openBundle(OSCTT_Immediately());
}

//----------------------------------------------
// name :  osc_send_closeBundle 
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_send_closeBundle ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    xmit->closeBundle();
}

//----------------------------------------------
// name :  osc_send_holdMesg 
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_send_holdMesg ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    xmit->holdMessage( (bool)GET_NEXT_INT(ARGS) );
}

//----------------------------------------------
// name :  osc_send_kickMesg 
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_send_kickMesg ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    xmit->kickMessage();
}


#include "chuck_dl.h"

