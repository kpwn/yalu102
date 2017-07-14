/*
 * Copyright (c) 2008 Apple Inc. All rights reserved.
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
#ifndef __OSKEXT_H__
#define __OSKEXT_H__

#include <sys/cdefs.h>

__BEGIN_DECLS

#include <CoreFoundation/CoreFoundation.h>
//#include <System/libkern/OSReturn.h>
//#include <System/libkern/OSKextLib.h>
//#include <System/libkern/OSKextLibPrivate.h>
#include <mach/mach.h>
#include <mach-o/arch.h>

// xxx - should I use "Clear" everywhere I use "Flush"

/*!
 * @header
 * @ignore CF_EXPORT
 * @ignore 
 *
 * The OSKext library provides a comprehensive interface for creating,
 * examining, and loading kernel extensions (kexts).
 *
 * <b>NOTICE:</b> This library is neither thread-safe nor garbage-collection
 * safe. You must use your own locking with threads sharing access to OSKext.
 * You can not use this library in an application with garbage collection.
 */
 
#pragma mark Types and Constants
/*******************************************************************************
* Types and Constants
*******************************************************************************/

/*!
 * @typedef  OSKextRef
 * @abstract A reference to a kernel extension object.
 *
 * @discussion
 * The OSKextRef type refers to a KXKext object, which represents a kernel
 * extension bundle on disk, from an archive, or loaded into the kernel.
 * OSKext is an opaque type that defines the characteristics and behavior of
 * OSKext objects.
 *
 * The kernel counterpart of OSKext is the OSKext libkern C++ class.
 */
typedef struct __OSKext * OSKextRef
                ;

#define kOSKextBundleExtension  "kext"
#define kOSKextMkextExtension   "mkext"

/*!
 * @typedef OSKextDiagnosticsFlags
 * @constant kOSKextDiagnosticsFlagAll
 * @constant kOSKextDiagnosticsFlagValidation
 * @constant kOSKextDiagnosticsFlagAuthentication
 * @constant kOSKextDiagnosticsFlagDependencies
 * @constant kOSKextDiagnosticsFlagWarnings
 */
enum {
    kOSKextDiagnosticsFlagNone           = (UInt32)       0x0U,

    kOSKextDiagnosticsFlagValidation     = (UInt32)       0x1U,
    kOSKextDiagnosticsFlagAuthentication = (UInt32)       0x2U,
    kOSKextDiagnosticsFlagDependencies   = (UInt32)       0x4U,
    kOSKextDiagnosticsFlagWarnings       = (UInt32)       0x8U,

    kOSKextDiagnosticsFlagAll            = (UInt32)0xFFFFFFFFU,
};
typedef UInt32 OSKextDiagnosticsFlags;
typedef UInt64 OSKextLogFlags;
typedef SInt64 OSKextVersion;
typedef kern_return_t OSReturn;

typedef uint8_t OSKextExcludeLevel;
#define kOSKextExcludeNone  (0)
#define kOSKextExcludeKext  (1)
#define kOSKextExcludeAll   (2)

/* Top-level keys for diagnostics dicts. See @link OSKextCopyDiagnostics@/link.
 */
// xxx - not sure we need to include these, but it's convenient
const CFStringRef kOSKextDiagnosticsValidationKey;
const CFStringRef kOSKextDiagnosticsAuthenticationKey;
const CFStringRef kOSKextDiagnosticsLinkLoadKey;
const CFStringRef kOSKextDiagnosticsDependenciesKey;
const CFStringRef kOSKextDiagnosticsWarningsKey;

#pragma mark Basic CF Functions
/*********************************************************************
* Basic CF Functions
*********************************************************************/

/*!
 * @function OSKextGetTypeID
 * @abstract Returns the type identifier for the OSKext opaque type.
 *
 * @result   The type identifier for the OSKext opaque type.
 */
CF_EXPORT CFTypeID
OSKextGetTypeID(void)
                ;

#pragma mark Module Configuration
/*********************************************************************
* Module Configuration
*********************************************************************/

/*!
 * @function OSKextSetArchitecture
 * @abstract Sets the architecture used for operations on kexts.
 *
 * @param    anArch
 * An <code>NXArchInfo</code> pointer with cputype/subtype or name set.
 * Ownership remains with the caller.
 * Pass <code>NULL</code> to set the architecture to taht of the running kernel.
 *  
 * @result
 * <code>true</code> if the architecture was set as desired,
 * <code>false</code> if <code>anArch</code> was not found;
 * if <code>false</code>, the architecture will now be that
 * of the running kernel.
 * xxx - should it just be left unchanged instead?
 *
 * @discussion
 * The kext library uses this architecture for any architecture-specific
 * operation, such as property lookups
 * (see @link OSKextGetValueForInfoDictionaryKey@/link),
 * dependency resolution, executable access during validation and linking.
 * The kext architecture is initially that of the running kernel (not of
 * the user-space process).
 *
 * This function looks up the system <code>NXArchInfo</code> struct
 * for the struct passed in, first using the cputype/subtype, and if that fails,
 * the name. You can therefore use this function to set the architecture
 * from an architecture name alone by passing an <code>NXArchInfo</code> struct
 * with the desired name,
 * but the cputype/subtype set to CPU_TYPE_ANY and CPU_SUBTYPE_ANY.
 *
 * Changing the kext architecture causes all kexts to flush their load info
 * and dependencies with @link OSKextFlushLoadInfo@/link.
 */
// xxx - should this also have a flushDependenciesFlag?
CF_EXPORT Boolean
OSKextSetArchitecture(const NXArchInfo * archInfo)
                ;

/*!
 * @function OSKextGetArchitecture
 * @abstract Gets the architecutre used for operations on kexts.
 *
 * @result
 * The architecture used for operations on kexts.
 * The caller does not own the returned pointer and should not free it.
 *
 * @discussion
 * The kext library uses this architecture for any architecture-specific
 * operation, such as property lookups
 * (see @link OSKextGetValueForInfoDictionaryKey@/link),
 * dependency resolution, executable access during validation and linking.
 * The kext architecture is initially that of the running kernel (not of
 * the user-space process).
 */
CF_EXPORT const NXArchInfo *
OSKextGetArchitecture(void)
                ;

/*!
 * @function OSKextGetRunningKernelArchitecture
 * @abstract Returns the architecture of the running kernel.
 *
 * @result
 * The architecture of the running kernel.
 * The caller does not own the returned pointer and should not free it.
 *
 * @discussion
 * This function consults the kernel task and returns a pointer to the
 * <code>NXArchInfo</code> struct representing its architecture.
 * The running kernel's architecture does not necessarily match that
 * of user space tasks (for example, a 32-bit user space task could be
 * running with a 64-bit kernel, or vice-versa).
 */
CF_EXPORT const NXArchInfo *
OSKextGetRunningKernelArchitecture(void)
                ;

/*! @function OSKextGetLogFlags 
 @param kext The kext to get the log flags. Pass NULL to get the global flag. */
CF_EXPORT OSKextLogFlags OSKextGetLogFlags(OSKextRef kext);
/*! @function OSKextSetLogFlags */
CF_EXPORT void OSKextSetLogFlags(OSKextRef kext, OSKextLogFlags logFlags);

/*!
 * @typedef OSKextLogFunction
 * @abstract
 * Prototype of a callback function used to log messages from the library.
 *
 * @param    format      A printf(3) style format string.
 *
 * @discussion
 * The default log function simply prints to <code>stderr</code
 * if <code>msgLogSpec</code> has any bits set.
 * If you set the log function to <code>NULL</code>,
 * no log messages will be printed.
 *
 * Log messages have no trailing newline, to accommodate system log facilities.
 */
/* xxx - no CF_EXPORT, compiler dies */
typedef void (*OSKextLogFunction)(
    OSKextLogFlags   flags,
    const char     * format,
    ...)
                ;

/*!
 * @function OSKextSetLogFunction
 * @abstract Sets the function called to log messages.
 *
 * @param    funct  The log function to set.
 *
 * @discussion
 * The default log function simply prints to stderr. If you set the
 * log function to <code>NULL</code>, no log messages will be printed.
 *
 * Log messages have no trailing newline, to accommodate system log facilities.
 */
CF_EXPORT void
OSKextSetLogFunction(OSKextLogFunction func)
                ;

/*!
 * @function OSKextSetSimulatedSafeBoot
 * @abstract Sets whether safe boot mode is simulated in the library.
 *
 * @param    flag  <code>true</code> to simulate safe boot mode,
 *                 <code>false</code> to not simulate.
 *
 * @discussion
 * Kexts without a valid OSBundleRequired property are not allowed to load
 * by the kernel when the system has started in safe boot mode.
 * If you turn on simulated safe boot, the kext library can
 * catch non-loadable kexts and present appropriate diagnostics.
 */
CF_EXPORT void
OSKextSetSimulatedSafeBoot(Boolean flag)
                ;

/*!
 * @function OSKextGetSimulatedSafeBoot
 * @abstract Returns whether safe boot mode is simulated in the library.
 *
 * @result
 * <code>true</code> if safe boot mode is being simulated,
 * <code>false</code> if not.
 *
 * @discussion
 * Kexts without a valid OSBundleRequired property are not allowed to load
 * by the kernel when the system has started in safe boot mode.
 * When simulated safe boot is on, the kext library can
 * catch non-loadable kexts and present appropriate diagnostics.
 */
CF_EXPORT Boolean
OSKextGetSimulatedSafeBoot(void)
                ;

/*!
 * @function OSKextGetActualSafeBoot
 * @abstract Returns whether safe boot mode is active.
 *
 * @result
 * <code>true</code> if safe boot mode is active,
 * <code>false</code> if not.
 *
 * @discussion
 * Kexts without a valid OSBundleRequired property are not allowed to load
 * by the kernel when the system has started in safe boot mode.
 * The kext library ignores actual safe boot mode, leaving decisions
 * regarding safe boot to the kernel.
 * If you want to analyze kexts in safe boot mode using the library,
 * call @link OSKextSetSimulatedSafeBoot@/link.
 */
// xxx - we used to disallow kexts w/debug-log flags in safe boot, too
CF_EXPORT Boolean
OSKextGetActualSafeBoot(void)
                ;

/*!
 * @function OSKextGetSystemExtensionsFolderURLs
 * @abstract Returns the standard system extension folder URLs.
 *
 * @result
 * An array containing the standard system extension folder URLs.
 * Ownership follows the
 * @link //apple_ref/doc/uid/20001148-SW1 Get Rule@/link.
 *
 * @discussion
 * To open all the kexts normally considered for loading by the system,
 * pass the return value of this function to
 * @link OSKextCreateKextsFromURLs@/link.
 */
CF_EXPORT CFArrayRef
OSKextGetSystemExtensionsFolderURLs(void)
                ;

/*!
 * @function OSKextSetRecordsDiagnostics
 * @abstract Sets whether kexts record diagnostic information.
 *
 * @param   flags    Indicates which diagnostics to record.
 *
 * @discussion
 * The kext library can record diagnostic information for kexts as
 * problems are encountered. These diagnostics can consume a fair
 * amount of memory, and should be generated only when desired.
 * Recording of diagnostics is set to
 * @link kOSKextDiagnosticsFlagNone@/link by default.
 * Use this function to turn diagnostics recording off (or back on)
 * for particular diagnostic types.
 *
 * Turning on diagnostics does not calculate or recalculate
 * diagnostics. Call the various functions that operate on kexts
 * to begin accumulating diagnostics.
 *
 * Turning diagnostics off does not clear any existing diagnostics.
 * Use @link OSKextFlushDiagnostics@/link explicitly to clear
 * any diagnostics currently stored.
 *
 * See also @link OSKextCopyDiagnostics@/link,
 * @link OSKextValidate@/link,
 * @link OSKextAuthenticate@/link, and
 * @link OSKextResolveDependencies@/link.
 */
// Could list a pile of see also's here....
CF_EXPORT void
OSKextSetRecordsDiagnostics(OSKextDiagnosticsFlags flags)
                ;

/*!
 * @function OSKextGetRecordsDiagnostics
 * @abstract
 * Returns what kinds of diagnostic information kexts record.
 *
 * @result
 * Flags indicating which types of diagnostics are currently
 * being recorded. You can bitwise-OR or -AND these with other flags
 * to add or remove diagnostics being recorded with
 * @link OSKextSetRecordsDiagnostics@/link.
 */
