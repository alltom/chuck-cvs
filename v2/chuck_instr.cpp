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
#include <typeinfo>
using namespace std;

#include "chuck_instr.h"
#include "chuck_vm.h"
#include "chuck_ugen.h"
#include "chuck_bbq.h"
#include "chuck_dl.h"
#include "chuck_type.h"




// define SP offset
#define push_( sp, val )         *(sp) = (val); (sp)++
#define push_float( sp, val )    *((t_CKFLOAT *&)sp) = (val); ((t_CKFLOAT *&)sp)++
#define pop_( sp, n )            sp -= (n)
#define val_( sp )               *(sp)




//-----------------------------------------------------------------------------
// name: name()
// desc: ...
//-----------------------------------------------------------------------------
const char * Chuck_Instr::name() const
{
     return typeid(*this).name();
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
void Chuck_Instr_Inc_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKINT *& reg_sp = (t_CKINT *&)shred->reg->sp;

    // pop word from reg stack
    pop_( reg_sp, 1 );
    // increment value
    (*(reg_sp-1))++;
    // copy value into mem stack
    *( (t_CKINT *)(mem_sp + *(reg_sp)) ) = *(reg_sp-1);
}





//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Dec_int
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Dec_int::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKINT *& reg_sp = (t_CKINT *&)shred->reg->sp;

    // pop word from reg stack
    pop_( reg_sp, 1 );
    // decrement value
    (*(reg_sp-1))--;
    // copy value into mem stack
    *( (t_CKINT *)(mem_sp + *(reg_sp)) ) = *(reg_sp-1);
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
    if( m_size == 4 )
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
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
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
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
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
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKFLOAT *& reg_sp = (t_CKFLOAT *&)shred->reg->sp;

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
    pop_( sp, 2 );
    push_( sp, val_(sp) < val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Gt_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) > val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Le_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) <= val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Ge_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) >= val_(sp+1) );
}



//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Eq_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) == val_(sp+1) );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Neq_double::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKFLOAT *& sp = (t_CKFLOAT *&)shred->reg->sp;
    pop_( sp, 2 );
    push_( sp, val_(sp) != val_(sp+1) );
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
void Chuck_Instr_Alloc_DWord::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
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
// desc: instantiate object
//-----------------------------------------------------------------------------
void Chuck_Instr_Instantiate_Object::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKBYTE *& mem_sp = (t_CKBYTE *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_Object * object = NULL;

    // sanity
    assert( this->type != NULL );

    // allocate the VM object
    object = new Chuck_Object;
    // init
    object->init();
    // allocate virtual table
    object->vtable = new Chuck_VTable;
    // set the type reference
    // TODO: reference count
    object->type_ref = this->type;
    // get the size
    object->size = type->obj_size;
    // allocate memory
    // TODO: check to ensure enough memory
    object->data = new t_CKBYTE[object->size];
    // zero it out
    memset( object->data, 0, object->size );

    // push the pointer on the operand stack
    push_( reg_sp, (t_CKUINT)object );
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
    Chuck_VM_Object ** obj = NULL;

    // pop word from reg stack
    pop_( reg_sp, 2 );
    // the previous reference
    obj = (Chuck_VM_Object **)(*(reg_sp+1));
    // release any previous reference
    if( *obj ) (*obj)->release();
    // copy popped value into memory
    *obj = *(Chuck_VM_Object **)reg_sp;
    // add reference
    if( *obj ) (*obj)->add_ref();

    // push the reference value to reg stack
    push_( reg_sp, *reg_sp );
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
        // pop the arguments, by number of bytes
        pop_( reg_sp, func->stack_depth );

        // make copies
        t_CKUINT * mem_sp2 = (t_CKUINT *)mem_sp;
        t_CKUINT * reg_sp2 = (t_CKUINT *)reg_sp;

        // push the arguments
        for( t_CKUINT i = 0; i < stack_depth; i++ )
            *mem_sp2++ = *reg_sp2++;
        // memcpy( mem_sp2, reg_sp2, stack_depth << 2 );
    }
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: builtin function call with void return type
//-----------------------------------------------------------------------------
void Chuck_Instr_Func_Call0::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_DL_Return retval;

    // pop the register stack
    pop_( reg_sp, 3 );
    // get the function to be called
    f_ck_func f = (f_ck_func)(*(reg_sp+1));
    // stack size - calle function arguments
    t_CKUINT stack_size = ((*reg_sp) >> 2) + ( *reg_sp & 0x3 ? 1 : 0 );
    // local stack size - variables and caller function arguments
    t_CKUINT local_depth = *(reg_sp+2) + *(mem_sp-1);
    // the amount to push in 4-byte words
    t_CKUINT push = (local_depth >> 2) + ( local_depth & 0x3 ? 1 : 0 );
    // pop the arguments for pass to callee function
    reg_sp -= stack_size;
    // push the mem stack passed the current function variables and arguments
    mem_sp += push;
    // make copies
    t_CKUINT * sp = reg_sp;
    t_CKUINT * mem = mem_sp;
    // copy to args
    for( t_CKUINT i = 0; i < stack_size; i++ )
        *mem++ = *sp++;
    // call the function
    f( mem_sp, &retval );
    mem_sp -= push;
    // push the return args
    // push_( reg_sp, retval.v_uint );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: builtin function call with 4-byte return type
