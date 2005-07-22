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
// name: chuck_ugen.h
// desc: chuck unit generator interface
//
// authors: Ge Wang (gewang@cs.princeton.edu)
//          Perry R. Cook (prc@cs.princeton.edu)
// date: spring 2004 - 1.1
//       spring 2005 - 1.2
//-----------------------------------------------------------------------------
#ifndef __CHUCK_UGEN_H__
#define __CHUCK_UGEN_H__

#include "chuck_def.h"
#include "chuck_oo.h"
#include "chuck_dl.h"


// forward reference
struct  Chuck_VM_Shred;


// op mode
#define UGEN_OP_PASS    -1
#define UGEN_OP_STOP    0
#define UGEN_OP_TICK    1

//-----------------------------------------------------------------------------
// name: struct Chuck_UGen
// dsec: ugen base
//-----------------------------------------------------------------------------
struct Chuck_UGen : public Chuck_Object
{
public:
    Chuck_UGen( );
    virtual ~Chuck_UGen( );
    virtual void init();
    virtual void done();

public: // src
    t_CKBOOL add( Chuck_UGen * src );
    t_CKBOOL remove( Chuck_UGen * src );
    t_CKVOID remove_all( );
    t_CKBOOL set_max_src( t_CKUINT num );
    t_CKUINT get_num_src( );
    t_CKUINT disconnect( t_CKBOOL recursive );
    t_CKUINT system_tick( t_CKTIME now );

protected:
    t_CKVOID add_by( Chuck_UGen * dest );
    t_CKVOID remove_by( Chuck_UGen * dest );

public:
    // tick function
    f_tick tick;
    // msg function
    f_pmsg pmsg;
    // channels (if more than one is required)
    Chuck_UGen ** m_multi_chan;
    // size of m_multi_chan;
    t_CKUINT m_multi_chan_size;
    // number of channels
    t_CKUINT m_num_ins;
    // number of channels
    t_CKUINT m_num_outs;
    // alloc multi channels
    void alloc_multi_chan( t_CKUINT num_ins, t_CKUINT num_outs );

public: // data
    Chuck_UGen ** m_src_list;
    t_CKUINT m_src_cap;
    t_CKUINT m_num_src;
    Chuck_UGen ** m_dest_list;
    t_CKUINT m_dest_cap;
    t_CKUINT m_num_dest;
    t_CKUINT m_max_src;
    t_CKTIME m_time;
    t_CKBOOL m_valid;
    t_CKBOOL m_use_next;
    SAMPLE m_sum;
    SAMPLE m_current;
    SAMPLE m_next;
    SAMPLE m_last;
    SAMPLE m_gain;
    SAMPLE m_pan;
    t_CKINT m_op;

    // the shred on which the ugen is created
    Chuck_VM_Shred * shred;
    // owner
    Chuck_UGen * owner;
};




#endif
