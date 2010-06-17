/*
 * UsbDevice.h
 *
 *  Created on: Jan 14, 2010
 *      Author: fgervais
 */

#ifndef USBDEVICE_H_
#define USBDEVICE_H_

#include <stdint.h>
#include "UsbDescriptors.h"

class UsbDevice {
public:
	UsbDevice(uint8_t address);
	virtual ~UsbDevice();

	DeviceDescriptor deviceDescriptor;

private:
	uint8_t address;
};

#endif /* USBDEVICE_H_ */
