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
// date: Autumn 2004
//-----------------------------------------------------------------------------
#include "chuck_oo.h"
#include "chuck_type.h"
#include "chuck_vm.h"
#include "chuck_instr.h"




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
    // set v ref
    m_v_ref = NULL;
    // add to vm allocator
    Chuck_VM_Alloc::instance()->add_object( this );
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
    //if( m_ref_count == 1 )
    //{
    //    // add to vm allocator
    //    Chuck_VM_Alloc::instance()->add_object( this );
    //}
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
        // tell the object manager to set this free
        Chuck_VM_Alloc::instance()->free_object( this );
    }
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
    assert( capacity > 0 );
    // set capacity
    m_vector.resize( capacity );
    // reset size
    m_size = 0;
    // set capacity
    m_capacity = capacity;
    // is object
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
    if( i < 0 || i >= m_capacity )
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
    if( i < 0 || i >= m_capacity )
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
    if( i < 0 || i >= m_capacity )
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
    t_CKUINT v = m_map[key];

    // if obj
    if( m_is_obj && v ) ((Chuck_Object *)v)->release();

    if( !v ) m_map.erase( key );
    else m_map[key] = val;

    // if obj
    if( m_is_obj && val ) ((Chuck_Object *)val)->add_ref();

    // return good
    return 1;
}




//-----------------------------------------------------------------------------
// name: erase()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::erase( const string & key )
{
    return m_map.erase( key );
}




//-----------------------------------------------------------------------------
// name: push_back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::push_back( t_CKUINT val )
{
    // check
    if( m_size + 1 < 0 )
        return 0;

    // add to vector
    m_vector.push_back( val );
    // reset capacity
    m_capacity = m_vector.capacity();
    
    // track size
    m_size++;

    return 1;
}




//-----------------------------------------------------------------------------
// name: pop_back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::pop_back( )
{
    // check
    if( m_size == 0 )
        return 0;

    // add to vector
    m_vector.pop_back();
    
    // track size
    m_size--;

    return 1;
}




//-----------------------------------------------------------------------------
// name: back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array4::back( t_CKUINT * val ) const
{
    // check
    if( m_size == 0 )
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
    // clear vector
    m_vector.clear();

    // set size
    m_size = 0;
}




//-----------------------------------------------------------------------------
// name: Chuck_Array8()
// desc: constructor
//-----------------------------------------------------------------------------
Chuck_Array8::Chuck_Array8( t_CKINT capacity )
{
    // sanity check
    assert( capacity > 0 );
    // set capacity
    m_vector.resize( capacity );
    // reset size
    m_size = 0;
    // set capacity
    m_capacity = capacity;
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
    if( i < 0 || i >= m_capacity )
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
    if( i < 0 || i >= m_capacity )
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
    if( i < 0 || i >= m_capacity )
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
    if( val == 0.0 ) m_map.erase( key );
    else m_map[key] = val;

    // return good
    return 1;
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
    // check
    if( m_size + 1 < 0 )
        return 0;

    // add to vector
    m_vector.push_back( val );
    // get capacity
    m_capacity = m_vector.capacity();
    
    // track size
    m_size++;

    return 1;
}




//-----------------------------------------------------------------------------
// name: pop_back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::pop_back( )
{
    // check
    if( m_size == 0 )
        return 0;

    // add to vector
    m_vector.pop_back();
    
    // track size
    m_size--;

    return 1;
}




//-----------------------------------------------------------------------------
// name: back()
// desc: ...
//-----------------------------------------------------------------------------
t_CKINT Chuck_Array8::back( t_CKFLOAT * val ) const
{
    // check
    if( m_size == 0 )
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
    // clear vector
    m_vector.clear();

    // set size
    m_size = 0;
}




//-----------------------------------------------------------------------------
// name: signal()
// desc: signal a event/condition variable, shreduling the next waiting shred
//       (if there is one or more)
//-----------------------------------------------------------------------------
void Chuck_Event::signal()
{
	if( !m_queue.empty() )
	{
        Chuck_VM_Shred * shred = m_queue.front();
		m_queue.pop();
		Chuck_VM_Shreduler * shreduler = shred->vm_ref->shreduler();
		shred->event = NULL;
		shreduler->shredule( shred );
		// push the current time
        t_CKTIME *& sp = (t_CKTIME *&)shred->reg->sp;
		push_( sp, shreduler->now_system );
	}
}




//-----------------------------------------------------------------------------
// name: remove()
// desc: remove a shred from the event queue.
//-----------------------------------------------------------------------------
t_CKBOOL Chuck_Event::remove( Chuck_VM_Shred * shred )
{
	std::queue<Chuck_VM_Shred *> temp;
	t_CKBOOL removed = FALSE;

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
	return removed;
}




//-----------------------------------------------------------------------------
// name: broadcast()
// desc: broadcast a event/condition variable, shreduling all waiting shreds
//-----------------------------------------------------------------------------
void Chuck_Event::broadcast()
{
	while( !m_queue.empty() )
	{
        Chuck_VM_Shred * shred = m_queue.front();
		m_queue.pop();
		Chuck_VM_Shreduler * shreduler = shred->vm_ref->shreduler();
		shred->event = NULL;
		shreduler->shredule( shred );
		// push the current time
        t_CKTIME *& sp = (t_CKTIME *&)shred->reg->sp;
		push_( sp, shreduler->now_system );
	}
}




//-----------------------------------------------------------------------------
// name: wait()
// desc: cause event/condition variable to block the current shred, putting it
//       on its waiting list, and suspennd the shred from the VM.
//-----------------------------------------------------------------------------
void Chuck_Event::wait( Chuck_VM_Shred * shred, Chuck_VM * vm )
{
	// make sure the shred info matches the vm
	assert( shred->vm_ref == vm );

    // suspend
    shred->is_running = FALSE;

	// add to waiting list
	m_queue.push( shred );

	// add event to shred
	assert( shred->event == NULL );
	shred->event = this;
}