CF_EXPORT OSKextDiagnosticsFlags
OSKextGetRecordsDiagnostics(void)
                ;


/*!
 * @function OSKextSetUsesCaches
 * @abstract Sets whether the kext library uses cache files.
 *
 * @param   flag
 *          <code>true</code> to use caches,
 *          <code>false</code> to not use caches.
 *
 * @discussion
 * The kext library normally uses caches to speed up reading kexts from
 * the system extensions folders.
 * Use this function to turn off use of caches; this will cause reading
 * of kexts to be dramatically slower.
 *
 * See also @link OSKextGetUsesCaches@/link.
 */
CF_EXPORT void
OSKextSetUsesCaches(Boolean flag)
                ;

/*!
 * @function OSKextGetUsesCaches
 * @abstract
 * Returns whether the OSKext library uses cache files.
 *
 * @result
 * <code>true</code> if the library uses caches to speed up reading of kexts,
 * <code>false</code> if it doesn't.
 *
 * @discussion
 * The kext library normally uses caches to speed up reading kexts from
 * the system extensions folders.
 * Use this function to check whether the library is using caches.
 *
 * See also @link OSKextGetUsesCaches@/link.
 */
CF_EXPORT Boolean
OSKextGetUsesCaches(void)
                ;

#pragma mark Instance Management
/*********************************************************************
* Instance Management
*********************************************************************/

/*!
 * @function OSKextCreate
 * @abstract Read a single kext from an on-disk bundle, without plugins.
 *
 * @param    allocator
 *           The allocator to use to allocate memory for the new object.
 *           Pass <code>NULL</code> or <code>kCFAllocatorDefault</code>
 *           to use the current default allocator.
 * @param    anURL
 *           The location of the bundle for which to create an OSKext object.
 * @result
 * An OSKext object created from the bundle at anURL.
 * Ownership follows the
 * @link //apple_ref/doc/uid/20001148-103029 Create Rule@/link.
 *
 * Returns <code>NULL</code> if there was a memory allocation problem or
 * if the bundle doesn't exist at anURL (see Discussion).
 * May return an existing OSKext object with the reference count incremented.
 *
 * @discussion
 * Once a kext has been created, it is cached in memory; the kext bundle cache
 * is flushed only periodically. OSKextCreate does not check that a cached kext
 * bundle still exists in the filesystem. If a kext bundle is deleted
 * from the filesystem, it is therefore possible for OSKextCreate
 * to return a cached bundle that has actually been deleted.
 *
 * OSKext uses a CFBundle briefly during initialization, but in order
 * to save memory and allow re-reading of kext bundle contents
 * from disk, does not retain it. Applications can save further memory
 * by flushing info dictionaries so that they are read from disk again
 * when needed; see @link OSKextFlushInfoDictionary@/link.
 */
CF_EXPORT OSKextRef
OSKextCreate(
    CFAllocatorRef allocator,
    CFURLRef       anURL)
                ;

/*!
 * @function OSKextCreateKextsFromURL
 * @abstract Reads all kexts at an on-disk URL along with their plugins.
 *
 * @param    allocator
 *           The allocator to use to allocate memory for the new objects.
 *           Pass <code>NULL</code> or <code>kCFAllocatorDefault</code>
 *           to use the current default allocator.
 * @param    anURL       The URL to scan for kernel extensions.
 * @result
 * Returns an array containing the kext objects created,
 * or <code>NULL</code> on failure.
 *
 * @discussion
 * This function scans <code>anURL</code> for kexts.
 * If <code>anURL</code> represents a kext,
 * this function reads that kext and its plugins.
 * If <code>anURL</code> represents a non-kext directory,
 * this functions scans the directory's immediate contents for kext bundles
 * and their plugins. It does not scan recursively; only plugins one level
 * deep are read.
 *
 * If given an URL for a kext that is a plugin of another kext, this
 * function does not scan for further plugins.
 *
 * This function does not scan for or read mkext files. Use
 * @link OSKextCreateKextsFromMkextFile@/link to read an mkext file.
 *
 * See @link OSKextCreate OSKextCreate @/link for information
 * about kext object caching.
 */
CFArrayRef OSKextCreateKextsFromURL(
    CFAllocatorRef allocator,
    CFURLRef anURL)
                ;

/*!
 * @function OSKextCreateKextsFromURLs
 * @abstract Reads all kexts at an array on-disk URLs along with their plugins.
 *
 * @param    allocator
 *           The allocator to use to allocate memory for the new objects.
 *           Pass <code>NULL</code> or <code>kCFAllocatorDefault</code>
 *           to use the current default allocator.
 * @param    arrayOfURLs The URLs to scan for kernel extensions.
 * @result
 * Returns an array containing the kext objects created,
 * or <code>NULL</code> on failure.
 *
 * @discussion
 * This function scans each URL in <code>arrayOfURLs</code> for kexts.
 * If a given URL represents a kext,
 * this function reads that kext and its plugins.
 * If the URL represents a non-kext directory, this functions scans the
 * directory's immediate contents for kext bundles and their plugins.
 * It does not scan recursively; only plugins one level deep are read.
 *
 * If given an URL for a kext that is a plugin of another kext, this
 * function does not scan for further plugins.
 *
 * This function does not scan for or read mkext files. Use
 * @link OSKextCreateKextsFromMkextFile@/link to read an mkext file.
 *
 * See @link OSKextCreate@/link for discussion about kext object caching.
 */
CF_EXPORT CFArrayRef
OSKextCreateKextsFromURLs(
    CFAllocatorRef allocator,
    CFArrayRef arrayOfURLs)
                ;

/*!
 * @function OSKextGetAllKexts
 * @abstract
 * Returns an array containing all of the kexts currently open in the application.
 *
 * @result
 * A CFArray object containing OSKext objects for each open kext
 * in the application. Ownership follows the
 * @link //apple_ref/doc/uid/20001148-SW1 Get Rule@/link.
 *
 * @discussion
 * This function is potentially expensive, so use with care.
 */
CF_EXPORT CFArrayRef
OSKextGetAllKexts(void)
                ;

/*! @function OSKextCopyKextsWithIdentifier */
CF_EXPORT CFArrayRef OSKextCopyKextsWithIdentifier(CFStringRef bundleID);
/*! @function OSKextGetKextsWithIdentifiers 
 @discussion Follows Create Rule.
 */
CF_EXPORT CFArrayRef OSKextGetKextsWithIdentifiers(CFArrayRef bundleIDs);

/*!
 * @function OSKextGetKextWithIdentifierAndVersion
 * @abstract
 * Locate a kext given its program-defined identifier and version.
 *
 * @param    aBundleID
 *           The identifier of the kext to locate.
 *           Note that identifier names are case-sensitive.
 * @param    aVersion
 *           The version of the kext to locate.
 * @result
 * An OSKext object, or <code>NULL</code> if the kext was not found.
 * Ownership follows the
 * @link //apple_ref/doc/uid/20001148-SW1 Get Rule@/link.
 *
 * @discussion
 * For a kext to be located using its identifier and version,
 * the kext object must have already been created.
 * See @link OSKextGetKextWithIdentifierAndVersion@/link for more.
 */
// xxx - check on same-version ordering with folks
CF_EXPORT OSKextRef
OSKextGetKextWithIdentifierAndVersion(
    CFStringRef aBundleID, OSKextVersion aVersion)
                ;

/*!
 * @function OSKextGetKextsWithIdentifier
 * @abstract Locate all kexts with a given program-defined identifier.
 *
 * @param    aBundleID
 *           The identifier of the kext to locate.
 *           Note that identifier names are case-sensitive.
 * @result
 * An CFArray of kext objects with the given identifier,
 * or <code>NULL</code> on error.
 * Ownership follows the
 * @link //apple_ref/doc/uid/20001148-103029 Create Rule@/link.
 *
 * @discussion
 * This function is useful for locating duplicates of a given kext.
 *
 * See @link OSKextGetKextWithIdentifier@/link for general information
 * on looking up kexts by identifier.
 */
CF_EXPORT CFArrayRef
OSKextCopyKextsWithIdentifier(
    CFStringRef aBundleID)
                ;

/*!
 * @function OSKextGetLoadedKextWithIdentifier
 * @abstract
 * Locate the loaded kext with a given program-defined identifier.
 *
 * @param    aBundleID
 *           The identifier of the kext to locate.
 *           Note that identifier names are case-sensitive.
 * @result
 * An OSKext object, or <code>NULL</code> if the kext was not found.
 * Ownership follows the
 * @link //apple_ref/doc/uid/20001148-SW1 Get Rule@/link.
 *
 * @discussion
 * You must call @link OSKextReadLoadedKextInfo@/link before calling this
 * function for it to find anything.
 *
 * See @link OSKextGetKextWithIdentifier@/link for general information
 * on looking up kexts by identifier.
 */
CF_EXPORT OSKextRef
OSKextGetLoadedKextWithIdentifier(
    CFStringRef aBundleID)
                ;

/*!
 * @function OSKextGetCompatibleKextWithIdentifier
 * @abstract
 * Locate the kext with a given program-defined identifier
 * that is compatible with a requested version.
 *
 * @param    aBundleID
 *           The identifier of the kext to locate.
 *           Note that identifier names are case-sensitive.
 * @param    requestedVersion
 *           The version that the kext must be compatible with.
 * @result
 * An OSKext object, or <code>NULL</code> if the kext was not found.
 * Ownership follows the
 * @link //apple_ref/doc/uid/20001148-SW1 Get Rule@/link.
 *
 * @discussion
 * A kext is compatible with a version if that version lies between
 * its CFBundleVersion and its OSBundleCompatibleVersion (inclusive).
 *
 * See @link OSKextGetKextWithIdentifier@/link for general information
 * on looking up kexts by identifier.
 */
CF_EXPORT OSKextRef
OSKextGetCompatibleKextWithIdentifier(
    CFStringRef  aBundleID,
    OSKextVersion requestedVersion)
                ;

#pragma mark Basic Accessors
/*********************************************************************
* Basic Accessors
*********************************************************************/

/*!
 * @function OSKextGetURL
 * @abstract Returns the CFURL of a kext bundle.
 *
 * @param    aKext  The kext to get the URL for.
 * @result   A CFURL object. Ownership follows the
 * @link //apple_ref/doc/uid/20001148-SW1 Get Rule@/link.
 *
 * @discussion
 * This function returns the CFURL that <code>aKext</code>
 * was originally created with.
 * It should be resolved to its base for comparison with other kext
 * URLs.
 */
CF_EXPORT CFURLRef
OSKextGetURL(OSKextRef aKext)
                ; // always absolute

/*!
 * @function OSKextGetIdentifier
 * @abstract Returns the bundle identifier from a kext's information property list.
 *
 * @param    aKext  The kext to get the identifier for.
 * @result
 * A CFString object containing the kext bundle's identifier,
 * or <code>NULL</code> if none was specified in the information property list.
 * Ownership follows the
 * @link //apple_ref/doc/uid/20001148-SW1 Get Rule@/link.
 */
CF_EXPORT CFStringRef
OSKextGetIdentifier(OSKextRef aKext)
                ;

/*!
 * @function OSKextGetValueForInfoDictionaryKey
 * @abstract
 * Returns a (possibly architecture-specific) value
 * in a kext's information dictionary.
 *
 * @param    aKext  The kext to get the property for.
 * @param    key    The base key identifying the property.
 * @result
 * A value corresponding to <code>key</code> in <code>aKext</code>'s
 * information dictionary.
 * If available, an architecture-specific value is returned,
 * otherwise the global value is returned. Ownership follows the
 * @link //apple_ref/doc/uid/20001148-SW1 Get Rule@/link.
 *
 * @discussion
 * This function first looks for a property value specific to the current kext
 * architecture, as set with @link OSKextSetArchitecture@/link,
 * by appending an underscore plus the architecture name to the base key.
 * If such a value is not found, it looks for the property using the base key.
 * For example, if the currently set architecture is i386, and
 * IOKitPersonalities is requested, this function looks up the key 
 * IOKitPersonalities_i386; if that doesn't exist, then it uses
 * IOKitPersonalities.
 *
 * Some properties are not allowed to be made architecture-specific;
 * if such keys are defined, they are ignored by this function:
 *
 * * CFBundleIdentifier
 * * CFBundleVersion
 * * CFBundleExecutable
 * * OSBundleIsInterface
 * * OSKernelResource
 *
 * This function looks up architecture-specific values for generic bundle
 * properties, such as CFBundlePackageType, but such values are of course
 * ignored by CFBundle.
 *
 * If you want to look up a property with a raw key, get the information
 * dictionary directly using  @link OSKextCopyInfoDictionary@/link or
 * by opening a CFBundle for the kext's URL.
 *
 * This function reads the info dictionary from disk if necessary.
 * See @link OSKextFlushInfoDictionary@/link.
 */
