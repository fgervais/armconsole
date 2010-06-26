/*
 * XboxControllerDriver.h
 *
 *  Created on: Jun 25, 2010
 *      Author: fgervais
 */

#ifndef XBOXCONTROLLERDRIVER_H_
#define XBOXCONTROLLERDRIVER_H_

#include <stdint.h>

class UsbDevice;

class XboxControllerDriver {
public:
	XboxControllerDriver(UsbDevice* device, uint8_t controllerNumber);
	virtual ~XboxControllerDriver();

	void configure(uint8_t controllerNumber);

private:
	UsbDevice* device;
};

#endif /* XBOXCONTROLLERDRIVER_H_ */
