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
// file: chuck_oo.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Ananya Misra (amisra@cs.princeton.edu)
//         Andrew Schran (aschran@princeton.edu) - fileIO implementation
// date: Autumn 2004
//-----------------------------------------------------------------------------
#include "chuck_oo.h"
#include "chuck_type.h"
#include "chuck_vm.h"
#include "chuck_instr.h"
#include "chuck_errmsg.h"
#include "chuck_dl.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>
using namespace std;


// initialize
t_CKBOOL Chuck_VM_Object::our_locks_in_effect = TRUE;
const t_CKINT Chuck_IO::READ_INT32 = 0x1;
const t_CKINT Chuck_IO::READ_INT16 = 0x2;
const t_CKINT Chuck_IO::READ_INT8 = 0x4;
const t_CKINT Chuck_IO::MODE_SYNC = 0;
const t_CKINT Chuck_IO::MODE_ASYNC = 1;
const t_CKINT Chuck_IO_File::FLAG_READ_WRITE = 0x8;
const t_CKINT Chuck_IO_File::FLAG_READONLY = 0x10;
const t_CKINT Chuck_IO_File::FLAG_WRITEONLY = 0x20;
const t_CKINT Chuck_IO_File::FLAG_APPEND = 0x40;
const t_CKINT Chuck_IO_File::TYPE_ASCII = 0x80;
const t_CKINT Chuck_IO_File::TYPE_BINARY = 0x100;




//-----------------------------------------------------------------------------
// name: init_ref()
// desc: initialize vm object
//-----------------------------------------------------------------------------
void Chuck_VM_Object::init_ref()
{
    // set reference count
    m_ref_count = 0;
    // set flag
    m_pooled = FALSE;
    // set to not locked
    m_locked = FALSE;
    // set v ref
    m_v_ref = NULL;
    // add to vm allocator
    // Chuck_VM_Alloc::instance()->add_object( this );
}




//-----------------------------------------------------------------------------
// name: add_ref()
// desc: add reference
//-----------------------------------------------------------------------------
void Chuck_VM_Object::add_ref()
{
    // increment reference count
    m_ref_count++;

    // if going from 0 to 1
    if( m_ref_count == 1 )
    {
        // add to vm allocator
        Chuck_VM_Alloc::instance()->add_object( this );
    }
}




//-----------------------------------------------------------------------------
// name: release()
// desc: remove reference
//-----------------------------------------------------------------------------
void Chuck_VM_Object::release()
{
    // make sure there is at least one reference
    assert( m_ref_count > 0 );
    // decrement
    m_ref_count--;
    
    // if no more references
    if( m_ref_count == 0 )
    {
        // this is not good
        if( our_locks_in_effect && m_locked )
        {
            EM_error2( 0, "internal error: releasing locked VM object!" );
            // fail
            assert( FALSE );
            // in case assert is disabled
            *(int *)0 = 1;
        }

        // tell the object manager to set this free
        Chuck_VM_Alloc::instance()->free_object( this );
    }
}




//-----------------------------------------------------------------------------
// name: lock()
// desc: lock to keep from deleted
//-----------------------------------------------------------------------------
void Chuck_VM_Object::lock()
{
    m_locked = TRUE;
}




//-----------------------------------------------------------------------------
// name: lock_all()
// desc: disallow deletion of locked objects
//-----------------------------------------------------------------------------
void Chuck_VM_Object::lock_all()
{
    // log
    EM_log( CK_LOG_SYSTEM, "locking down special objects..." );
    // set flag
    our_locks_in_effect = TRUE;
}




//-----------------------------------------------------------------------------
// name: unlock_all()
// desc: allow deletion of locked objects (USE WITH CAUTION!)
//-----------------------------------------------------------------------------
void Chuck_VM_Object::unlock_all()
{
    // log
    EM_log( CK_LOG_SYSTEM, "unprotecting special objects..." );
    // set flag
    our_locks_in_effect = FALSE;
}




// static member
Chuck_VM_Alloc * Chuck_VM_Alloc::our_instance = NULL;


//-----------------------------------------------------------------------------
// name: instance()
// desc: return static instance
//-----------------------------------------------------------------------------
Chuck_VM_Alloc * Chuck_VM_Alloc::instance()
{
    if( !our_instance )
    {
        our_instance = new Chuck_VM_Alloc;
        assert( our_instance != NULL );
    }
    
    return our_instance;
}




//-----------------------------------------------------------------------------
// name: add_object()
// desc: add newly allocated vm object
//-----------------------------------------------------------------------------
void Chuck_VM_Alloc::add_object( Chuck_VM_Object * obj )
{
    // do log
    if( DO_LOG( CK_LOG_CRAZY ) )
    {
        // log it
        EM_log( CK_LOG_CRAZY, "adding '%s' (0x%lx)...",
            mini_type( typeid(*obj).name() ), obj );
    }

    // add it to map
}




//-----------------------------------------------------------------------------
// name: free_object()
// desc: free vm object - reference count should be 0
//-----------------------------------------------------------------------------
void Chuck_VM_Alloc::free_object( Chuck_VM_Object * obj )
{
    // make sure the ref count is 0
    assert( obj && obj->m_ref_count == 0 );

    // do log
    if( DO_LOG( CK_LOG_FINEST ) )
    {
        // log it
        EM_log( CK_LOG_FINEST, "freeing '%s' (0x%lx)...",
            mini_type( typeid(*obj).name() ), obj );
    }

    // remove it from map

    // delete it
    delete obj;
}




//-----------------------------------------------------------------------------
// name: Chuck_VM_Alloc()
// desc: constructor
//-----------------------------------------------------------------------------
Chuck_VM_Alloc::Chuck_VM_Alloc()
{ }




//-----------------------------------------------------------------------------
// name: ~Chuck_VM_Alloc()
// desc: destructor
//-----------------------------------------------------------------------------
Chuck_VM_Alloc::~Chuck_VM_Alloc()
{ }




