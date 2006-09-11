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
// file: chuck_instr.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#include <math.h>
#include <limits.h>

#include "chuck_type.h"
#include "chuck_instr.h"
#include "chuck_vm.h"
#include "chuck_ugen.h"
#include "chuck_bbq.h"
#include "chuck_dl.h"
#include "chuck_errmsg.h"

#include <typeinfo>
using namespace std;




//-----------------------------------------------------------------------------
// name: name()
// desc: ...
//-----------------------------------------------------------------------------
const char * Chuck_Instr::name() const
{
     return mini_type( typeid(*this).name() );
}




//-----------------------------------------------------------------------------
// name: handle_overflow()
// desc: stack overflow
//-----------------------------------------------------------------------------
static void handle_overflow( Chuck_VM_Shred * shred, Chuck_VM * vm )
{
    // we have a problem
    fprintf( stderr, 
        "[chuck](VM): Exception StackOverflow in shred[id=%d:%s]\n",
        shred->xid, shred->name.c_str() );
    // do something!
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Add_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp; 
    pop_( sp, 2 );
    push_( sp, val_(sp) + val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_PreInc_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKINT **& reg_sp = (t_CKINT **&)shred->reg->sp;
    t_CKINT *&  the_sp = (t_CKINT *&)shred->reg->sp;

    // pointer
    pop_( reg_sp, 1 );
    // increment value
    (**(reg_sp))++;
    // value on stack
    push_( the_sp, **(reg_sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_PostInc_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKINT **& reg_sp = (t_CKINT **&)shred->reg->sp;
    t_CKINT *&  the_sp = (t_CKINT *&)shred->reg->sp;
    t_CKINT *   ptr;

    // pointer
    pop_( reg_sp, 1 );
    // copy
    ptr = *reg_sp;
    // value on stack
    push_( the_sp, **(reg_sp) );
    // increment value
    (*(ptr))++;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_PreDec_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKINT **& reg_sp = (t_CKINT **&)shred->reg->sp;
    t_CKINT *&  the_sp = (t_CKINT *&)shred->reg->sp;

    // pointer
    pop_( reg_sp, 1 );
    // decrement value
    (**(reg_sp))--;
    // value on stack
    push_( the_sp, **(reg_sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_PostDec_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKINT **& reg_sp = (t_CKINT **&)shred->reg->sp;
    t_CKINT *&  the_sp = (t_CKINT *&)shred->reg->sp;
    t_CKINT *   ptr;

    // pointer
    pop_( reg_sp, 1 );
    // copy
    ptr = *reg_sp;
    // value on stack
    push_( the_sp, **(reg_sp) );
    // decrement value
    (*(ptr))--;
}




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Dec_int_Addr
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Dec_int_Addr::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // decrement value
    (*((t_CKINT *)(m_val)))--;
}




//-----------------------------------------------------------------------------
// name: exexute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Complement_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    (*(sp-1)) = ~(*(sp-1));
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mod_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp; 
    pop_( sp, 2 );
    push_( sp, val_(sp) % val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mod_int_Reverse::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp; 
    pop_( sp, 2 );
    push_( sp, val_(sp+1) % val_(sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Minus_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp; 
    pop_( sp, 2 );
    push_( sp, val_(sp) - val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Minus_int_Reverse::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp; 
    pop_( sp, 2 );
    push_( sp, val_(sp+1) - val_(sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Times_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) * val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Divide_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) / val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Divide_int_Reverse::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp+1) / val_(sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Add_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) + val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Minus_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) - val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Minus_double_Reverse::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp+1) - val_(sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Times_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) * val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Divide_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) / val_(sp+1) );
}





//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Divide_double_Reverse::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp+1) / val_(sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mod_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp; 
    pop_( sp, 2 );
    push_( sp, ::fmod( val_(sp), val_(sp+1) ) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mod_double_Reverse::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp; 
    pop_( sp, 2 );
    push_( sp, ::fmod( val_(sp+1), val_(sp) ) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Add_int_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT temp, *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    temp = **(t_CKINT **)(sp+1) += val_(sp);
    push_( sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mod_int_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT temp, *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    temp = **(t_CKINT **)(sp+1) %= val_(sp);
    push_( sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Minus_int_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT temp, *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    temp = **(t_CKINT **)(sp+1) -= val_(sp);
    push_( sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Times_int_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT temp, *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    temp = **(t_CKINT **)(sp+1) *= val_(sp);
    push_( sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Divide_int_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT temp, *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    temp = **(t_CKINT **)(sp+1) /= val_(sp);
    push_( sp, temp );
}



//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Add_double_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT temp;
    t_CKBYTE *& sp = (t_CKBYTE *&)shred->reg->sp;
    // pop value + pointer
    pop_( sp, sz_FLOAT + sz_UINT );
    // assign
    temp = **(t_CKFLOAT **)(sp+sz_FLOAT) += val_((t_CKFLOAT *&)sp);
    // push result
    push_( (t_CKFLOAT *&)sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Minus_double_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT temp;
    t_CKBYTE *& sp = (t_CKBYTE *&)shred->reg->sp;
    // pop value + pointer
    pop_( sp, sz_FLOAT + sz_UINT );
    // assign
    temp = **(t_CKFLOAT **)(sp+sz_FLOAT) -= val_((t_CKFLOAT *&)sp);
    // push result
    push_( (t_CKFLOAT *&)sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Times_double_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT temp;
    t_CKBYTE *& sp = (t_CKBYTE *&)shred->reg->sp;
    // pop value + pointer
    pop_( sp, sz_FLOAT + sz_UINT );
    // assign
    temp = **(t_CKFLOAT **)(sp+sz_FLOAT) *= val_((t_CKFLOAT *&)sp);
    // push result
    push_( (t_CKFLOAT *&)sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Divide_double_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT temp;
    t_CKBYTE *& sp = (t_CKBYTE *&)shred->reg->sp;
    // pop value + pointer
    pop_( sp, sz_FLOAT + sz_UINT );
    // assign
    temp = **(t_CKFLOAT **)(sp+sz_FLOAT) /= val_((t_CKFLOAT *&)sp);
    // push result
    push_( (t_CKFLOAT *&)sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mod_double_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT temp;
    t_CKBYTE *& sp = (t_CKBYTE *&)shred->reg->sp;
    // pop value + pointer
    pop_( sp, sz_FLOAT + sz_UINT );
    // assign
    temp = ::fmod( **(t_CKFLOAT **)(sp+sz_FLOAT), val_((t_CKFLOAT *&)sp) );
    **(t_CKFLOAT **)(sp+sz_FLOAT) = temp;
    // push result
    push_( (t_CKFLOAT *&)sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_Imm::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // push val into reg stack
    push_( reg_sp, m_val );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_Imm2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& reg_sp = (t_CKFLOAT *&)shred->reg->sp;

    // push val into reg stack
    push_( reg_sp, m_val );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Dup_Last::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // dup val into reg stack
    push_( reg_sp, *(reg_sp-1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Dup_Last2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& reg_sp = (t_CKFLOAT *&)shred->reg->sp;

    // dup val into reg stack
    push_( reg_sp, *(reg_sp-1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_Now::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKTIME *& reg_sp = (t_CKTIME *&)shred->reg->sp;

    // push val into reg stack
    push_( reg_sp, shred->now );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_Me::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // push val into reg stack
    push_( reg_sp, (t_CKUINT)shred );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_This::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;

    // push val into reg stack
    push_( reg_sp, *(mem_sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_Start::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKTIME *& reg_sp = (t_CKTIME *&)shred->reg->sp;

    // push val into reg stack
    push_( reg_sp, shred->start );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_Maybe::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& reg_sp = (t_CKINT *&)shred->reg->sp;

    // push val into reg stack
    float num = (float)rand() / (float)RAND_MAX;
    push_( reg_sp, num > .5 );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: push the value pointed to by m_val onto register stack
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_Deref::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    if( m_size == 4 ) // ISSUE: 64-bit
    {
        t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
        push_( reg_sp, *((t_CKUINT *)m_val) );
    }
    else
    {
        t_CKFLOAT *& reg_sp = (t_CKFLOAT *&)shred->reg->sp;
        push_( reg_sp, *((t_CKFLOAT *)m_val) );
    }
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: push value from memory stack to register stack
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_Mem::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)(base?shred->base_ref->stack:shred->mem->sp);
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // push mem stack content into reg stack
    push_( reg_sp, *((t_CKUINT *)(mem_sp + m_val)) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_Mem2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)(base?shred->base_ref->stack:shred->mem->sp);
    t_CKFLOAT *& reg_sp = (t_CKFLOAT *&)shred->reg->sp;

    // push mem stack content into reg stack
    push_( reg_sp, *((t_CKFLOAT *)(mem_sp + m_val)) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Push_Mem_Addr::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)(base?shred->base_ref->stack:shred->mem->sp);
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // push mem stack addr into reg stack
    push_( reg_sp, (t_CKUINT)(mem_sp + m_val) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Pop_Mem::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // pop word from reg stack
    pop_( reg_sp, 2 );
    // copy popped value into mem stack
    *((t_CKUINT *)(mem_sp + *(reg_sp+1) )) = *reg_sp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Pop_Word::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // pop word from reg stack 
    pop_( reg_sp, 1 );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Pop_Word2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& reg_sp = (t_CKFLOAT *&)shred->reg->sp;

    // pop word from reg stack 
    pop_( reg_sp, 1 );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Reg_Pop_Word3::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // pop word from reg stack 
    pop_( reg_sp, m_val );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mem_Set_Imm::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT * mem_sp = (t_CKUINT *)(shred->mem->sp + m_offset);
    
    // set
    *(mem_sp) = m_val;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mem_Set_Imm2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT * mem_sp = (t_CKFLOAT *)(shred->mem->sp + m_offset);
    
    // set
    *(mem_sp) = m_val;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mem_Push_Imm::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    
    // pop word from reg stack 
    push_( mem_sp, m_val );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mem_Push_Imm2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& mem_sp = (t_CKFLOAT *&)shred->mem->sp;
    
    // pop word from reg stack 
    push_( mem_sp, m_val );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mem_Pop_Word::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    
    // pop word from reg stack 
    pop_( mem_sp, 1 );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mem_Pop_Word2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& mem_sp = (t_CKFLOAT *&)shred->mem->sp;
    
    // pop word from reg stack 
    pop_( mem_sp, 1 );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Mem_Pop_Word3::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    
    // pop word from reg stack 
    pop_( mem_sp, m_val );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Lt_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) < val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Gt_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) > val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Le_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) <= val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Ge_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) >= val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Eq_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) == val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Neq_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) != val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Not_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    (*(sp-1)) = !(*(sp-1));
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Negate_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    (*(sp-1)) = -(*(sp-1));
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Negate_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    (*(sp-1)) = -(*(sp-1));
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Lt_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) < val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Gt_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) > val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Le_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) <= val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Ge_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) >= val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Eq_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) == val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Branch_Neq_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    if( val_(sp) != val_(sp+1) )
        shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_And::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) & val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_Or::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) | val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_Xor::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) ^ val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_Shift_Right::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) >> val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_Shift_Right_Reverse::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp+1) >> val_(sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_Shift_Left::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) << val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_Shift_Left_Reverse::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp+1) << val_(sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_And_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT temp, *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    temp = **(t_CKINT **)(sp+1) &= val_(sp);
    push_( sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_Or_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT temp, *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    temp = **(t_CKINT **)(sp+1) |= val_(sp);
    push_( sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_Xor_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT temp, *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    temp = **(t_CKINT **)(sp+1) ^= val_(sp);
    push_( sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_Shift_Right_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT temp, *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    temp = **(t_CKINT **)(sp+1) >>= val_(sp);
    push_( sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Binary_Shift_Left_Assign::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT temp, *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    temp = **(t_CKINT **)(sp+1) <<= val_(sp);
    push_( sp, temp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Lt_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) < val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Gt_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) > val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Le_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) <= val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Ge_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) >= val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Eq_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) == val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Neq_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) != val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Lt_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    t_CKUINT *& sp_uint = (t_CKUINT *&)sp;
    pop_( sp, 2 );
    push_( sp_uint, val_(sp) < val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Gt_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    t_CKUINT *& sp_uint = (t_CKUINT *&)sp;
    pop_( sp, 2 );
    push_( sp_uint, val_(sp) > val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Le_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    t_CKUINT *& sp_uint = (t_CKUINT *&)sp;
    pop_( sp, 2 );
    push_( sp_uint, val_(sp) <= val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Ge_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    t_CKUINT *& sp_uint = (t_CKUINT *&)sp;
    pop_( sp, 2 );
    push_( sp_uint, val_(sp) >= val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Eq_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    t_CKUINT *& sp_uint = (t_CKUINT *&)sp;
    pop_( sp, 2 );
    push_( sp_uint, val_(sp) == val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Neq_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    t_CKUINT *& sp_uint = (t_CKUINT *&)sp;
    pop_( sp, 2 );
    push_( sp_uint, val_(sp) != val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_And::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) && val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Or::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) || val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Goto::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    shred->next_pc = m_jmp;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Nop::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // no op
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_EOC::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // end the shred
    shred->is_done = TRUE;
    shred->is_running = FALSE;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: alloc local
//-----------------------------------------------------------------------------
void Chuck_Instr_Alloc_Word::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // zero out the memory stack
    *( (t_CKUINT *)(mem_sp + m_val) ) = 0;
    // push addr onto operand stack
    push_( reg_sp, (t_CKUINT)(mem_sp + m_val) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: alloc local
//-----------------------------------------------------------------------------
void Chuck_Instr_Alloc_Word2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // zero out the memory stack
    *( (t_CKFLOAT *)(mem_sp + m_val) ) = 0.0;
    // push addr onto operand stack
    push_( reg_sp, (t_CKUINT)(mem_sp + m_val) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: alloc member
//-----------------------------------------------------------------------------
void Chuck_Instr_Alloc_Member_Word::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // get the object
    Chuck_Object * obj = (Chuck_Object *)*(mem_sp);
    // zero out the memory stack
    *( (t_CKUINT *)(obj->data + m_val) ) = 0;
    // push addr onto operand stack
    push_( reg_sp, (t_CKUINT)(obj->data + m_val) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: alloc member
//-----------------------------------------------------------------------------
void Chuck_Instr_Alloc_Member_Word2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // get the object
    Chuck_Object * obj = (Chuck_Object *)*(mem_sp);
    // zero out the memory stack
    *( (t_CKFLOAT *)(obj->data + m_val) ) = 0.0;
    // push addr onto operand stack
    push_( reg_sp, (t_CKUINT)(obj->data + m_val) );
}




static Chuck_Instr_Func_Call g_func_call;
static Chuck_Instr_Func_Call_Member g_func_call_member( 0 );
//-----------------------------------------------------------------------------
// name: call_pre_constructor()
// desc: ...
//-----------------------------------------------------------------------------
inline void call_pre_constructor( Chuck_VM * vm, Chuck_VM_Shred * shred, 
                                  Chuck_VM_Code * pre_ctor, t_CKUINT stack_offset )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // sanity
    assert( pre_ctor != NULL );

    // first duplicate the top of the stack, which should be object pointer
    push_( reg_sp, *(reg_sp-1) );
    // push the pre constructor
    push_( reg_sp, (t_CKUINT)pre_ctor );
    // push the stack offset
    push_( reg_sp, stack_offset );

    // call the function
    if( pre_ctor->native_func != 0 )
        g_func_call_member.execute( vm, shred );
    else
        g_func_call.execute( vm, shred );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: object pre construct
//-----------------------------------------------------------------------------
void Chuck_Instr_Pre_Constructor::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    call_pre_constructor( vm, shred, pre_ctor, stack_offset );
}




//-----------------------------------------------------------------------------
// name: instantiate_object()
// desc: ...
//-----------------------------------------------------------------------------
t_CKBOOL initialize_object( Chuck_Object * object, Chuck_Type * type )
{
    // sanity
    assert( type != NULL );
    assert( type->info != NULL );

    // allocate virtual table
    object->vtable = new Chuck_VTable;
    if( !object->vtable ) goto out_of_memory;
    // copy the object's virtual table
    object->vtable->funcs = type->info->obj_v_table.funcs;
    // set the type reference
    // TODO: reference count
    object->type_ref = type;
    object->type_ref->add_ref();
    // get the size
    object->size = type->obj_size;
    // allocate memory
    if( object->size )
    {
        // check to ensure enough memory
        object->data = new t_CKBYTE[object->size];
        if( !object->data ) goto out_of_memory;
        // zero it out
        memset( object->data, 0, object->size );
    }
    else object->data = NULL;

    // special
    if( type->ugen_info )
    {
        // ugen
        Chuck_UGen * ugen = (Chuck_UGen *)object;
        if( type->ugen_info->tick ) ugen->tick = type->ugen_info->tick;
        if( type->ugen_info->pmsg ) ugen->pmsg = type->ugen_info->pmsg;
        // allocate multi chan
        ugen->alloc_multi_chan( type->ugen_info->num_ins, 
                                type->ugen_info->num_outs );
        // allocate the channels
        for( t_CKUINT i = 0; i < ugen->m_multi_chan_size; i++ )
        {
            // allocate ugen for each
            Chuck_Object * obj = instantiate_and_initialize_object(
                &t_ugen, ugen->shred );
            // cast to ugen
            ugen->m_multi_chan[i] = (Chuck_UGen *)obj;
            // additional reference count
            ugen->m_multi_chan[i]->add_ref();
            // owner
            ugen->m_multi_chan[i]->owner = ugen;
            // ref count
            ugen->add_ref();
        }
    }

    return TRUE;

out_of_memory:

    // we have a problem
    fprintf( stderr, 
        "[chuck](VM): OutOfMemory: while instantiating object '%s'\n",
        type->c_name() );

    // delete
    if( object ) SAFE_DELETE( object->vtable );

    // return FALSE
    return FALSE;
}




//-----------------------------------------------------------------------------
// name: instantiate_and_initialize_object()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Object * instantiate_and_initialize_object( Chuck_Type * type, Chuck_VM_Shred * shred )
{
    Chuck_Object * object = NULL;

    // sanity
    assert( type != NULL );
    assert( type->info != NULL );

    // allocate the VM object
    if( !type->ugen_info )
    {
        // check type TODO: make this faster
        if( isa( type, &t_event ) ) object = new Chuck_Event;
        else if( isa( type, &t_string ) ) object = new Chuck_String;
        else object = new Chuck_Object;
    }
    else
    {
        // make ugen
        Chuck_UGen * ugen;
        object = ugen = new Chuck_UGen;
        if( shred )
        {
            ugen->shred = shred;
            shred->add( ugen );
        }
    }
    
    // check to see enough memory
    if( !object ) goto out_of_memory;

    // initialize
    if( !initialize_object( object, type ) ) goto error;

    return object;

out_of_memory:

    // we have a problem
    fprintf( stderr, 
        "[chuck](VM): OutOfMemory: while instantiating object '%s'\n",
        type->c_name() );

error:

    // delete
    SAFE_DELETE( object );

    // return NULL
    return NULL;
}




//-----------------------------------------------------------------------------
// name: instantiate_object()
// desc: ...
//-----------------------------------------------------------------------------
inline void instantiate_object( Chuck_VM * vm, Chuck_VM_Shred * shred,
                                Chuck_Type * type )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // allocate the VM object
    Chuck_Object * object = instantiate_and_initialize_object( type, shred );
    if( !object ) goto error;

    // push the pointer on the operand stack
    push_( reg_sp, (t_CKUINT)object );

    // call preconstructor
    // call_pre_constructor( vm, shred, object, type, stack_offset );

    return;

error:

    // do something!
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: instantiate object
//-----------------------------------------------------------------------------
void Chuck_Instr_Instantiate_Object::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    instantiate_object( vm, shred, this->type  );
}




//-----------------------------------------------------------------------------
// name: params()
// desc: ...
//-----------------------------------------------------------------------------
const char * Chuck_Instr_Instantiate_Object::params() const
{
    static char buffer[256];
    sprintf( buffer, "%s", this->type->c_name() );
    return buffer;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: object pre construct top
//-----------------------------------------------------------------------------
void Chuck_Instr_Pre_Ctor_Array_Top::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // see if we are done with all elements in the array
    if( *(reg_sp-2) >= *(reg_sp-1) )
        shred->next_pc = m_val;
    else
    {
        // instantiate
        instantiate_object( vm, shred, type );
    }
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: object pre construct bottom
//-----------------------------------------------------------------------------
void Chuck_Instr_Pre_Ctor_Array_Bottom::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // pop the object
    pop_( reg_sp, 1 );
    
    // cast the object
    Chuck_Object * obj = (Chuck_Object *)(*(reg_sp));

    // assign object
    t_CKUINT * array = (t_CKUINT *)(*(reg_sp-3));
    // get the object pointer
    Chuck_Object ** dest = (Chuck_Object **)(array[*(reg_sp-2)]);
    // copy
    *dest = obj;
    // ref count
    obj->add_ref();
    // increment the index
    (*(reg_sp-2))++; //= (*(reg_sp-2)) + 1;

    // goto top
    shred->next_pc = m_val;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: object pre construct post
//-----------------------------------------------------------------------------
void Chuck_Instr_Pre_Ctor_Array_Post::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // pop the array, index, and size
    pop_( reg_sp, 3 );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: assign primitive (word)
//-----------------------------------------------------------------------------
void Chuck_Instr_Assign_Primitive::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // pop word from reg stack
    pop_( reg_sp, 2 );
    // copy popped value into mem stack
    *((t_CKUINT *)(*(reg_sp+1))) = *reg_sp;

    push_( reg_sp, *reg_sp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: assign primitive (2 word)
//-----------------------------------------------------------------------------
void Chuck_Instr_Assign_Primitive2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // pop word from reg stack
    pop_( reg_sp, 3 );
    // copy popped value into mem stack
    *( (t_CKFLOAT *)(*(reg_sp+2)) ) = *(t_CKFLOAT *)reg_sp;

    t_CKFLOAT *& sp_double = (t_CKFLOAT *&)reg_sp;
    push_( sp_double, *sp_double );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: assign object with reference counting and releasing previous reference
//-----------------------------------------------------------------------------
void Chuck_Instr_Assign_Object::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_VM_Object ** obj = NULL, * done = NULL;

    // pop word from reg stack
    pop_( reg_sp, 2 );
    // the previous reference
    obj = (Chuck_VM_Object **)(*(reg_sp+1));
    // save the reference (release should come after, in case same object)
    done = *obj;
    // copy popped value into memory
    *obj = (Chuck_VM_Object *)(*(reg_sp));
    // add reference
    if( *obj ) (*obj)->add_ref();
    // release
    if( done ) done->release();

    // copy
    // memcpy( (void *)*(reg_sp+1), *obj, sizeof(t_CKUINT) );
    // push the reference value to reg stack
    push_( reg_sp, (t_CKUINT)*obj );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: assign string
//-----------------------------------------------------------------------------
void Chuck_Instr_Assign_String::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_String * lhs = NULL;
    Chuck_String ** rhs_ptr = NULL;

    // pop word from reg stack
    pop_( reg_sp, 2 );
    // the previous reference
    rhs_ptr = (Chuck_String **)(*(reg_sp+1));
    // copy popped value into memory
    lhs = (Chuck_String *)(*(reg_sp));
    // release any previous reference
    if( *rhs_ptr )
    {
        if( lhs ) (*rhs_ptr)->str = lhs->str;
        else
        {
            // release reference
            (*rhs_ptr)->release();
            (*rhs_ptr) = NULL;
        }
    }
    else
    {
        // if left is not null, yes
        if( lhs != NULL )
        {
            (*rhs_ptr) = (Chuck_String *)instantiate_and_initialize_object( &t_string, shred );
            // add ref
            (*rhs_ptr)->add_ref();
            (*rhs_ptr)->str = lhs->str;
        }
        //EM_error2( 0, "internal error: somehow the type checker has allowed NULL strings" );
        //EM_error2( 0, "we are sorry for the inconvenience but..." );
        //EM_error2( 0, "we have to crash now.  Thanks." );
        //assert( FALSE );
    }

    // copy
    // memcpy( (void *)*(reg_sp+1), *obj, sizeof(t_CKUINT) );
    // push the reference value to reg stack
    push_( reg_sp, (t_CKUINT)*rhs_ptr );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: release one reference on object
//-----------------------------------------------------------------------------
void Chuck_Instr_Chuck_Release_Object::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_VM_Object * obj = NULL;

    // pop word from reg stack
    pop_( reg_sp, 1 );
    // copy popped value into mem stack
    obj = *( (Chuck_VM_Object **)(mem_sp + *(reg_sp)) );
    // release
    obj->release();
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Func_To_Code::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // get func
    Chuck_Func * func = (Chuck_Func *)*(reg_sp-1);
    // make sure
    assert( func != NULL );
    // code
    *(reg_sp-1) = (t_CKUINT)func->code;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Func_Call::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // pop word
    pop_( reg_sp, 2 );
    // get the function to be called as code
    Chuck_VM_Code * func = (Chuck_VM_Code *)*reg_sp;
    // get the local stack depth - caller local variables
    t_CKUINT local_depth = *(reg_sp+1);
    // convert to number of 4-byte words, extra partial word counts as additional word
    local_depth = ( local_depth >> 2 ) + ( local_depth & 0x3 ? 1 : 0 );
    // get the stack depth of the callee function args
    t_CKUINT stack_depth = ( func->stack_depth >> 2 ) + ( func->stack_depth & 0x3 ? 1 : 0 );
    // get the previous stack depth - caller function args
    t_CKUINT prev_stack = ( *(mem_sp-1) >> 2 ) + ( *(mem_sp-1) & 0x3 ? 1 : 0 );

    // jump the sp
    mem_sp += prev_stack + local_depth;
    // push the prev stack
    push_( mem_sp, prev_stack + local_depth );
    // push the current function
    push_( mem_sp, (t_CKUINT)shred->code );
    // push the pc
    push_( mem_sp, (t_CKUINT)(shred->pc + 1) );
    // push the stack depth
    push_( mem_sp, stack_depth );
    // set the pc to 0
    shred->next_pc = 0;
    // set the code
    shred->code = func;
    // set the instruction to the function instruction
    shred->instr = func->instr;

    // if there are arguments to be passed
    if( stack_depth )
    {
        // pop the arguments, by number of words
        pop_( reg_sp, stack_depth );

        // make copies
        t_CKUINT * mem_sp2 = (t_CKUINT *)mem_sp;
        t_CKUINT * reg_sp2 = (t_CKUINT *)reg_sp;

        // need this
        if( func->need_this )
        {
            // copy this from end of arguments to the front
            *mem_sp2++ = *(reg_sp2 + stack_depth - 1);
            // one less word to copy
            stack_depth--;
        }
        
        // push the arguments
        for( t_CKUINT i = 0; i < stack_depth; i++ )
            *mem_sp2++ = *reg_sp2++;
    }

    // detect overflow/underflow
    if( overflow_( shred->mem ) ) goto error_overflow;

    return;

error_overflow:
    
    handle_overflow( shred, vm );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: imported member function call with return
//-----------------------------------------------------------------------------
void Chuck_Instr_Func_Call_Member::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_DL_Return retval;

    // pop word
    pop_( reg_sp, 2 );
    // get the function to be called as code
    Chuck_VM_Code * func = (Chuck_VM_Code *)*reg_sp;
    // get the function to be called
    f_mfun f = (f_mfun)func->native_func;
    // get the local stack depth - caller local variables
    t_CKUINT local_depth = *(reg_sp+1);
    // convert to number of 4-byte words, extra partial word counts as additional word
    local_depth = ( local_depth >> 2 ) + ( local_depth & 0x3 ? 1 : 0 );
    // get the stack depth of the callee function args
    t_CKUINT stack_depth = ( func->stack_depth >> 2 ) + ( func->stack_depth & 0x3 ? 1 : 0 );
    // UNUSED: get the previous stack depth - caller function args
    // UNUSED: t_CKUINT prev_stack = ( *(mem_sp-1) >> 2 ) + ( *(mem_sp-1) & 0x3 ? 1 : 0 );
    // the amount to push in 4-byte words
    t_CKUINT push = local_depth;
    // push the mem stack passed the current function variables and arguments
    mem_sp += push;

    // pass args
    if( stack_depth )
    {
        // pop the arguments for pass to callee function
        reg_sp -= stack_depth;

        // make copies
        t_CKUINT * reg_sp2 = reg_sp;
        t_CKUINT * mem_sp2 = mem_sp;
        
        // need this
        if( func->need_this )
        {
            // copy this from end of arguments to the front
            *mem_sp2++ = *(reg_sp2 + stack_depth - 1);
            // one less word to copy
            stack_depth--;
        }
        // copy to args
        for( t_CKUINT i = 0; i < stack_depth; i++ )
            *mem_sp2++ = *reg_sp2++;
    }

    // detect overflow/underflow
    if( overflow_( shred->mem ) ) goto error_overflow;

    // call the function
    f( (Chuck_Object *)(*mem_sp), mem_sp + 1, &retval );
    mem_sp -= push;
    
    // push the return
    if( m_val == 4 ) // ISSUE: 64-bit
    {
        // push the return args
        push_( reg_sp, retval.v_uint );
    }
    else if( m_val == 8 ) // ISSUE: 64-bit
    {
        // push the return args
        t_CKFLOAT *& sp_double = (t_CKFLOAT *&)reg_sp;
        push_( sp_double, retval.v_float );
    }
    else if( m_val == 0 ) { }
    else assert( FALSE );

    return;

error_overflow:
    
    handle_overflow( shred, vm );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: imported static function call with return
//-----------------------------------------------------------------------------
void Chuck_Instr_Func_Call_Static::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_DL_Return retval;

    // pop word
    pop_( reg_sp, 2 );
    // get the function to be called as code
    Chuck_VM_Code * func = (Chuck_VM_Code *)*reg_sp;
    // get the function to be called
    f_sfun f = (f_sfun)func->native_func;
    // get the local stack depth - caller local variables
    t_CKUINT local_depth = *(reg_sp+1);
    // convert to number of 4-byte words, extra partial word counts as additional word
    local_depth = ( local_depth >> 2 ) + ( local_depth & 0x3 ? 1 : 0 );
    // get the stack depth of the callee function args
    t_CKUINT stack_depth = ( func->stack_depth >> 2 ) + ( func->stack_depth & 0x3 ? 1 : 0 );
    // UNUSED: get the previous stack depth - caller function args
    // UNUSED: t_CKUINT prev_stack = ( *(mem_sp-1) >> 2 ) + ( *(mem_sp-1) & 0x3 ? 1 : 0 );    
    // the amount to push in 4-byte words
    t_CKUINT push = local_depth;
    // push the mem stack passed the current function variables and arguments
    mem_sp += push;

    // pass args
    if( stack_depth )
    {
        // pop the arguments for pass to callee function
        reg_sp -= stack_depth;

        // make copies
        t_CKUINT * reg_sp2 = reg_sp;
        t_CKUINT * mem_sp2 = mem_sp;
        
        // need this
        if( func->need_this )
        {
            // copy this from end of arguments to the front
            *mem_sp2++ = *(reg_sp2 + stack_depth - 1);
            // advance reg pointer
            reg_sp2++;
            // one less word to copy
            stack_depth--;
        }
        // copy to args
        for( t_CKUINT i = 0; i < stack_depth; i++ )
            *mem_sp2++ = *reg_sp2++;
    }

    // detect overflow/underflow
    if( overflow_( shred->mem ) ) goto error_overflow;

    // call the function
    f( mem_sp, &retval );
    mem_sp -= push;

    // push the return
    if( m_val == 4 ) // ISSUE: 64-bit
    {
        // push the return args
        push_( reg_sp, retval.v_uint );
    }
    else if( m_val == 8 ) // ISSUE: 64-bit
    {
        // push the return args
        t_CKFLOAT *& sp_double = (t_CKFLOAT *&)reg_sp;
        push_( sp_double, retval.v_float );
    }
    else if( m_val == 0 ) { }
    else assert( FALSE );

    return;

error_overflow:
    
    handle_overflow( shred, vm );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Func_Return::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    // UNUSED: t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // pop pc
    pop_( mem_sp, 2 );
    shred->next_pc = *mem_sp;
    // pop the code
    pop_( mem_sp, 1 );
    Chuck_VM_Code * func = (Chuck_VM_Code *)*mem_sp;
    // pop the prev_stack
    pop_( mem_sp, 1 );
    // jump the prev stack
    mem_sp -= *(mem_sp);

    // set the shred 
    shred->code = func;
    shred->instr = func->instr;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Spork::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    t_CKUINT this_ptr = 0;

    // pop the stack
    pop_( reg_sp, 1 );
    // get the code
    Chuck_VM_Code * code = *(Chuck_VM_Code **)reg_sp;
    // spork it
    Chuck_VM_Shred * sh = vm->spork( code, shred );
    // pop the stack
    pop_( reg_sp, 1 );
    // get the func
    Chuck_Func * func = (Chuck_Func *)(*reg_sp);
    // need this?
    if( func->is_member )
    {
        // pop the stack
        pop_( reg_sp, 1 );
        // get this
        this_ptr = *reg_sp;
    }
    // copy args
    if( m_val )
    {
        pop_( shred->reg->sp, m_val );
        memcpy( sh->reg->sp, shred->reg->sp, m_val );
        sh->reg->sp += m_val;
    }
    // copy this, if need
    if( func->is_member )
    {
        push_( (t_CKUINT*&)sh->reg->sp, this_ptr );
    }
    // copy func
    push_( (t_CKUINT*&)sh->reg->sp, (t_CKUINT)func );
    // push the stack
    push_( reg_sp, (t_CKUINT)sh );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Time_Advance::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKTIME *& sp = (t_CKTIME *&)shred->reg->sp;

    // pop word from reg stack
    pop_( sp, 1 );

    if( *sp < shred->now )
    {
        // we have a problem
        fprintf( stderr, 
            "[chuck](VM): DestTimeNegativeException: '%.6f' in shred[id=%d:%s]\n",
            *sp, shred->xid, shred->name.c_str() );
        // do something!
        shred->is_running = FALSE;
        shred->is_done = TRUE;

        return;
    }

    // shredule the shred
    vm->shreduler()->shredule( shred, *sp );
    // suspend
    shred->is_running = FALSE;

    // track time advance
    CK_TRACK( Chuck_Stats::instance()->advance_time( shred, *sp ) );

    push_( sp, *sp );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Event_Wait::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;

    // pop word from reg stack
    pop_( sp, 1 );

    Chuck_Event * event = (Chuck_Event *)(*sp);

    // check for null
    if( !event ) goto null_pointer;

    // wait
    event->wait( shred, vm );

    return;

null_pointer:
    // we have a problem
    fprintf( stderr, 
        "[chuck](VM): NullPointerException: (null Event wait) in shred[id=%d:%s]\n",
        shred->xid, shred->name.c_str() );
    goto done;

done:
    // do something!
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}




//-----------------------------------------------------------------------------
// name: Chuck_Instr_Array_Init()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Instr_Array_Init::Chuck_Instr_Array_Init( Chuck_Type * t , t_CKINT length )
{
    // set
    m_length = length;
    // copy
    m_type_ref = t;
    // remember
    // m_type_ref->add_ref();
    // type
    m_param_str = new char[64];
    // obj
    m_is_obj = isobj( m_type_ref );
    const char * str = m_type_ref->c_name();
    t_CKUINT len = strlen( str );
    // copy
    if( len < 48 )
        strcpy( m_param_str, str );
    else
    {
        strncpy( m_param_str, str, 48 );
        strcpy( m_param_str + 48, "..." );
    }

    // append length
    char buffer[16];
    sprintf( buffer, "[%ld]", m_length );
    strcat( m_param_str, buffer );
}




//-----------------------------------------------------------------------------
// name: ~Chuck_Instr_Array_Init()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Instr_Array_Init::~Chuck_Instr_Array_Init()
{
    // delete
    delete [] m_param_str;
    m_param_str = NULL;
    // release
    //m_type_ref->release();
    m_type_ref = NULL;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Array_Init::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // reg stack pointer
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

    // allocate the array
    if( m_type_ref->size == 4 ) // ISSUE: 64-bit
    {
        // pop the values
        pop_( reg_sp, m_length );
        // instantiate array
        Chuck_Array4 * array = new Chuck_Array4( m_is_obj, m_length );
        // problem
        if( !array ) goto out_of_memory;
        // initialize object
        initialize_object( array, &t_array );
        // fill array
        for( t_CKINT i = 0; i < m_length; i++ )
            array->set( i, *(reg_sp + i) );
        // set size
        array->m_size = m_length;
        // push the pointer
        push_( reg_sp, (t_CKUINT)array );
    }
    else if( m_type_ref->size == 8 ) // ISSUE: 64-bit
    {
        // pop the values
        pop_( reg_sp, 2 * m_length );
        // instantiate array
        Chuck_Array8 * array = new Chuck_Array8( m_length );
        // problem
        if( !array ) goto out_of_memory;
        // fill array
        t_CKFLOAT * sp = (t_CKFLOAT *)reg_sp;
        // intialize object
        initialize_object( array, &t_array );
        // fill array
        for( t_CKINT i = 0; i < m_length; i++ )
            array->set( i, *(sp + i) );
        // set size
        array->m_size = m_length;
        // push the pointer
        push_( reg_sp, (t_CKUINT)array );
    }
    else assert( FALSE );

    return;

out_of_memory:

    // we have a problem
    fprintf( stderr, 
        "[chuck](VM): OutOfMemory: while initializing arrays\n" );

    // do something!
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}




//-----------------------------------------------------------------------------
// name: Chuck_Instr_Array_Alloc()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Instr_Array_Alloc::Chuck_Instr_Array_Alloc( t_CKUINT depth, Chuck_Type * t,
                                                  t_CKUINT offset, t_CKBOOL is_ref )
{
    // set
    m_depth = depth;
    // copy
    m_type_ref = t;
    // remember
    // m_type_ref->add_ref();
    // type
    m_param_str = new char[64];
    // obj
    m_is_obj = isobj( m_type_ref );
    // offset for pre constructor
    m_stack_offset = offset;
    // is object ref
    m_is_ref = is_ref;
    const char * str = m_type_ref->c_name();
    t_CKUINT len = strlen( str );
    // copy
    if( len < 64 )
        strcpy( m_param_str, str );
    else
    {
        strncpy( m_param_str, str, 60 );
        strcpy( m_param_str + 60, "..." );
    }
}




//-----------------------------------------------------------------------------
// name: ~Chuck_Instr_Array_Alloc()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Instr_Array_Alloc::~Chuck_Instr_Array_Alloc()
{
    // delete
    delete [] m_param_str;
    m_param_str = NULL;
    // release
    //m_type_ref->release();
    m_type_ref = NULL;
}




//-----------------------------------------------------------------------------
// name: do_alloc_array()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Object * do_alloc_array( t_CKINT * capacity, const t_CKINT * top,
                               t_CKUINT size, t_CKBOOL is_obj,
                               t_CKUINT * objs, t_CKINT & index )
{
    // not top level
    Chuck_Array4 * base;
    Chuck_Object * next;
    t_CKINT i;

    // see if top level
    if( capacity >= top )
    {
        // check size
        if( size == 4 ) // ISSUE: 64-bit
        {
            Chuck_Array4 * base = new Chuck_Array4( is_obj, *capacity );
            if( !base ) goto out_of_memory;

            // if object
            if( is_obj && objs )
            {
                // loop
                for( i = 0; i < *capacity; i++ )
                {
                    // add to array for later allocation
                    objs[index++] = base->addr( i );
                }
            }

            // initialize object
            initialize_object( base, &t_array );
            return base;
        }
        else
        {
            Chuck_Array8 * base = new Chuck_Array8( *capacity );
            if( !base ) goto out_of_memory;

            // initialize object
            initialize_object( base, &t_array );
            return base;
        }

        // shouldn't get here
        assert( FALSE );
    }

    // not top level
    base = new Chuck_Array4( TRUE, *capacity );
    if( !base ) goto out_of_memory;

    // allocate the next level
    for( i = 0; i < base->capacity(); i++ )
    {
        // the next
        next = do_alloc_array( capacity+1, top, size, is_obj, objs, index );
        // error if NULL
        if( !next ) goto error;
        // set that, with ref count
        base->set( i, (t_CKUINT)next );
    }

    // initialize object
    initialize_object( base, &t_array );
    return base;

out_of_memory:
    // we have a problem
    fprintf( stderr, 
        "[chuck](VM): OutOfMemory: while allocating arrays\n" );
    return NULL;

error:
    // base shouldn't have been ref counted
    SAFE_DELETE( base );
    return NULL;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Array_Alloc::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // reg stack pointer
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    // ref
    t_CKUINT ref = 0;
    // the total number of objects to allocate
    t_CKUINT num_obj = 0;
    // the index to pass to the arrays
    t_CKINT index = 0;
    // number
    t_CKFLOAT num = 1.0;

    // if need instantiation
    if( m_is_obj && !m_is_ref )
    {
        t_CKINT * curr = (t_CKINT *)(reg_sp - m_depth);
        t_CKINT * top = (t_CKINT *)(reg_sp - 1);

        num_obj = 1;
        num = 1.0;
        // product of all dims
        while( curr <= top )
        {
            num_obj *= *(curr);

            // overflow
            num *= (t_CKFLOAT)(*(curr));
            if( num > (t_CKFLOAT)INT_MAX ) goto overflow;

            curr++;
        }

        // check to see if we need to allocate
        if( num > shred->obj_array_size )
        {
            SAFE_DELETE( shred->obj_array );
            shred->obj_array_size = 0;
            shred->obj_array = new t_CKUINT[num_obj];
            if( !shred->obj_array ) goto out_of_memory;
            shred->obj_array_size = num_obj;
        }
    }

    // recursively allocate
    ref = (t_CKUINT)do_alloc_array( 
        (t_CKINT *)(reg_sp - m_depth),
        (t_CKINT *)(reg_sp - 1),
        m_type_ref->size,
        m_is_obj,
        shred->obj_array, index 
    );

    // pop the indices - this protects the contents of the stack
    // do_alloc_array writes stuff to the stack
    pop_( reg_sp, m_depth );

    // make sure
    assert( index == (t_CKINT)num_obj );

    // problem
    if( !ref ) goto error;

    // push array
    push_( reg_sp, ref );

    // if need to instantiate
    if( m_is_obj && !m_is_ref )
    {
        // push objects to instantiate
        push_( reg_sp, (t_CKUINT)shred->obj_array );
        // push index to use
        push_( reg_sp, 0 );
        // push size
        push_( reg_sp, (t_CKUINT)num_obj );
    }

    return;

overflow:
    // we have a problem
    fprintf( stderr,
        "[chuck](VM): OverFlow: requested array size too big\n" );

out_of_memory:
    // we have a problem
    fprintf( stderr, 
        "[chuck](VM): OutOfMemory: while allocating arrays\n" );

error:
    // done
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}


//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Array_Access::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // reg stack pointer
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    // UNUSED: t_CKUINT *& reg_sp = sp;
    t_CKINT i = 0;
    t_CKUINT val = 0;
    t_CKFLOAT fval = 0;

    // pop
    pop_( sp, 2 );

    // 4 or 8
    if( m_size == 4 ) // ISSUE: 64-bit
    {
        // get array
        Chuck_Array4 * arr = (Chuck_Array4 *)(*sp);
        // get index
        i = (t_CKINT)(*(sp+1));
        // check if writing
        if( m_emit_addr ) {
            // get the addr
            val = arr->addr( i );
            // exception
            if( !val ) goto error;
            // push the addr
            push_( sp, val );
        } else {
            // get the value
            if( !arr->get( i, &val ) )
                goto error;
            // push the value
            push_( sp, val );
        }
    }
    else if( m_size == 8 ) // ISSUE: 64-bit
    {
        // get array
        Chuck_Array8 * arr = (Chuck_Array8 *)(*sp);
        // get index
        i = (t_CKINT)(*(sp+1));
        // check if writing
        if( m_emit_addr ) {
            // get the addr
            val = arr->addr( i );
            // exception
            if( !val ) goto error;
            // push the addr
            push_( sp, val );
        } else {
            // get the value
            if( !arr->get( i, &fval ) )
                goto error;
            // push the value
            push_( ((t_CKFLOAT *&)sp), fval );
        }
    }
    else
        assert( FALSE );

    return;

error:
    // we have a problem
    fprintf( stderr, 
             "[chuck](VM): ArrayOutofBounds in shred[id=%d:%s], PC=[%d], index=[%d]\n", 
             shred->xid, shred->name.c_str(), shred->pc, i );

    // do something!
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Array_Map_Access::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // reg stack pointer
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_String * key = NULL;
    t_CKUINT val = 0;
    t_CKFLOAT fval = 0;

    // pop
    pop_( sp, 2 );

    // 4 or 8
    if( m_size == 4 ) // ISSUE: 64-bit
    {
        // get array
        Chuck_Array4 * arr = (Chuck_Array4 *)(*sp);
        // get index
        key = (Chuck_String *)(*(sp+1));
        // check if writing
        if( m_emit_addr ) {
            // get the addr
            val = arr->addr( key->str );
            // exception
            if( !val ) goto error;
            // push the addr
            push_( sp, val );
        } else {
            // get the value
            if( !arr->get( key->str, &val ) )
                goto error;
            // push the value
            push_( sp, val );
        }
    }
    else if( m_size == 8 ) // ISSUE: 64-bit
    {
        // get array
        Chuck_Array8 * arr = (Chuck_Array8 *)(*sp);
        // get index
        key = (Chuck_String *)(*(sp+1));
        // check if writing
        if( m_emit_addr ) {
            // get the addr
            val = arr->addr( key->str );
            // exception
            if( !val ) goto error;
            // push the addr
            push_( sp, val );
        } else {
            // get the value
            if( !arr->get( key->str, &fval ) )
                goto error;
            // push the value
            push_( ((t_CKFLOAT *&)sp), fval );
        }
    }
    else
        assert( FALSE );

    return;

error:
    // we have a problem
    fprintf( stderr, 
             "[chuck](VM): InternalArrayMap error in shred[id=%d:%s], PC=[%d], index=[%s]\n", 
             shred->xid, shred->name.c_str(), shred->pc, key->str.c_str() );

    // do something!
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Array_Access_Multi::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // reg stack pointer
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    t_CKINT i = 0;
    t_CKUINT val = 0, j;
    t_CKFLOAT fval = 0;
    t_CKINT * ptr = NULL;

    // pop all indices then array
    pop_( sp, m_depth + 1 );

    // get array
    Chuck_Array4 * base = (Chuck_Array4 *)(*sp);
    ptr = (t_CKINT *)(sp+1);

    // make sure
    assert( m_depth > 1 );
    // loop through indices
    for( j = 0; j < (m_depth-1); j++ )
    {
        // get index
        i = *ptr++;
        // get the array
        if( !base->get( i, &val ) )
            goto error;
        // set the array
        base = (Chuck_Array4 *)val;
    }

    // 4 or 8
    if( m_size == 4 ) // ISSUE: 64-bit
    {
        // get arry
        Chuck_Array4 * arr = base;
        // get index
        i = (t_CKINT)(*ptr);
        // check if writing
        if( m_emit_addr ) {
            // get the addr
            val = arr->addr( i );
            // exception
            if( !val ) goto error;
            // push the addr
            push_( sp, val );
        } else {
            // get the value
            if( !arr->get( i, &val ) )
                goto error;
            // push the value
            push_( sp, val );
        }
    }
    else if( m_size == 8 ) // ISSUE: 64-bit
    {
        // get array
        Chuck_Array8 * arr = (Chuck_Array8 *)(base);
        // get index
        i = (t_CKINT)(*ptr);
        // check if writing
        if( m_emit_addr ) {
            // get the addr
            val = arr->addr( i );
            // exception
            if( !val ) goto error;
            // push the addr
            push_( sp, val );
        } else {
            // get the value
            if( !arr->get( i, &fval ) )
                goto error;
            // push the value
            push_( ((t_CKFLOAT *&)sp), fval );
        }
    }
    else
        assert( FALSE );

    return;

error:
    // we have a problem
    fprintf( stderr, 
             "[chuck](VM): ArrayOutofBounds in shred[id=%d:%s], PC=[%d], index=[%d]\n", 
             shred->xid, shred->name.c_str(), shred->pc, i );

    // do something!
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Dot_Member_Data::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // register stack pointer
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    // the pointer
    t_CKUINT data;
    
    // pop the object pointer
    pop_( sp, 1 );
    // get the object pointer
    Chuck_Object * obj = (Chuck_Object *)(*sp);
    // check
    if( !obj ) goto error;
    // calculate the data pointer
    data = (t_CKUINT)(obj->data + m_offset);
    
    // emit addr or value
    if( m_emit_addr )
    {
        // push the address
        push_( sp, data );
    }
    else
    {
        // 4 or 8
        if( m_size == 4 ) { push_( sp, *((t_CKUINT *)data) ); } // ISSUE: 64-bit
        else if( m_size == 8 ) { push_float( sp, *((t_CKFLOAT *)data) ); } // ISSUE: 64-bit
        else assert( FALSE );
    }

    return;

error:
    // we have a problem
    fprintf( stderr, 
             "[chuck](VM): NullPointerException: shred[id=%d:%s:(%d)], PC=[%d]\n", 
             shred->xid, shred->name.c_str(), shred->pc );

    // do something!
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Dot_Member_Func::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // register stack pointer
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    // the pointer
    t_CKUINT data;
    
    // pop the object pointer
    pop_( sp, 1 );
    // get the object pointer
    Chuck_Object * obj = (Chuck_Object *)(*sp);
    // check
    if( !obj ) goto error;
    // make sure we are in range
    assert( m_offset < obj->vtable->funcs.size() );
    // calculate the data pointer
    data = (t_CKUINT)(obj->vtable->funcs[m_offset]);
    
    // push the address
    push_( sp, data );

    return;

error:
    // we have a problem
    fprintf( stderr, 
             "[chuck](VM): NullPointerException: shred[id=%d:%s], PC=[%d]\n", 
             shred->xid, shred->name.c_str(), shred->pc );

    // do something!
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Dot_Static_Data::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // register stack pointer
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    // the pointer
    t_CKUINT data;
    
    // pop the type pointer
    pop_( sp, 1 );
    // get the object pointer
    Chuck_Type * t_class = (Chuck_Type *)(*sp);
    // make sure
    assert( (m_offset + m_size) <= t_class->info->class_data_size );
    // calculate the data pointer
    data = (t_CKUINT)(t_class->info->class_data + m_offset);
    
    // emit addr or value
    if( m_emit_addr )
    {
        // push the address
        push_( sp, data );
    }
    else
    {
        // 4 or 8
        if( m_size == 4 ) { push_( sp, *((t_CKUINT *)data) ); } // ISSUE: 64-bit
        else if( m_size == 8 ) { push_float( sp, *((t_CKFLOAT *)data) ); } // ISSUE: 64-bit
        else assert( FALSE );
    }
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Dot_Static_Import_Data::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // register stack pointer
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    
    // emit addr or value
    if( m_emit_addr )
    {
        // push the address
        push_( sp, (t_CKUINT)m_addr );
    }
    else
    {
        // 4 or 8
        if( m_size == 4 ) { push_( sp, *((t_CKUINT *)m_addr) ); } // ISSUE: 64-bit
        else if( m_size == 8 ) { push_float( sp, *((t_CKFLOAT *)m_addr) ); } // ISSUE: 64-bit
        else assert( FALSE );
    }
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Dot_Static_Func::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // register stack pointer
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    
    // pop the type pointer
    pop_( sp, 1 );
    
    // push the address
    push_( sp, (t_CKUINT)(m_func) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Cast_double2int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    t_CKINT *& sp_int = (t_CKINT *&)sp;
    pop_( sp, 1 );
    push_( sp_int, (t_CKINT)(*sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Cast_int2double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;
    t_CKFLOAT *& sp_double = (t_CKFLOAT *&)sp;
    pop_( sp, 1 );
    push_( sp_double, (t_CKFLOAT)(*sp) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Op_string::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_String * lhs = NULL;
    Chuck_String * rhs = NULL;

    // pop
    pop_( sp, 2 );
    // get the string references
    lhs = (Chuck_String *)*sp;
    rhs = (Chuck_String *)*(sp + 1);
    // neither should be null
    if( !lhs || !rhs ) goto null_pointer;

    // look
    switch( m_val )
    {
    case ae_op_eq:
        push_( sp, lhs->str == rhs->str );
    break;

    case ae_op_neq:
        push_( sp, lhs->str != rhs->str );
    break;

    case ae_op_lt:
        push_( sp, lhs->str < rhs->str );
    break;

    case ae_op_le:
        push_( sp, lhs->str <= rhs->str );
    break;

    case ae_op_gt:
        push_( sp, lhs->str > rhs->str );
    break;

    case ae_op_ge:
        push_( sp, lhs->str >= rhs->str );
    break;

    default:
        goto invalid_op;
    break;
    }

    return;

null_pointer:
    // we have a problem
    fprintf( stderr, 
        "[chuck](VM): NullPointerException: (during string op) in shred[id=%d:%s]\n",
        shred->xid, shred->name.c_str() );
    goto done;

invalid_op:
    // we have a problem
    fprintf( stderr,
        "[chuck](VM): InvalidStringOpException: '%d' in shred[id=%d:%s]\n",
        m_val, shred->xid, shred->name.c_str() );
    goto done;

done:
    // do something!
    shred->is_running = FALSE;
    shred->is_done = TRUE;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_ADC::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    push_( reg_sp, (t_CKUINT)vm->m_adc );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_DAC::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    push_( reg_sp, (t_CKUINT)vm->m_dac );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Bunghole::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    push_( reg_sp, (t_CKUINT)vm->m_bunghole );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_Link::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    Chuck_UGen **& sp = (Chuck_UGen **&)shred->reg->sp;

    pop_( sp, 2 );
    (*(sp + 1))->add( *sp );
    push_( sp, *(sp + 1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_UnLink::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    Chuck_UGen **& sp = (Chuck_UGen **&)shred->reg->sp;
    
    pop_( sp, 2 );
    (*(sp+1))->remove( *sp );
    push_( sp, *(sp + 1) );
}



/*
//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_Ctrl::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    
    pop_( sp, 4 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp+1);
    f_ctrl ctrl = (f_ctrl)*(sp+2);
    f_cget cget = (f_cget)*(sp+3);
    // set now
    ugen->now = shred->now;
    // call ctrl
    ctrl( ugen, (void *)sp );
    if( cget ) cget( ugen, (void *)sp );
    // push the new value
    push_( sp, *sp);
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_CGet::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;

    pop_( sp, 2 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp);
    f_cget cget = (f_cget)*(sp+1);
    // set now
    ugen->now = shred->now;
    // call cget
    cget( ugen, (void *)sp );
    // push the new value
    push_( sp, *sp);
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_Ctrl2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    
    pop_( sp, 4 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp+1);
    f_ctrl ctrl = (f_ctrl)*(sp+2);
    f_cget cget = (f_cget)*(sp+3);
    // set now
    ugen->now = shred->now;
    // call ctrl
    pop_( sp, 1 );
    ctrl( ugen, (void *)sp );
    if( cget ) cget( ugen, (void *)sp );
    // push the new value
    ((t_CKFLOAT *&)shred->reg->sp)++;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_CGet2::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;

    pop_( sp, 2 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp);
    f_cget cget = (f_cget)*(sp+1);
    // set now
    ugen->now = shred->now;
    // call cget
    cget( ugen, (void *)sp );
    // push the new value
    ((t_CKFLOAT *&)shred->reg->sp)++;
}
*/



//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_PMsg::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    Chuck_UGen **& sp = (Chuck_UGen **&)shred->reg->sp;
    
    pop_( sp, 2 );
    
    // (*(sp + 1))->pmsg( shred->now, *sp );
    
    push_( sp, *(sp + 1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Init_Loop_Counter::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKINT *& sp = (t_CKINT *&)shred->reg->sp;

    // pop the value
    pop_( sp, 1 );

    // copy it
    (*(t_CKINT *)m_val) = *sp >= 0 ? *sp : -*sp;
}




// hack
Chuck_Instr_Hack::Chuck_Instr_Hack( Chuck_Type * type )
{
    this->m_type_ref = type;
    // this->m_type_ref->add_ref();
}

Chuck_Instr_Hack::~Chuck_Instr_Hack()
{
//    this->m_type_ref->release();
}

void Chuck_Instr_Hack::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // look at the type
    if( m_type_ref->size == 4 ) // ISSUE: 64-bit
    {
        t_CKINT * sp = (t_CKINT *)shred->reg->sp;
        if( !isa( m_type_ref, &t_string ) )
            // print it
            fprintf( stderr, "%d :(%s)\n", *(sp-1), m_type_ref->c_name() );
        else
            fprintf( stderr, "\"%s\" : (%s)\n", ((Chuck_String *)*(sp-1))->str.c_str(), m_type_ref->c_name() );
    }
    else if( m_type_ref->size == 8 ) // ISSUE: 64-bit
    {
        t_CKFLOAT * sp = (t_CKFLOAT *)shred->reg->sp;
        // print it
        fprintf( stderr, "%f :(%s)\n", *(sp-1), m_type_ref->c_name() );
    }
    else if( m_type_ref->size == 0 )
    {
        fprintf( stderr, "... :(%s)\n", m_type_ref->c_name() );
    }
    else
        assert( FALSE );

    // flush
    fflush( stderr );
}

const char * Chuck_Instr_Hack::params() const
{
    static char buffer[256];
    sprintf( buffer, "(%s)", m_type_ref->c_name() );
    return buffer;
}




// gack
Chuck_Instr_Gack::Chuck_Instr_Gack( const std::vector<Chuck_Type *> & types )
{
    m_type_refs = types;

    // add refs
}

Chuck_Instr_Gack::~Chuck_Instr_Gack()
{
    // release refs
}

void Chuck_Instr_Gack::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE * the_sp = (t_CKBYTE *)shred->reg->sp;

    if( m_type_refs.size() == 1 )
    {
        Chuck_Instr_Hack hack( m_type_refs[0] );
        hack.execute( vm, shred );
        return;
    }

    // loop over types
    t_CKUINT i;

    // find the start of the expression
    for( i = 0; i < m_type_refs.size(); i++ )
    {
        Chuck_Type * type = m_type_refs[i];
        the_sp -= type->size;
    }

    // print
    for( i = 0; i < m_type_refs.size(); i++ )
    {
        Chuck_Type * type = m_type_refs[i];

        // look at the type
        if( type->size == 4 ) // ISSUE: 64-bit
        {
            t_CKINT * sp = (t_CKINT *)the_sp;
            if( !isa( type, &t_string ) )
            {
                if( isa( type, &t_object ) )
                    // print it
                    fprintf( stderr, "0x%x ", *(sp) );
                else
                    // print it
                    fprintf( stderr, "%d ", *(sp) );
            }
            else
                fprintf( stderr, "%s ", ((Chuck_String *)*(sp))->str.c_str() );

            the_sp += 4;
        }
        else if( type->size == 8 ) // ISSUE: 64-bit
        {
            t_CKFLOAT * sp = (t_CKFLOAT *)the_sp;
            // print it
            fprintf( stderr, "%f ", *(sp) );

            the_sp += 8;
        }
        else if( type->size == 0 )
        {
            fprintf( stderr, "... " );
        }
        else
            assert( FALSE );
    }

    fprintf( stderr, "\n" );
    
    // flush
    fflush( stderr );
}

const char * Chuck_Instr_Gack::params() const
{
    static char buffer[256];
    sprintf( buffer, "( many types )" );
    return buffer;
}
