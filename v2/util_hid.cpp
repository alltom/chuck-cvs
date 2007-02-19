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
// file: util_hid.cpp
// desc: refactored HID joystick, keyboard, mouse support
//
// author: Spencer Salazar (ssalazar@cs.princeton.edu)
// date: summer 2006
//-----------------------------------------------------------------------------

#include "chuck_def.h"
#include "chuck_errmsg.h"
#include "util_hid.h"
#include "hidio_sdl.h"

#include <vector>
#include <map>

using namespace std;

#ifdef __PLATFORM_MACOSX__
#pragma mark OS X General HID support

/* TODO: ***********************************************************************

Make Joystick/Mouse/Keyboard_open/_close thread safe (i.e., do all ->configure 
and ->startQueue calls on the Hid thread).  *** DONE

Make ->disconnect delete the element list *** worked around

Make Keyboard, Mouse dis/reattachment work *** DONE

*******************************************************************************/


#include <mach/mach.h>
#include <mach/mach_error.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <Kernel/IOKit/hidsystem/IOHIDUsageTables.h>
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <IOKit/IOMessage.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>

// check for OS X 10.4/CGEvent
#include <AvailabilityMacros.h>
#undef MAC_OS_X_VERSION_MIN_REQUIRED
#undef MAC_OS_X_VERSION_MAX_ALLOWED
#define MAC_OS_X_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_10_3
#define MAC_OS_X_VERSION_MAX_ALLOWED MAC_OS_X_VERSION_10_4
#include <ApplicationServices/ApplicationServices.h>
#ifdef __CGEVENT_H__
#define __CK_HID_CURSOR_TRACK__
#endif

#ifndef __DAN__
#undef __CK_HID_CURSOR_TRACK__
#endif

#ifdef __CK_HID_WIIREMOTE__
#include <IOBluetooth/IOBluetoothUserLib.h>
#include "util_buffers.h"
#endif // __CK_HID_WIIREMOTE__

class lockable
{
public:
    void lock()
    {
        m_mutex.acquire();
    }

    void unlock()
    {
        m_mutex.release();
    }

private:
    XMutex m_mutex;
};


template< typename _Tp, typename _Alloc = allocator< _Tp > >
class xvector : public vector< _Tp, _Alloc >, public lockable
{
};

template <typename _Key, typename _Tp, typename _Compare = less<_Key>,
typename _Alloc = allocator<pair<const _Key, _Tp> > >
class xmultimap : public multimap< _Key, _Tp, _Compare, _Alloc >, public lockable
{
};

class OSX_Device : public lockable
{
public:
    OSX_Device()
    {
        refcount = 0;
    }
    
    virtual ~OSX_Device() {}
    
    virtual void open() {};
    virtual void close() {};
    
    t_CKUINT refcount; // incremented on open, decremented on close
};

class OSX_Hid_Device_Element
{
public:
    OSX_Hid_Device_Element()
    {
        cookie = 0;
        num = 0;
        value = 0;
        usage = usage_page = 0;
        min = 0;
        max = 0;
    }
    
    IOHIDElementCookie cookie;
    
    t_CKINT type;
    t_CKINT num;
    t_CKINT value;
    
    t_CKINT usage_page;
    t_CKINT usage;
    
    t_CKINT min;
    t_CKINT max;
};

class OSX_Hid_Device : public OSX_Device
{
public:
    OSX_Hid_Device()
    {
        configed = preconfiged = FALSE;
        plugInInterface = NULL;
        interface = NULL;
        queue = NULL;
        strncpy( name, "Device", 256 );
        elements = NULL;
        outputs = NULL;
        usage = usage_page = 0;
        type = num = -1;
        buttons = -1;
        axes = -1;
        hats = -1;
        wheels = -1;
        refcount = 0;
        hidProperties = NULL;
        removal_notification = NULL;
        vendorID = productID = locationID = NULL;
        manufacturer = product = NULL;
    }
    
    ~OSX_Hid_Device()
    {
        cleanup();
    }
    
    t_CKINT preconfigure( io_object_t ioHIDDeviceObject );
    t_CKINT configure();
    void enumerate_elements( CFArrayRef cfElements );
    t_CKINT start();
    t_CKINT stop();
    t_CKBOOL is_connected() const;
    t_CKBOOL is_same_as( io_object_t ioHIDDeviceObject ) const;
    void disconnect();
    void cleanup();
    
    IOCFPlugInInterface ** plugInInterface;
    IOHIDDeviceInterface ** interface; // device interface
    IOHIDQueueInterface ** queue; // device event queue
    
    char name[256];
    
    // uniquely identifying information
    /* For ChucK's purposes, HID devices are uniquely identified by the tuple of
       (USB device location, vendor ID, product ID, 
       manufacturer name, product name ) */
    CFNumberRef locationID;
    CFNumberRef vendorID;
    CFNumberRef productID;
    CFStringRef manufacturer;
    CFStringRef product;
    
    t_CKBOOL preconfiged, configed;
    
    t_CKINT type;
    t_CKINT num;
    t_CKINT usage_page;
    t_CKINT usage;
    
    map< IOHIDElementCookie, OSX_Hid_Device_Element * > * elements;
    
    /* Note: setting any of these to -1 informs the element enumerating 
        method that we are not interested in that particular element type */
    t_CKINT buttons;
    t_CKINT axes;
    t_CKINT hats;
    t_CKINT wheels;
    
    /* outputs are stored in a map of vectors of OSX_Hid_Device_Elements
        Thus if ( *outputs )[CK_HID_OUTPUT_TYPE] != NULL && 
        ( *outputs )[CK_HID_OUTPUT_TYPE]->size() > n, 
        ( *outputs )[CK_HID_OUTPUT_TYPE]->at( n ) is the nth output of type
        CK_HID_OUTPUT_TYPE. */
    map< unsigned int, vector< OSX_Hid_Device_Element * > * > * outputs;
        
    CFRunLoopSourceRef eventSource;
    
    CFMutableDictionaryRef hidProperties;
    
    io_object_t removal_notification;
    
    const static t_CKINT event_queue_size = 50; 
    // queues use wired kernel memory so should be set to as small as possible
    // but should account for the maximum possible events in the queue
    // USB updates will likely occur at 100 Hz so one must account for this rate
    // if states change quickly (updates are only posted on state changes)
};

struct OSX_Hid_op
{
    enum
    {
        open,
        close
    } type;
    
    xvector< OSX_Hid_Device * > * v;
    xvector< OSX_Hid_Device * >::size_type index;
};

#pragma mark global variables

// hid run loop (event dispatcher)
static CFRunLoopRef rlHid = NULL;
// special hid run loop mode (limits events to hid device events only)
//static const CFStringRef kCFRunLoopChuckHidMode = CFSTR( "ChucKHid" );
static const CFStringRef kCFRunLoopChuckHidMode = kCFRunLoopDefaultMode;

// general hid callback for device events
static void Hid_callback( void * target, IOReturn result, 
                          void * refcon, void * sender );

// CFRunLoop source for open/close device operations
static CFRunLoopSourceRef hidOpSource = NULL;
// cbuffer for open/close device operations
static CBufferSimple * hid_operation_buffer = NULL;
// callback for open/close device operations
static void Hid_do_operation( void * info );

// IO iterator for new hid devices
// we only keep track of this so we can release it later
io_iterator_t hid_iterator = NULL;

// notification port for device add/remove events
static IONotificationPortRef newDeviceNotificationPort = NULL;
// callback for new devices
static void Hid_new_devices( void * refcon, io_iterator_t iterator );
// callback for device removal
static void Hid_device_removed( void * refcon, io_service_t service,
                                natural_t messageType, void * messageArgument );

/* the mutexes should be acquired whenever making changes to the corresponding
vectors or accessing them from a non-hid thread after the hid thread has 
been started */
static xvector< OSX_Hid_Device * > * joysticks = NULL;
static xvector< OSX_Hid_Device * > * mice = NULL;
static xvector< OSX_Hid_Device * > * keyboards = NULL;

static xmultimap< string, OSX_Hid_Device * > * joystick_names = NULL;
static xmultimap< string, OSX_Hid_Device * > * mouse_names = NULL;
static xmultimap< string, OSX_Hid_Device * > * keyboard_names = NULL;

// has hid been initialized?
static t_CKBOOL g_hid_init = FALSE;
// table to translate keys to ASCII
static t_CKBYTE g_hid_key_table[256];

// cursor track stuff
#ifdef __CK_HID_CURSOR_TRACK__
static t_CKINT cursorX = 0;
static t_CKINT cursorY = 0;
static t_CKFLOAT scaledCursorX = 0;
static t_CKFLOAT scaledCursorY = 0;
static CFRunLoopRef rlCursorTrack = NULL;
static t_CKBOOL g_ct_go = FALSE;
#endif // __CK_HID_CURSOR_TRACK__

#ifdef __CK_HID_WIIREMOTE__
// wii remote stuff
static CFRunLoopSourceRef cfrlWiiRemoteSource = NULL;
static void WiiRemote_cfrl_callback( void * info );
#endif

t_CKINT OSX_Hid_Device::preconfigure( io_object_t ioHIDDeviceObject )
{
    // can only be called in Hid thread, or if the hid thread hasnt started
    assert( rlHid == CFRunLoopGetCurrent() || rlHid == NULL );

    if( preconfiged )
        return 0;
    
    switch( type )
    {
        case CK_HID_DEV_JOYSTICK:
            strncpy( name, "Joystick", 256 );
            buttons = 0;
            axes = 0;
            wheels = 0;
            hats = 0;
            break;
            
        case CK_HID_DEV_MOUSE:
            strncpy( name, "Mouse", 256 );
            buttons = 0;
            axes = 0;
            wheels = 0;
            hats = -1;
            break;
            
        case CK_HID_DEV_KEYBOARD:
            strncpy( name, "Keyboard", 256 );
            buttons = 0;
            axes = -1;
            wheels = -1;
            hats = -1;
            break;
    }
    
    // retrieve a dictionary of device properties
    kern_return_t kern_result = IORegistryEntryCreateCFProperties( ioHIDDeviceObject, 
                                                                   &hidProperties, 
                                                                   kCFAllocatorDefault, 
                                                                   kNilOptions);

    if( kern_result != KERN_SUCCESS || hidProperties == NULL )
    {
        hidProperties = NULL;
        return -1;
    }
    
    // get the device name, and copy it into the device record
    CFTypeRef refCF = CFDictionaryGetValue( hidProperties, 
                                            CFSTR( kIOHIDProductKey ) );
    if( refCF )
        CFStringGetCString( ( CFStringRef )refCF, name, 256, 
                            kCFStringEncodingASCII );
    
    // retrieve uniquely identifying information
    if( !locationID )
    {
        locationID = ( CFNumberRef ) CFDictionaryGetValue( hidProperties,
                                                           CFSTR( kIOHIDLocationIDKey ) );
        if( locationID )
            CFRetain( locationID );
    }
    
    if( !vendorID )
    {
        vendorID = ( CFNumberRef ) CFDictionaryGetValue( hidProperties,
                                                         CFSTR( kIOHIDVendorIDKey ) );
        if( vendorID )
            CFRetain( vendorID );
    }
    
    if( !productID )
    {
        productID = ( CFNumberRef ) CFDictionaryGetValue( hidProperties,
                                          CFSTR( kIOHIDProductIDKey ) );
        if( productID )
            CFRetain( productID );
    }
    
    if( !manufacturer )
    {
        manufacturer = ( CFStringRef ) CFDictionaryGetValue( hidProperties,
                                                             CFSTR( kIOHIDManufacturerKey ) );
        if( manufacturer )
            CFRetain( manufacturer );
    }
    
    if( !product )
    {
        product = ( CFStringRef ) CFDictionaryGetValue( hidProperties,
                                                        CFSTR( kIOHIDProductKey ) );
        if( product )
            CFRetain( product );
    }
    
    // create plugin interface
    IOReturn result;
    SInt32 score = 0;
    result = IOCreatePlugInInterfaceForService( ioHIDDeviceObject,
                                                kIOHIDDeviceUserClientTypeID,
                                                kIOCFPlugInInterfaceID,
                                                &plugInInterface,
                                                &score);
    
    if( result != kIOReturnSuccess )
    {
        CFRelease( hidProperties );
        hidProperties = NULL;
        return -1;
    }
    
#ifdef __LITTLE_ENDIAN__ 
    /* MacBooks and MacBook Pros have some sort of "phantom" trackpad, which 
       shows up in the HID registry and claims to no wheels.  The device
       name is usually Trackpad with the manufacturer being Apple.  This may
       disable legitimate Trackpads, but hopefully not... */
    if( type == CK_HID_DEV_MOUSE && strncmp( "Trackpad", name, 256 ) == 0 )
    {
        refCF = CFDictionaryGetValue( hidProperties, 
                                      CFSTR( kIOHIDManufacturerKey ) );
        if( refCF != NULL && 
            CFStringCompare( ( CFStringRef ) refCF, CFSTR( "Apple" ), 0 ) == 0 )
        {
            preconfiged = TRUE;
            configure();
            
            if( wheels == 0 )
                // this is the "phantom" trackpad
            {
                // iterate through the element map, delete device element records
                map< IOHIDElementCookie, OSX_Hid_Device_Element * >::iterator iter, end;                    
                end = elements->end();
                for( iter = elements->begin(); iter != end; iter++ )
                    delete iter->second;
                delete elements;
                elements = NULL;
                
                if( queue )
                {
                    ( *queue )->dispose( queue );
                    ( *queue )->Release( queue );
                    queue = NULL;
                }
                
                if( interface )
                {
                    ( *interface )->close( interface );
                    ( *interface )->Release( interface );
                    interface = NULL;
                }
                
                return -1;
            }
        }
    }
#endif // __LITTLE_ENDIAN__
    
    // register callback for removal notification, among a few other things
    result = IOServiceAddInterestNotification( newDeviceNotificationPort, 
                                               ioHIDDeviceObject,
                                               kIOGeneralInterest,
                                               Hid_device_removed,
                                               this,
                                               &removal_notification );
    
    preconfiged = TRUE;
    
    return 0;
}

t_CKINT OSX_Hid_Device::configure()
{
    // can only be called in Hid thread, or if the hid thread hasnt started
    assert( rlHid == NULL || rlHid == CFRunLoopGetCurrent() );

    if( configed )
        return 0;
    
    if( !preconfiged )
        return -1;
    
    EM_log( CK_LOG_INFO, "hid: configuring %s", name );
    
    // create the device interface
    HRESULT plugInResult = S_OK;
    plugInResult = (*plugInInterface)->QueryInterface( plugInInterface,
                                                       CFUUIDGetUUIDBytes( kIOHIDDeviceInterfaceID ),
                                                       ( LPVOID * ) &interface );
    (*plugInInterface)->Release( plugInInterface );
    plugInInterface = NULL;
    if( plugInResult != S_OK )
    {
        CFRelease( hidProperties );
        hidProperties = NULL;
        return -1;
    }
    
    // open the interface
    IOReturn result;
    result = (*interface)->open( interface, 0 );
    if( result != kIOReturnSuccess )
    {
        CFRelease( hidProperties );
        hidProperties = NULL;
        (*interface)->Release( interface );
        interface = NULL;
        return -1;
    }
    
    // create an event queue, so we dont have to poll individual elements
    queue = (*interface)->allocQueue( interface );
    if( !queue )
    {
        CFRelease( hidProperties );
        hidProperties = NULL;
        (*interface)->close( interface );
        (*interface)->Release( interface );
        interface = NULL;
        return -1;
    }
    
    result = (*queue)->create( queue, 0, event_queue_size );
    if( result != kIOReturnSuccess )
    {
        CFRelease( hidProperties );
        hidProperties = NULL;
        (*queue)->Release( queue );
        queue = NULL;
        (*interface)->close( interface );
        (*interface)->Release( interface );
        interface = NULL;
        return -1;
    }
    
    // set up the call back mechanism
    result = (*queue)->createAsyncEventSource( queue, &eventSource );
    if( result != kIOReturnSuccess )
    {
        CFRelease( hidProperties );
        hidProperties = NULL;
        (*queue)->dispose( queue );
        (*queue)->Release( queue );
        queue = NULL;
        (*interface)->close( interface );
        (*interface)->Release( interface );
        interface = NULL;
        return -1;
    }
    
    result = (*queue)->setEventCallout( queue, Hid_callback, NULL, this );
    if( result != kIOReturnSuccess )
    {
        CFRelease( hidProperties );
        hidProperties = NULL;
        (*queue)->dispose( queue );
        (*queue)->Release( queue );
        queue = NULL;
        (*interface)->close( interface );
        (*interface)->Release( interface );
        interface = NULL;
        return -1;
    }
    
    if( hidProperties != NULL )
    {
        if( elements == NULL )
        {
            // retrieve the array of elements...
            CFTypeRef refCF = CFDictionaryGetValue( hidProperties, 
                                                    CFSTR( kIOHIDElementKey ) );
            if( !refCF )
            {
                CFRelease( hidProperties );
                hidProperties = NULL;
                (*queue)->dispose( queue );
                (*queue)->Release( queue );
                queue = NULL;
                (*interface)->close( interface );
                (*interface)->Release( interface );
                interface = NULL;
                return -1;
            }
            
            // ...allocate space for element records...
            elements = new map< IOHIDElementCookie, OSX_Hid_Device_Element * >;
            // axes = 0;
            // buttons = 0;
            // hats = 0;
            
            // ...and parse the element array recursively
            enumerate_elements( ( CFArrayRef ) refCF );
        }
        
        else
        {
            map< IOHIDElementCookie, OSX_Hid_Device_Element * >::iterator iter, end;
            end = elements->end();
            for( iter = elements->begin(); iter != end; iter++ )
                ( *queue )->addElement( queue, iter->second->cookie, 0 );
        }
        
        CFRelease( hidProperties );
        hidProperties = NULL;
    }
        
    configed = TRUE;
    
    return 0;
}

