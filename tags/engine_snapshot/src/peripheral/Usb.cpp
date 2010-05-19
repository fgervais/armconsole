/*
 * Usb.cpp
 *
 *  Created on: Jan 12, 2010
 *      Author: fgervais
 */

#include "Usb.h"
#include "UsbDevice.h"

Usb::Usb(uint8_t maxDeviceNumber) {
	this->maxDeviceNumber = maxDeviceNumber;

	// Allocate memory for the array of USB devices.
	device = new UsbDevice*[maxDeviceNumber];
}

Usb::~Usb() {
	/* Free memory */
	for(uint8_t i=0; i<maxDeviceNumber; i++) {
		delete[] device[i];
	}
	delete[] device;
}
