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
// name: digitalio_rtaudio.h
// desc: digitalio over RtAudio (from Gary Scavone)
//
// author: Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
//-----------------------------------------------------------------------------
#ifndef __DIGITAL_IO_H__
#define __DIGITAL_IO_H__




//-----------------------------------------------------------------------------
// define and forward references
//-----------------------------------------------------------------------------
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

// defaults
#define BUFFER_SIZE_OUT_DEFAULT      512
#define BUFFER_SIZE_IN_DEFAULT       512
#define BUFFER_SIZE_DEFAULT          512
#define NUM_OUT_BUFFERS_DEFAULT      4
#define NUM_IN_BUFFERS_DEFAULT       4
#define NUM_BUFFERS_DEFAULT          4
#define NUM_CHANNELS_DEFAULT         2       // number of channels
  #if defined(__LINUX_ALSA__ ) || defined(__LINUX_OSS__)
#define SAMPLING_RATE_DEFAULT        48000   // sampling rate
#define USE_CB_DEFAULT               TRUE    // callback
  #else
#define SAMPLING_RATE_DEFAULT        44100   // sampling rate
#define USE_CB_DEFAULT               TRUE    // callback
  #endif
#define BITS_PER_SAMPLE_DEFAULT      16      // sample size
#define DEVICE_NUM_OUT_DEFAULT       0
#define DEVICE_NUM_IN_DEFAULT        0

// sample types
#define SAMPLE_SHORT         short
#define SAMPLE_INTERNAL      SAMPLE_SHORT
#ifndef SAMPLE
#define SAMPLE_FLOAT         float
#define SAMPLE               SAMPLE_FLOAT
#endif
#define S_MAX                0x7fff  // max value for 16 bit
#define S_MIN                -0x7fff // min value for 16 bit

// external to internal sample conversion
#define SAMPLE_TO_INTERNAL(s) ( (SAMPLE_INTERNAL)((SAMPLE)S_MAX * s) )
#define INTERNAL_TO_SAMPLE(s) ( (SAMPLE)((SAMPLE)s / S_MAX) )

// types
#define DWORD__                unsigned int
#define UINT__                 DWORD__
#define BOOL__                 DWORD__
#define FLOAT__                float
#define BYTE__                 unsigned char


#ifndef FALSE
  #define FALSE                0
#endif
#ifndef TRUE
  #define TRUE                 1
#endif

class RtAudio;




//-----------------------------------------------------------------------------
// name: Digitalio
// desc: ...
//-----------------------------------------------------------------------------
class Digitalio
{
public:
    static BOOL__ initialize( DWORD__ num_channels = NUM_CHANNELS_DEFAULT,
                              DWORD__ sampling_rate = SAMPLING_RATE_DEFAULT,
                              DWORD__ bps = BITS_PER_SAMPLE_DEFAULT,
                              DWORD__ buffer_size = BUFFER_SIZE_DEFAULT,
                              DWORD__ num_buffers = NUM_BUFFERS_DEFAULT );
    static void shutdown();

public:
    static DWORD__ sampling_rate( ) { return m_sampling_rate; }
    static DWORD__ num_channels_out( ) { return m_num_channels_out; }
    static DWORD__ num_channels_in( ) { return m_num_channels_in; }
    static DWORD__ bps( ) { return m_bps; }
    static DWORD__ buffer_size( ) { return m_buffer_size; }
    static DWORD__ num_buffers( ) { return m_num_buffers; }
    static RtAudio * audio( ) { return m_rtaudio; }
    static BOOL__  start( );
    static BOOL__  stop( );
    static BOOL__  tick( );
    static int cb( char * buffer, int buffer_size, void * user_data );

public: // data
    static BOOL__ m_init;
    static DWORD__ m_start;
    static DWORD__ m_tick_count;
    static DWORD__ m_num_channels_out;
    static DWORD__ m_num_channels_in;
    static DWORD__ m_sampling_rate;
    static DWORD__ m_bps;
    static DWORD__ m_buffer_size;
    static DWORD__ m_num_buffers;
    static RtAudio * m_rtaudio;
    static SAMPLE * m_buffer_out;
    static SAMPLE * m_buffer_in;
    static SAMPLE ** m_write_ptr;
    static SAMPLE ** m_read_ptr;
    static BOOL__ m_out_ready;
    static BOOL__ m_in_ready;
    static BOOL__ m_use_cb;
    static BOOL__ m_go;
    
