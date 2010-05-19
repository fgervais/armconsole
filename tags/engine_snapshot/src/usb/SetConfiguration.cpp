/*
 * SetConfiguration.cpp
 *
 *  Created on: Nov 14, 2009
 *      Author: fgervais
 */

#include "SetConfiguration.h"

SetConfiguration::SetConfiguration(uint8_t configuration) {
	// Set all the Base class member
	bmRequestType	= 0x00;
	bRequest		= 0x09;
	wValue			= configuration;
	wIndex			= 0x0000;
	wLength			= 0x0000;
}

SetConfiguration::~SetConfiguration() {

}
