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
// file: ulib_gl.cpp
// desc: ...
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//         Philip L. Davidson (philipd@alumni.princeton.edu)
// date: Spring 2004
//-----------------------------------------------------------------------------
#include "ulib_gl.h"

#include <GL/gl.h>

// lazy phil's preprocessor functions
// if these shouldn't be here, tell me
#define GL_CKADDEXPORT(t, n) QUERY->add_export( QUERY, #t, #n, gl_##n##_impl, TRUE )
#define GL_CKADDPARAM(t, n)  QUERY->add_param ( QUERY, #t, #n )

#define VOID_RETURN_TYPE int
//-----------------------------------------------------------------------------
// name: gl_query()
// desc: query entry point
//-----------------------------------------------------------------------------
//DLL_QUERY gl_query( Chuck_DL_Query * QUERY )
CK_DLL_QUERY
{
    QUERY->set_name( QUERY, "gl" );
    
    // Add Begin
    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Begin );
    GL_CKADDPARAM  ( uint, which );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, BindTexture );
    GL_CKADDPARAM  ( uint, target ); 
    GL_CKADDPARAM  ( uint, texture ); 

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Clear );
    GL_CKADDPARAM( uint, mask );  //do we have a bitfield op?

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, ClearColor );
    GL_CKADDPARAM( float, r ); 
    GL_CKADDPARAM( float, g ); 
    GL_CKADDPARAM( float, b ); 
    GL_CKADDPARAM( float, a ); 

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Color3f );
    GL_CKADDPARAM( float, r ); 
    GL_CKADDPARAM( float, g ); 
    GL_CKADDPARAM( float, b ); 

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Color4f );
    GL_CKADDPARAM( float, r ); 
    GL_CKADDPARAM( float, g ); 
    GL_CKADDPARAM( float, b ); 
    GL_CKADDPARAM( float, a ); 

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Disable );
    GL_CKADDPARAM( uint, cap ); 

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Enable );
    GL_CKADDPARAM( uint, cap ); 

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, End );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Flush );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, FrontFace );
    GL_CKADDPARAM(uint, mode);

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Frustum );
    GL_CKADDPARAM(float, left );
    GL_CKADDPARAM(float, right );
    GL_CKADDPARAM(float, bottom );
    GL_CKADDPARAM(float, top );
    GL_CKADDPARAM(float, znear );
    GL_CKADDPARAM(float, zfar );

    //let's do gl gets... but we need arrays, and then pointers to them!
    //GL_CKADDEXPORT ( VOID_RETURN_TYPE, GetBooleanv );
    GL_CKADDEXPORT( uint, GetError );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Lighti );
    GL_CKADDPARAM ( uint, light );
    GL_CKADDPARAM ( uint, pname );
    GL_CKADDPARAM ( int, param );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Lightf );
    GL_CKADDPARAM ( uint, light );
    GL_CKADDPARAM ( uint, pname );
    GL_CKADDPARAM ( float, param );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, LineWidth );
    GL_CKADDPARAM ( float, width );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, LoadIdentity );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, MatrixMode );
    GL_CKADDPARAM ( uint, mode );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Normal3f );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );
    GL_CKADDPARAM ( float, z );
    
    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Ortho );
    GL_CKADDPARAM(float, left );
    GL_CKADDPARAM(float, right );
    GL_CKADDPARAM(float, bottom );
    GL_CKADDPARAM(float, top );
    GL_CKADDPARAM(float, znear );
    GL_CKADDPARAM(float, zfar );
    

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, PushAttrib );
    GL_CKADDPARAM ( uint, which );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, PopAttrib );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, PushMatrix );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, PopMatrix );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, PolygonMode );
    GL_CKADDPARAM ( uint, face );
    GL_CKADDPARAM ( uint, mode );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Rotatef );
    GL_CKADDPARAM ( float, degrees );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );
    GL_CKADDPARAM ( float, z );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Scalef );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );
    GL_CKADDPARAM ( float, z );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, ShadeModel );
    GL_CKADDPARAM ( uint, mode );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, TexCoord1f );
    GL_CKADDPARAM ( float, u );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, TexCoord2f );
    GL_CKADDPARAM ( float, u );
    GL_CKADDPARAM ( float, v );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, TexImage1D );
    GL_CKADDPARAM ( uint, target );
    GL_CKADDPARAM ( int, level );
    GL_CKADDPARAM ( int, internalformat );
    GL_CKADDPARAM ( uint, width );
    GL_CKADDPARAM ( int, border );
    GL_CKADDPARAM ( uint, format );
    GL_CKADDPARAM ( uint, type );
    GL_CKADDPARAM ( void, pixels ); // void*!!

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, TexImage2D );
    GL_CKADDPARAM ( uint, target );
    GL_CKADDPARAM ( uint, level );
    GL_CKADDPARAM ( uint, internalformat );
    GL_CKADDPARAM ( uint, width );
    GL_CKADDPARAM ( uint, height );
    GL_CKADDPARAM ( uint, border );
    GL_CKADDPARAM ( uint, format );
    GL_CKADDPARAM ( uint, type );
    GL_CKADDPARAM ( void, pixels ); // void*!!

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Translatef );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );
    GL_CKADDPARAM ( float, z );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Vertex3f );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );
    GL_CKADDPARAM ( float, z );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Vertex2f );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );

    GL_CKADDEXPORT ( VOID_RETURN_TYPE, Viewport );
    GL_CKADDPARAM ( int, x );
    GL_CKADDPARAM ( int, y );
    GL_CKADDPARAM ( uint, width );
    GL_CKADDPARAM ( uint, height );
    
    return TRUE;
}


