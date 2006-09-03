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

#pragma mark OS X Joystick support
#ifdef __MACOSX_CORE__

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
        min = 0;
        max = 0;
    }
    
    IOHIDElementCookie cookie;
    
    t_CKINT type;
    t_CKINT num;
    t_CKINT value;
    
    t_CKINT min;
    t_CKINT max;
};

struct OSX_Device
{
    OSX_Device()
    {
        interface = NULL;
        queue = NULL;
        strncpy( name, "Device", 256 );
        elements = NULL;
        buttons = -1;
        axes = -1;
        hats = -1;
        refcount = 0;
    }
        
    IOHIDDeviceInterface ** interface; // device interface
    IOHIDQueueInterface ** queue; // device event queue
    
    char name[256];
    
    t_CKINT usage_page;
    t_CKINT usage;
    
    /* Note: setting any of these to -1 informs the element enumerating 
        method that we are not interested in that particular element type */
    map< IOHIDElementCookie, OSX_Device_Element * > * elements;
    t_CKINT buttons;
    t_CKINT axes;
    t_CKINT hats;
    
    t_CKUINT refcount; // incremented on open, decremented on close
    
    const static t_CKINT event_queue_size = 50; 
    // queues use wired kernel memory so should be set to as small as possible
    // but should account for the maximum possible events in the queue
    // USB updates will likely occur at 100 Hz so one must account for this rate
    // if states change quickly (updates are only posted on state changes)
};

vector< OSX_Device * > * joysticks = NULL;

//------------------------------------------------------------------------------
// name: OSX_enumerate_device_elements
// desc: perform depth depth first search on potentially nested arrays of 
// elements on the device, 
//------------------------------------------------------------------------------
static void OSX_enumerate_device_elements( CFArrayRef elements, 
                                           OSX_Device * record )
{
    CFTypeRef refCF = 0;
    CFDictionaryRef element_dictionary;
    OSX_Device_Element * element;
    t_CKINT usage_page, usage, element_type;
    IOReturn result;
    
    CFIndex i, len = CFArrayGetCount( elements );
    for( i = 0; i < len; i++ )
    {
        // retrieve the dictionary of the ith element's data
        refCF = CFArrayGetValueAtIndex( elements, i );
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
                                if( record->axes != -1 )
                                {
                                    element = new OSX_Device_Element; 
                                    
                                    element->num = record->axes;
                                    element->type = CK_HID_JOYSTICK_AXIS;
                                    
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
                                    
                                    result = ( *( record->queue ) )->addElement( record->queue, element->cookie, 0 );
                                    if( result != kIOReturnSuccess )
                                    {
                                        delete element;
                                        continue;
                                    }
                                    
                                    EM_log( CK_LOG_FINE, "adding axis %d", record->axes );
                                    
                                    //record->axes->insert( pair< IOHIDElementCookie, OSX_Device_Element * >( element->cookie, element ) );
                                    ( *( record->elements ) )[element->cookie] = element;
                                    record->axes++;
                                }
                                
                                break;
                                
                            case kHIDUsage_GD_Hatswitch:
                                // this is a hat
                                if( record->hats != -1 )
                                {
                                    element = new OSX_Device_Element; 
                                    
                                    element->type = CK_HID_JOYSTICK_HAT;
                                    element->num = record->hats;
                                    
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
                                    
                                    result = ( *( record->queue ) )->addElement( record->queue, element->cookie, 0 );
                                    if( result != kIOReturnSuccess )
                                    {
                                        delete element;
                                        continue;
                                    }

                                    EM_log( CK_LOG_FINE, "adding hat %d", record->hats );
                                    
                                    //record->hats->insert( pair< IOHIDElementCookie, OSX_Device_Element * >( element->cookie, element ) );
                                    ( *( record->elements ) )[element->cookie] = element;
                                    record->hats++;
                                }
                                
                                break;
                        }
                        
                        break;
                        
                    case kHIDPage_Button:
                        // this is a button
                        if( record->buttons != -1 )
                        {
                            element = new OSX_Device_Element; 
                            
                            element->type = CK_HID_BUTTON_DOWN;
                            element->num = record->buttons;
                            
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
                            
                            result = ( *( record->queue ) )->addElement( record->queue, element->cookie, 0 );
                            if( result != kIOReturnSuccess )
                            {
                                delete element;
                                continue;
                            }
                            
                            EM_log( CK_LOG_FINE, "adding button %d", record->buttons );
                            
                            //record->buttons->insert( pair< IOHIDElementCookie, OSX_Device_Element * >( element->cookie, element ) );
                            ( *( record->elements ) )[element->cookie] = element;
                            record->buttons++;
                        }
                        
                        break;
                }
                
                break;
                
            case kIOHIDElementTypeCollection:
                refCF = CFDictionaryGetValue( element_dictionary, CFSTR( kIOHIDElementKey ) );
                if( !refCF )
                    continue;
                
                OSX_enumerate_device_elements( ( CFArrayRef ) refCF, record );
                break;
        }
    }
}

