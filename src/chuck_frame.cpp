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
// file: chuck_frame.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#include "chuck_frame.h"
extern "C" {
#include "chuck_symbol.h"
}



//-----------------------------------------------------------------------------
// name: struct F_Frame_
// desc: ...
//-----------------------------------------------------------------------------
struct F_Frame_
{
    Temp_Label label;
    F_Access_List head;
    F_Access_List tail;
    unsigned int num_access;
    unsigned int curr_offset;
};




//-----------------------------------------------------------------------------
// name: F_new_frame()
// desc: ...
//-----------------------------------------------------------------------------
F_Frame F_new_frame( Temp_Label name )
{
    F_Frame f = (F_Frame)checked_malloc( sizeof(F_Frame_) );
    f->label = name;
    f->tail = f->head = (F_Access_List)checked_malloc( sizeof(F_Access_List_) );
    f->head->head = NULL;
    f->head->tail = NULL;
    f->num_access = 0;
    f->curr_offset = 0;

    return f;
}




//-----------------------------------------------------------------------------
// name: F_name()
// desc: ...
//-----------------------------------------------------------------------------
Temp_Label F_name( F_Frame f )
{
    return f->label;
}




//-----------------------------------------------------------------------------
// name: F_formals()
// desc: ...
//-----------------------------------------------------------------------------
F_Access_List F_formals( F_Frame f )
{
    return f->head->tail;
}




//-----------------------------------------------------------------------------
// name: F_formals()
// desc: ...
//-----------------------------------------------------------------------------
unsigned int F_offset( F_Access a )
{
    return a->offset;
}




//-----------------------------------------------------------------------------
// name: F_alloc_local()
// desc: ...
//-----------------------------------------------------------------------------
F_Access F_alloc_local( F_Frame f, unsigned int size, unsigned int is_global )
{
    F_Access a = (F_Access)checked_malloc( sizeof( F_Access_ ) );
    a->offset = f->curr_offset;
    a->global = is_global;
    a->size = size;
    f->curr_offset += size;
    //f->num_access++;
    //f->tail->tail = (F_Access_List)checked_malloc( sizeof( F_Access_List_ ) );
    //f->tail->head = a;
    //f->tail = f->tail->tail;

    return a;
}




//-----------------------------------------------------------------------------
// name: F_begin_scope()
// desc: ...
//-----------------------------------------------------------------------------
void F_begin_scope( F_Frame f, S_table t )
{
    TAB_enter( t, insert_symbol("<mark>"), NULL );
}




//-----------------------------------------------------------------------------
// name: F_remove_locals()
// desc: ....
//-----------------------------------------------------------------------------
void F_end_scope( F_Frame f, S_table t )
{
    S_Symbol s = NULL;
    S_Symbol mark = insert_symbol("<mark>");
    F_Access v = NULL;
    do {
        v = (F_Access)TAB_topv(t);
        s = (S_Symbol)TAB_pop(t);
        if( s == mark ) break;
        assert(v);
        f->curr_offset -= v->size;
        free(v);
    }while( TRUE );
}




//-----------------------------------------------------------------------------
// name: F_stack_depth()
// desc: ...
//-----------------------------------------------------------------------------
unsigned int F_stack_depth( F_Frame f )
{
    return f->curr_offset;
}