//-----------------------------------------------------------------------------
void Chuck_Instr_Func_Call4::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_DL_Return retval;

    pop_( reg_sp, 3 );
    f_ck_func f = (f_ck_func)(*(reg_sp+1));
    t_CKUINT stack_size = ((*reg_sp) >> 2) + ( *reg_sp & 0x3 ? 1 : 0 );
    t_CKUINT local_depth = *(reg_sp+2) + *(mem_sp-1);
    t_CKUINT push = ((local_depth) >> 2) + ( local_depth & 0x3 ? 1 : 0 );
    reg_sp -= stack_size;
    mem_sp += push;
    t_CKUINT * sp = reg_sp;
    t_CKUINT * mem = mem_sp;
    // copy to args
    for( t_CKUINT i = 0; i < stack_size; i++ )
        *mem++ = *sp++;
    // call the function
    f( mem_sp, &retval );
    mem_sp -= push;
    // push the return args
    push_( reg_sp, retval.v_uint );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: builtin function call with 8-byte return type
//-----------------------------------------------------------------------------
void Chuck_Instr_Func_Call8::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_DL_Return retval;

    pop_( reg_sp, 3 );
    f_ck_func f = (f_ck_func)(*(reg_sp+1));
    t_CKUINT stack_size = ((*reg_sp) >> 2) + ( *reg_sp & 0x3 ? 1 : 0 );
    t_CKUINT local_depth = *(reg_sp+2) + *(mem_sp-1);
    t_CKUINT push = (local_depth >> 2) + ( local_depth & 0x3 ? 1 : 0 );
    reg_sp -= stack_size;
    mem_sp += push;
    t_CKUINT * sp = reg_sp;
    t_CKUINT * mem = mem_sp;
    // copy to args
    for( t_CKUINT i = 0; i < stack_size; i++ )
        *mem++ = *sp++;
    // call the function
    f( mem_sp, &retval );
    mem_sp -= push;
    // push the return args
    t_CKFLOAT *& sp_double = (t_CKFLOAT *&)reg_sp;
    push_( sp_double, retval.v_float );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Func_Return::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& mem_sp = (t_CKUINT *&)shred->mem->sp;
    t_CKUINT *& reg_sp = (t_CKUINT *&)shred->reg->sp;

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

    // pop the stack
    pop_( reg_sp, 1 );
    // get the code
    Chuck_VM_Code * code = *(Chuck_VM_Code **)reg_sp;
    // spork it
    Chuck_VM_Shred * sh = vm->spork( code, shred );
    // copy args
    if( m_val )
    {
        pop_( shred->reg->sp, m_val );
        memcpy( sh->reg->sp, shred->reg->sp, m_val );
        sh->reg->sp += m_val;
    }
    // push the stack
    push_( reg_sp, sh->id );
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
            "[chuck](VM): Exception DestTimeNegative: '%.6f'\n", *sp );
        // do something!
        shred->is_running = FALSE;

        return;
    }

    // shredule the shred
    vm->shreduler()->shredule( shred, *sp );
    // suspend
    shred->is_running = FALSE;

    push_( sp, *sp );
}



