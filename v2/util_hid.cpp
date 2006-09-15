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
// file: util_hid.c
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

#ifdef __MACOSX_CORE__
#pragma mark OS X General HID support

#include <mach/mach.h>
#include <mach/mach_error.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <Kernel/IOKit/hidsystem/IOHIDUsageTables.h>
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <CoreFoundation/CoreFoundation.h>

void Hid_callback( void * target, IOReturn result, 
                   void * refcon, void * sender );

struct OSX_Device_Element
{
    OSX_Device_Element()
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

struct OSX_Device
{
    OSX_Device()
    {
        configed = preconfiged = FALSE;
        plugInInterface = NULL;
        interface = NULL;
        queue = NULL;
        strncpy( name, "Device", 256 );
        elements = NULL;
        usage = usage_page = 0;
        type = num = 0;
        buttons = -1;
        axes = -1;
        hats = -1;
        refcount = 0;
        stop_queue = add_to_run_loop = FALSE;
        hidProperties = NULL;
    }
    
    t_CKINT preconfigure( io_object_t ioHIDDeviceObject, t_CKINT dev_type );
    t_CKINT configure();
    void enumerate_elements( CFArrayRef cfElements );
    
    IOCFPlugInInterface ** plugInInterface;
    IOHIDDeviceInterface ** interface; // device interface
    IOHIDQueueInterface ** queue; // device event queue
    
    char name[256];
    
    t_CKBOOL preconfiged, configed;
    
    t_CKINT type;
    t_CKINT num;
    t_CKINT usage_page;
    t_CKINT usage;
    
    map< IOHIDElementCookie, OSX_Device_Element * > * elements;
    /* Note: setting any of these to -1 informs the element enumerating 
        method that we are not interested in that particular element type */
    t_CKINT buttons;
    t_CKINT axes;
    t_CKINT hats;
    
    t_CKUINT refcount; // incremented on open, decremented on close
    t_CKBOOL add_to_run_loop; // used to indicate that the event source needs to be added to the run loop 
    t_CKBOOL stop_queue; // used to indicate that the event queue should be stopped
    
    CFRunLoopSourceRef eventSource;
    
    CFMutableDictionaryRef hidProperties;
    
    const static t_CKINT event_queue_size = 50; 
    // queues use wired kernel memory so should be set to as small as possible
    // but should account for the maximum possible events in the queue
    // USB updates will likely occur at 100 Hz so one must account for this rate
    // if states change quickly (updates are only posted on state changes)
};

t_CKINT OSX_Device::preconfigure( io_object_t ioHIDDeviceObject, t_CKINT dev_type )
{
    if( preconfiged )
        return 0;
    
    // retrieve a dictionary of device properties
    kern_return_t kern_result = IORegistryEntryCreateCFProperties( ioHIDDeviceObject, 
                                                                   &hidProperties, 
                                                                   kCFAllocatorDefault, 
                                                                   kNilOptions);
    if( kern_result != KERN_SUCCESS || hidProperties == 0 )
    {
        hidProperties = NULL;
        return -1;
    }
    
    switch( dev_type )
    {
        case CK_HID_DEV_JOYSTICK:
            strncpy( name, "Joystick", 256 );
            axes = 0;
            hats = 0;
            buttons = 0;
            break;
            
        case CK_HID_DEV_MOUSE:
            strncpy( name, "Mouse", 256 );
            axes = 0;
            hats = -1;
            buttons = 0;
            break;
            
        case CK_HID_DEV_KEYBOARD:
            strncpy( name, "Keyboard", 256 );
            axes = -1;
            hats = -1;
            buttons = 0;
            break;
    }

    // get the device name, and copy it into the device record
    CFTypeRef refCF = CFDictionaryGetValue( hidProperties, 
                                            CFSTR( kIOHIDProductKey ) );
    if( refCF )
        CFStringGetCString( ( CFStringRef )refCF, name, 256, 
                            kCFStringEncodingASCII );
    
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
    
    preconfiged = TRUE;
    
    return 0;
}

t_CKINT OSX_Device::configure()
{
    if( configed )
        return 0;
    
    if( !preconfiged )
        return -1;
    
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
    elements = new map< IOHIDElementCookie, OSX_Device_Element * >;
    // axes = 0;
    // buttons = 0;
    // hats = 0;
    
    // ...and parse the element array recursively
    enumerate_elements( ( CFArrayRef ) refCF );
    
    CFRelease( hidProperties );
    hidProperties = NULL;
    
    configed = TRUE;
    
    return 0;
}

//------------------------------------------------------------------------------
// name: enumerate_device_elements
// desc: perform depth depth first search on potentially nested arrays of 
// elements on the device, add found elements to the vector
//------------------------------------------------------------------------------
void OSX_Device::enumerate_elements( CFArrayRef cfElements )
{
    CFTypeRef refCF = 0;
    CFDictionaryRef element_dictionary;
    OSX_Device_Element * element;
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
                            case kHIDUsage_GD_Wheel:
                                // this is an axis
                                if( axes == -1 )
                                    continue;
                                
                                element = new OSX_Device_Element;
                                
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
                                
                            case kHIDUsage_GD_Hatswitch:
                                // this is a hat
                                if( hats == -1 )
                                    continue;

                                element = new OSX_Device_Element; 
                                    
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
                        
                        element = new OSX_Device_Element; 
                        
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
                }
                
