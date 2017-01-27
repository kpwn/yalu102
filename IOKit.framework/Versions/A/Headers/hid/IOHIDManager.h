/*
 * Copyright (c) 1999-2008 Apple Computer, Inc.  All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_HID_IOHIDMANAGER_H_
#define _IOKIT_HID_IOHIDMANAGER_H_

#include <IOKit/IOTypes.h>
#include <IOKit/IOReturn.h>
#include <IOKit/hid/IOHIDLib.h>
#include <CoreFoundation/CoreFoundation.h>

/*!
	@header IOHIDManager
    IOHIDManager defines an Human Interface Device (HID) managment object.  
    It provides global interaction with managed HID devices such as 
    discovery/removal and receiving input events.  IOHIDManager is also a CFType 
    object and as such conforms to all the conventions expected such object.
    <p>
    This documentation assumes that you have a basic understanding of the 
    material contained in <a href="http://developer.apple.com/documentation/DeviceDrivers/Conceptual/AccessingHardware/index.html"><i>Accessing Hardware From Applications</i></a>
    For definitions of I/O Kit terms used in this documentation, such as 
    matching dictionary, family, and driver, see the overview of I/O Kit terms 
    and concepts n the "Device Access and the I/O Kit" chapter of 
    <i>Accessing Hardware From Applications</i>.
    
    This documentation also assumes you have read <a href="http://developer.apple.com/documentation/DeviceDrivers/HumanInterfaceDeviceForceFeedback-date.html"><i>Human Interface Device & Force Feedback</i></a>.
    Please review documentation before using this reference.
    <p>
    All of the information described in this document is contained in the header 
    file <font face="Courier New,Courier,Monaco">IOHIDManager.h</font> found at 
    <font face="Courier New,Courier,Monaco">/System/Library/Frameworks/IOKit.framework/Headers/hid/IOHIDManager.h</font>.
*/

__BEGIN_DECLS

/*! @typedef IOHIDManagerRef
	@abstract This is the type of a reference to the IOHIDManager.
*/
typedef struct __IOHIDManager * IOHIDManagerRef;

/*!
	@function   IOHIDManagerGetTypeID
	@abstract   Returns the type identifier of all IOHIDManager instances.
*/
CF_EXPORT
CFTypeID IOHIDManagerGetTypeID(void) 
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*!
	@function   IOHIDManagerCreate
	@abstract   Creates an IOHIDManager object.
    @discussion The IOHIDManager object is meant as a global management system
                for communicating with HID devices.
    @param      allocator Allocator to be used during creation.
    @param      options Reserved for future use.
    @result     Returns a new IOHIDManagerRef.
*/
CF_EXPORT 
IOHIDManagerRef IOHIDManagerCreate(     
                                CFAllocatorRef                  allocator,
                                IOOptionBits                    options)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
                                
/*!
	@function   IOHIDManagerOpen
	@abstract   Opens the IOHIDManager.
    @discussion This will open both current and future devices that are 
                enumerated. To establish an exclusive link use the 
                kIOHIDOptionsTypeSeizeDevice option. 
    @param      manager Reference to an IOHIDManager.
    @param      options Option bits to be sent down to the manager and device.
    @result     Returns kIOReturnSuccess if successful.
*/
CF_EXPORT
IOReturn IOHIDManagerOpen(
                                IOHIDManagerRef                 manager,
                                IOOptionBits                    options)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
                                
/*!
	@function   IOHIDManagerClose
	@abstract   Closes the IOHIDManager.
    @discussion This will also close all devices that are currently enumerated.
    @param      manager Reference to an IOHIDManager.
    @param      options Option bits to be sent down to the manager and device.
    @result     Returns kIOReturnSuccess if successful.
*/
CF_EXPORT
IOReturn IOHIDManagerClose(
                                IOHIDManagerRef                 manager,
                                IOOptionBits                    options)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*!
	@function   IOHIDManagerGetProperty
	@abstract   Obtains a property of an IOHIDManager.
    @discussion Property keys are prefixed by kIOHIDDevice and declared in 
                <IOKit/hid/IOHIDKeys.h>.
    @param      manager Reference to an IOHIDManager.
    @param      key CFStringRef containing key to be used when querying the 
                manager.
    @result     Returns CFTypeRef containing the property.