// Begin-->glBegin();
CK_DLL_FUNC( gl_Begin_impl )
{
    t_CKUINT mode = GET_CK_UINT(ARGS);
    glBegin( mode );
}

// BindTexture-->glBindTexture();
CK_DLL_FUNC( gl_BindTexture_impl )
{
    t_CKUINT target  = GET_CK_UINT_N(ARGS,0);
    t_CKUINT texture = GET_CK_UINT_N(ARGS,1);
    glBindTexture( target, texture );
}


// Clear-->glClear()
CK_DLL_FUNC( gl_Clear_impl )
{
    t_CKUINT v = GET_CK_UINT(ARGS);
    glClear( v );
}

// ClearColor-->glClearColor()
CK_DLL_FUNC( gl_ClearColor_impl )
{
    t_CKFLOAT r = GET_CK_FLOAT(ARGS);
    t_CKFLOAT g = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT b = GET_CK_FLOAT_N(ARGS,2);
    t_CKFLOAT a = GET_CK_FLOAT_N(ARGS,3);
    glClearColor( r, g, b, a );
}

// Color3f->glColor3d()
CK_DLL_FUNC( gl_Color3f_impl )
{
    t_CKFLOAT r = GET_CK_FLOAT(ARGS);
    t_CKFLOAT g = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT b = GET_CK_FLOAT_N(ARGS,2);

    glColor3d( r, g, b);
}

// Color4f->glColor4d()
CK_DLL_FUNC( gl_Color4f_impl )
{
    t_CKFLOAT r = GET_CK_FLOAT(ARGS);
    t_CKFLOAT g = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT b = GET_CK_FLOAT_N(ARGS,2);
    t_CKFLOAT a = GET_CK_FLOAT_N(ARGS,3);

    glColor4d( r, g, b, a);
}

// Disable-->glDisable()
CK_DLL_FUNC( gl_Disable_impl )
{
    t_CKUINT v = GET_CK_UINT(ARGS);
    glDisable( v );
}

// Enable-->glEnable
CK_DLL_FUNC( gl_Enable_impl )
{
    t_CKUINT which = GET_CK_UINT(ARGS);
    glEnable( which );
}

// End->glEnd()
CK_DLL_FUNC( gl_End_impl )
{
    glEnd();
}

// etc....
CK_DLL_FUNC( gl_Flush_impl )
{
    glFlush();
}