                break;
                
            case kIOHIDElementTypeCollection:
                refCF = CFDictionaryGetValue( element_dictionary, CFSTR( kIOHIDElementKey ) );
                if( !refCF )
                    continue;
                    
                enumerate_elements( ( CFArrayRef ) refCF );
                break;
        }
    }
}

static vector< OSX_Device * > * joysticks = NULL;
static vector< OSX_Device * > * mice = NULL;
static vector< OSX_Device * > * keyboards = NULL;

CFRunLoopRef rlHid;
CFStringRef kCFRunLoopChuckHidMode = CFSTR( "ChucKHid" );
CFRunLoopSourceRef cfrlDummySource;
static t_CKBOOL g_hid_init = FALSE;

void Hid_init()
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
    joysticks = new vector< OSX_Device * >;
    mice = new vector< OSX_Device * >;
    keyboards = new vector< OSX_Device * >;
    
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
        
    result = IOServiceGetMatchingServices( kIOMasterPortDefault, 
                                           hidMatchDictionary, 
                                           &hidObjectIterator );
    if( result != kIOReturnSuccess || hidObjectIterator == 0 )
    {
        EM_log( CK_LOG_SEVERE, "hid: error: unable to retrieving matching services, unable to initialize" );
        return;
    }

    CFRelease( refCF );
    
    io_object_t ioHIDDeviceObject = 0;
    t_CKINT usage, usage_page;
    t_CKINT joysticks_seen = 0, mice_seen = 0, keyboards_seen = 0;
    while( ioHIDDeviceObject = IOIteratorNext( hidObjectIterator ) )
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
                EM_log( CK_LOG_INFO, "joystick: configuring joystick %i", joysticks_seen++ );
                EM_pushlog();
                
                // allocate the device record, set usage page and usage
                OSX_Device * new_device = new OSX_Device;
                new_device->type = CK_HID_DEV_JOYSTICK;
                new_device->num = joysticks->size();
                new_device->usage_page = usage_page;
                new_device->usage = usage;
                
                if( !new_device->preconfigure( ioHIDDeviceObject, CK_HID_DEV_JOYSTICK ) )
                    joysticks->push_back( new_device );
                else
                {
                    EM_log( CK_LOG_INFO, "joystick: error during configuration" );
                    delete new_device;
                }
                
                EM_poplog();
            }
            
            if( usage == kHIDUsage_GD_Mouse )
                // this is a mouse
            {
                EM_log( CK_LOG_INFO, "mouse: configuring mouse %i", mice_seen++ );
                EM_pushlog();
                
                // allocate the device record, set usage page and usage
                OSX_Device * new_device = new OSX_Device;
                new_device->type = CK_HID_DEV_MOUSE;
                new_device->usage_page = usage_page;
                new_device->usage = usage;
                
                if( !new_device->preconfigure( ioHIDDeviceObject, CK_HID_DEV_MOUSE ) )
                    mice->push_back( new_device );
                else
                {
                    EM_log( CK_LOG_INFO, "mouse: error during configuration" );
                    delete new_device;
                }
                
                EM_poplog();
            }

            if( usage == kHIDUsage_GD_Keyboard || usage == kHIDUsage_GD_Keypad )
                // this is a keyboard
            {
                EM_log( CK_LOG_INFO, "keyboard: configuring keyboard %i", 
                        keyboards_seen++ );
                EM_pushlog();
                
                // allocate the device record, set usage page and usage
                OSX_Device * new_device = new OSX_Device;
                new_device->type = CK_HID_DEV_KEYBOARD;
                new_device->usage_page = usage_page;
                new_device->usage = usage;
                
                if( !new_device->preconfigure( ioHIDDeviceObject, 
                                               CK_HID_DEV_KEYBOARD ) )
                    keyboards->push_back( new_device );
                else
                {
                    EM_log( CK_LOG_INFO, 
                            "keyboard: error during configuration" );
                    delete new_device;
                }
                
                EM_poplog();
            }
        }
    }
    
    IOObjectRelease( hidObjectIterator );
}

