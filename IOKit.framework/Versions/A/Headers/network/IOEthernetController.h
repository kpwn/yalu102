/*
 * Copyright (c) 1998-2008 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOETHERNETCONTROLLER_H
#define _IOETHERNETCONTROLLER_H

#include <IOKit/network/IONetworkController.h>

/*! @defined kIOEthernetControllerClass
    @abstract kIOEthernetControllerClass is the name of the
        IOEthernetController class. */

#define kIOEthernetControllerClass        "IOEthernetController"

/*! @defined kIOEthernetAddressSize
    @abstract The number of bytes in an Ethernet hardware address. */

#define kIOEthernetAddressSize            6

/*! @defined kIOEthernetMaxPacketSize
    @abstract The maximum size of an Ethernet packet, including
        the FCS bytes. */

#define kIOEthernetMaxPacketSize          1518

/*! @defined kIOEthernetMinPacketSize
    @abstract The minimum size of an Ethernet packet, including
        the FCS bytes. */

#define kIOEthernetMinPacketSize          64

/*! @defined kIOEthernetCRCSize
    @abstract The size in bytes of the 32-bit CRC value appended
        to the end of each Ethernet frame. */

#define kIOEthernetCRCSize                4

/*! @defined kIOEthernetWakeOnLANFilterGroup
    @abstract kIOEthernetWakeOnLANFilterGroup describes the name assigned
        to the Ethernet Wake-On-LAN filter group. This group represents
        wake filters that are supported by the controller. */

#define kIOEthernetWakeOnLANFilterGroup   "IOEthernetWakeOnLANFilterGroup"

/*! @defined kIOEthernetDisabledWakeOnLANFilterGroup
    @abstract kIOEthernetDisabledWakeOnLANFilterGroup describes the name
        assigned to the disabled Ethernet Wake-On-LAN filter group. This
        group represents wake filters that are currently disabled.
        Membership in this group is dynamic. */

#define kIOEthernetDisabledWakeOnLANFilterGroup \
        "IOEthernetDisabledWakeOnLANFilterGroup"

/*! @enum WakeOnLANFilters.
    @abstract All filters in the Wake-on-LAN filter group.
    @discussion Each filter listed will respond to a network event that
        will trigger a system wake-up.
    @constant kIOEthernetWakeOnMagicPacket Reception of a Magic Packet.
    @constant kIOEthernetWakeOnPacketAddressMatch Reception of a packet
    which passes through any of the address filtering mechanisms based
    on its destination Ethernet address. This may include unicast,
    broadcast, or multicast addresses depending on the current state
    and setting of the corresponding packet filters. */

enum {
    kIOEthernetWakeOnMagicPacket         = 0x00000001,
    kIOEthernetWakeOnPacketAddressMatch  = 0x00000002
};

/*
 * Kernel
 */
#if defined(KERNEL) && defined(__cplusplus)

struct IOEthernetAddress {
	UInt8 bytes[kIOEthernetAddressSize];
};

/*! @const gIOEthernetWakeOnLANFilterGroup
    @discussion gIOEthernetWakeOnLANFilterGroup is an OSSymbol object
    that contains the name of the Ethernet Wake-on-LAN filter group
    defined by kIOEthernetWakeOnLANFilterGroup. */

extern const OSSymbol * gIOEthernetWakeOnLANFilterGroup;

/*! @const gIOEthernetDisabledWakeOnLANFilterGroup
    @discussion gIOEthernetDisabledWakeOnLANFilterGroup is an OSSymbol object
    that contains the name of the disabled Ethernet Wake-on-LAN filter group
    defined by kIOEthernetDisabledWakeOnLANFilterGroup. */

extern const OSSymbol * gIOEthernetDisabledWakeOnLANFilterGroup;

/*! @class IOEthernetController
    @abstract Abstract superclass for Ethernet controllers. 
    @discussion Ethernet controller drivers should subclass IOEthernetController, and implement
    or override the hardware specific methods to create an Ethernet driver.
    An interface object (an IOEthernetInterface instance) must be
    instantiated by the driver, through attachInterface(), to connect
    the controller driver to the data link layer. 
*/

class IOEthernetController : public IONetworkController
{
    OSDeclareAbstractStructors( IOEthernetController )

protected:
    struct ExpansionData { };
    /*! @var reserved
        Reserved for future use.  (Internal use only)  */
    ExpansionData *  _reserved;


public:

/*! @function initialize
    @abstract IOEthernetController class initializer.
    @discussion Creates global OSSymbol objects that are used as keys. */

    static void initialize();

/*! @function init
    @abstract Initializes an IOEthernetController object.
    @param properties A dictionary object containing a property table
        associated with this instance.
    @result Returns true on success, false otherwise. 
*/ 

