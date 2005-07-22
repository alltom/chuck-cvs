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
// name: chuck_ugen.cpp
// desc: chuck unit generator interface
//
// authors: Ge Wang (gewang@cs.princeton.edu)
//          Perry R. Cook (prc@cs.princeton.edu)
// date: spring 2004 - 1.1
//       spring 2005 - 1.2
//-----------------------------------------------------------------------------
#include "chuck_ugen.h"
#include "chuck_vm.h"
using namespace std;




//-----------------------------------------------------------------------------
// name: Chuck_UGen()
// desc: constructor
//-----------------------------------------------------------------------------
Chuck_UGen::Chuck_UGen()
{
    this->init();
}




//-----------------------------------------------------------------------------
// name: ~Chuck_UGen()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_UGen::~Chuck_UGen()
{
    this->done();
}




//-----------------------------------------------------------------------------
// name: init()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_UGen::init()
{
    tick = NULL;
    pmsg = NULL;
    m_multi_chan = NULL;
    m_multi_chan_size = 0;
    m_num_ins = 1;
    m_num_outs = 1;
    
    m_num_src = 0;
    m_num_dest = 0;
    m_max_src = 0xffffffff;
    m_time = 0;
    m_valid = TRUE;
    m_sum = 0.0f;
    m_current = 0.0f;
    m_last = 0.0f;
    m_op = UGEN_OP_TICK;
    m_gain = 1.0f;
    m_pan = 1.0f;
    m_next = 0.0f;
    m_use_next = FALSE;

    shred = NULL;
    owner = NULL;
}




//-----------------------------------------------------------------------------
// name: done()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_UGen::done()
{
    if( this->shred )
        shred->remove( this );

    assert( this->m_ref_count == 0 );

    this->remove_all();
    m_valid = FALSE;

    // TODO: m_multi_chan, break ref count loop
}




//-----------------------------------------------------------------------------
// name: alloc_multi_chan()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_UGen::alloc_multi_chan( t_CKUINT num_ins, t_CKUINT num_outs )
{
    // get max of num_ins and num_outs
    m_multi_chan_size = ( num_ins > num_outs ? num_ins : num_outs );
    // if more than one
    if( m_multi_chan_size > 1 )
    {
        // allocate
        m_multi_chan = new Chuck_UGen *[m_multi_chan_size];
        // zero it out, whoever call this will fill in
        memset( m_multi_chan, 0, m_multi_chan_size * sizeof(Chuck_UGen *) );
    }
    else
        m_multi_chan_size = 0;
    
    // remember
    m_num_ins = num_ins;
    m_num_outs = num_outs;
}




