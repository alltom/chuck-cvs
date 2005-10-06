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
// file: chuck_shell.h
// desc: ...
//
// author: Spencer Salazar (ssalazar@princeton.edu)
// date: Autumn 2005
//-----------------------------------------------------------------------------
#ifndef __CHUCK_SHELL_H__
#define __CHUCK_SHELL_H__

#include "chuck_def.h"
#include "chuck_vm.h"
#include "chuck_compile.h"

#include <map>
#include <string>


// forward references
class Chuck_Shell_Mode;
class Chuck_Shell_UI;

//-----------------------------------------------------------------------------
// name: class Chuck_Shell
// desc: controller class for facilitating interaction between a shell UI and a 
//      shell mode.
//-----------------------------------------------------------------------------
class Chuck_Shell
{
public:
    Chuck_Shell();
    ~Chuck_Shell();
    
    t_CKBOOL init(Chuck_VM *,Chuck_Compiler *,Chuck_Shell_UI *);
    void run();
    void stop();

protected:
    std::map< std::string, Chuck_Shell_Mode * > modes;
    Chuck_Shell_Mode *current_mode;
    Chuck_Shell_UI *ui;
    Chuck_VM *vm;
    Chuck_Compiler *compiler;
    t_CKBOOL initialized;
    
};

//-----------------------------------------------------------------------------
// name: class Chuck_Shell_Mode
// desc: superclass to various types of Chuck shell modes
//-----------------------------------------------------------------------------
class Chuck_Shell_Mode
{
public:
    Chuck_Shell_Mode();
    ~Chuck_Shell_Mode();
    
    virtual t_CKBOOL init(Chuck_VM *,Chuck_Compiler *);
    virtual t_CKBOOL execute(const std::string &,std::string &)=0;
    virtual t_CKBOOL switch_vm(Chuck_VM *,Chuck_VM **);
    virtual t_CKBOOL switch_compiler(Chuck_Compiler *,Chuck_Compiler **);
    
protected:
    Chuck_VM *vm;
    Chuck_Compiler *compiler;
    t_CKBOOL initialized;
};

//-----------------------------------------------------------------------------
// name: class Chuck_Shell_UI
// desc: superclass to various types of Chuck UIs
//-----------------------------------------------------------------------------
class Chuck_Shell_UI
{
public:
    Chuck_Shell_UI();
    ~Chuck_Shell_UI();
    
    virtual t_CKBOOL init();
    virtual t_CKBOOL nextCommand(std::string &)=0;
    virtual void nextResult(const std::string &)=0;
};

// prototype for shred thread routine
void * shell_cb( void * p );


#endif //__CHUCK_SHELL_H__