    virtual bool init(OSDictionary * properties);

/*! @function getPacketFilters
    @abstract Gets the set of packet filters supported by the Ethernet 
    controller in the given filter group.
    @discussion The default implementation of the abstract method inherited
    from IONetworkController. When the filter group specified is
    gIONetworkFilterGroup, then this method will return a value formed by
    a bitwise OR of kIOPacketFilterUnicast, kIOPacketFilterBroadcast,
    kIOPacketFilterMulticast, kIOPacketFilterPromiscuous. Otherwise, the 
    return value will be set to zero (0). Subclasses must override this
    method if their filtering capability differs from what is reported by
    this default implementation. This method is called from the workloop
    context, and the result is published to the I/O Kit Registry.
    @param group The name of the filter group.
    @param filters Pointer to the mask of supported filters returned by
    	this method.
    @result Returns kIOReturnSuccess. Drivers that override this
    method must return kIOReturnSuccess to indicate success, or an error 
    return code otherwise. 
*/

    virtual IOReturn getPacketFilters(const OSSymbol * group,
                                      UInt32 *         filters) const;

/*! @function enablePacketFilter
    @abstract Enables one of the supported packet filters from the
    given filter group.
    @discussion The default implementation of the abstract method inherited
    from IONetworkController. This method will call setMulticastMode() or
    setPromiscuousMode() when the multicast or the promiscuous filter is to be
    enabled. Requests to disable the Unicast or Broadcast filters are handled
    silently, without informing the subclass. Subclasses can override this
    method to change this default behavior, or to extend it to handle
    additional filter types or filter groups. This method call is synchronized
    by the workloop's gate.
    @param group The name of the filter group containing the filter to be
    enabled.
    @param aFilter The filter to enable.
    @param enabledFilters All filters currently enabled by the client.
    @param options Optional flags for the enable request.
    @result Returns the value returned by setMulticastMode() or setPromiscuousMode() if
    either of those two methods are called. Returns kIOReturnSuccess if the filter
    specified is kIOPacketFilterUnicast or kIOPacketFilterBroadcast.
    Returns kIOReturnUnsupported if the filter group specified is not
    gIONetworkFilterGroup. 
*/

    virtual IOReturn enablePacketFilter(const OSSymbol * group,
                                        UInt32           aFilter,
                                        UInt32           enabledFilters,
                                        IOOptionBits     options = 0);

/*! @function disablePacketFilter
    @abstract Disables a packet filter that is currently enabled from the
    given filter group.
    @discussion The default implementation of the abstract method inherited
    from IONetworkController. This method will call setMulticastMode() or
    setPromiscuousMode() when the multicast or the promiscuous filter is to be
    disabled. Requests to disable the Unicast or Broadcast filters are handled
    silently, without informing the subclass. Subclasses can override this
    method to change this default behavior, or to extend it to handle
    additional filter types or filter groups. This method call is synchronized
    by the workloop's gate.
    @param group The name of the filter group containing the filter to be
    disabled.
    @param aFilter The filter to disable.
    @param enabledFilters All filters currently enabled by the client.
    @param options Optional flags for the disable request.
    @result Returns the value returned by setMulticastMode() or setPromiscuousMode() if
    either of those two methods are called. Returns kIOReturnSuccess if the filter
    specified is kIOPacketFilterUnicast or kIOPacketFilterBroadcast.
    Returns kIOReturnUnsupported if the filter group specified is not
    gIONetworkFilterGroup. 
*/

    virtual IOReturn disablePacketFilter(const OSSymbol * group,
                                         UInt32           aFilter,
                                         UInt32           enabledFilters,
                                         IOOptionBits     options = 0);

/*! @function getHardwareAddress
    @abstract Gets the Ethernet controller's station address.
    @discussion The default implementation of the abstract method inherited
    from IONetworkController. This method will call the overloaded form
    IOEthernetController::getHardwareAddress() that subclasses are expected
    to override.
    @param addr The buffer where the controller's hardware address should
           be written.
    @param inOutAddrBytes The size of the address buffer provided by the
           client, and replaced by this method with the actual size of
           the hardware address in bytes.
    @result Returns kIOReturnSuccess on success, or an error otherwise. 
*/

    virtual IOReturn getHardwareAddress(void *   addr,
                                        UInt32 * inOutAddrBytes);

/*! @function setHardwareAddress
    @abstract Sets or changes the station address used by the Ethernet
    controller.
    @discussion The default implementation of the abstract method inherited
    from IONetworkController. This method will call the overloaded form
    IOEthernetController::setHardwareAddress() that subclasses are expected
    to override.
    @param addr The buffer containing the hardware address provided by
    the client.
    @param addrBytes The size of the address buffer provided by the
    client in bytes.
    @result Returns kIOReturnSuccess on success, or an error otherwise. 
*/

