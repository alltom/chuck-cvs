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

//-----------------------------------------------------------------------------
// name: gl_query()
// desc: query entry point
//-----------------------------------------------------------------------------
//DLL_QUERY gl_query( Chuck_DL_Query * QUERY )
CK_DLL_QUERY
{
    QUERY->set_name( QUERY, "gl" );
    
    // Add Begin
    GL_CKADDEXPORT ( int, Begin );
    GL_CKADDPARAM  ( uint, which );

    GL_CKADDEXPORT ( int, BindTexture );
    GL_CKADDPARAM  ( uint, target ); 
    GL_CKADDPARAM  ( uint, texture ); 

    GL_CKADDEXPORT ( int, Clear );
    GL_CKADDPARAM( uint, mask );  //do we have a bitfield op?

    GL_CKADDEXPORT ( int, ClearColor );
    GL_CKADDPARAM( float, r ); 
    GL_CKADDPARAM( float, g ); 
    GL_CKADDPARAM( float, b ); 
    GL_CKADDPARAM( float, a ); 

    GL_CKADDEXPORT ( int, Color3f );
    GL_CKADDPARAM( float, r ); 
    GL_CKADDPARAM( float, g ); 
    GL_CKADDPARAM( float, b ); 

    GL_CKADDEXPORT ( int, Color4f );
    GL_CKADDPARAM( float, r ); 
    GL_CKADDPARAM( float, g ); 
    GL_CKADDPARAM( float, b ); 
    GL_CKADDPARAM( float, a ); 

    GL_CKADDEXPORT ( int, Disable );
    GL_CKADDPARAM( uint, cap ); 

    GL_CKADDEXPORT ( int, Enable );
    GL_CKADDPARAM( uint, cap ); 

    GL_CKADDEXPORT ( int, End );

    GL_CKADDEXPORT ( int, Flush );

    GL_CKADDEXPORT ( int, FrontFace );
    GL_CKADDPARAM(uint, mode);

    GL_CKADDEXPORT ( int, Frustum );
    GL_CKADDPARAM(float, left );
    GL_CKADDPARAM(float, right );
    GL_CKADDPARAM(float, bottom );
    GL_CKADDPARAM(float, top );
    GL_CKADDPARAM(float, znear );
    GL_CKADDPARAM(float, zfar );

    //let's do gl gets... but we need arrays, and then pointers to them!
    //GL_CKADDEXPORT ( int, GetBooleanv );
    GL_CKADDEXPORT( uint, GetError );

    GL_CKADDEXPORT ( int, Lighti );
    GL_CKADDPARAM ( uint, light );
    GL_CKADDPARAM ( uint, pname );
    GL_CKADDPARAM ( int, param );

    GL_CKADDEXPORT ( int, Lightf );
    GL_CKADDPARAM ( uint, light );
    GL_CKADDPARAM ( uint, pname );
    GL_CKADDPARAM ( float, param );

    GL_CKADDEXPORT ( int, LineWidth );
    GL_CKADDPARAM ( float, width );

    GL_CKADDEXPORT ( int, LoadIdentity );

    GL_CKADDEXPORT ( int, MatrixMode );
    GL_CKADDPARAM ( uint, mode );

    GL_CKADDEXPORT ( int, Normal3f );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );
    GL_CKADDPARAM ( float, z );
    
    GL_CKADDEXPORT ( int, Ortho );
    GL_CKADDPARAM(float, left );
    GL_CKADDPARAM(float, right );
    GL_CKADDPARAM(float, bottom );
    GL_CKADDPARAM(float, top );
    GL_CKADDPARAM(float, znear );
    GL_CKADDPARAM(float, zfar );
    

    GL_CKADDEXPORT ( int, PushAttrib );
    GL_CKADDPARAM ( uint, which );

    GL_CKADDEXPORT ( int, PopAttrib );

    GL_CKADDEXPORT ( int, PushMatrix );

    GL_CKADDEXPORT ( int, PopMatrix );

    GL_CKADDEXPORT ( int, PolygonMode );
    GL_CKADDPARAM ( uint, face );
    GL_CKADDPARAM ( uint, mode );

    GL_CKADDEXPORT ( int, Rotatef );
    GL_CKADDPARAM ( float, degrees );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );
    GL_CKADDPARAM ( float, z );

    GL_CKADDEXPORT ( int, Scalef );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );
    GL_CKADDPARAM ( float, z );

    GL_CKADDEXPORT ( int, ShadeModel );
    GL_CKADDPARAM ( uint, mode );

    GL_CKADDEXPORT ( int, TexCoord1f );
    GL_CKADDPARAM ( float, u );

    GL_CKADDEXPORT ( int, TexCoord2f );
    GL_CKADDPARAM ( float, u );
    GL_CKADDPARAM ( float, v );

    GL_CKADDEXPORT ( int, TexImage1D );
    GL_CKADDPARAM ( uint, target );
    GL_CKADDPARAM ( int, level );
    GL_CKADDPARAM ( int, internalformat );
    GL_CKADDPARAM ( uint, width );
    GL_CKADDPARAM ( int, border );
    GL_CKADDPARAM ( uint, format );
    GL_CKADDPARAM ( uint, type );
    GL_CKADDPARAM ( void, pixels ); // void*!!

    GL_CKADDEXPORT ( int, TexImage2D );
    GL_CKADDPARAM ( uint, target );
    GL_CKADDPARAM ( uint, level );
    GL_CKADDPARAM ( uint, internalformat );
    GL_CKADDPARAM ( uint, width );
    GL_CKADDPARAM ( uint, height );
    GL_CKADDPARAM ( uint, border );
    GL_CKADDPARAM ( uint, format );
    GL_CKADDPARAM ( uint, type );
    GL_CKADDPARAM ( void, pixels ); // void*!!

    GL_CKADDEXPORT ( int, Translatef );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );
    GL_CKADDPARAM ( float, z );

    GL_CKADDEXPORT ( int, Vertex3f );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );
    GL_CKADDPARAM ( float, z );

    GL_CKADDEXPORT ( int, Vertex2f );
    GL_CKADDPARAM ( float, x );
    GL_CKADDPARAM ( float, y );

    GL_CKADDEXPORT ( int, Viewport );
    GL_CKADDPARAM ( int, x );
    GL_CKADDPARAM ( int, y );
    GL_CKADDPARAM ( uint, width );
    GL_CKADDPARAM ( uint, height );
    
    return TRUE;
}