CF_EXPORT CFTypeRef
OSKextGetValueForInfoDictionaryKey(
    OSKextRef   aKext,
    CFStringRef key)
                ;

/*!
 * @function OSKextCopyInfoDictionary
 * @abstract Returns a copy of a kext's information dictionary.
 *
 * @param    aKext  The kext to get the information dictionary for.
 * @result
 * A CFDictionary object containing the data stored in the kext's
 * information property list.
 * OSKext may add extra keys to this dictionary for its own use.
 * Ownership follows the
 * @link //apple_ref/doc/uid/20001148-103029 Create Rule@/link.
 *
 * @discussion
 * This function uses @link IOCFUnserialize@/link to parse the XML
 * of the info dictionary, in order to match the parsing used in the kernel.
 */
CF_EXPORT CFMutableDictionaryRef
OSKextCopyInfoDictionary(OSKextRef aKext)
                ;

/*!
 * @function OSKextFlushInfoDictionary
 * @abstract Releases a kext's info dictionary.
 *
 * @param    aKext  The kext that should release its info dictionary.
 *                  If <code>NULL</code>, all open kexts' info dictionaries
 *                  are flushed.
 *
 * @discussion
 * OSKexts accumulate a fair amount of information as they are created
 * and used; when many kext objects are created the total memory
 * consumption can be significant. If an application won't be using
 * kexts for long periods of time, it can flush this information
 * to save memory; the information will later be re-read from disk
 * or recreated as necessary.
 *
 * Flushing info dictionaries also allows kext objects to synchronize
 * with updates on disk; they will automatically re-sort in the
 * internal lookup tables to reflect changed versions, for example.
 * (xxx - well they will! that's not fully implemented yet)
 *
 * See also @link OSKextFlushDependencies@/link,
 * @link OSKextFlushLoadInfo@/link,
 * and @link OSKextFlushDiagnostics@/link.
 *
 * Kext objects created from an mkext cannot flush their info
 * dictionaries.
 */
CF_EXPORT void
OSKextFlushInfoDictionary(OSKextRef aKext)
                ;

/*!
 * @function OSKextGetVersion
 * @abstract Returns the version of a kext.
 *
 * @param    aKext  The kext to get the version for.
 * @result   The version of the kext as a 64-bit integer.
 *
 * @discussion
 * xxx - needs more info on version format
 * xxx - need to fix definition of OSKextVersion.
 */
CF_EXPORT OSKextVersion
OSKextGetVersion(OSKextRef aKext)
                ;

/*!
 * @function OSKextGetCompatibleVersion
 * @abstract Returns the compatible version of a kext
 *
 * @param    aKext  The kext to get the compatible version for.
 * @result   The compatible version of the kext as a 64-bit integer.
 *
 * @discussion
 * This function returns the value of a kext's OSBundleCompatibleVersion
 * property, parsed into a 64-bit integer that can be compared using
 * standard integer comparison operators.
 *
 * xxx - needs more info on version format, ref to tn: http://developer.apple.com/technotes/tn/tn1132.html
 */
CF_EXPORT OSKextVersion
OSKextGetCompatibleVersion(OSKextRef aKext)
                ;

/*!
 * @function OSKextCopyUUIDForArchitecture
 * @abstract
 * Returns the compiler-generated UUID of a kext for a given architecture.
 *
 * @param    aKext   The kext to get the UUID for.
 * @param    anArch  The architecture desired.
 *                   Pass <code>NULL</code> to get the UUID for the
 *                   current architecture.
 * @result
 * A CFData object containing the UUID of the kext's executable
 * for <code>anArch</code>, or the currently set architecture if
 * <code>anArch</code> is <code>NULL</code>.
 *
 * @discussion
 * UUIDs are used in addition to bundle versions to check the identify of
 * kexts loaded in the kernel.
 */
CF_EXPORT CFDataRef
OSKextCopyUUIDForArchitecture(OSKextRef aKext,
    const NXArchInfo * anArch)
                ;

/*!
 * @function OSKextIsKernelComponent
 * @abstract Returns whether a kext represents a kerne programming interface.
 *
 * @param    aKext  The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> represents
 * a kerne programming interface, <code>false</code> otherwise.
 *
 * @discussion
 * A small set of kexts represent interfaces built into the kernel that can
 * be linked against individually. These are commonly known as
 * kernel programming interfaces (KPIs),
 * and the kexts containing them as "pseudokexts".
 *
 * If a kext is a kernel component, then it is also always an interface
 * (see @link OSKextIsInterface@/link).
 */
CF_EXPORT Boolean
OSKextIsKernelComponent(OSKextRef aKext)
                ;

/*!
 * @function OSKextIsInterface
 * @abstract
 * Returns whether a kext acts as a linkage subset for another kext,
 * also known as a symbol set.
 *
 * @param    aKext  The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> is an interface kext,
 * <code>false</code> otherwise.
 *
 * @discussion
 * An interface kext has no actual code in its executable, but merely
 * re-exports a set of symbols (typically a subset)
 * from those of its dependencies.
 *
 * Currently the only interface kexts are the kernel component kexts
 * the define the kernel programming interfaces for Mac OS X.
 */
CF_EXPORT Boolean
OSKextIsInterface(OSKextRef aKext)
                ;

/*!
 * @function OSKextIsLibrary
 * @abstract Returns whether a kext can be declared as a library.
 *
 * @param    aKext  The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> is a library kext, <code>false</code>
 * otherwise.
 *
 * @discussion
 * A kext is a library kext if it has a valid OSBundleCompatibleVersion
 * property. Another kext can link against a library kext by listing
 * the library's identifier and required version in its OSBundleLibraries
 * property.
 */
CF_EXPORT Boolean
OSKextIsLibrary(OSKextRef aKext)
                ;

/*!
 * @function OSKextDeclaresExecutable
 * @abstract Returns whether a kext declares a CFBundleExecutable property.
 *
 * @param    aKext  The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> has a nonempty CFBundleExecutable
 * property, <code>false</code> otherwise.
 *
 * @discussion
 * A kext with an executable is either a loadable kext with actual executable
 * code, or an interface kext whose executable serves to restrict linkage
 * to a subset of the symbols of another kext. See @link OSKextIsInterface@/link
 * for more on the latter type.
 */
CF_EXPORT Boolean
OSKextDeclaresExecutable(OSKextRef aKext)
                ;

/*!
 * @function OSKextHasLogOrDebugFlags
 * @abstract
 * Returns whether a kext has OSBundleEnableKextLogging set to a true value or
 * any of its IOKitPersonalities has a nonzero IOKitDebug property.
 *
 * @param    aKext  The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> has a true OSBundleEnableKextLogging
 * property or if any personality for the currently set architecture has a
 * nonzero IOKitDebug property, <code>false</code> otherwise.
 *
 * @discussion
 * Kexts built for distribution should not have any logging or debug flags set.
 */
CF_EXPORT Boolean
OSKextHasLogOrDebugFlags(OSKextRef aKext)
                ;

/*!
 * @function OSKextIsLoadableInSafeBoot
 * @abstract Returns whether the kernel will load a kext during safe boot.
 *
 * @param    aKext  The kext to examine.
 * @result
 * <code>true</code> if the kernel will allow <code>aKext</code> during
 * safe boot, <code>false</code> otherwise.
 *
 * @discussion
 * A kext is loadable during safe boot if it has an OSBundleRequired
 * property for the kernel's architecture with a value of
 * "Root", "Local-Root", "Network-Root", "Console", or "Safe Boot".
 *
 * This function does not generally cover the issue of loadability due
 * to problems with validation, authentication, or dependency resolution.
 * To determine whether a kext can actually be loaded, use
 * @link OSKextIsLoadable@/link.
 */
CF_EXPORT Boolean
OSKextIsLoadableInSafeBoot(OSKextRef aKext)
                ;

/*!
 * @function OSKextCopyArchitectures
 * @abstract
 * Returns a list of <code>NXArchInfo</code> structs for the architectures
 * found in a kext's executable.
 *
 * @param    aKext  The kext to examine.
 * @result
 * A <code>NULL</code>-terminated list of <code>NXArchInfo</code>
 * struct pointers describing
 * the architectures supported by <code>aKext</code>,
 * or <code>NULL</code> if <code>aKext</code> has no executable.
 * The caller is responsible for freeing the list, but not the individual
 * <code>NXArchInfo</code> pointers.
 */
CF_EXPORT const NXArchInfo **
OSKextCopyArchitectures(OSKextRef aKext)
                ;

/*!
 * @function OSKextSupportsArchitecture
 * @abstract Returns whether a kext has an executable for a given architecture.
 *
 * @param    aKext     The kext to examine.
 * @param    anArch    The <code>NXArchInfo</code> description to check.
 *                     If <code>NULL</code>, uses the currently set architecture.
 * @result
 * <code>true</code> if <code>aKext</code>'s executable contains code for
 * <code>anArch</code>.
 *
 * @discussion
 */
// null for current (NOT host) arch
Boolean    OSKextSupportsArchitecture(OSKextRef aKext,
    const NXArchInfo * anArch)
;  // if NULL, uses current default

/*!
 * @function OSKextCopyPlugins
 * @abstract Retrieve the plugins of a kext.
 *
 * @param    aKext  The kext to get plugins for.
 * @result
 * An array containing the kexts that are plugins of <code>aKext</code>.
 *
 * @discussion
 * This function scans the plugins folder of <code>aKext</code>
 * and returns an array of the kexts found.
 * This may result in the creation of new kext objects.
 *
 * If <code>aKext</code> is a plugin of another kext 
 * see @link OSKextIsPlugin@/link),
 * this function returns an empty array.
 */
CF_EXPORT CFArrayRef
OSKextCopyPlugins(OSKextRef aKext)
                ;
                
/*!
 * @function OSKextIsPlugin
 * @abstract Returns whether a kext is a plugin of some other kext.
 *
 * @param    aKext     The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> is a plugin of another kext,
 * <code>false</code> otherwise.
 *
 * @discussion
 * This function uses a simple algorithm that just checks for the substring
 * ".kext/" in the absolute path leading up to the kext's bundle name.
 */
CF_EXPORT Boolean
OSKextIsPlugin(OSKextRef aKext)
                ;

/*!
 * @function OSKextCopyContainerForPluginKext
 * @abstract Returns the kext that is the container for a plugin kext.
 *
 * @param    aKext     The kext to find a parent for.
 * @result
 * The kext that contains <code>aKext</code> in its PlugIns folder,
 * or <code>NULL</code> if <code>aKext</code> is not a plugin.
 * Ownership follows the
 * @link //apple_ref/doc/uid/20001148-103029 Create Rule@/link.
 *
 * @discussion
 * This function creates a kext object for the longest portion of the
 * path leading to <code>aKext</code>, then checks to see whether
 * <code>aKext</code> is a plugin of that kext by <code>CFBundle</code>'s
 * semantics. If so, that kext is returned.
 * This may result in the creation of a new kext.
 */
CF_EXPORT OSKextRef
OSKextCopyContainerForPluginKext(OSKextRef aKext)
                ;

/*!
 * @function OSKextCopyPersonalitiesArray
 * @abstract Returns the personalities of a kext for the current kext architecture.
 *
 * @param    aKext  The kext to retrieve personalities for.
 * @result   An array of IOKit personality dictionaries.
 *
 * @discussion
 * IOKitPersonalities in kexts require some processing before being sent
 * to the kernel, and the functions that send personalities expect an array
 * rather than a dictionary. This function facilitates such use.
 *
 * Use @link OSKextGetValueForInfoDictionaryKey@/link to retrieve the raw
 * personalities dictionary for the current architecture.
 */