//------------------------------------------------------------------------------
// name: enumerate_device_elements
// desc: perform depth depth first search on potentially nested arrays of 
// elements on the device, add found elements to the vector
//------------------------------------------------------------------------------
void OSX_Hid_Device::enumerate_elements( CFArrayRef cfElements )
{
    // can only be called in Hid thread, or if the hid thread hasnt started
    assert( rlHid == NULL || rlHid == CFRunLoopGetCurrent() );

    CFTypeRef refCF = 0;
    CFDictionaryRef element_dictionary;
    OSX_Hid_Device_Element * element;
    t_CKINT usage_page, usage, element_type;
    IOReturn result;
    
    CFIndex i, len = CFArrayGetCount( cfElements );
    for( i = 0; i < len; i++ )
    {
        // retrieve the dictionary of the ith element's data
        refCF = CFArrayGetValueAtIndex( cfElements, i );
        element_dictionary = ( CFDictionaryRef ) refCF;
        
        // get element type
        refCF = CFDictionaryGetValue( element_dictionary, 
                                      CFSTR( kIOHIDElementTypeKey ) );
        if( !refCF || 
            !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, 
                               &element_type ) )
            continue;
        
        // get element usage page
        refCF = CFDictionaryGetValue( element_dictionary, 
                                      CFSTR( kIOHIDElementUsagePageKey ) );
        if( !refCF || 
            !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, 
                               &usage_page ) )
            continue;
        
        // get element usage
        refCF = CFDictionaryGetValue( element_dictionary, 
                                      CFSTR( kIOHIDElementUsageKey ) );
        if( !refCF || 
            !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, 
                               &usage ) )
            continue;
        
        switch( element_type )
        {
            case kIOHIDElementTypeInput_Misc:
            case kIOHIDElementTypeInput_Button:
            case kIOHIDElementTypeInput_Axis:
            case kIOHIDElementTypeInput_ScanCodes:
                // an input of some sort
                switch( usage_page )
                {
                    case kHIDPage_GenericDesktop:
                        switch( usage )
                        {
                            case kHIDUsage_GD_X:
                            case kHIDUsage_GD_Y:
                            case kHIDUsage_GD_Z:
                            case kHIDUsage_GD_Rx:
                            case kHIDUsage_GD_Ry:
                            case kHIDUsage_GD_Rz:
                            case kHIDUsage_GD_Slider:
                            case kHIDUsage_GD_Dial:
                                // this is an axis
                                if( axes == -1 )
                                    continue;
                                
                                element = new OSX_Hid_Device_Element;
                                
                                element->num = axes;
                                if( this->type == CK_HID_DEV_JOYSTICK )
                                    element->type = CK_HID_JOYSTICK_AXIS;
                                else
                                    element->type = CK_HID_MOUSE_MOTION;
                                element->usage = usage;
                                element->usage_page = usage_page;
                                    
                                refCF = CFDictionaryGetValue( element_dictionary,
                                                              CFSTR( kIOHIDElementCookieKey ) );
                                if( !refCF || 
                                    !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, &element->cookie ) )
                                {
                                    delete element;
                                    continue;
                                }
                                    
                                refCF = CFDictionaryGetValue( element_dictionary,
                                                              CFSTR( kIOHIDElementMinKey ) );
                                if( !refCF || 
                                    !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, &element->min ) )
                                {
                                    delete element;
                                    continue;
                                }
                                    
                                refCF = CFDictionaryGetValue( element_dictionary,
                                                              CFSTR( kIOHIDElementMaxKey ) );
                                if( !refCF || 
                                    !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, &element->max ) )
                                {
                                    delete element;
                                    continue;
                                }
                                    
                                result = (*queue)->addElement( queue, element->cookie, 0 );
                                if( result != kIOReturnSuccess )
                                {
                                    delete element;
                                    continue;
                                }
                                    
                                EM_log( CK_LOG_FINE, "adding axis %d", axes );
                                
                                (*elements)[element->cookie] = element;
                                axes++;
                                
                                break;
                                
                            case kHIDUsage_GD_Wheel:
                                // this is an wheel
                                if( wheels == -1 )
                                    continue;
                                
                                element = new OSX_Hid_Device_Element;
                                
                                element->num = wheels;
                                if( this->type == CK_HID_DEV_JOYSTICK )
                                    element->type = CK_HID_JOYSTICK_AXIS;
                                else
                                    element->type = CK_HID_MOUSE_WHEEL;
                                element->usage = usage;
                                element->usage_page = usage_page;
                                
                                refCF = CFDictionaryGetValue( element_dictionary,
                                                              CFSTR( kIOHIDElementCookieKey ) );
                                if( !refCF || 
                                    !CFNumberGetValue( ( CFNumberRef ) refCF, 
                                                       kCFNumberLongType, 
                                                       &element->cookie ) )
                                {
                                    delete element;
                                    continue;
                                }
                                    
                                result = (*queue)->addElement( queue, 
                                                               element->cookie, 
                                                               0 );
                                if( result != kIOReturnSuccess )
                                {
                                    delete element;
                                    continue;
                                }
                                    
                                EM_log( CK_LOG_FINE, "adding wheel %d", wheels );
                                
                                (*elements)[element->cookie] = element;
                                wheels++;
                                break;
                                
                            case kHIDUsage_GD_Hatswitch:
                                // this is a hat
                                if( hats == -1 )
                                    continue;

                                element = new OSX_Hid_Device_Element; 
                                    
                                element->type = CK_HID_JOYSTICK_HAT;
                                element->num = hats;
                                element->usage = usage;
                                element->usage_page = usage_page;
                                
                                refCF = CFDictionaryGetValue( element_dictionary,
                                                              CFSTR( kIOHIDElementCookieKey ) );
                                if( !refCF || 
                                    !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, &element->cookie ) )
                                {
                                    delete element;
                                    continue;
                                }
                                    
                                result = (*queue)->addElement( queue, element->cookie, 0 );
                                if( result != kIOReturnSuccess )
                                {
                                    delete element;
                                    continue;
                                }
                                    
                                EM_log( CK_LOG_FINE, "adding hat %d", hats );
                                
                                (*elements)[element->cookie] = element;
                                hats++;
                                
                                break;
                                
                            default:
                                EM_log( CK_LOG_INFO, "unknown page: %i usage: %i\n", usage_page, usage );
                        }
                        
                        break;
                        
                    case kHIDPage_Button:
                    case kHIDPage_KeyboardOrKeypad:
                        // this is a button
                        if( buttons == -1 )
                            continue;
                        
                        /* filter out error and reserved usages*/
                        if( usage_page == kHIDPage_KeyboardOrKeypad &&
                            !( usage >= kHIDUsage_KeyboardA &&
                               usage <= kHIDUsage_KeyboardRightGUI ) )
                            continue;
                        
                        element = new OSX_Hid_Device_Element; 
                        
                        element->type = CK_HID_BUTTON_DOWN;
                        element->num = buttons;
                        element->usage = usage;
                        element->usage_page = usage_page;
                        
                        refCF = CFDictionaryGetValue( element_dictionary,
                                                      CFSTR( kIOHIDElementCookieKey ) );
                        if( !refCF || 
                            !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, &element->cookie ) )
                        {
                            delete element;
                            continue;
                        }
                            
                        result = (*queue)->addElement( queue, element->cookie, 0 );
                        if( result != kIOReturnSuccess )
                        {
                            delete element;
                            continue;
                        }
                            
                        EM_log( CK_LOG_FINE, "adding button %d", buttons );
                        
                        (*elements)[element->cookie] = element;
                        buttons++;
                        
                        break;
                        
                    default:
                        EM_log( CK_LOG_FINER, "unknown page: %i usage: %i\n", usage_page, usage );
                }
                
                break;
                
            case kIOHIDElementTypeCollection:
                refCF = CFDictionaryGetValue( element_dictionary, CFSTR( kIOHIDElementKey ) );
                if( !refCF )
                    continue;
                    
                enumerate_elements( ( CFArrayRef ) refCF );
                break;
                
            case kIOHIDElementTypeOutput:
                switch( usage_page )
                {
                    case kHIDPage_LEDs:
                        /* filter out error and reserved usages */
                        if( usage > kHIDUsage_LED_ExternalPowerConnected )
                            continue;
                        
                        element = new OSX_Hid_Device_Element;
                        
                        refCF = CFDictionaryGetValue( element_dictionary,
                                                      CFSTR( kIOHIDElementCookieKey ) );
                        if( !refCF || 
                            !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, &element->cookie ) )
                        {
                            delete element;
                            continue;
                        }
                        
                        refCF = CFDictionaryGetValue( element_dictionary,
                                                          CFSTR( kIOHIDElementMinKey ) );
                        if( !refCF || 
                            !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, &element->min ) )
                        {
                            delete element;
                            continue;
                        }
                            
                        refCF = CFDictionaryGetValue( element_dictionary,
                                                      CFSTR( kIOHIDElementMaxKey ) );
                        if( !refCF || 
                            !CFNumberGetValue( ( CFNumberRef ) refCF, kCFNumberLongType, &element->max ) )
                        {
                            delete element;
                            continue;
                        }
                            
                        if( outputs == NULL )
                            outputs = new map< unsigned int, vector< OSX_Hid_Device_Element * > * >;
                        
                        if( outputs->find( CK_HID_LED ) == outputs->end() )
                            ( *outputs )[CK_HID_LED] = new vector< OSX_Hid_Device_Element * >;
                            
                        element->type = CK_HID_LED;
                        element->num = ( *outputs )[CK_HID_LED]->size();
                        element->usage = usage;
                        element->usage_page = usage_page;
                        
                        EM_log( CK_LOG_FINE, "adding led %d", element->num );
                        
                        ( *outputs )[CK_HID_LED]->push_back( element );
                        
                        break;
                }
                
                break;
        }
    }
}

t_CKINT OSX_Hid_Device::start()
{
    // can only be called in Hid thread
    assert( rlHid == CFRunLoopGetCurrent() );
    
    EM_log( CK_LOG_INFO, "hid: starting %s", name );
    EM_pushlog();
    
    // configure if needed
    if( !configed )
    {
        // lock the device so we can mutate it
        this->lock();
        
        // configure
        if( configure() )
        {
            // error during configuration!
            this->unlock();
            
            EM_log( CK_LOG_SEVERE, "hid: error configuring %s", name );
            EM_poplog();
            
            return -1;
        }
        
        this->unlock();
    }
    
    EM_log( CK_LOG_INFO, "hid: adding %s to run loop", name );
    CFRunLoopAddSource( rlHid, eventSource, kCFRunLoopChuckHidMode );
    
    IOReturn result = ( *queue )->start( queue );
    if( result != kIOReturnSuccess )
    {
        EM_log( CK_LOG_SEVERE, "hid: error starting event queue for %s", name );
        EM_poplog();
        return -1;
    }
    
    EM_poplog();
    
    return 0;
}

t_CKINT OSX_Hid_Device::stop()
{
    // can only be called in Hid thread
    assert( rlHid == CFRunLoopGetCurrent() );
    
    if( queue )
    {
        IOReturn result = ( *queue )->stop( queue );
        if( result != kIOReturnSuccess )
            EM_log( CK_LOG_INFO, "hid: error stopping event queue for %s", name );
    }
    
    CFRunLoopRemoveSource( rlHid, eventSource, kCFRunLoopChuckHidMode );
    
    return 0;
}

t_CKBOOL OSX_Hid_Device::is_connected() const
{
    if( plugInInterface == NULL && interface == NULL )
        return FALSE;
    return TRUE;
}

t_CKBOOL OSX_Hid_Device::is_same_as( io_object_t ioHIDDeviceObject ) const
{
    CFMutableDictionaryRef hidProperties2 = NULL;
    t_CKBOOL result = FALSE;
    
    // retrieve a dictionary of device properties
    kern_return_t kern_result = IORegistryEntryCreateCFProperties( ioHIDDeviceObject, 
                                                                   &hidProperties2, 
                                                                   kCFAllocatorDefault, 
                                                                   kNilOptions );
    
    if( kern_result != KERN_SUCCESS || hidProperties2 == NULL )
    {
        return FALSE;
    }
    
    // if no "uniquely identifying" info is available, then assume its new
    if( !locationID && !vendorID && !productID && !manufacturer && !product )
        goto end;
    
    CFNumberRef locationID2 = ( CFNumberRef ) CFDictionaryGetValue( hidProperties2,
                                                                    CFSTR( kIOHIDLocationIDKey ) );
    if( ( locationID && !locationID2 ) || ( !locationID && locationID2 ) )
        goto end; // not the same
    if( locationID && locationID2 && CFNumberCompare( locationID, locationID2, NULL ) )
        goto end; // not the same
    
    CFNumberRef vendorID2 = ( CFNumberRef ) CFDictionaryGetValue( hidProperties2,
                                                                  CFSTR( kIOHIDVendorIDKey ) );
    if( ( vendorID && !vendorID2 ) || ( !vendorID && vendorID2 ) )
        goto end; // not the same
    if( vendorID && vendorID2 && CFNumberCompare( vendorID, vendorID2, NULL ) )
        goto end; // not the same
    
    CFNumberRef productID2 = ( CFNumberRef ) CFDictionaryGetValue( hidProperties2,
                                                                   CFSTR( kIOHIDProductIDKey ) );
    if( ( productID && !productID2 ) || ( !productID && productID2 ) )
        goto end; // not the same
    if( productID && productID2 && CFNumberCompare( productID, productID2, NULL ) )
        goto end; // not the same
    
    CFStringRef manufacturer2 = ( CFStringRef ) CFDictionaryGetValue( hidProperties2,
                                                                      CFSTR( kIOHIDManufacturerKey ) );
    if( ( manufacturer && !manufacturer2 ) || ( !manufacturer && manufacturer2 ) )
        goto end; // not the same
    if( manufacturer && manufacturer2 && CFStringCompare( manufacturer, manufacturer2, 0 ) )
        goto end; // not the same
    
    CFStringRef product2 = ( CFStringRef ) CFDictionaryGetValue( hidProperties2,
                                                                 CFSTR( kIOHIDProductKey ) );
    if( ( product && !product2 ) || ( !product && product2 ) )
        goto end; // not the same
    if( product && product2 && CFStringCompare( product, product2, 0 ) )
        goto end; // not the same
    
    // if execution reaches this point, then they are the same
    result = TRUE; 
    
end:
    CFRelease( hidProperties2 );
    return result;
}

//------------------------------------------------------------------------------
// name: disconnect()
// desc: called when device is physically disconnected from the machine.  
// deallocates per-connection resources, but leaves stuff that should be the
// same across connections (element list, name, other general properties, etc)
//------------------------------------------------------------------------------
void OSX_Hid_Device::disconnect()
{
    // can only be called in Hid thread, or if the hid thread hasnt started
    assert( rlHid == NULL || rlHid == CFRunLoopGetCurrent() );
    
    if( plugInInterface )
    {
        ( *plugInInterface )->Release( plugInInterface );
        plugInInterface = NULL;
    }
    
    if( queue )
    {
        if( refcount )
        {
            ( *queue )->stop( queue );
            
            if( rlHid )
                CFRunLoopRemoveSource( rlHid, eventSource, 
                                       kCFRunLoopChuckHidMode );
        }
        
        ( *queue )->dispose( queue );
        ( *queue )->Release( queue );
        queue = NULL;
    }
    
    if( interface )
    {
        ( *interface )->close( interface );
        ( *interface )->Release( interface );
        interface = NULL;
    }
    
    if( removal_notification )
    {
        IOObjectRelease( removal_notification );
        removal_notification = NULL;
    }
    
    configed = FALSE;
    preconfiged = FALSE;
}

//------------------------------------------------------------------------------
// name: cleanup()
// desc: deallocates all resources associated with the device
//------------------------------------------------------------------------------
void OSX_Hid_Device::cleanup()
{
    // can only be called in Hid thread, or if the hid thread hasnt started
    assert( rlHid == NULL || rlHid == CFRunLoopGetCurrent() );

    disconnect();
    
    // delete all elements
    if( elements )
    {
        this->lock();
        
        // iterate through the axis map, delete device element records
        map< IOHIDElementCookie, OSX_Hid_Device_Element * >::iterator iter, end;
        end = elements->end();
        for( iter = elements->begin(); iter != end; iter++ )
            delete iter->second;
        delete elements;
        elements = NULL;
        
        // TODO: delete outputs also
        
        this->unlock();
    }
    
    if( hidProperties )
    {
        CFRelease( hidProperties );
        hidProperties = NULL;
    }
    
    this->lock();
    
    if( locationID )
    {
        CFRelease( locationID );
        locationID = NULL;
    }
    
    if( vendorID )
    {
        CFRelease( vendorID );
        vendorID = NULL;
    }
    
    if( productID )
    {
        CFRelease( productID );
        productID = NULL;
    }
    
    if( manufacturer )
    {
        CFRelease( manufacturer );
        manufacturer = NULL;
    }
    
    if( product )
    {
        CFRelease( product );
        product = NULL;
    }
    
    this->unlock();
}

static void Hid_key_table_init()
{
    memset( g_hid_key_table, 0, sizeof( g_hid_key_table ) );
    
    // ASCII letters
    g_hid_key_table[kHIDUsage_KeyboardA] = 'A';
    g_hid_key_table[kHIDUsage_KeyboardB] = 'B';
    g_hid_key_table[kHIDUsage_KeyboardC] = 'C';
    g_hid_key_table[kHIDUsage_KeyboardD] = 'D';
    g_hid_key_table[kHIDUsage_KeyboardE] = 'E';
    g_hid_key_table[kHIDUsage_KeyboardF] = 'F';
    g_hid_key_table[kHIDUsage_KeyboardG] = 'G';
    g_hid_key_table[kHIDUsage_KeyboardH] = 'H';
    g_hid_key_table[kHIDUsage_KeyboardI] = 'I';
    g_hid_key_table[kHIDUsage_KeyboardJ] = 'J';
    g_hid_key_table[kHIDUsage_KeyboardK] = 'K';
    g_hid_key_table[kHIDUsage_KeyboardL] = 'L';
    g_hid_key_table[kHIDUsage_KeyboardM] = 'M';
    g_hid_key_table[kHIDUsage_KeyboardN] = 'N';
    g_hid_key_table[kHIDUsage_KeyboardO] = 'O';
    g_hid_key_table[kHIDUsage_KeyboardP] = 'P';
    g_hid_key_table[kHIDUsage_KeyboardQ] = 'Q';
    g_hid_key_table[kHIDUsage_KeyboardR] = 'R';
    g_hid_key_table[kHIDUsage_KeyboardS] = 'S';
    g_hid_key_table[kHIDUsage_KeyboardT] = 'T';
    g_hid_key_table[kHIDUsage_KeyboardU] = 'U';
    g_hid_key_table[kHIDUsage_KeyboardV] = 'V';
    g_hid_key_table[kHIDUsage_KeyboardW] = 'W';
    g_hid_key_table[kHIDUsage_KeyboardX] = 'X';
    g_hid_key_table[kHIDUsage_KeyboardY] = 'Y';
    g_hid_key_table[kHIDUsage_KeyboardZ] = 'Z';
    
    // ASCII numbers
    g_hid_key_table[kHIDUsage_Keyboard1] = '1';
    g_hid_key_table[kHIDUsage_Keyboard2] = '2';
    g_hid_key_table[kHIDUsage_Keyboard3] = '3';
    g_hid_key_table[kHIDUsage_Keyboard4] = '4';
    g_hid_key_table[kHIDUsage_Keyboard5] = '5';
    g_hid_key_table[kHIDUsage_Keyboard6] = '6';
    g_hid_key_table[kHIDUsage_Keyboard7] = '7';
    g_hid_key_table[kHIDUsage_Keyboard8] = '8';
    g_hid_key_table[kHIDUsage_Keyboard9] = '9';
    g_hid_key_table[kHIDUsage_Keyboard0] = '0';
    
    // ASCII whitespace
    g_hid_key_table[kHIDUsage_KeyboardReturnOrEnter] = '\n';
    g_hid_key_table[kHIDUsage_KeyboardTab] = '\t';
    g_hid_key_table[kHIDUsage_KeyboardSpacebar] = ' ';
    
    // ASCII symbols
    g_hid_key_table[kHIDUsage_KeyboardHyphen] = '-';
    g_hid_key_table[kHIDUsage_KeyboardEqualSign] = '=';
    g_hid_key_table[kHIDUsage_KeyboardOpenBracket] = '{';
    g_hid_key_table[kHIDUsage_KeyboardCloseBracket] = '}';
    g_hid_key_table[kHIDUsage_KeyboardBackslash] = '\\';
    g_hid_key_table[kHIDUsage_KeyboardNonUSPound] = '#';
    g_hid_key_table[kHIDUsage_KeyboardSemicolon] = ';';
    g_hid_key_table[kHIDUsage_KeyboardQuote] = '\'';
    g_hid_key_table[kHIDUsage_KeyboardGraveAccentAndTilde] = '`';
    g_hid_key_table[kHIDUsage_KeyboardComma] = ',';
    g_hid_key_table[kHIDUsage_KeyboardPeriod] = '.';
    g_hid_key_table[kHIDUsage_KeyboardSlash] = '/';
    
    // ASCII keypad symbols/whitespace
    g_hid_key_table[kHIDUsage_KeypadSlash] = '/';
    g_hid_key_table[kHIDUsage_KeypadAsterisk] = '*';
    g_hid_key_table[kHIDUsage_KeypadHyphen] = '-';
    g_hid_key_table[kHIDUsage_KeypadPlus] = '+';
    g_hid_key_table[kHIDUsage_KeypadEnter] = '\n';
    
    // ASCII keypad numbers
    g_hid_key_table[kHIDUsage_Keypad1] = '1';
    g_hid_key_table[kHIDUsage_Keypad2] = '2';
    g_hid_key_table[kHIDUsage_Keypad3] = '3';
    g_hid_key_table[kHIDUsage_Keypad4] = '4';
    g_hid_key_table[kHIDUsage_Keypad5] = '5';
    g_hid_key_table[kHIDUsage_Keypad6] = '6';
    g_hid_key_table[kHIDUsage_Keypad7] = '7';
    g_hid_key_table[kHIDUsage_Keypad8] = '8';
    g_hid_key_table[kHIDUsage_Keypad9] = '9';
    g_hid_key_table[kHIDUsage_Keypad0] = '0';
    
    // ASCII keypad symbols
    g_hid_key_table[kHIDUsage_KeypadPeriod] = '.';
    g_hid_key_table[kHIDUsage_KeyboardNonUSBackslash] = '\\';
    g_hid_key_table[kHIDUsage_KeypadEqualSign] = '=';
    g_hid_key_table[kHIDUsage_KeypadComma] = ',';
    g_hid_key_table[kHIDUsage_KeypadEqualSignAS400] = '=';
}

t_CKINT Hid_hwkey_to_ascii( t_CKINT hwkey )
{
    if( hwkey < 0 || hwkey >= sizeof( g_hid_key_table ) )
        return -1;
    
    return g_hid_key_table[hwkey];
}

