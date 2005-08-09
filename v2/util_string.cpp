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
// name: util_string.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Summer 2005
//-----------------------------------------------------------------------------
#include "util_string.h"
using namespace std;




//-----------------------------------------------------------------------------
// name: itoa()
// desc: ...
//-----------------------------------------------------------------------------
string itoa( t_CKINT val )
{
    char buffer[128];
    sprintf( buffer, "%li", val );
    return string(buffer);
}



//-----------------------------------------------------------------------------
// name: ftoa()
// desc: ...
//-----------------------------------------------------------------------------
string ftoa( t_CKFLOAT val, t_CKUINT precision )
{
    char str[32];
    char buffer[128];
    if( precision > 32 ) precision = 32;
    sprintf( str, "%%.%lif", precision );
    sprintf( buffer, str, val );
    return string(buffer);
}




//-----------------------------------------------------------------------------
// name: tolower()
// desc: ...
//-----------------------------------------------------------------------------
string tolower( const string & str )
{
    string s = str;
    t_CKUINT len = s.length();

    // loop
    for( t_CKUINT i = 0; i < len; i++ )
        if( s[i] >= 'A' && s[i] <= 'Z' )
            s[i] += 32;

    return s;
}




//-----------------------------------------------------------------------------
// name: toupper()
// desc: ...
//-----------------------------------------------------------------------------
string toupper( const string & str )
{
    string s = str;
    t_CKUINT len = s.length();

    // loop
    for( t_CKUINT i = 0; i < len; i++ )
        if( s[i] >= 'a' && s[i] <= 'z' )
            s[i] -= 32;

    return s;
}
