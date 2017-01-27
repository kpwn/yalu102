/*
 * Copyright (c) 1998-2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 * 
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
#ifndef _IOKIT_IOPM_H
#define _IOKIT_IOPM_H

#include <IOKit/IOTypes.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOReturn.h>

#ifdef __ppc__
#include <IOKit/pwr_mgt/IOPMDeprecated.h>
#endif

/*! @header IOPM.h
    @abstract Defines power management constants and keys used by both in-kernel and user space power management.
    @discussion IOPM.h defines a range of power management constants used in several in-kernel and user space APIs. Most significantly, the IOPMPowerFlags used to specify the fields of an IOPMPowerState struct are defined here.

    Most of the constants defined in IOPM.h are deprecated or for Apple internal use only, and are not elaborated on in headerdoc.
*/

enum {
    kIOPMMaxPowerStates = 10,
    IOPMMaxPowerStates = kIOPMMaxPowerStates
};

/*! @enum IOPMPowerFlags
    @abstract Bits are used in defining capabilityFlags, inputPowerRequirements, and outputPowerCharacter in the IOPMPowerState structure.
    @discussion These bits may be bitwise-OR'd together in the IOPMPowerState capabilityFlags field, the outputPowerCharacter field, and/or the inputPowerRequirement field. 
    
    The comments clearly mark whether each flag should be used in the capabilityFlags field, outputPowerCharacter field, and inputPowerRequirement field, or all three.
    
    The value of capabilityFlags, inputPowerRequirement or outputPowerCharacter may be 0. Most drivers implement their 'OFF' state, used when asleep, by defininf each of the 3 fields as 0.
    
    The bits listed below are only the most common bits used to define a device's power states. Your device's IO family may require that your device specify other input or output power flags to interact properly. Consult family-specific documentation to determine if your IOPower plane parents or children require other power flags; they probably don't.

    @constant kIOPMPowerOn Indicates the device is on, requires power, and provides power. Useful as a: Capability, InputPowerRequirement, OutputPowerCharacter

    @constant kIOPMDeviceUsable Indicates the device is usable in this state. Useful only as a Capability

    @constant kIOPMLowPower 
    Indicates device is in a low power state. May be bitwis-OR'd together
    with kIOPMDeviceUsable flag, to indicate the device is still usable.
    
    A device with a capability of kIOPMLowPower may:
       Require either 0 or kIOPMPowerOn from its power parent
       Offer either kIOPMLowPower, kIOPMPowerOn, or 0 (no power at all)
         to its power plane children.

    Useful only as a Capability, although USB drivers should consult USB family documentation for other valid circumstances to use the kIOPMLowPower bit.

    @constant kIOPMPreventIdleSleep
    In the capability field of a power state, disallows idle system sleep while the device is in that state.
    
    For example, displays and disks set this capability for their ON power state; since the system may not idle sleep while the display (and thus keyboard or mouse) or the disk is active.

    Useful only as a Capability.
    
    @constant kIOPMSleepCapability 
    Used only by certain IOKit Families (USB). Not defined or used by generic Power Management. Read your family documentation to see if you should define a powerstate using these capabilities.
    
    @constant kIOPMRestartCapability
    Used only by certain IOKit Families (USB). Not defined or used by generic Power Management. Read your family documentation to see if you should define a powerstate using these capabilities.
    
    @constant kIOPMSleep
    Used only by certain IOKit Families (USB). Not defined or used by generic Power Management. Read your family documentation to see if you should define a powerstate using these capabilities.

    @constant kIOPMRestart
    Used only by certain IOKit Families (USB). Not defined or used by generic Power Management. Read your family documentation to see if you should define a powerstate using these capabilities.
*/
typedef unsigned long IOPMPowerFlags;
enum {
    kIOPMPowerOn                    = 0x00000002,
    kIOPMDeviceUsable               = 0x00008000,
    kIOPMLowPower                   = 0x00010000,
    kIOPMPreventIdleSleep           = 0x00000040,
    kIOPMSleepCapability            = 0x00000004,
    kIOPMRestartCapability          = 0x00000080,
    kIOPMSleep                      = 0x00000001,
    kIOPMRestart                    = 0x00000080
};

