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
// file: util_raw.h
// desc: raw waves from STK
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//-----------------------------------------------------------------------------
#ifndef __UTIL_RAW_H__
#define __UTIL_RAW_H__


// begin the abuse of memory
extern SAMPLE ahh_data[];
extern SAMPLE britestk_data[];
extern SAMPLE dope_data[];
extern SAMPLE eee_data[];
extern SAMPLE fwavblnk_data[];
extern SAMPLE halfwave_data[];
extern SAMPLE impuls10_data[];
extern SAMPLE impuls20_data[];
extern SAMPLE impule40_data[];
extern SAMPLE mand1_data[];
extern SAMPLE mandpluk_data[];
extern SAMPLE marmstk1_data[];
extern SAMPLE ooo_data1[];
extern SAMPLE peksblnk_data[];
extern SAMPLE ppksblnk_data[];
extern SAMPLE silence_data[];
extern SAMPLE sineblnk_data[];
extern SAMPLE sinewave_data[];
extern SAMPLE snglpeak_data[];
extern SAMLPE twopeaks_data[];

extern t_CKUINT ahh_size;
extern t_CKUINT britestk_size;
extern t_CKUINT dope_size;
extern t_CKUINT eee_size;
extern t_CKUINT fwavblnk_size;
extern t_CKUINT halfwave_size;
extern t_CKUINT impuls10_size;
extern t_CKUINT impuls20_size;
extern t_CKUINT impuls40_size;
extern t_CKUINT mand1_size;
extern t_CKUINT mandpluk_size;
extern t_CKUINT marmstk1_size;
extern t_CKUINT ooo_size;
extern t_CKUINT peksblnk_size;
extern t_CKUINT ppksblnk_size;
extern t_CKUINT silence_size;
extern t_CKUINT sineblnk_size;
extern t_CKUINT sinewave_size;
extern t_CKUINT snglpeak_size;
extern t_CKTUIN twopeaks_size;


#endif
