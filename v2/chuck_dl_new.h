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
// name: chuck_dl.h
// desc: chuck dynamic linking header
//
// authors: Ge Wang (gewang@cs.princeton.edu)
//          Perry R. Cook (prc@cs.princeton.edu)
// mac os code based on apple's open source
//
// date: spring 2004 - 1.1
//       spring 2005 - 1.2
//-----------------------------------------------------------------------------
#ifndef __CHUCK_DL_H__
#define __CHUCK_DL_H__

#include "chuck_def.h"
#include "chuck_oo.h"
#include <string>
#include <vector>
#include <map>


// dynamic linking class interface prototypes
extern "C" {
// object
typedef Chuck_Object * (* f_ctor)( Chuck_Object * self );
typedef t_CKVOID       (* f_dtor)( Chuck_Object * self );
typedef t_CKVOID       (* f_mfun)( Chuck_Object * self, void * ARGS, Chuck_DL_Return & RETURN );
typedef t_CKVOID       (* f_sfun)( void * ARGS, Chuck_DL_Return & RETURN );
// ugen specific
typedef t_CKBOOL       (* f_tick)( Chuck_UGen * self, SAMPLE in, SAMPLE * out );
typedef t_CKBOOL       (* f_pmsg)( Chuck_UGen * self, const char * msg, void * ARGS );
}



#endif