//-----------------------------------------------------------------------------
// name: Chuck_Instr_Array_Alloc()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Instr_Array_Alloc::Chuck_Instr_Array_Alloc( t_CKUINT depth, Chuck_Type * t )
{
    // set
    m_depth = depth;
    // copy
    m_type_ref = t;
    // remember
    m_type_ref->add_ref();
    // type
    m_param_str = new char[64];
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
    m_type_ref->release();
    m_type_ref = NULL;
}




//-----------------------------------------------------------------------------
// name: do_alloc_array()
// desc: ...
//-----------------------------------------------------------------------------
Chuck_Object * do_alloc_array( t_CKINT * capacity, const t_CKINT * top, t_CKUINT size )
{
    // not top level
    Chuck_Array4 * base;
    Chuck_Object * next;
    t_CKINT i;

    // see if top level
    if( capacity >= top )
    {
        // check size
        if( size == 4 )
        {
            Chuck_Array4 * base = new Chuck_Array4( *capacity );
            if( !base ) goto out_of_memory;
            return base;
        }
        else
        {
            Chuck_Array8 * base = new Chuck_Array8( *capacity );
            if( !base ) goto out_of_memory;
            return base;
        }

        // shouldn't get here
        assert( FALSE );
    }

    // not top level
    base = new Chuck_Array4( *capacity );
    if( !base ) goto out_of_memory;

    // allocate the next level
    for( i = 0; i < base->size(); i++ )
    {
        // the next
        next = do_alloc_array( capacity+1, top, size );
        // error if NULL
        if( !next ) goto error;
        // set that
        base->set( i, (t_CKUINT)next );
        // ref count it
        next->add_ref();
    }

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

    // pop the indices
    pop_( reg_sp, m_depth );
    // recursively allocate
    ref = (t_CKUINT)do_alloc_array( 
        (t_CKINT *)reg_sp,
        (t_CKINT *)(reg_sp + m_depth - 1),
        m_type_ref->size
    );

    // problem
    if( !ref )
    {
        // done
        shred->is_running = FALSE;
        return;
    }

    // push
    push_( reg_sp, ref );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_Array_Access::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    // reg stack pointer
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    t_CKINT i = 0;
    t_CKUINT val = 0;
    t_CKFLOAT fval = 0;

    // pop
    pop_( sp, 2 );

    // 4 or 8
    if( m_size == 4 )
    {
        // get array
        Chuck_Array4 * arr = (Chuck_Array4 *)(*sp);
        // get index
        i = (t_CKINT)(*sp+1);
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
    else if( m_size == 8 )
    {
        // get array
        Chuck_Array8 * arr = (Chuck_Array8 *)(*sp);
        // get index
        i = (t_CKINT)(*sp+1);
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
             "[chuck](VM): ArrayOutofBounds in shred '%s': index='%d'\n", 
             shred->name.c_str(), i );
    // do something!
    shred->is_running = FALSE;
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
    if( m_size == 4 )
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
    else if( m_size == 8 )
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
             "[chuck](VM): ArrayOutofBounds in shred '%s': index='%d'\n", 
             shred->name.c_str(), i );
    // do something!
    shred->is_running = FALSE;
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
        if( m_size == 4 ) { push_( sp, *((t_CKUINT *)data) ); }
        else if( m_size == 8 ) { push_float( sp, *((t_CKFLOAT *)data) ); }
        else assert( FALSE );
    }
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
    // calculate the data pointer
    data = (t_CKUINT)(obj->vtable->funcs[m_offset]);
    
    // push the address
    push_( sp, data );
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
        if( m_size == 4 ) { push_( sp, *((t_CKUINT *)data) ); }
        else if( m_size == 8 ) { push_float( sp, *((t_CKFLOAT *)data) ); }
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
    push_( sp, (t_CKUINT)m_func );
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




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_Alloc::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    Chuck_UGen_Info * info = NULL;
    Chuck_UGen * ugen = NULL;

    pop_( sp, 1 );
    info = (Chuck_UGen_Info *)*(sp);
    ugen = new Chuck_UGen;
    // copy the info over
    ugen->ctor = info->ctor;
    ugen->dtor = info->dtor;
    ugen->tick = info->tick;
    ugen->pmsg = info->pmsg;
    ugen->m_max_src = info->max_src;
    // call the constructor
    ugen->state = ugen->ctor ? ugen->ctor( shred->now ) : NULL ;

    // setup the reference with the shred
    ugen->shred = shred;
    shred->add( ugen );
    push_( sp, (t_CKUINT)ugen );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_DeAlloc::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
}




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
    // call ctrl
    ctrl( shred->now, ugen->state, (void *)sp );
    if( cget ) cget( shred->now, ugen->state, (void *)sp );
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
    // call cget
    cget( shred->now, ugen->state, (void *)sp );
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
    // call ctrl
    pop_( sp, 1 );
    ctrl( shred->now, ugen->state, (void *)sp );
    if( cget ) cget( shred->now, ugen->state, (void *)sp );
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
    // call cget
    cget( shred->now, ugen->state, (void *)sp );
    // push the new value
    ((t_CKFLOAT *&)shred->reg->sp)++;
}




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
void Chuck_Instr_UGen_Ctrl_Op::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    
    pop_( sp, 4 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp+1);
    ugen->m_op = *(t_CKINT *)sp;
    // push the new value
    push_( sp, *sp);
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_CGet_Op::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;

    pop_( sp, 2 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp);
    // push the new value
    push_( sp, ugen->m_op );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_Ctrl_Gain::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    
    // HACK: this won't work for 64-bit long
    ((Chuck_UGen *)*(sp-3))->m_gain = (float)*(t_CKFLOAT *)(sp-5);
    pop_( sp, 5 );

    // push the new value
    ((t_CKFLOAT *&)shred->reg->sp)++;
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_CGet_Gain::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;

    pop_( sp, 2 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp);
    // push the new value
    t_CKFLOAT *& sp_double = (t_CKFLOAT *&)shred->reg->sp;
    push_( sp_double, (t_CKFLOAT)ugen->m_gain );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_UGen_CGet_Last::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;

    pop_( sp, 2 );
    Chuck_UGen * ugen = (Chuck_UGen *)*(sp);
    // push the new value
    t_CKFLOAT *& sp_double = (t_CKFLOAT *&)shred->reg->sp;
    push_( sp_double, (t_CKFLOAT)ugen->m_current );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_DLL_Load::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    t_CKINT retval = FALSE;
    Chuck_DLL * dll = NULL;
    pop_( sp, 2 );
    
    // load the DLL into the vm
    dll = vm->dll_load( (const char *)(*sp) );
    // load the DLL into the namespace
    if( dll ) retval = type_engine_add_dll( (Chuck_Env *)vm->get_env(), dll,
                                            (const char *)(*(sp+1)) );

    // push the result
    push_( sp, (t_CKUINT)dll );
}




//-----------------------------------------------------------------------------
// name: execute()
// desc: ...
//-----------------------------------------------------------------------------
void Chuck_Instr_DLL_Unload::execute( Chuck_VM * vm, Chuck_VM_Shred * shred )
{
    t_CKUINT *& sp = (t_CKUINT *&)shred->reg->sp;
    t_CKINT retval = FALSE;
    Chuck_DLL * dll = NULL;
    pop_( sp, 1 );

    // unload the dll
    dll = (Chuck_DLL *)(*sp);
    if( dll ) retval = vm->dll_unload( dll );

    // push the result
    push_( sp, retval );
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
