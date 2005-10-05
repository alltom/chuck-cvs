
//-----------------------------------------------------------------------------
// file: chuck_console.h
// desc: ...
//
// author: Spencer Salazar (ssalazar@princeton.edu)
// date: Autumn 2005
//-----------------------------------------------------------------------------
#ifndef __CHUCK_CONSOLE_H__
#define __CHUCK_CONSOLE_H__

#include "chuck_def.h"
#include "chuck_shell.h"
#include <readline/readline.h>

//-----------------------------------------------------------------------------
// name: class Chuck_Console
// desc: for ttyp access to chuck shell
//-----------------------------------------------------------------------------

class Chuck_Console : Chuck_Shell_UI
{
public:
    Chuck_Console();
    ~Chuck_Console();

    t_CKBOOL init();
    t_CKBOOL nextCommand(std::string &);
    void nextResult(const std::string &);

};

#endif //__CHUCK_CONSOLE_H__
