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
// file: chuck_instr.h
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#ifndef __CHUCK_INSTR_H__
#define __CHUCK_INSTR_H__

#include <stdio.h>
#include "chuck_def.h"


// forward reference
class Chuck_VM;
class Chuck_VM_Shred;

// define
#define sint t_CKINT
#define uint t_CKUINT




//-----------------------------------------------------------------------------
// name: class Chuck_Instr
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr
{
public:
    virtual ~Chuck_Instr() { }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred ) = 0;

public:
    virtual const char * name() const;
    virtual const char * params() const 
    { return ""; }
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Op
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Op : public Chuck_Instr
{
public:
    inline void set( uint jmp ) { m_jmp = jmp; }

public:
    virtual const char * params() const
    { static char buffer[256]; sprintf( buffer, "%d", m_jmp ); return buffer; }

protected:
    uint m_jmp;
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Unary_Op
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Unary_Op : public Chuck_Instr
{
public:
    inline void set( uint val ) { m_val = val; }
    inline uint get() { return m_val; }

public:
    virtual const char * params() const
    { static char buffer[256]; sprintf( buffer, "%d", m_val ); return buffer; }

protected:
    uint m_val;
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Unary_Op2
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Unary_Op2 : public Chuck_Instr
{
public:
    inline void set( double val ) { m_val = val; }
    inline double get() { return m_val; }

public:
    virtual const char * params() const
    { static char buffer[256]; sprintf( buffer, "%.6f", m_val ); return buffer; }

protected:
    double m_val;
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_Op
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Op : public Chuck_Instr
{ };




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Add_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Add_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Inc_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Inc_int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};





//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Dec_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Dec_int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Complement_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Complement_int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Mod_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Mod_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Minus_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Minus_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Times_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Times_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Divide_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Divide_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Add_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Add_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Minus_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Minus_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Times_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Times_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Divide_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Divide_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Mod_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Mod_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Lt_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Lt_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Lt_int( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Gt_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Gt_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Gt_int( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Le_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Le_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Le_int( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Ge_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Ge_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Ge_int( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Eq_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Eq_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Eq_int( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Neq_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Neq_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Neq_int( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Lt_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Lt_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Lt_double( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Gt_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Gt_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Gt_double( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Le_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Le_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Le_double( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Ge_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Ge_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Ge_double( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Eq_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Eq_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Eq_double( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Neq_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Neq_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Neq_double( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Lt_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Lt_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Gt_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Gt_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Le_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Le_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Ge_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Ge_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Eq_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Eq_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Neq_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Neq_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Not_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Not_int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Negate_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Negate_int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Negate_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Negate_double : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Lt_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Lt_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Gt_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Gt_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Le_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Le_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Ge_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Ge_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Eq_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Eq_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Neq_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Neq_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_And
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_And : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_Or
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Or : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_Xor
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Xor : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_Shift_Right
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Shift_Right : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_Shift_Left
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Shift_Left : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_And
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_And : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Or
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Or : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Goto
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Goto : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Goto( uint jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Pop_Word
// desc: pop word from reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Pop_Word : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Pop_Word2
// desc: pop double word from reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Pop_Word2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Pop_Mem
// desc: pop word, and copy it to the mem stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Pop_Mem: public Chuck_Instr_Unary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Pop_Mem2
// desc: pop double word, and copy it to the mem stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Pop_Mem2: public Chuck_Instr_Unary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Imm
// desc: push immediate to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Imm : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Imm( uint val )
    { this->set( val ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Imm2
// desc: push double immediate to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Imm2 : public Chuck_Instr_Unary_Op2
{
public:
    Chuck_Instr_Reg_Push_Imm2( double val )
    { this->set( val ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Now
// desc: push value of now to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Now : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Start
// desc: push value of start to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Start : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Maybe
// desc: push immediate to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Maybe : public Chuck_Instr_Unary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Mem
// desc: push a variable from mem stack to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Mem : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Mem( uint src )
    { this->set( src ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Mem2
// desc: push a double variable from mem stack to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Mem2 : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Mem2( uint src )
    { this->set( src ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Deref
// desc: push a double variable from main memory to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Deref : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Deref( uint src, uint size )
    { this->set( src ); m_size = size; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    uint m_size;
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Deref
// desc: push a double variable from main memory to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Deref2 : public Chuck_Instr_Unary_Op2
{
public:
    Chuck_Instr_Reg_Push_Deref2( double src )
    { this->set( src ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Mem_Push_Imm
// desc: push a value unto mem stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Mem_Push_Imm : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Mem_Push_Imm( uint src )
    { this->set( src ); }
    
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};



//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Mem_Push_Imm2
// desc: push a double value unto mem stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Mem_Push_Imm2 : public Chuck_Instr_Unary_Op2
{
public:
    Chuck_Instr_Mem_Push_Imm2( double src )
    { this->set( src ); }
    
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};



//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Mem_Pop_Word
// desc: pop a value from mem stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Mem_Pop_Word : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Mem_Pop_Word2
// desc: pop a value from mem stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Mem_Pop_Word2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Nop
// desc: no op
//-----------------------------------------------------------------------------
class Chuck_Instr_Nop : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_EOC
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_EOC : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Alloc_Word
// desc: alloc local - leaves offset on operand stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Alloc_Word : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Alloc_Word( t_CKUINT offset  )
    { this->set( offset ); }

    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Alloc_DWord
// desc: alloc local - leaves offset on operand stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Alloc_DWord : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Alloc_DWord( t_CKUINT offset  )
    { this->set( offset ); }

    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Assign_Primitive
// desc: assign primitive (word)
//-----------------------------------------------------------------------------
class Chuck_Instr_Assign_Primitive : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Assign_Primitive2
// desc: assign primitive (2 word)
//-----------------------------------------------------------------------------
class Chuck_Instr_Assign_Primitive2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Assign_Primitive_Deref
// desc: assign primitive (word) from pointer
//-----------------------------------------------------------------------------
class Chuck_Instr_Assign_Primitive_Deref : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Assign_Primitive2_Deref
// desc: assign primitive (2 word) from pointer
//-----------------------------------------------------------------------------
class Chuck_Instr_Assign_Primitive2_Deref : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Assign_Object
// desc: assign object with reference counting and releasing previous reference
//-----------------------------------------------------------------------------
class Chuck_Instr_Assign_Object : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Assign_Object2
// desc: assign object with reference counting and NOT releasing previous
//-----------------------------------------------------------------------------
class Chuck_Instr_Assign_Object2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Assign_Object_Deref
// desc: object with reference counting from pointer
//-----------------------------------------------------------------------------
class Chuck_Instr_Assign_Object_Deref : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Chuck_Release_Object
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Chuck_Release_Object : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Func_Call
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Func_Call : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Func_Call2
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Func_Call2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Func_Call3
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Func_Call3 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Func_Call0
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Func_Call0 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Func_Return
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Func_Return : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Spork
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Spork : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Spork( uint v = 0 ) { this->set( v ); }
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Time_Advance
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Time_Advance : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_ADC
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_ADC : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_DAC
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_DAC : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Bunghole
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Bunghole : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_Link
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_Link : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_UnLink
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_UnLink : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_Alloc
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_Alloc : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_DeAlloc
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_DeAlloc : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_Ctrl
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_Ctrl : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_CGet
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_CGet : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_Ctrl2
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_Ctrl2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_CGet2
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_CGet2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_PMsg
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_PMsg : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_Ctrl_Op
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_Ctrl_Op : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_CGet_Op
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_CGet_Op : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_CGet_Gain
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_CGet_Gain : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_Ctrl_Gain
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_Ctrl_Gain : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_UGen_CGet_Last
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_UGen_CGet_Last : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_DLL_Load
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_DLL_Load : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_DLL_Unload
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_DLL_Unload : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Cast_double2int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Cast_double2int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Cast_int2double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Cast_int2double : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




#endif