// Begin-->glBegin();
CK_DLL_FUNC( gl_Begin_impl )
{
    t_CKUINT mode = pull_ckUINT(ARGS);
    glBegin( mode );
}

// BindTexture-->glBindTexture();
CK_DLL_FUNC( gl_BindTexture_impl )
{
    t_CKUINT target  = pull_ckUINT(ARGS);
    t_CKUINT texture = pull_ckUINT(ARGS);
    glBindTexture( target, texture );
}


// Clear-->glClear()
CK_DLL_FUNC( gl_Clear_impl )
{
    t_CKUINT v = pull_ckUINT(ARGS);
    glClear( v );
}

// ClearColor-->glClearColor()
CK_DLL_FUNC( gl_ClearColor_impl )
{
    t_CKFLOAT r = pull_ckFLOAT(ARGS);
    t_CKFLOAT g = pull_ckFLOAT(ARGS);
    t_CKFLOAT b = pull_ckFLOAT(ARGS);
    t_CKFLOAT a = pull_ckFLOAT(ARGS);
    glClearColor( r, g, b, a );
}

// Color3f->glColor3d()
CK_DLL_FUNC( gl_Color3f_impl )
{
    t_CKFLOAT r = pull_ckFLOAT(ARGS);
    t_CKFLOAT g = pull_ckFLOAT(ARGS);
    t_CKFLOAT b = pull_ckFLOAT(ARGS);

    glColor3d( r, g, b);
}

