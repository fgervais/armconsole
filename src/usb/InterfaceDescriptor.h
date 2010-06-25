/*
 * InterfaceDescriptor.h
 *
 *  Created on: Nov 4, 2009
 *      Author: fgervais
 */

#ifndef INTERFACEDESCRIPTOR_H_
#define INTERFACEDESCRIPTOR_H_

#include "Descriptor.h"

#include <stdint.h>

class EndpointDescriptor;

class InterfaceDescriptor: public Descriptor {
public:
	static const uint16_t TYPE = 0x04;
	static const uint16_t LENGTH = 0x09;

	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndPoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;

	EndpointDescriptor** endpointDescriptor;

	InterfaceDescriptor(uint8_t* rawDescriptor);
	virtual ~InterfaceDescriptor();

	EndpointDescriptor* getEndpointDescriptor(uint32_t number) { return endpointDescriptor[number]; }
};

#endif /* INTERFACEDESCRIPTOR_H_ */