    static DWORD__ m_dac_n;
    static DWORD__ m_adc_n;
};




//-----------------------------------------------------------------------------
// name: TickOut
// desc: ...
//-----------------------------------------------------------------------------
class TickOut
{
public:
    virtual ~TickOut() {}

public:
    virtual BOOL__ reset() { return true; }
    virtual BOOL__ tick_out( SAMPLE s ) = 0;
    virtual BOOL__ tick_out( SAMPLE l, SAMPLE r ) = 0;
    virtual BOOL__ tick_out( const SAMPLE * out, DWORD__ n ) = 0;
};




//-----------------------------------------------------------------------------
// name: TickIn
// desc: ...
//-----------------------------------------------------------------------------
class TickIn
{
public:
    virtual ~TickIn() { }

public:
    virtual BOOL__ reset() { return true; }
    
    virtual BOOL__ tick_in( SAMPLE * in ) = 0;
    virtual BOOL__ tick_in( SAMPLE * l, SAMPLE * r ) = 0;
    virtual BOOL__ tick_in( SAMPLE * in, DWORD__ n ) = 0;

    virtual SAMPLE tick( )
    { SAMPLE in; return ( tick_in( &in ) ? in : (SAMPLE)0.0f ); }
};




//-----------------------------------------------------------------------------
// name: class DigitalOut
// desc: ...
//-----------------------------------------------------------------------------
class DigitalOut : public TickOut
{
public:
    DigitalOut();
    ~DigitalOut();

public:
    BOOL__ initialize();
    void cleanup();

    BOOL__ start();
    BOOL__ stop();

public:
    virtual BOOL__ tick_out( SAMPLE s );
    virtual BOOL__ tick_out( SAMPLE l, SAMPLE r );
    virtual BOOL__ tick_out( const SAMPLE * samples, DWORD__ n );

public:
    DWORD__ render();

protected:
    SAMPLE * m_data_ptr_out;
    SAMPLE * m_data_max_out;

    inline BOOL__ prepare_tick_out();
};




//-----------------------------------------------------------------------------
// name: class DigitalIn
// desc: ...
//-----------------------------------------------------------------------------
class DigitalIn : public TickIn
{
public:
    DigitalIn();
    ~DigitalIn();

public: // in
    BOOL__ initialize();
    void cleanup();

    BOOL__ start();
    BOOL__ stop();

public:
    virtual BOOL__ tick_in( SAMPLE * s );
    virtual BOOL__ tick_in( SAMPLE * l, SAMPLE * r );
    virtual BOOL__ tick_in( SAMPLE * samples, DWORD__ n );

public:
    DWORD__ capture( );

protected:
    SAMPLE * m_data_ptr_in;
    SAMPLE * m_data_max_in;

    inline BOOL__ prepare_tick_in();
};




//-----------------------------------------------------------------------------
// name: AudioBufferX
// desc: ...
//-----------------------------------------------------------------------------
class AudioBufferX
{
public:
    AudioBufferX( DWORD__ size = 0 );

public:
    BOOL__ initialize( DWORD__ size );
    void cleanup();
    SAMPLE * data();
    DWORD__ size();

protected:
    SAMPLE * m_data;
    DWORD__ m_size;

    SAMPLE * m_ptr_curr;
    SAMPLE * m_ptr_end;
};




//-----------------------------------------------------------------------------
// name: AudioBufferIn
// desc: ...
//-----------------------------------------------------------------------------
class AudioBufferIn : public TickIn, public AudioBufferX
{
public:
    AudioBufferIn( DWORD__ size = 0 );
    virtual ~AudioBufferIn();

public:
    virtual BOOL__ reset();
    virtual BOOL__ tick_in( SAMPLE * in );
    virtual BOOL__ tick_in( SAMPLE * l, SAMPLE * r );
    virtual BOOL__ tick_in( SAMPLE * in, DWORD__ n );
};




//-----------------------------------------------------------------------------
// name: AudioBufferOut
// desc: ...
//-----------------------------------------------------------------------------
class AudioBufferOut : public TickOut, public AudioBufferX
{
public:
    AudioBufferOut( DWORD__ size = 0 );
    virtual ~AudioBufferOut();

public:
    virtual BOOL__ reset();
    virtual BOOL__ tick_out( SAMPLE s );
    virtual BOOL__ tick_out( SAMPLE l, SAMPLE r );
    virtual BOOL__ tick_out( const SAMPLE * out, DWORD__ n );
};




#endif