// Color4f->glColor4d()
CK_DLL_FUNC( gl_Color4f_impl )
{
    t_CKFLOAT r = pull_ckFLOAT(ARGS);
    t_CKFLOAT g = pull_ckFLOAT(ARGS);
    t_CKFLOAT b = pull_ckFLOAT(ARGS);
    t_CKFLOAT a = pull_ckFLOAT(ARGS);

    glColor4d( r, g, b, a);
}

// Disable-->glDisable()
CK_DLL_FUNC( gl_Disable_impl )
{
    t_CKUINT v = pull_ckUINT(ARGS);
    glDisable( v );
}

// Enable-->glEnable
CK_DLL_FUNC( gl_Enable_impl )
{
    t_CKUINT which = pull_ckUINT(ARGS);
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
    t_CKUINT mode = pull_ckUINT(ARGS);
    glFrontFace(mode);
}

CK_DLL_FUNC( gl_Frustum_impl )
{
    t_CKFLOAT left = pull_ckFLOAT(ARGS);
    t_CKFLOAT right = pull_ckFLOAT(ARGS);
    t_CKFLOAT bottom = pull_ckFLOAT(ARGS);
    t_CKFLOAT top = pull_ckFLOAT(ARGS);
    t_CKFLOAT znear = pull_ckFLOAT(ARGS);
    t_CKFLOAT zfar = pull_ckFLOAT(ARGS);
    glFrustum(left,right,bottom,top,znear,zfar);
}

CK_DLL_FUNC( gl_GetError_impl )
{
    RETURN->v_uint = (uint) glGetError();
}
// 
CK_DLL_FUNC( gl_Lighti_impl )
{
    t_CKUINT light = pull_ckUINT(ARGS);
    t_CKUINT pname = pull_ckUINT(ARGS);
    int      param =  pull_ckINT(ARGS);
    glLighti(light, pname, param);
}

//
CK_DLL_FUNC( gl_Lightf_impl )
{
    t_CKUINT light = pull_ckUINT(ARGS);
    t_CKUINT pname = pull_ckUINT(ARGS);
    t_CKFLOAT param = pull_ckFLOAT(ARGS);
    glLightf(light, pname, param);
}

CK_DLL_FUNC( gl_LineWidth_impl ) {  
    t_CKFLOAT width = pull_ckFLOAT(ARGS);
    glLineWidth(width);
}

CK_DLL_FUNC( gl_LoadIdentity_impl ) {   
    glLoadIdentity();
}

CK_DLL_FUNC( gl_MatrixMode_impl ) {   
    t_CKUINT mode = pull_ckUINT(ARGS);
    glMatrixMode(mode);
}

// Normal3f --> glNormal3d
CK_DLL_FUNC( gl_Normal3f_impl ) {   
    t_CKFLOAT x = pull_ckFLOAT(ARGS);
    t_CKFLOAT y = pull_ckFLOAT(ARGS);
    t_CKFLOAT z = pull_ckFLOAT(ARGS);
    glNormal3d(x,y,z);
}

CK_DLL_FUNC( gl_Ortho_impl )
{
    t_CKFLOAT left = pull_ckFLOAT(ARGS);
    t_CKFLOAT right = pull_ckFLOAT(ARGS);
    t_CKFLOAT bottom = pull_ckFLOAT(ARGS);
    t_CKFLOAT top = pull_ckFLOAT(ARGS);
    t_CKFLOAT znear = pull_ckFLOAT(ARGS);
    t_CKFLOAT zfar = pull_ckFLOAT(ARGS);
    glOrtho(left,right,bottom,top,znear,zfar);
}

CK_DLL_FUNC( gl_PushAttrib_impl ) {   
  t_CKUINT mask = pull_ckUINT(ARGS);
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
    t_CKUINT face = pull_ckUINT(ARGS);
    t_CKUINT mode = pull_ckUINT(ARGS);
    glPolygonMode(face, mode);
}