//-----------------------------------------------------------------------------
// name: Chuck_Object()
// desc: constructor
//-----------------------------------------------------------------------------
Chuck_Object::Chuck_Object()
{
    // zero virtual table
    vtable = NULL;
    // zero type
    type_ref = NULL;
    // zero size
    size = 0;
    // zero data
    data = NULL;

    // add to vm allocator
    Chuck_VM_Alloc::instance()->add_object( this );
}




//-----------------------------------------------------------------------------
// name: Chuck_Object()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Object::~Chuck_Object()
{
    // free
    if( vtable ) { delete vtable; vtable = NULL; }
    if( type_ref ) { type_ref->release(); type_ref = NULL; }
    if( data ) { delete [] data; size = 0; data = NULL; }
}




//-----------------------------------------------------------------------------
// name: Chuck_Array4()
// desc: constructor
//-----------------------------------------------------------------------------
Chuck_Array4::Chuck_Array4( t_CKBOOL is_obj, t_CKINT capacity )
{
    // sanity check
    assert( capacity >= 0 );
    // set size
    m_vector.resize( capacity );
    // clear (as non-object, so no releases)
    m_is_obj = FALSE;
    this->zero( 0, m_vector.capacity() );
    // is object (set after clear)
    m_is_obj = is_obj;
}




//-----------------------------------------------------------------------------
// name: ~Chuck_Array4()
// desc: destructor
//-----------------------------------------------------------------------------
Chuck_Array4::~Chuck_Array4()
{
    // do nothing
}




//-----------------------------------------------------------------------------
// name: addr()
// desc: ...
//-----------------------------------------------------------------------------
t_CKUINT Chuck_Array4::addr( t_CKINT i )
{
    // bound check
    if( i < 0 || i >= m_vector.capacity() )
        return 0;

    // get the addr
    return (t_CKUINT)(&m_vector[i]);
}




//-----------------------------------------------------------------------------
// name: addr()
// desc: ...
//-----------------------------------------------------------------------------
t_CKUINT Chuck_Array4::addr( const string & key )
{
    // get the addr
    return (t_CKUINT)(&m_map[key]);
}