CF_EXPORT CFArrayRef
OSKextCopyPersonalitiesArray(OSKextRef aKext)
                ;

// xxx - would it be awful to have a function that takes a CFTypeRef that's
// xxx - a single kext, an array of kexts, or NULL for all open kexts?

/*!
 * @function OSKextCopyPersonalitiesOfKexts
 * @abstract
 * Returns the personalities of multiple kexts for the current kext architecture.
 *
 * @param    kextArray  The kexts to retrieve personalities for.
 *           Pass <code>NULL</code> to retrieve personalities for all open kexts.
 * @result   An array of IOKit personality dictionaries.
 *
 * @discussion
 * IOKitPersonalities in kexts require some processing before being sent
 * to the kernel, and the functions that send personalities expect an array
 * rather than a dictionary. This function facilitates such use.
 */
CF_EXPORT CFArrayRef
OSKextCopyPersonalitiesOfKexts(CFArrayRef kextArray)
                ;

/*!
 * @function OSKextCopyExecutableForArchitecture
 * @abstract
 * Returns a kext's Mach-O executable, thinned to a single architecture
 * if desired.
 *
 * @param    aKext   The kext to retrieve the executable from.
 * @param    anArch  The architecture desired.
 *                   Pass <code>NULL</code> to get the whole, unthinned exectuable.
 * @result
 * A <code>CFData</code> object containing the code
 * for the specified architecture, or <code>NULL</code> if it can't be found.
 *
 * @discussion
 * Note that this function does not use the default kext architecture set by
 * @link OSKextSetArchitecture@/link when given no architecture.
 */
CF_EXPORT CFDataRef
OSKextCopyExecutableForArchitecture(OSKextRef aKext,
    const NXArchInfo * anArch)
                ;

/*!
 * @function OSKextCopyResource
 * @abstract Returns a named resource from a kext's bundle.
 *
 * @param    aKext         The kext to get a resource from.
 * @param    resourceName  The name of the requested resource.
 * @param    resourceType  The abstract type of the requested resource.
 *                         The type is expressed as a filename extension,
 *                         such as <code>jpg</code>.
 *                         Pass <code>NULL</code> if you don’t need
 *                         to search by type.
 * 
 * @result
 * A <code>CFData</code> object containing the resource file's contents,
 * or <code>NULL</code> if the resource can't be found.
 *
 * @discussion
 * For a kext created from an on-disk URL, this function uses CFBundle
 * semantics to locate the resource file. Note that localized resources
 * and resource subdirectories are not available to kexts calling
 * @link //apple_ref/c/func/OSKextRequestResource OSKextRequestResource@/link
 * from within the kernel (because the process providing them runs as root).
 * You can store such resources in a kext bundle and use them in applications,
 * but they will not be available to the kext loaded in kernel.
 */
CF_EXPORT CFDataRef
OSKextCopyResource(
    OSKextRef   aKext,
    CFStringRef resourceName,
    CFStringRef resourceType)
                ;

#pragma mark Dependency Resolution
/*********************************************************************
* Dependency Resolution
*********************************************************************/

/*!
 * @function OSKextResolveDependencies
 * @abstract Calculate the dependency graph for a kext.
 *
 * @param    aKext   The kext to resolve dependencies for.
 * @result
 * <code>true</code> if all dependencies are successfully resolved,
 * <code>false</code> otherwise.
 *
 * @discussion
 * This function examines the OSBundleLibraries property of <code>aKext</code>
 * and looks for compatible open kexts. Priority is given to kexts marked as
 * loaded (see @link OSKextReadLoadedKextInfo@/link and
 * @link OSKextFlushLoadInfo@/link). Otherwise the most recent
 * compatible version is used; if multiple kexts with the same identifier and
 * version exist, the last one created is used.
 *
 * Any problems resolving dependencies are stored in a diagnostics dictionary,
 * which you can retrieve using @link OSKextCopyDiagnostics@/link.
 *
 * If a kext's dependencies have already been resolved, this function does
 * no work and returns <code>true</code>.
 * If you want to recalculate a kext's dependencies, call
 * @link OSKextFlushDependencies@/link first.
 */
// xxx - check on same-version ordering with folks
CF_EXPORT Boolean
OSKextResolveDependencies(OSKextRef aKext)
                ;

/*!
 * @function OSKextFlushDependencies
 * @abstract Clears the dependency graph of a kext.
 *
 * @param    aKext   The kext to flush dependencies for.
 *                   Pass <code>NULL</code> to flush dependencies for all kexts.
 *
 * @discussion
 * OSKexts accumulate a fair amount of information as they are created
 * and used; when many kext objects are created the total memory
 * consumption can be significant. If an application won't be using
 * kexts for long periods of time, it can flush this information
 * to save memory; the information will later be re-read from disk
 * or recreated as necessary.
 *
 * Flushing dependencies is also useful when readling loaded kext
 * information from the kernel with @link OSKextReadLoadedKextInfo@/link
 * (in fact, that function takes a flag to do it as part of its work).
 * Dependency resolution gives priority to kexts marked as loaded,
 * so it's wise to call flush dependencies before reading load info
 * and before doing any operation that resolves dependencies.
 * Conversely, if you want to resolve dependencies without consideration
 * for which kexts are loaded, call @link OSKextFlushLoadInfo@/link
 * beforehand (with or without the flag to flush dependencies).
 *
 * This function also clears any dependency resolution diagnostics.
 * See also @link OSKextFlushInfoDictionary@/link,
 * @link OSKextFlushLoadInfo@/link,
 * and @link OSKextFlushDiagnostics@/link.
 */
CF_EXPORT void
OSKextFlushDependencies(OSKextRef aKext)
                ;

/*!
 * @function OSKextCopyDeclaredDependencies
 * @abstract Returns the kexts identified in a kext's OSBundleLibraries property.
 *
 * @param    aKext         The kext to get dependencies for.
 * @param    needAllFlag   If <code>true</code>, the function returns
 *                         <code>NULL</code> if any dependency isn't found.
 * @result
 * A <code>CFArray</code> containing the kexts found.
 * If <code>needAllFlag</code> is <code>false</code>, the array may be missing
 * some kexts. If <code>needAllFlag</code> is <code>true</code> and not all
 * dependencies are found, this function returns <code>NULL</code>.
 *
 * @discussion
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
CF_EXPORT CFArrayRef
OSKextCopyDeclaredDependencies(
    OSKextRef aKext,
    Boolean   needAllFlag)
                ;
    

/*!
 * @function OSKextCopyLinkDependencies
 * @abstract Returns the kexts that a kext links directly against.
 *
 * @param    aKext         The kext to get dependencies for.
 * @param    needAllFlag   If <code>true</code>, the function returns
 *                         <code>NULL</code> if any dependency isn't found.
 * @result
 * A <code>CFArray</code> containing the kexts found.
 * If <code>needAllFlag</code> is <code>false</code>, the array may be missing
 * some kexts. If <code>needAllFlag</code> is <code>true</code> and not all
 * dependencies are found, this function returns <code>NULL</code>.
 *
 * @discussion
 * Link dependencies are how loaded kext relationships are tracked
 * in the kernel (as shown by the <code>kextstat(8)</code> program).
 * Some library kexts contain no executable, merely collecting sets of
 * other libraries for convenience or compatiblity purposes.
 * This function follows through such indirect libraries to find kexts
 * with executables that <code>aKext</code> will actually link against.
 *
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
 // xxx - need to insert a manpage link on kextstat
CF_EXPORT CFArrayRef
OSKextCopyLinkDependencies(
    OSKextRef aKext,
    Boolean   needAllFlag)
                ;

/*!
 * @function OSKextCopyLoadList
 * @abstract
 * Returns the entire list of kexts in the load graph of a kext
 * (including that kext).
 *
 * @param    aKext         The kext to get the load list for.
 * @param    needAllFlag   If <code>true</code>, the function returns
 *                         <code>NULL</code> if any dependency isn't found.
 * @result
 * A <code>CFArray</code> containing the kexts found.
 * If <code>needAllFlag</code> is <code>false</code>, the array may be missing
 * some kexts. If <code>needAllFlag</code> is <code>true</code> and not all
 * dependencies are found, this function returns <code>NULL</code>.
 *
 * @discussion
 * The fully-resolved load list represents all the kexts needed to load
 * <code>aKext</code> into the kernel, in an order guaranteed to work for
 * sequential starting and matching with IOKit personalities.
 *
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
CF_EXPORT CFMutableArrayRef
OSKextCopyLoadList(
    OSKextRef aKext,
    Boolean   needAllFlag)
                ;

/*!
 * @function OSKextCopyLoadListForKexts
 * @abstract
 * Returns the entire list of kexts in load order of the merged load graph 
 * of the provided list of kexts (including those kexts).
 *
 * @param       kexts       The kexts to get the load list for.
 * @param       needAllFlag If <code>true</code>, the function returns 
 *                          <code>NULL</code> if any dependency isn't found.
 *
 * @result
 * A <code>CFArray</code> containing the kexts found.
 * If <code>needAllFlag</code> is <code>false</code>, the array may be missing
 * some kexts. If <code>needAllFlag</code> is <code>true</code> and not all
 * dependencies are found, this function returns <code>NULL</code>.
 *
 * @discussion
 * The fully-resolved load list represents all of the kexts needed to load
 * every kext in <code>kexts</code> into the kernel, in an order guaranteed to
 * work for sequential starting and matching with IOKit personalities.
 * 
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
CF_EXPORT CFMutableArrayRef 
OSKextCopyLoadListForKexts(
    CFArrayRef  kexts, 
    Boolean     needAllFlag)
                ;


/*!
 * @function OSKextCopyAllDependencies
 * @abstract
 * Returns all kexts that a given kexts depends on, directly or indirectly.
 *
 * @param    aKext         The kext to get dependencies for.
 * @param    needAllFlag   If <code>true</code>, the function returns
 *                         <code>NULL</code> if any dependency isn't found.
 * @result
 * A <code>CFArray</code> containing the kexts found.
 * If <code>needAllFlag</code> is <code>false</code>, the array may be missing
 * some kexts. If <code>needAllFlag</code> is <code>true</code> and not all
 * dependencies are found, this function returns <code>NULL</code>.
 *
 * @discussion
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
CF_EXPORT CFMutableArrayRef
OSKextCopyAllDependencies(
    OSKextRef aKext,
    Boolean   needAllFlag)
                ;

/*!
 * @function OSKextCopyIndirectDependencies
 * @abstract
 * Returns all kexts that a given kexts depends on indirectly.
 *
 * @param    aKext         The kext to get dependencies for.
 * @param    needAllFlag   If <code>true</code>, the function returns
 *                         <code>NULL</code> if any dependency isn't found.
 * @result
 * A <code>CFArray</code> containing the kexts found.
 * If <code>needAllFlag</code> is <code>false</code>, the array may be missing
 * some kexts. If <code>needAllFlag</code> is <code>true</code> and not all
 * dependencies are found, this function returns <code>NULL</code>.
 *
 * @discussion
 * Note that the list of indirect dependencies includes all kexts declared
 * as dependencies by the direct dependencies of <code>aKext</code> -
 * it may therefore include a direct dependency as well if some other
 * kext in the load graph declares it.
 *
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
// xxx - This one isn't really useful, is it?
CFMutableArrayRef OSKextCopyIndirectDependencies(
    OSKextRef aKext,
    Boolean   needAllFlag)
;

/*!
 * @function OSKextDependsOnKext
 * @abstract
 * Returns whether a kext depends on a given library kext,
 * directly or indirectly.
 * 
 * @param    aKext         The kext to examine.
 * @param    libraryKext   The possible library kext.
 * @param    directFlag
 *           If <code>true</code>, only checks for a direct declared dependency;
 *           if <code>false</code> check for direct or indirect dependencies.
 * @result
 * <code>true</code> if <code>aKext</code> depends on <code>libraryKext</code>,
 * either directly or indirectly per <code>directFlag</code>.
 * Returns <code>false</code> otherwise.
 *
 * @discussion
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 *
 * This function works with actual dependency resolution, not potential.
 * If there are multiple kexts with the same bundle identifier,
 * <code>aKext</code> may not be the one chosen during resolution
 * and so it might appear that no kexts depend on it, even though they could.
 */
