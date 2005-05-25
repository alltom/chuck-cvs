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
// file: ulib_opensoundcontrol.h
// desc: ...
//
// author: Philip L. Davidson (philipd@alumni.princeton.edu)
//         Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2005
//-----------------------------------------------------------------------------

#ifndef __ULIB_OPENSOUNDCONTROL_H__
#define __ULIB_OPENSOUNDCONTROL_H__

#include "chuck_dl.h"
#include "chuck_oo.h"
#include "chuck_def.h"

DLL_QUERY opensoundcontrol_query ( Chuck_DL_Query* query );

CK_DLL_CTOR ( osc_send_ctor );
CK_DLL_DTOR ( osc_send_dtor );
CK_DLL_MFUN ( osc_send_setHost );
CK_DLL_MFUN ( osc_send_startMesg );
CK_DLL_MFUN ( osc_send_addInt );
CK_DLL_MFUN ( osc_send_addFloat );
CK_DLL_MFUN ( osc_send_addString );
CK_DLL_MFUN ( osc_send_openBundle );
CK_DLL_MFUN ( osc_send_closeBundle );
CK_DLL_MFUN ( osc_send_holdMesg );
CK_DLL_MFUN ( osc_send_kickMesg );

#endif