//-----------------------------------------------------------------------------
// name: get()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::get( t_CKINT i, t_CKUINT * val )
{
    // bound check
    if( i < 0 || i >= m_vector.capacity() )
        return 0;

    // get the value
    *val = m_vector[i];

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: get()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::get( const string & key, t_CKUINT * val )
{
    // set to zero
    *val = 0;
    // find
    map<string, t_CKUINT>::iterator iter = m_map.find( key );
    // check
    if( iter != m_map.end() ) *val = (*iter).second;

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: set()
// desc: include ref counting
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::set( t_CKINT i, t_CKUINT val )
{
    // bound check
    if( i < 0 || i >= m_vector.capacity() )
        return 0;

    t_CKUINT v = m_vector[i];

    // if obj
    if( m_is_obj && v ) ((Chuck_Object *)v)->release();

    // set the value
    m_vector[i] = val;

    // if obj
    if( m_is_obj && val ) ((Chuck_Object *)val)->add_ref();

    // return good
    return 1;
}





//-----------------------------------------------------------------------------
// name: set()
// desc: include ref counting
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::set( const string & key, t_CKUINT val )
{
    map<string, t_CKUINT>::iterator iter = m_map.find( key );

    // if obj
    if( m_is_obj && iter != m_map.end() ) 
        ((Chuck_Object *)(*iter).second)->release();

    if( !val ) m_map.erase( key );
    else m_map[key] = val;

    // if obj
    if( m_is_obj && val ) ((Chuck_Object *)val)->add_ref();

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: find()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::find( const string & key )
{
    return m_map.find( key ) != m_map.end();
}




//-----------------------------------------------------------------------------
// name: erase()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::erase( const string & key )
{
    map<string, t_CKUINT>::iterator iter = m_map.find( key );
    t_CKINT v = iter != m_map.end();

    // if obj
    if( m_is_obj && iter != m_map.end() ) 
        ((Chuck_Object *)(*iter).second)->release();

    // erase
    if( v ) m_map.erase( key );

    return v;
}




//-----------------------------------------------------------------------------
// name: push_back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::push_back( t_CKUINT val )
{
    // TODO: release reference or assume 0?

    // add to vector
    m_vector.push_back( val );

    return 1;
}




//-----------------------------------------------------------------------------
// name: pop_back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::pop_back( )
{
    // check
    if( m_vector.size() == 0 )
        return 0;

    // if obj
    if( m_is_obj )
    {
        Chuck_Object * v = (Chuck_Object *)m_vector[m_vector.size()-1];
        if( v ) v->release();
    }
    
    // zero
    m_vector[m_vector.size()-1] = 0;
    // add to vector
    m_vector.pop_back();
    
    return 1;
}




//-----------------------------------------------------------------------------
// name: back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::back( t_CKUINT * val ) const
{
    // check
    if( m_vector.size() == 0 )
        return 0;

    // get
    *val = m_vector.back();
    
    return 1;
}




//-----------------------------------------------------------------------------
// name: clear()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Array4::clear( )
{
    // zero
    zero( 0, m_vector.size() );

    // clear vector
    // m_vector.clear();
}




//-----------------------------------------------------------------------------
// name: set_capacity()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::set_capacity( t_CKINT capacity )
{
    // sanity check
    assert( capacity >= 0 );

    // ensure size
    set_size( capacity );

    return m_vector.capacity();
}




//-----------------------------------------------------------------------------
// name: set_size()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::set_size( t_CKINT size )
{
    // sanity check
    assert( size >= 0 );

    // if clearing size
    if( size < m_vector.size() )
    {
        // zero out section
        zero( size, m_vector.size() );
    }

    // what the size was
    t_CKINT size2 = m_vector.size();
    // resize vector
    m_vector.resize( size );

    // if clearing size
    if( m_vector.size() > size2 )
    {
        // zero out section
        zero( size2, m_vector.size() );
    }

    return m_vector.size();
}




//-----------------------------------------------------------------------------
// name: zero()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Array4::zero( t_CKUINT start, t_CKUINT end )
{
    // sanity check
    assert( start <= m_vector.capacity() && end <= m_vector.capacity() );

    // if contains objects
    if( m_is_obj )
    {
        Chuck_Object * v = NULL;
        for( t_CKUINT i = start; i < end; i++ )
        {
            // get it
            v = (Chuck_Object *)m_vector[i];
            // release
            if( v )
            {
                v->release();
                m_vector[i] = 0;
            }
        }
    }
    else
    {
        for( t_CKUINT i = start; i < end; i++ )
        {
            // zero
            m_vector[i] = 0;
        }
    }
}




//-----------------------------------------------------------------------------
// name: Chuck_Array8()
// desc: constructor
//-----------------------------------------------------------------------------
Chuck_Array8::Chuck_Array8( t_CKINT capacity )
{
    // sanity check
    assert( capacity >= 0 );
    // set size
    m_vector.resize( capacity );
    // clear
    this->zero( 0, m_vector.capacity() );
}




//-----------------------------------------------------------------------------
// name: ~Chuck_Array8()
// desc: destructor
//-----------------------------------------------------------------------------
Chuck_Array8::~Chuck_Array8()
{
    // do nothing
}




//-----------------------------------------------------------------------------
// name: addr()
// desc: ...
//-----------------------------------------------------------------------------
t_CKUINT Chuck_Array8::addr( t_CKINT i )
{
    // bound check
    if( i < 0 || i >= m_vector.capacity() )
        return 0;

    // get the addr
    return (t_CKUINT)(&m_vector[i]);
}




//-----------------------------------------------------------------------------
// name: addr()
// desc: ...
//-----------------------------------------------------------------------------
t_CKUINT Chuck_Array8::addr( const string & key )
{
    // get the addr
    return (t_CKUINT)(&m_map[key]);
}




//-----------------------------------------------------------------------------
// name: get()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::get( t_CKINT i, t_CKFLOAT * val )
{
    // bound check
    if( i < 0 || i >= m_vector.capacity() )
        return 0;

    // get the value
    *val = m_vector[i];

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: get()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::get( const string & key, t_CKFLOAT * val )
{
    // set to zero
    *val = 0.0;

    // iterator
    map<string, t_CKFLOAT>::iterator iter = m_map.find( key );

    // check
    if( iter != m_map.end() )
    {
        // get the value
        *val = (*iter).second;
    }

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: set()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::set( t_CKINT i, t_CKFLOAT val )
{
    // bound check
    if( i < 0 || i >= m_vector.capacity() )
        return 0;

    // set the value
    m_vector[i] = val;

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: set()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::set( const string & key, t_CKFLOAT val )
{
    map<string, t_CKFLOAT>::iterator iter = m_map.find( key );

    if( !val ) m_map.erase( key );
    else m_map[key] = val;

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: set()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::find( const string & key )
{
    return m_map.find( key ) != m_map.end();
}



//-----------------------------------------------------------------------------
// name: set()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::erase( const string & key )
{
    return m_map.erase( key );
}




//-----------------------------------------------------------------------------
// name: push_back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::push_back( t_CKFLOAT val )
{
    // add to vector
    m_vector.push_back( val );

    return 1;
}




//-----------------------------------------------------------------------------
// name: pop_back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::pop_back( )
{
    // check
    if( m_vector.size() == 0 )
        return 0;

    // zero
    m_vector[m_vector.size()-1] = 0.0;
    // add to vector
    m_vector.pop_back();
    
    return 1;
}




//-----------------------------------------------------------------------------
// name: back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::back( t_CKFLOAT * val ) const
{
    // check
    if( m_vector.size() == 0 )
        return 0;

    // get
    *val = m_vector.back();
    
    return 1;
}




//-----------------------------------------------------------------------------
// name: clear()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Array8::clear( )
{
    // zero
    zero( 0, m_vector.size() );

    // clear vector
    // m_vector.clear();
}




//-----------------------------------------------------------------------------
// name: set_capacity()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::set_capacity( t_CKINT capacity )
{
    // sanity check
    assert( capacity >= 0 );

    // ensure size
    set_size( capacity );

    return m_vector.capacity();
}




//-----------------------------------------------------------------------------
// name: set_size()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::set_size( t_CKINT size )
{
    // sanity check
    assert( size >= 0 );

    // if clearing size
    if( size < m_vector.size() )
    {
        // zero out section
        zero( size, m_vector.size() );
    }

    // what the size was
    t_CKINT size2 = m_vector.size();
    // resize vector
    m_vector.resize( size );

    // if clearing size
    if( m_vector.size() > size2 )
    {
        // zero out section
        zero( size2, m_vector.size() );
    }

    return m_vector.size();
}




//-----------------------------------------------------------------------------
// name: zero()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Array8::zero( t_CKUINT start, t_CKUINT end )
{
    // sanity check
    assert( start <= m_vector.capacity() && end <= m_vector.capacity() );

    for( t_CKUINT i = start; i < end; i++ )
    {
        // zero
        m_vector[i] = 0.0;
    }
}




//-----------------------------------------------------------------------------
// name: Chuck_Array16()
// desc: constructor
//-----------------------------------------------------------------------------
Chuck_Array16::Chuck_Array16( t_CKINT capacity )
{
    // sanity check
    assert( capacity >= 0 );
    // set size
    m_vector.resize( capacity );
    // clear
    this->zero( 0, m_vector.capacity() );
}




//-----------------------------------------------------------------------------
// name: ~Chuck_Array16()
// desc: destructor
//-----------------------------------------------------------------------------
Chuck_Array16::~Chuck_Array16()
{
    // do nothing
}




//-----------------------------------------------------------------------------
// name: addr()
// desc: ...
//-----------------------------------------------------------------------------
t_CKUINT Chuck_Array16::addr( t_CKINT i )
{
    // bound check
    if( i < 0 || i >= m_vector.capacity() )
        return 0;

    // get the addr
    return (t_CKUINT)(&m_vector[i]);
}




//-----------------------------------------------------------------------------
// name: addr()
// desc: ...
//-----------------------------------------------------------------------------
t_CKUINT Chuck_Array16::addr( const string & key )
{
    // get the addr
    return (t_CKUINT)(&m_map[key]);
}




//-----------------------------------------------------------------------------
// name: get()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::get( t_CKINT i, t_CKCOMPLEX * val )
{
    // bound check
    if( i < 0 || i >= m_vector.capacity() )
        return 0;

    // get the value
    *val = m_vector[i];

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: get()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::get( const string & key, t_CKCOMPLEX * val )
{
    // set to zero
    val->re = 0.0;
    val->im = 0.0;

    // iterator
    map<string, t_CKCOMPLEX>::iterator iter = m_map.find( key );

    // check
    if( iter != m_map.end() )
    {
        // get the value
        *val = (*iter).second;
    }

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: set()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::set( t_CKINT i, t_CKCOMPLEX val )
{
    // bound check
    if( i < 0 || i >= m_vector.capacity() )
        return 0;

    // set the value
    m_vector[i] = val;

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: set()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::set( const string & key, t_CKCOMPLEX val )
{
    map<string, t_CKCOMPLEX>::iterator iter = m_map.find( key );

    if( val.re == 0 && val.im == 0 ) m_map.erase( key );
    else m_map[key] = val;

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: set()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::find( const string & key )
{
    return m_map.find( key ) != m_map.end();
}



//-----------------------------------------------------------------------------
// name: set()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::erase( const string & key )
{
    return m_map.erase( key );
}




//-----------------------------------------------------------------------------
// name: push_back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::push_back( t_CKCOMPLEX val )
{
    // add to vector
    m_vector.push_back( val );
    
    return 1;
}




//-----------------------------------------------------------------------------
// name: pop_back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::pop_back( )
{
    // check
    if( m_vector.size() == 0 )
        return 0;

    // zero
    m_vector[m_vector.size()-1].re = 0.0;
    m_vector[m_vector.size()-1].im = 0.0;
    // add to vector
    m_vector.pop_back();
    
    return 1;
}




//-----------------------------------------------------------------------------
// name: back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::back( t_CKCOMPLEX * val ) const
{
    // check
    if( m_vector.size() == 0 )
        return 0;

    // get
    *val = m_vector.back();
    
    return 1;
}




//-----------------------------------------------------------------------------
// name: clear()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Array16::clear( )
{
    // zero
    zero( 0, m_vector.size() );

    // clear vector
    // m_vector.clear();
}




//-----------------------------------------------------------------------------
// name: set_capacity()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::set_capacity( t_CKINT capacity )
{
    // sanity check
    assert( capacity >= 0 );

    // ensure size
    set_size( capacity );

    return m_vector.capacity();
}




//-----------------------------------------------------------------------------
// name: set_size()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array16::set_size( t_CKINT size )
{
    // sanity check
    assert( size >= 0 );

    // if clearing size
    if( size < m_vector.size() )
    {
        // zero out section
        zero( size, m_vector.size() );
    }

    // remember
    t_CKINT size2 = m_vector.size();
    // resize vector
    m_vector.resize( size );

    // if clearing size
    if( m_vector.size() > size2 )
    {
        // zero out section
        zero( size2, m_vector.size() );
    }

    return m_vector.size();
}




//-----------------------------------------------------------------------------
// name: zero()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Array16::zero( t_CKUINT start, t_CKUINT end )
{
    // sanity check
    assert( start <= m_vector.capacity() && end <= m_vector.capacity() );

    for( t_CKUINT i = start; i < end; i++ )
    {
        // zero
        m_vector[i].re = 0.0;
        m_vector[i].im = 0.0;
    }
}




// static
t_CKUINT Chuck_Event::our_can_wait = 0;

//-----------------------------------------------------------------------------
// name: signal()
// desc: signal a event/condition variable, shreduling the next waiting shred
//       (if there is one or more)
//-----------------------------------------------------------------------------
void Chuck_Event::signal()
{
    m_queue_lock.acquire();
    if( !m_queue.empty() )
    {
        Chuck_VM_Shred * shred = m_queue.front();
        m_queue.pop();
        m_queue_lock.release();
        Chuck_VM_Shreduler * shreduler = shred->vm_ref->shreduler();
        shred->event = NULL;
        shreduler->remove_blocked( shred );
        shreduler->shredule( shred );
        // push the current time
        t_CKTIME *& sp = (t_CKTIME *&)shred->reg->sp;
        push_( sp, shreduler->now_system );
    }
    else
        m_queue_lock.release();
}




//-----------------------------------------------------------------------------
// name: remove()
// desc: remove a shred from the event queue.
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Event::remove( Chuck_VM_Shred * shred )
{
    queue<Chuck_VM_Shred *> temp;
    t_CKBOOL removed = FALSE;
    m_queue_lock.acquire();
    while( !m_queue.empty() )
    {
        if( m_queue.front() != shred )
            temp.push( m_queue.front() );
        else {
            shred->event = NULL;
            removed = TRUE;
        }
        m_queue.pop();
    }

    m_queue = temp;
    m_queue_lock.release();
    return removed;
}




//-----------------------------------------------------------------------------
// name: queue_broadcast()
// desc: queue the event to broadcast a event/condition variable, by the owner
//       of the queue
//-----------------------------------------------------------------------------
void Chuck_Event::queue_broadcast()
{
    // TODO: handle multiple VM
    m_queue_lock.acquire();
    if( !m_queue.empty() )
    {
        Chuck_VM_Shred * shred = m_queue.front();
        m_queue_lock.release();
        shred->vm_ref->queue_event( this, 1 );
    }
    else
        m_queue_lock.release();

}




//-----------------------------------------------------------------------------
// name: broadcast()
// desc: broadcast a event/condition variable, shreduling all waiting shreds
//-----------------------------------------------------------------------------
void Chuck_Event::broadcast()
{
    m_queue_lock.acquire();
    while( !m_queue.empty() )
    {
        m_queue_lock.release();
        this->signal();
        m_queue_lock.acquire();
    }
    m_queue_lock.release();
}




//-----------------------------------------------------------------------------
// name: wait()
// desc: cause event/condition variable to block the current shred, putting it
//       on its waiting list, and suspennd the shred from the VM.
//-----------------------------------------------------------------------------
void Chuck_Event::wait( Chuck_VM_Shred * shred, Chuck_VM * vm )
{
    EM_log( CK_LOG_FINE, "shred '%d' wait on event '%x'...", shred->xid, (t_CKUINT)this );
    // make sure the shred info matches the vm
    assert( shred->vm_ref == vm );
    
    Chuck_DL_Return RETURN;
    f_mfun canwaitplease = (f_mfun)this->vtable->funcs[our_can_wait]->code->native_func;
    canwaitplease( this, NULL, &RETURN, shred ); // TODO: check this is right shred
    // RETURN.v_int = 1;

    // see if we can wait
    if( RETURN.v_int )
    {
        // suspend
        shred->is_running = FALSE;

        // add to waiting list
        m_queue_lock.acquire();
        m_queue.push( shred );
        m_queue_lock.release();

        // add event to shred
        assert( shred->event == NULL );
        shred->event = this;

        // add shred to shreduler
        vm->shreduler()->add_blocked( shred );
    }
    else // can't wait
    {
        // push the current time
        t_CKTIME *& sp = (t_CKTIME *&)shred->reg->sp;
        push_( sp, shred->now );
    }
}




//-----------------------------------------------------------------------------
// name: Chuck_IO Constructor
// desc: Empty because you cannot construct a Chuck_IO object
//-----------------------------------------------------------------------------
Chuck_IO::Chuck_IO()
{ }




//-----------------------------------------------------------------------------
// name: Chuck_IO Destructor
// desc: Empty becuase you cannot destruct a Chuck_IO object
//-----------------------------------------------------------------------------
Chuck_IO::~Chuck_IO()
{ }




//-----------------------------------------------------------------------------
// name: Chuck_IO_File()
// desc: constructor
//-----------------------------------------------------------------------------
Chuck_IO_File::Chuck_IO_File()
{
    // zero things out
    m_flags = 0;
    m_iomode = MODE_SYNC;
    m_path = "";
    m_dir = NULL;
    m_asyncEvent = new Chuck_Event;
    initialize_object( m_asyncEvent, &t_event );
    m_thread = new XThread;
}




//-----------------------------------------------------------------------------
// name: ~Chuck_IO_File()
// desc: destructor
//-----------------------------------------------------------------------------
Chuck_IO_File::~Chuck_IO_File()
{
    // clean up
    this->close();
    delete m_asyncEvent;
    delete m_thread;
}




//-----------------------------------------------------------------------------
// name: open
// desc: open file from disk
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_IO_File::open( const string & path, t_CKINT flags )
{
    // log
    EM_log( CK_LOG_INFO, "FileIO: opening file from disk..." );
    EM_log( CK_LOG_INFO, "FileIO: path: %s", path.c_str() );
    EM_pushlog();
    
    // if no flag specified, make it READ by default
    if( !(flags & FLAG_READ_WRITE) &&
        !(flags & FLAG_READONLY) &&
        !(flags & FLAG_WRITEONLY) &&
        !(flags & FLAG_APPEND) )
    {
        flags |= FLAG_READONLY;
    }

    // if both read and write, enable read and write
    if( flags & FLAG_READONLY && flags & FLAG_WRITEONLY )
    {
        flags ^= FLAG_READONLY;
        flags ^= FLAG_WRITEONLY;
        flags |= FLAG_READ_WRITE;
    }
    
    // check flags for errors
    if ((flags & TYPE_ASCII) &&
        (flags & TYPE_BINARY)) {
        EM_error3( "[chuck](via FileIO): cannot open file in both ASCII and binary mode" );
        goto error;
    }
    
    if ((flags & FLAG_READ_WRITE) &&
        (flags & FLAG_READONLY)) {
        EM_error3( "[chuck](via FileIO): conflicting flags: READ_WRITE and READ" );
        goto error;
    }
    
    if ((flags & FLAG_READ_WRITE) &&
        (flags & FLAG_WRITEONLY)) {
        EM_error3( "[chuck](via FileIO): conflicting flags: READ_WRITE and WRITE" );
        goto error;
    }
    
    if ((flags & FLAG_READ_WRITE) &&
        (flags & FLAG_APPEND)) {
        EM_error3( "[chuck](via FileIO): conflicting flags: READ_WRITE and APPEND" );
        goto error;
    }
    
    if ((flags & FLAG_WRITEONLY) &&
        (flags & FLAG_READONLY)) {
        EM_error3( "[chuck](via FileIO): conflicting flags: WRITE and READ" );
        goto error;
    }
    
    if ((flags & FLAG_APPEND) &&
        (flags & FLAG_READONLY)) {
        EM_error3( "[chuck](via FileIO): conflicting flags: APPEND and FLAG_READ" );
        goto error;
    }
    
    // set open flags
    ios_base::openmode mode;
    
    if (flags & FLAG_READ_WRITE)
        mode = ios_base::in | ios_base::out;
    else if (flags & FLAG_READONLY)
        mode = ios_base::in;
    else if (flags & FLAG_APPEND)
        mode = ios_base::out | ios_base::app;
    else if (flags & FLAG_WRITEONLY)
        mode = ios_base::out | ios_base::trunc;
    
    if (flags & TYPE_BINARY)
        mode |= ios_base::binary;
    
    // close first
    if (m_io.is_open())
        this->close();
    
    // try to open as a dir first
    if (m_dir = opendir( path.c_str() ) ) {
        EM_poplog();
        return TRUE;
    }
    
    // not a dir, create file if it does not exist unless flag is
    // readonly
    if ( !(flags & FLAG_READONLY) )
    {
        m_io.open( path.c_str(), ios_base::in );
        if ( m_io.fail() )
        {
            m_io.clear();
            m_io.open( path.c_str(), ios_base::out | ios_base::trunc );
            m_io.close();
        }
        else
            m_io.close();
    }
    
    //open file
    m_io.open( path.c_str(), mode );
    
    // seek to beginning if necessary
    if (flags & FLAG_READ_WRITE)
    {
        m_io.seekp(0);
        m_io.seekg(0);
    }
    
    /* ATODO: Ge's code
     // windows sucks for being creative in the wrong places
     #ifdef __PLATFORM_WIN32__
     // if( flags ^ Chuck_IO::TRUNCATE && flags | Chuck_IO::READ ) nMode |= ios::nocreate;
     m_io.open( path.c_str(), nMode );
     #else
     m_io.open( path.c_str(), (_Ios_Openmode)nMode );
     #endif
     */
    
    // check for error
    if( !(m_io.is_open()) )
    {
        // EM_error3( "[chuck](via FileIO): cannot open file: '%s'", path.c_str() );
        goto error;
    }
    
    // set path
    m_path = path;
    // set flags
    m_flags = flags;
    if (!(flags & TYPE_BINARY))
        m_flags |= Chuck_IO_File::TYPE_ASCII; // ASCII is default
    // set mode
    m_iomode = MODE_SYNC;
    
    // pop
    EM_poplog();
    
    return TRUE;
    
error:
    
    // pop
    EM_poplog();
    
    // reset
    m_path = "";
    m_flags = 0;
    m_iomode = MODE_SYNC;
    m_io.clear();
    m_io.close();
    
    return FALSE;
}




//-----------------------------------------------------------------------------
// name: close
// desc: close file
//-----------------------------------------------------------------------------
void Chuck_IO_File::close()
{
    // log
    EM_log( CK_LOG_INFO, "FileIO: closing file '%s'...", m_path.c_str() );
    // close it
    m_io.close();
    m_flags = 0;
    m_path = "";
    m_iomode = Chuck_IO::MODE_SYNC;
    if ( m_dir ) {
        closedir( m_dir );
        m_dir = NULL;
    }
}




//-----------------------------------------------------------------------------
// name: good()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_IO_File::good()
{
    return m_dir || m_io.is_open();
    // return (!m_dir) && (!m_io.is_open());
}




//-----------------------------------------------------------------------------
// name: flush()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_IO_File::flush()
{
    // sanity
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot flush on directory" );
        return;
    }
    m_io.flush();
}




//-----------------------------------------------------------------------------
// name: mode()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_IO_File::mode()
{
    // sanity
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot get mode on directory" );
        return -1;
    }
    return m_iomode;
}




//-----------------------------------------------------------------------------
// name: mode( t_CKINT flag )
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_IO_File::mode( t_CKINT flag )
{    
    // sanity
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot set mode on directory" );
        return;
    }
    if ( (flag != Chuck_IO::MODE_ASYNC) && (flag != Chuck_IO::MODE_SYNC) )
    {
        EM_error3( "[chuck](via FileIO): invalid mode flag" );
        return;
    }
    
    m_iomode = flag;
}




//-----------------------------------------------------------------------------
// name: size()
// desc: Returns the size of the file in bytes, or -1 if no file is opened or
//       if a directory is opened.
//-----------------------------------------------------------------------------
t_CKINT Chuck_IO_File::size()
{
    if (!(m_io.is_open())) return -1;
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot get size on a directory" );
        return -1;
    }
    
    // no easy way to find file size in C++
    // have to seek to end, report position
    FILE * stream = fopen( m_path.c_str(), "r" );
    fseek( stream, 0L, SEEK_END );
    int endPos = ftell( stream );
    fclose( stream );
    return endPos;
}




//-----------------------------------------------------------------------------
// name: seek( t_CKINT pos )
// desc: Seeks to the specified byte offset in the file.
//-----------------------------------------------------------------------------
void Chuck_IO_File::seek( t_CKINT pos )
{
    if ( !(m_io.is_open()) )
    {
        EM_error3( "[chuck](via FileIO): cannot seek: no file is open" );
        return;
    }
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot seek on a directory" );
        return;
    }
    m_io.seekg( pos );
    m_io.seekp( pos );
}




//-----------------------------------------------------------------------------
// name: tell()
// desc: Returns the byte offset into the file, or -1 if no file is opened.
//-----------------------------------------------------------------------------
t_CKINT Chuck_IO_File::tell()
{
    if (!(m_io.is_open()))
        return -1;
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot tell on directory" );
        return -1;
    }
    
    return m_io.tellg();
}




//-----------------------------------------------------------------------------
// name: isDir()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_IO_File::isDir()
{
    return m_dir != NULL;
}




//-----------------------------------------------------------------------------
// name: dirList()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Array4 * Chuck_IO_File::dirList()
{
    // sanity
    if ( !m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot get list: no directory open" );
        Chuck_Array4 *ret = new Chuck_Array4( TRUE, 0 );
        initialize_object( ret, &t_array );
        return ret;
    }
    
    // fill vector with entry names
    rewinddir( m_dir );
    std::vector<Chuck_String *> entrylist;
    struct dirent *ent;
    while ( ent = readdir( m_dir ) )
    {
        Chuck_String *s = (Chuck_String *)instantiate_and_initialize_object( &t_string, NULL );
        s->str = std::string( ent->d_name );
        if ( s->str != ".." && s->str != "." )
            // don't include .. and . in the list
            entrylist.push_back( s );
    }
    
    // make array
    Chuck_Array4 *array = new Chuck_Array4( true, entrylist.size() );
    initialize_object( array, &t_array );
    for ( int i = 0; i < entrylist.size(); i++ )
        array->set( i, (t_CKUINT) entrylist[i] );
    return array;
}




//-----------------------------------------------------------------------------
// name: read( t_CKINT length )
// desc: ...
//-----------------------------------------------------------------------------
Chuck_String * Chuck_IO_File::read( t_CKINT length )
{
    // sanity
    if (!(m_io.is_open())) {
        EM_error3( "[chuck](via FileIO): cannot read: no file open" );
        return new Chuck_String( "" );
    }
    
    if (m_io.fail()) {
        EM_error3( "[chuck](via FileIO): cannot read: I/O stream failed" );
        return new Chuck_String( "" );
    }
    
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot read on a directory" );
        return new Chuck_String( "" );
    }
    
    char buf[length+1];
    m_io.read( buf, length );
    buf[m_io.gcount()] = '\0';
    string s( buf );
    return new Chuck_String( s );
}




//-----------------------------------------------------------------------------
// name: readLine()
// desc: read line
//-----------------------------------------------------------------------------
Chuck_String * Chuck_IO_File::readLine()
{
    // sanity
    if (!(m_io.is_open())) {
        EM_error3( "[chuck](via FileIO): cannot readLine: no file open" );
        return new Chuck_String( "" );
    }
    
    if (m_io.fail()) {
        EM_error3( "[chuck](via FileIO): cannot readLine: I/O stream failed" );
        return new Chuck_String( "" );
    }
    
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot readLine on directory" );
        return new Chuck_String( "" );
    }
    
    string s;
    getline( m_io, s );
    return new Chuck_String( s );
}




//-----------------------------------------------------------------------------
// name: readInt( t_CKINT flags )
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_IO_File::readInt( t_CKINT flags )
{
    // sanity
    if (!(m_io.is_open())) {
        EM_error3( "[chuck](via FileIO): cannot readInt: no file open" );
        return 0;
    }
    
    if (m_io.eof()) {
        EM_error3( "[chuck](via FileIO): cannot readInt: EOF reached" );
        return 0;
    }
    
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot read on directory" );
        return 0;
    }
    
    if (m_io.fail()) {
        EM_error3( "[chuck](via FileIO): cannot readInt: I/O stream failed" );
        return 0;
    }
    
    if (m_flags & TYPE_ASCII) {
        // ASCII
        t_CKINT val = 0;
        m_io >> val;
        if (m_io.fail())
            EM_error3( "[chuck](via FileIO): cannot readInt: I/O stream failed" );
        return val;
        
    } else if (m_flags & TYPE_BINARY) {
        // binary
        if (flags & Chuck_IO::READ_INT32) {
            // 32-bit
            t_CKINT i;
            m_io.read( (char *)&i, 4 );
            if (m_io.gcount() != 4)
                EM_error3( "[chuck](via FileIO): cannot readInt: not enough bytes left" );
            else if (m_io.fail())
                EM_error3( "[chuck](via FileIO): cannot readInt: I/O stream failed" );
            return i;
        } else if (flags & Chuck_IO::READ_INT16) {
            // 16-bit
            int16_t i;
            m_io.read( (char *)&i, 2 );
            if (m_io.gcount() != 2)
                EM_error3( "[chuck](via FileIO): cannot readInt: not enough bytes left" );
            else if (m_io.fail())
                EM_error3( "[chuck](via FileIO): cannot readInt: I/O stream failed" );
            return (t_CKINT) i;
        } else if (flags & Chuck_IO::READ_INT8) {
            // 8-bit
            int8_t i;
            m_io.read( (char *)&i, 1 );
            if (m_io.gcount() != 1)
                EM_error3( "[chuck](via FileIO): cannot readInt: not enough bytes left" );
            else if (m_io.fail())
                EM_error3( "[chuck](via FileIO): cannot readInt: I/O stream failed" );
            return (t_CKINT) i;
        } else {
            EM_error3( "[chuck](via FileIO): readInt error: invalid int size flag" );
            return 0;
        }
        
    } else {
        EM_error3( "[chuck](via FileIO): readInt error: invalid ASCII/binary flag" );
        return 0;
    }
}




//-----------------------------------------------------------------------------
// name: readFloat()
// desc: read next as (ascii) floating point value
//-----------------------------------------------------------------------------
t_CKFLOAT Chuck_IO_File::readFloat()
{
    // sanity
    if (!(m_io.is_open())) {
        EM_error3( "[chuck](via FileIO): cannot readFloat: no file open" );
        return 0;
    }
    
    if (m_io.eof()) {
        EM_error3( "[chuck](via FileIO): cannot readFloat: EOF reached" );
        return 0;
    }
    
    if (m_io.fail()) {
        EM_error3( "[chuck](via FileIO): cannot readFloat: I/O stream failed" );
        return 0;
    }
    
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot read a directory" );
        return 0;
    }
    
    if (m_flags & TYPE_ASCII) {
        // ASCII
        t_CKFLOAT val = 0;
        m_io >> val;
        if (m_io.fail())
            EM_error3( "[chuck](via FileIO): cannot readFloat: I/O stream failed" );
        return val;
        
    } else if (m_flags & TYPE_BINARY) {
        // binary
        t_CKFLOAT i;
        m_io.read( (char *)&i, sizeof(t_CKFLOAT) );
        if (m_io.gcount() != sizeof(t_CKFLOAT) )
            EM_error3( "[chuck](via FileIO): cannot readFloat: not enough bytes left" );
        else if (m_io.fail())
            EM_error3( "[chuck](via FileIO): cannot readInt: I/O stream failed" );
        return i;
        
    } else {
        EM_error3( "[chuck](via FileIO): readFloat error: invalid ASCII/binary flag" );
        return 0;
    }
}

/* (ATODO: doesn't look like asynchronous reads will work)
 
 THREAD_RETURN ( THREAD_TYPE Chuck_IO_File::read_thread ) ( void *data )
 {
 // (ATODO: test this)
 cerr << "In thread" << endl;
 async_args *args = (async_args *)data;
 Chuck_String *ret = (Chuck_String *)(args->RETURN);
 ret = args->fileio_obj->read( args->intArg );
 cerr << "Called FileIO.read(int)" << endl;
 args->fileio_obj->m_asyncEvent->broadcast(); // wake up
 cerr << "Broadcasted on event" << endl;
 cerr << "Sleeping" << endl;
 sleep(5);
 cerr << "Woke up" << endl;
 delete args;
 cerr << "Deleted args" << endl;
 
 return (THREAD_RETURN)0;
 }
 
 THREAD_RETURN ( THREAD_TYPE Chuck_IO_File::readLine_thread ) ( void *data )
 {
 // not yet implemented
 return NULL;
 }
 
 THREAD_RETURN ( THREAD_TYPE Chuck_IO_File::readInt_thread ) ( void *data )
 {
 // (ATODO: test this)
 async_args *args = (async_args *)data;
 Chuck_DL_Return *ret = (Chuck_DL_Return *)(args->RETURN);
 ret->v_int = args->fileio_obj->readInt( args->intArg );
 cerr << "Called readInt, set ret->v_int to " << ret->v_int << endl;
 args->fileio_obj->m_asyncEvent->broadcast(); // wake up
 cerr << "Called broadcast" << endl;
 delete args;
 
 return (THREAD_RETURN)0;
 }
 
 THREAD_RETURN ( THREAD_TYPE Chuck_IO_File::readFloat_thread ) ( void *data )
 {
 // not yet implemented
 return NULL;
 }
 */

//-----------------------------------------------------------------------------
// name: eof()
// desc: end of file?
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_IO_File::eof()
{
    if( !m_io.is_open() )
    {
        // EM_error3( "[chuck](via FileIO): cannot check eof: no file open" );
        return TRUE;
    }
    if( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot check eof on directory" );
        return TRUE;
    }

    return m_io.eof();
}




//-----------------------------------------------------------------------------
// name: write( const std::string & val )
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_IO_File::write( const std::string & val )
{
    // sanity
    if (!(m_io.is_open())) {
        EM_error3( "[chuck](via FileIO): cannot write: no file open" );
        return;
    }
    
    if (m_io.fail()) {
        EM_error3( "[chuck](via FileIO): cannot write: I/O stream failed" );
        return;
    }
    
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot write to a directory" );
        return;
    }
    
    m_io.write( val.c_str(), val.size() );
    
    if (m_io.fail()) { // check both before and after write if stream is ok
        EM_error3( "[chuck](via FileIO): cannot write: I/O stream failed" );
    }
}




