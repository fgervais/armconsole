/*
 * GetDescriptor.cpp
 *
 *  Created on: Oct 28, 2009
 *      Author: fgervais
 */

#include "GetDescriptor.h"

GetDescriptor::GetDescriptor(uint16_t descriptorTypeIndex, uint16_t descriptorLength) {
	// Set all the Base class member
	bmRequestType	= 0x80;
	bRequest		= 0x06;
	wValue		= descriptorTypeIndex;
	wIndex		= 0x0000;
	wLength		= descriptorLength;
}

GetDescriptor::~GetDescriptor() {

}