void Hid_init()
{
    Hid_key_table_init();
    
    rlHid = CFRunLoopGetCurrent();
    
    // set up notification for new added devices/removed devices
    CFRunLoopAddSource( rlHid, 
                        IONotificationPortGetRunLoopSource( newDeviceNotificationPort ),
                        kCFRunLoopChuckHidMode );
    
    // add open/close operation source
    CFRunLoopSourceContext opSourceContext;
    opSourceContext.version = 0;
    opSourceContext.info = NULL;
    opSourceContext.retain = NULL;
    opSourceContext.release = NULL;
    opSourceContext.copyDescription = NULL;
    opSourceContext.equal = NULL;
    opSourceContext.hash = NULL;
    opSourceContext.schedule = NULL;
    opSourceContext.cancel = NULL;
    opSourceContext.perform = Hid_do_operation;
    CFRunLoopSourceRef _hidOpSource = CFRunLoopSourceCreate( kCFAllocatorDefault, 
                                                             0, 
                                                             &opSourceContext );
    CFRunLoopAddSource( rlHid, _hidOpSource, kCFRunLoopChuckHidMode );
    hidOpSource = _hidOpSource;
    
    Hid_do_operation( NULL );
    
#ifdef __CK_HID_WIIREMOTE__
    // add wii remote source
    CFRunLoopSourceContext wrSourceContext;
    wrSourceContext.version = 0;
    wrSourceContext.info = NULL;
    wrSourceContext.retain = NULL;
    wrSourceContext.release = NULL;
    wrSourceContext.copyDescription = NULL;
    wrSourceContext.equal = NULL;
    wrSourceContext.hash = NULL;
    wrSourceContext.schedule = NULL;
    wrSourceContext.cancel = NULL;
    wrSourceContext.perform = WiiRemote_cfrl_callback;
    CFRunLoopSourceRef _cfrlWiiRemoteSource = CFRunLoopSourceCreate( kCFAllocatorDefault, 
                                                                     0, 
                                                                     &wrSourceContext );
    CFRunLoopAddSource( rlHid, _cfrlWiiRemoteSource, kCFRunLoopChuckHidMode );
    cfrlWiiRemoteSource = _cfrlWiiRemoteSource;
    
    WiiRemote_cfrl_callback( NULL );
#endif
}

void Hid_init2()
{
    // verify that the joystick system has not already been initialized
    if( g_hid_init == TRUE )
        return;
    
    g_hid_init = TRUE;
        
	IOReturn result = kIOReturnSuccess;
	io_iterator_t hidObjectIterator = 0;
    CFTypeRef refCF;
    t_CKINT filter_usage_page = kHIDPage_GenericDesktop;
    
    // allocate vectors of device records
    joysticks = new xvector< OSX_Hid_Device * >;
    mice = new xvector< OSX_Hid_Device * >;
    keyboards = new xvector< OSX_Hid_Device * >;
    
    joystick_names = new xmultimap< string, OSX_Hid_Device * >;
    mouse_names = new xmultimap< string, OSX_Hid_Device * >;
    keyboard_names = new xmultimap< string, OSX_Hid_Device * >;
    
    hid_operation_buffer = new CBufferSimple;
    hid_operation_buffer->initialize( 20, sizeof( OSX_Hid_op ) );
    
	CFMutableDictionaryRef hidMatchDictionary = IOServiceMatching( kIOHIDDeviceKey );
    if( !hidMatchDictionary )
    {
        EM_log( CK_LOG_SEVERE, "hid: error: unable to retrieving hidMatchDictionary, unable to initialize" );
        return;
    }
    
    refCF = ( CFTypeRef ) CFNumberCreate( kCFAllocatorDefault, 
                                          kCFNumberLongType, 
                                          &filter_usage_page );
    CFDictionarySetValue( hidMatchDictionary, 
                          CFSTR( kIOHIDPrimaryUsagePageKey ), refCF );
    
    newDeviceNotificationPort = IONotificationPortCreate( kIOMasterPortDefault );
    
    result = IOServiceAddMatchingNotification( newDeviceNotificationPort,
                                               kIOFirstMatchNotification,
                                               hidMatchDictionary,
                                               Hid_new_devices,
                                               NULL,
                                               &hidObjectIterator );
    
    if( result != kIOReturnSuccess || hidObjectIterator == 0 )
    {
        EM_log( CK_LOG_SEVERE, "hid: error: unable to retrieving matching services, unable to initialize" );
        return;
    }

    CFRelease( refCF );
    
    Hid_new_devices( NULL, hidObjectIterator );
}

static void Hid_new_devices( void * refcon, io_iterator_t iterator )
{
    EM_log( CK_LOG_INFO, "hid: new device(s) found" );
    
    char __temp[256];
    
    CFTypeRef refCF = NULL;
    
    io_object_t ioHIDDeviceObject = 0;
    t_CKINT usage, usage_page;
    t_CKINT joysticks_seen = joysticks->size(), 
        mice_seen = mice->size(), 
        keyboards_seen = keyboards->size();
    
    while( ioHIDDeviceObject = IOIteratorNext( iterator ) )
    {        
        // ascertain device information
        
        // first, determine the device usage page and usage
        usage = usage_page = -1;
        
        refCF = IORegistryEntryCreateCFProperty( ioHIDDeviceObject, 
                                                 CFSTR( kIOHIDPrimaryUsagePageKey ),
                                                 kCFAllocatorDefault, 
                                                 kNilOptions );
        if( !refCF )
            continue;
        
        CFNumberGetValue( ( CFNumberRef )refCF, kCFNumberLongType, &usage_page );
        CFRelease( refCF );
        
        refCF = IORegistryEntryCreateCFProperty( ioHIDDeviceObject, 
                                                 CFSTR( kIOHIDPrimaryUsageKey ),
                                                 kCFAllocatorDefault, 
                                                 kNilOptions);
        if( !refCF )
            continue;
        
        CFNumberGetValue( ( CFNumberRef )refCF, kCFNumberLongType, &usage );
        CFRelease( refCF );
        
        if ( usage_page == kHIDPage_GenericDesktop )
        {
            if( usage == kHIDUsage_GD_Joystick || 
                usage == kHIDUsage_GD_GamePad )
                // this is a joystick, create a new item in the joystick array
            {
                // see if this an device that was disconnected being reconnected
                refCF = IORegistryEntryCreateCFProperty( ioHIDDeviceObject, 
                                                         CFSTR( kIOHIDProductKey ),
                                                         kCFAllocatorDefault, 
                                                         kNilOptions);
                
                if( refCF )
                {
                    CFStringGetCString( ( CFStringRef ) refCF, __temp, 256,
                                        kCFStringEncodingASCII );
                    CFRelease( refCF );                    
                }
                
                else
                    strncpy( __temp, "Joystick", 256 );
                
                pair< xmultimap< string, OSX_Hid_Device * >::const_iterator, 
                      xmultimap< string, OSX_Hid_Device * >::const_iterator > name_range 
                    = joystick_names->equal_range( string( __temp ) );
                
                xmultimap< string, OSX_Hid_Device * >::const_iterator 
                    start = name_range.first, 
                    end = name_range.second;
                
                t_CKBOOL match_found = FALSE;
                
                for( ; start != end; start++ )
                {
                    OSX_Hid_Device * old_device = ( *start ).second;
                    if( !old_device->is_connected() && 
                        old_device->is_same_as( ioHIDDeviceObject ) )
                    {
                        match_found = TRUE;
                        
                        EM_log( CK_LOG_INFO, "hid: joystick %s reattached", 
                                old_device->name );
                        
                        if( old_device->preconfigure( ioHIDDeviceObject ) )
                        {
                            EM_log( CK_LOG_INFO, "hid: error during reconfiguration of %s",
                                    old_device->name );
                            break;
                        }
                        
                        if( old_device->refcount &&
                            !old_device->start() )
                        {                            
                            HidMsg msg;
                            msg.device_type = old_device->type;
                            msg.device_num = old_device->num;
                            msg.type = CK_HID_DEVICE_CONNECTED;
                            
                            HidInManager::push_message( msg );
                        }
                        
                        break;
                    }
                }
                
                if( match_found )
                    continue;
                
                // create a new device record for the device
                EM_log( CK_LOG_INFO, "hid: preconfiguring joystick %i", joysticks_seen );
                EM_pushlog();
                
                // allocate the device record, set usage page and usage
                OSX_Hid_Device * new_device = new OSX_Hid_Device;
                new_device->type = CK_HID_DEV_JOYSTICK;
                new_device->num = joysticks->size();
                new_device->usage_page = usage_page;
                new_device->usage = usage;
                
                if( !new_device->preconfigure( ioHIDDeviceObject ) )
                {
                    joysticks->lock();
                    joysticks->push_back( new_device );
                    joysticks->unlock();
                    
                    joystick_names->lock();
                    joystick_names->insert( pair< string, OSX_Hid_Device * >( string( new_device->name ), 
                                                                              new_device ) );
                    joystick_names->unlock();
                }
                
                else
                {
                    EM_log( CK_LOG_INFO, 
                            "hid: error during preconfiguration of joystick %i",
                            joysticks_seen );
                    delete new_device;
                }
                
                joysticks_seen++;
                
                EM_poplog();
            }
            
            if( usage == kHIDUsage_GD_Mouse )
                // this is a mouse
            {
                // see if this an device that was disconnected being reconnected
                refCF = IORegistryEntryCreateCFProperty( ioHIDDeviceObject, 
                                                         CFSTR( kIOHIDProductKey ),
                                                         kCFAllocatorDefault, 
                                                         kNilOptions);
                
                if( refCF )
                {
                    CFStringGetCString( ( CFStringRef ) refCF, __temp, 256,
                                        kCFStringEncodingASCII );
                    CFRelease( refCF );                    
                }
                
                else
                    strncpy( __temp, "Mouse", 256 );
                
                pair< xmultimap< string, OSX_Hid_Device * >::const_iterator, 
                    xmultimap< string, OSX_Hid_Device * >::const_iterator > name_range 
                    = mouse_names->equal_range( string( __temp ) );
                
                xmultimap< string, OSX_Hid_Device * >::const_iterator 
                    start = name_range.first, 
                    end = name_range.second;
                
                t_CKBOOL match_found = FALSE;
                
                for( ; start != end; start++ )
                {
                    OSX_Hid_Device * old_device = ( *start ).second;
                    if( !old_device->is_connected() && 
                        old_device->is_same_as( ioHIDDeviceObject ) )
                    {
                        match_found = TRUE;
                        
                        EM_log( CK_LOG_INFO, "hid: mouse %s reattached", 
                                old_device->name );
                        
                        if( old_device->preconfigure( ioHIDDeviceObject ) )
                        {
                            EM_log( CK_LOG_INFO, "hid: error during reconfiguration of %s",
                                    old_device->name );
                            break;
                        }
                        
                        if( old_device->refcount &&
                            !old_device->start() )
                        {                            
                            HidMsg msg;
                            msg.device_type = old_device->type;
                            msg.device_num = old_device->num;
                            msg.type = CK_HID_DEVICE_CONNECTED;
                            
                            HidInManager::push_message( msg );
                        }
                        
                        break;
                    }
                }
                
                if( match_found )
                    continue;
                
                // create a new device record for the device
                EM_log( CK_LOG_INFO, "hid: preconfiguring mouse %i", mice_seen );
                EM_pushlog();
                
                // allocate the device record, set usage page and usage
                OSX_Hid_Device * new_device = new OSX_Hid_Device;
                new_device->type = CK_HID_DEV_MOUSE;
                new_device->num = mice->size();
                new_device->usage_page = usage_page;
                new_device->usage = usage;
                
                if( !new_device->preconfigure( ioHIDDeviceObject ) )
                {
                    mice->lock();
                    mice->push_back( new_device );
                    mice->unlock();
                    
                    mouse_names->lock();
                    mouse_names->insert( pair< string, OSX_Hid_Device * >( string( new_device->name ), 
                                                                           new_device ) );
                    mouse_names->unlock();                    
                }
                
                else
                {
                    EM_log( CK_LOG_INFO, 
                            "hid: error during preconfiguration of mouse %i",
                            mice_seen );
                    delete new_device;
                }
                
                mice_seen++;
                
                EM_poplog();
            }
            
            if( usage == kHIDUsage_GD_Keyboard || usage == kHIDUsage_GD_Keypad )
                // this is a keyboard
            {
                // see if this an device that was disconnected being reconnected
                refCF = IORegistryEntryCreateCFProperty( ioHIDDeviceObject, 
                                                         CFSTR( kIOHIDProductKey ),
                                                         kCFAllocatorDefault, 
                                                         kNilOptions);
                
                if( refCF )
                {
                    CFStringGetCString( ( CFStringRef ) refCF, __temp, 256,
                                        kCFStringEncodingASCII );
                    CFRelease( refCF );                    
                }
                
                else
                    strncpy( __temp, "Keyboard", 256 );
                    
                pair< xmultimap< string, OSX_Hid_Device * >::const_iterator, 
                    xmultimap< string, OSX_Hid_Device * >::const_iterator > name_range 
                    = keyboard_names->equal_range( string( __temp ) );
                
                xmultimap< string, OSX_Hid_Device * >::const_iterator 
                    start = name_range.first, 
                    end = name_range.second;
                
                t_CKBOOL match_found = FALSE;
                
                for( ; start != end; start++ )
                {
                    OSX_Hid_Device * old_device = ( *start ).second;
                    if( !old_device->is_connected() && 
                        old_device->is_same_as( ioHIDDeviceObject ) )
                    {
                        match_found = TRUE;
                        
                        EM_log( CK_LOG_INFO, "hid: keyboard %s reattached", 
                                old_device->name );
                        
                        if( old_device->preconfigure( ioHIDDeviceObject ) )
                        {
                            EM_log( CK_LOG_INFO, "hid: error during reconfiguration of %s",
                                    old_device->name );
                            break;
                        }
                        
                        if( old_device->refcount &&
                            !old_device->start() )
                        {                            
                            HidMsg msg;
                            msg.device_type = old_device->type;
                            msg.device_num = old_device->num;
                            msg.type = CK_HID_DEVICE_CONNECTED;
                            
                            HidInManager::push_message( msg );
                        }
                        
                        break;
                    }
                }
                
                if( match_found )
                    continue;
                
                // create a new device record for the device
                EM_log( CK_LOG_INFO, "hid: preconfiguring keyboard %i", 
                        keyboards_seen );
                EM_pushlog();
                
                // allocate the device record, set usage page and usage
                OSX_Hid_Device * new_device = new OSX_Hid_Device;
                new_device->type = CK_HID_DEV_KEYBOARD;
                new_device->num = keyboards->size();
                new_device->usage_page = usage_page;
                new_device->usage = usage;
                
                if( !new_device->preconfigure( ioHIDDeviceObject ) )
                {
                    keyboards->lock();
                    keyboards->push_back( new_device );
                    keyboards->unlock();
                    
                    keyboard_names->lock();
                    keyboard_names->insert( pair< string, OSX_Hid_Device * >( string( new_device->name ), 
                                                                              new_device ) );
                    keyboard_names->unlock();                    
                }
                
                else
                {
                    EM_log( CK_LOG_INFO, 
                            "hid: error during preconfiguration of keyboard %i",
                            keyboards_seen );
                    delete new_device;
                }
                
                keyboards_seen++;
                
                EM_poplog();
            }
        }
    }
}

void Hid_device_removed( void * refcon, io_service_t service,
                         natural_t messageType, void * messageArgument )
{
    if( messageType == kIOMessageServiceIsTerminated && refcon )
    {
        OSX_Hid_Device * device = ( OSX_Hid_Device * ) refcon;
        
        if( device->is_connected() )
        {
            EM_log( CK_LOG_INFO, "hid: %s disconnected", device->name );
            
            device->lock();
            device->disconnect();
            device->unlock();
            
            HidMsg msg;
            msg.device_type = device->type;
            msg.device_num = device->num;
            msg.type = CK_HID_DEVICE_DISCONNECTED;
            
            HidInManager::push_message( msg );
        }
    }
}

void Hid_poll()
{    
    CFRunLoopRunInMode( kCFRunLoopChuckHidMode, 60 * 60 * 24, false );
}

void Hid_callback( void * target, IOReturn result, 
                   void * refcon, void * sender)
{
    OSX_Hid_Device * device = ( OSX_Hid_Device * ) refcon;
    OSX_Hid_Device_Element * element;
    AbsoluteTime atZero = { 0, 0 };
    IOHIDEventStruct event;
    HidMsg msg;
    
    while( result == kIOReturnSuccess )
    {
        result = ( *( device->queue ) )->getNextEvent( device->queue, 
                                                       &event, atZero, 0 );
        if( result == kIOReturnUnderrun )
            break;
        if( result != kIOReturnSuccess )
        {
            EM_log( CK_LOG_INFO, "hid: warning: getNextEvent failed" );
            break;
        }
        
        element = ( *( device->elements ) )[event.elementCookie];
        
        msg.clear();
        msg.device_type = device->type;
        msg.device_num = device->num;
        msg.type = element->type;
        msg.eid = element->num;
        
        switch( msg.type )
        {
            case CK_HID_JOYSTICK_AXIS:
                // scale the value to [-1.0, 1.0]
                msg.fdata[0] = ((t_CKFLOAT)(event.value - element->min)) * 2.0 / ((t_CKFLOAT)(element->max - element->min)) - 1.0;
                break;
                
            case CK_HID_JOYSTICK_HAT:
                msg.idata[0] = event.value;
                break;
                
            case CK_HID_MOUSE_MOTION:
                msg.eid = 0;
                if( element->usage == kHIDUsage_GD_X )
                {
                    msg.idata[0] = event.value;
                    msg.idata[1] = 0;
                }
                
                else
                {
                    msg.idata[0] = 0;
                    msg.idata[1] = event.value;
                }
                
#ifndef __CK_HID_CURSOR_TRACK__
                Point p;
                GetGlobalMouse( &p );
                
                msg.idata[2] = p.h;
                msg.idata[3] = p.v;
                
                CGDirectDisplayID display;
                CGDisplayCount displayCount;
                
                CGPoint cgp;
                cgp.x = p.h;
                cgp.y = p.v;
                
                if( CGGetDisplaysWithPoint( cgp, 1, &display, &displayCount ) ==
                    kCGErrorSuccess )
                {
                    CGRect bounds = CGDisplayBounds( display );
                    
                    msg.fdata[0] = ( ( t_CKFLOAT ) ( p.h - bounds.origin.x ) ) /
                                   ( bounds.size.width - 1 );
                    msg.fdata[1] = ( ( t_CKFLOAT ) ( p.v - bounds.origin.y ) ) /
                                   ( bounds.size.height - 1 );
                }
#else
                msg.idata[2] = cursorX;
                msg.idata[3] = cursorY;
                msg.fdata[0] = scaledCursorX;
                msg.fdata[1] = scaledCursorY;
#endif // __CK_HID_CURSOR_TRACK__
                
                break;
                
            case CK_HID_MOUSE_WHEEL:
                msg.eid = 0;
                
                if( element->num == 1 ) // "X" wheel motion
                {
                    msg.idata[0] = event.value;
                    msg.idata[1] = 0;
                }
                    
                else // "Y" wheel motion - the default for single wheel systems
                {
                    msg.idata[0] = 0;
                    msg.idata[1] = event.value;
                }
                
                break;
                
            case CK_HID_BUTTON_DOWN:
                if( event.value == 0 )
                    msg.type = CK_HID_BUTTON_UP;
                
                msg.idata[0] = event.value;
                
                if( msg.device_type == CK_HID_DEV_KEYBOARD )
                {
                    msg.eid = element->usage;
                    msg.idata[1] = element->usage;
                    msg.idata[2] = Hid_hwkey_to_ascii( element->usage );
                }
                
                break;
        }
        
        HidInManager::push_message( msg );
    }
}

static void Hid_do_operation( void * info )
{
    OSX_Hid_op op;
    
    while( hid_operation_buffer->get( &op, 1 ) )
    {
        
        OSX_Hid_Device * device = NULL;
        
        switch( op.type )
        {
            case OSX_Hid_op::open:
                if( op.index >= op.v->size() )
                {
                    EM_log( CK_LOG_SEVERE, "hid: error: no such device %i", op.index );
                    return;
                }
                
                device = op.v->at( op.index );
                
                if( device->refcount == 0 )
                    device->start();
                    
                    device->refcount++;
                
                break;
                
            case OSX_Hid_op::close:
                if( op.index >= op.v->size() )
                {
                    EM_log( CK_LOG_SEVERE, "hid: error: no such device %i", op.index );
                    return;
                }
                
                device = op.v->at( op.index );
                
                device->refcount--;
                
                if( device->refcount == 0 )
                    device->stop();
                    
                    break;
        }
    }
}

void Hid_quit()
{
    // stop the run loop; since thread_going is FALSE, the poll thread will exit
    if( rlHid )
        CFRunLoopStop( rlHid );

    CFRunLoopRemoveSource( rlHid, hidOpSource, kCFRunLoopChuckHidMode );
    CFRelease( hidOpSource );
    
    rlHid = NULL;
    
#ifdef __CK_HID_CURSOR_TRACK__
    Mouse_stop_cursor_track();
#endif // __CK_HID_CURSOR_TRACK__
}