// callback for the CFRunLoop dummy source
void Hid_cfrl_callback( void * info )
{
    CFRunLoopStop( rlHid );
}

void Hid_poll()
{
    if( rlHid == NULL )
    {
        rlHid = CFRunLoopGetCurrent();
        
        // we set up a dummy source to use in case no hid devices are open.
        // this is important because the CFRunLoop will stop immediately if it
        // has no sources.  Thus, Hid_poll() will run without ever blocking on
        // input, which will kill the CPU.  So we set up a dummy source that is
        // always present, but never activates.  
        CFRunLoopSourceContext cfrlSourceContext;
        cfrlSourceContext.version = 0;
        cfrlSourceContext.info = NULL;
        cfrlSourceContext.retain = NULL;
        cfrlSourceContext.release = NULL;
        cfrlSourceContext.copyDescription = NULL;
        cfrlSourceContext.equal = NULL;
        cfrlSourceContext.hash = NULL;
        cfrlSourceContext.schedule = NULL;
        cfrlSourceContext.cancel = NULL;
        cfrlSourceContext.perform = Hid_cfrl_callback;
        
        cfrlDummySource = CFRunLoopSourceCreate( kCFAllocatorDefault, 0, 
                                                 &cfrlSourceContext );
        CFRunLoopAddSource( rlHid, cfrlDummySource, kCFRunLoopChuckHidMode );
    }
    
    // TODO: set this up to use a pipe or circular buffer, so that we dont have
    // to iterate through every device
    OSX_Device * device;
    vector< OSX_Device * >::size_type i, len = joysticks->size();
    for( i = 0; i < len; i++ )
    {
        device = joysticks->at( i );
        if( device->add_to_run_loop )
        {
            EM_log( CK_LOG_INFO, "joystick: adding device %i to run loop", i );
            device->add_to_run_loop = FALSE;
            CFRunLoopAddSource( rlHid, device->eventSource, 
                                kCFRunLoopChuckHidMode );
        }
        
        if( device->stop_queue )
        {
            device->stop_queue = FALSE;
            
            CFRunLoopRemoveSource( rlHid, device->eventSource, 
                                   kCFRunLoopChuckHidMode );
            
            IOReturn result = ( *( device->queue ) )->stop( device->queue );
            if( result != kIOReturnSuccess )
                EM_log( CK_LOG_INFO, "joystick: error stopping event queue" );
        }
    }
    
    len = mice->size();
    for( i = 0; i < len; i++ )
    {
        device = mice->at( i );
        if( device->add_to_run_loop )
        {
            EM_log( CK_LOG_INFO, "mouse: adding device %i to run loop", i );
            device->add_to_run_loop = FALSE;
            CFRunLoopAddSource( rlHid, device->eventSource, 
                                kCFRunLoopChuckHidMode );
        }
        
        if( device->stop_queue )
        {
            device->stop_queue = FALSE;
            
            CFRunLoopRemoveSource( rlHid, device->eventSource, 
                                   kCFRunLoopChuckHidMode );
            
            IOReturn result = ( *( device->queue ) )->stop( device->queue );
            if( result != kIOReturnSuccess )
                EM_log( CK_LOG_INFO, "mouse: error stopping event queue" );
        }
    }
    
    len = keyboards->size();
    for( i = 0; i < len; i++ )
    {
        device = keyboards->at( i );
        if( device->add_to_run_loop )
        {
            EM_log( CK_LOG_INFO, "keyboard: adding device %i to run loop", i );
            device->add_to_run_loop = FALSE;
            CFRunLoopAddSource( rlHid, device->eventSource, 
                                kCFRunLoopChuckHidMode );
        }
        
        if( device->stop_queue )
        {
            device->stop_queue = FALSE;
            
            CFRunLoopRemoveSource( rlHid, device->eventSource, 
                                   kCFRunLoopChuckHidMode );
            
            IOReturn result = ( *( device->queue ) )->stop( device->queue );
            if( result != kIOReturnSuccess )
                EM_log( CK_LOG_INFO, "keyboard: error stopping event queue" );
        }
    }
    
    CFRunLoopRunInMode( kCFRunLoopChuckHidMode, 60 * 60 * 24, false );
}

