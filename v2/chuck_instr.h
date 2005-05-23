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
struct Chuck_VM;
struct Chuck_VM_Shred;
struct Chuck_Type;
struct Chuck_Func;




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr
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
// name: struct Chuck_Instr_Branch_Op
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Op : public Chuck_Instr
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
// name: struct Chuck_Instr_Unary_Op
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Unary_Op : public Chuck_Instr
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
// name: struct Chuck_Instr_Unary_Op2
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Unary_Op2 : public Chuck_Instr
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
// name: struct Chuck_Instr_Binary_Op
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Op : public Chuck_Instr
{ };




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Add_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Add_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Inc_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Inc_int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Dec_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Dec_int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Complement_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Complement_int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mod_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mod_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mod_int_Reverse
// desc: same as mod_int, operands reversed, for %=>
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mod_int_Reverse : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Minus_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Minus_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Minus_int_Reverse
// desc: same as minus_int, operands reversed, for -=>
//-----------------------------------------------------------------------------
struct Chuck_Instr_Minus_int_Reverse : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Times_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Times_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Divide_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Divide_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Divide_int_Reverse
// desc: same as divide_int, operands reversed, for /=>
//-----------------------------------------------------------------------------
struct Chuck_Instr_Divide_int_Reverse : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Add_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Add_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Minus_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Minus_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Minus_double_Reverse
// desc: same as minus_double, operands reversed, for -=>
//-----------------------------------------------------------------------------
struct Chuck_Instr_Minus_double_Reverse : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Times_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Times_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Divide_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Divide_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Divide_double_Reverse
// desc: same as divide_double, operands reversed, for /=>
//-----------------------------------------------------------------------------
struct Chuck_Instr_Divide_double_Reverse : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mod_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mod_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mod_double_Reverse
// desc: same as mod_double, operands reversed, for %=>
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mod_double_Reverse : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Add_int_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Add_int_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mod_int_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mod_int_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Minus_int_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Minus_int_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Times_int_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Times_int_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Divide_int_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Divide_int_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Add_double_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Add_double_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Minus_double_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Minus_double_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Times_double_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Times_double_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Divide_double_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Divide_double_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mod_double_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mod_double_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Lt_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Lt_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Lt_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Gt_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Gt_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Gt_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Le_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Le_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Le_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Ge_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Ge_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Ge_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Eq_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Eq_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Eq_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Neq_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Neq_int : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Neq_int( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Lt_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Lt_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Lt_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Gt_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Gt_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Gt_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Le_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Le_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Le_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Ge_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Ge_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Ge_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Eq_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Eq_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Eq_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Branch_Neq_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Branch_Neq_double : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Branch_Neq_double( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Lt_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Lt_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Gt_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Gt_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Le_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Le_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Ge_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Ge_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Eq_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Eq_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Neq_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Neq_int : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Not_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Not_int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Negate_int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Negate_int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Negate_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Negate_double : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Lt_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Lt_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Gt_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Gt_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Le_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Le_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Ge_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Ge_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Eq_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Eq_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Neq_double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Neq_double : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_And
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_And : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_Or
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Or : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_Xor
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Xor : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_Shift_Right
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Shift_Right : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_Shift_Right_Reverse
// desc: same as shift_right, operands reversed, for >>=>
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Shift_Right_Reverse : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_Shift_Left
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Shift_Left : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_Shift_Left_Reverse
// desc: same as shift_left, operands reversed, for <<=>
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Shift_Left_Reverse : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_And_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_And_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_Or_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Or_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_Xor_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Xor_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_Shift_Right_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Shift_Right_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Binary_Shift_Left_Assign
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Binary_Shift_Left_Assign : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_And
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_And : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Or
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Or : public Chuck_Instr_Binary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Goto
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Goto : public Chuck_Instr_Branch_Op
{
public:
    Chuck_Instr_Goto( t_CKUINT jmp ) { this->set( jmp ); }
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Pop_Word
// desc: pop word from reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Pop_Word : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Pop_Word2
// desc: pop t_CKFLOAT word from reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Pop_Word2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Pop_Mem
// desc: pop word, and copy it to the mem stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Pop_Mem: public Chuck_Instr_Unary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Pop_Mem2
// desc: pop t_CKFLOAT word, and copy it to the mem stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Pop_Mem2: public Chuck_Instr_Unary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Imm
// desc: push immediate to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Imm : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Imm( t_CKUINT val )
    { this->set( val ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Imm2
// desc: push t_CKFLOAT immediate to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Imm2 : public Chuck_Instr_Unary_Op2
{
public:
    Chuck_Instr_Reg_Push_Imm2( t_CKFLOAT val )
    { this->set( val ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Dup_Last
// desc: duplicate last value on reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Dup_Last : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Dup_Last2
// desc: duplicate last float on reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Dup_Last2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Now
// desc: push value of now to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Now : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Me
// desc: push value of me to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Me : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_This
// desc: push value of now to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_This : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Start
// desc: push value of start to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Start : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Maybe
// desc: push immediate to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Maybe : public Chuck_Instr_Unary_Op
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Mem
// desc: push a variable from mem stack to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Mem : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Mem( t_CKUINT src, t_CKBOOL use_base = FALSE )
    { this->set( src ); base = use_base; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "src=%d, base=%d", m_val, base );
      return buffer; }

protected:
    // use global stack base
    t_CKBOOL base;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Mem2
// desc: push a t_CKFLOAT variable from mem stack to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Mem2 : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Mem2( t_CKUINT src, t_CKBOOL use_base = FALSE )
    { this->set( src ); base = use_base; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "src=%d, base=%d", m_val, base );
      return buffer; }

protected:
    // use global stack base
    t_CKBOOL base;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Mem_Addr
// desc: push addr from mem stack to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Mem_Addr : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Mem_Addr( t_CKUINT src, t_CKBOOL use_base )
    { this->set( src ); base = use_base; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "src=%d, base=%d", m_val, base );
      return buffer; }

protected:
    // use global stack base
    t_CKBOOL base;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Deref
// desc: push value from pointer
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Deref : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Reg_Push_Deref( t_CKUINT src, t_CKUINT size )
    { this->set( src ); m_size = size; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    t_CKUINT m_size;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Reg_Push_Deref
// desc: push a t_CKFLOAT variable from main memory to reg stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Reg_Push_Deref2 : public Chuck_Instr_Unary_Op2
{
public:
    Chuck_Instr_Reg_Push_Deref2( t_CKFLOAT src )
    { this->set( src ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mem_Set_Imm
// desc: set a value unto mem stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mem_Set_Imm : public Chuck_Instr
{
public:
    Chuck_Instr_Mem_Set_Imm( t_CKUINT offset, t_CKUINT val )
    { m_offset = offset; m_val = val; }
    
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "offset=%d, value=%d", m_offset, m_val );
      return buffer; }

protected:
    t_CKUINT m_offset;
    t_CKUINT m_val;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mem_Set_Imm2
// desc: set a value unto mem stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mem_Set_Imm2 : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Mem_Set_Imm2( t_CKUINT offset, t_CKFLOAT val )
    { m_offset = offset; m_val = val; }
    
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "offset=%d, value=%f", m_offset, m_val );
      return buffer; }

protected:
    t_CKUINT m_offset;
    t_CKFLOAT m_val;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mem_Push_Imm
// desc: push a value unto mem stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mem_Push_Imm : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Mem_Push_Imm( t_CKUINT src )
    { this->set( src ); }
    
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};



//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mem_Push_Imm2
// desc: push a t_CKFLOAT value unto mem stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mem_Push_Imm2 : public Chuck_Instr_Unary_Op2
{
public:
    Chuck_Instr_Mem_Push_Imm2( t_CKFLOAT src )
    { this->set( src ); }
    
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};



//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mem_Pop_Word
// desc: pop a value from mem stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mem_Pop_Word : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Mem_Pop_Word2
// desc: pop a value from mem stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Mem_Pop_Word2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Nop
// desc: no op
//-----------------------------------------------------------------------------
struct Chuck_Instr_Nop : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_EOC
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_EOC : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Alloc_Word
// desc: alloc local - leaves addr on operand stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Alloc_Word : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Alloc_Word( t_CKUINT offset  )
    { this->set( offset ); }

    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Alloc_Word2
// desc: alloc local - leaves addr on operand stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Alloc_Word2 : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Alloc_Word2( t_CKUINT offset  )
    { this->set( offset ); }

    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Alloc_Member_Word
// desc: alloc member - leaves addr on operand stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Alloc_Member_Word : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Alloc_Member_Word( t_CKUINT offset  )
    { this->set( offset ); }

    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Alloc_Member_Word2
// desc: alloc member - leaves addr on operand stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Alloc_Member_Word2 : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Alloc_Member_Word2( t_CKUINT offset  )
    { this->set( offset ); }

    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Instantiate_Object
// desc: instantiate object - leaves reference value on operand stack
//-----------------------------------------------------------------------------
struct Chuck_Instr_Instantiate_Object : public Chuck_Instr
{
public:
    Chuck_Instr_Instantiate_Object( Chuck_Type * t )
    { this->type = t; }

    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const;

public:
    Chuck_Type * type;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Pre_Constructor
// desc: preconstruct object
//-----------------------------------------------------------------------------
struct Chuck_Instr_Pre_Constructor : public Chuck_Instr
{
public:
    Chuck_Instr_Pre_Constructor( Chuck_VM_Code * pre, t_CKUINT offset )
    { pre_ctor = pre; this->stack_offset = offset; }

    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    // virtual const char * params() const;

public:
    Chuck_VM_Code * pre_ctor;
    t_CKUINT stack_offset;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Pre_Ctor_Array_Top
// desc: preconstruct object in array (top)
//-----------------------------------------------------------------------------
struct Chuck_Instr_Pre_Ctor_Array_Top : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Pre_Ctor_Array_Top( Chuck_Type * t )
    { this->type = t; }

    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    // virtual const char * params() const;

public:
    Chuck_Type * type;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Pre_Ctor_Array_Bottom
// desc: preconstruct object in array (bottom)
//-----------------------------------------------------------------------------
struct Chuck_Instr_Pre_Ctor_Array_Bottom : public Chuck_Instr_Unary_Op
{
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    // virtual const char * params() const;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Pre_Ctor_Array_Post
// desc: preconstruct object in array (clean up)
//-----------------------------------------------------------------------------
struct Chuck_Instr_Pre_Ctor_Array_Post : public Chuck_Instr
{
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    // virtual const char * params() const;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Assign_Primitive
// desc: assign primitive (word)
//-----------------------------------------------------------------------------
struct Chuck_Instr_Assign_Primitive : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Assign_Primitive2
// desc: assign primitive (2 word)
//-----------------------------------------------------------------------------
struct Chuck_Instr_Assign_Primitive2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Assign_Object
// desc: assign object with reference counting and releasing previous reference
//-----------------------------------------------------------------------------
struct Chuck_Instr_Assign_Object : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Assign_Object
// desc: map object with reference counting and releasing previous reference
//-----------------------------------------------------------------------------
struct Chuck_Instr_Assign_Object_To_Map : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Assign_Object_To_Map( t_CKUINT size )
    { this->set( size ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Chuck_Release_Object
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Chuck_Release_Object : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Func_To_Code
// desc: Chuck_Func * to Chuck_VM_Code *
//-----------------------------------------------------------------------------
struct Chuck_Instr_Func_To_Code : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Func_Call
// desc: user-defined function call
//-----------------------------------------------------------------------------
struct Chuck_Instr_Func_Call : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Func_Call_Member
// desc: imported member function call with return
//-----------------------------------------------------------------------------
struct Chuck_Instr_Func_Call_Member : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Func_Call_Member( t_CKUINT ret_size )
    { this->set( ret_size ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Func_Call_Static
// desc: imported static function call with return
//-----------------------------------------------------------------------------
struct Chuck_Instr_Func_Call_Static : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Func_Call_Static( t_CKUINT ret_size )
    { this->set( ret_size ); }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Func_Return
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Func_Return : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Spork
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Spork : public Chuck_Instr_Unary_Op
{
public:
    Chuck_Instr_Spork( t_CKUINT v = 0 ) { this->set( v ); }
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Time_Advance
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Time_Advance : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Array_Init
// desc: for [ ... ] values
//-----------------------------------------------------------------------------
struct Chuck_Instr_Array_Init : public Chuck_Instr
{
public:
    Chuck_Instr_Array_Init( Chuck_Type * the_type, t_CKINT length );
    virtual ~Chuck_Instr_Array_Init();

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const { return m_param_str; }

protected:
    Chuck_Type * m_type_ref;
    t_CKINT m_length;
    t_CKBOOL m_is_obj;
    char * m_param_str;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Array_Alloc
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Array_Alloc : public Chuck_Instr
{
public:
    Chuck_Instr_Array_Alloc( t_CKUINT depth, Chuck_Type * the_type,
        t_CKUINT offset, t_CKBOOL ref );
    virtual ~Chuck_Instr_Array_Alloc();

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const { return m_param_str; }

protected:
    t_CKUINT m_depth;
    Chuck_Type * m_type_ref;
    t_CKBOOL m_is_obj;
    char * m_param_str;
    t_CKUINT m_stack_offset;
    t_CKBOOL m_is_ref;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Array_Access
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Array_Access : public Chuck_Instr
{
public:
    Chuck_Instr_Array_Access( t_CKUINT size, t_CKUINT emit_addr, 
        t_CKUINT istr = FALSE )
    { m_size = size; m_emit_addr = emit_addr; m_istr = istr; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "size=%d, emit_addr=%d istr=%d", 
               m_size, m_emit_addr, m_istr );
      return buffer; }

protected:
    t_CKUINT m_size;
    t_CKUINT m_emit_addr;
    t_CKUINT m_istr;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Array_Map_Access
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Array_Map_Access : public Chuck_Instr
{
public:
    Chuck_Instr_Array_Map_Access( t_CKUINT size, t_CKUINT emit_addr )
    { m_size = size; m_emit_addr = emit_addr; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "size=%d, emit_addr=%d", m_size, m_emit_addr );
      return buffer; }

protected:
    t_CKUINT m_size;
    t_CKUINT m_emit_addr;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Array_Access_Multi
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Array_Access_Multi : public Chuck_Instr
{
public:
    Chuck_Instr_Array_Access_Multi( t_CKUINT depth, t_CKUINT size, t_CKUINT emit_addr )
    { m_size = size; m_depth = depth; m_emit_addr = emit_addr; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "depth=%d, size=%d, emit_addr=%d", m_depth, m_size, m_emit_addr );
      return buffer; }

protected:
    t_CKUINT m_depth;
    t_CKUINT m_size;
    t_CKUINT m_emit_addr;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Dot_Member_Data
// desc: access the member data of object by offset
//-----------------------------------------------------------------------------
struct Chuck_Instr_Dot_Member_Data : public Chuck_Instr
{
public:
    Chuck_Instr_Dot_Member_Data( t_CKUINT offset, t_CKUINT size, t_CKUINT emit_addr )
    { m_offset = offset; m_size = size; m_emit_addr = emit_addr; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "offset=%d, size=%d, emit_addr=%d", m_offset, m_size, m_emit_addr );
      return buffer; }

protected:
    t_CKUINT m_offset;
    t_CKUINT m_size;
    t_CKUINT m_emit_addr;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Dot_Member_Func
// desc: access the member function of object by offset
//-----------------------------------------------------------------------------
struct Chuck_Instr_Dot_Member_Func : public Chuck_Instr
{
public:
    Chuck_Instr_Dot_Member_Func( t_CKUINT offset )
    { m_offset = offset; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "offset=%d", m_offset );
      return buffer; }

protected:
    t_CKUINT m_offset;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Dot_Static_Data
// desc: access the static data of object by offset
//-----------------------------------------------------------------------------
struct Chuck_Instr_Dot_Static_Data : public Chuck_Instr
{
public:
    Chuck_Instr_Dot_Static_Data( t_CKUINT offset, t_CKUINT size, t_CKUINT emit_addr )
    { m_offset = offset; m_size = size; m_emit_addr = emit_addr; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "offset=%d, size=%d, emit_addr=%d", m_offset, m_size, m_emit_addr );
      return buffer; }

protected:
    t_CKUINT m_offset;
    t_CKUINT m_size;
    t_CKUINT m_emit_addr;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Dot_Static_Import_Data
// desc: access the static data of object by pointer
//-----------------------------------------------------------------------------
struct Chuck_Instr_Dot_Static_Import_Data : public Chuck_Instr
{
public:
    Chuck_Instr_Dot_Static_Import_Data( void * addr, t_CKUINT size, t_CKUINT emit_addr )
    { m_addr = addr; m_size = size; m_emit_addr = emit_addr; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "addr=%d, size=%d, emit_addr=%d", (t_CKUINT)m_addr, m_size, m_emit_addr );
      return buffer; }

protected:
    void * m_addr;
    t_CKUINT m_size;
    t_CKUINT m_emit_addr;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Dot_Static_Func
// desc: access the static function of object
//-----------------------------------------------------------------------------
struct Chuck_Instr_Dot_Static_Func : public Chuck_Instr
{
public:
    Chuck_Instr_Dot_Static_Func( Chuck_Func * func )
    { m_func = func; }

public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const
    { static char buffer[256];
      sprintf( buffer, "func=%d", (t_CKUINT)m_func );
      return buffer; }

protected:
    Chuck_Func * m_func;
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_ADC
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_ADC : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_DAC
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_DAC : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Bunghole
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Bunghole : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_UGen_Link
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_UGen_Link : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_UGen_UnLink
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_UGen_UnLink : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_UGen_Ctrl
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_UGen_Ctrl : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_UGen_CGet
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_UGen_CGet : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_UGen_Ctrl2
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_UGen_Ctrl2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_UGen_CGet2
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_UGen_CGet2 : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_UGen_PMsg
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_UGen_PMsg : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Cast_double2int
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Cast_double2int : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Cast_int2double
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Cast_int2double : public Chuck_Instr
{
public:
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Instr_Hack
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Instr_Hack : public Chuck_Instr
{
public:
    Chuck_Instr_Hack( Chuck_Type * type );
    virtual ~Chuck_Instr_Hack();
    virtual void execute( Chuck_VM * vm, Chuck_VM_Shred * shred );
    virtual const char * params() const;

protected:
    Chuck_Type * m_type_ref;
};




// runtime functions
Chuck_Object * instantiate_and_initialize_object( Chuck_Type * type, Chuck_VM_Shred * shred );
// initialize object using Type
t_CKBOOL initialize_object( Chuck_Object * obj, Chuck_Type * type );




#endif