void Joystick_init()
{
    // verify that the joystick system has not already been initialized
    if( joysticks != NULL )
        return;
    
	IOReturn result = kIOReturnSuccess;
	io_iterator_t hidObjectIterator = 0;
    CFTypeRef refCF;
    t_CKINT filter_usage_page = kHIDPage_GenericDesktop;
    
    // allocate vector of joystick device records
    joysticks = new vector< OSX_Device * >;
    
	CFMutableDictionaryRef hidMatchDictionary = IOServiceMatching( kIOHIDDeviceKey );
    if( !hidMatchDictionary )
    {
        EM_log( CK_LOG_SEVERE, "joystick: error: retrieving hidMatchDictionary, unable to initialize" );
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
        EM_log( CK_LOG_SEVERE, "joystick: error: retrieving matching services, unable to initialize" );
        return;
    }

    CFRelease( refCF );
    
    io_object_t ioHIDDeviceObject = 0;
    CFMutableDictionaryRef hidProperties = 0;
    kern_return_t kern_result;
    t_CKINT usage, usage_page;
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
                EM_log( CK_LOG_INFO, "joystick: configuring joystick %i", 
                        joysticks->size() );
                EM_pushlog();
                
                // retrieve a dictionary of device properties
                kern_result = IORegistryEntryCreateCFProperties( ioHIDDeviceObject, 
                                                                 &hidProperties, 
                                                                 kCFAllocatorDefault, 
                                                                 kNilOptions);
                if( kern_result != KERN_SUCCESS || hidProperties == 0 )
                {
                    EM_poplog();
                    continue;
                }
                
                // allocate the device record, set usage page and usage
                OSX_Device * new_device = new OSX_Device;
                new_device->usage_page = usage_page;
                new_device->usage = usage;
                
                // get the joystick name, and copy it into the device record
                refCF = CFDictionaryGetValue( hidProperties, 
                                              CFSTR( kIOHIDProductKey ) );
                if( refCF )
                    CFStringGetCString( ( CFStringRef )refCF, new_device->name, 256, 
                                        kCFStringEncodingASCII );
                else
                    strncpy( new_device->name, "Joystick", 256 );
                
                /*** TODO: move this part into Joystick_open, wait until it is 
                    absolutely necessary to parse the device properties  ***/
                // create the IOKit device interface
                SInt32 score = 0;
                IOCFPlugInInterface ** plugInInterface = NULL;
                result = IOCreatePlugInInterfaceForService( ioHIDDeviceObject,
                                                            kIOHIDDeviceUserClientTypeID,
                                                            kIOCFPlugInInterfaceID,
                                                            &plugInInterface,
                                                            &score);
                if( result != kIOReturnSuccess )
                {
                    CFRelease( hidProperties );
                    ( *plugInInterface )->Release( plugInInterface );
                    delete new_device;
                    EM_poplog();
                    continue;
                }
                
                HRESULT plugInResult = S_OK;
                plugInResult = ( *plugInInterface )->QueryInterface( plugInInterface,
                                                                     CFUUIDGetUUIDBytes( kIOHIDDeviceInterfaceID ),
                                                                     ( LPVOID * ) &( new_device->interface ) );
                ( *plugInInterface )->Release( plugInInterface );
                if( plugInResult != S_OK )
                {
                    CFRelease( hidProperties );
                    delete new_device;
                    EM_poplog();
                    continue;
                }
                
                result = ( *( new_device->interface ) )->open( new_device->interface, 0 );
                if( result != kIOReturnSuccess )
                {
                    CFRelease( hidProperties );
                    ( *( new_device->interface ) )->Release( new_device->interface );
                    delete new_device;
                    EM_poplog();
                    continue;
                }
                
                // create an event queue, so we dont have to poll individual elements
                
                new_device->queue = ( *( new_device->interface ) )->allocQueue( new_device->interface );
                if( !new_device->queue )
                {
                    CFRelease( hidProperties );
                    ( *( new_device->interface ) )->close( new_device->interface );
                    ( *( new_device->interface ) )->Release( new_device->interface );
                    delete new_device;
                    EM_poplog();
                    continue;
                }
                
                result = ( *( new_device->queue ) )->create( new_device->queue, 0, 
                                                             OSX_Device::event_queue_size );
                if( result != kIOReturnSuccess )
                {
                    CFRelease( hidProperties );
                    ( *( new_device->queue ) )->Release( new_device->queue );
                    ( *( new_device->interface ) )->close( new_device->interface );
                    ( *( new_device->interface ) )->Release( new_device->interface );
                    delete new_device;
                    EM_poplog();
                }
                
                // retrieve the array of elements...
                refCF = CFDictionaryGetValue( hidProperties, 
                                              CFSTR( kIOHIDElementKey ) );
                if( !refCF )
                {
                    CFRelease( hidProperties );
                    ( *( new_device->queue ) )->dispose( new_device->queue );
                    ( *( new_device->queue ) )->Release( new_device->queue );
                    ( *( new_device->interface ) )->close( new_device->interface );
                    ( *( new_device->interface ) )->Release( new_device->interface );
                    delete new_device;
                    EM_poplog();
                    continue;
                }
                
                // ...allocate space for element records...
                new_device->elements = new map< IOHIDElementCookie, OSX_Device_Element * >;
                new_device->axes = 0;
                new_device->buttons = 0;
                new_device->hats = 0;

                // ...and parse the element array recursively
                OSX_enumerate_device_elements( ( CFArrayRef ) refCF, new_device );
                
                EM_poplog();
                
                joysticks->push_back( new_device );
                CFRelease( hidProperties );
            }
        }
    }
    
    IOObjectRelease( hidObjectIterator );
}