CF_EXPORT Boolean
OSKextDependsOnKext(
    OSKextRef aKext,
    OSKextRef libraryKext,
    Boolean   directFlag)
                ;

/*!
 * @function OSKextCopyDependents
 * @abstract
 * Return all kexts that depend on a given kext, directly or indirectly.
 *
 * @param    aKext         The kext to get dependents for.
 * @param    directFlag
 *           <code>true</code> to get only kexts that declare a direct
 *           dependency on <code>aKext</code>,
 *           <code>false</code> to get all dependents, direct or indirect.
 * @result
 * An array of all kexts that ultimately depend on <code>aKext</code>,
 * directly or indirectly according to <code>directFlag</code>.
 *
 * @discussion
 * This function calls @link OSKextResolveDependencies@/link on all open
 * kexts to find dependencies. This can be somewhat expensive.
 *
 * This function works with actual dependency resolution, not potential.
 * If there are multiple kexts with the same bundle identifier,
 * <code>aKext</code> may not be the one chosen during resolution
 * and so it might appear that no kexts depend on it, even though they could.
 */
CF_EXPORT CFMutableArrayRef
OSKextCopyDependents(OSKextRef aKext,
    Boolean directFlag)
                ;

/*!
 * @function OSKextIsCompatibleWithVersion
 * @abstract Returns whether a library kext is compatible with a given version.
 *
 * @param    aKext      The kext to examine.
 * @param    aVersion   The kext version to check compatibility with
 *
 * @result
 * <code>truer</code> if <code>aKext</code> has a compatible version and
 * if <code>aVersion</code> is between the version and compatible version of
 * <code>aKext</code> (inclusive), <code>false</code> otherwise.
 *
 * @discussion
 */
CF_EXPORT Boolean
OSKextIsCompatibleWithVersion(
    OSKextRef    aKext,
    OSKextVersion aVersion)
                ;

/*!
 * @function OSKextLogDependencyGraph
 * @abstract Prints the resolved dependency graph of a kext.
 *
 * @param    aKext         The kext to log the dependency graph of.
 * @param    bundleIDFlag  <code>true</code> to log kexts by bundle ID,
 *                         <code>false</code> to log them by URL. 
 * @param    linkFlag      <code>true</code> to log the link graph only,
 *                         <code>false</code> to log the full dependency graph.
 *
 * @discussion
 * <code>linkFlag</code> allows you to display the dependencies as they will
 * be recorded in the kernel when the kext is loaded.
 * If it is <code>true</code>, then only kexts with executables are included
 * in the logged dependency graph. If it is <code>false</code>, all kexts
 * needed to load </code>aKext</code> are included.
 *
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
CF_EXPORT void
OSKextLogDependencyGraph(OSKextRef aKext,
    Boolean bundleIDFlag,
    Boolean linkFlag)
                ;

/*!
 * @function OSKextFindLinkDependencies
 * @abstract Finds kexts that define symbols a kext needs resolved.
 *
 * @param    aKext              The kext to examine.
 * @param    nonKPIFlag
 *           <code>false</code> to look in com.apple.kpi.* kernel component
 *           for kernel symbols;
 *           <code>true</code> to look in com.apple.kernel.* kernel components
 *           instead (not recommended for later versions of Mac OS X).
 * @param    allowUnsupportedFlag
 *           <code>false</code> to skip unsupported libraries in the search;
 *           <code>true</code> to include them.
 * @param    onedefSymbolsOut
 *           A dictionary containing symbols that were found once;
 *           keys are the symbols, values are the kexts defining the key symbol.
 *           Ownership follows the Create Rule. 
 *           Pass <code>NULL</code> to not retrieve.
 * @param    undefinedSymbolsOut
 *           A dictionary containing symbols that weren't found;
 *           keys are the symbols, values are undefined.
 *           Ownership follows the Create Rule. 
 *           Pass <code>NULL</code> to not retrieve.
 * @param    multiplyDefinedSymbolsOut
 *           A dictionary containing symbols found in multiple library kexts;
 *           keys are the symbols, values are the kexts defining the key symbol.
 *           Ownership follows the Create Rule. 
 *           Pass <code>NULL</code> to not retrieve.
 * @param    multipleDefinitionLibraries
 *           A array of all library kexts in which multiply defined symbols
 *           were found; sorted by CFBundleIdentifier.

 *           Ownership follows the Create Rule. 
 *           Pass <code>NULL</code> to not retrieve.
 * @result
 * An array of kexts that export symbols referenced by <code>aKext</code>,
 * sorted by CFBundleIdentifier.
 *
 * @discussion
 * This function searches in all open kexts for symbols referenced by
 * <code>aKext</code>, ignoring the OSBundleLibraries property.
 * You can use this function to find out what you should list
 * in a kext's OSBundleLibraries property.
 *
 * If <code>undefinedSymbolsOut</code> has a nonzero count,
 * the symbols named by its keys could not be found in any open kext.
 *
 * If <code>multiplyDefinedSymbolsOut</code> has a nonzero count,
 * some of the result kexts define the same symbol,
 * and if those duplicates are listed
 * in OSBundleLibraries a link failure will occur.
 * You can inspect the contents of <code>multiplyDefinedSymbolsOut</code>
 * and <code>multipleDefinitionLibraries</code>
 * by hand to sort out which libraries you should actually declare.
 *
 * This function is fairly expensive, as it has to search through all open
 * kexts' information dictionaries, and all library kexts' executables.
 */
CFArrayRef OSKextFindLinkDependencies(
    OSKextRef         aKext,
    Boolean           nonKPIFlag,
    Boolean           allowUnsupportedFlag,
    CFDictionaryRef * undefinedSymbolsOut,
    CFDictionaryRef * onedefSymbolsOut,
    CFDictionaryRef * multiplyDefinedSymbolsOut,
    CFArrayRef      * multipleDefinitionLibrariesOut)
                ;

#pragma mark Linking and Loading; Other Kernel Operations
/*********************************************************************
* Linking and Loading; Other Kernel Operations
*********************************************************************/

/*!
 * @function OSKextLoad
 * @abstract Loads a kext and its dependencies into the kernel.
 *
 * @param    aKext   The kext to load.
 * @result
 * kOSReturnSuccess if <code>aKext</code> is successfully loaded
 * into the kernel, or was already loaded, or an error result if not.
 *
 * @discussion
 * The calling process must have an effective user ID of 0 (root)
 * to load kexts into the kernel.
 *
 * A kext and all its dependencies must pass all validation and authentication
 * tests to be loadable. See @link OSKextIsLoadable@/link for more information.
 *
 * All kexts loaded into the kernel are started, but IOKit personalities
 * are not sent to the IOCatalogue.
 * See @link OSKextSendPersonalitiesToKernel@/link.
 *
 * This function calls @link OSKextFlushLoadInfo@/link and clears dependencies
 * for all open kexts. It then calls @link OSKextCopyLoadList@/link
 * to find dependencies and @link OSKextReadLoadedKextInfo@/link on the
 * resulting load list.
 */
// xxx - need to list errors that may be returned
CF_EXPORT OSReturn
OSKextLoad(OSKextRef aKext)
                ;

/*!
 * @function OSKextLoadWithOptions
 * @abstract Loads a kext and its dependencies into the kernel.
 *
 * @param    aKext                       The kext to load.
 * @param    startExclusion
 *           <code>kOSKextExcludeAll</code> to omit starting <code>aKext</code>
 *           and any of its dependencies not already started,
 *           <code>kOSKextExcludeKext</code> to start the dependencies of
 *           <code>aKext</code> but not <code>aKext</code> itself,
 *           or <code>kOSKextExcludeNone</code> to start all loaded kexts.
 * @param    addPersonalitiesExclusion
 *           <code>kOSKextExcludeAll</code> to omit sending
 *           IOKitPersonalities to the IOCatalogue for <code>aKext</code>
 *           and any of its dependencies
 *           (though they may already be in the IOCatalogue),
 *           <code>kOSKextExcludeKext</code> to send personalities for the
 *           dependencies of <code>aKext</code> but not <code>aKext</code> itself,
 *           or <code>kOSKextExcludeNone</code> to send all personalities.
 * @param    personalityNames
 *           The names of IOKitPersonalities in <code>aKext</code> to send,
 *           <code>addPersonalitiesExclusion</code> allowing.
 *           If <code>NULL</code> all personalities are sent.
 *           This parameter only affects <code>aKext</code>; if dependency
 *           personalities are sent, they are all sent.
 * @param   delayAutounloadFlag
 *           <code>true</code> to cause the kernel's automatic kext unloader
 *           to skip <code>aKext</code> for one cycle, giving extra time
 *           to set up a debug session. <code>false</code> for normal behavior.
 *
 * @result
 * Returns <code>kOSReturnSuccess</code> on success, other values on failure.
 *
 * @discussion
 * This function allows a kext to be loaded but not started or matched
 * (for IOKit kexts), which is useful in some debug scenarios.
 * After calling this function, you may need to call @link OSKextStart@/link
 * to start <code>aKext</code> (along with its dependencies).
 * You may also need to call @link OSKextSendPersonalitiesToKernel@/link
 * for any kexts excluded from matching via
 * <code>addPersonalitiesExclusion</code>.
 * 
 * The calling process must have an effective user ID of 0 (root)
 * to load kexts into the kernel.
 *
 * A kext and all its dependencies must pass all validation and authentication
 * tests to be loadable. See @link OSKextIsLoadable@/link for more information.
 *
 * This function calls @link OSKextFlushLoadInfo@/link and clears dependencies
 * for all open kexts. It then calls @link OSKextCopyLoadList@/link
 * to find dependencies and @link OSKextReadLoadedKextInfo@/link on the
 * resulting load list.
 */
// xxx - need to list errors that may be returned
CF_EXPORT OSReturn
OSKextLoadWithOptions(
    OSKextRef           aKext,
    OSKextExcludeLevel  startExclusion,
    OSKextExcludeLevel  addPersonalitiesExclusion,
    CFArrayRef          personalityNames,
    Boolean             delayAutounloadFlag)
                ;
  
/*!
 * @function OSKextGenerateDebugSymbols
 * @abstract Generate debug symbols for a kext and its dependencies.
 *
 * @param aKext
 *        The kext to generate debug symbols for.
 * @param kernelImage
 *        The kernel Mach-O or symbol file to use for linking.
 *        If <code>NULL</code>, the running kernel is used.
 *
 * @result
 * A dictionary whose keys are the bundle IDs of the kexts for which symbols
 * have been generated with ".sym" appended, and whose values are CFData
 * objects containing the symbol files. Returns <code>NULL</code> on error.
 *
 * @discussion
 * The result includes only non-interface kexts that have an executable
 * and a load address set.
 * Load addresses are set by @link OSKextReadLoadedKextInfo@/link or by
 * @link OSKextSetLoadAddress@/link.
 *
 * If using the running kernel for load addresses, the current architecture
 * set with @link OSKextSetArchitecture@/link must match that of the running
 * kernel or this function returns <code>NULL</code>.
 *
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
CF_EXPORT CFDictionaryRef
OSKextGenerateDebugSymbols(
    OSKextRef aKext,
    CFDataRef kernelImage)
                ;

/*!
 * @function OSKextNeedsLoadAddressForDebugSymbols
 * @abstract
 * Returns whether a kext needs a load address set to generate debug symbols.
 *
 * @param    aKext    The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> needs a load address set
 * in order to generate debug symbols with
 * @link OSKextGenerateDebugSymbols@/link, <code>false</code> otherwise.
 */
CF_EXPORT Boolean
OSKextNeedsLoadAddressForDebugSymbols(OSKextRef aKext)
                ;

/*!
 * @function OSKextUnload
 * @abstract Unloads a kext from the kernel if possible.
 *
 * @param    aKext   The kext to unload.
 * @param    terminateServiceAndRemovePersonalities
 * If <code>true</code>, and the kext to unload has no kexts loaded
 * against it, all IOService objects defined by that kext will be
 * asked to terminate so that the unload can proceed, and the personalities
 * for the kext will be removed from the IOCatalogue.
 *
 * @result
 * <code>kOSReturnSuccess</code> on success, an error code
 * (typically <code>kOSKextReturnInUse</code>) on failure.
 *
 * @discussion
 * The calling process must have an effective user ID of 0 (root)
 * to unload kexts from the kernel.
 * A kext cannot be unloaded if it has any loaed dependents.
 *
 * See also the @link IOCatalogueTerminate@/link function.
 */
