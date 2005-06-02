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
// file: ulib_opsc.cpp
// desc: ...
//
// author: Philip L. Davidson (philipd@alumni.princeton.edu)
//         Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2005
//-----------------------------------------------------------------------------


#include "ulib_opsc.h"
#include "chuck_type.h"
#include "chuck_vm.h"
#include "chuck_dl.h"
#include "util_opsc.h"

static t_CKUINT osc_send_offset_data = 0;
static t_CKUINT osc_recv_offset_data = 0;
static t_CKUINT osc_address_offset_data = 0;

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

    type_engine_import_class_end( env );

    // init base class
    if( !type_engine_import_class_begin( env, "OSC_Addr", "event",
                                         env->global(), osc_address_ctor ) )
		return FALSE;

    // add member variable  - OSCAddress object

    osc_address_offset_data = type_engine_import_mvar( env, "int", "@OSC_Address_data", FALSE );
    if( osc_recv_offset_data == CK_INVALID_OFFSET ) goto error;

    func = make_new_mfun( "int", "set", osc_address_set );
    func->add_arg( "string" , "addr" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "hasMesg", osc_address_has_mesg );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "nextMesg", osc_address_next_mesg );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "getInt", osc_address_next_int );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "float", "getFloat", osc_address_next_float );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "string", "getString", osc_address_next_string );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "can_wait", osc_address_can_wait );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    type_engine_import_class_end( env );

    // init base class
    if( !type_engine_import_class_begin( env, "OSC_Recv", "Object",
                                         env->global(), osc_recv_ctor ) )
		return FALSE;

    // add member variable  - OSCReceiver object
    osc_recv_offset_data = type_engine_import_mvar( env, "int", "@OSC_Recv_data", FALSE );
    if( osc_recv_offset_data == CK_INVALID_OFFSET ) goto error;

    func = make_new_mfun( "int", "port", osc_recv_port );
    func->add_arg( "int" , "port" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "listen", osc_recv_listen );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "add_address", osc_recv_add_address );
    func->add_arg( "OSC_Addr" , "addr" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "int", "remove_address", osc_recv_remove_address );
    func->add_arg( "OSC_Addr" , "addr" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    func = make_new_mfun( "OSC_Addr", "event", osc_recv_new_address );
    func->add_arg( "string" , "spec" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    type_engine_import_class_end( env );
    return TRUE;

error:

    fprintf( stderr, "class import error!\n" );
    // end the class import
    type_engine_import_class_end( env );
    return FALSE;
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
    xmit->holdMessage( GET_NEXT_INT(ARGS) != 0 );
}

//----------------------------------------------
// name :  osc_send_kickMesg 
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_send_kickMesg ) { 
    OSC_Transmitter* xmit = (OSC_Transmitter *)OBJ_MEMBER_INT(SELF, osc_send_offset_data);
    xmit->kickMessage();
}

//----------------------------------------------
// name :  osc_address_ctor  
// desc : CTOR function 
//-----------------------------------------------
CK_DLL_CTOR( osc_address_ctor ) { 
    OSCSrc * addr = new OSCSrc();
    addr->SELF = SELF;
//    fprintf(stderr,"address:ptr %x\n", (uint)addr);
//    fprintf(stderr,"self:ptr %x\n", (uint)SELF);
    OBJ_MEMBER_INT( SELF, osc_address_offset_data ) = (t_CKINT)addr;
}

CK_DLL_MFUN( osc_address_set ) { 
    OSCSrc * addr = (OSCSrc *)OBJ_MEMBER_INT( SELF, osc_address_offset_data );
    addr->setSpec ( (char*)(GET_NEXT_STRING(ARGS))->str.c_str() );
    RETURN->v_int = 0;
}

//----------------------------------------------
// name :  osc_address_can_wait   
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN(  osc_address_can_wait  ) { 
    OSCSrc * addr = (OSCSrc *)OBJ_MEMBER_INT( SELF, osc_address_offset_data );
    RETURN->v_int = ( addr->has_mesg() ) ? 0 : 1;
}
   
//----------------------------------------------
// name :  osc_address_has_mesg   
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN(  osc_address_has_mesg  ) { 
    OSCSrc * addr = (OSCSrc *)OBJ_MEMBER_INT( SELF, osc_address_offset_data );
    RETURN->v_int = ( addr->has_mesg() ) ? 1 : 0 ;
}

