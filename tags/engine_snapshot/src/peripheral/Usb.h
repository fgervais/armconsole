/*
 * Usb.h
 *
 *  Created on: Jan 12, 2010
 *      Author: fgervais
 */

#ifndef USB_H_
#define USB_H_

#include "Peripheral.h"

#include <stdint.h>

class UsbDevice;

class Usb: public Peripheral {
public:
	Usb(uint8_t maxDeviceNumber);
	virtual ~Usb();

private:
	UsbDevice** device;
	uint8_t maxDeviceNumber;

	void ohciInit();
	void ohciReset();
};

#endif /* USB_H_ */