// xxx - need to list errors that may be returned
CF_EXPORT OSReturn
OSKextUnload(OSKextRef aKext,
    Boolean terminateServiceAndRemovePersonalities)
                ;

/*!
 * OSKextUnload
 * @abstract Unloads a kext from the kernel if possible, based on its
 *           CFBundleIdentifier.
 *
 * @param    kextIdentifier   The CFBundleIDentifier of the kext to unload.
 * @param    terminateServiceAndRemovePersonalities
 * If <code>true</code>, and the kext to unload has no kexts loaded
 * against it, all IOService objects defined by that kext will be
 * asked to terminate so that the unload can proceed, and the personalities
 * for the kext will be removed from the IOCatalogue.
 *
 * @result
 * <code>kOSReturnSuccess</code> on success, an error code
 * (typically <code>kOSKextReturnInUse</code>) on failure.
 *
 * @discussion
 * The calling process must have an effective user ID of 0 (root)
 * to unload kexts from the kernel.
 * A kext cannot be unloaded if it has any loaed dependents.
 *
 * See also the @link IOCatalogueTerminate@/link function.
 */
// xxx - need to list errors that may be returned
CF_EXPORT OSReturn
OSKextUnloadKextWithIdentifier(CFStringRef kextIdentifier,
    Boolean terminateServiceAndRemovePersonalities)
                ;

/*!
 * @function OSKextIsStarted
 * @abstract Returns whether a kext loaded in the kernel is started.
 *
 * @param    aKext   The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> is active and running in the kernel,
 * <code>false</code> if not loaded or loaded and not started.
 *
 * @discussion
 * This function returns the state recorded the last time
 * @link OSKextReadLoadedKextInfo@/link was called.
 */
CF_EXPORT Boolean
OSKextIsStarted(OSKextRef aKext)
                ;

/*!
 * @function OSKextStart
 * @abstract Starts a kext loaded in the kernel if possible.
 *
 * @param    aKext   The kext to start.
 * @result
 * <code>kOSReturnSuccess</code> on success, an error code on failure.
 *
 * @discussion
 * This function allows an application to start a kext that is loaded
 * in the kernel (typically via @link OSKextLoadWithOptions@/link),
 * and any of its dependencies that aren't also started.
 * If the kext is already started, this does nothing.
 *
 * To start a kext means to call its start function, which is distinct from
 * the start method of an IOService object.
 * 
 * The calling process must have an effective user ID of 0 (root)
 * to start kexts in the kernel.
// xxx - need to list errors that may be returned
 */
CF_EXPORT OSReturn
OSKextStart(OSKextRef aKext)
                ;

/*!
 * @function OSKextStop
 * @abstract Stops a kext loaded in the kernel if possible.
 *
 * @param    aKext   The kext to stop.
 * @result
 * <code>kOSReturnSuccess</code> on success, an error code on failure.
 *
 * @discussion
 * This function allows an application to stop a kext that is loaded
 * in the kernel without unloading it. This may be useful in certain
 * debugging scenarios.
 *
 * A kext cannot be stopped via this function if it has any dependents
 * that reference it, any client references within the kernel,
 * or any instances of libkern/IOKit C++ classes.
 *
 * To stop a kext means to call its stop function, which is distinct from
 * the stop method of an IOService object.
 * 
 * The calling process must have an effective user ID of 0 (root)
 * to stop kexts in the kernel.
 */
// xxx - should it be allowed to stop a kext with dependents that are
// xxx - themselves not started?
// xxx - need to list errors that may be returned
CF_EXPORT OSReturn
OSKextStop(OSKextRef aKext)
                ;

/*!
 * @function OSKextSendPersonalitiesToKernel
 * @abstract Sends an array of IOKit personalities to the kernel.
 *
 * @param    personalities   The personalities to send.
 * @result
 * <code>kOSReturnSuccess</code> on success, an error code on failure.
 *
 * @discussion
 * This function simply sends an anonymous array of personalities to the
 * I/O Kit's IOCatalogue object in the kernel.
 * You can get personalities from kexts using
 * @link OSKextCopyPersonalitiesArray@/link or
 * @link OSKextCopyPersonalitiesOfKexts@/link.
 * 
 * The calling process must have an effective user ID of 0 (root)
 * to send personalities to the kernel.
 */
// xxx - need to list errors that may be returned
CF_EXPORT OSReturn
OSKextSendPersonalitiesToKernel(CFArrayRef personalities)
                ;

/*!
 * @function OSKextSendKextPersonalitiesToKernel
 * @abstract
 * Sends the personalities of a single kext to the kernel, optionally
 * filtered by name.
 *
 * @param    aKext              The kext whose personalities to send.
 * @param    personalityNames
 *           An array of names. Only personalities from <code>aKext</code>
 *           with these names are sent to the kernel.
 * @result
 * <code>kOSReturnSuccess</code> on success, an error code on failure.
 *
 * @discussion
 * If any names in <code>personalityNames</code> are not found,
 * they are simply skipped rather than causing an error.
 *
 * This function may be useful in certain debugging scenarios,
 * where a particular personality is causing problems.
 * 
 * The calling process must have an effective user ID of 0 (root)
 * to send personalities to the kernel.
 */
// xxx - should names not found cause an error?
// xxx - need to list errors that may be returned
CF_EXPORT OSReturn
OSKextSendKextPersonalitiesToKernel(
    OSKextRef  aKext,
    CFArrayRef personalityNames)
                ;

/*!
 * @function OSKextSendPersonalitiesOfKextsToKernel
 * Sends the personalities of multiple kext to the kernel in a single
 * operation.
 *
 * @param    kextArray     The kexts whose personalities to send.
 * @result
 * <code>kOSReturnSuccess</code> on success, an error code on failure.
 *
 * @discussion
 * This function performs one data transfer to the kernel, collecting
 * all the personalities of the kexts in <code>kextArray</code>.
 * 
 * The calling process must have an effective user ID of 0 (root)
 * to send personalities to the kernel.
 */
// xxx - need to list errors that may be returned
CF_EXPORT OSReturn
OSKextSendPersonalitiesOfKextsToKernel(CFArrayRef kextArray)
                ;

/*!
 * @function OSKextRemoveKextPersonalitiesFromKernel
 * Removes all personalities for a kext's bundle identifier from the kernel.
 *
 * @param    aKext     The kext whose personalities to remove.
 * @result
 * <code>kOSReturnSuccess</code> on success, an error code on failure.
 *
 * @discussion
 * This function removes from the kernel's IOCatalogue all personalities
 * whose CFBundleIdentifiers match that of <code>aKext</code> so that no
 * further load requests for <code>aKext</code> will be made. This is
 * typically done when it is discovered that <code>aKext</code> cannot
 * be loaded from user space (if it fails to load in the kernel, matching
 * personalities are automatically removed).
 *
 * Note that kexts other than <code>aKext</code> might have published
 * personalities in the IOCatalogue under <code>aKext</code>'s identifier.
 * Such personalities will also be removed, since they trigger load requests
 * for a kext that cannot be loaded. The OSKext library adds an
 * IOPersonalityPublisher property to such personalities, that gives the
 * bundle identifier of the originating kext.
 *
 * This function differs from
 * @link OSKextRemovePersonalitiesForIdentifierFromKernel@/link
 * by having a kext object to check for logging or other purposes.
 * 
 * The calling process must have an effective user ID of 0 (root)
 * to remove personalities from the kernel.
 */
// xxx - need to list errors that may be returned
// xxx - this is a not-so-thin shim over IOCatalogueSendData (kIOCatalogueRemoveDrivers)
// xxx - might we just want the function to take a bundle ID?
// xxx - does IOCatalogueSendData really require root access?
CF_EXPORT OSReturn
OSKextRemoveKextPersonalitiesFromKernel(OSKextRef aKext)
                ;

/*!
 * @function OSKextRemovePersonalitiesForIdentifierFromKernel
 * Removes all personalities for a given bundle identifier from the kernel.
 *
 * @param    aBundleID     The bundle identifier for which to remove personalities.
 * @result
 * <code>kOSReturnSuccess</code> on success, an error code on failure.
 *
 * @discussion
 * This function removes from the kernel's IOCatalogue all personalities
 * whose CFBundleIdentifiers are <code>aBundleID</code> so that no
 * further load requests for the kext with that identifier will be made.
 * This is typically done when it is discovered no kext can be found for
 * <code>aBundleID</code>.
 *
 * This function differs from
 * @link OSKextRemoveKextPersonalitiesFromKernel@/link
 * by not having a kext object to check for logging or other purposes.
 * 
 * The calling process must have an effective user ID of 0 (root)
 * to remove personalities from the kernel.
 */
// xxx - need to list errors that may be returned
// xxx - this is a not-so-thin shim over IOCatalogueSendData (kIOCatalogueRemoveDrivers)
// xxx - I want 2 separate functions for logging; a kext can have flags in it
// xxx - does IOCatalogueSendData really require root access?
CF_EXPORT OSReturn
OSKextRemovePersonalitiesForIdentifierFromKernel(CFStringRef aBundleID)
                ;

/*!
 * @function OSKextCreateLoadedKextInfo
 * @abstract Returns information about loaded kexts in a dictionary.
 *
 * @param    kextIdentifiers   An array of kext identifiers to read from the kernel.
 *                             Pass <code>NULL</code> to read info for all loaded kexts.
 * @result
 * An array of dictionaries containing information about loaded kexts.
 *
 * @discussion
 * This function gets information from the kernel without affecting any
 * kext objects. If you want to update open kext objects to reflect
 * whether they are loaded in the kernel, use @link OSKextReadLoadedKextInfo@/link.
 */
// xxx - need to document the keys from OSKextLib.h.
CF_EXPORT CFArrayRef
OSKextCreateLoadedKextInfo(
    CFArrayRef kextIdentifiers)
                ;

/*!
 * @function OSKextReadLoadedKextInfo
 * @abstract Updates kext objects with load info from the kernel.
 *
 * @param kextIdentifiers   An array of kext identifiers to read
 *                          from the kernel;
 *                          all matching kexts have their load info updated.
 *                          Pass <code>NULL</code> to update load info for all kexts.
 * @param flushDependenciesFlag
 *        <code>true</code> to clear dependency information from kexts,
 *        <code>false</code> to leave it in place.
 *
 * @result
 * <code>kOSReturnSuccess</code> if all information is successfully updated,
 * an error code otherwise. Specifically, if the current architecture set
 * by @link OSKextSetArchitecture@/link is not that of the running kernel,
 * this function returns <code>kOSKextReturnExecutableArchNotFound</code>.
 *
 * @discussion
 * The load status of kexts primarily affects dependency resolution in the
 * kext library, in that kexts marked as loaded are given priority over
 * all other kexts of the same identifier.
 * See @link OSKextResolveDependencies@/link for more.
 *
 * This function calls @link OSKextFlushLoadInfo@/link on the kexts,
 * which clears any previous load info for them
 * (or for all kexts if <code>kextIdentifiers</code> is <code>NULL</code>).
 * If <code>flushDependenciesFlag</code> is <code>true</code>,
 * resolved dependencies (which may not match the loaded kexts
 * in the kernel) are also flushed.
 * Load addresses are then set from the kernel,
 * but dependencies are resolved as needed.
 *
 * If <code>flushDependenciesFlag</code> is <code>false</code>,
 * existing dependency graphs are maintained, allowing you to
 * check whether the dependencies, as resolved before reading
 * load information, reflect loaded kexts (by getting the load list
 * via @link OSKextCopyLoadList@/link and checking the kexts in it
 * with @link OSKextIsLoaded@/link).
 */
// xxx - need to list errors that may be returned
CF_EXPORT OSReturn
OSKextReadLoadedKextInfo(
    CFArrayRef kexts,
    Boolean    flushDependenciesFlag)
                ;

