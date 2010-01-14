/*
 * DeviceDescriptor.cpp
 *
 *  Created on: Nov 4, 2009
 *      Author: fgervais
 */

#include "DeviceDescriptor.h"

DeviceDescriptor::DeviceDescriptor(uint8_t* rawDescriptor) {
	bLength			= rawDescriptor[0];
	bDescriptorType	= rawDescriptor[1];
	bcdUsb			= (rawDescriptor[3] << 8) | rawDescriptor[2];
	bDeviceClass	= rawDescriptor[4];
	bDeviceSubClass	= rawDescriptor[5];
	bDeviceProtocol	= rawDescriptor[6];
	bMaxPacketSize0	= rawDescriptor[7];
	idVendor		= (rawDescriptor[9] << 8) | rawDescriptor[8];
	idProduct		= (rawDescriptor[11] << 8) | rawDescriptor[10];
	bcdDevice		= (rawDescriptor[13] << 8) | rawDescriptor[12];
	iManufacturer	= rawDescriptor[14];
	iProduct		= rawDescriptor[15];
	iSerialNumber	= rawDescriptor[16];
	bNumConfigurations = rawDescriptor[17];
}

DeviceDescriptor::~DeviceDescriptor() {

}
