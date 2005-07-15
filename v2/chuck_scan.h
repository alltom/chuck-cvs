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
// file: chuck_scan.h
// desc: chuck type-system / type-checker pre-scan
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Summer 2005 - original
//-----------------------------------------------------------------------------
#ifndef __CHUCK_SCAN_H__
#define __CHUCK_SCAN_H__

#include "chuck_type.h"

// initialize scan
t_CKBOOL type_engine_init_scan( Chuck_Env * env );
// scan a program into the env
t_CKBOOL type_engine_scan_prog( Chuck_Env * env, a_Program prog, 
                                te_HowMuch val = te_do_all );
// scan a statement
t_CKBOOL type_engine_scan_stmt( Chuck_Env * env, a_Stmt stmt );
// scan a class
t_CKBOOL type_engine_scan_class_def( Chuck_Env * env, a_Class_Def def );
// scan an expression
t_CKBOOL type_engine_scan_exp( Chuck_Env * env, a_Exp exp );
// scan a program into the env
t_CKBOOL type_engine_2ndscan_prog( Chuck_Env * env, a_Program prog,
                                   te_HowMuch val = te_do_all );
// scan a class
t_CKBOOL type_engine_2ndscan_class_def( Chuck_Env * env, a_Class_Def def );
// scan a statement
t_CKBOOL type_engine_2ndscan_stmt( Chuck_Env * env, a_Stmt stmt );
// scan an expression
t_CKBOOL type_engine_2ndscan_exp( Chuck_Env * env, a_Exp exp );



#endif