//-----------------------------------------------------------------------------
// name: write( t_CKINT val )
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_IO_File::write( t_CKINT val )
{
    // sanity
    if (!(m_io.is_open())) {
        EM_error3( "[chuck](via FileIO): cannot write: no file open" );
        return;
    }
    
    if (m_io.fail()) {
        EM_error3( "[chuck](via FileIO): cannot write: I/O stream failed" );
        return;
    }
    
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot write on directory" );
        return;
    }
    
    if (m_flags & TYPE_ASCII) {
        m_io << val;
    } else if (m_flags & TYPE_BINARY) {
        m_io.write( (char *)&val, sizeof(t_CKINT) );
    } else {
        EM_error3( "[chuck](via FileIO): write error: invalid ASCII/binary flag" );
    }
    
    if (m_io.fail()) { // check both before and after write if stream is ok
        EM_error3( "[chuck](via FileIO): cannot write: I/O stream failed" );
    }
}




//-----------------------------------------------------------------------------
// name: write( t_CKFLOAT val )
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_IO_File::write( t_CKFLOAT val )
{
    // sanity
    if (!(m_io.is_open())) {
        EM_error3( "[chuck](via FileIO): cannot write: no file open" );
        return;
    }
    
    if (m_io.fail()) {
        EM_error3( "[chuck](via FileIO): cannot write: I/O stream failed" );
        return;
    }
    
    if ( m_dir )
    {
        EM_error3( "[chuck](via FileIO): cannot write to a directory" );
        return;
    }
    
    if (m_flags & TYPE_ASCII) {
        m_io << val;
    } else if (m_flags & TYPE_BINARY) {
        m_io.write( (char *)&val, sizeof(t_CKFLOAT) );
    } else {
        EM_error3( "[chuck](via FileIO): write error: invalid ASCII/binary flag" );
    }
    
    if (m_io.fail()) { // check both before and after write if stream is ok
        EM_error3( "[chuck](via FileIO): cannot write: I/O stream failed" );
    }
}




// static helper functions for writing asynchronously
THREAD_RETURN ( THREAD_TYPE Chuck_IO_File::writeStr_thread ) ( void *data )
{
    async_args *args = (async_args *)data;
    args->fileio_obj->write ( args->stringArg );
    Chuck_Event *e = args->fileio_obj->m_asyncEvent;
    delete args;
    e->broadcast(); // wake up
    
    return (THREAD_RETURN)0;
}

THREAD_RETURN ( THREAD_TYPE Chuck_IO_File::writeInt_thread ) ( void *data )
{
    async_args *args = (async_args *)data;
    args->fileio_obj->write ( args->intArg );
    args->fileio_obj->m_asyncEvent->broadcast(); // wake up
    delete args;
    
    return (THREAD_RETURN)0;
}

THREAD_RETURN ( THREAD_TYPE Chuck_IO_File::writeFloat_thread ) ( void *data )
{
    async_args *args = (async_args *)data;
    args->fileio_obj->write ( args->floatArg );
    args->fileio_obj->m_asyncEvent->broadcast(); // wake up
    delete args;
    
    return (THREAD_RETURN)0;
}
