/*
 * Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
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
/*
 * HISTORY
 *
 */

#ifndef _IOKIT_IOPOWERSOURCES_H
#define _IOKIT_IOPOWERSOURCES_H

#include <sys/cdefs.h>
__BEGIN_DECLS

/*!
    @functiongroup Low Power Warnings
*/

/*! @constant kIOPSNotifyLowBattery
    @abstract Notify(3) key. The system delivers notifications on this key when the 
    battery time remaining drops into a warnable level.
 */
#define kIOPSNotifyLowBattery   "com.apple.system.powersources.lowbattery"

typedef enum {
/*! @constant kIOPSLowBatteryWarningNone
    @abstract The system is not in a low battery situation, or is on drawing from an external power source.
    @discussion The system displays no low power warnings; neither should application clients of this
    API.
 */
    kIOPSLowBatteryWarningNone  = 1,

/*! @constant kIOPSLowBatteryWarningEarly
    @abstract The system is in an early low battery situation.
    @discussion Per Apple's definition, the battery has dropped below 22% remaining power.
    OS X alerts the user by changing the color of BatteryMonitor to red. Warning the user is optional
    for full screen apps.
 */
    kIOPSLowBatteryWarningEarly = 2,

/*! @constant kIOPSLowBatteryWarningFinal
    @abstract The battery can provide no more than 10 minutes of runtime.
    @discussion OS X makes no guarantees that the system shall remain in Final Warning for 10 minutes.
    Batteries are frequently calibrated differently and may provide runtime for more or less than
    the estimated 10 minutes.
 */
    kIOPSLowBatteryWarningFinal = 3
} IOPSLowBatteryWarningLevel;

/*! @function IOPSGetBatteryWarningLevel
    @abstract Indicates whether the system is at a low battery warning level.
    @discussion If your app runs in full screen mode and occludes OS X's battery monitor's low
    battery warnings, you should alert the user at least when the system is in kIOPSLowBatteryWarnFinal.
 */
IOPSLowBatteryWarningLevel IOPSGetBatteryWarningLevel(void);

/*! 
    @functiongroup Power Source Descriptions
*/

/*! 
    @header IOPowerSources.h
    IOPowerSources provides uniform access to the state of power sources attached to the system.
    You can receive a change notification when any power source data changes.
    "Power sources" currently include batteries and UPS devices.<br>
    The header follows CF semantics in that it is the caller's responsibility to CFRelease() anything
    returned by a "Copy" function, and the caller should not CFRelease() anything returned by a "Get" function.
*/

typedef void  (*IOPowerSourceCallbackType)(void *context);

/*! @function IOPSCopyPowerSourcesInfo
    @abstract Returns a blob of Power Source information in an opaque CFTypeRef. 
    @discussion Clients should not directly access data in the returned CFTypeRef - 
        they should use the accessor functions IOPSCopyPowerSourcesList and 
        IOPSGetPowerSourceDescription, instead.
    @result NULL if errors were encountered, a CFTypeRef otherwise.
        Caller must CFRelease() the return value when done accessing it.
*/
CFTypeRef IOPSCopyPowerSourcesInfo(void);

/*! @function IOPSCopyPowerSourcesList
    @abstract Returns a CFArray of Power Source handles, each of type CFTypeRef.
    @discussion  The caller shouldn't directly access the CFTypeRefs, but should use
        IOPSGetPowerSourceDescription on each member of the CFArrayRef.
    @param  blob Takes the CFTypeRef returned by IOPSCopyPowerSourcesInfo()
    @result Returns NULL if errors were encountered, otherwise a CFArray of CFTypeRefs.
        Caller must CFRelease() the returned CFArrayRef.
*/
CFArrayRef IOPSCopyPowerSourcesList(CFTypeRef blob);

/*! @function IOPSGetPowerSourceDescription
    
    @abstract Returns a CFDictionary with readable information about the specific power source.
    @discussion See the C-strings defined in IOPSKeys.h for specific keys into the dictionary.
        Don't expect all keys to be present in any dictionary. Some power sources, for example,
        may not support the "Time Remaining To Empty" key and it will not be present in their dictionaries.
    @param blob The CFTypeRef returned by IOPSCopyPowerSourcesInfo()
    @param ps One of the CFTypeRefs in the CFArray returned by IOPSCopyPowerSourcesList()
    @result Returns NULL if an error was encountered, otherwise a CFDictionary. Caller should 
        NOT release the returned CFDictionary - it will be released as part of the CFTypeRef returned by
        IOPSCopyPowerSourcesInfo().
*/
CFDictionaryRef IOPSGetPowerSourceDescription(CFTypeRef blob, CFTypeRef ps);

/*! @function IOPSNotificationCreateRunLoopSource
    
    @abstract  Returns a CFRunLoopSourceRef that notifies the caller when power source
        information changes.
    @param callback A function to be called whenever any power source is added, removed, or changes.
    @param context Any user-defined pointer, passed to the IOPowerSource callback.
    @result Returns NULL if an error was encountered, otherwise a CFRunLoopSource. Caller must
        release the CFRunLoopSource.
*/
CFRunLoopSourceRef IOPSNotificationCreateRunLoopSource(IOPowerSourceCallbackType, void *);


/*! @function IOPSCopyExternalPowerAdapterDetails
 *  @abstract Returns a CFDictionary that describes the attached (AC) external
 *      power adapter (if any external power adapter is attached.
 *  @discussion Use the kIOPSPowerAdapter... keys described in IOPSKeys.h
 *      to interpret the returned CFDictionary.
 *  @result Returns a CFDictionary on success. Caller must release the returned
 *      dictionary. If no adapter is attached, or if there's an error,  returns NULL.
 */
CFDictionaryRef IOPSCopyExternalPowerAdapterDetails(void);

__END_DECLS

#endif /* _IOKIT_IOPOWERSOURCES_H */