/*
 * Private IOPMPowerFlags
 *
 * For Apple use only
 * Not for use with non-Apple drivers
 * Their behavior is undefined
 */
enum {
    kIOPMClockNormal                = 0x0004,
    kIOPMClockRunning               = 0x0008,
    kIOPMPreventSystemSleep         = 0x0010,
    kIOPMDoze                       = 0x0400,
    kIOPMChildClamp                 = 0x0080,
    kIOPMChildClamp2                = 0x0200,
    kIOPMNotPowerManaged            = 0x0800
};


/*
 * Deprecated IOPMPowerFlags
 * Their behavior is undefined when used in IOPMPowerState
 * Capability, InputPowerRequirement, or OutputPowerCharacter fields.
 */
enum {
    kIOPMMaxPerformance             = 0x4000,
    kIOPMPassThrough                = 0x0100,
    kIOPMAuxPowerOn                 = 0x0020,
    kIOPMNotAttainable              = 0x0001,
    kIOPMContextRetained            = 0x2000,
    kIOPMConfigRetained             = 0x1000,
    kIOPMStaticPowerValid           = 0x0800,
    kIOPMSoftSleep                  = 0x0400,
    kIOPMCapabilitiesMask =     kIOPMPowerOn | kIOPMDeviceUsable | 
                                kIOPMMaxPerformance | kIOPMContextRetained | 
                                kIOPMConfigRetained | kIOPMSleepCapability |
                                kIOPMRestartCapability
};

/*
 * Support for old names of IOPMPowerFlag constants
 */
enum {
    IOPMNotAttainable           = kIOPMNotAttainable,
    IOPMPowerOn                 = kIOPMPowerOn,
    IOPMClockNormal             = kIOPMClockNormal,
    IOPMClockRunning            = kIOPMClockRunning,
    IOPMAuxPowerOn              = kIOPMAuxPowerOn,
    IOPMDeviceUsable            = kIOPMDeviceUsable,
    IOPMMaxPerformance          = kIOPMMaxPerformance,
    IOPMContextRetained         = kIOPMContextRetained,
    IOPMConfigRetained          = kIOPMConfigRetained,
    IOPMNotPowerManaged         = kIOPMNotPowerManaged,
    IOPMSoftSleep               = kIOPMSoftSleep
};


enum {
    kIOPMNextHigherState        = 1,
    kIOPMHighestState           = 2,
    kIOPMNextLowerState         = 3,
    kIOPMLowestState            = 4
};

enum {
    IOPMNextHigherState         = kIOPMNextHigherState,
    IOPMHighestState            = kIOPMHighestState,
    IOPMNextLowerState          = kIOPMNextLowerState,
    IOPMLowestState             = kIOPMLowestState
};

// Internal commands used by power managment command queue
enum {
    kIOPMBroadcastAggressiveness = 1,
    kIOPMUnidleDevice
};

// Power consumption unknown value
enum {
    kIOPMUnknown = 0xFFFF
};

/*******************************************************************************
 *
 * Root Domain property keys of interest
 *
 ******************************************************************************/

/* AppleClamshellState 
 * reflects the state of the clamshell (lid) on a portable.
 * It has a boolean value.
 *  true        == clamshell is closed
 *  false       == clamshell is open
 *  not present == no clamshell on this hardware
 */
#define kAppleClamshellStateKey             "AppleClamshellState"

/* AppleClamshellCausesSleep 
 * reflects the clamshell close behavior on a portable. 
 * It has a boolean value.
 *  true        == system will sleep when clamshell is closed
 *  false       == system will not sleep on clamshell close 
 *                  (typically external display mode)
 *  not present == no clamshell on this hardware
 */
