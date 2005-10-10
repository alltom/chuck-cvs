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
// file: ulib_std.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#ifdef AJAY
  #include <fstream>
  #include <iostream>
#endif
#include "ulib_std.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "util_math.h"
#include "util_string.h"
#include "chuck_type.h"
#include "chuck_instr.h"

#if defined(__PLATFORM_WIN32__)

#include <windows.h>

int setenv( const char *n, const char *v, int i )
{
    return !SetEnvironmentVariable(n, v);
}

#endif

#ifdef AJAY

#include "chuck_type.h"
#include "chuck_oo.h"
using namespace std;

CK_DLL_CTOR( VCR_ctor );
CK_DLL_MFUN( VCR_load );
CK_DLL_MFUN( VCR_reset );
CK_DLL_MFUN( VCR_seek );
CK_DLL_MFUN( VCR_more );
CK_DLL_MFUN( VCR_curr );
CK_DLL_MFUN( VCR_next );
CK_DLL_MFUN( VCR_pos );
CK_DLL_MFUN( VCR_size );
CK_DLL_MFUN( VCR_name );

static t_CKUINT VCR_offset_data = 0;

#endif


//-----------------------------------------------------------------------------
// name: libstd_query()
// desc: query entry point
//-----------------------------------------------------------------------------
DLL_QUERY libstd_query( Chuck_DL_Query * QUERY )
{
    // set name
    QUERY->setname( QUERY, "Std" );

    /*! \example
    std.rand2f( 100.0, 1000.0 ) => stdout;
    */

    // begin class
    QUERY->begin_class( QUERY, "std", "Object" );

    // add abs
    QUERY->add_sfun( QUERY, abs_impl, "float", "abs" );
    QUERY->add_arg( QUERY, "float", "value" );
    
    // add rand
    QUERY->add_sfun( QUERY, rand_impl, "int", "rand"); //! return int between 0 and RAND_MAX
    
    // add rand2
    QUERY->add_sfun( QUERY, rand2_impl, "int", "rand2" ); //! integer between [min,max]
    QUERY->add_arg( QUERY, "int", "min" ); 
    QUERY->add_arg( QUERY, "int", "max" ); 
    
    // add rand
    QUERY->add_sfun( QUERY, randf_impl, "float", "randf" ); //! rand between -1.0,1.0

    // add rand2
    QUERY->add_sfun( QUERY, rand2f_impl, "float", "rand2f" ); //! rand between min and max
    QUERY->add_arg( QUERY, "float", "min" );
    QUERY->add_arg( QUERY, "float", "max" );

    // add sgn
    QUERY->add_sfun( QUERY, sgn_impl, "float", "sgn" ); //! return sign of value (-1, 0, 1)
    QUERY->add_arg( QUERY, "float", "value" );

    // add system
    //! see \example std.ck
    QUERY->add_sfun( QUERY, system_impl, "int", "system" ); //! issue system command
    QUERY->add_arg( QUERY, "string", "cmd" );

    // add atoi
    QUERY->add_sfun( QUERY, atoi_impl, "int", "atoi" ); //! string to integer
    QUERY->add_arg( QUERY, "string", "value" );

    // add atof
    QUERY->add_sfun( QUERY, atof_impl, "float", "atof" ); //! string to float
    QUERY->add_arg( QUERY, "string", "value" );

    // add itoa
    QUERY->add_sfun( QUERY, itoa_impl, "string", "itoa" ); //! int to string
    QUERY->add_arg( QUERY, "int", "i" );

    // add ftoa
    QUERY->add_sfun( QUERY, ftoa_impl, "string", "ftoa" ); //! float to string
    QUERY->add_arg( QUERY, "float", "f" );
    QUERY->add_arg( QUERY, "int", "precision" );

    // add getenv
    QUERY->add_sfun( QUERY, getenv_impl, "string", "getenv" ); //! fetch environment variable
    QUERY->add_arg( QUERY, "string", "value" );

    // add setenv
    QUERY->add_sfun( QUERY, setenv_impl, "int", "setenv" ); //! set environment variable
    QUERY->add_arg( QUERY, "string", "key" );
    QUERY->add_arg( QUERY, "string", "value" );

    // add mtof
    //! see \example mand-o-matic.ck
    QUERY->add_sfun( QUERY, mtof_impl, "float", "mtof" ); //! midi note to frequency
    QUERY->add_arg( QUERY, "float", "value" );

    // add ftom
    QUERY->add_sfun( QUERY, ftom_impl, "float", "ftom" ); //! frequency to midi note
    QUERY->add_arg( QUERY, "float", "value" );

    // add powtodb
    QUERY->add_sfun( QUERY, powtodb_impl, "float", "powtodb" ); //! linear power to decibel 
    QUERY->add_arg( QUERY, "float", "value" );

    // add rmstodb
    QUERY->add_sfun( QUERY, rmstodb_impl, "float", "rmstodb" ); //! rms to decibel
    QUERY->add_arg( QUERY, "float", "value" );

    // add dbtopow
    QUERY->add_sfun( QUERY, dbtopow_impl, "float", "dbtopow" ); //! decibel to linear
    QUERY->add_arg( QUERY, "float", "value" );

    // add dbtorms
    QUERY->add_sfun( QUERY, dbtorms_impl, "float", "dbtorms" ); //! decibel to rms
    QUERY->add_arg( QUERY, "float", "value" );

    // finish class
    QUERY->end_class( QUERY );

    // seed the rand
    srand( time( NULL ) );

#ifdef AJAY

    Chuck_DL_Func * func = NULL;
    Chuck_Env * env = Chuck_Env::instance();

    // begin class
    // init base class
    if( !type_engine_import_class_begin( env, "VCR", "Object",
                                         env->global(), VCR_ctor ) )
        return FALSE;

    // add member variable
    VCR_offset_data = type_engine_import_mvar( env, "int", "@me", FALSE );
    if( VCR_offset_data == CK_INVALID_OFFSET ) goto error;

    // add load()
    func = make_new_mfun( "int", "load", VCR_load );
    func->add_arg( "string", "filename" );
    func->add_arg( "int", "column" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add reset()
    func = make_new_mfun( "int", "reset", VCR_reset );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add seek()
    func = make_new_mfun( "int", "seek", VCR_seek );
    func->add_arg( "int", "where" );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add more()
    func = make_new_mfun( "int", "more", VCR_more );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add curr()
    func = make_new_mfun( "float", "curr", VCR_curr );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add next()
    func = make_new_mfun( "int", "next", VCR_next );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add pos()
    func = make_new_mfun( "int", "pos", VCR_pos );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add size()
    func = make_new_mfun( "int", "size", VCR_size );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // add name()
    func = make_new_mfun( "string", "name", VCR_name );
    if( !type_engine_import_mfun( env, func ) ) goto error;

    // end the class import
    type_engine_import_class_end( env );

    return TRUE;

error:

    // end the class import
    type_engine_import_class_end( env );
    
    return FALSE;

#else

    return TRUE;

#endif
}

#define RAND_INV_RANGE(r) (RAND_MAX / (r))

int irand_exclusive ( int max ) { 
  int x = ::rand();
  
  while (x >= max * RAND_INV_RANGE (max)) 
    x = ::rand();

  x /= RAND_INV_RANGE (max);
  return x;
}


// abs
CK_DLL_SFUN( abs_impl )
{
  t_CKFLOAT v = GET_CK_FLOAT(ARGS);
  RETURN->v_float = v >= 0.0f ? v : -v;
}

// fabs
CK_DLL_SFUN( fabs_impl )
{
    RETURN->v_float = ::fabs( GET_CK_FLOAT(ARGS) );
}

// rand
CK_DLL_SFUN( rand_impl )
{
    RETURN->v_int = ::rand();
}

// randf
CK_DLL_SFUN( randf_impl )
{
    RETURN->v_float = ( 2.0 * ::rand() / (t_CKFLOAT)RAND_MAX - 1.0 );
}

// randf
CK_DLL_SFUN( rand2f_impl )
{
    t_CKFLOAT min = GET_CK_FLOAT(ARGS), max = *((t_CKFLOAT *)ARGS + 1);
    RETURN->v_float = min + (max-min)*(::rand()/(t_CKFLOAT)RAND_MAX);
}

// randi
CK_DLL_SFUN( rand2_impl ) // inclusive.
{
    int min = *(int *)ARGS, max = *((int *)ARGS + 1);
    int range = max - min; 
    if ( range == 0 ) { 
     RETURN->v_int = min;
    }
    //else if ( range < RAND_MAX / 2 ) { 
    //  RETURN->v_int = ( range > 0 ) ? min + irand_exclusive(1 + range) : max + irand_exclusive ( -range + 1 ) ;
    //}
    else { 
      if ( range > 0 ) { 
    RETURN->v_int = min + (int) ( (1.0 + range) * ( ::rand()/(RAND_MAX+1.0) ) );
      }
      else { 
    RETURN->v_int = min - (int) ( (-range + 1.0) * ( ::rand()/(RAND_MAX+1.0) ) );
      }
    }
}

// sgn
CK_DLL_SFUN( sgn_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = v == 0.0f ? 0.0f : ( v > 0.0f ? 1.0f : -1.0f );
}

// system
CK_DLL_SFUN( system_impl )
{
    const char * cmd = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_int = system( cmd );
}

// aoti
CK_DLL_SFUN( atoi_impl )
{
    const char * v = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_int = atoi( v );
}

// atof
CK_DLL_SFUN( atof_impl )
{
    const char * v = GET_CK_STRING(ARGS)->str.c_str();
    RETURN->v_float = atof( v );
}

// itoa
CK_DLL_SFUN( itoa_impl )
{
    t_CKINT i = GET_CK_INT(ARGS);
    Chuck_String * a = (Chuck_String *)instantiate_and_initialize_object( &t_string, NULL );
    a->str = itoa( i );
    RETURN->v_string = a;
}

// ftoa
CK_DLL_SFUN( ftoa_impl )
{
    t_CKFLOAT f = GET_NEXT_FLOAT(ARGS);
    t_CKINT p = GET_NEXT_INT(ARGS);
    Chuck_String * a = (Chuck_String *)instantiate_and_initialize_object( &t_string, NULL );
    a->str = ftoa( f, (t_CKUINT)p );
    RETURN->v_string = a;
}

// getenv
static Chuck_String g_str; // PROBLEM: not thread friendly
CK_DLL_SFUN( getenv_impl )
{
    const char * v = GET_CK_STRING(ARGS)->str.c_str();
    const char * s = getenv( v );
    Chuck_String * a = (Chuck_String *)instantiate_and_initialize_object( &t_string, NULL );
    a->str = s ? s : "";
    RETURN->v_string = a;
}

// setenv
CK_DLL_SFUN( setenv_impl )
{
    const char * v1 = GET_NEXT_STRING(ARGS)->str.c_str();
    const char * v2 = GET_NEXT_STRING(ARGS)->str.c_str();
    RETURN->v_int = setenv( v1, v2, 1 );
}


// mtof
CK_DLL_SFUN( mtof_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = mtof(v);
}

// ftom
CK_DLL_SFUN( ftom_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = ftom(v);
}

// powtodb
CK_DLL_SFUN( powtodb_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = powtodb(v);
}

// rmstodb
CK_DLL_SFUN( rmstodb_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = rmstodb(v);
}

// dbtopow
CK_DLL_SFUN( dbtopow_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = dbtopow(v);
}

// dbtorms
CK_DLL_SFUN( dbtorms_impl )
{
    t_CKFLOAT v = GET_CK_FLOAT(ARGS);
    RETURN->v_float = dbtorms(v);
}


#ifdef AJAY


class ColumnReader
{
public:
    ColumnReader();
    virtual ~ColumnReader();

    bool init( const string & filename, long col );
    
    bool reset() { if( !fin.good() ) return false; where = 0; return true; }
    bool seek( long pos ) { if( pos < 0 || pos >= values.size() ) return false; where = pos; return true; }
    bool more() { return where < values.size(); }
    double curr();
    bool next() { if( where < values.size() ) where++; return more(); }
    long pos() { return where; }
    long size() { return values.size(); }
    string name() { return n; }

    Chuck_String s;

protected:
    bool get_str( string & out );
    bool get_double( double & out );

protected:
    ifstream fin;
    string n;
    long column;
    long where;
    char line[0x1000];
    long len;

    vector<double> values;
};


ColumnReader::ColumnReader()
{
    n = "[NONE]";
    column = -1;
    where = -1;
    len = 0x1000;
}


ColumnReader::~ColumnReader()
{
    // close file
    if( fin.good() ) 
        fin.close();
}

bool ColumnReader::init( const string & filename, long col )
{
    // hmm
    if( col < 1 )
    {
        cerr << "column must be greater than 0!!!" << endl;
        return false;
    }

    // open file
    fin.open( filename.c_str(), ios::in );
    // yes
    if( !fin.good() )
    {
        cerr << "ColumnReader: cannot open file: '" << filename << "'..." << endl;
        return false;
    }

    // set column
    column = col;

    // read first line
    if( !fin.getline( line, len ) )
    {
        cerr << "ColumnReader: cannot read first line: '" << filename << "'..." << endl;
        return false;
    }

    // get the name
    if( !get_str( n ) )
    {
        cerr << "ColumnReader: cannot seek to column " << col << ": " << filename << "..." << endl;
        return false;
    }

    double v;
    long i = 1;
    // read values
    while( fin.getline( line, len ) )
    {
        v = 0.0;
        // get value
        if( !get_double( v ) )
        {
            cerr << "ColumnReader: cannot read column " << v << " on line i: " << n << "..." << endl;
            return false;
        }

        values.push_back( v );
    }

    // well
    if( values.size() == 0 )
    {
        cerr << "ColumnReader: file doesn't not contain data after first line: " << n << "..." << endl;
        return false;
    }

    // set location
    where = 0;

    s.str = n;

    return true;
}

double ColumnReader::curr()
{
    if( where >= values.size() )
    {
        cerr << "ColumnReader: trying to read beyond end of file: " << n << "..." << endl;
        return 0.0;
    }

    return values[where];
}

bool ColumnReader::get_double( double & out )
{
    assert( column > 0 );
    long c = 1;
    
    char * start = line;
    char * curr = start;

    while( c < column )
    {
        // move past value
        while( *curr && *curr != ',' ) curr++;
        // move past ,
        while( *curr && *curr == ',' ) curr++;
        // check
        if( *curr == '\0' )
        {
            cerr << "ColumnReader: cannot find column " << column << ": " << n << endl;
            return false;
        }

        // increment
        c++;
        // set start
        start = curr;
    }

    // move past value
    while( *curr && *curr != ',' ) curr++;
    // end
    *curr = '\0';

    out = atof( start );
    
    return true;
}


bool ColumnReader::get_str( string & out )
{
    assert( column > 0 );
    long c = 1;
    
    char * start = line;
    char * curr = start;

    while( c < column )
    {
        // move past value
        while( *curr && *curr != ',' ) curr++;
        // move past ,
        while( *curr && *curr == ',' ) curr++;
        // check
        if( *curr == '\0' )
        {
            cerr << "ColumnReader: cannot find column " << column << ": " << n << endl;
            return false;
        }

        // increment
        c++;
        // set start
        start = curr;
    }

    // move past value
    while( *curr && *curr != ',' ) curr++;
    // end
    *curr = '\0';

    out = start;
    
    return true;
}


CK_DLL_CTOR( VCR_ctor )
{
    OBJ_MEMBER_INT(SELF, VCR_offset_data) = (t_CKUINT)new ColumnReader;
}

CK_DLL_MFUN( VCR_load )
{
    ColumnReader * vcr = (ColumnReader*)OBJ_MEMBER_INT(SELF, VCR_offset_data);
    const char * filename = GET_NEXT_STRING(ARGS)->str.c_str();
    t_CKINT column = GET_NEXT_INT(ARGS);
    RETURN->v_int = vcr->init( filename, column );
}

CK_DLL_MFUN( VCR_reset )
{
    ColumnReader * vcr = (ColumnReader*)OBJ_MEMBER_INT(SELF, VCR_offset_data);
    RETURN->v_int = vcr->reset();
}

CK_DLL_MFUN( VCR_seek )
{
    ColumnReader * vcr = (ColumnReader*)OBJ_MEMBER_INT(SELF, VCR_offset_data);
    t_CKINT where = GET_CK_INT(ARGS);
    RETURN->v_int = vcr->seek( where );
}

CK_DLL_MFUN( VCR_more )
{
    ColumnReader * vcr = (ColumnReader*)OBJ_MEMBER_INT(SELF, VCR_offset_data);
    RETURN->v_int = vcr->more();
}

CK_DLL_MFUN( VCR_curr )
{
    ColumnReader * vcr = (ColumnReader*)OBJ_MEMBER_INT(SELF, VCR_offset_data);
    RETURN->v_float = vcr->curr();
}

CK_DLL_MFUN( VCR_next )
{
    ColumnReader * vcr = (ColumnReader*)OBJ_MEMBER_INT(SELF, VCR_offset_data);
    RETURN->v_int = vcr->next();
}

CK_DLL_MFUN( VCR_pos )
{
    ColumnReader * vcr = (ColumnReader*)OBJ_MEMBER_INT(SELF, VCR_offset_data);
    RETURN->v_int = vcr->pos();
}

CK_DLL_MFUN( VCR_size )
{
    ColumnReader * vcr = (ColumnReader*)OBJ_MEMBER_INT(SELF, VCR_offset_data);
    RETURN->v_int = vcr->size();
}

CK_DLL_MFUN( VCR_name )
{
    ColumnReader * vcr = (ColumnReader*)OBJ_MEMBER_INT(SELF, VCR_offset_data);
    RETURN->v_string = &(vcr->s);
}


#endif