void Hid_callback( void * target, IOReturn result, 
                   void * refcon, void * sender)
{
    OSX_Device * device = ( OSX_Device * ) refcon;
    OSX_Device_Element * element;
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
        
        msg.device_type = device->type;
        msg.device_num = device->num;
        msg.type = element->type;
        msg.eid = element->num;
        
        switch( msg.type )
        {
            case CK_HID_JOYSTICK_AXIS:
                // quick and dirty scaling of the value to [-1.0, 1.0]
                msg.fdata[0] = ((t_CKFLOAT)(event.value - element->min)) * 2.0 / ((t_CKFLOAT)(element->max - element->min)) - 1.0;
                break;
                
            case CK_HID_JOYSTICK_HAT:
                msg.idata[0] = event.value;
                break;
                
            case CK_HID_MOUSE_MOTION:
                if( element->usage = kHIDUsage_GD_X )
                {
                    msg.idata[0] = event.value;
                    msg.idata[1] = 0;
                }
                
                else
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
                    msg.eid = element->usage;
                break;
        }
        
        HidInManager::push_message( msg );
    }
}

void Hid_quit()
{
    // stop the run loop; since thread_going is FALSE, the poll thread will exit
    if( rlHid )
        CFRunLoopStop( rlHid );  
    rlHid = NULL;
}

void Hid_quit2()
{
    if( g_hid_init == FALSE )
        return;
    g_hid_init = FALSE;
    
    vector< OSX_Device * >::size_type i, len = joysticks->size();
    map< IOHIDElementCookie, OSX_Device_Element * >::iterator iter, end;
    OSX_Device * joystick;
    for( i = 0; i < len; i++ )
    {
        joystick = joysticks->at( i );
        
        if( joystick->elements )
        {
            // iterate through the axis map, delete device element records
            end = joystick->elements->end();
            for( iter = joystick->elements->begin(); iter != end; iter++ )
                delete iter->second;
            delete joystick->elements;
            joystick->axes = NULL;
        }
        
        if( joystick->interface )
        {
            ( *( joystick->interface ) )->close( joystick->interface );
            ( *( joystick->interface ) )->Release( joystick->interface );
            joystick->interface = NULL;
        }
        
        if( joystick->queue )
        {
            if( joystick->refcount > 0 )
                ( *( joystick->queue ) )->stop( joystick->queue );
            ( *( joystick->queue ) )->dispose( joystick->queue );
            ( *( joystick->queue ) )->Release( joystick->queue );
            joystick->queue = NULL;
        }
        
        delete joystick;
    }
    
    delete joysticks;
    joysticks = NULL;
}


#pragma mark OS X Joystick support

void Joystick_init()
{
    Hid_init();
}

void Joystick_poll()
{
    if( joysticks == NULL )
        return;
    
    // for each open device, poll the event queue
    vector< OSX_Device * >::size_type i, len = joysticks->size();
    for( i = 0; i < len; i++ )
        if( joysticks->at( i )->refcount > 0 )
            Hid_callback( NULL, kIOReturnSuccess, joysticks->at( i ), NULL );
}

