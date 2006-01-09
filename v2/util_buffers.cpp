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
// file: util_buffers.cpp
// desc: buffer implementation
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Ananya Misra (amisra@cs.princeton.edu)
// date: Spring 2004
//       Summer 2005 - updated to allow many readers
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include "util_buffers.h"




//-----------------------------------------------------------------------------
// name: CBufferAdvance()
// desc: constructor
//-----------------------------------------------------------------------------
CBufferAdvance::CBufferAdvance()
{
    m_data = NULL;
    m_data_width = m_write_offset = m_max_elem = 0; // = m_read_offset
}




//-----------------------------------------------------------------------------
// name: ~CBufferAdvance()
// desc: destructor
//-----------------------------------------------------------------------------
CBufferAdvance::~CBufferAdvance()
{
    this->cleanup();
}




//-----------------------------------------------------------------------------
// name: initialize()
// desc: initialize
//-----------------------------------------------------------------------------
BOOL__ CBufferAdvance::initialize( UINT__ num_elem, UINT__ width )
{
    // cleanup
    cleanup();

    // allocate
    m_data = (BYTE__ *)malloc( num_elem * width );
    if( !m_data )
        return false;

    m_data_width = width;
    //m_read_offset = 0;
    m_write_offset = 0;
    m_max_elem = (SINT__)num_elem;

    return true;
}




//-----------------------------------------------------------------------------
// name: cleanup()
// desc: cleanup
//-----------------------------------------------------------------------------
void CBufferAdvance::cleanup()
{
    if( !m_data )
        return;

    free( m_data );

    m_data = NULL;
    m_data_width = 0;
    m_write_offset = m_max_elem = 0; // = m_read_offset
}




//-----------------------------------------------------------------------------
// name: UINT__ join()
// desc: shred can call this to get an index into the vector of read pointers
//-----------------------------------------------------------------------------
UINT__ CBufferAdvance::join( Chuck_Event * event )
{
    // index of new pointer that will be pushed back
    UINT__ read_offset_index;
    
    // add new pointer pointing (as pointers do) to current write offset
    // (shreds don't get interrupted, so m_write_offset will always be correct, right?)
    // (uh, hope so...)
    if( !m_free.empty() )
    {
        read_offset_index = m_free.front();
        m_free.pop();
        //assert( read_offset_index < m_read_offsets.size() );
        m_read_offsets[read_offset_index] = ReadOffset( m_write_offset, event );
    }
    else
    {
        read_offset_index = m_read_offsets.size();
        m_read_offsets.push_back( ReadOffset( m_write_offset, event ) );
    }

    // return index
    return read_offset_index;
}


//-----------------------------------------------------------------------------
// name: resign
// desc: shred quits buffer; frees its index
//-----------------------------------------------------------------------------
void CBufferAdvance::resign( UINT__ read_offset_index )
{
    // make sure read_offset_index passed in is valid
    if( read_offset_index < 0 || read_offset_index >= m_read_offsets.size() )
        return;
    
    // add this index to free queue
    m_free.push( read_offset_index );

    // "invalidate" the pointer at that index
    m_read_offsets[read_offset_index].read_offset = -1;
}


//-----------------------------------------------------------------------------
// name: put()
// desc: put
//-----------------------------------------------------------------------------
/*void CBufferAdvance::put( void * data, UINT__ num_elem )
{
    UINT__ i, j;
    BYTE__ * d = (BYTE__ *)data;

    // copy
    for( i = 0; i < num_elem; i++ )
    {
        for( j = 0; j < m_data_width; j++ )
        {
            m_data[m_write_offset*m_data_width+j] = d[i*m_data_width+j];
        }

        // move the write
        m_write_offset++;

        // wrap
        if( m_write_offset >= m_max_elem )
            m_write_offset = 0;
    }
}*/


void CBufferAdvance::put( void * data, UINT__ num_elem )
{
    UINT__ i, j;
    BYTE__ * d = (BYTE__ *)data;

    // copy
    for( i = 0; i < num_elem; i++ )
    {
        for( j = 0; j < m_data_width; j++ )
        {
            m_data[m_write_offset*m_data_width+j] = d[i*m_data_width+j];
        }

        // move the write
        m_write_offset++;
        // wrap
        if( m_write_offset >= m_max_elem )
            m_write_offset = 0;

        // possibility of expelling evil shreds
        for( j = 0; j < m_read_offsets.size(); j++ )
        {
            if( m_write_offset == m_read_offsets[j].read_offset )
            {
                // inform shred with index j that it has lost its privileges?
                // invalidate its read_offset
                // m_read_offsets[j].read_offset = -1;
            }

            if( m_read_offsets[j].event )
                m_read_offsets[j].event->queue_broadcast();
        }
    }
}




