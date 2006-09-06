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
        buttons = -1;
        axes = -1;
        hats = -1;
        refcount = 0;
        stop_queue = FALSE;
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
    t_CKINT usage_page;
    t_CKINT usage;
    
    /* Note: setting any of these to -1 informs the element enumerating 
        method that we are not interested in that particular element type */
    map< IOHIDElementCookie, OSX_Device_Element * > * elements;
    t_CKINT buttons;
    t_CKINT axes;
    t_CKINT hats;
    
    t_CKUINT refcount; // incremented on open, decremented on close
    t_CKBOOL stop_queue; // used to indicate that the event queue should be stopped
    
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
            buttons = -1;
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
    axes = 0;
    buttons = 0;
    hats = 0;
    
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
                        // this is a button
                        if( buttons == -1 )
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
static t_CKBOOL g_hid_init = FALSE;

void HID_init()
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
    t_CKINT joysticks_seen = 0, mice_seen = 0;
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
        }
    }
    
    IOObjectRelease( hidObjectIterator );
}

void HID_quit()
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
    HID_init();
}

void Joystick_poll()
{
    if( joysticks == NULL )
        return;
    
    // for each open device, poll the event queue
    OSX_Device * joystick;
    OSX_Device_Element * element;
    IOReturn result;
    AbsoluteTime atZero = { 0, 0 };
    IOHIDEventStruct event;
    HidMsg msg;
    
    vector< OSX_Device * >::size_type i, len = joysticks->size();
    for( i = 0; i < len; i++ )
    {
        joystick = joysticks->at( i );
        if( joystick->refcount > 0 )
        {
            // TODO: poll axis elements directly, so we can use less 
            // memory in the event queue?
            result = ( *( joystick->queue ) )->getNextEvent( joystick->queue, 
                                                             &event, atZero, 0 );
            if( result == kIOReturnUnderrun )
                continue;
            if( result != kIOReturnSuccess )
            {
                EM_log( CK_LOG_INFO, "joystick: warning: getNextEvent failed" );
                continue;
            }
            
            element = ( *( joystick->elements ) )[event.elementCookie];
            
            msg.device_type = CK_HID_DEV_JOYSTICK;
            msg.device_num = i;
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
                    
                case CK_HID_BUTTON_DOWN:
                    if( event.value == 0 )
                        msg.type = CK_HID_BUTTON_UP;
                    msg.idata[0] = event.value;
                    break;
            }
            
            HidInManager::push_message( msg );
        }
    }
}

void Joystick_quit()
{
    HID_quit();
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
            /* TODO: make this thread safe without using a mutex */
        {
            IOReturn result = ( *( joystick->queue ) )->stop( joystick->queue );
            if( result != kIOReturnSuccess )
            {
                EM_log( CK_LOG_INFO, "joystick: error stopping event queue" );
                return -1;
            }
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
    HID_init();
}

void Mouse_poll()
{
    if( mice == NULL )
        return;
    
    // for each open device, poll the event queue
    OSX_Device * mouse;
    OSX_Device_Element * element;
    IOReturn result;
    AbsoluteTime atZero = { 0, 0 };
    IOHIDEventStruct event;
    HidMsg msg;
    
    vector< OSX_Device * >::size_type i, len = mice->size();
    for( i = 0; i < len; i++ )
    {
        if( mice->at( i )->refcount > 0 )
        {
            // TODO: poll axis elements directly, so we can use less 
            // memory in the event queue?
            mouse = mice->at( i );

            result = ( *( mouse->queue ) )->getNextEvent( mouse->queue, 
                                                          &event, atZero, 0 );
            if( result == kIOReturnUnderrun )
                continue;
            if( result != kIOReturnSuccess )
            {
                EM_log( CK_LOG_INFO, "joystick: warning: getNextEvent failed" );
                continue;
            }
            
            element = ( *( mouse->elements ) )[event.elementCookie];
            
            msg.device_type = CK_HID_DEV_MOUSE;
            msg.device_num = i;
            msg.type = element->type;
            msg.eid = element->num;
            
            switch( msg.type )
            {
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
                    break;
            }
            
            HidInManager::push_message( msg );
        }
    }
}

void Mouse_quit()
{
    HID_quit();
}

int Mouse_count()
{
    return 0;
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
        
        // TODO: make this part thread-safe!
        if( mouse->refcount == 0 )
        {
            IOReturn result = ( *( mouse->queue ) )->stop( mouse->queue );
            if( result != kIOReturnSuccess )
            {
                EM_log( CK_LOG_INFO, "mouse: error stopping event queue" );
                return -1;
            }
        }
    }
    
    else
    {
        EM_log( CK_LOG_INFO, "mouse: warning: mouse %i closed when not open", m );
        return -1;
    }
    
    return 0;
}

#elif defined( __PLATFORM_WIN32__ ) || defined( __WINDOWS_PTHREAD__ )
#pragma mark Windows joystick support

#include <windows.h>
#define DIRECTINPUT_VERSION 0x0500
#include <dinput.h>

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

	LPDIRECTINPUTDEVICE lpdiJoystick;
	DIJOYSTATE2 last_state;
	DIDEVCAPS caps;

	char name[MAX_PATH];

	t_CKUINT refcount;
	t_CKBOOL needs_close;

};

const static LONG axis_min = -32767;
const static LONG axis_max = 32767;


static vector< win32_joystick * > * joysticks;
static LPDIRECTINPUT lpdi = NULL;

static BOOL CALLBACK DIEnumJoystickProc( LPCDIDEVICEINSTANCE lpddi,
										 LPVOID pvRef )
{
	GUID guid = lpddi->guidProduct;
	win32_joystick * js = new win32_joystick;

	if( lpdi->CreateDevice( guid, &js->lpdiJoystick, NULL ) != DI_OK )
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

	if( lpdiJoystick->SetProperty(DIPROP_RANGE, &diprg.diph) != DI_OK )
	{
		
	}

	return DIENUM_CONTINUE;
}

void Joystick_init()
{
	if( lpdi != NULL )
		return;

	EM_log( CK_LOG_INFO, "initializing joystick" );

    HINSTANCE hInstance = GetModuleHandle( NULL );

	if( DirectInputCreate( hInstance, DIRECTINPUT_VERSION,
							&lpdi, NULL) != DI_OK )
		return;

	joysticks = new vector< win32_joystick * >;
	if( lpdi->EnumDevices( DIDEVTYPE_JOYSTICK, DIEnumJoystickProc, 
						   NULL, DIEDFL_ATTACHEDONLY ) != DI_OK )
	{
		delete joysticks;
		joysticks = NULL;
		lpdi->Release();
		lpdi = NULL;
		return;
	}
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
												 DIDFT_ABSAXIS ) != DI_OK )
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

	joystick->needs_close = TRUE;

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

void Mouse_init()
{
    
}

void Mouse_poll()
{
    
}

void Mouse_quit()
{
    
}

int Mousek_count()
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


#elif defined( __LINUX_ALSA__ ) || defined( __LINUX_OSS__ ) || defined( __LINUX_JACK__ )
#pragma mark Linux joystick support

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

#endif

