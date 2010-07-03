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
#include "Debug.h"
#include "Gpio.h"
#include "GpioPin.h"
#include <cstdio>

XboxControllerDriver::XboxControllerDriver(UsbDevice* device) {
	this->device = device;
}

XboxControllerDriver::~XboxControllerDriver() {

}

void XboxControllerDriver::transferCompleted(HCDRequest* request) {;
	if(request == &statusRequest) {
		/*char debug[80];
		sprintf(debug, "%d %d %d %d %d %d", (unsigned int)request->transactionBuffer[0],
				(unsigned int)request->transactionBuffer[1],
				(unsigned int)request->transactionBuffer[2],
				(unsigned int)request->transactionBuffer[3],
				(unsigned int)request->transactionBuffer[4],
				(unsigned int)request->transactionBuffer[5]);
		Debug::writeLine(debug);*/

		if(request->transactionBuffer[1] == 0x01) {
			gamepadStatus.fill(&request->transactionBuffer[4]);
		}

		if(LPC2478::getHCD()->sendRequest(request) == 0) {
			Debug::writeLine("USB request failed");
		}
	}
}

void XboxControllerDriver::configure() {

}

void XboxControllerDriver::setLedState(LedState state, uint8_t controllerNumber) {
	uint8_t* tdBuffer = (uint8_t*)(USB_MEMORY+0x2000);

	tdBuffer[0] = 0x00;
	tdBuffer[1] = 0x00;
	tdBuffer[2] = 0x08;
	tdBuffer[3] = 0x40 + (((uint8_t)state) % 0x0E);
	tdBuffer[4] = 0x00;
	tdBuffer[5] = 0x00;
	tdBuffer[6] = 0x00;
	tdBuffer[7] = 0x00;
	tdBuffer[8] = 0x00;
	tdBuffer[9] = 0x00;
	tdBuffer[10] = 0x00;
	tdBuffer[11] = 0x00;

	ledStateRequest.device = device;
	ledStateRequest.interfaceIndex = controllerNumber*2;
	ledStateRequest.endpointIndex = 1;
	ledStateRequest.transactionBuffer = tdBuffer;
	ledStateRequest.transactionLength = 12;
	ledStateRequest.listener = this;

	LPC2478::getHCD()->sendRequest(&ledStateRequest);
}

void XboxControllerDriver::setRumbleState(uint8_t smallSpeed, uint8_t largeSpeed, uint8_t controllerNumber) {
	uint8_t* tdBuffer = (uint8_t*)(USB_MEMORY+0x2010);

	tdBuffer[0] = 0x00;
	tdBuffer[1] = 0x01;
	tdBuffer[2] = 0x0F;
	tdBuffer[3] = 0xC0;
	tdBuffer[4] = 0x00;
	tdBuffer[5] = largeSpeed;
	tdBuffer[6] = smallSpeed;
	tdBuffer[7] = 0x00;
	tdBuffer[8] = 0x00;
	tdBuffer[9] = 0x00;
	tdBuffer[10] = 0x00;
	tdBuffer[11] = 0x00;

	rumbleStateRequest.device = device;
	rumbleStateRequest.interfaceIndex = controllerNumber*2;
	rumbleStateRequest.endpointIndex = 1;
	rumbleStateRequest.transactionBuffer = tdBuffer;
	rumbleStateRequest.transactionLength = 12;
	rumbleStateRequest.listener = this;

	LPC2478::getHCD()->sendRequest(&rumbleStateRequest);
}

GamepadInputReport* XboxControllerDriver::getStatus(uint8_t controllerNumber) {
	uint8_t* tdBuffer = (uint8_t*)(USB_MEMORY+0x2020);

	statusRequest.device = device;
	statusRequest.interfaceIndex = controllerNumber*2;
	statusRequest.endpointIndex = 0;
	statusRequest.transactionBuffer = tdBuffer;
	statusRequest.transactionLength = 0x1d;
	statusRequest.listener = this;

	LPC2478::getHCD()->sendRequest(&statusRequest);

	return &gamepadStatus;
}
