/*
 * InterfaceDescriptor.cpp
 *
 *  Created on: Nov 4, 2009
 *      Author: fgervais
 */

#include "InterfaceDescriptor.h"
#include "EndpointDescriptor.h"

InterfaceDescriptor::InterfaceDescriptor(uint8_t* rawDescriptor) {
	bLength				= rawDescriptor[0];
	bDescriptorType		= rawDescriptor[1];
	bInterfaceNumber	= rawDescriptor[2];
	bAlternateSetting	= rawDescriptor[3];
	bNumEndPoints		= rawDescriptor[4];
	bInterfaceClass		= rawDescriptor[5];
	bInterfaceSubClass	= rawDescriptor[6];
	bInterfaceProtocol	= rawDescriptor[7];
	iInterface			= rawDescriptor[8];

	endpointDescriptor = new EndpointDescriptor*[bNumEndPoints];

	uint32_t rawDescriptorOffset = InterfaceDescriptor::LENGTH;

	// Skip the Class or Vendor-Specific descriptor if any
	if(rawDescriptor[rawDescriptorOffset+1] != EndpointDescriptor::TYPE) {
		rawDescriptorOffset += rawDescriptor[rawDescriptorOffset];
	}

	for(uint8_t i=0; i<bNumEndPoints; i++) {
		if(i > 0) {
			rawDescriptorOffset += EndpointDescriptor::LENGTH;
		}
		endpointDescriptor[i] = new EndpointDescriptor(&rawDescriptor[rawDescriptorOffset]);
	}
}

InterfaceDescriptor::~InterfaceDescriptor() {
	for(uint8_t i=0; i<bNumEndPoints; i++) {
		delete endpointDescriptor[i];
	}
	delete[] endpointDescriptor;
}