#define kAppleClamshellCausesSleepKey       "AppleClamshellCausesSleep"

/* kIOPMSleepWakeUUIDKey
 * Key refers to a CFStringRef that will uniquely identify
 * a sleep/wake cycle for logging & tracking.
 * The key becomes valid at the beginning of a sleep cycle - before we
 * initiate any sleep/wake notifications.
 * The key becomes invalid at the completion of a system wakeup. The
 * property will not be present in the IOPMrootDomain's registry entry
 * when it is invalid.
 * 
 * See IOPMrootDomain notification kIOPMMessageSleepWakeUUIDChange
 */
 #define kIOPMSleepWakeUUIDKey              "SleepWakeUUID"

/*******************************************************************************
 *
 * Root Domain general interest messages
 *
 * Available by registering for interest type 'gIOGeneralInterest' 
 * on IOPMrootDomain. 
 *
 ******************************************************************************/

/* kIOPMMessageClamshellStateChange
 * Delivered as a general interest notification on the IOPMrootDomain
 * IOPMrootDomain sends this message when state of either AppleClamshellState
 * or AppleClamshellCausesSleep changes. If this clamshell change results in
 * a sleep, the sleep will initiate soon AFTER delivery of this message.
 * The state of both variables is encoded in a bitfield argument sent with
 * the message. Check bits 0 and 1 using kClamshellStateBit & kClamshellSleepBit
 */
enum {
    kClamshellStateBit = (1 << 0),
    kClamshellSleepBit = (1 << 1)
};

#define kIOPMMessageClamshellStateChange   \
                iokit_family_msg(sub_iokit_powermanagement, 0x100)

/* kIOPMMessageFeatureChange
 * Delivered when the set of supported features ("Supported Features" dictionary
 * under IOPMrootDomain registry) changes in some way. Typically addition or
 * removal of a supported feature.
 * RootDomain passes no argument with this message.
 */
#define kIOPMMessageFeatureChange           \
                iokit_family_msg(sub_iokit_powermanagement, 0x110)

/* kIOPMMessageInflowDisableCancelled
 * The battery has drained completely to its "Fully Discharged" state. 
 * If a user process has disabled battery inflow for battery 
 * calibration, we forcibly re-enable Inflow at this point.
 * If inflow HAS been forcibly re-enabled, bit 0
 * (kInflowForciblyEnabledBit) will be set.
 */
enum {
    kInflowForciblyEnabledBit = (1 << 0)
};

/* kIOPMMessageInternalBatteryFullyDischarged
 * The battery has drained completely to its "Fully Discharged" state. 
 */
#define kIOPMMessageInternalBatteryFullyDischarged  \
                iokit_family_msg(sub_iokit_powermanagement, 0x120)

/* kIOPMMessageSystemPowerEventOccurred
 * Some major system thermal property has changed, and interested clients may
 * modify their behavior.
 */
#define kIOPMMessageSystemPowerEventOccurred  \
                iokit_family_msg(sub_iokit_powermanagement, 0x130)

/* kIOPMMessageSleepWakeUUIDChange
 * Either a new SleepWakeUUID has been specified at the beginning of a sleep,
 * or we're removing the existing property upon completion of a wakeup.
 */
#define kIOPMMessageSleepWakeUUIDChange  \
                iokit_family_msg(sub_iokit_powermanagement, 0x140)
                
/* kIOPMMessageSleepWakeUUIDSet
 * Argument accompanying the kIOPMMessageSleepWakeUUIDChange notification when 
 * a new UUID has been specified.
 */
#define kIOPMMessageSleepWakeUUIDSet                    ((void *)1)

/* kIOPMMessageSleepWakeUUIDCleared
 * Argument accompanying the kIOPMMessageSleepWakeUUIDChange notification when 
 * the current UUID has been removed.
 */
#define kIOPMMessageSleepWakeUUIDCleared                ((void *)0)