void Hid_quit2()
{
    if( g_hid_init == FALSE )
        return;
    g_hid_init = FALSE;
    
    delete hid_operation_buffer;
    
    xvector< OSX_Hid_Device * >::size_type i, len = joysticks->size();
    for( i = 0; i < len; i++ )
        delete joysticks->at( i );
    
    delete joysticks;
    joysticks = NULL;
    
    // TODO: delete keyboard, mouse vectors, xmultimaps
}

int Hid_count( xvector< OSX_Hid_Device * > * v )
{
    if( v == NULL )
        return 0;
    
    v->lock();
    
    int count = v->size();
    
    v->unlock();
    
    return count;
}

int Hid_open( xvector< OSX_Hid_Device * > * v, int i )
{
    if( v == NULL || i < 0 )
        return -1;
    
    v->lock();
    
    if( i >= v->size() )
    {
        v->unlock();
        return -1;
    }
    
    v->unlock();
    
    OSX_Hid_op op;
    op.type = OSX_Hid_op::open;
    op.v = v;
    op.index = i;
    
    hid_operation_buffer->put( &op, 1 );
    
    if( hidOpSource && rlHid )
    {
        CFRunLoopSourceSignal( hidOpSource );
        CFRunLoopWakeUp( rlHid );
    }
    
    return 0;
}

int Hid_close( xvector< OSX_Hid_Device * > * v, int i )
{
    if( v == NULL || i < 0 )
        return -1;
    
    v->lock();
    
    if( i >= v->size() )
    {
        v->unlock();
        return -1;
    }
    
    v->unlock();
    
    OSX_Hid_op op;
    op.type = OSX_Hid_op::close;
    op.v = v;
    op.index = i;
    
    hid_operation_buffer->put( &op, 1 );
    
    if( hidOpSource && rlHid )
    {
        CFRunLoopSourceSignal( hidOpSource );
        CFRunLoopWakeUp( rlHid );
    }        
    
    return 0;
}

const char * Hid_name( xvector< OSX_Hid_Device * > * v, int i )
{
    if( v == NULL || i < 0 )
        return NULL;
    
    v->lock();
    
    if( i >= v->size() )
    {
        v->unlock();
        return NULL;
    }
    
    OSX_Hid_Device * device = v->at( i );
    
    v->unlock();
    
    device->lock();
    
    const char * name = device->name;
    
    device->unlock();
    
    return name;
}

#pragma mark OS X Joystick support

void Joystick_init()
{
    Hid_init2();
}

void Joystick_poll()
{
}

void Joystick_quit()
{
    Hid_quit2();
}

int Joystick_count()
{
    return Hid_count( joysticks );
}

int Joystick_open( int js )
{
    return Hid_open( joysticks, js );
}

int Joystick_close( int js )
{
    return Hid_close( joysticks, js );
}

const char * Joystick_name( int js )
{
    return Hid_name( joysticks, js );
}

#pragma mark OS X Mouse support

void Mouse_init()
{
    Hid_init2();
}

void Mouse_poll()
{
}

void Mouse_quit()
{
    Hid_quit2();
}

int Mouse_count()
{
    return Hid_count( mice );
}

int Mouse_open( int m )
{
    return Hid_open( mice, m );
}

int Mouse_close( int m )
{
    return Hid_close( mice, m );
}

const char * Mouse_name( int m )
{
    return Hid_name( mice, m );
}

#ifdef __CK_HID_CURSOR_TRACK__
CGPoint CGEventGetLocation(CGEventRef event);

CGEventRef Mouse_cursor_track_cb( CGEventTapProxy proxy, CGEventType type, 
                                  CGEventRef event, void * refcon )
{
    CGPoint p = CGEventGetLocation( event );
    
    cursorX = ( t_CKINT ) p.x;
    cursorY = ( t_CKINT ) p.y;
    
    CGDirectDisplayID display;
    CGDisplayCount displayCount;
    
    if( CGGetDisplaysWithPoint( p, 1, &display, &displayCount ) ==
        kCGErrorSuccess )
    {
        CGRect bounds = CGDisplayBounds( display );
        
        scaledCursorX = ( ( t_CKFLOAT ) ( p.x - bounds.origin.x ) ) /
            ( bounds.size.width - 1 );
        scaledCursorY = ( ( t_CKFLOAT ) ( p.y - bounds.origin.y ) ) /
            ( bounds.size.height - 1 );
    }
    
    return event;
}

void * Mouse_cursor_track( void * )
{
    EM_log( CK_LOG_INFO, "hid: starting cursor track" );
    
    cursorX = 0;
    cursorY = 0;

    rlCursorTrack = CFRunLoopGetCurrent();
    
    CFMachPortRef machPort = CGEventTapCreate( kCGSessionEventTap,
                                               kCGHeadInsertEventTap,
                                               kCGEventTapOptionListenOnly,
                                               CGEventMaskBit( kCGEventMouseMoved ),
                                               Mouse_cursor_track_cb, NULL );
    CFRunLoopSourceRef tapSource;
    if( machPort != NULL )
    {
        tapSource = CFMachPortCreateRunLoopSource( NULL, machPort, 0 );
        CFRunLoopAddSource( rlCursorTrack, tapSource, kCFRunLoopChuckHidMode );
    }
    
    else
    {
        EM_log( CK_LOG_WARNING, "hid: cursor position listener startup failed" );
        return 0;
    }
    
    while( g_ct_go )
        CFRunLoopRunInMode( kCFRunLoopChuckHidMode, 60 * 60 * 24, FALSE );
    
    CFRelease( machPort );
    CFRelease( tapSource );
    rlCursorTrack = NULL;
    
    EM_log( CK_LOG_INFO, "hid: stopping cursor track" );
    
    return 0;
}

#endif // __CK_HID_CURSOR_TRACK__

int Mouse_start_cursor_track()
{
#ifdef __CK_HID_CURSOR_TRACK__
    if( &CGEventTapCreate == NULL )
        return -1;
    
    if( g_ct_go )
        return 0;
    
    g_ct_go = TRUE;
    pthread_t ct_thread;
    
    if( pthread_create( &ct_thread, NULL, Mouse_cursor_track, NULL ) != 0 )
    {
        EM_log( CK_LOG_WARNING, "hid: cursor track thread failed to start" );
        return -1;
    }
    
    return 0;
#else
    return -1;
#endif // __CK_HID_CURSOR_TRACK__
}

int Mouse_stop_cursor_track()
{
#ifdef __CK_HID_CURSOR_TRACK__
    if( g_ct_go )
    {
        g_ct_go = FALSE;
        
        if( rlCursorTrack )
            CFRunLoopStop( rlCursorTrack );
        
        cursorX = 0;
        cursorY = 0;
    }
#endif // __CK_HID_CURSOR_TRACK__
    return 0;
}

//#endif /* __CK_HID_CURSORTRACK__ */

#pragma mark OS X Keyboard support
void Keyboard_init()
{
    Hid_init2();
}

void Keyboard_poll()
{
    
}

void Keyboard_quit()
{
    Hid_quit2();
}

int Keyboard_count()
{
    return Hid_count( keyboards );
}

int Keyboard_open( int k )
{
    return Hid_open( keyboards, k );
}

int Keyboard_close( int k )
{
    return Hid_close( keyboards, k );
}

int Keyboard_send( int k, const HidMsg * msg )
{
    if( keyboards == NULL || k < 0 || k >= keyboards->size() )
        return -1;
    
    OSX_Hid_Device * keyboard = ( *keyboards )[k];
    
    if( keyboard->outputs == NULL || // no outputs
        keyboard->outputs->find( msg->type ) == keyboard->outputs->end() )
        // no outputs of that type
        return -1;
    
    if( msg->eid < 0 || msg->eid >= ( *( keyboard->outputs ) )[msg->type]->size() )
        // invalid output type element number
        return -1;
    
    OSX_Hid_Device_Element * output = ( *( *( keyboard->outputs ) )[msg->type] )[msg->eid];
    
    IOHIDEventStruct eventStruct;
    
    eventStruct.type = kIOHIDElementTypeOutput;
    eventStruct.elementCookie = output->cookie;
    eventStruct.timestamp = UpTime();
    eventStruct.longValueSize = 0;
    eventStruct.longValue = NULL;
    
    if( msg->idata[0] < output->min )
        eventStruct.value = output->min;
    else if( msg->idata[0] > output->max )
        eventStruct.value = output->max;
    else
        eventStruct.value = msg->idata[0];
    
    IOReturn result;
    result = ( *( keyboard->interface ) )->setElementValue( keyboard->interface, 
                                                            output->cookie, 
                                                            &eventStruct, 
                                                            0, 0, 0, 0 );
    if( result != kIOReturnSuccess )
        return -1;
    
    return 0;
}

const char * Keyboard_name( int k )
{
    return Hid_name( keyboards, k );
}

#pragma mark OS X Tilt/Sudden Motion Sensor support

enum
{
    kSMSPowerbookDataType,
    kSMSMacBookProDataType
};

static struct t_TiltSensor_data
{
    union
    {
        struct t_powerbook
        {
            int8_t x;
            int8_t y;
            int8_t z;
            int8_t pad[57];
        } powerbook;
        
        struct t_macbookpro
        {
            int16_t x;
            int16_t y;
            int16_t z;
            int8_t pad[34];
        } macbookpro;
    } data;
    
    int kernFunc;
    char * servMatch;
    int dataType;
    io_connect_t dataPort;
    
    int detected; // 0 = detection not yet run, -1 = no sensor found, 1 = sensor found
    int refcount;
    
    t_TiltSensor_data()
    {
        refcount = 0;
        kernFunc = 0;
        servMatch = NULL;
        dataType = -1;
        dataPort = 0;
        detected = 0;
    }
    
} TiltSensor_data;

static int TiltSensor_test( int kernFunc, char * servMatch, int dataType )
{
    kern_return_t result;
    io_iterator_t iterator;
    io_object_t aDevice;
    io_connect_t dataPort;
    
    IOItemCount structureInputSize;
    IOByteCount structureOutputSize;
        
    CFMutableDictionaryRef matchingDictionary = IOServiceMatching( servMatch );
    
    result = IOServiceGetMatchingServices( kIOMasterPortDefault, matchingDictionary, &iterator );
    
    if( result != KERN_SUCCESS )
        return 0;
    
    aDevice = IOIteratorNext( iterator );
    IOObjectRelease( iterator );
    
    if( aDevice == 0 )
        return 0;
    
    result = IOServiceOpen( aDevice, mach_task_self(), 0, &dataPort );
    IOObjectRelease( aDevice );
    
    if ( result != KERN_SUCCESS )
        return 0;
    
    switch( dataType )
    {
        case kSMSPowerbookDataType:
            structureInputSize = sizeof( TiltSensor_data.data.powerbook );
            structureOutputSize = sizeof( TiltSensor_data.data.powerbook );
            break;
            
        case kSMSMacBookProDataType:
            structureInputSize = sizeof( TiltSensor_data.data.macbookpro );
            structureOutputSize = sizeof( TiltSensor_data.data.macbookpro );
            break;
            
        default:
            IOServiceClose( dataPort );
            return 0;
    }
    
    memset( &TiltSensor_data.data, 0, sizeof( TiltSensor_data.data ) );
    memset( &TiltSensor_data.data, 0, sizeof( TiltSensor_data.data ) );
    
    result = IOConnectMethodStructureIStructureO( dataPort, 
                                                  kernFunc, 
                                                  structureInputSize,
                                                  &structureOutputSize, 
                                                  &TiltSensor_data.data, 
                                                  &TiltSensor_data.data );
    
    if ( result != KERN_SUCCESS )
    {
        IOServiceClose( dataPort );
        return 0;
    }
    
    // leave dataPort open for future use
    TiltSensor_data.dataPort = dataPort;

    return 1;
}

static int TiltSensor_do_read()
{
    kern_return_t result;
    IOItemCount structureInputSize;
    IOByteCount structureOutputSize;

    switch( TiltSensor_data.dataType )
    {
        case kSMSPowerbookDataType:
            structureInputSize = sizeof( TiltSensor_data.data.powerbook );
            structureOutputSize = sizeof( TiltSensor_data.data.powerbook );
            break;
            
        case kSMSMacBookProDataType:
            structureInputSize = sizeof( TiltSensor_data.data.macbookpro );
            structureOutputSize = sizeof( TiltSensor_data.data.macbookpro );
            break;
            
        default:
            return 0;
    }
    
    memset( &TiltSensor_data.data, 0, sizeof( TiltSensor_data.data ) );
    memset( &TiltSensor_data.data, 0, sizeof( TiltSensor_data.data ) );
    
    result = IOConnectMethodStructureIStructureO( TiltSensor_data.dataPort, 
                                                  TiltSensor_data.kernFunc, 
                                                  structureInputSize,
                                                  &structureOutputSize, 
                                                  &TiltSensor_data.data, 
                                                  &TiltSensor_data.data );
    
    return 1;
}

static int TiltSensor_detect()
{
    // try different interfaces until we find one that works
    
    SInt32 osx_version;
    int powerbookKernFunc = 0;
    
    Gestalt( gestaltSystemVersion, &osx_version );
    
    if( osx_version == 0x1040 || 
        osx_version == 0x1041 ||
        osx_version == 0x1042 ||
        osx_version == 0x1043 )
        powerbookKernFunc = 24;
    
    else
        powerbookKernFunc = 21;
    
    // ibook/powerbook (OS X 10.4.x) tilt sensor interface
    if( TiltSensor_test( powerbookKernFunc, "IOI2CMotionSensor", kSMSPowerbookDataType ) )
    {
        TiltSensor_data.kernFunc = powerbookKernFunc;
        TiltSensor_data.dataType = kSMSPowerbookDataType;
        TiltSensor_data.detected = 1;
        return 1;
    }
    
    // hi resolution powerbook tilt sensor interface
    if( TiltSensor_test( powerbookKernFunc, "PMUMotionSensor", kSMSPowerbookDataType ) )
    {
        TiltSensor_data.kernFunc = powerbookKernFunc;
        TiltSensor_data.dataType = kSMSPowerbookDataType;
        TiltSensor_data.detected = 1;
        return 1;
    }
    
    // mac book (pro) tilt sensor interface
    if( TiltSensor_test( 5, "SMCMotionSensor", kSMSMacBookProDataType ) )
    {
        TiltSensor_data.kernFunc = 5;
        TiltSensor_data.dataType = kSMSMacBookProDataType;
        TiltSensor_data.detected = 1;
        return 1;
    }
    
    TiltSensor_data.detected = -1;
    
    return 0;
}

void TiltSensor_init()
{
    
}

void TiltSensor_quit()
{
    if( TiltSensor_data.dataPort == 0 )
        IOServiceClose( TiltSensor_data.dataPort );
}

void TiltSensor_probe()
{
    
}

int TiltSensor_count()
{
    if( TiltSensor_data.detected == 0 )
        TiltSensor_detect();

    if( TiltSensor_data.detected == -1 )
        return 0;
    
    else if( TiltSensor_data.detected == 1 )
        return 1;
    
    return 0;
}

int TiltSensor_open( int ts )
{
    if( TiltSensor_data.detected == 0 )
        TiltSensor_detect();
    
    if( TiltSensor_data.detected == -1 )
        return -1;
    
    TiltSensor_data.refcount++;
    
    return 0;
}

int TiltSensor_close( int ts )
{
    TiltSensor_data.refcount--;
    
    return 0;
}

const char * TiltSensor_name( int ts )
{
    return "Apple Sudden Motion Sensor";
}

int TiltSensor_read( int ts, int type, int num, HidMsg * msg )
{
    if( type != CK_HID_ACCELEROMETER )
        return -1;
    
    if( TiltSensor_data.detected == -1 )
        return -1;
    
    if( !TiltSensor_do_read() )
        return -1;
    
    if( TiltSensor_data.dataType == kSMSPowerbookDataType )
    {
        msg->idata[0] = TiltSensor_data.data.powerbook.x;
        msg->idata[1] = TiltSensor_data.data.powerbook.y;
        msg->idata[2] = TiltSensor_data.data.powerbook.z;
    }
    
    else if( TiltSensor_data.dataType == kSMSMacBookProDataType )
    {
        msg->idata[0] = TiltSensor_data.data.macbookpro.x;
        msg->idata[1] = TiltSensor_data.data.macbookpro.y;
        msg->idata[2] = TiltSensor_data.data.macbookpro.z;
    }
    
    return 0;
}

#ifdef __CK_HID_WIIREMOTE__
#pragma mark OS X Wii Remote support

class Bluetooth_Device : public lockable
{
public:
    Bluetooth_Device()
    {
        device = NULL;
        memset( &address, 0, sizeof( address ) );
        strncpy( name, "Bluetooth Device", 256 );
        interrupt_channel = NULL;
        control_channel = NULL;
        disconnect_notification = NULL;
        
        type = 0;
        num = -1;
        
        refcount = 0;
    }
    
    virtual ~Bluetooth_Device()
    {
        close();
    }
    
    virtual t_CKINT open() { return -1; }
    virtual t_CKINT connect() { return -1; }
    virtual t_CKINT control_init() { return -1; }
    virtual t_CKINT interrupt_init() { return -1; }
    virtual t_CKINT disconnect() { return -1; }
    virtual t_CKINT close() { return -1; }
    virtual t_CKBOOL is_connected() { return FALSE; }
    
    virtual void control_receive( void * data, size_t size ) {};
    virtual void interrupt_receive( void * data, size_t size ) {};
    
    virtual void control_send( void * data, size_t size ) {};
    virtual void interrupt_send( void * data, size_t size ) {};
    
    IOBluetoothDeviceRef device;
    BluetoothDeviceAddress address;
    IOBluetoothL2CAPChannelRef interrupt_channel;
    IOBluetoothL2CAPChannelRef control_channel;
    IOBluetoothUserNotificationRef disconnect_notification;
    char name[256];
    
    t_CKUINT type;
    t_CKINT num;
    t_CKUINT refcount;
};

class WiiRemote : public Bluetooth_Device
{
public:
    WiiRemote()
    {
        force_feedback_enabled = FALSE;
        motion_sensor_enabled = FALSE;
        ir_sensor_enabled = FALSE;
        led1 = led2 = led3 = led4 = FALSE;
        memset( &buttons, 0, sizeof( buttons ) );
        memset( &accels, 0, sizeof( accels ) );
        memset( &ir, 0xff, sizeof( ir ) );
    }
    
    virtual t_CKINT open();
    virtual t_CKINT connect();
    virtual t_CKINT control_init();
    virtual t_CKINT disconnect();
    virtual t_CKINT close();
    virtual t_CKBOOL is_connected();
    
    virtual void control_receive( void * data, size_t size );
    virtual void interrupt_receive( void * data, size_t size );
    
    virtual void control_send( const void * data, size_t size );
    virtual void write_memory( const void * data, size_t size, unsigned long address );
    
    virtual void enable_force_feedback( t_CKBOOL enable );
    virtual void enable_motion_sensor( t_CKBOOL enable );
    virtual void enable_ir_sensor( t_CKBOOL enable );
    virtual void enable_leds( t_CKBOOL l1, t_CKBOOL l2, 
                              t_CKBOOL l3, t_CKBOOL l4 );
    virtual void set_led( t_CKUINT led, t_CKBOOL state );
    
    t_CKBOOL force_feedback_enabled;
    t_CKBOOL motion_sensor_enabled;
    t_CKBOOL ir_sensor_enabled;
    t_CKBOOL led1, led2, led3, led4;
    
    t_CKBYTE buttons[2];
    t_CKBYTE accels[3];
    t_CKBYTE ir[12];

    enum
    {
        ButtonHome = 0,
        Button1,
        Button2,
        ButtonPlus,
        ButtonMinus,
        ButtonA,
        ButtonB,
        ButtonUp,
        ButtonRight,
        ButtonDown,
        ButtonLeft
    };
};

bool operator< ( BluetoothDeviceAddress a, BluetoothDeviceAddress b )
{
    for( int i = 0; i < 6; i++ )
    {
        if( a.data[i] < b.data[i] )
            return true;
        if( a.data[i] > b.data[i] )
            return false;
    }

    return false;
}

static xvector< WiiRemote * > * wiiremotes = NULL;

/* the user can open a wiimote with any id number he chooses without an error,
and will then be sent the appropriate message when that wiimote is connected.  
The nth wiimote is simply the nth wiimote that chuck hid detects, and since 
there is absolutely no way to determine how many wiimotes will connect in 
advance, this number must be left unbounded.  To support this, WiiRemote_open
will add empty WiiRemotes to wiiremotes with refcount of 1 if it the wiimote
its opening isnt there yet, and hope it will be opened at some future time.  
Thus when discovering wiimotes we have to distinguish between wiimotes that have
been discovered and wiimotes that have been opened but have not yet been 
discovered.  all wiimotes with indices less than g_wr_next_real_wiimote have 
actually been discovered, and anything at or above that index in wiiremotes has
not actually been discovered, but (if its non-NULL) has been opened.  */
static xvector< WiiRemote * >::size_type g_next_real_wiimote = 0;