/*!
 * @function OSKextIsLoaded
 * @abstract Returns whether a kext is loaded in the kernel.
 *
 * @param    aKext   The kext to examine.
 * @result
 * <code>true</code> if a kext with the same identifier, version, and UUID
 * as <code>aKext</code> is loaded in the kernel, <code>false</code> otherwise.
 *
 * @discussion
 * You must call @link OSKextReadLoadedKextInfo@/link for this flag to be meaningful,
 * which in turn requires the current library architecture to match that
 * of the running kernel.
 * Use @link OSKextOtherVersionIsLoaded@/link to check whether
 * a different version of <code>aKext</code> is loaded.
 */
CF_EXPORT Boolean
OSKextIsLoaded(OSKextRef aKext)
                ;

/*!
 * @function OSKextGetLoadAddress
 * @abstract Returns the actual or simulated kernel load address of a kext.
 *
 * @param    aKext   The kext to examine.
 * @result
 * The load address of <code>aKext</code>, whether read from the kernel using
 * @link OSKextReadLoadedKextInfo@/link or set for symbol generation using
 * @link OSKextSetLoadAddress@/link.
 *
 * @discussion
 * Load addresses are accessed as 64-bit numbers even for 32-bit architectures;
 * cast or truncate the value as necessary.
 *
 * You must call @link OSKextReadLoadedKextInfo@/link or
 * @link OSKextSetLoadAddress@/link for the load address to be nonzero.
 */
CF_EXPORT uint64_t
OSKextGetLoadAddress(OSKextRef aKext)
                ;

/*!
 * @function OSKextSetLoadAddress
 * @abstract
 * Sets the simulated kernel load address of a kext for symbol generation.
 *
 * @param    aKext   The kext to set a load address for.
 * @result
 * <code>true</code> if the address was set, <code>false</code> if not
 * (because it's too large for the current architecture).
 *
 * @discussion
 * Load addresses are accessed as 64-bit numbers even for 32-bit architectures.
 * If you attempt to set a load address that is too large for a 32-bit link,
 * this function returns <code>false</code>.
 * See @link OSKextSetArchitecture@/link.
 *
 * Setting a load address manually is useful for generating debug symbols
 * with @link OSKextGenerateDebugSymbols@/link.
 */
CF_EXPORT Boolean
OSKextSetLoadAddress(OSKextRef aKext, uint64_t anAddress)
                ;

/*!
 * @function OSKextGetLoadTag
 * @abstract
 * Returns the load tag of a kext if it's loaded in the kernel.
 *
 * @param    aKext   The kext to examine.
 * @result
 * The load tag (also known as the index or kmod id) of <code>aKext</code>,
 * if it's loaded in the kernel (use @link OSKextIsLoaded@/link to check that).
 * Returns 0 if <code>aKext</code> is not loaded or if it can't be determined
 * whether it is loaded.
 *
 * @discussion
 * You must call @link OSKextReadLoadedKextInfo@/link for the load tag
 * to be meaningful, which in turn requires the current library architecture
 * to match that of the running kernel.
 * Use @link OSKextIsLoaded@/link to check whether <code>aKext</code>
 * is loaded.
 */
CF_EXPORT uint32_t
OSKextGetLoadTag(OSKextRef aKext)
                ;

/*!
 * @function OSKextFlushLoadInfo
 * @abstract Clears all load and dependency information from a kext.
 *
 * @param aKext
 *        The kext to clear. If <code>NULL</code>, all open
 *        kexts have their load info flushed.
 * @param flushDependenciesFlag
 *        <code>true</code> to clear dependency information from kexts,
 *        <code>false</code> to leave it in place.
 *
 * @discussion
 * Flushing load information releases the internal load information
 * structs of a kext, including load flags, the bundle executable,
 * link/load diagnostics, and if <code>flushDependenciesFlag</code> is true,
 * the dependencies.
 *
 * OSKexts accumulate a fair amount of information as they are created
 * and used; when many kext objects are created the total memory
 * consumption can be significant. If an application won't be using
 * kexts for long periods of time, it can flush this information
 * to save memory; the information will later be re-read from disk
 * or recreated as necessary.
 *
 * Flushing load info is also useful after readling loaded kext
 * information from the kernel with @link OSKextReadLoadedKextInfo@/link
 * or after working with executables. Executables in particular
 * consume a lot of application memory, often unnecessarily, since
 * the code is loaded and running in the kernel.
 *
 * See also @link OSKextFlushInfoDictionaries@/link,
 * @link OSKextFlushDependencies@/link,
 * and @link OSKextFlushDiagnostics@/link.
 */
CF_EXPORT void
OSKextFlushLoadInfo(
    OSKextRef aKext,
    Boolean flushDependenciesFlag)
                ;

/*!
 * @function OSKextCopyAllRequestedIdentifiers
 * @abstract
 * Copies the list of all unique bundle identifiers of kext load requests
 * that the kernel has received since power-on.
 *
 * @result
 * A CFArray object, or <code>NULL</code> if the copy failed.
 * Ownership follows the 
 * @link //apple_ref/doc/uid/20001148-103029 Create Rule@/link.
 *
 */
CF_EXPORT CFArrayRef 
OSKextCopyAllRequestedIdentifiers(void)
                ;

#pragma mark Sanity Checking and Diagnostics
/*********************************************************************
* Sanity Checking and Diagnostics
*********************************************************************/

/*!
 * @function OSKextParseVersionCFString
 * @abstract
 * Parses a kext version from a CFString.
 *
 * @param    versionString  The kext version string to parse.
 * @result
 * The numeric rendering of <code>versionString</code>, which can
 * compared arithmetically with other valid version numbers.
 * Returns -1 if the version couldn't be parsed.
 */
CF_EXPORT OSKextVersion
OSKextParseVersionCFString(CFStringRef versionString)
                ;

/*! @function OSKextParseVersionString */
CF_EXPORT OSKextVersion OSKextParseVersionString(const char* versionString);
CF_EXPORT void OSKextVersionGetString(OSKextVersion version, char* buffer, size_t bufferLength);


/*!
 * @function OSKextValidate
 * @abstract
 * Sanity checks a kext's info dictionary and executable for the currently
 * set architecture.
 *
 * @param    aKext  The kext to validate.
 * @result
 * <code>true</code> if <code>aKext</code> passes all validation tests,
 * <code>false</code> otherwise.
 *
 * @discussion
 * This function forces full validation of a kext, collecting all
 * errors found in the validation diagnostics if recording is turned on
 * (with @link OSKextSetRecordsDiagnostics@/link). You can get the diagnostics
 * using @link OSKextCopyDiagnostics@/link.
 *
 * If safe boot is currently simulated (see
 * @link OSKextSetSimulatedSafeBoot@/link),
 * any kext not loadable during safe boot will fail validation.
 */
// compare with CFBundlePreflightExecutable()
CF_EXPORT Boolean
OSKextValidate(OSKextRef aKext)
                ;

/*!
 * @function OSKextIsValid
 * @abstract
 * Returns whether a kext is valid or not.
 *
 * @param    aKext  The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> passes all validation tests,
 * <code>false</code> otherwise.
 *
 * @discussion
 * This function avoids doing full validation if any problems have been
 * discovered with the kext during other operations. To perform a full
 * validation of all possible problems, use @link OSKextValidate@/link.
 */
CF_EXPORT Boolean
OSKextIsValid(OSKextRef aKext)
                ;

/*!
 * @function OSKextValidateDependencies
 * @abstract Validates all dependencies of a kext.
 *
 * @param    aKext  The kexts whose dependencies to validate.
 * @result
 * <code>true</code> if all dependencies of <code>aKext</code> are valid,
 * <code>false</code> if any of them are not.
 *
 * @discussion
 * This function calls @link OSKextValidate@/link on <code>aKext</code> and
 * all the current dependencies of <code>aKext</code>.
 * Use @link OSKextCopyDiagnostics@/link
 * with a flag of @link kOSKextDiagnosticsFlagDependencies@/link
 * to get the dependency-resolution failures caused by invalid dependencies.
 *
 * If safe boot is currently simulated (see
 * @link OSKextSetSimulatedSafeBoot@/link),
 * any kext not loadable during safe boot will fail validation.
 *
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
CF_EXPORT Boolean
OSKextValidateDependencies(OSKextRef aKext)
                ;

/*!
 * @function OSKextAuthenticate
 * @abstract Authenticates file ownership and permissions of a kext.
 *
 * @param    aKext   The kext to examine.
 * @result
 * <code>true</code> if all directories and files in <code>aKext</code>'s
 * bundle or mkext source have secure ownership and permissions,
 * <code>false</code> if they do not, or if authentication could not be done.
 *
 * @discussion
 * This function forces full authentication of a kext, collecting all
 * errors found in the authentication diagnostics, collecting all
 * errors found in the validation diagnostics if recording is turned on
 * (with @link OSKextSetRecordsDiagnostics@/link). You can get the diagnostics
 * using @link OSKextCopyDiagnostics@/link.
 *
 * For a kext to be loadable into the kernel, it must be owned by
 * user root, group wheel, and its constituent files and directories
 * must be nonwritable by group and other.
 * A kext created from an mkext file uses that mkext file for authentication
 * (see @link OSKextCreateKextsFromMkextFile@/link.
 */
CF_EXPORT Boolean
OSKextAuthenticate(OSKextRef aKext)
                ;

/*!
 * @function OSKextIsAuthentic
 * @abstract
 * Returns whether a kext is authentic or not.
 *
 * @param    aKext  The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> passes all authentication tests,
 * <code>false</code> otherwise.
 *
 * @discussion
 * This function avoids doing full authentication if any problems have been
 * discovered with the kext during other operations. To perform a full
 * validation of all possible problems, use @link OSKextValidate@/link.
 */
CF_EXPORT Boolean
OSKextIsAuthentic(OSKextRef aKext)
                ;

/*!
 * @function OSKextAuthenticateDependencies
 * @abstract Authenticates all dependencies of a kext.
 *
 * @param    aKext  The kexts whose dependencies to authenticate.
 * @result
 * <code>true</code> if all dependencies of <code>aKext</code> are authentic,
 * <code>false</code> if any of them are not.
 *
 * @discussion
 * This function calls @link OSKextAuthenticate@/link on <code>aKext</code>
 * and all the current dependencies of <code>aKext</code>.
 * Use @link OSKextCopyDiagnostics@/link
 * with a flag of @link kOSKextDiagnosticsFlagDependencies@/link
 * to get the dependency-resolution failures caused by inauthentic dependencies.
 *
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
CF_EXPORT Boolean
OSKextAuthenticateDependencies(OSKextRef aKext)
                ;

/*!
 * @function OSKextIsLoadable
 * @abstract
 * Returns whether a kext appears loadable for the current architecture.
 *
 * @param    aKext   The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> has no problems with validation,
 * authentication, or dependency resolution for the current architecture,
 * and if it's eligible for the current safe boot mode.
 * Returns <code>false</code> otherwise.
 *
 * @discussion
 * This function resolves dependencies for <code>aKext</code>,
 * validates and authenticsates <code>aKext</code> and its dependencies,
 * and if safe boot is simulated also checks the OSBundleRequired property
 * of <code>aKext</code> and its dependencies. If all these tests pass,
 * the kext is considered loadable for the current architecture
 * (which need not match that of the running kernel).
 *
 * See @link OSKextSetArchitecture@/link and
 * @link OSKextSetSimulatedSafeBoot@/link.
 *
 * This function calls @link OSKextResolveDependencies@/link to find
 * dependencies.
 */
CF_EXPORT Boolean
OSKextIsLoadable(OSKextRef aKext)
                ;

/*!
 * @function OSKextCopyDiagnostics
 * @abstract Returns diagnostics information for a kext.
 *
 * @param    aKext      The kext to get diagnostics for.
 * @param    typeFlags  Flags indicating which diagnostics to retrieve.
 * @result
 * A dictionary containing diagnostics for <cod>aKext</code>, suitable
 * for display in an outline view or printout.
 *
 * @discussion
 * You can use this function after validating, authenticating, resolving
 * dependencies, or generating debug symbols to get all the problems encountered
 * with those operations.
 *
 * The exact keys and values used for diagnostics are for informational purposes
 * only, are not formally defined, and may change without warning.
 *
 * You can use @link OSKextLogDiagnostics@/link to print nicely-formatted
 * reports of any problems found with kexts.
 */