/*******************************************************************************
 *
 * Power commands issued to root domain
 * Use with IOPMrootDomain::receivePowerNotification()
 *
 * These commands are issued from system drivers only:
 *      ApplePMU, AppleSMU, IOGraphics, AppleACPIFamily
 *
 ******************************************************************************/
enum {
  kIOPMSleepNow                 = (1<<0),  // put machine to sleep now
  kIOPMAllowSleep               = (1<<1),  // allow idle sleep
  kIOPMPreventSleep             = (1<<2),  // do not allow idle sleep
  kIOPMPowerButton              = (1<<3),  // power button was pressed
  kIOPMClamshellClosed          = (1<<4),  // clamshell was closed
  kIOPMPowerEmergency           = (1<<5),  // battery dangerously low
  kIOPMDisableClamshell         = (1<<6),  // do not sleep on clamshell closure
  kIOPMEnableClamshell          = (1<<7),  // sleep on clamshell closure
  kIOPMProcessorSpeedChange     = (1<<8),  // change the processor speed
  kIOPMOverTemp                 = (1<<9),  // system dangerously hot
  kIOPMClamshellOpened          = (1<<10)  // clamshell was opened
};


/*******************************************************************************
 *
 * Power Management Return Codes
 *
 ******************************************************************************/
enum {
    kIOPMNoErr                  = 0,

    // Returned by driver's setPowerState(), powerStateWillChangeTo(),
    // powerStateDidChangeTo(), or acknowledgeSetPowerState() to
    // implicitly acknowledge power change upon function return.
    kIOPMAckImplied             = 0,

    // Deprecated
    kIOPMWillAckLater           = 1,

    // Returned by requestPowerDomainState() to indicate
    // unrecognized specification parameter.
    kIOPMBadSpecification       = 4,

    // Returned by requestPowerDomainState() to indicate
    // no power state matches search specification.
    kIOPMNoSuchState            = 5,

    // Deprecated
    kIOPMCannotRaisePower       = 6,

    // Deprecated
    kIOPMParameterError         = 7,

    // Returned when power management state is accessed
    // before driver has called PMinit().
    kIOPMNotYetInitialized      = 8,

    // And the old constants; deprecated
    IOPMNoErr = kIOPMNoErr,
    IOPMAckImplied = kIOPMAckImplied, 
    IOPMWillAckLater = kIOPMWillAckLater,
    IOPMBadSpecification = kIOPMBadSpecification,
    IOPMNoSuchState = kIOPMNoSuchState,
    IOPMCannotRaisePower = kIOPMCannotRaisePower,
    IOPMParameterError = kIOPMParameterError,
    IOPMNotYetInitialized = kIOPMNotYetInitialized
};


// IOPMPowerSource class descriptive strings
// Power Source state is published as properties to the IORegistry under these 
// keys.
#define kIOPMPSExternalConnectedKey                 "ExternalConnected"
#define kIOPMPSExternalChargeCapableKey             "ExternalChargeCapable"
#define kIOPMPSBatteryInstalledKey                  "BatteryInstalled"
#define kIOPMPSIsChargingKey                        "IsCharging"
#define kIOPMFullyChargedKey                        "FullyCharged"
#define kIOPMPSAtWarnLevelKey                       "AtWarnLevel"
#define kIOPMPSAtCriticalLevelKey                   "AtCriticalLevel"
#define kIOPMPSCurrentCapacityKey                   "CurrentCapacity"
#define kIOPMPSMaxCapacityKey                       "MaxCapacity"
#define kIOPMPSDesignCapacityKey                    "DesignCapacity"
#define kIOPMPSTimeRemainingKey                     "TimeRemaining"
#define kIOPMPSAmperageKey                          "Amperage"
#define kIOPMPSVoltageKey                           "Voltage"
#define kIOPMPSCycleCountKey                        "CycleCount"
#define kIOPMPSMaxErrKey                            "MaxErr"
#define kIOPMPSAdapterInfoKey                       "AdapterInfo"
#define kIOPMPSLocationKey                          "Location"
#define kIOPMPSErrorConditionKey                    "ErrorCondition"
#define kIOPMPSManufacturerKey                      "Manufacturer"
#define kIOPMPSManufactureDateKey                   "ManufactureDate"
#define kIOPMPSModelKey                             "Model"
#define kIOPMPSSerialKey                            "Serial"
#define kIOPMDeviceNameKey                          "DeviceName"
#define kIOPMPSLegacyBatteryInfoKey                 "LegacyBatteryInfo"
#define kIOPMPSBatteryHealthKey                     "BatteryHealth"
#define kIOPMPSHealthConfidenceKey                  "HealthConfidence"
#define kIOPMPSCapacityEstimatedKey	                "CapacityEstimated"
#define kIOPMPSBatteryChargeStatusKey               "ChargeStatus"
#define kIOPMPSBatteryTemperatureKey                "Temperature"