//-----------------------------------------------------------------------------
// name: get()
// desc: get
//-----------------------------------------------------------------------------
/*UINT__ CBufferAdvance::get( void * data, UINT__ num_elem )
{
    UINT__ i, j;
    BYTE__ * d = (BYTE__ *)data;

    // read catch up with write
    if( m_read_offset == m_write_offset )
        return 0;

    // copy
    for( i = 0; i < num_elem; i++ )
    {
        for( j = 0; j < m_data_width; j++ )
        {
            d[i*m_data_width+j] = m_data[m_read_offset*m_data_width+j];
        }

        // move read
        m_read_offset++;
        
        // catch up
        if( m_read_offset == m_write_offset )
        {
            i++;
            break;
        }

        // wrap
        if( m_read_offset >= m_max_elem )
            m_read_offset = 0;
    }

    // return number of elems
    return 1;
}*/


BOOL__ CBufferAdvance::empty( UINT__ read_offset_index )
{
    // make sure index is valid
    if( read_offset_index >= m_read_offsets.size() )
        return TRUE;
    if( m_read_offsets[read_offset_index].read_offset < 0 )
        return TRUE;

    SINT__ m_read_offset = m_read_offsets[read_offset_index].read_offset;

    // see if caught up
    return m_read_offset == m_write_offset;
}


UINT__ CBufferAdvance::get( void * data, UINT__ num_elem, UINT__ read_offset_index )
{
    UINT__ i, j;
    BYTE__ * d = (BYTE__ *)data;

    // make sure index is valid
    if( read_offset_index >= m_read_offsets.size() )
        return 0;
    if( m_read_offsets[read_offset_index].read_offset < 0 )
        return 0;

    SINT__ m_read_offset = m_read_offsets[read_offset_index].read_offset;

    // read catch up with write
    if( m_read_offset == m_write_offset )
        return 0;

    // copy
    for( i = 0; i < num_elem; i++ )
    {
        for( j = 0; j < m_data_width; j++ )
        {
            d[i*m_data_width+j] = m_data[m_read_offset*m_data_width+j];
        }

        // move read
        m_read_offset++;
        
        // catch up
        if( m_read_offset == m_write_offset )
        {
            i++;
            break;
        }

        // wrap
        if( m_read_offset >= m_max_elem )
            m_read_offset = 0;
    }

    // update read offset at given index
    m_read_offsets[read_offset_index].read_offset = m_read_offset;

    // return number of elems
    return i;
}




//-----------------------------------------------------------------------------
// name: CBufferSimple()
// desc: constructor
//-----------------------------------------------------------------------------
CBufferSimple::CBufferSimple()
{
    m_data = NULL;
    m_data_width = m_read_offset = m_write_offset = m_max_elem = 0;
}




//-----------------------------------------------------------------------------
// name: ~CBufferSimple()
// desc: destructor
//-----------------------------------------------------------------------------
CBufferSimple::~CBufferSimple()
{
    this->cleanup();
}




//-----------------------------------------------------------------------------
// name: initialize()
// desc: initialize
//-----------------------------------------------------------------------------
BOOL__ CBufferSimple::initialize( UINT__ num_elem, UINT__ width )
{
    // cleanup
    cleanup();

    // allocate
    m_data = (BYTE__ *)malloc( num_elem * width );
    if( !m_data )
        return false;

    m_data_width = width;
    m_read_offset = 0;
    m_write_offset = 0;
    m_max_elem = num_elem;

    return true;
}




//-----------------------------------------------------------------------------
// name: cleanup()
// desc: cleanup
//-----------------------------------------------------------------------------
void CBufferSimple::cleanup()
{
    if( !m_data )
        return;

    free( m_data );

    m_data = NULL;
    m_data_width = m_read_offset = m_write_offset = m_max_elem = 0;
}




//-----------------------------------------------------------------------------
// name: put()
// desc: put
//-----------------------------------------------------------------------------
void CBufferSimple::put( void * data, UINT__ num_elem )
{
    UINT__ i, j;
    BYTE__ * d = (BYTE__ *)data;

    // copy
    for( i = 0; i < num_elem; i++ )
    {
        for( j = 0; j < m_data_width; j++ )
        {
            m_data[m_write_offset*m_data_width+j] = d[i*m_data_width+j];
        }

        // move the write
        m_write_offset++;

        // wrap
        if( m_write_offset >= m_max_elem )
            m_write_offset = 0;
    }
}




//-----------------------------------------------------------------------------
// name: get()
// desc: get
//-----------------------------------------------------------------------------
UINT__ CBufferSimple::get( void * data, UINT__ num_elem )
{
    UINT__ i, j;
    BYTE__ * d = (BYTE__ *)data;

    // read catch up with write
    if( m_read_offset == m_write_offset )
        return 0;

    // copy
    for( i = 0; i < num_elem; i++ )
    {
        for( j = 0; j < m_data_width; j++ )
        {
            d[i*m_data_width+j] = m_data[m_read_offset*m_data_width+j];
        }

        // move read
        m_read_offset++;
        
        // catch up
        if( m_read_offset == m_write_offset )
        {
            i++;
            break;
        }

        // wrap
        if( m_read_offset >= m_max_elem )
            m_read_offset = 0;
    }

    // return number of elems
    return 1; // shouldn't it return i?
}