*/
CF_EXPORT
CFTypeRef IOHIDManagerGetProperty(
                                IOHIDManagerRef                 manager,
                                CFStringRef                     key)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
                                
/*!
	@function   IOHIDManagerSetProperty
	@abstract   Sets a property for an IOHIDManager.
    @discussion Property keys are prefixed by kIOHIDDevice and kIOHIDManager and
                declared in <IOKit/hid/IOHIDKeys.h>. This method will propagate 
                any relevent properties to current and future devices that are 
                enumerated.
    @param      manager Reference to an IOHIDManager.
    @param      key CFStringRef containing key to be used when modifiying the 
                device property.
    @param      property CFTypeRef containg the property to be set.
    @result     Returns TRUE if successful.
*/
CF_EXPORT
Boolean IOHIDManagerSetProperty(
                                IOHIDManagerRef                 manager,
                                CFStringRef                     key,
                                CFTypeRef                       value)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
                                        
/*! @function   IOHIDDeviceScheduleWithRunLoop
    @abstract   Schedules HID manager with run loop.
    @discussion Formally associates manager with client's run loop. Scheduling
                this device with the run loop is necessary before making use of
                any asynchronous APIs.  This will propagate to current and 
                future devices that are enumerated.
    @param      manager Reference to an IOHIDManager.
    @param      runLoop RunLoop to be used when scheduling any asynchronous 
                activity.
    @param      runLoopMode Run loop mode to be used when scheduling any 
                asynchronous activity.
*/
CF_EXPORT
void IOHIDManagerScheduleWithRunLoop(
                                IOHIDManagerRef                 manager,
                                CFRunLoopRef                    runLoop, 
                                CFStringRef                     runLoopMode)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*! @function   IOHIDDeviceUnscheduleFromRunLoop
    @abstract   Unschedules HID manager with run loop.
    @discussion Formally disassociates device with client's run loop. This will 
                propagate to current devices that are enumerated.
    @param      manager Reference to an IOHIDManager.
    @param      runLoop RunLoop to be used when unscheduling any asynchronous 
                activity.
    @param      runLoopMode Run loop mode to be used when unscheduling any 
                asynchronous activity.
*/
CF_EXPORT
void IOHIDManagerUnscheduleFromRunLoop(
                                IOHIDManagerRef                 manager,
                                CFRunLoopRef                    runLoop, 
                                CFStringRef                     runLoopMode)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
                                
/*! @function   IOHIDManagerSetDeviceMatching
    @abstract   Sets matching criteria for device enumeration.
    @discussion Matching keys are prefixed by kIOHIDDevice and declared in 
                <IOKit/hid/IOHIDKeys.h>.  Passing a NULL dictionary will result
                in all devices being enumerated. Any subsequent calls will cause
                the hid manager to release previously enumerated devices and 
                restart the enuerate process using the revised criteria.  If 
                interested in multiple, specific device classes, please defer to
                using IOHIDManagerSetDeviceMatchingMultiple.
    @param      manager Reference to an IOHIDManager.
    @param      matching CFDictionaryRef containg device matching criteria.
*/
CF_EXPORT
void IOHIDManagerSetDeviceMatching(
                                IOHIDManagerRef                 manager,
                                CFDictionaryRef                 matching)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
                                
/*! @function   IOHIDManagerSetDeviceMatchingMultiple
    @abstract   Sets multiple matching criteria for device enumeration.
    @discussion Matching keys are prefixed by kIOHIDDevice and declared in 
                <IOKit/hid/IOHIDKeys.h>.  This method is useful if interested 
                in multiple, specific device classes.
    @param      manager Reference to an IOHIDManager.
    @param      multiple CFArrayRef containing multiple CFDictionaryRef objects
                containg device matching criteria.
*/
CF_EXPORT
void IOHIDManagerSetDeviceMatchingMultiple(
                                IOHIDManagerRef                 manager,
                                CFArrayRef                      multiple)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
                                
