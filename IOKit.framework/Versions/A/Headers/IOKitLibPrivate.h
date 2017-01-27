/*
* Copyright (c) 2008 Apple Inc. All rights reserved.
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

#include <CoreFoundation/CFMachPort.h>

#if !TARGET_OS_EMBEDDED
#include <dispatch/dispatch.h>
#endif /* !TARGET_OS_EMBEDDED */


struct IONotificationPort
{
mach_port_t		masterPort;
mach_port_t		wakePort;
CFRunLoopSourceRef	source;
#if !TARGET_OS_EMBEDDED
dispatch_queue_t	dispatchQueue;
#endif /* !TARGET_OS_EMBEDDED */
};
typedef struct IONotificationPort IONotificationPort;

CFMutableDictionaryRef
IOMakeMatching(
mach_port_t	masterPort,
unsigned int	type,
unsigned int	options,
void *		args,
unsigned int	argsSize );

void
IODispatchCalloutFromCFMessage(
CFMachPortRef port,
void *msg,
CFIndex size,
void *info );

kern_return_t
iokit_user_client_trap(
io_connect_t	connect,
unsigned int	index,
uintptr_t p1,
uintptr_t p2,
uintptr_t p3,
uintptr_t p4,
uintptr_t p5,
uintptr_t p6 );

kern_return_t
IOServiceGetState(
io_service_t    service,
uint64_t *	state );

kern_return_t
IOServiceGetBusyStateAndTime(
io_service_t    service,
uint64_t *	state,
uint32_t *	busy_state,
uint64_t *	accumulated_busy_time);

// masks for getState()
enum {
kIOServiceInactiveState	= 0x00000001,
kIOServiceRegisteredState	= 0x00000002,
kIOServiceMatchedState	= 0x00000004,
kIOServiceFirstPublishState	= 0x00000008,
kIOServiceFirstMatchState	= 0x00000010
};

#if !TARGET_OS_EMBEDDED

/*! @function IONotificationPortSetDispatchQueue
@abstract Sets a dispatch queue to be used to listen for notifications.
@discussion A notification object may deliver notifications to a dispatch client.
@param notify The notification object.
@param queue A dispatch queue. */

__OSX_AVAILABLE_STARTING(__MAC_10_6,__IPHONE_NA)
void
IONotificationPortSetDispatchQueue(
IONotificationPortRef notify, dispatch_queue_t queue );

#endif /* !TARGET_OS_EMBEDDED */
