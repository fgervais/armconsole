/*
 * SetAddress.cpp
 *
 *  Created on: Oct 28, 2009
 *      Author: fgervais
 */

#include "SetAddress.h"

SetAddress::SetAddress(uint16_t address) {
	// Set all the Base class member
	bmRequestType	= 0x00;
	bRequest		= 0x05;
	wValue			= address;
	wIndex			= 0x0000;
	wLength			= 0x0000;
}

SetAddress::~SetAddress() {

}