// kIOPMPSBatteryChargeStatusKey may have one of the following values, or may have
// no value. If kIOPMBatteryChargeStatusKey has a NULL value (or no value) associated with it
// then charge is proceeding normally. If one of these battery charge status reasons is listed,
// then the charge may have been interrupted.
#define kIOPMBatteryChargeStatusTooHot              "HighTemperature"
#define kIOPMBatteryChargeStatusTooCold             "LowTemperature"
#define kIOPMBatteryChargeStatusGradient            "BatteryTemperatureGradient"

// Definitions for battery location, in case of multiple batteries.
// A location of 0 is unspecified
// Location is undefined for single battery systems
enum {
    kIOPMPSLocationLeft = 1001,
    kIOPMPSLocationRight = 1002
};

// Battery quality health types, specified by BatteryHealth and HealthConfidence
// properties in an IOPMPowerSource battery kext.
enum {
    kIOPMUndefinedValue = 0,
    kIOPMPoorValue      = 1,
    kIOPMFairValue      = 2,
    kIOPMGoodValue      = 3
};

// Battery's time remaining estimate is invalid this long (seconds) after a wake
#define kIOPMPSInvalidWakeSecondsKey           "BatteryInvalidWakeSeconds"

// Battery must wait this long (seconds) after being completely charged before
// the battery is settled.
#define kIOPMPSPostChargeWaitSecondsKey        "PostChargeWaitSeconds"

// Battery must wait this long (seconds) after being completely discharged 
// before the battery is settled.
#define kIOPMPSPostDishargeWaitSecondsKey      "PostDischargeWaitSeconds"


/* CPU Power Management status keys
 * Pass as arguments to IOPMrootDomain::systemPowerEventOccurred
 * Or as arguments to IOPMSystemPowerEventOccurred()
 * Or to decode the dictionary obtained from IOPMCopyCPUPowerStatus()
 * These keys reflect restrictions placed on the CPU by the system
 * to bring the CPU's power consumption within allowable thermal and 
 * power constraints.
 */


/* kIOPMGraphicsPowerLimitsKey
 *   The key representing the dictionary of graphics power limits.
 *   The dictionary contains the other kIOPMCPUPower keys & their associated
 *   values (e.g. Speed limit, Processor Count, and Schedule limits).
 */
#define kIOPMGraphicsPowerLimitsKey                     "Graphics_Power_Limits"

/* kIOPMGraphicsPowerLimitPerformanceKey
 *   The key representing the percent of overall performance made available
 *   by the graphics chip as a percentage (integer 0 - 100).
 */
#define kIOPMGraphicsPowerLimitPerformanceKey           "Graphics_Power_Performance"



/* kIOPMCPUPowerLimitsKey
 *   The key representing the dictionary of CPU Power Limits.
 *   The dictionary contains the other kIOPMCPUPower keys & their associated
 *   values (e.g. Speed limit, Processor Count, and Schedule limits).
 */
#define kIOPMCPUPowerLimitsKey                          "CPU_Power_Limits"