    virtual IOReturn setHardwareAddress(const void * addr,
                                        UInt32       addrBytes);

/*! @function getMaxPacketSize
    @abstract Gets the maximum packet size supported by the Ethernet
        controller, including the frame header and FCS.
    @param maxSize Pointer to the return value.
    @result Returns kIOReturnSuccess on success, or an error code otherwise. 
*/

    virtual IOReturn getMaxPacketSize(UInt32 * maxSize) const;

/*! @function getMinPacketSize
    @abstract Gets the minimum packet size supported by the Ethernet
        controller, including the frame header and FCS.
    @param minSize Pointer to the return value.
    @result Returns kIOReturnSuccess on success, or an error code otherwise. 
*/

    virtual IOReturn getMinPacketSize(UInt32 * minSize) const;

/*! @function getPacketFilters
    @abstract Gets the set of packet filters supported by the Ethernet 
    controller in the network filter group.
    @param filters Pointer to the return value containing a mask of
    supported filters.
    @result Returns kIOReturnSuccess. Drivers that override this
    method must return kIOReturnSuccess to indicate success, or an error 
    return code otherwise. 
*/

    virtual IOReturn getPacketFilters(UInt32 * filters) const;

/*! @function getHardwareAddress
    @abstract Gets the Ethernet controller's permanent station address.
    @discussion Ethernet drivers must implement this method, by reading the
    address from hardware and writing it to the buffer provided. This method
    is called from the workloop context.
    @param addrP Pointer to an IOEthernetAddress where the hardware address
    should be returned.
    @result Returns kIOReturnSuccess on success, or an error return code otherwise. 
*/

    virtual IOReturn getHardwareAddress(IOEthernetAddress * addrP) = 0;

/*! @function setHardwareAddress
    @abstract Sets or changes the station address used by the Ethernet
        controller.
    @discussion This method is called in response to a client command to
    change the station address used by the Ethernet controller. Implementation
    of this method is optional. This method is called from the workloop context.
    @param addrP Pointer to an IOEthernetAddress containing the new station
    address.
    @result The default implementation will always return kIOReturnUnsupported.
    If overridden, drivers must return kIOReturnSuccess on success, or an error
    return code otherwise. 
*/

    virtual IOReturn setHardwareAddress(const IOEthernetAddress * addrP);

/*! @function setMulticastMode
    @abstract Enables or disables multicast mode.
    @discussion Called by enablePacketFilter() or disablePacketFilter()
    when there is a change in the activation state of the multicast filter
    identified by kIOPacketFilterMulticast. This method is called from the
    workloop context.
    @param active True to enable multicast mode, false to disable it.
    @result Returns kIOReturnUnsupported. If overridden, drivers must return
    kIOReturnSuccess on success, or an error return code otherwise. 
*/

    virtual IOReturn setMulticastMode(bool active);

/*! @function setMulticastList
    @abstract Sets the list of multicast addresses a multicast filter
    should use to match against the destination address of an incoming frame.
    @discussion This method sets the list of multicast addresses that the multicast filter
    should use to match against the destination address of an incoming frame.
    The frame should be accepted when a match occurs.  Called when the multicast group membership of an interface
    object is changed. Drivers that support kIOPacketFilterMulticast should
    override this method and update the hardware multicast filter using the
    list of Ethernet addresses provided. Perfect multicast filtering is
    preferred if supported by the hardware, in order to reduce the number of
    unwanted packets received. If the number of multicast addresses in the
    list exceeds what the hardware is capable of supporting, or if perfect
    filtering is not supported, then ideally the hardware should be programmed
    to perform imperfect filtering, through some form of hash filtering
    mechanism. Only as a last resort should the driver enable reception of
    all multicast packets to satisfy this request. This method is called
    from the workloop context, and only if the driver reports
    kIOPacketFilterMulticast support in getPacketFilters().
    @param addrs An array of Ethernet addresses. This argument must be
        ignored if the count argument is 0.
    @param count The number of Ethernet addresses in the list. This value
        will be zero when the list becomes empty.
    @result Returns kIOReturnUnsupported. Drivers must return kIOReturnSuccess to
    indicate success, or an error return code otherwise. 
*/

    virtual IOReturn setMulticastList(IOEthernetAddress * addrs, 
                                      UInt32              count);

/*! @function setPromiscuousMode
    @abstract Enables or disables promiscuous mode.
    @discussion Called by enablePacketFilter() or disablePacketFilter()
    when there is a change in the activation state of the promiscuous
    filter identified by kIOPacketFilterPromiscuous. This method is 
    called from the workloop context.
    @param active True to enable promiscuous mode, false to disable it.
    @result Returns kIOReturnUnsupported. If overridden, drivers must return
    kIOReturnSuccess on success, or an error return code otherwise. 
*/

