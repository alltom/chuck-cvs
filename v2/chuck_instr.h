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
struct Chuck_Type;




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
    inline void set( t_CKUINT jmp ) { m_jmp = jmp; }

public:
    virtual const char * params() const
    { static char buffer[256]; sprintf( buffer, "%d", m_jmp ); return buffer; }

protected:
    t_CKUINT m_jmp;
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Unary_Op
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Unary_Op : public Chuck_Instr
{
public:
    inline void set( t_CKUINT val ) { m_val = val; }
    inline t_CKUINT get() { return m_val; }

public:
    virtual const char * params() const
    { static char buffer[256]; sprintf( buffer, "%d", m_val ); return buffer; }

protected:
    t_CKUINT m_val;
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Unary_Op2
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Unary_Op2 : public Chuck_Instr
{
public:
    inline void set( t_CKFLOAT val ) { m_val = val; }
    inline t_CKFLOAT get() { return m_val; }

public:
    virtual const char * params() const
    { static char buffer[256]; sprintf( buffer, "%.6f", m_val ); return buffer; }

protected:
    t_CKFLOAT m_val;
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
// name: class Chuck_Instr_Mod_int_Reverse
// desc: same as mod_int, operands reversed, for %=>
//-----------------------------------------------------------------------------
class Chuck_Instr_Mod_int_Reverse : public Chuck_Instr_Binary_Op
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
// name: class Chuck_Instr_Minus_int_Reverse
// desc: same as minus_int, operands reversed, for -=>
//-----------------------------------------------------------------------------
class Chuck_Instr_Minus_int_Reverse : public Chuck_Instr_Binary_Op
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
// name: class Chuck_Instr_Divide_int_Reverse
// desc: same as divide_int, operands reversed, for /=>
//-----------------------------------------------------------------------------
class Chuck_Instr_Divide_int_Reverse : public Chuck_Instr_Binary_Op
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
// name: class Chuck_Instr_Minus_double_Reverse
// desc: same as minus_double, operands reversed, for -=>
//-----------------------------------------------------------------------------
class Chuck_Instr_Minus_double_Reverse : public Chuck_Instr_Binary_Op
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
// name: class Chuck_Instr_Divide_double_Reverse
// desc: same as divide_double, operands reversed, for /=>
//-----------------------------------------------------------------------------
class Chuck_Instr_Divide_double_Reverse : public Chuck_Instr_Binary_Op
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
// name: class Chuck_Instr_Mod_double_Reverse
// desc: same as mod_double, operands reversed, for %=>
//-----------------------------------------------------------------------------
class Chuck_Instr_Mod_double_Reverse : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Add_int_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Add_int_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Mod_int_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Mod_int_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Minus_int_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Minus_int_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Times_int_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Times_int_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Divide_int_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Divide_int_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Add_double_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Add_double_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Minus_double_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Minus_double_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Times_double_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Times_double_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Divide_double_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Divide_double_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Mod_double_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Mod_double_Assign : public Chuck_Instr_Binary_Op
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
    Chuck_Instr_Branch_Lt_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Gt_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Gt_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Gt_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Le_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Le_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Le_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Ge_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Ge_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Ge_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Eq_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Eq_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Eq_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Neq_int
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Neq_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Neq_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Lt_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Lt_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Lt_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Gt_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Gt_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Gt_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Le_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Le_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Le_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Ge_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Ge_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Ge_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Eq_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Eq_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Eq_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Branch_Neq_double
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Branch_Neq_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Neq_double( t_CKUINT jmp ) { this->set( jmp ); }
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
// name: class Chuck_Instr_Binary_Shift_Right_Reverse
// desc: same as shift_right, operands reversed, for >>=>
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Shift_Right_Reverse : public Chuck_Instr_Binary_Op
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
// name: class Chuck_Instr_Binary_Shift_Left_Reverse
// desc: same as shift_left, operands reversed, for <<=>
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Shift_Left_Reverse : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_And_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_And_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_Or_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Or_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_Xor_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Xor_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_Shift_Right_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Shift_Right_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Binary_Shift_Left_Assign
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Binary_Shift_Left_Assign : public Chuck_Instr_Binary_Op
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
    Chuck_Instr_Goto( t_CKUINT jmp ) { this->set( jmp ); }
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
// desc: pop t_CKFLOAT word from reg stack
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
// desc: pop t_CKFLOAT word, and copy it to the mem stack
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
    Chuck_Instr_Reg_Push_Imm( t_CKUINT val )
    { this->set( val ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Imm2
// desc: push t_CKFLOAT immediate to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Imm2 : public Chuck_Instr_Unary_Op2
{
public:
    Chuck_Instr_Reg_Push_Imm2( t_CKFLOAT val )
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
    Chuck_Instr_Reg_Push_Mem( t_CKUINT src )
    { this->set( src ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Mem2
// desc: push a t_CKFLOAT variable from mem stack to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Mem2 : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Mem2( t_CKUINT src )
    { this->set( src ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Mem_Addr
// desc: push addr from mem stack to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Mem_Addr : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Mem_Addr( t_CKUINT src )
    { this->set( src ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Deref
// desc: push value from pointer
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Deref : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Deref( t_CKUINT src, t_CKUINT size )
    { this->set( src ); m_size = size; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    t_CKUINT m_size;
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Reg_Push_Deref
// desc: push a t_CKFLOAT variable from main memory to reg stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Reg_Push_Deref2 : public Chuck_Instr_Unary_Op2
{
public:
    Chuck_Instr_Reg_Push_Deref2( t_CKFLOAT src )
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
    Chuck_Instr_Mem_Push_Imm( t_CKUINT src )
    { this->set( src ); }
    
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};



//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Mem_Push_Imm2
// desc: push a t_CKFLOAT value unto mem stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Mem_Push_Imm2 : public Chuck_Instr_Unary_Op2
{
public:
    Chuck_Instr_Mem_Push_Imm2( t_CKFLOAT src )
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
// name: class Chuck_Instr_Instantiate_Object
// desc: instantiate object - leaves reference value on operand stack
//-----------------------------------------------------------------------------
class Chuck_Instr_Instantiate_Object : public Chuck_Instr
{
public:
    Chuck_Instr_Instantiate_Object( Chuck_Type * type )
    { this->type = type; }

    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );

public:
    Chuck_Type * type;
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
// name: class Chuck_Instr_Assign_Object
// desc: assign object with reference counting and releasing previous reference
//-----------------------------------------------------------------------------
class Chuck_Instr_Assign_Object : public Chuck_Instr
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
    Chuck_Instr_Spork( t_CKUINT v = 0 ) { this->set( v ); }
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
// name: class Chuck_Instr_Array_Alloc
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Array_Alloc : public Chuck_Instr
{
public:
    Chuck_Instr_Array_Alloc( t_CKUINT depth, Chuck_Type * base_type );
    virtual ~Chuck_Instr_Array_Alloc();

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const { return m_param_str; }

protected:
    t_CKUINT m_depth;
    Chuck_Type * m_type_ref;
    char * m_param_str;
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Array_Access
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Array_Access : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Array_Access( t_CKUINT size )
    { this->set( size ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: class Chuck_Instr_Array_Access_Multi
// desc: ...
//-----------------------------------------------------------------------------
class Chuck_Instr_Array_Access_Multi : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Array_Access_Multi( t_CKUINT depth, t_CKUINT size )
    { this->set( size ); m_depth = depth; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params()
    { static char buffer[256];
      sprintf( buffer, "depth=%d, size=%d", m_depth, m_val );
      return buffer; }

protected:
    t_CKUINT m_depth;
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