/*! @function   IOHIDManagerCopyDevices
    @abstract   Obtains currently enumerated devices.
    @param      manager Reference to an IOHIDManager.
    @result     CFSetRef containing IOHIDDeviceRefs.
*/
CF_EXPORT
CFSetRef IOHIDManagerCopyDevices(
                                IOHIDManagerRef                 manager)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*! @function   IOHIDManagerRegisterDeviceMatchingCallback
    @abstract   Registers a callback to be used a device is enumerated.
    @discussion Only device matching the set criteria will be enumerated.
    @param      manager Reference to an IOHIDManagerRef.
    @param      callback Pointer to a callback method of type 
                IOHIDDeviceCallback.
    @param      context Pointer to data to be passed to the callback.
*/
CF_EXPORT
void IOHIDManagerRegisterDeviceMatchingCallback(
                                IOHIDManagerRef                 manager,
                                IOHIDDeviceCallback             callback,
                                void *                          context)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*! @function   IOHIDManagerRegisterDeviceRemovalCallback
    @abstract   Registers a callback to be used when any enumerated device is 
                removed.
    @discussion In most cases this occurs when a device is unplugged.
    @param      manager Reference to an IOHIDManagerRef.
    @param      callback Pointer to a callback method of type 
                IOHIDDeviceCallback.
    @param      context Pointer to data to be passed to the callback.
*/
CF_EXPORT
void IOHIDManagerRegisterDeviceRemovalCallback(
                                IOHIDManagerRef                 manager,
                                IOHIDDeviceCallback             callback,
                                void *                          context)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*! @function   IOHIDManagerRegisterInputReportCallback
    @abstract   Registers a callback to be used when an input report is issued by 
                any enumerated device.
    @discussion An input report is an interrupt driver report issued by a device.
    @param      manager Reference to an IOHIDManagerRef.
    @param      callback Pointer to a callback method of type IOHIDReportCallback.
    @param      context Pointer to data to be passed to the callback.
*/
CF_EXPORT
void IOHIDManagerRegisterInputReportCallback( 
                                    IOHIDManagerRef             manager,
                                    IOHIDReportCallback         callback,
                                    void *                      context)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
                                                                    
/*! @function   IOHIDManagerRegisterInputValueCallback
    @abstract   Registers a callback to be used when an input value is issued by 
                any enumerated device.
    @discussion An input element refers to any element of type 
                kIOHIDElementTypeInput and is usually issued by interrupt driven
                reports.
    @param      manager Reference to an IOHIDManagerRef.
    @param      callback Pointer to a callback method of type IOHIDValueCallback.
    @param      context Pointer to data to be passed to the callback.
*/
CF_EXPORT
void IOHIDManagerRegisterInputValueCallback( 
                                IOHIDManagerRef                 manager,
                                IOHIDValueCallback              callback,
                                void *                          context)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*! @function   IOHIDManagerSetInputValueMatching
    @abstract   Sets matching criteria for input values received via 
                IOHIDManagerRegisterInputValueCallback.
    @discussion Matching keys are prefixed by kIOHIDElement and declared in 
                <IOKit/hid/IOHIDKeys.h>.  Passing a NULL dictionary will result
                in all devices being enumerated. Any subsequent calls will cause
                the hid manager to release previously matched input elements and 
                restart the matching process using the revised criteria.  If 
                interested in multiple, specific device elements, please defer to
                using IOHIDManagerSetInputValueMatchingMultiple.
    @param      manager Reference to an IOHIDManager.
    @param      matching CFDictionaryRef containg device matching criteria.
*/
CF_EXPORT
void IOHIDManagerSetInputValueMatching(
                                IOHIDManagerRef                 manager,
                                CFDictionaryRef                 matching)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
                                
/*! @function   IOHIDManagerSetInputValueMatchingMultiple
    @abstract   Sets multiple matching criteria for input values received via 
                IOHIDManagerRegisterInputValueCallback.
    @discussion Matching keys are prefixed by kIOHIDElement and declared in 
                <IOKit/hid/IOHIDKeys.h>.  This method is useful if interested 
                in multiple, specific elements .
    @param      manager Reference to an IOHIDManager.
    @param      multiple CFArrayRef containing multiple CFDictionaryRef objects
                containg input element matching criteria.
*/
CF_EXPORT
void IOHIDManagerSetInputValueMatchingMultiple(
                                IOHIDManagerRef                 manager,
                                CFArrayRef                      multiple)
AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

                                    
__END_DECLS

#endif /* _IOKIT_HID_IOHIDMANAGER_H_ */