    virtual IOReturn setPromiscuousMode(bool active);

/*! @function setWakeOnMagicPacket
    @abstract Enables or disables the wake on Magic Packet support.
    @discussion Called by enablePacketFilter() or disablePacketFilter()
    when there is a change in the activation state of the Wake-on-LAN
    filter identified by kIOEthernetWakeOnMagicPacket. This method is
    called from the workloop context.
    @param active True to enable support for system wake on reception
    of a Magic Packet, false to disable it.
    @result Returns kIOReturnUnsupported. If overridden, drivers must return
    kIOReturnSuccess on success, or an error return code otherwise. 
*/

    virtual IOReturn setWakeOnMagicPacket(bool active);

protected:

/*! @function createInterface
    @abstract Creates an IOEthernetInterface object.
    @discussion This method allocates and returns a new IOEthernetInterface instance.
    A subclass of IONetworkController must implement this method and return
    a matching interface object. The implementation in IOEthernetController
    will return an IOEthernetInterface object. Subclasses of
    IOEthernetController, such as Ethernet controller drivers, will have
    little reason to override this implementation.
    @result Returns a newly allocated and initialized IOEthernetInterface object. 
*/

    virtual IONetworkInterface * createInterface();

/*! @function free
    @abstract Frees the IOEthernetController instance. 
    @discussion This method releases resources, and is
    then followed by a call to super::free(). */

    virtual void free();

/*! @function publishProperties
    @abstract Publishes Ethernet controller properties and capabilities.
    @discussion This method publishes Ethernet controller properties to the property
    table. For instance, getHardwareAddress() is called to fetch the
    hardware address, and the address is then published to the property
    table. This method call is synchronized by the workloop's gate,
    and must never be called directly by subclasses.
    @result Returns true if all properties and capabilities were discovered,
    and published successfully, false otherwise. Returning false will
    prevent client objects from attaching to the Ethernet controller
    since a property that a client relies upon may be missing. 
*/

    virtual bool publishProperties();

	/*! @function getVlanTagDemand
		@abstract Fetch the demand for hardware vlan tag stuffing
		for the given packet before it is transmitted on the network.
		@discussion A network controller that can insert 802.1Q vlan tags for output
		packets must call this method to obtain vlan tag information that it must
		insert into the given output packet.
		@param packet A mbuf containing a packet that may require vlan tag stuffing.
		@param vlanTag After calling, the low order 16 bits contain the 802.1Q priority and
		vlan ID tag in host order.  The hi-order 16 bits are currently unused and should be ignored.
		@result true if vlanTag has been set and should be used.
		false if no vlan tag stuffing is required for this packet. */

    OSMetaClassDeclareReservedUsed( IOEthernetController,  0);
	virtual bool getVlanTagDemand(mbuf_t m, UInt32 *vlanTag);

	/*! @function setVlanTag
		@abstract Encode a received packet with the vlan tag result reported
		by the hardware.
		@discussion A network controller that can strip 802.1Q vlan tag information for a
		received packet should call this method to encode the result on the
		packet, before passing it up towards the protocol stacks.
		@param packet A mbuf containing a packet that has had its 802.1q vlan tag stripped by
		the hardware.
		@param vlanTag A value in host order that contains the 802.1q vlan tag and priority
		in the low order 16 bits.  The hi order word is currently unused and should be set to 0. */

	OSMetaClassDeclareReservedUsed( IOEthernetController,  1);
	virtual void setVlanTag(mbuf_t m, UInt32 vlanTag);
	
    // Virtual function padding
    OSMetaClassDeclareReservedUnused( IOEthernetController,  2);
    OSMetaClassDeclareReservedUnused( IOEthernetController,  3);
    OSMetaClassDeclareReservedUnused( IOEthernetController,  4);
    OSMetaClassDeclareReservedUnused( IOEthernetController,  5);
    OSMetaClassDeclareReservedUnused( IOEthernetController,  6);
    OSMetaClassDeclareReservedUnused( IOEthernetController,  7);
    OSMetaClassDeclareReservedUnused( IOEthernetController,  8);
    OSMetaClassDeclareReservedUnused( IOEthernetController,  9);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 10);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 11);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 12);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 13);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 14);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 15);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 16);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 17);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 18);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 19);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 20);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 21);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 22);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 23);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 24);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 25);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 26);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 27);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 28);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 29);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 30);
    OSMetaClassDeclareReservedUnused( IOEthernetController, 31);
};

/*
 * FIXME: remove this.
 */
enum {
    kIOEnetPromiscuousModeOff   = false,
    kIOEnetPromiscuousModeOn    = true,
    kIOEnetPromiscuousModeAll   = true,
    kIOEnetMulticastModeOff     = false,
    kIOEnetMulticastModeFilter  = true
};
typedef bool IOEnetPromiscuousMode;
typedef bool IOEnetMulticastMode;

#endif /* defined(KERNEL) && defined(__cplusplus) */

#endif /* !_IOETHERNETCONTROLLER_H */
