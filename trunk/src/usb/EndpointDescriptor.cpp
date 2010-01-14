/*
 * EndpointDescriptor.cpp
 *
 *  Created on: Nov 5, 2009
 *      Author: fgervais
 */

#include "EndpointDescriptor.h"

EndpointDescriptor::EndpointDescriptor(uint8_t* rawDescriptor) {
	bLength				= rawDescriptor[0];
	bDescriptorType		= rawDescriptor[1];
	bEndpointAddress	= rawDescriptor[2];
	bmAttributes		= rawDescriptor[3];
	wMaxPacketSize		= (rawDescriptor[5] << 8) | rawDescriptor[4];
	bInterval			= rawDescriptor[6];
}

EndpointDescriptor::~EndpointDescriptor() {

}