void Joystick_quit()
{
    Hid_quit2();
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
    
    OSX_Device * joystick = joysticks->at( js );

    if( joystick->refcount == 0 )
    {
        if( joystick->configure() )
        {
            EM_log( CK_LOG_SEVERE, "joystick: error in joystick configuration" );
            return -1;
        }
        
        IOReturn result = ( *( joystick->queue ) )->start( joystick->queue );
        if( result != kIOReturnSuccess )
        {
            EM_log( CK_LOG_SEVERE, "joystick: error starting event queue" );
            return -1;
        }

        joystick->add_to_run_loop = TRUE;
        if( rlHid )
            CFRunLoopStop( rlHid );
    }
    
    joystick->refcount++;
    
    return 0;
}

int Joystick_close( int js )
{
    if( joysticks == NULL || js < 0 || js >= joysticks->size() )
        return -1;
    
    OSX_Device * joystick = joysticks->at( js );
    
    if( joystick->refcount > 0 )
    {
        joystick->refcount--;
        
        if( joystick->refcount == 0 )
        {
            joystick->stop_queue = TRUE;
            if( rlHid )
                CFRunLoopStop( rlHid );
        }
    }
    
    else
    {
        EM_log( CK_LOG_INFO, "joystick: warning: joystick %i closed when not open", js );
        return -1;
    }
    
    return 0;
}

#pragma mark OS X Mouse support

void Mouse_init()
{
    Hid_init();
}

void Mouse_poll()
{
    if( mice == NULL )
        return;
    
    // for each open device, poll the event queue    
    vector< OSX_Device * >::size_type i, len = mice->size();
    for( i = 0; i < len; i++ )
        if( mice->at( i )->refcount > 0 )
            Hid_callback( NULL, kIOReturnSuccess, mice->at( i ), NULL );
}

void Mouse_quit()
{
    Hid_quit2();
}

int Mouse_count()
{
    if( mice == NULL )
        return 0;
    return mice->size();
}

int Mouse_open( int m )
{
    if( mice == NULL || m < 0 || m >= mice->size() )
        return -1;
        
    OSX_Device * mouse = mice->at( m );
    
    if( mouse->refcount == 0 )
    {
        if( mouse->configure() )
        {
            EM_log( CK_LOG_SEVERE, "mouse: error in mouse configuration" );
            return -1;
        }
        
        IOReturn result = ( *( mouse->queue ) )->start( mouse->queue );
        if( result != kIOReturnSuccess )
        {
            EM_log( CK_LOG_SEVERE, "mouse: error starting event queue" );
            return -1;
        }
        
        mouse->add_to_run_loop = TRUE;
        if( rlHid )
            CFRunLoopStop( rlHid );        
    }
    
    mouse->refcount++;
    
    return 0;
}

int Mouse_close( int m )
{
    if( mice == NULL || m < 0 || m >= mice->size() )
        return -1;
    
    OSX_Device * mouse = mice->at( m );
    
    if( mouse->refcount > 0 )
    {
        mouse->refcount--;
        
        if( mouse->refcount == 0 )
        {
            mouse->stop_queue = TRUE;
            if( rlHid )
                CFRunLoopStop( rlHid );
        }
    }
    
    else
    {
        EM_log( CK_LOG_INFO, "mouse: warning: mouse %i closed when not open", m );
        return -1;
    }
    
    return 0;
}

#pragma mark OS X Keyboard support
void Keyboard_init()
{
    Hid_init();
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
    if( keyboards == NULL )
        return -1;
    return keyboards->size();
}

int Keyboard_open( int k )
{
    if( keyboards == NULL || k < 0 || k >= keyboards->size() )
        return -1;
    
    OSX_Device * keyboard = keyboards->at( k );
    
    if( keyboard->refcount == 0 )
    {
        if( keyboard->configure() )
        {
            EM_log( CK_LOG_SEVERE, "keyboard: error in keyboard configuration" );
            return -1;
        }
        
        IOReturn result = ( *( keyboard->queue ) )->start( keyboard->queue );
        if( result != kIOReturnSuccess )
        {
            EM_log( CK_LOG_SEVERE, "keyboard: error starting event queue" );
            return -1;
        }
        
        keyboard->add_to_run_loop = TRUE;
        if( rlHid )
            CFRunLoopStop( rlHid );
    }
    
    keyboard->refcount++;
    
    return 0;
}

