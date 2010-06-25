/*
 * ConfigurationDescriptor.cpp
 *
 *  Created on: Nov 4, 2009
 *      Author: fgervais
 */

#include "ConfigurationDescriptor.h"
#include "InterfaceDescriptor.h"
#include "EndpointDescriptor.h"

ConfigurationDescriptor::ConfigurationDescriptor(uint8_t* rawDescriptor) {
	bLength				= rawDescriptor[0];
	bDescriptorType		= rawDescriptor[1];
	wTotalLength		= (rawDescriptor[3] << 8) | rawDescriptor[2];
	bNumInterfaces		= rawDescriptor[4];
	bConfigurationValue	= rawDescriptor[5];
	iConfiguration		= rawDescriptor[6];
	bmAttributes		= rawDescriptor[7];
	bMaxPower			= rawDescriptor[8];

	interfaceDescriptor = new InterfaceDescriptor*[bNumInterfaces];

	uint32_t rawDescriptorOffset = ConfigurationDescriptor::LENGTH;
	for(uint8_t i=0; i<bNumInterfaces; i++) {
		if(i > 0) {
			// Previous interface length
			rawDescriptorOffset += InterfaceDescriptor::LENGTH;
			// Class or Vendor-Specific descriptor if any
			if(rawDescriptor[rawDescriptorOffset+1] != EndpointDescriptor::TYPE) {
				rawDescriptorOffset += rawDescriptor[rawDescriptorOffset];
			}
			// Previous interface endpoints length
			rawDescriptorOffset += (interfaceDescriptor[i-1]->bNumEndPoints * EndpointDescriptor::LENGTH);
		}

		interfaceDescriptor[i] = new InterfaceDescriptor(&rawDescriptor[rawDescriptorOffset]);
	}
}

ConfigurationDescriptor::~ConfigurationDescriptor() {
	for(uint8_t i=0; i<bNumInterfaces; i++) {
		delete interfaceDescriptor[i];
	}
	delete[] interfaceDescriptor;
}
