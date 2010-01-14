/*
 * InterfaceDescriptor.cpp
 *
 *  Created on: Nov 4, 2009
 *      Author: fgervais
 */

#include "InterfaceDescriptor.h"

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
}

InterfaceDescriptor::~InterfaceDescriptor() {

}