//-----------------------------------------------------------------------------
// name: set_max_src()
// dsec: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_UGen::set_max_src( t_CKUINT num )
{
    m_max_src = num;
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: get_num_src()
// desc: ...
//-----------------------------------------------------------------------------
t_CKUINT Chuck_UGen::get_num_src()
{
    return m_num_src;
}




//-----------------------------------------------------------------------------
// name: add()
// dsec: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_UGen::add( Chuck_UGen * src )
{
    // examine ins and outs
    t_CKUINT outs = src->m_num_outs;
    t_CKUINT ins = this->m_num_ins;

    if( outs == 1 && ins == 1 )
    {
        if( m_num_src >= m_max_src )
            return FALSE;

        // append
        m_num_src++;
        m_src_list.push_back( src );
        src->add_ref();
        src->add_by( this );
    }
    else if( outs == 2 && ins == 2 )
    {
        // add to each channel
        if( !this->m_multi_chan[0]->add( src->m_multi_chan[0] ) ) return FALSE;
        if( !this->m_multi_chan[1]->add( src->m_multi_chan[1] ) ) return FALSE;
    }
    else if( outs == 1 && ins == 2 )
    {
        // add to each channel
        if( !this->m_multi_chan[0]->add( src ) ) return FALSE;
        if( !this->m_multi_chan[1]->add( src ) ) return FALSE;
    }
    else if( outs == 2 && ins == 1 )
    {
        if( m_num_src >= m_max_src )
            return FALSE;

        // append
        m_num_src++;
        m_src_list.push_back( src );
        src->add_ref();
        src->add_by( this );
    }
    else
    {
        assert( FALSE );
    }

    return TRUE;
}




//-----------------------------------------------------------------------------
// name: add_by()
// dsec: ...
//-----------------------------------------------------------------------------
void Chuck_UGen::add_by( Chuck_UGen * dest )
{
    // append
    m_num_dest++;
    m_dest_list.push_back( dest );
}




//-----------------------------------------------------------------------------
// name: remove()
// dsec: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_UGen::remove( Chuck_UGen * src )
{
    // ins and outs
    t_CKUINT outs = src->m_num_outs;
    t_CKUINT ins = this->m_num_ins;
    t_CKBOOL ret = FALSE;

    // take action
    if( outs == 1 && ins == 1 )
    {
        if( m_num_src == 0 ) return FALSE;

        // remove
        for( unsigned int i = 0; i < m_num_src; i++ )
            if( m_src_list[i] == src )
            {
                ret = TRUE;
                for( unsigned int j = i+1; j < m_num_src; j++ )
                    m_src_list[j-1] = m_src_list[j];

                m_src_list[--m_num_src] = NULL;
                m_src_list.pop_back();
                src->remove_by( this );
                src->release();
            }
    }
    else if( outs == 2 && ins == 2 )
    {
        if( !m_multi_chan[0]->remove( src->m_multi_chan[0] ) ) return FALSE;
        if( !m_multi_chan[1]->remove( src->m_multi_chan[1] ) ) return FALSE;
        ret = TRUE;
    }
    else if( outs == 1 && ins == 2 )
    {
        if( !m_multi_chan[0]->remove( src ) ) return FALSE;
        if( !m_multi_chan[1]->remove( src ) ) return FALSE;
        ret = TRUE;
    }
    else if( outs == 2 && ins == 1 )
    {
        if( m_num_src == 0 ) return FALSE;

        // remove
        for( unsigned int i = 0; i < m_num_src; i++ )
            if( m_src_list[i] == src )
            {
                ret = TRUE;
                for( unsigned int j = i+1; j < m_num_src; j++ )
                    m_src_list[j-1] = m_src_list[j];

                m_src_list[--m_num_src] = NULL;
                m_src_list.pop_back();
                src->remove_by( this );
                src->release();
            }
    }

    return ret;
}




//-----------------------------------------------------------------------------
// name: remove_by()
// dsec: ...
//-----------------------------------------------------------------------------
void Chuck_UGen::remove_by( Chuck_UGen * dest )
{
    // remove
    for( unsigned int i = 0; i < m_num_dest; i++ )
        if( m_dest_list[i] == dest )
        {
            for( unsigned int j = i+1; j < m_num_dest; j++ )
                m_dest_list[j-1] = m_dest_list[j];

            m_dest_list[--m_num_dest] = NULL;
            m_dest_list.pop_back();
        }
}




//-----------------------------------------------------------------------------
// name: add()
// dsec: ...
//-----------------------------------------------------------------------------
void Chuck_UGen::remove_all( )
{
    assert( this->m_num_dest == 0 );
    
    // remove
    for( unsigned int i = 0; i < m_num_src; i++ )
        m_src_list[i]->release();

    m_num_src = 0;
    m_src_list.clear();
}




//-----------------------------------------------------------------------------
// name: disconnect()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_UGen::disconnect( t_CKBOOL recursive )
{
    // remove
    for( unsigned int i = 0; i < m_num_dest; i++ )
        m_dest_list[i]->remove( this );

    m_num_dest = 0;
    m_dest_list.clear();
    
    // disconnect src too?
    if( recursive )
        this->remove_all();
    
    return TRUE;
}




//-----------------------------------------------------------------------------
// name: tick()
// dsec: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_UGen::system_tick( t_CKTIME now )
{
    if( m_time >= now )
        return m_valid;

    m_last = m_current;
    // inc time
    m_time = now;
    m_sum = m_num_src && m_src_list[0]->system_tick( now ) ? 
            m_src_list[0]->m_current : 0.0f;

    // tick the src list
    t_CKUINT i; Chuck_UGen * ugen;
    for( i = 1; i < m_num_src; i++ )
    {
        ugen = m_src_list[i];
        if( ugen->m_time < now ) ugen->system_tick( now );
        if( ugen->m_valid )
        {
            if( m_op <= 1 )
                m_sum += ugen->m_current;
            else // special ops
            {
                switch( m_op )
                {
                case 2: m_sum -= ugen->m_current; break;
                case 3: m_sum *= ugen->m_current; break;
                case 4: m_sum /= ugen->m_current; break;
                default: m_sum += ugen->m_current; break;
                }
            }
        }
    }

    // tick multiple channels
    for( i = 0; i < m_multi_chan_size; i++ )
    {
        ugen = m_multi_chan[i];
        if( ugen->m_time < now ) ugen->system_tick( now );
        if( ugen->m_valid )
        {
            // multiple channels are added
            m_sum += ugen->m_current;
        }
    }

    // if owner
    if( owner != NULL && owner->m_time < now )
        owner->system_tick( now );

    if( m_op > 0 )  // UGEN_OP_TICK
    {
        // tick the ugen
        if( tick ) m_valid = tick( this, m_sum, &m_current );
        if( !m_valid ) m_current = 0.0f;
        m_current *= m_gain * m_pan;
        return m_valid;
    }
    else if( m_op < 0 ) // UGEN_OP_PASS
    {
        // pass through
        m_current = m_sum;
        return TRUE;
    }
    else // UGEN_OP_STOP
        m_current = 0.0f;
    
    return TRUE;
}


