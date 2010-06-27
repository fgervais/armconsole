/*
 * XboxControllerDriver.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: fgervais
 */

#include "XboxControllerDriver.h"
#include "UsbDevice.h"
#include "HostControllerDriver.h"
#include "LPC2478.h"

XboxControllerDriver::XboxControllerDriver(UsbDevice* device, uint8_t controllerNumber) {
	this->device = device;
}

XboxControllerDriver::~XboxControllerDriver() {

}

void XboxControllerDriver::configure(uint8_t controllerNumber) {
	/*if(rootHubPort[hubPortNumber].device->getDeviceDescriptor()->idVendor == 0x045e) { // Xbox receiver
		if(rootHubPort[hubPortNumber].device->getDeviceDescriptor()->idProduct == 0x0719) { // Xbox receiver
			Debug::writeLine("Xbox wireless receiver connected");

			// Create the output interrupt report
			uint8_t command = 0x02;
			tdBuffer[0] = 0x00;
			tdBuffer[1] = 0x00;
			tdBuffer[2] = 0x08;
			tdBuffer[3] = 0x40 + (command % 0x0E);
			tdBuffer[4] = 0x00;
			tdBuffer[5] = 0x00;
			tdBuffer[6] = 0x00;
			tdBuffer[7] = 0x00;
			tdBuffer[8] = 0x00;
			tdBuffer[9] = 0x00;
			tdBuffer[10] = 0x00;
			tdBuffer[11] = 0x00;

			// Set the maximum packet size
			intOutEd->Control = (32 << 16);
			if(rootHubPort[hubPortNumber].lowSpeed) {
				intOutEd->Control |= (1 << 13);	// Low speed endpoint
			}
			// Device address
			intOutEd->Control |= (rootHubPort[hubPortNumber].deviceAddressesStart);
			// Endpoint number
			intOutEd->Control |= (1 << 7);

			setupPeriodicOut(tdBuffer, 12);

			while(1) {
				while(!transferCompleted);
				transferCompleted = 0;
				Debug::writeLine(".");
			}
		}
	}
	else if(rootHubPort[hubPortNumber].device->getDeviceDescriptor()->idVendor == 0x046d) { // Mouse
		if(rootHubPort[hubPortNumber].device->getDeviceDescriptor()->idProduct == 0xc018) { // Mouse
			Debug::writeLine("Logitech mouse connected");
		}
	}
	else {
		Debug::writeLine("Unknown USB device");
	}*/

	// Memory allocation in USB memory segment
	/*headTd = (HcTd *)(USB_MEMORY+0x100);
	tailTd = (HcTd *)(USB_MEMORY+0x120);
	ctrlEd = (HcEd *)(USB_MEMORY+0x140);
	intInEd = (HcEd *)(USB_MEMORY+0x150);
	intOutEd = (HcEd *)(USB_MEMORY+0x160);
	tdBuffer = (uint8_t *)(USB_MEMORY+0x170);
	userBuffer = (uint8_t *)(USB_MEMORY+0x1000);*/

	// Set the interrupt output report

	// Create the output interrupt report
	uint8_t command = 0x02;
	uint8_t* tdBuffer = (uint8_t*)(USB_MEMORY+0x2000);

	tdBuffer[0] = 0x00;
	tdBuffer[1] = 0x00;
	tdBuffer[2] = 0x08;
	tdBuffer[3] = 0x40 + (command % 0x0E);
	tdBuffer[4] = 0x00;
	tdBuffer[5] = 0x00;
	tdBuffer[6] = 0x00;
	tdBuffer[7] = 0x00;
	tdBuffer[8] = 0x00;
	tdBuffer[9] = 0x00;
	tdBuffer[10] = 0x00;
	tdBuffer[11] = 0x00;

	LPC2478::getHCD()->usbRequest(device, 0, 1, tdBuffer, 12);
}