void Joystick_poll()
{
    if( joysticks == NULL )
        return;
    
    // for each open device, poll the event queue
    OSX_Device * joystick;
    OSX_Device_Element * element;
    //map< IOHIDElementCookie, OSX_Device_Element * >::iterator iter_element;
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
    if( joysticks == NULL )
        return;
    
    vector< OSX_Device * >::size_type i, len = joysticks->size();
    OSX_Device * joystick;
    for( i = 0; i < len; i++ )
    {
        map< IOHIDElementCookie, OSX_Device_Element * >::iterator iter, end;
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

int Joystick_count()
{
    if( joysticks == NULL )
        return 0;
    
    return joysticks->size();
}

int Joystick_open( int js )
{
    if( joysticks == NULL )
        return -1;

    if( js < 0 || js >= joysticks->size() )
        return -1;
    
    OSX_Device * joystick = joysticks->at( js );

    if( joystick->refcount == 0 )
    {
        IOReturn result = ( *( joystick->queue ) )->start( joystick->queue );
        if( result != kIOReturnSuccess )
        {
            EM_log( CK_LOG_INFO, "joystick: error: starting event queue" );
            return -1;
        }
    }
    
    joystick->refcount++;
    
    return 0;
}

int Joystick_close( int js )
{
    if( joysticks == NULL )
        return -1;

    if( js < 0 || js >= joysticks->size() )
        return -1;
    
    OSX_Device * joystick = joysticks->at( js );
    
    if( joystick > 0 )
    {
        joystick->refcount--;
        
        if( joystick->refcount == 0 )
        {
            IOReturn result = ( *( joystick->queue ) )->stop( joystick->queue );
            if( result != kIOReturnSuccess )
            {
                EM_log( CK_LOG_INFO, "joystick: error: stopping event queue" );
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

#else

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

