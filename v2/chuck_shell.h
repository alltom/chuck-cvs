
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
    static std::map<const char *,Chuck_Shell_Mode *> modes;
    Chuck_Shell_Mode *current_mode;
    Chuck_Shell_UI *ui;
    Chuck_VM *vm;
    Chuck_Compiler *compiler;
    
};

//-----------------------------------------------------------------------------
// name: class Chuck_Shell_Mode
// desc: superclass to various types of Chuck shells
//-----------------------------------------------------------------------------
class Chuck_Shell_Mode
{
public:
    Chuck_Shell_Mode();
    ~Chuck_Shell_Mode();
    
    virtual t_CKBOOL init(Chuck_VM *,Chuck_Compiler *);
    virtual t_CKBOOL execute(const std::string &,std::string &)=0;
    
protected:
    Chuck_VM *vm;
    Chuck_Compiler *compiler;
};

class Chuck_Shell_UI
{
public:
    Chuck_Shell_UI();
    ~Chuck_Shell_UI();
    
    virtual t_CKBOOL init();
    virtual t_CKBOOL nextCommand(std::string &)=0;
    virtual void nextResult(const std::string &)=0;
}

#endif //__CHUCK_SHELL_H__