CK_DLL_FUNC( gl_FrontFace_impl )
{
    t_CKUINT mode = GET_CK_UINT(ARGS);
    glFrontFace(mode);
}

CK_DLL_FUNC( gl_Frustum_impl )
{
    t_CKFLOAT left = GET_CK_FLOAT_N(ARGS,0);
    t_CKFLOAT right = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT bottom = GET_CK_FLOAT_N(ARGS,2);
    t_CKFLOAT top = GET_CK_FLOAT_N(ARGS,3);
    t_CKFLOAT znear = GET_CK_FLOAT_N(ARGS,4);
    t_CKFLOAT zfar = GET_CK_FLOAT_N(ARGS,5);
    glFrustum(left,right,bottom,top,znear,zfar);
}

CK_DLL_FUNC( gl_GetError_impl )
{
    RETURN->v_uint = (uint) glGetError();
}
// 
CK_DLL_FUNC( gl_Lighti_impl )
{
    t_CKUINT light = GET_CK_UINT_N(ARGS,0);
    t_CKUINT pname = GET_CK_UINT_N(ARGS,1);
    int      param =  GET_CK_INT_N(ARGS,2);
    glLighti(light, pname, param);
}

//
CK_DLL_FUNC( gl_Lightf_impl )
{
    t_CKUINT light = GET_CK_UINT_N(ARGS,0);
    t_CKUINT pname = GET_CK_UINT_N(ARGS,1);
    t_CKFLOAT param = GET_CK_FLOAT_N(ARGS,2);
    glLightf(light, pname, param);
}

CK_DLL_FUNC( gl_LineWidth_impl ) {  
    t_CKFLOAT width = GET_CK_FLOAT(ARGS);
    glLineWidth(width);
}

CK_DLL_FUNC( gl_LoadIdentity_impl ) {   
    glLoadIdentity();
}

CK_DLL_FUNC( gl_MatrixMode_impl ) {   
    t_CKUINT mode = GET_CK_UINT(ARGS);
    glMatrixMode(mode);
}

// Normal3f --> glNormal3d
CK_DLL_FUNC( gl_Normal3f_impl ) {   
    t_CKFLOAT x = GET_CK_FLOAT_N(ARGS,0);
    t_CKFLOAT y = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT z = GET_CK_FLOAT_N(ARGS,2);
    glNormal3d(x,y,z);
}

CK_DLL_FUNC( gl_Ortho_impl )
{
    t_CKFLOAT left = GET_CK_FLOAT_N(ARGS,0);
    t_CKFLOAT right = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT bottom = GET_CK_FLOAT_N(ARGS,2);
    t_CKFLOAT top = GET_CK_FLOAT_N(ARGS,3);
    t_CKFLOAT znear = GET_CK_FLOAT_N(ARGS,4);
    t_CKFLOAT zfar = GET_CK_FLOAT_N(ARGS,5);
    glOrtho(left,right,bottom,top,znear,zfar);
}

CK_DLL_FUNC( gl_PushAttrib_impl ) {   
  t_CKUINT mask = GET_CK_UINT(ARGS);
    glPushAttrib(mask);
}

CK_DLL_FUNC( gl_PopAttrib_impl ) {   
    glPopAttrib();
}

CK_DLL_FUNC( gl_PushMatrix_impl ) {   
    glPushMatrix();
}

CK_DLL_FUNC( gl_PopMatrix_impl ) {   
    glPopMatrix();
}

CK_DLL_FUNC( gl_PolygonMode_impl ) {   
    t_CKUINT face = GET_CK_UINT(ARGS);
    t_CKUINT mode = GET_CK_UINT_N(ARGS,1);
    glPolygonMode(face, mode);
}

// Rotatef --> glRotated()
CK_DLL_FUNC( gl_Rotatef_impl ) {   
    t_CKFLOAT degrees = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT x = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT y = GET_CK_FLOAT_N(ARGS,2);
    t_CKFLOAT z = GET_CK_FLOAT_N(ARGS,3);
    glRotated(degrees, x, y, z);
}

