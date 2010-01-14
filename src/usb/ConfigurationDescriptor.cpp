/*
 * ConfigurationDescriptor.cpp
 *
 *  Created on: Nov 4, 2009
 *      Author: fgervais
 */

#include "ConfigurationDescriptor.h"

ConfigurationDescriptor::ConfigurationDescriptor(uint8_t* rawDescriptor) {
	bLength				= rawDescriptor[0];
	bDescriptorType		= rawDescriptor[1];
	wTotalLength		= (rawDescriptor[3] << 8) | rawDescriptor[2];
	bNumInterface		= rawDescriptor[4];
	bConfigurationValue	= rawDescriptor[5];
	iConfiguration		= rawDescriptor[6];
	bmAttributes		= rawDescriptor[7];
	bMaxPower			= rawDescriptor[8];
}

ConfigurationDescriptor::~ConfigurationDescriptor() {

}