//----------------------------------------------
// name :  osc_address_next_mesg   
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN(  osc_address_next_mesg  ) { 
    OSCSrc * addr = (OSCSrc *)OBJ_MEMBER_INT( SELF, osc_address_offset_data );
    RETURN->v_int = ( addr->next_mesg() ) ? 1 : 0 ;
}

//----------------------------------------------
// name :  osc_address_next_int   
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN(  osc_address_next_int  ) { 
    OSCSrc * addr = (OSCSrc *)OBJ_MEMBER_INT( SELF, osc_address_offset_data );
    RETURN->v_int = addr->next_int();
}

//----------------------------------------------
// name :  osc_address_next_float   
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN(  osc_address_next_float  ) { 
    OSCSrc * addr = (OSCSrc *)OBJ_MEMBER_INT( SELF, osc_address_offset_data );
    RETURN->v_float = addr->next_float();
}

//----------------------------------------------
// name :  osc_address_next_string   
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN(  osc_address_next_string  ) { 
    OSCSrc * addr = (OSCSrc *)OBJ_MEMBER_INT( SELF, osc_address_offset_data );
    char * cs = addr->next_string();
    addr->p_str.str = "fuzzybunnies";//cs;
    RETURN->v_string = &(addr->p_str);
    
}

// OSC_Recv functions 


//----------------------------------------------
// name :  osc_recv_ctor  
// desc : CTOR function 
//-----------------------------------------------
CK_DLL_CTOR( osc_recv_ctor ) { 
    OSC_Receiver * recv = new OSC_Receiver();
    OBJ_MEMBER_INT( SELF, osc_send_offset_data ) = (t_CKINT)recv;
}

//----------------------------------------------
// name :  osc_recv_dtor  
// desc : DTOR function 
//-----------------------------------------------
CK_DLL_DTOR( osc_recv_dtor ) { 
    OSC_Receiver * recv = (OSC_Receiver *)OBJ_MEMBER_INT(SELF, osc_recv_offset_data);
    delete recv;
}

//----------------------------------------------
// name :  osc_recv_port  
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_recv_port ) { 

    OSC_Receiver * recv = (OSC_Receiver *)OBJ_MEMBER_INT(SELF, osc_recv_offset_data);
    recv->bind_to_port( (int)GET_NEXT_INT(ARGS) );
}

//----------------------------------------------
// name :  osc_recv_add_listener  
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_recv_add_address ) { 
    OSC_Receiver * recv = (OSC_Receiver *)OBJ_MEMBER_INT(SELF, osc_recv_offset_data);
    Chuck_Object* addr_obj = GET_NEXT_OBJECT(ARGS); //address object class...
    OSCSrc * addr = (OSCSrc *)OBJ_MEMBER_INT( addr_obj, osc_address_offset_data ); 
    recv->add_address( addr );
}

//----------------------------------------------
// name :  osc_recv_remove_listener  
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_recv_remove_address ) { 
    OSC_Receiver * recv = (OSC_Receiver *)OBJ_MEMBER_INT(SELF, osc_recv_offset_data);
    Chuck_Object* addr_obj = GET_NEXT_OBJECT(ARGS); //listener object class...
    OSCSrc * addr = (OSCSrc *)OBJ_MEMBER_INT( addr_obj, osc_address_offset_data ); 
    recv->remove_address( addr );
}

//----------------------------------------------
// name :  osc_recv_new_address  
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_recv_new_address ) { 
    OSC_Receiver * recv = (OSC_Receiver *)OBJ_MEMBER_INT(SELF, osc_recv_offset_data);
    Chuck_String* spec_obj = (Chuck_String*) GET_NEXT_STRING(ARGS); //listener object class...

    RETURN->v_object = recv->new_event ( (char*)spec_obj->str.c_str() );
}



// need to add a listen function in Receiver which opens up a recv loop on another thread.
// address then subscribe to a receiver to take in events. 

//----------------------------------------------
// name :  osc_recv_listen  
// desc : MFUN function 
//-----------------------------------------------
CK_DLL_MFUN( osc_recv_listen ) { 
    OSC_Receiver * recv = (OSC_Receiver *)OBJ_MEMBER_INT(SELF, osc_recv_offset_data);
    recv->listen();
}