/* kIOPMCPUPowerLimitProcessorSpeedKey defines the speed & voltage limits placed 
 *   on the CPU.
 *   Represented as a percentage (0-100) of maximum CPU speed.
 */
#define kIOPMCPUPowerLimitProcessorSpeedKey             "CPU_Speed_Limit"

/* kIOPMCPUPowerLimitProcessorCountKey reflects how many, if any, CPUs have been
 *   taken offline. Represented as an integer number of CPUs (0 - Max CPUs).
 */
#define kIOPMCPUPowerLimitProcessorCountKey             "CPU_Available_CPUs"

/* kIOPMCPUPowerLimitSchedulerTimeKey represents the percentage (0-100) of CPU time 
 *   available. 100% at normal operation. The OS may limit this time for a percentage
 *   less than 100%.
 */
#define kIOPMCPUPowerLimitSchedulerTimeKey              "CPU_Scheduler_Limit"


/* Thermal Level Warning Key
 * Indicates the thermal constraints placed on the system. This value may 
 * cause clients to action to consume fewer system resources.
 * The value associated with this warning is defined by the platform.
 */
#define kIOPMThermalLevelWarningKey                     "Thermal_Level_Warning"

/* Thermal Warning Level values
 *      kIOPMThermalWarningLevelNormal - under normal operating conditions
 *      kIOPMThermalWarningLevelDanger - thermal pressure may cause system slowdown
 *      kIOPMThermalWarningLevelCrisis - thermal conditions may cause imminent shutdown
 *
 * The platform may define additional thermal levels if necessary.
 */
enum {
  kIOPMThermalWarningLevelNormal    = 0,
  kIOPMThermalWarningLevelDanger    = 5,
  kIOPMThermalWarningLevelCrisis    = 10
};


// PM Settings Controller setting types
// Settings types used primarily with:
//      IOPMrootDomain::registerPMSettingController
// The values are identical to the similarly named keys for use in user space
// PM settings work. Those keys are defined in IOPMLibPrivate.h.
#define kIOPMSettingWakeOnRingKey                   "Wake On Modem Ring"
#define kIOPMSettingRestartOnPowerLossKey           "Automatic Restart On Power Loss"
#define kIOPMSettingWakeOnACChangeKey               "Wake On AC Change"
#define kIOPMSettingSleepOnPowerButtonKey           "Sleep On Power Button"
#define kIOPMSettingWakeOnClamshellKey              "Wake On Clamshell Open"
#define kIOPMSettingReduceBrightnessKey             "ReduceBrightness"
#define kIOPMSettingDisplaySleepUsesDimKey          "Display Sleep Uses Dim"
#define kIOPMSettingTimeZoneOffsetKey               "TimeZoneOffsetSeconds"
#define kIOPMSettingMobileMotionModuleKey           "MobileMotionModule"
#define kIOPMSettingGraphicsSwitchKey               "GPUSwitch"

// Setting controlling drivers can register to receive scheduled wake data
// Either in "CF seconds" type, or structured calendar data in a formatted
// IOPMCalendarStruct defined below.
#define kIOPMSettingAutoWakeSecondsKey              "wake"
#define kIOPMSettingAutoWakeCalendarKey             "WakeByCalendarDate"
#define kIOPMSettingAutoPowerSecondsKey             "poweron"
#define kIOPMSettingAutoPowerCalendarKey            "PowerByCalendarDate"

// Debug seconds auto wake
// Used by sleep cycling debug tools
#define kIOPMSettingDebugWakeRelativeKey            "WakeRelativeToSleep"
#define kIOPMSettingDebugPowerRelativeKey           "PowerRelativeToShutdown"

// Maintenance wake calendar.
#define kIOPMSettingMaintenanceWakeCalendarKey      "MaintenanceWakeCalendarDate"

struct IOPMCalendarStruct {
    UInt32      year;
    UInt8       month;
    UInt8       day;
    UInt8       hour;
    UInt8       minute;
    UInt8       second;
};
typedef struct IOPMCalendarStruct IOPMCalendarStruct;