// Rotatef --> glRotated()
CK_DLL_FUNC( gl_Rotatef_impl ) {   
    t_CKFLOAT degrees = pull_ckFLOAT(ARGS);
    t_CKFLOAT x = pull_ckFLOAT(ARGS);
    t_CKFLOAT y = pull_ckFLOAT(ARGS);
    t_CKFLOAT z = pull_ckFLOAT(ARGS);
    glRotated(degrees, x, y, z);
}

// Scalef --> glScaled();
CK_DLL_FUNC( gl_Scalef_impl ) {   
    t_CKFLOAT x = pull_ckFLOAT(ARGS);
    t_CKFLOAT y = pull_ckFLOAT(ARGS);
    t_CKFLOAT z = pull_ckFLOAT(ARGS);
    glScaled(x,y,z);
}

CK_DLL_FUNC( gl_ShadeModel_impl ) {   
    t_CKUINT mode = pull_ckUINT(ARGS);
    glShadeModel(mode);
}

// TexCoord1f --> glTexCoord1d
CK_DLL_FUNC( gl_TexCoord1f_impl ) {   
    t_CKFLOAT u = pull_ckFLOAT(ARGS);
    glTexCoord1d(u);
}

// TexCoord2f --> glTexCoord2d
CK_DLL_FUNC( gl_TexCoord2f_impl ) {   
    t_CKFLOAT u = pull_ckFLOAT(ARGS);
    t_CKFLOAT v = pull_ckFLOAT(ARGS);
    glTexCoord2d(u,v);
}

CK_DLL_FUNC( gl_TexImage1D_impl ) {   

    t_CKUINT target = pull_ckUINT(ARGS);
    int  level  = pull_ckINT(ARGS);
    int  internalformat  = pull_ckINT(ARGS);
    t_CKUINT width = pull_ckUINT(ARGS);
    int  border = pull_ckINT(ARGS);
    t_CKUINT format = pull_ckUINT(ARGS);
    t_CKUINT type = pull_ckUINT(ARGS);
    t_CKUINT pixu = pull_ckUINT(ARGS);
    void* pixels = (void*)pixu;

    glTexImage1D(target, level, internalformat, width, border, format, type, pixels);
}

CK_DLL_FUNC( gl_TexImage2D_impl ) {   

    t_CKUINT target = pull_ckUINT(ARGS);
    int  level  = pull_ckINT(ARGS);
    int  internalformat  = pull_ckINT(ARGS);
    t_CKUINT width = pull_ckUINT(ARGS);
    t_CKUINT height = pull_ckUINT(ARGS);
    int  border = pull_ckINT(ARGS);
    t_CKUINT format = pull_ckUINT(ARGS);
    t_CKUINT type = pull_ckUINT(ARGS);
    t_CKUINT pixu = pull_ckUINT(ARGS);
    void* pixels = (void*)pixu;

    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);

}

// Translatef --> glTranslated
CK_DLL_FUNC( gl_Translatef_impl ) {   
    t_CKFLOAT x = pull_ckFLOAT(ARGS);
    t_CKFLOAT y = pull_ckFLOAT(ARGS);
    t_CKFLOAT z = pull_ckFLOAT(ARGS);
    glTranslated(x,y,z);
}

CK_DLL_FUNC( gl_Vertex2f_impl ) {   
    t_CKFLOAT x = pull_ckFLOAT(ARGS);
    t_CKFLOAT y = pull_ckFLOAT(ARGS);
    glVertex2d(x,y);
}

// Vertex3f --> glVertex3d()
CK_DLL_FUNC( gl_Vertex3f_impl ) {   
    t_CKFLOAT x = pull_ckFLOAT(ARGS);
    t_CKFLOAT y = pull_ckFLOAT(ARGS);
    t_CKFLOAT z = pull_ckFLOAT(ARGS);
    glVertex3d(x,y,z);
}

// 
CK_DLL_FUNC( gl_Viewport_impl ) {   
    int  x = pull_ckINT(ARGS);
    int  y = pull_ckINT(ARGS);
    t_CKUINT width = pull_ckUINT(ARGS);
    t_CKUINT height = pull_ckUINT(ARGS);
    glViewport(x,y,width,height);
}