static map< BluetoothDeviceAddress, WiiRemote * > * wr_addresses = NULL;
static t_CKBOOL g_bt_query_active = FALSE; // is a query currently active?

void Bluetooth_device_connected( void * userRefCon, 
                                 IOBluetoothDeviceRef deviceRef,
                                 IOReturn status )
{
    if( status == noErr )
        ( ( Bluetooth_Device * ) userRefCon )->connect();
    else
        EM_log( CK_LOG_WARNING, "hid: error: opening Wii Remote Controller connection" );
}

void Bluetooth_device_control_event( IOBluetoothL2CAPChannelRef l2capChannel,
                                     void * refCon,
                                     IOBluetoothL2CAPChannelEvent * event )
{
    switch( event->eventType )
    {
        case kIOBluetoothL2CAPChannelEventTypeOpenComplete:
            ( ( Bluetooth_Device * ) refCon )->control_init();
            break;
            
        case kIOBluetoothL2CAPChannelEventTypeData:
            ( ( Bluetooth_Device * ) refCon )->control_receive( event->u.data.dataPtr, 
                                                                event->u.data.dataSize );
            break;
            
        case kIOBluetoothL2CAPChannelEventTypeWriteComplete:
            if( event->status != noErr )
            {
                EM_log( CK_LOG_WARNING, "hid: error: writing data to control L2CAP channel for '%s'", 
                        ( ( Bluetooth_Device * ) refCon )->name );
            }
            
            break;
            
        case kIOBluetoothL2CAPChannelEventTypeClosed:
            EM_log( CK_LOG_FINE, "hid: error: control L2CAP channel for '%s' closed",
                    ( ( Bluetooth_Device * ) refCon )->name );
            break;
    }        
}

void Bluetooth_device_interrupt_event( IOBluetoothL2CAPChannelRef l2capChannel,
                                       void * refCon,
                                       IOBluetoothL2CAPChannelEvent * event )
{
    switch( event->eventType )
    {
        case kIOBluetoothL2CAPChannelEventTypeData:
            ( ( Bluetooth_Device * ) refCon )->control_receive( event->u.data.dataPtr, 
                                                                event->u.data.dataSize );
            break;
    }        
}

void Bluetooth_device_disconnected( void * userRefCon, 
                                    IOBluetoothUserNotificationRef inRef, 
                                    IOBluetoothObjectRef objectRef )
{
    ( ( Bluetooth_Device * ) userRefCon )->disconnect();
}

t_CKINT WiiRemote::open()
{
    // see if its already connected, reconnect if so
    if( IOBluetoothDeviceIsConnected( device ) )
    {
        if( IOBluetoothDeviceCloseConnection( device ) != noErr )
        {
            EM_log( CK_LOG_WARNING, "hid: error: closing Wii Remote Controller connection" );
            return -1;
        }
    }
    
    if( IOBluetoothDeviceOpenConnection( device, Bluetooth_device_connected,
                                         this ) != noErr)
    {
        EM_log( CK_LOG_WARNING, "hid: error: opening Wii Remote Controller connection" );
        return -1;
    }
    
    return 0;
}

t_CKINT WiiRemote::connect()
{    
    disconnect_notification = IOBluetoothDeviceRegisterForDisconnectNotification( device,
                                                                                  Bluetooth_device_disconnected,
                                                                                  this );
    
    if( disconnect_notification == NULL )
    {
        EM_log( CK_LOG_WARNING, "hid: error: registering for Wii Remote Controller disconnection notification" );
        return -1;
    }
    
    if( IOBluetoothDeviceOpenL2CAPChannelAsync( device, &control_channel, 17, 
                                                Bluetooth_device_control_event, 
                                                this ) != kIOReturnSuccess )
    {
        EM_log( CK_LOG_WARNING, "hid: error: opening Wii Remote Controller L2CAP connection" );
        return -1;
    }
    fprintf( stderr, "l2cap control channel ref: 0x%x\n", control_channel );
    if( IOBluetoothDeviceOpenL2CAPChannelAsync( device, &interrupt_channel, 19, 
                                                Bluetooth_device_interrupt_event, 
                                                this ) != kIOReturnSuccess )
    {
        EM_log( CK_LOG_WARNING, "hid: error: opening Wii Remote Controller L2CAP connection" );
        return -1;
    }
        
    return 0;
}

t_CKINT WiiRemote::control_init()
{
    enable_motion_sensor( TRUE );
    enable_ir_sensor( TRUE );
    enable_force_feedback( FALSE );
    enable_leds( ( num % 4 ) == 0, ( ( num - 1 ) % 4 ) == 0, 
                 ( ( num - 2 ) % 4 ) == 0, ( ( num - 3 ) % 4 ) == 0 );
    
    HidMsg msg;
    
    msg.device_num = num;
    msg.device_type = CK_HID_DEV_WIIREMOTE;
    msg.type = CK_HID_DEVICE_CONNECTED;
    
    HidInManager::push_message( msg );    
    
    return 0;
}

t_CKINT WiiRemote::disconnect()
{
    if( interrupt_channel )
    {
        IOBluetoothL2CAPChannelCloseChannel( interrupt_channel );
        IOBluetoothObjectRelease( interrupt_channel );
        interrupt_channel = NULL;
    }
    
    if( control_channel )
    {
        IOBluetoothL2CAPChannelCloseChannel( control_channel );
        IOBluetoothObjectRelease( control_channel );
        control_channel = NULL;
    }
    
    if( device )
    {
        IOBluetoothDeviceCloseConnection( device );
        IOBluetoothObjectRelease( device );
        device = NULL;
    }
    
    if( disconnect_notification )
    {
        IOBluetoothUserNotificationUnregister( disconnect_notification );
        disconnect_notification = NULL;
    }
    
    HidMsg msg;
    
    msg.device_num = num;
    msg.device_type = CK_HID_DEV_WIIREMOTE;
    msg.type = CK_HID_DEVICE_DISCONNECTED;
    
    HidInManager::push_message( msg );    
    
    return 0;
}

t_CKINT WiiRemote::close()
{
    if( is_connected() )
        disconnect();
    return 0;
}

t_CKBOOL WiiRemote::is_connected()
{
    if( device == NULL )
        return FALSE;
    return TRUE;
}