// Scalef --> glScaled();
CK_DLL_FUNC( gl_Scalef_impl ) {   
    t_CKFLOAT x = GET_CK_FLOAT_N(ARGS,0);
    t_CKFLOAT y = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT z = GET_CK_FLOAT_N(ARGS,2);
    glScaled(x,y,z);
}

CK_DLL_FUNC( gl_ShadeModel_impl ) {   
    t_CKUINT mode = GET_CK_UINT(ARGS);
    glShadeModel(mode);
}

// TexCoord1f --> glTexCoord1d
CK_DLL_FUNC( gl_TexCoord1f_impl ) {   
    t_CKFLOAT u = GET_CK_FLOAT_N(ARGS,0);
    glTexCoord1d(u);
}

// TexCoord2f --> glTexCoord2d
CK_DLL_FUNC( gl_TexCoord2f_impl ) {   
    t_CKFLOAT u = GET_CK_FLOAT_N(ARGS,0);
    t_CKFLOAT v = GET_CK_FLOAT_N(ARGS,1);
    glTexCoord2d(u,v);
}

CK_DLL_FUNC( gl_TexImage1D_impl ) {   

    t_CKUINT target = GET_CK_UINT_N(ARGS,0);
    int  level  = GET_CK_INT_N(ARGS,1);
    int  internalformat  = GET_CK_INT_N(ARGS,2);
    t_CKUINT width = GET_CK_UINT_N(ARGS,3);
    int  border = GET_CK_INT_N(ARGS,4);
    t_CKUINT format = GET_CK_UINT_N(ARGS,5);
    t_CKUINT type = GET_CK_UINT_N(ARGS,6);
    t_CKUINT pixu = GET_CK_UINT_N(ARGS,7);
    void* pixels = (void*)pixu;

    glTexImage1D(target, level, internalformat, width, border, format, type, pixels);
}

CK_DLL_FUNC( gl_TexImage2D_impl ) {   

    t_CKUINT target = GET_CK_UINT_N(ARGS,0);
    int  level  = GET_CK_INT_N(ARGS,1);
    int  internalformat  = GET_CK_INT_N(ARGS,2);
    t_CKUINT width = GET_CK_UINT_N(ARGS,3);
    t_CKUINT height = GET_CK_UINT_N(ARGS,4);
    int  border = GET_CK_INT_N(ARGS,5);
    t_CKUINT format = GET_CK_UINT_N(ARGS,6);
    t_CKUINT type = GET_CK_UINT_N(ARGS,7);
    t_CKUINT pixu = GET_CK_UINT_N(ARGS,8);
    void* pixels = (void*)pixu;

    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);

}

// Translatef --> glTranslated
CK_DLL_FUNC( gl_Translatef_impl ) {   
    t_CKFLOAT x = GET_CK_FLOAT_N(ARGS,0);
    t_CKFLOAT y = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT z = GET_CK_FLOAT_N(ARGS,2);
    glTranslated(x,y,z);
}

CK_DLL_FUNC( gl_Vertex2f_impl ) {   
    t_CKFLOAT x = GET_CK_FLOAT_N(ARGS,0);
    t_CKFLOAT y = GET_CK_FLOAT_N(ARGS,1);
    glVertex2d(x,y);
}

// Vertex3f --> glVertex3d()
CK_DLL_FUNC( gl_Vertex3f_impl ) {   
    t_CKFLOAT x = GET_CK_FLOAT_N(ARGS,0);
    t_CKFLOAT y = GET_CK_FLOAT_N(ARGS,1);
    t_CKFLOAT z = GET_CK_FLOAT_N(ARGS,2);
    glVertex3d(x,y,z);
}

// 
CK_DLL_FUNC( gl_Viewport_impl ) {   
    int  x = GET_CK_INT_N(ARGS,0);
    int  y = GET_CK_INT_N(ARGS,1);
    t_CKUINT width = GET_CK_UINT_N(ARGS,2);
    t_CKUINT height = GET_CK_UINT_N(ARGS,3);
    glViewport(x,y,width,height);
}