// SetAggressiveness types
enum {
    kPMGeneralAggressiveness = 0,
    kPMMinutesToDim,
    kPMMinutesToSpinDown,
    kPMMinutesToSleep,
    kPMEthernetWakeOnLANSettings,
    kPMSetProcessorSpeed,
    kPMPowerSource,
    kPMMotionSensor,
    kPMLastAggressivenessType
};
#define kMaxType (kPMLastAggressivenessType-1)

// SetAggressiveness values for the kPMPowerSource aggressiveness type
enum {
    kIOPMInternalPower = 1,
    kIOPMExternalPower
};

#define kIOREMSleepEnabledKey               "REMSleepEnabled"

// Strings for deciphering the dictionary returned from IOPMCopyBatteryInfo
#define kIOBatteryInfoKey                   "IOBatteryInfo"
#define kIOBatteryCurrentChargeKey          "Current"
#define kIOBatteryCapacityKey               "Capacity"
#define kIOBatteryFlagsKey                  "Flags"
#define kIOBatteryVoltageKey                "Voltage"
#define kIOBatteryAmperageKey               "Amperage"
#define kIOBatteryCycleCountKey             "Cycle Count"

enum {
    kIOBatteryInstalled         = (1 << 2),
    kIOBatteryCharge            = (1 << 1),
    kIOBatteryChargerConnect    = (1 << 0)
};


// Private power management message indicating battery data has changed
// Indicates new data resides in the IORegistry
#define kIOPMMessageBatteryStatusHasChanged         iokit_family_msg(sub_iokit_pmu, 0x100)

// Apple private Legacy messages for re-routing AutoWake and AutoPower messages to the PMU
// through newer user space IOPMSchedulePowerEvent API
#define kIOPMUMessageLegacyAutoWake                 iokit_family_msg(sub_iokit_pmu, 0x200)
#define kIOPMUMessageLegacyAutoPower                iokit_family_msg(sub_iokit_pmu, 0x210)

// For use with IOPMPowerSource bFlags
#define IOPM_POWER_SOURCE_REV   2
enum {
  kIOPMACInstalled      = kIOBatteryChargerConnect,
  kIOPMBatteryCharging  = kIOBatteryCharge,
  kIOPMBatteryInstalled = kIOBatteryInstalled,
  kIOPMUPSInstalled     = (1<<3),
  kIOPMBatteryAtWarn    = (1<<4),
  kIOPMBatteryDepleted  = (1<<5),
  kIOPMACnoChargeCapability = (1<<6),     // AC adapter cannot charge battery
  kIOPMRawLowBattery    = (1<<7),         // used only by  Platform Expert
  kIOPMForceLowSpeed    = (1<<8),         // set by Platfm Expert, chk'd by Pwr Plugin
  kIOPMClosedClamshell  = (1<<9),         // set by PMU - reflects state of the clamshell
  kIOPMClamshellStateOnWake = (1<<10)     // used only by Platform Expert
};


// **********************************************
// Internal power management data structures
// **********************************************

#if KERNEL && __cplusplus
class IOService;

enum {
    kIOPowerEmergencyLevel = 1000
};

enum {
    kIOPMSubclassPolicy,
    kIOPMSuperclassPolicy1
};

struct stateChangeNote{
    IOPMPowerFlags    stateFlags;
    unsigned long    stateNum;
    void *         powerRef;
};
typedef struct stateChangeNote stateChangeNote;

struct IOPowerStateChangeNotification {
    void *        powerRef;
    unsigned long    returnValue;
    unsigned long    stateNumber;
    IOPMPowerFlags    stateFlags;
};
typedef struct IOPowerStateChangeNotification IOPowerStateChangeNotification;
typedef IOPowerStateChangeNotification sleepWakeNote;
#endif /* KERNEL && __cplusplus */

#endif /* ! _IOKIT_IOPM_H */

