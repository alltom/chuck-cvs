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
// file: chuck_otf.h
// desc: on-the-fly programming utilities
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2004
//-----------------------------------------------------------------------------
#ifndef __CHUCK_OTF_H__
#define __CHUCK_OTF_H__

#include "chuck_def.h"
#include <memory.h>


//-----------------------------------------------------------------------------
// name: struct Net_Msg()
// desc: ...
//-----------------------------------------------------------------------------
struct Net_Msg
{
    t_CKUINT header;
    t_CKUINT type;
    t_CKUINT param;
    t_CKUINT param2;
    t_CKUINT param3;
    t_CKUINT length;
    char buffer[512];
    
    Net_Msg() { this->clear(); }
    void clear() { header = type = param = param2 = param3 = length = 0;
                   memset( buffer, 0, sizeof(buffer) ); }
};


#endif