int Keyboard_close( int k )
{
    if( keyboards == NULL || k < 0 || k >= keyboards->size() )
        return -1;
    
    OSX_Device * keyboard = keyboards->at( k );
    
    if( keyboard->refcount > 0 )
    {
        keyboard->refcount--;
        
        if( keyboard->refcount == 0 )
        {
            keyboard->stop_queue = TRUE;
            if( rlHid )
                CFRunLoopStop( rlHid );
        }
    }
    
    else
    {
        EM_log( CK_LOG_INFO, "keyboard: warning: keyboard %i closed when not open", k );
        return -1;
    }
    
    return 0;
}

#elif ( defined( __PLATFORM_WIN32__ ) || defined( __WINDOWS_PTHREAD__ ) ) && !defined( USE_RAWINPUT )
/*****************************************************************************
Windows general HID support
*****************************************************************************/
#pragma mark Window general HID support

void Hid_init()
{
    
}

void Hid_poll()
{
    usleep( 10 );
    Joystick_poll();
    Keyboard_poll();
    Mouse_poll();
}

void Hid_quit()
{
    
}

/*****************************************************************************
Windows joystick support
*****************************************************************************/
#pragma mark Windows joystick support

#include <windows.h>
#define DIRECTINPUT_VERSION 0x0500
#include <dinput.h>

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
	GUID guid = lpddi->guidProduct;
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

void Joystick_poll()
{
	if( !joysticks )
		return;

	win32_joystick * joystick;
	HidMsg msg;
	vector< win32_joystick * >::size_type i, len = joysticks->size();
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

			for( int j = 0; j < joystick->caps.dwButtons && j < 128; j++ )
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

		if( joystick->lpdiJoystick->Acquire() != DI_OK )
		{
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
	GUID guid = lpddi->guidProduct;
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

		if( keyboard->lpdiKeyboard->Acquire() != DI_OK )
		{
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
	GUID guid = lpddi->guidProduct;
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

		if( mouse->lpdiMouse->Acquire() != DI_OK )
		{
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

#define CK_HID_DIR "/dev/input"
#define CK_HID_MOUSEFILE "mouse%d"
#define CK_HID_JOYSTICKFILE "js%d"
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
        strncpy( name, "Joystick", 128 );
    }
    
    virtual void callback() = 0;
    
    int fd;       // file descriptor
    t_CKINT num; // index in the joysticks vector
    
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
        ps2_mouse_event event;
        HidMsg msg;
        ssize_t len;
                
        while( ( len = read( fd, &event, sizeof( event ) ) ) > 0 )
        {
            if( len < sizeof( event ) )
            {
                EM_log( CK_LOG_WARNING, "mouse: read event from mouse %i smaller than expected (%i), ignoring", num, len );
                continue;
            }
            
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
            
            memcpy( &last_event, &event, sizeof( last_event ) );
        }
    }
    
    int m_num;   // /dev/input/mouse# <-- the #
    ps2_mouse_event last_event;
};

static vector< linux_joystick * > * joysticks = NULL;
static vector< linux_mouse * > * mice = NULL;

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
        if( errno == EACCES )
            EM_log( CK_LOG_WARNING, "hid: error opening %s, unable to initialize mice", CK_HID_DIR );
        EM_poplog();
        return;
    }
    
    while( dir_entity = readdir( dir_handle ) )
    {
        if( sscanf( dir_entity->d_name, CK_HID_MOUSEFILE, &m_num ) )
        {
            snprintf( buf, CK_HID_STRBUFSIZE, "%s/%s", CK_HID_DIR, 
                      dir_entity->d_name );
            if( ( fd = open( buf, O_RDONLY | O_NONBLOCK ) ) >= 0 || 
                errno == EACCES ) /* wait to report access errors until the 
                                     device is actually opened */
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
            }
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

#endif

