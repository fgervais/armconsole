/*
 * DeviceDescriptor.h
 *
 *  Created on: Nov 4, 2009
 *      Author: fgervais
 */

#ifndef DEVICEDESCRIPTOR_H_
#define DEVICEDESCRIPTOR_H_

#include "Descriptor.h"

#include <stdint.h>

class DeviceDescriptor: public Descriptor {
public:
	uint16_t bcdUsb;
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;

	DeviceDescriptor(uint8_t* rawDescriptor);
	virtual ~DeviceDescriptor();
};

#endif /* DEVICEDESCRIPTOR_H_ */