// xxx - need to tie in with the kernel & linker to get link failures
CFDictionaryRef OSKextCopyDiagnostics(OSKextRef aKext,
        OSKextDiagnosticsFlags typeFlags)
;

/*!
 * @function OSKextLogDiagnostics
 * @abstract Logs specified diagnostics for a kext.
 *
 * @param    aKext      The kext to log diagnostics for.
 * @param    typeFlags  Flags indicating which diagnostics to log.
 *
 * @discussion
 * You can use this function after validating, authenticating, resolving
 * dependencies, or generating debug symbols to display all the problems
 * encountered with those operations.
 */
CF_EXPORT void
OSKextLogDiagnostics(
    OSKextRef              aKext,
    OSKextDiagnosticsFlags typeFlags)
                ;

/*!
 * @function OSKextFlushDiagnostics
 * @abstract Clears all diagnostics information from a kext.
 *
 * @param   aKext
 *          The kext to clear diagnostics from.
 *          Pass <code>NULL</code> to flush diagnostics from all open kexts.
 * @param   typeFlags  Flags indicating which diagnostics to flush.
 *
 * @discussion
 * OSKexts accumulate a fair amount of information as they are created
 * and used; when many kext objects are created the total memory
 * consumption can be significant. If an application won't be using
 * kexts for long periods of time, it can flush this information
 * to save memory; the information will later be re-read from disk
 * or recreated as necessary.
 *
 * See also @link OSKextFlushInfoDictionaries@/link,
 * @link OSKextFlushDependencies@/link,
 * and @link OSKextFlushLoadInfo@/link.
 */
CF_EXPORT void
OSKextFlushDiagnostics(OSKextRef aKext, OSKextDiagnosticsFlags typeFlags)
                ;

#pragma mark Mkext and Prelinked Kernel Files
/*********************************************************************
* Mkext and Prelinked Kernel Files
*********************************************************************/
// xxx - Use "Flag" or "Mask"?
typedef uint32_t OSKextRequiredFlags
;
#define kOSKextOSBundleRequiredNone              0x0
#define kOSKextOSBundleRequiredRootFlag          0x1ULL
#define kOSKextOSBundleRequiredLocalRootFlag     0x1ULL << 1
#define kOSKextOSBundleRequiredNetworkRootFlag   0x1ULL << 2
#define kOSKextOSBundleRequiredSafeBootFlag      0x1ULL << 3
#define kOSKextOSBundleRequiredConsoleFlag       0x1ULL << 4

/*!
 * @function OSKextIsFromMkext
 * @abstract Returns whether a kext was created from an mkext archive.
 *
 * @param    aKext  The kext to examine.
 * @result
 * <code>true</code> if <code>aKext</code> was created from an mkext archive,
 * <code>false</code> if it was created from an on-disk bundle.
 *
 * @discussion
 * A kext created from an mkext will have only its info dictionary, executable,
 * and any resources listed in its the OSBundleStartupResources property.
 */
CF_EXPORT Boolean
OSKextIsFromMkext(OSKextRef aKext)
                ;

/*!
 * @function OSKextMatchesRequiredFlags
 * @abstract
 * Returns whether a kext matches a given set of flags for inclusion in an
 * mkext archive.
 *
 * @param    aKext          The kext to examine.
 * @param    requiredFlags
 *           Flags indicating which values of OSBundleRequired
 *           are needed.
 * @result
 * <code>true</code> if <code>aKext</code>'s OSBundleRequired property
 * matches one of the specified flags, <code>false</code> otherwise.
 *
 * @discussion
 * This function is used to select kexts for inclusion in an mkext archive.
 * See @link OSKextFilterRequiredKexts@/link and
 * @link OSKextCreateMkext@/link.
 */
CF_EXPORT Boolean
OSKextMatchesRequiredFlags(OSKextRef aKext,
    OSKextRequiredFlags requiredFlags)
                ;

/*!
 * @function OSKextFilterRequiredKexts
 * @abstract
 * Filters an array of kexts based on a set of flags
 * for inclusion in an mkext archive.
 *
 * @param    kextArray
 *           The kexts to filter.
 *           Pass <code>NULL</code> to filter all open kexts.
 * @param    requiredFlags
 *           Flags indicating which values of OSBundleRequired
 *           are needed.
 * @result
 * An array of kexts matching <code>requiredFlags</code>.
 *
 * @discussion
 * This function is used to select kexts for inclusion in an mkext archive.
 * See @link OSKextCreateMkext@/link.
 * 
 */
CF_EXPORT CFArrayRef
OSKextFilterRequiredKexts(CFArrayRef kextArray,
    OSKextRequiredFlags requiredFlags)
                ;

/*!
 * @function OSKextCreateMkext
 * @abstract Create an mkext file from an array of kexts and set of flags.
 *
 * @param    allocator
 *           The allocator to use to allocate memory for the new object.
 *           Pass <code>NULL</code> or <code>kCFAllocatorDefault</code>
 *           to use the current default allocator.
 * @param    kextArray
 *           The kexts to include in the mkext.
 *           Pass <code>NULL</code> to use all open kexts.
 * @param    volumeRootURL
 *           If non-<code>NULL</code>, kexts with this URL as a prefix
 *           strip it when saving their paths in the mkext.
 *           This allows creation of mkexts for volumes
 *           other than the current startup volume.
 * @param    requiredFlags
 *           A set of flags that filters <code>kextArray</code> to a subset
 *           of kexts required for specific startup scenarios
 *           (typically local disk vs. network).
 * @param    compressFlag
 *           <code>true</code> to compress data in the mkext,
 *           <code>false</code> to include them at full size.
 *           COMPRESSED MKEXTS ARE NOT SUPPORTED IN THE KERNEL YET.
 * @result
 * A <code>CFData</code> object containing the mkext file data on success,
 * <code>NULL</code> on failure.
 *
 * @discussion
 * This function creates a single-architecture mkext file for the currently
 * set library architecture (see @link OSKextSetArchitecture@/link). Kexts
 * with executables lacking code for that architecture are not included.
 *
 * If you want to create a multi-architecture mkext, create a set of single
 * architecture mkext files and use lipo(1) or combine them programmatically.
 *
 * This function generates mkext files in a new format that only works on
 * Mac OS X 10.6 or later.
 */
// xxx - add a version param and generate old-format mkexts?
// xxx - add flag to take only most recent version of a given bundle ID?
CF_EXPORT CFDataRef
OSKextCreateMkext(
    CFAllocatorRef      allocator,
    CFArrayRef          kextArray,
    CFURLRef            volumeRootURL,
    OSKextRequiredFlags requiredFlags,
    Boolean             compressFlag)
                ;
    
/*!
 * @function OSKextCreateKextsFromMkextFile
 * @abstract Reads all kexts from an mkext file on disk. 
 *
 * @param    alocator
 *           The allocator to use to allocate memory for the new objects.
 *           Pass <code>NULL</code> or <code>kCFAllocatorDefault</code>
 *           to use the current default allocator.
 * @param    mkextURL
 *           The mkext file URL from which to create kexts.
 * @result
 * Returns an array containing the kext objects created,
 * or <code>NULL</code> on failure.
 *
 * @discussion
 * This function creates kext objects from an mkext file rather than from
 * bundles on disk. Kexts created with this function are authenticated using
 * the mkext file at <code>mkextURL</code>.
 *
 * A kext created from an mkext has only its info dictionary, executable,
 * and any resources listed in its the OSBundleStartupResources property.
 *
 * Kexts created from an mkext are not uniqued using filesystem URLs,
 * which belong to bundles actually in the filesystem;
 * @link OSKextCreate@/link will never return a kext extracted from an mkext
 * that originally had the URL given (even though the new mkext format
 * stores that original URL).
 * This also means that if you open the same mkext file multiple times,
 * you will create distinct, identical copies of the kexts in that mkext file.
 */
CF_EXPORT CFArrayRef
OSKextCreateKextsFromMkextFile(CFAllocatorRef allocator,
    CFURLRef mkextURL)
                ;

/*!
 * @function OSKextCreateKextsFromMkextData
 * @abstract Reads all kexts from an mkext file in memory. 
 *
 * @param    alocator
 *           The allocator to use to allocate memory for the new objects.
 *           Pass <code>NULL</code> or <code>kCFAllocatorDefault</code>
 *           to use the current default allocator.
 * @param    mkextData
 *           The mkext file data from which to create kexts.
 * @result
 * Returns an array containing the kext objects created,
 * or <code>NULL</code> on failure.
 *
 * @discussion
 * This function creates kext objects from an mkext file in memory
 * rather than from bundles on disk. Lacking any file in the filesystem,
 * kexts created with this function can not be authenticated.
 *
 * A kext created from an mkext has only its info dictionary, executable,
 * and any resources listed in its the OSBundleStartupResources property.
 *
 * Kexts created from an mkext are not uniqued using filesystem URLs,
 * which belong to bundles actually in the filesystem;
 * @link OSKextCreate@/link will never return a kext extracted from an mkext
 * that originally had the URL given (even though the new mkext format
 * stores that original URL).
 * This also means that if you open the same mkext file multiple times,
 * you will create distinct, identical copies of the kexts in that mkext file.
 */
CF_EXPORT CFArrayRef
OSKextCreateKextsFromMkextData(CFAllocatorRef allocator,
    CFDataRef mkextData)
                ;

/*!
 * @function OSKextCreatePrelinkedKernel
 * @abstract Creates a prelinked kernel from a kernel file and all open kexts.
 *
 * @param    kernelImage     The kernel image to use.
 * @param    kextArray
 *           The kexts to include in the prelinked kernel.
 *           Pass <code>NULL</code> to consult the running kernel
 *           for kexts to include from those open;
 *           the current architecture must match the runninng kernel's.
 * @param    volumeRootURL
 *           If non-<code>NULL</code>, kexts with this URL as a prefix
 *           strip it when saving their paths in the prelinked kernel.
 *           This allows creation of prelinked kernels from folders
 *           other than /System/Library/Extensions.
 * @param    needAllFlag
 *           If <code>true</code> and any kext fails to link,
 *           the skipAuthenticationFlag returns <code>NULL</code>.
 * @param    skipAuthenticationFlag
 *           If <code>true</code>, kexts are not authenticated for inclusion
 *           in the prelinked kernel.
 * @param    printDiagnosticsFlag
 *           If <code>true</code>, problems with the kexts that prevent
 *           inclusion of a kext in the prelinked kernel are logged via
 *           @link OSKextLogDiagnostics OSKextLogDiagnostics@/link.
 * @param    symbolsOut
 *           If non-<code>NULL</code> debug symbols for <code>kernelImage</code>
 *           and all kexts included in the result are returned by reference.
 *
 * @result
 * A <code>CFData</code> object containing the prelinked kernel image based
 * on the running kernel.
 * Returns <code>NULL</code> if <code>needAllFlag</code> is <code>true</code>
 * and any kext fails to link.
 */
CF_EXPORT CFDataRef
OSKextCreatePrelinkedKernel(
    CFDataRef         kernelImage,
    CFArrayRef        kextArray,
    CFURLRef          volumeRootURL,
    Boolean           needAllFlag,
    Boolean           skipAuthenticationFlag,
    Boolean           printDiagnosticsFlag,
    CFDictionaryRef * symbolsOut)
                ;

/*!
 * @function OSKextotherCFBundleVersionIsLoaded 
 * @abstract
 * Returns whether another version of a given kext is loaded in the kernel.
 *
 * @param    aKext   The kext to examine.
 * @result
 * <code>true</code> if a kext with the same identifier,
 * but a different version or UUID
 * from <code>aKext</code>, is loaded in the kernel.
 * Returns <code>false</code> if <code>aKext</code> is loaded or if
 * no kext with the same identifier is loaded.
 *
 * @discussion
 * You must call @link OSKextReadLoadedKextInfo@/link for this flag to be meaningful,
 * which in turn requires the current library architecture to match that
 * of the running kernel.
 * Use @link OSKextIsLoaded@/link to check whether <code>aKext</code>
 * itself is loaded. 
 */
CF_EXPORT Boolean OSKextotherCFBundleVersionIsLoaded(OSKextRef kext, Boolean* uuidFlag);

__END_DECLS

#endif /* __OSKEXT_H__ */