void WiiRemote::control_receive( void * data, size_t size )
{
    unsigned char * d = ( unsigned char * ) data;
    HidMsg msg;
    
    if( ( d[1] & 0xf0 ) == 0x30 )
    {
        /* buttons */
        
        if( ( d[3] & 0x80 ) ^ ( buttons[1] & 0x80 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[3] & 0x80 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = ButtonHome;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        if( ( d[3] & 0x02 ) ^ ( buttons[1] & 0x02 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[3] & 0x02 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = Button1;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        if( ( d[3] & 0x01 ) ^ ( buttons[1] & 0x01 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[3] & 0x01 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = Button2;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        if( ( d[2] & 0x10 ) ^ ( buttons[0] & 0x10 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[2] & 0x10 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = ButtonPlus;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        if( ( d[3] & 0x10 ) ^ ( buttons[1] & 0x10 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[3] & 0x10 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = ButtonMinus;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        if( ( d[3] & 0x08 ) ^ ( buttons[1] & 0x08 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[3] & 0x08 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = ButtonA;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        if( ( d[3] & 0x04 ) ^ ( buttons[1] & 0x04 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[3] & 0x04 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = ButtonB;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        if( ( d[2] & 0x08 ) ^ ( buttons[0] & 0x08 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[2] & 0x08 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = ButtonUp;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        if( ( d[2] & 0x02 ) ^ ( buttons[0] & 0x02 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[2] & 0x02 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = ButtonRight;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        if( ( d[2] & 0x04 ) ^ ( buttons[0] & 0x04 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[2] & 0x04 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = ButtonDown;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        if( ( d[2] & 0x01 ) ^ ( buttons[0] & 0x01 ) )
        {
            msg.device_num = num;
            msg.device_type = CK_HID_DEV_WIIREMOTE;
            msg.type = ( d[2] & 0x01 ) ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = ButtonLeft;
            
            HidInManager::push_message( msg );
            
            msg.clear();
        }
        
        memcpy( buttons, d + 2, sizeof( buttons ) );
        
        /* accelerometers */
        if( d[1] & 0x01 )
        {
            if( d[4] ^ accels[0] ||
                d[5] ^ accels[1] ||
                d[6] ^ accels[2] )
            {
                msg.device_num = num;
                msg.device_type = CK_HID_DEV_WIIREMOTE;
                msg.type = CK_HID_ACCELEROMETER;
                msg.eid = 0;
                msg.idata[0] = d[4];
                msg.idata[1] = d[5];
                msg.idata[2] = d[6];
                
                HidInManager::push_message( msg );
                
                msg.clear();            
            }
            
            memcpy( accels, d + 4, sizeof( accels ) );
        }
        
        /* ir sensor */
        if( d[1] & 0x02 )
        {
            unsigned i;
            
            for( i = 0; i < 4; i++ )
            {
                if( ( d[7 + i * 3] ^ ir[i * 3] ||
                      d[7 + i * 3 + 1] ^ ir[i * 3 + 1] ||
                      d[7 + i * 3 + 2] ^ ir[i * 3 + 2] ) &&
                    ( d[7 + i * 3] != 0xff ) && 
                    ( d[7 + i * 3 + 1] != 0xff ) && 
                    ( d[7 + i * 3 + 2] != 0xff ) )
                {
                    msg.device_num = num;
                    msg.device_type = CK_HID_DEV_WIIREMOTE;
                    msg.type = CK_HID_WIIREMOTE_IR;
                    msg.eid = i;
                    // x
                    msg.idata[0] = d[7 + 3 * i] + ( ( d[7 + 3 * i + 2] << 4 ) & 0x300 );
                    // y
                    msg.idata[1] = d[7 + 3 * i + 1] + ( ( d[7 + 3 * i + 2] << 2 ) & 0x300 );
                    // size
                    msg.idata[2] = d[7 + 3 * i + 2] & 0xffff;
                    
                    HidInManager::push_message( msg );
                    
                    msg.clear();
                }
            }
            
            memcpy( ir, d + 7, sizeof( ir ) );
        }
    }
}

void WiiRemote::interrupt_receive( void * data, size_t size )
{
    
}

void WiiRemote::control_send( const void * data, size_t size )
{
    assert( size <= 22 );
    
	unsigned char buf[23];
    
	memset( buf, 0, 23 );
	buf[0] = 0x52;
    memcpy( buf+1, data, size );
	
    size++;
    
    //printf( "send (%i):", size );
    //for( int i = 0; i < size; i++ )
    //    printf( " %02x", buf[i] );
    //printf( "\n" );
    fprintf( stderr, "l2cap control channel ref: 0x%x\n", control_channel );
    
    IOReturn result;
    result = IOBluetoothL2CAPChannelWriteSync( control_channel, buf, size );
    //IOReturn result = IOBluetoothL2CAPChannelWriteAsync( control_channel, buf, size, NULL );
    
    if( result != kIOReturnSuccess )
        EM_log( CK_LOG_WARNING, 
                "hid: error: sending data to Wii Remote Controller %i (error 0x%x)", 
                num, result );
}

void WiiRemote::write_memory( const void * data, size_t size, unsigned long address )
{
    assert( size <= 16 );
    
	unsigned char cmd[22];
    
    memset( cmd, 0, 22 );
    memcpy( cmd + 6, data, size );
        
    cmd[0] = 0x16;
    cmd[1] = ( address >> 24 ) & 0xff;
    cmd[2] = ( address >> 16 ) & 0xff;
    cmd[3] = ( address >> 8 ) & 0xff;
    cmd[4] = address & 0xff;
    cmd[5] = size;
    
    if( force_feedback_enabled )
        cmd[1] |= 0x01;
    
    control_send( cmd, 22 );
}

void WiiRemote::enable_force_feedback( t_CKBOOL enable )
{
    force_feedback_enabled = enable;
    
    unsigned char cmd[] = { 0x13, 0x00 };
	if( enable )
        cmd[1] |= 0x01;
	if( ir_sensor_enabled )
        cmd[1] |= 0x04;    
    
    control_send( cmd, 2 );
}

void WiiRemote::enable_motion_sensor( t_CKBOOL enable )
{
    motion_sensor_enabled = enable;
    
    unsigned char cmd[] = { 0x12, 0x00, 0x30 };
    if( motion_sensor_enabled )
        cmd[2] |= 0x01;
    if( ir_sensor_enabled )
        cmd[2] |= 0x02;
    if( force_feedback_enabled )
        cmd[1] |= 0x01;
    
    control_send( cmd, 3 );
}

void WiiRemote::enable_ir_sensor( t_CKBOOL enable )
{
    ir_sensor_enabled = enable;
    
    enable_motion_sensor( motion_sensor_enabled );
    enable_force_feedback( force_feedback_enabled );
    
    unsigned char cmd[] = { 0x1a, 0x00 };
    if( ir_sensor_enabled )
        cmd[1] |= 0x04;
    if( force_feedback_enabled )
        cmd[1] |= 0x01;
    
    control_send( cmd, 2 );
    
    if( ir_sensor_enabled )
    {
        //unsigned char en0[] = { 0x01 };
        //write_memory( en, 1, 0x04b00030 );
        //usleep(10000);

        unsigned char en[] = { 0x08 };
        write_memory( en, 1, 0x04b00030 );
        //usleep(10000);

        unsigned char sensitivity_block1[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0xc0 };
        write_memory( sensitivity_block1, 9, 0x04b00000 );
        //usleep(10000);

        unsigned char sensitivity_block2[] = { 0x40, 0x00 };
        write_memory( sensitivity_block2, 2, 0x04b0001a );
        //usleep(10000);

        unsigned char mode[] = { 0x03 };
        write_memory( mode, 1, 0x04b00033 );
       // usleep(10000);

        //unsigned char what[] = { 0x08 };
        //write_memory( what, 1, 0x04b00030 );
        //usleep(10000);

    }
}

void WiiRemote::set_led( t_CKUINT led, t_CKBOOL state )
{
    switch( led )
    {
        case 0:
            led1 = state;
            break;
            
        case 1:
            led2 = state;
            break;
            
        case 2:
            led3 = state;
            break;
            
        case 3:
            led4 = state;
            break;
    }
    
    enable_leds( led1, led2, led3, led4 );
}

void WiiRemote::enable_leds( t_CKBOOL l1, t_CKBOOL l2, 
                             t_CKBOOL l3, t_CKBOOL l4 )
{
    led1 = l1;
    led2 = l2;
    led3 = l3;
    led4 = l4;
    
	unsigned char cmd[] = { 0x11, 0x00 };
	if( force_feedback_enabled )
        cmd[1] |= 0x01;
	if( l1 )
        cmd[1] |= 0x10;
	if( l2 )
        cmd[1] |= 0x20;
	if( l3 )
        cmd[1] |= 0x40;
	if( l4 )
        cmd[1] |= 0x80;
	
	control_send( cmd, 2 );
}

void Bluetooth_inquiry_device_found( void * userRefCon,
                                     IOBluetoothDeviceInquiryRef inquiryRef, 
                                     IOBluetoothDeviceRef deviceRef )
{
    CFStringRef device_name = IOBluetoothDeviceGetName( deviceRef );
    const BluetoothDeviceAddress * address = IOBluetoothDeviceGetAddress( deviceRef );
    
    if( device_name == NULL )
        return;
    
    // is device Nintendo Wii Remote controller?
    if( CFStringCompare( device_name, CFSTR( "Nintendo RVL-CNT-01" ), 0 ) == 0 )
    {
        // has this already been detected?
        if( wr_addresses->find( *address ) == wr_addresses->end() )
        {
            WiiRemote * wr;
            
            if( g_next_real_wiimote >= wiiremotes->size() )
            {
                wr = new WiiRemote;
                wiiremotes->push_back( wr );
            }
            
            else if( ( *wiiremotes )[g_next_real_wiimote] == NULL )
            {
                wr = new WiiRemote;
                ( *wiiremotes )[g_next_real_wiimote] = wr;
            }
            
            else
            {
                wr = ( *wiiremotes )[g_next_real_wiimote];
            }
            
            // set member data
            wr->device = deviceRef;
            memcpy( &wr->address, address, sizeof( BluetoothDeviceAddress ) );
            strncpy( wr->name, "Nintendo RVL-CNT-01", 256 );
            wr->num = g_next_real_wiimote;
            wr->type = CK_HID_DEV_WIIREMOTE;
            
            ( *wr_addresses )[wr->address] = wr;
            
            EM_log( CK_LOG_INFO, "hid: found Wii Remote Controller" );
            
            if( wr->refcount )
                wr->open();
            
            g_next_real_wiimote++;
        }
        
        else if( !( *wr_addresses )[*address]->is_connected() )
        {
            WiiRemote * wr = ( *wr_addresses )[*address];
            
            if( wr->refcount )
                wr->open();
        }
    }
    
    else
    {
        EM_log( CK_LOG_INFO, "hid: found bluetooth device" );
    }
}

void Bluetooth_inquiry_complete( void * userRefCon,
                                 IOBluetoothDeviceInquiryRef inquiryRef, 
                                 IOReturn error, 
                                 Boolean aborted )
{
    g_bt_query_active = FALSE;
}

int WiiRemote_query()
{
    EM_log( CK_LOG_INFO, "hid: performing bluetooth query" );
    if( g_bt_query_active )
        return 0;
    
    if( IOBluetoothLocalDeviceAvailable() == FALSE )
    {
        EM_log( CK_LOG_WARNING, "hid: error: bluetooth unavailable" );
        return -1;
    }
    
    IOBluetoothDeviceInquiryRef btDeviceInquiry = IOBluetoothDeviceInquiryCreateWithCallbackRefCon( NULL );
        
    if( btDeviceInquiry == NULL )
    {
        EM_log( CK_LOG_WARNING, "hid: error: creating bluetooth device inquiry" );
        return -1;
    }
    
    if( IOBluetoothDeviceInquirySetDeviceFoundCallback( btDeviceInquiry,
                                                        Bluetooth_inquiry_device_found ) 
        != noErr )
    {
        EM_log( CK_LOG_WARNING, "hid: error: setting bluetooth device inquiry callback" );
        return -1;
    }
    
    if( IOBluetoothDeviceInquirySetCompleteCallback( btDeviceInquiry,
                                                     Bluetooth_inquiry_complete )
        != noErr )
    {
        EM_log( CK_LOG_WARNING, "hid: error: setting bluetooth device inquiry completion callback" );
        return -1;
    }
    
    if( IOBluetoothDeviceInquirySetUpdateNewDeviceNames( btDeviceInquiry,
                                                         TRUE )
        != noErr )
    {
        EM_log( CK_LOG_WARNING, "hid: error: setting bluetooth device inquiry name update flag" );
        return -1;
    }
    
    if( IOBluetoothDeviceInquirySetInquiryLength( btDeviceInquiry, 20 ) != noErr )
    {
        EM_log( CK_LOG_WARNING, "hid: error: setting bluetooth inquiry length" );
        return -1;
    }
    
    if( IOBluetoothDeviceInquiryStart( btDeviceInquiry ) != noErr )
    {
        EM_log( CK_LOG_WARNING, "hid: error: starting bluetooth device inquiry" );
        return -1;
    }
    
    g_bt_query_active = TRUE;
    
    return 0;
}

/* -- WiiRemote inter-thread communication -- 
Every IOBluetooth function needs to be called from the HID thread because none 
of it is thread safe, and it also requires a CFRunLoop to be running at some 
point.  So, WiiRemote_open and _close, which are called from the VM thread, put 
open and close messages into a thread-safe circular buffer and then signal the 
hid thread to indicate that new info is available on the cbuf.  The hid thread
will then process any pending open/close messages in the cbuf.  

send messages now also are carried out in the hid thread through this mechanism.  
*/

struct WiiRemoteOp
{
    enum 
    {
        open,
        close,
        send
    } op;
    
    int index;
};

static CBufferSimple * WiiRemoteOp_cbuf = NULL;
static CBufferSimple * WiiRemoteOp_msgbuf = NULL;

static void WiiRemote_cfrl_callback( void * info )
{
    WiiRemoteOp wro;
    HidMsg msg;
    t_CKBOOL do_query = FALSE;
    
    while( WiiRemoteOp_cbuf->get( &wro, 1 ) )
    {
        switch( wro.op )
        {
            case WiiRemoteOp::open:
                // does it exist already?
                if( wro.index < wiiremotes->size() )
                    // yes
                {
                    // is the device connected?
                    if( !( *wiiremotes )[wro.index]->is_connected() )
                        // no, so do a query
                        do_query = TRUE;
                }
                
                else
                    // no, so create it
                {
                    while( wro.index > wiiremotes->size() )
                        wiiremotes->push_back( NULL );
                    wiiremotes->push_back( new WiiRemote );
                    
                    do_query = TRUE;
                }
            
                ( *wiiremotes )[wro.index]->refcount++;
                
                break;
        
            case WiiRemoteOp::close:
                if( wro.index < wiiremotes->size() )
                {
                    if( --( *wiiremotes )[wro.index]->refcount == 0 )
                        ( *wiiremotes )[wro.index]->close();
                }
                
                break;
                
            case WiiRemoteOp::send:
                if( WiiRemoteOp_msgbuf->get( &msg, 1 ) )
                {
                    // double-check remote number for validity
                    if( wro.index < 0 || wro.index >= wiiremotes->size() ||
                        ( *wiiremotes )[wro.index] == NULL || 
                        !( *wiiremotes )[wro.index]->is_connected() )
                    {
                        // report failure
                        HidInManager::push_message( msg );
                        break;
                    }
                    
                    switch( msg.type )
                    {
                        case CK_HID_LED:
                            if( msg.eid < 0 || msg.eid >= 4 )
                            {
                                // report failure
                                HidInManager::push_message( msg );
                                break;
                            }
                            
                            ( *wiiremotes )[wro.index]->set_led( msg.eid, msg.idata[0] );
                            
                            break;
                            
                        /*case CK_HID_FORCE_FEEDBACK:
                            if( msg.*/
                    }
                }
                
                break;
        }
    }
    
    if( do_query )
        WiiRemote_query();
}

static void WiiRemote_signal()
{
    if( cfrlWiiRemoteSource && rlHid )
    {
        CFRunLoopSourceSignal( cfrlWiiRemoteSource );
        CFRunLoopWakeUp( rlHid );
    }
}

#endif // __CK_HID_WIIREMOTE

void WiiRemote_init()
{
#ifdef __CK_HID_WIIREMOTE__
    Hid_init2();
    
    wiiremotes = new xvector< WiiRemote * >;
    wr_addresses = new map< BluetoothDeviceAddress, WiiRemote * >;
    
    WiiRemoteOp_cbuf = new CBufferSimple;
    WiiRemoteOp_cbuf->initialize( 20, sizeof( WiiRemoteOp ) );
    
    WiiRemoteOp_msgbuf = new CBufferSimple;
    WiiRemoteOp_msgbuf->initialize( 100, sizeof( HidMsg ) );
#endif // __CK_HID_WIIREMOTE
}

void WiiRemote_poll()
{
}

void WiiRemote_quit()
{
#ifdef __CK_HID_WIIREMOTE__
    for( xvector< WiiRemote * >::size_type i = 0; i < wiiremotes->size(); i++ )
    {
        if( ( *wiiremotes )[i] )
            SAFE_DELETE( ( *wiiremotes )[i] );
    }
    SAFE_DELETE( wiiremotes );
    SAFE_DELETE( wr_addresses );
    SAFE_DELETE( WiiRemoteOp_cbuf );
    
    Hid_quit2();
#endif // __CK_HID_WIIREMOTE__
}

int WiiRemote_count()
{    
#ifdef __CK_HID_WIIREMOTE__
    return wiiremotes->size();
#else
    return 0;
#endif // __CK_HID_WIIREMOTE__
}

int WiiRemote_open( int wr )
{
#ifdef __CK_HID_WIIREMOTE__
    WiiRemoteOp wro;
    wro.op = WiiRemoteOp::open;
    wro.index = wr;
    
    WiiRemoteOp_cbuf->put( &wro, 1 );
    
    WiiRemote_signal();
    
    return 0;
#else
    return -1;
#endif // __CK_HID_WIIREMOTE__
}

int WiiRemote_close( int wr )
{
#ifdef __CK_HID_WIIREMOTE__
    WiiRemoteOp wro;
    wro.op = WiiRemoteOp::close;
    wro.index = wr;
    
    WiiRemoteOp_cbuf->put( &wro, 1 );
    
    WiiRemote_signal();
    
    return 0;
#else
    return -1;
#endif // __CK_HID_WIIREMOTE__
}

int WiiRemote_send( int wr, const HidMsg * msg )
{
#ifdef __CK_HID_WIIREMOTE__
    wiiremotes->lock();
    
    if( wr < 0 || wr >= wiiremotes->size() )
    {
        wiiremotes->unlock();
        return -1;
    }
    
    WiiRemote * wiiremote = ( *wiiremotes )[wr];
    
    wiiremotes->unlock();
    
    if( wiiremote == NULL )
        return -1;
        
    
    wiiremote->lock();
    
    if( !wiiremote->is_connected() )
    {
        wiiremote->unlock();
        return -1;
    }
    
    wiiremote->unlock();
    
    WiiRemoteOp_msgbuf->put( ( void * )msg, 1 );
    
    WiiRemoteOp wro;
    wro.op = WiiRemoteOp::send;
    wro.index = wr;
    
    WiiRemoteOp_cbuf->put( &wro, 1 );
    
    WiiRemote_signal(); 
    
    return 0;
#else
    return -1;
#endif
}

const char * WiiRemote_name( int wr )
{
#ifdef __CK_HID_WIIREMOTE__
    return "Nintendo Wii Remote";
#else
    return " ";
#endif // __CK_HID_WIIREMOTE__
}

#elif ( defined( __PLATFORM_WIN32__ ) || defined( __WINDOWS_PTHREAD__ ) ) && !defined( USE_RAWINPUT )
/*****************************************************************************
Windows general HID support
*****************************************************************************/
#pragma mark Windows general HID support

#include <windows.h>
#define DIRECTINPUT_VERSION 0x0500
#include <dinput.h>

/* for performance, we use device event notifications to tell us when a device 
   has new data.  However, serial devices don't seem to support event 
   notification, so we have to fall back to the periodic sleeping and polling.  
   g_wait_function is usually set to call WaitForSingleObject (wait for event
   notification), but devices for which SetEventNotification fails will replace
   it with a wrapper around usleep.  */
static HANDLE g_device_event = NULL;
static void (*g_wait_function)() = NULL;

static void Hid_wait_usleep()
{
	usleep( 10 );
}

static void Hid_wait_event()
{
	WaitForSingleObject( g_device_event, INFINITE );
}

void Hid_init()
{
    if( g_device_event != NULL )
		return;

	g_device_event = CreateEvent( NULL, FALSE, FALSE, NULL );
	if( g_device_event == NULL )
		EM_log( CK_LOG_SEVERE, "hid: error: unable to create event (win32 error %i)", GetLastError() );
	g_wait_function = Hid_wait_event;
}

void Hid_poll()
{
    g_wait_function();
    Joystick_poll();
    Keyboard_poll();
    Mouse_poll();
}

void Hid_quit()
{
	SetEvent( g_device_event );
	/*if( g_device_event != NULL )
	{
		CloseHandle( g_device_event );
		g_device_event = NULL;
	}*/
}

/*****************************************************************************
Windows joystick support
*****************************************************************************/
#pragma mark Windows joystick support

static LPDIRECTINPUT lpdi = NULL;

struct win32_joystick
{
	win32_joystick()
	{
		lpdiJoystick = NULL;
		refcount = 0;
		needs_close = FALSE;
		strncpy( name, "Joystick", MAX_PATH );
		memset( &last_state, 0, sizeof( last_state ) );
		memset( &caps, 0, sizeof( caps ) );
		caps.dwSize = sizeof( DIDEVCAPS );
	}

	LPDIRECTINPUTDEVICE2 lpdiJoystick;
	DIJOYSTATE2 last_state;
	DIDEVCAPS caps;

	char name[MAX_PATH];

	t_CKUINT refcount;
	t_CKBOOL needs_close;

};

const static LONG axis_min = -32767;
const static LONG axis_max = 32767;


static vector< win32_joystick * > * joysticks;

static BOOL CALLBACK DIEnumJoystickProc( LPCDIDEVICEINSTANCE lpddi,
										 LPVOID pvRef )
{
	GUID guid = lpddi->guidInstance;
	win32_joystick * js = new win32_joystick;

	EM_log( CK_LOG_INFO, "found %s", lpddi->tszProductName );

	strncpy( js->name, lpddi->tszProductName, MAX_PATH );

	if( lpdi->CreateDevice( guid, ( LPDIRECTINPUTDEVICE * )&js->lpdiJoystick, NULL ) != DI_OK )
	{
		delete js;
		return DIENUM_CONTINUE;
	}

	joysticks->push_back( js );

	return DIENUM_CONTINUE;
}

static BOOL CALLBACK DIEnumJoystickObjectsProc( LPCDIDEVICEOBJECTINSTANCE lpdidoi,
											    LPVOID pvRef )
{
	LPDIRECTINPUTDEVICE lpdiJoystick = ( LPDIRECTINPUTDEVICE ) pvRef;

	DIPROPRANGE diprg; 

	// set axis minimum and maximum range
	diprg.diph.dwSize = sizeof(DIPROPRANGE); 
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	diprg.diph.dwHow = DIPH_BYID; 
	diprg.diph.dwObj = lpdidoi->dwType; 
	diprg.lMin = axis_min; 
	diprg.lMax = axis_max; 

	if( lpdiJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) != DI_OK )
	{
		
	}

	return DIENUM_CONTINUE;
}

void Joystick_init()
{
	if( joysticks != NULL )
		return;

	EM_log( CK_LOG_INFO, "initializing joystick" );
	EM_pushlog();

    HINSTANCE hInstance = GetModuleHandle( NULL );

	if( lpdi == NULL )
	{
		if( DirectInputCreate( hInstance, DIRECTINPUT_VERSION, 
							   &lpdi, NULL) != DI_OK )
		{
			lpdi = NULL;
			EM_log( CK_LOG_SEVERE, "error: unable to initialize DirectInput, initialization failed" );
			EM_poplog();
			return;
		}
	}

	joysticks = new vector< win32_joystick * >;
	if( lpdi->EnumDevices( DIDEVTYPE_JOYSTICK, DIEnumJoystickProc, 
						   NULL, DIEDFL_ATTACHEDONLY ) != DI_OK )
	{
		delete joysticks;
		joysticks = NULL;
		lpdi->Release();
		lpdi = NULL;
		EM_log( CK_LOG_SEVERE, "error: unable to enumerate devices, initialization failed" );
		EM_poplog();
		return;
	}

	EM_poplog();
}

t_CKINT Joystick_translate_POV( DWORD v )
{
#define CK_POV_CENTER 0
#define CK_POV_UP 1
#define CK_POV_RIGHT 2
#define CK_POV_DOWN 4
#define CK_POV_LEFT 8

#define CK_POV_UP_LBORDER 29250
#define CK_POV_UP_RBORDER 6750
#define CK_POV_RIGHT_LBORDER 2250
#define CK_POV_RIGHT_RBORDER 15750
#define CK_POV_DOWN_LBORDER 11250
#define CK_POV_DOWN_RBORDER 24750
#define CK_POV_LEFT_LBORDER 20250
#define CK_POV_LEFT_RBORDER 33750

	t_CKINT r = 0;

	if( LOWORD(v) == 0xffff )
		return CK_POV_CENTER;

	if( v > CK_POV_UP_LBORDER || v < CK_POV_UP_RBORDER )
		r |= CK_POV_UP;
	if( v > CK_POV_RIGHT_LBORDER && v < CK_POV_RIGHT_RBORDER )
		r |= CK_POV_RIGHT;
	if( v > CK_POV_DOWN_LBORDER && v < CK_POV_DOWN_RBORDER )
		r |= CK_POV_DOWN;
	if( v > CK_POV_LEFT_LBORDER && v < CK_POV_LEFT_RBORDER )
		r |= CK_POV_LEFT;

	return r;
}

void Joystick_poll()
{
	if( !joysticks )
		return;

	win32_joystick * joystick;
	HidMsg msg;
	vector< win32_joystick * >::size_type i, len = joysticks->size();
	int j;
	for( i = 0; i < len; i++ )
	{
		joystick = joysticks->at( i );
		if( joystick->refcount )
		{
			// TODO: convert this to buffered input, or maybe notifications
			DIJOYSTATE2 state;
			
			joystick->lpdiJoystick->Poll();

			if( joystick->lpdiJoystick->GetDeviceState( sizeof( DIJOYSTATE2 ), &state ) 
				!= DI_OK )
			{
				EM_log( CK_LOG_WARNING, "joystick: GetDeviceState failed for %s", joystick->name );
				continue;
			}

			if( state.lX != joystick->last_state.lX )
			{
				msg.clear();
				msg.device_num = i;
				msg.device_type = CK_HID_DEV_JOYSTICK;
				msg.eid = 0;
				msg.type = CK_HID_JOYSTICK_AXIS;
                msg.fdata[0] = ((float)state.lX)/((float)axis_max);
				HidInManager::push_message( msg );
			}

			if( state.lY != joystick->last_state.lY )
			{
				msg.clear();
				msg.device_num = i;
				msg.device_type = CK_HID_DEV_JOYSTICK;
				msg.eid = 1;
				msg.type = CK_HID_JOYSTICK_AXIS;
                msg.fdata[0] = ((float)state.lY)/((float)axis_max);
				HidInManager::push_message( msg );
			}

			if( state.lZ != joystick->last_state.lZ )
			{
				msg.clear();
				msg.device_num = i;
				msg.device_type = CK_HID_DEV_JOYSTICK;
				msg.eid = 2;
				msg.type = CK_HID_JOYSTICK_AXIS;
                msg.fdata[0] = ((float)state.lZ)/((float)axis_max);
				HidInManager::push_message( msg );
			}

			if( state.lRx != joystick->last_state.lRx )
			{
				msg.clear();
				msg.device_num = i;
				msg.device_type = CK_HID_DEV_JOYSTICK;
				msg.eid = 3;
				msg.type = CK_HID_JOYSTICK_AXIS;
                msg.fdata[0] = ((float)state.lRx)/((float)axis_max);
				HidInManager::push_message( msg );
			}

			if( state.lRy != joystick->last_state.lRy )
			{
				msg.clear();
				msg.device_num = i;
				msg.device_type = CK_HID_DEV_JOYSTICK;
				msg.eid = 4;
				msg.type = CK_HID_JOYSTICK_AXIS;
                msg.fdata[0] = ((float)state.lRy)/((float)axis_max);
				HidInManager::push_message( msg );
			}

			if( state.lRz != joystick->last_state.lRz )
			{
				msg.clear();
				msg.device_num = i;
				msg.device_type = CK_HID_DEV_JOYSTICK;
				msg.eid = 5;
				msg.type = CK_HID_JOYSTICK_AXIS;
                msg.fdata[0] = ((float)state.lRz)/((float)axis_max);
				HidInManager::push_message( msg );
			}

			for( j = 0; j < 2; j++ )
			{
				if( state.rglSlider[j] != joystick->last_state.rglSlider[j] )
				{
					msg.clear();
					msg.device_num = i;
					msg.device_type = CK_HID_DEV_JOYSTICK;
					msg.eid = 5 + j;
					msg.type = CK_HID_JOYSTICK_AXIS;
		            msg.fdata[0] = ((float)state.rglSlider[j])/((float)axis_max);
					HidInManager::push_message( msg );
				}
			}

			for( j = 0; j < joystick->caps.dwPOVs && j < 4; j++ )
			{
				if( state.rgdwPOV[j] != joystick->last_state.rgdwPOV[j] )
				{
					msg.clear();
					msg.device_num = i;
					msg.device_type = CK_HID_DEV_JOYSTICK;
					msg.eid = j;
					msg.type = CK_HID_JOYSTICK_HAT;
					msg.idata[0] = Joystick_translate_POV( state.rgdwPOV[j] );
					msg.fdata[0] = (t_CKFLOAT)state.rgdwPOV[j];
					HidInManager::push_message( msg );
				}
			}

			for( j = 0; j < joystick->caps.dwButtons && j < 128; j++ )
			{
				if( ( state.rgbButtons[j] & 0x80 ) ^ 
					( joystick->last_state.rgbButtons[j] & 0x80 ) )
				{
					msg.clear();
					msg.device_num = i;
					msg.device_type = CK_HID_DEV_JOYSTICK;
					msg.eid = j;
					msg.type = ( state.rgbButtons[j] & 0x80 ) ? CK_HID_BUTTON_DOWN : 
																CK_HID_BUTTON_UP;
					msg.idata[0] = ( state.rgbButtons[j] & 0x80 ) ? 1 : 0;
					HidInManager::push_message( msg );
				}
			}

			joystick->last_state = state;
		}

		else if( joystick->needs_close )
		{
			joystick->needs_close = FALSE;
			joystick->lpdiJoystick->Unacquire();
			joystick->lpdiJoystick->SetEventNotification( NULL );
		}
	}

}

void Joystick_quit()
{
	if( joysticks )
	{
		win32_joystick * joystick;
		vector< win32_joystick * >::size_type i, len = joysticks->size();
		for( i = 0; i < len; i++ )
		{
			joystick = joysticks->at( i );

			if( joystick->refcount > 0 || joystick->needs_close)
			{
				joystick->needs_close = FALSE;
				joystick->refcount = 0;
				joystick->lpdiJoystick->Unacquire();
			    joystick->lpdiJoystick->SetEventNotification( NULL );
			}

			joystick->lpdiJoystick->Release();
			delete joystick;
		}

		delete joysticks;
		joysticks = NULL;
	}

	if( lpdi )
	{
		lpdi->Release();
		lpdi = NULL;
	}
}

int Joystick_count()
{
    if( !joysticks )
		return 0;
	return joysticks->size();
}

int Joystick_open( int js )
{
	if( !joysticks || js < 0 || js >= joysticks->size() )
		return -1;

	win32_joystick * joystick = joysticks->at( js );

	if( joystick->refcount == 0 )
	{
		if( joystick->lpdiJoystick->EnumObjects( DIEnumJoystickObjectsProc, 
												 joystick->lpdiJoystick, 
												 DIDFT_AXIS ) != DI_OK )
		{
			return -1;
		}

		if( joystick->lpdiJoystick->GetCapabilities( &joystick->caps ) != DI_OK )
		{
			return -1;
		}

		if( joystick->lpdiJoystick->SetDataFormat( &c_dfDIJoystick2 ) != DI_OK )
		{
			return -1;
		}

		if( joystick->lpdiJoystick->SetEventNotification( g_device_event ) != DI_OK )
		{
			// fallback to sleep+poll mode
			g_wait_function = Hid_wait_usleep;
            SetEvent( g_device_event );
		}

		if( joystick->lpdiJoystick->Acquire() != DI_OK )
		{
			joystick->lpdiJoystick->SetEventNotification( NULL );
			return -1;
		}
	}
	
	joystick->refcount++;
	
    return 0;
}

int Joystick_close( int js )
{
	if( !joysticks || js < 0 || js >= joysticks->size() )
		return -1;

	win32_joystick * joystick = joysticks->at( js );

	joystick->refcount--;

	if( joystick->refcount < 1 )
		joystick->needs_close = TRUE;

	return 0;
}

const char * Joystick_name( int js )
{
    if( !joysticks || js < 0 || js >= joysticks->size() )
        return NULL;
    
    return joysticks->at( js )->name;
}

/*****************************************************************************
 Windows keyboard support
 *****************************************************************************/
#pragma mark Windows keyboards support

#define DINPUT_KEYBUFFER_SIZE 256

struct win32_keyboard
{
	win32_keyboard()
	{
		lpdiKeyboard = NULL;
		refcount = 0;
		needs_close = FALSE;
		strncpy( name, "Keyboard", MAX_PATH );
		memset( &last_state, 0, DINPUT_KEYBUFFER_SIZE );
	}

	LPDIRECTINPUTDEVICE2 lpdiKeyboard;
	char last_state[DINPUT_KEYBUFFER_SIZE];
	DIDEVCAPS caps;

	char name[MAX_PATH];

	t_CKUINT refcount;
	t_CKBOOL needs_close;

};

static vector< win32_keyboard * > * keyboards;

static BOOL CALLBACK DIEnumKeyboardProc( LPCDIDEVICEINSTANCE lpddi,
										 LPVOID pvRef )
{
	GUID guid = lpddi->guidInstance;
	win32_keyboard * keyboard = new win32_keyboard;

	EM_log( CK_LOG_INFO, "found %s", lpddi->tszProductName );

	strncpy( keyboard->name, lpddi->tszProductName, MAX_PATH );

	if( lpdi->CreateDevice( guid,
							( LPDIRECTINPUTDEVICE * ) &keyboard->lpdiKeyboard,
							NULL ) != DI_OK )
	{
		delete keyboard;
		EM_log( CK_LOG_WARNING, "error: unable to initialize device %s", 
				lpddi->tszProductName );
		return DIENUM_CONTINUE;
	}

	keyboards->push_back( keyboard );

	return DIENUM_CONTINUE;
}

void Keyboard_init()
{
	if( keyboards != NULL )
		return;

	EM_log( CK_LOG_INFO, "initializing keyboard" );
	EM_pushlog();

    HINSTANCE hInstance = GetModuleHandle( NULL );

	if( lpdi == NULL )
	{
		if( DirectInputCreate( hInstance, DIRECTINPUT_VERSION, 
							   &lpdi, NULL) != DI_OK )
		{
			lpdi = NULL;
			EM_log( CK_LOG_SEVERE, "error: unable to initialize DirectInput, initialization failed" );
			EM_poplog();
			return;
		}
	}

	keyboards = new vector< win32_keyboard * >;
	if( lpdi->EnumDevices( DIDEVTYPE_KEYBOARD, DIEnumKeyboardProc, 
						   NULL, DIEDFL_ATTACHEDONLY ) != DI_OK )
	{
		delete keyboards;
		keyboards = NULL;
		lpdi->Release();
		lpdi = NULL;
		EM_log( CK_LOG_SEVERE, "error: unable to enumerate devices, initialization failed" );
		EM_poplog();
		return;
	}

	EM_poplog();
}

void Keyboard_poll()
{
	if( !keyboards )
		return;

	win32_keyboard * keyboard;
	HidMsg msg;
	vector< win32_keyboard * >::size_type i, len = keyboards->size();
	for( i = 0; i < len; i++ )
	{
		keyboard = keyboards->at( i );
		if( keyboard->refcount )
		{
			// TODO: convert this to buffered input, or maybe notifications
			char state[DINPUT_KEYBUFFER_SIZE];
			
			keyboard->lpdiKeyboard->Poll();

			if( keyboard->lpdiKeyboard->GetDeviceState( DINPUT_KEYBUFFER_SIZE, state ) 
				!= DI_OK )
			{
				EM_log( CK_LOG_WARNING, "keyboard: GetDeviceState failed for %s", 
						keyboard->name );
				continue;
			}

			for( int j = 0; j < DINPUT_KEYBUFFER_SIZE; j++ )
			{
				if( ( state[j] & 0x80 ) ^ ( keyboard->last_state[j] & 0x80 ) )
				{
					msg.clear();
					msg.device_num = i;
					msg.device_type = CK_HID_DEV_KEYBOARD;
					msg.type = ( state[j] & 0x80 ) ? CK_HID_BUTTON_DOWN :
													 CK_HID_BUTTON_UP;
					msg.eid = j;
					msg.idata[0] = ( state[j] & 0x80 ) ? 1 : 0;
					HidInManager::push_message( msg );
				}
			}

			memcpy( keyboard->last_state, state, DINPUT_KEYBUFFER_SIZE );
		}

		else if( keyboard->needs_close )
		{
			keyboard->needs_close = FALSE;
			keyboard->lpdiKeyboard->Unacquire();
            keyboard->lpdiKeyboard->SetEventNotification( NULL );
		}
	}
}

void Keyboard_quit()
{
	if( keyboards )
	{
		win32_keyboard * keyboard;
		vector< win32_keyboard * >::size_type i, len = keyboards->size();
		for( i = 0; i < len; i++ )
		{
			keyboard = keyboards->at( i );

			if( keyboard->refcount > 0 || keyboard->needs_close)
			{
				keyboard->needs_close = FALSE;
				keyboard->refcount = 0;
				keyboard->lpdiKeyboard->Unacquire();
                keyboard->lpdiKeyboard->SetEventNotification( NULL );
			}

			keyboard->lpdiKeyboard->Release();
			delete keyboard;
		}

		delete keyboards;
		keyboards = NULL;
	}

	if( lpdi )
	{
		lpdi->Release();
		lpdi = NULL;
	}
}

int Keyboard_count()
{
    if( !keyboards )
		return 0;
	return keyboards->size();
}

int Keyboard_open( int k )
{
	if( !keyboards || k < 0 || k >= keyboards->size() )
		return -1;

	win32_keyboard * keyboard = keyboards->at( k );

	if( keyboard->refcount == 0 )
	{
		if( keyboard->lpdiKeyboard->SetDataFormat( &c_dfDIKeyboard ) != DI_OK )
		{
			return -1;
		}

		if( keyboard->lpdiKeyboard->SetEventNotification( g_device_event ) != DI_OK )
		{
			// fallback to sleep+poll mode
			g_wait_function = Hid_wait_usleep;
            SetEvent( g_device_event );
		}

		if( keyboard->lpdiKeyboard->Acquire() != DI_OK )
		{
            keyboard->lpdiKeyboard->SetEventNotification( NULL );
			return -1;
		}
	}
	
	keyboard->refcount++;
	
    return 0;
}

int Keyboard_close( int k )
{
	if( !keyboards || k < 0 || k >= keyboards->size() )
		return -1;

	win32_keyboard * keyboard = keyboards->at( k );

	keyboard->refcount--;

	if( keyboard->refcount < 1 )
		keyboard->needs_close = TRUE;

	return 0;
}

const char * Keyboard_name( int kb )
{
    if( !keyboards || kb < 0 || kb >= keyboards->size() )
        return NULL;
    
    return keyboards->at( kb )->name;
}

/*****************************************************************************
 Windows mouse support
 *****************************************************************************/
#pragma mark Windows mouse support

struct win32_mouse
{
	win32_mouse()
	{
		refcount = 0;
		needs_close = FALSE;

		lpdiMouse = NULL;
		memset( &last_state, 0, sizeof( last_state ) );
	}

	LPDIRECTINPUTDEVICE2 lpdiMouse;
	DIMOUSESTATE last_state;

	char name[MAX_PATH];

	t_CKUINT refcount;
	t_CKBOOL needs_close;
};

static vector< win32_mouse * > * mice;

static BOOL CALLBACK DIEnumMouseProc( LPCDIDEVICEINSTANCE lpddi,
									  LPVOID pvRef )
{
	GUID guid = lpddi->guidInstance;
	win32_mouse * mouse = new win32_mouse;

	EM_log( CK_LOG_INFO, "found %s", lpddi->tszProductName );

	strncpy( mouse->name, lpddi->tszProductName, MAX_PATH );

	if( lpdi->CreateDevice( guid, ( LPDIRECTINPUTDEVICE * ) &mouse->lpdiMouse,
							NULL ) != DI_OK )
	{
		delete mouse;
		return DIENUM_CONTINUE;
	}

	mice->push_back( mouse );

	return DIENUM_CONTINUE;
}

void Mouse_init()
{
	if( mice != NULL )
		return;

	EM_log( CK_LOG_INFO, "initializing mouse" );
	EM_pushlog();

    HINSTANCE hInstance = GetModuleHandle( NULL );

	if( lpdi == NULL )
	{
		if( DirectInputCreate( hInstance, DIRECTINPUT_VERSION, 
							   &lpdi, NULL) != DI_OK )
		{
			lpdi = NULL;
			EM_poplog();
			return;
		}
	}

	mice = new vector< win32_mouse * >;
	if( lpdi->EnumDevices( DIDEVTYPE_MOUSE, DIEnumMouseProc, 
						   NULL, DIEDFL_ATTACHEDONLY ) != DI_OK )
	{
		delete mice;
		mice = NULL;
		lpdi->Release();
		lpdi = NULL;
		EM_poplog();
		return;
	}

	EM_poplog();
}

void Mouse_poll()
{
	if( !mice )
		return;

	win32_mouse * mouse;
	HidMsg msg;
	vector< win32_mouse * >::size_type i, len = mice->size();
	for( i = 0; i < len; i++ )
	{
		mouse = mice->at( i );
		if( mouse->refcount )
		{
			// TODO: convert this to buffered input, or maybe notifications
			DIMOUSESTATE state;
			
			mouse->lpdiMouse->Poll();

			if( mouse->lpdiMouse->GetDeviceState( sizeof( DIMOUSESTATE ), &state ) 
				!= DI_OK )
			{
				EM_log( CK_LOG_WARNING, "mouse: GetDeviceState failed for %s", mouse->name );
				continue;
			}

			if( state.lX != 0 || state.lY != 0 )
			{
				msg.clear();
				msg.device_num = i;
				msg.device_type = CK_HID_DEV_MOUSE;
				msg.type = CK_HID_MOUSE_MOTION;
				msg.eid = 0;
				msg.idata[0] = state.lX;
				msg.idata[1] = state.lY;
				HidInManager::push_message( msg );
			}

            if( state.lZ != 0 )
            {
                msg.clear();
                msg.device_num = i;
                msg.device_type = CK_HID_DEV_MOUSE;
                msg.type = CK_HID_MOUSE_WHEEL;
                msg.eid = 0;
                msg.idata[0] = 0;
                msg.idata[1] = state.lZ;
                HidInManager::push_message( msg );
            }

			for( int j = 0; j < 4; j++ )
			{
				if( ( state.rgbButtons[j] & 0x80 ) ^
					( mouse->last_state.rgbButtons[j] & 0x80 ) )
				{
					msg.clear();
					msg.device_num = i;
					msg.device_type = CK_HID_DEV_MOUSE;
					msg.type = ( state.rgbButtons[j] & 0x80 ) ? CK_HID_BUTTON_DOWN :
																CK_HID_BUTTON_UP;
					msg.eid = j;
					msg.idata[0] = ( state.rgbButtons[j] & 0x80 ) ? 1 : 0;
					HidInManager::push_message( msg );
				}
			}

			mouse->last_state = state;
		}

		else if( mouse->needs_close )
		{
			mouse->needs_close = FALSE;
			mouse->lpdiMouse->Unacquire();
            mouse->lpdiMouse->SetEventNotification( NULL );
		}
	}
}

void Mouse_quit()
{
	if( mice )
	{
		win32_mouse * mouse;
		vector< win32_mouse * >::size_type i, len = mice->size();
		for( i = 0; i < len; i++ )
		{
			mouse = mice->at( i );

			if( mouse->refcount > 0 || mouse->needs_close)
			{
				mouse->needs_close = FALSE;
				mouse->refcount = 0;
				mouse->lpdiMouse->Unacquire();
                mouse->lpdiMouse->SetEventNotification( NULL );
			}

			mouse->lpdiMouse->Release();
			delete mouse;
		}

		delete mice;
		mice = NULL;
	}

	if( lpdi )
	{
		lpdi->Release();
		lpdi = NULL;
	}
}

int Mouse_count()
{
    if( !mice )
		return 0;
	return mice->size();
}

int Mouse_open( int m )
{
	if( !mice || m < 0 || m >= mice->size() )
		return -1;

	win32_mouse * mouse = mice->at( m );

	if( mouse->refcount == 0 )
	{
		if( mouse->lpdiMouse->SetDataFormat( &c_dfDIMouse ) != DI_OK )
		{
			return -1;
		}

		if( mouse->lpdiMouse->SetEventNotification( g_device_event ) != DI_OK )
		{
			// fallback to sleep+poll mode
			g_wait_function = Hid_wait_usleep;
            SetEvent( g_device_event );
		}

		if( mouse->lpdiMouse->Acquire() != DI_OK )
		{
            mouse->lpdiMouse->SetEventNotification( g_device_event );
			return -1;
		}
	}
	
	mouse->refcount++;

	return 0;
}

int Mouse_close( int m )
{
	if( !mice || m < 0 || m >= mice->size() )
		return -1;

	win32_mouse * mouse = mice->at( m );

	mouse->refcount--;

	if( mouse->refcount < 1 )
		mouse->needs_close = TRUE; // let the polling thread take care of it

	return 0;
}

const char * Mouse_name( int m )
{
    if( !mice || m < 0 || m >= mice->size() )
        return NULL;
    
    return mice->at( m )->name;
}

#elif defined( __PLATFORM_WIN32__ ) || defined( __WINDOWS_PTHREAD__ ) && defined( USE_RAWINPUT )

void Joystick_init()
{
    
}

void Joystick_poll()
{
    
}

void Joystick_quit()
{
    
}

int Joystick_count()
{
    return 0;
}

int Joystick_open( int js )
{
    return -1;
}

int Joystick_close( int js )
{
    return -1;
}

void Mouse_init()
{
    
}

void Mouse_poll()
{
    
}

void Mouse_quit()
{
    
}

int Mouse_count()
{
    return 0;
}

int Mouse_open( int js )
{
    return -1;
}

int Mouse_close( int js )
{
    return -1;
}

void Keyboard_init()
{
    
}

void Keyboard_poll()
{
    
}

void Keyboard_quit()
{
    
}

int Keyboard_count()
{
    return 0;
}

int Keyboard_open( int js )
{
    return -1;
}

int Keyboard_close( int js )
{
    return -1;
}

#elif defined( __LINUX_ALSA__ ) || defined( __LINUX_OSS__ ) || defined( __LINUX_JACK__ )
/*****************************************************************************
Linux general HID support
*****************************************************************************/
#pragma mark Linux general HID support

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/joystick.h>
#include <linux/input.h>

#define CK_HID_DIR ("/dev/input")
#define CK_HID_MOUSEFILE ("mouse%d")
#define CK_HID_JOYSTICKFILE ("js%d")
#define CK_HID_EVDEVFILE ("event%d")
#define CK_HID_STRBUFSIZE (1024)
#define CK_HID_NAMESIZE (128)

class linux_device
{
public:
    linux_device()
    {
        fd = -1;
        num = -1;
        refcount = 0;
        pollfds_i = 0;
        filename[0] = '\0';
        needs_open = needs_close = FALSE;
        strncpy( name, "(name unknown)", CK_HID_NAMESIZE );
    }
    
    virtual void callback() = 0;
    
    int fd;       // file descriptor
    t_CKINT num; // index in the respective device vector {joysticks, mice, keyboards}
    
    t_CKUINT refcount;
    
    size_t pollfds_i;
    
    t_CKBOOL needs_open;
    t_CKBOOL needs_close;
    
    char filename[CK_HID_STRBUFSIZE];
    char name[CK_HID_NAMESIZE];
};

class linux_joystick : public linux_device
{
public:
    linux_joystick() : linux_device()
    {
        js_num = -1;
    }
    
    virtual void callback()
    {        
        js_event event;
        HidMsg msg;
        ssize_t len;
                
        while( ( len = read( fd, &event, sizeof( event ) ) ) > 0 )
        {
            if( len < sizeof( event ) )
            {
                EM_log( CK_LOG_WARNING, "joystick: read event from %s smaller than expected, ignoring", name );
                continue;
            }
            
            if( event.type == JS_EVENT_INIT )
                continue;
            
            msg.device_type = CK_HID_DEV_JOYSTICK;
            msg.device_num = num;
            msg.eid = event.number;
            
            switch( event.type )
            {
                case JS_EVENT_BUTTON:
                    msg.type = event.value ? CK_HID_BUTTON_DOWN : 
                                             CK_HID_BUTTON_UP;
                    msg.idata[0] = event.value;
                    break;
                case JS_EVENT_AXIS:
                    msg.type = CK_HID_JOYSTICK_AXIS;
                    msg.fdata[0] = ((t_CKFLOAT)event.value) / ((t_CKFLOAT) SHRT_MAX);
                    break;
                default:
                    EM_log( CK_LOG_WARNING, "joystick: unknown event type from %s, ignoring", name );
                    continue;
            }
            
            HidInManager::push_message( msg );
        }
    }
    
    int js_num;   // /dev/input/js# <-- the #
};

#define __LITTLE_ENDIAN__
struct ps2_mouse_event
{
#ifdef __BIG_ENDIAN__
    unsigned unused:2;
    unsigned y_reverse_motion:1;
    unsigned x_reverse_motion:1;
    unsigned always1:1;
    unsigned button3:1;
    unsigned button2:1;
    unsigned button1:1;
    signed dx:8;
    signed dy:8;
#elif defined( __LITTLE_ENDIAN__ )
    unsigned button1:1;
    unsigned button2:1;
    unsigned button3:1;
    unsigned always1:1;
    unsigned x_reverse_motion:1;
    unsigned y_reverse_motion:1;
    unsigned unused:2;
    signed dx:8;
    signed dy:8;
#else
#error unknown endian mode (both __LITTLE_ENDIAN__ and __BIG_ENDIAN__ undefined)
#endif
} __attribute__ ((__packed__));

class linux_mouse : public linux_device
{
public:
    linux_mouse() : linux_device()
    {
        m_num = -1;
        memset( &last_event, 0, sizeof( last_event ) );
    }
    
    virtual void callback()
    {        
        //ps2_mouse_event event;
        input_event event;
        HidMsg msg;
        ssize_t len;
                
        while( ( len = read( fd, &event, sizeof( event ) ) ) > 0 )
        {
            if( len < sizeof( event ) )
            {
                EM_log( CK_LOG_WARNING, "mouse: read event from mouse %i smaller than expected (%i), ignoring", num, len );
                continue;
            }
            
            switch( event.type )
            {
                case EV_KEY:
                    if( event.code & BTN_MOUSE )
                    {
                        msg.clear();
                        msg.device_type = CK_HID_DEV_MOUSE;
                        msg.device_num = num;
                        msg.eid = event.code - BTN_MOUSE;
                        msg.type = event.value ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
                        msg.idata[0] = event.value;
                        HidInManager::push_message( msg ); 
                    }
                    
                    break;
                
                case EV_REL:
                    msg.clear();
                    msg.device_type = CK_HID_DEV_MOUSE;
                    msg.device_num = num;
                    
                    switch( event.code )
                    {
                        case REL_X:
                            msg.type = CK_HID_MOUSE_MOTION;
                            msg.idata[0] = event.value;
                            msg.idata[1] = 0;
                            break;
                    
                        case REL_Y:
                            msg.type = CK_HID_MOUSE_MOTION;
                            msg.idata[0] = 0;
                            msg.idata[1] = event.value;
                            break;
                    
                        case REL_HWHEEL:
                            msg.type = CK_HID_MOUSE_WHEEL;
                            msg.idata[0] = event.value;
                            msg.idata[1] = 0;
                            break;
                    
                        case REL_Z:
                        case REL_WHEEL:
                            msg.type = CK_HID_MOUSE_WHEEL;
                            msg.idata[0] = 0;
                            msg.idata[1] = event.value;
                            break;
                    }
                    
                    HidInManager::push_message( msg );
                    
                    break;
                    
            }
            
/*            
            if( event.dx || event.dy )
            {
                msg.device_type = CK_HID_DEV_MOUSE;
                msg.device_num = num;
                msg.eid = 0;
                msg.type = CK_HID_MOUSE_MOTION;
                msg.idata[0] = event.dx;
                msg.idata[1] = event.dy;
                HidInManager::push_message( msg );
            }
            
            if( event.button1 ^ last_event.button1 )
            {
                msg.device_type = CK_HID_DEV_MOUSE;
                msg.device_num = num;
                msg.eid = 0;
                msg.type = event.button1 ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
                msg.idata[0] = event.button1;
                HidInManager::push_message( msg );
            }
            
            if( event.button2 ^ last_event.button2 )
            {
                msg.device_type = CK_HID_DEV_MOUSE;
                msg.device_num = num;
                msg.eid = 2;
                msg.type = event.button2 ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
                msg.idata[0] = event.button2;
                HidInManager::push_message( msg );
            }
            
            if( event.button3 ^ last_event.button3 )
            {
                msg.device_type = CK_HID_DEV_MOUSE;
                msg.device_num = num;
                msg.eid = 3;
                msg.type = event.button3 ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
                msg.idata[0] = event.button3;
                HidInManager::push_message( msg );
            }
            */
            memcpy( &last_event, &event, sizeof( last_event ) );
        }
    }
    
    int m_num;   // /dev/input/mouse# <-- the #
    //ps2_mouse_event last_event;
    input_event last_event;
};

class linux_keyboard : public linux_device
{
public:
    linux_keyboard() : linux_device()
    {
    }
    
    virtual void callback()
    {
        input_event event;
        HidMsg msg;
        ssize_t len;
                
        while( ( len = read( fd, &event, sizeof( event ) ) ) > 0 )
        {
            if( len < sizeof( event ) )
            {
                EM_log( CK_LOG_WARNING, "keyboard: read event from keyboard %i smaller than expected (%i), ignoring", num, len );
                continue;
            }
            
            if( event.value == 2 )
                continue;
            
            msg.clear();
            msg.device_type = CK_HID_DEV_KEYBOARD;
            msg.device_num = num;
            msg.type = event.value ? CK_HID_BUTTON_DOWN : CK_HID_BUTTON_UP;
            msg.eid = event.code;
            msg.idata[0] = event.value;
            
            HidInManager::push_message( msg );
        }
    }
};

static vector< linux_joystick * > * joysticks = NULL;
static vector< linux_mouse * > * mice = NULL;
static vector< linux_keyboard * > * keyboards = NULL;

static map< int, linux_device * > * device_map = NULL;

static pollfd * pollfds = NULL;
#define DEFAULT_POLLFDS_SIZE (1)
static size_t pollfds_size = 0;
static size_t pollfds_end = 0;
static int hid_channel_r = -1; // HID communications channel, read fd
static int hid_channel_w = -1; // HID communications channel, write fd

struct hid_channel_msg
{
    int action;
#define HID_CHANNEL_OPEN 1
#define HID_CHANNEL_CLOSE 0
#define HID_CHANNEL_QUIT -1
    linux_device * device;
};

static t_CKBOOL g_hid_init = FALSE;

void Hid_init()
{
    if( g_hid_init )
        return;
    
    pollfds = new pollfd[DEFAULT_POLLFDS_SIZE];
    pollfds_size = DEFAULT_POLLFDS_SIZE;
    pollfds_end = 0;
    
    int filedes[2];
    if( pipe( filedes ) )
    {
        EM_log( CK_LOG_SEVERE, "hid: unable to create pipe, initialization failed" );
        return;
    }
    
    hid_channel_r = filedes[0];
    hid_channel_w = filedes[1];
    
    int fd_flags = fcntl( hid_channel_r, F_GETFL );
	fcntl( hid_channel_r, F_SETFL, fd_flags | O_NONBLOCK );
    
    /* right now, the hid_channel is just used as a dummy file descriptor
       passed to poll, such that poll will work/block even when there are
       no open devices.  In the future hid_channel could also be used to 
       communicate between the VM thread and the HID thread */
    pollfds[0].fd = hid_channel_r;
    pollfds[0].events = POLLIN;
    pollfds[0].revents = 0;
    pollfds_end = 1;
    
    device_map = new map< int, linux_device * >;
    
    g_hid_init = TRUE;
}

void Hid_poll()
{
    if( !g_hid_init )
        return;

    hid_channel_msg hcm;
    while( poll( pollfds, pollfds_end, -1 ) > 0 )
    {
        for( int i = 1; i < pollfds_end; i++ )
        {
            if( pollfds[i].revents & POLLIN )
            {
                ( *device_map )[pollfds[i].fd]->callback();
                pollfds[i].revents = 0;
            }
        }
        
        if( pollfds[0].revents & POLLIN )
        {
            while( read( hid_channel_r, &hcm, sizeof( hcm ) ) > 0 )
            {
                if( hcm.action == HID_CHANNEL_OPEN )
                {
                    if( pollfds_end >= pollfds_size )
                    {
                        pollfds_size = pollfds_end * 2;
                        pollfd * t_pollfds = new pollfd[pollfds_size];
                        memcpy( t_pollfds, pollfds, pollfds_end * sizeof( pollfd ) );
                        delete[] pollfds;
                        pollfds = t_pollfds;
                    }

                    pollfds[pollfds_end].fd = hcm.device->fd;
                    pollfds[pollfds_end].events = POLLIN;
                    pollfds[pollfds_end].revents = 0;
                    hcm.device->pollfds_i = pollfds_end;
                    ( *device_map )[hcm.device->fd] = hcm.device;
                    pollfds_end++;
                }
                
                else if( hcm.action == HID_CHANNEL_CLOSE )
                {
                    // erase the closing entry by copying the last entry into it
                    // this is okay even when joystick->pollfds_i == pollfds_end
                    // because we decrement pollfds_end
                    ( *device_map )[pollfds[pollfds_end - 1].fd]->pollfds_i = hcm.device->pollfds_i;  
                    pollfds[hcm.device->pollfds_i] = pollfds[pollfds_end - 1];
                    pollfds_end--;
                    close( hcm.device->fd );
                    device_map->erase( hcm.device->fd );
                }
                
                else if( hcm.action == HID_CHANNEL_QUIT )
                {
                    close( hid_channel_r );
                    return;
                }

            }
            
            break;
        }
    }
}

void Hid_quit()
{
    if( !g_hid_init )
        return;
    
    hid_channel_msg hcm = { HID_CHANNEL_QUIT, NULL };
    write( hid_channel_w, &hcm, sizeof( hcm ) );
    close( hid_channel_w );
        
    delete[] pollfds;
    delete device_map;
        
    g_hid_init = FALSE;
}

int TiltSensor_read( t_CKINT * x, t_CKINT * y, t_CKINT * z )
{
    return 0;
}

/*****************************************************************************
Linux joystick support
*****************************************************************************/
#pragma mark Linux joystick support

void Joystick_init()
{
    if( joysticks != NULL )
        return;
    
    EM_log( CK_LOG_INFO, "initializing joysticks" );
    EM_pushlog();
        
    joysticks = new vector< linux_joystick * >;
        
    DIR * dir_handle;
    struct dirent * dir_entity;
    struct stat stat_buf;
    int js_num, fd, i;
    uid_t uid = geteuid();
    gid_t gid = getegid();
    char buf[CK_HID_STRBUFSIZE];
    linux_joystick * js;

    dir_handle = opendir( CK_HID_DIR );
    if( dir_handle == NULL )
    {
        if( errno == EACCES )
            EM_log( CK_LOG_WARNING, "hid: error opening %s, unable to initialize joystick", CK_HID_DIR );
        EM_poplog();
        return;
    }
    
    while( dir_entity = readdir( dir_handle ) )
    {
        if( sscanf( dir_entity->d_name, CK_HID_JOYSTICKFILE, &js_num ) )
        {
            snprintf( buf, CK_HID_STRBUFSIZE, "%s/%s", CK_HID_DIR, 
                      dir_entity->d_name );
            if( ( fd = open( buf, O_RDONLY | O_NONBLOCK ) ) >= 0 || 
                errno == EACCES ) /* wait to report access errors until the 
                                     device is actually opened */
            {
                js = new linux_joystick;
                js->js_num = js_num;
                js->num = joysticks->size();
                if( fd >= 0 )
                {
                    ioctl( fd, JSIOCGNAME( CK_HID_NAMESIZE ), js->name );
                    close( fd ); // no need to keep the file open
                }
                strncpy( js->filename, buf, CK_HID_STRBUFSIZE );
                joysticks->push_back( js );
                EM_log( CK_LOG_INFO, "joystick: found device %s", js->name );
            }
        }
    }

    closedir( dir_handle );
    EM_poplog();
}

void Joystick_poll()
{
    
}

void Joystick_quit()
{
    if( joysticks == NULL )
        return;
    
    vector< linux_joystick * >::size_type i, len = joysticks->size();
    
    for( i = 0; i < len; i++ )
        delete joysticks->at( i );
    
    delete joysticks;
    joysticks = NULL;
}

int Joystick_count()
{
    if( joysticks == NULL )
        return 0;
    return joysticks->size();
}

int Joystick_open( int js )
{
    if( joysticks == NULL || js < 0 || js >= joysticks->size() )
        return -1;
    
    linux_joystick * joystick = joysticks->at( js );
        
    if( joystick->refcount == 0 )
    {
        if( ( joystick->fd = open( joystick->filename, O_RDONLY | O_NONBLOCK ) ) < 0 )
        {
            EM_log( CK_LOG_SEVERE, "joystick: unable to open %s: %s", joystick->filename, strerror( errno ) );
            return -1;
        }
        
        hid_channel_msg hcm = { HID_CHANNEL_OPEN, joystick };
        write( hid_channel_w, &hcm, sizeof( hcm ) );
    }
    
    joystick->refcount++;
    
    return 0;
}

int Joystick_close( int js )
{
    if( joysticks == NULL || js < 0 || js >= joysticks->size() )
        return -1;
    
    linux_joystick * joystick = joysticks->at( js );
    
    joystick->refcount--;
    
    if( joystick->refcount == 0 )
    {
        hid_channel_msg hcm = { HID_CHANNEL_CLOSE, joystick };
        write( hid_channel_w, &hcm, sizeof( hcm ) );
    }
    
    
    return 0;
}

const char * Joystick_name( int js )
{
    if( joysticks == NULL || js < 0 || js >= joysticks->size() )
        return NULL;
    
    return joysticks->at( js )->name;
}

#define test_bit( array, bit )    (array[bit/8] & (1<<(bit%8)))

void Mouse_configure( const char * filename )
{
    struct stat statbuf;
    int fd, devmajor, devminor, is_mouse = 0;
    linux_mouse * mouse;
    unsigned char relcaps[(REL_MAX / 8) + 1];
    unsigned char abscaps[(ABS_MAX / 8) + 1];
    unsigned char keycaps[(KEY_MAX / 8) + 1];
    
    if( stat( filename, &statbuf ) == -1 )
        return;
    
    if( S_ISCHR( statbuf.st_mode ) == 0 )
        return; /* not a character device... */
    
    devmajor = ( statbuf.st_rdev & 0xFF00 ) >> 8;
    devminor = ( statbuf.st_rdev & 0x00FF );
    if ( ( devmajor != 13 ) || ( devminor < 64 ) || ( devminor > 96 ) )
        return; /* not an evdev. */
    
    if( ( fd = open( filename, O_RDONLY | O_NONBLOCK ) ) < 0 )
        return;
    
    memset( relcaps, 0, sizeof( relcaps ) );
    memset( abscaps, 0, sizeof( abscaps ) );
    memset( keycaps, 0, sizeof( keycaps ) );
    
    //int num_keys = 0;
    if( ioctl( fd, EVIOCGBIT( EV_KEY, sizeof( keycaps ) ), keycaps ) == -1 )
        return;
    
    is_mouse = 0;
    if( ioctl( fd, EVIOCGBIT( EV_REL, sizeof( relcaps ) ), relcaps ) != -1 )
    {
        if( test_bit( relcaps, REL_X ) && test_bit( relcaps, REL_Y ) && 
            test_bit( keycaps, BTN_MOUSE ) )
        {
            is_mouse = 1;
        }
        
        if( test_bit( relcaps, REL_DIAL ) )
            is_mouse = 1;
    }
    
    if( ioctl( fd, EVIOCGBIT( EV_ABS, sizeof( abscaps ) ), abscaps ) != -1 )
    {
        if( test_bit( abscaps, ABS_X ) && test_bit( abscaps, ABS_Y ) && 
            ( test_bit( keycaps, BTN_MOUSE ) || test_bit( keycaps, BTN_TOUCH ) ) )
            is_mouse = 1;
    }
    
    if( !is_mouse )
        return;
    
    mouse = new linux_mouse;
    mouse->num = mice->size();
    ioctl( fd, EVIOCGNAME( CK_HID_NAMESIZE ), mouse->name );
    if( fd >= 0 )
        close( fd ); // no need to keep the file open
    strncpy( mouse->filename, filename, CK_HID_STRBUFSIZE );
    mice->push_back( mouse );
    EM_log( CK_LOG_INFO, "mouse: found device %s", mouse->name );
}

void Mouse_init()
{
    if( mice != NULL )
        return;
    
    EM_log( CK_LOG_INFO, "initializing mice" );
    EM_pushlog();
        
    mice = new vector< linux_mouse * >;
        
    DIR * dir_handle;
    struct dirent * dir_entity;
    struct stat stat_buf;
    int m_num, fd, i;
    char buf[CK_HID_STRBUFSIZE];
    linux_mouse * mouse;

    dir_handle = opendir( CK_HID_DIR );
    if( dir_handle == NULL )
    {
        EM_log( CK_LOG_WARNING, "hid: error opening %s, unable to initialize mice", CK_HID_DIR );
        EM_poplog();
        return;
    }
    
    while( dir_entity = readdir( dir_handle ) )
    {
        if( sscanf( dir_entity->d_name, CK_HID_EVDEVFILE, &m_num ) )
        {
            snprintf( buf, CK_HID_STRBUFSIZE, "%s/%s", CK_HID_DIR, 
                      dir_entity->d_name );
            Mouse_configure( buf );
/*            if( ( fd = open( buf, O_RDONLY | O_NONBLOCK ) ) >= 0 || 
                errno == EACCES ) *//* wait to report access errors until the 
                                     device is actually opened *//*
            {
                mouse = new linux_mouse;
                mouse->m_num = m_num;
                mouse->num = mice->size();
                //ioctl( fd, JSIOCGNAME( CK_HID_NAMESIZE ), js->name );
                if( fd >= 0 )
                    close( fd ); // no need to keep the file open
                strncpy( mouse->filename, buf, CK_HID_STRBUFSIZE );
                mice->push_back( mouse );
                //EM_log( CK_LOG_INFO, "mouse: found device %s", mouse->name );
            }*/
        }
    }

    closedir( dir_handle );
    EM_poplog();
    
}

void Mouse_poll()
{
    
}

void Mouse_quit()
{
    if( mice == NULL )
        return;
    
    vector< linux_mouse * >::size_type i, len = mice->size();
    
    for( i = 0; i < len; i++ )
        delete mice->at( i );
    
    delete mice;
    mice = NULL;
}

int Mouse_count()
{
    if( !mice )
        return 0;
    return mice->size();
}

int Mouse_open( int m )
{
    if( mice == NULL || m < 0 || m >= mice->size() )
        return -1;
    
    linux_mouse * mouse = mice->at( m );
        
    if( mouse->refcount == 0 )
    {
        if( ( mouse->fd = open( mouse->filename, O_RDONLY | O_NONBLOCK ) ) < 0 )
        {
            EM_log( CK_LOG_SEVERE, "mouse: unable to open %s: %s", mouse->filename, strerror( errno ) );
            return -1;
        }
        
        mouse->needs_open = TRUE;
        hid_channel_msg hcm = { HID_CHANNEL_OPEN, mouse };
        write( hid_channel_w, &hcm, sizeof( hcm ) );
    }
    
    mouse->refcount++;
    
    return 0;
}

int Mouse_close( int m )
{
    if( mice == NULL || m < 0 || m >= mice->size() )
        return -1;
    
    linux_mouse * mouse = mice->at( m );
    
    mouse->refcount--;
    
    if( mouse->refcount == 0 )
    {
        mouse->needs_close = TRUE;
        hid_channel_msg hcm = { HID_CHANNEL_CLOSE, mouse };
        write( hid_channel_w, &hcm, sizeof( hcm ) );
    }
    
    return 0;
}

const char * Mouse_name( int m )
{
    if( mice == NULL || m < 0 || m >= mice->size() )
        return NULL;
    return mice->at( m )->name;
}
    
void Keyboard_configure( const char * filename )
{
    struct stat statbuf;
    int fd, devmajor, devminor;
    linux_keyboard * keyboard;
    unsigned char relcaps[(REL_MAX / 8) + 1];
    unsigned char abscaps[(ABS_MAX / 8) + 1];
    unsigned char keycaps[(KEY_MAX / 8) + 1];
    
    if( stat( filename, &statbuf ) == -1 )
        return;

    if( S_ISCHR( statbuf.st_mode ) == 0 )
        return; /* not a character device... */
    
    devmajor = ( statbuf.st_rdev & 0xFF00 ) >> 8;
    devminor = ( statbuf.st_rdev & 0x00FF );
    if ( ( devmajor != 13 ) || ( devminor < 64 ) || ( devminor > 96 ) )
        return; /* not an evdev. */
    
    if( ( fd = open( filename, O_RDONLY | O_NONBLOCK ) ) < 0 )
        return;
        
    memset( relcaps, 0, sizeof( relcaps ) );
    memset( abscaps, 0, sizeof( abscaps ) );
    memset( keycaps, 0, sizeof( keycaps ) );
    
    int num_keys = 0;
    if( ioctl( fd, EVIOCGBIT( EV_KEY, sizeof( keycaps ) ), keycaps ) == -1 )
        return;
        
    if( ioctl( fd, EVIOCGBIT( EV_REL, sizeof( relcaps ) ), relcaps ) != -1 )
    {
        for( int i = 0; i < sizeof( relcaps ); i++ )
            if( relcaps[i] )
                return;
    }
    
    if( ioctl( fd, EVIOCGBIT( EV_ABS, sizeof( abscaps ) ), abscaps ) != -1 )
    {
        for( int i = 0; i < sizeof( abscaps ); i++ )
            if( abscaps[i] )
                return;
    }
    
    for( int i = 0; i < sizeof( keycaps ); i++ )
    {
        for( int j = 0; j < 8; j++ )
        {
            if( keycaps[i] & ( 1 << j ) )
                num_keys++;
        }
    }
    
    keyboard = new linux_keyboard;
    keyboard->num = keyboards->size();
    ioctl( fd, EVIOCGNAME( CK_HID_NAMESIZE ), keyboard->name );
    if( fd >= 0 )
        close( fd ); // no need to keep the file open
    strncpy( keyboard->filename, filename, CK_HID_STRBUFSIZE );
    keyboards->push_back( keyboard );
    EM_log( CK_LOG_INFO, "keyboard: found device %s", keyboard->name );
}

void Keyboard_init()
{
    if( keyboards != NULL )
        return;
    
    EM_log( CK_LOG_INFO, "initializing keyboards" );
    EM_pushlog();
        
    keyboards = new vector< linux_keyboard * >;
        
    DIR * dir_handle;
    struct dirent * dir_entity;
    
    int m_num;
        
    char buf[CK_HID_STRBUFSIZE];
    
    dir_handle = opendir( CK_HID_DIR );
    if( dir_handle == NULL )
    {
        if( errno == EACCES )
            EM_log( CK_LOG_WARNING, "hid: error opening %s, unable to initialize keyboards", CK_HID_DIR );
        EM_poplog();
        return;
    }
    
    while( dir_entity = readdir( dir_handle ) )
    {
        if( sscanf( dir_entity->d_name, CK_HID_EVDEVFILE, &m_num ) )
        {
            snprintf( buf, CK_HID_STRBUFSIZE, "%s/%s", CK_HID_DIR, 
                      dir_entity->d_name );
            Keyboard_configure( buf );
        }
    }

    closedir( dir_handle );
    EM_poplog();

}

void Keyboard_poll()
{
    
}

void Keyboard_quit()
{
    
}

int Keyboard_count()
{
    if( keyboards == NULL )
        return 0;
    return keyboards->size();
}

int Keyboard_open( int k )
{
    if( keyboards == NULL || k < 0 || k >= keyboards->size() )
        return -1;
    
    linux_keyboard * keyboard = keyboards->at( k );
        
    if( keyboard->refcount == 0 )
    {
        if( ( keyboard->fd = open( keyboard->filename, O_RDONLY | O_NONBLOCK ) ) < 0 )
        {
            EM_log( CK_LOG_SEVERE, "keyboard: unable to open %s: %s", keyboard->filename, strerror( errno ) );
            return -1;
        }
        
        hid_channel_msg hcm = { HID_CHANNEL_OPEN, keyboard };
        write( hid_channel_w, &hcm, sizeof( hcm ) );
    }
    
    keyboard->refcount++;
    
    return 0;
}

int Keyboard_close( int k )
{
    if( keyboards == NULL || k < 0 || k >= keyboards->size() )
        return -1;
    
    linux_keyboard * keyboard = keyboards->at( k );
    
    keyboard->refcount--;
    
    if( keyboard->refcount == 0 )
    {
        hid_channel_msg hcm = { HID_CHANNEL_CLOSE, keyboard };
        write( hid_channel_w, &hcm, sizeof( hcm ) );
    }
    
    return 0;
}

const char * Keyboard_name( int k )
{
    if( keyboards == NULL || k < 0 || k >= keyboards->size() )
        return NULL;
    
    return keyboards->at( k )->name;
}

#endif


#pragma mark Hid graveyard
/***** empty functions for stuff that isn't yet cross platform *****/
#ifndef __PLATFORM_MACOSX__
/*** empty functions for Mac-only stuff ***/

int Mouse_start_cursor_track(){ return -1; }
int Mouse_stop_cursor_track(){ return -1; }

int Keyboard_send( int k, const HidMsg * msg ){ return -1; }
int WiiRemote_send( int k, const HidMsg * msg ){ return -1; }

void WiiRemote_init(){}
void WiiRemote_poll(){}
void WiiRemote_quit(){}
void WiiRemote_probe(){}
int WiiRemote_count(){ return 0; }
int WiiRemote_open( int wr ){ return -1; }
int WiiRemote_open( const char * name ){ return -1; }
int WiiRemote_close( int wr ){ return -1; }
int WiiRemote_send( const HidMsg * msg ){ return -1; }
const char * WiiRemote_name( int wr ){ return NULL; }

void TiltSensor_init(){}
void TiltSensor_quit(){}
void TiltSensor_probe(){}
int TiltSensor_count(){ return 0; }
int TiltSensor_open( int ts ){ return -1; }
int TiltSensor_close( int ts ){ return -1; }
int TiltSensor_read( int ts, int type, int num, HidMsg * msg ){ return -1; }
const char * TiltSensor_name( int ts ){ return NULL; }


#endif



