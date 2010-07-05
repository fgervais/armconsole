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
#include "MemoryPool.h"
#include <cstdio>

XboxControllerDriver::XboxControllerDriver(UsbDevice* device, MemoryPool* memoryPool) {
	this->device = device;
	this->memoryPool = memoryPool;

	// Memory allocation inside the USB memory pool
	for(uint8_t i=0; i<NUMBER_OF_CONTROLLER; i++) {
		ledStateBuffer[i] = (uint8_t*)memoryPool->head +(100*i) + 0;		// 12 bytes
		rumbleStateBuffer[i] = (uint8_t*)memoryPool->head +(100*i) + 12;	// 12 bytes
		queryBuffer[i] = (uint8_t*)memoryPool->head + (100*i) + 24;			// 29 bytes
	}
}

XboxControllerDriver::~XboxControllerDriver() {
	LPC2478::getHCD()->freeMemoryPool(memoryPool);
}

void XboxControllerDriver::transferCompleted(HCDRequest* request) {;
	uint8_t controllerNumber = request->interfaceIndex >> 1;

	if(request->tag == QUERY_TAG) {
		if(request->transactionBuffer[1] == 0x01) {
			gamepadStatus[controllerNumber].fill(&request->transactionBuffer[4]);
		}
		else if(request->transactionBuffer[1] == 0x80 || request->transactionBuffer[1] == 0xC0) {
			setLedState((LedState)(XboxControllerDriver::ON_1 + controllerNumber), controllerNumber);
		}

		// Send another query request since it's periodic
		if(LPC2478::getHCD()->sendRequest(request) == 0) {
			Debug::writeLine("USB request failed");
		}
	}
}

void XboxControllerDriver::query(uint8_t controllerNumber) {
	statusRequest[controllerNumber].device = device;
	statusRequest[controllerNumber].interfaceIndex = controllerNumber<<1;
	statusRequest[controllerNumber].endpointIndex = 0;
	statusRequest[controllerNumber].transactionBuffer = queryBuffer[controllerNumber];
	statusRequest[controllerNumber].transactionLength = 0x1d;
	statusRequest[controllerNumber].listener = this;
	statusRequest[controllerNumber].tag = QUERY_TAG;

	LPC2478::getHCD()->sendRequest(&statusRequest[controllerNumber]);
}

void XboxControllerDriver::setLedState(LedState state, uint8_t controllerNumber) {
	ledStateBuffer[controllerNumber][0] = 0x00;
	ledStateBuffer[controllerNumber][1] = 0x00;
	ledStateBuffer[controllerNumber][2] = 0x08;
	ledStateBuffer[controllerNumber][3] = 0x40 + (((uint8_t)state) % 0x0E);
	ledStateBuffer[controllerNumber][4] = 0x00;
	ledStateBuffer[controllerNumber][5] = 0x00;
	ledStateBuffer[controllerNumber][6] = 0x00;
	ledStateBuffer[controllerNumber][7] = 0x00;
	ledStateBuffer[controllerNumber][8] = 0x00;
	ledStateBuffer[controllerNumber][9] = 0x00;
	ledStateBuffer[controllerNumber][10] = 0x00;
	ledStateBuffer[controllerNumber][11] = 0x00;

	ledStateRequest[controllerNumber].device = device;
	ledStateRequest[controllerNumber].interfaceIndex = controllerNumber<<1;
	ledStateRequest[controllerNumber].endpointIndex = 1;
	ledStateRequest[controllerNumber].transactionBuffer = ledStateBuffer[controllerNumber];
	ledStateRequest[controllerNumber].transactionLength = 12;
	ledStateRequest[controllerNumber].listener = this;
	ledStateRequest[controllerNumber].tag = LED_STATE_TAG;

	LPC2478::getHCD()->sendRequest(&ledStateRequest[controllerNumber]);
}

void XboxControllerDriver::setRumbleState(uint8_t smallSpeed, uint8_t largeSpeed, uint8_t controllerNumber) {
	rumbleStateBuffer[controllerNumber][0] = 0x00;
	rumbleStateBuffer[controllerNumber][1] = 0x01;
	rumbleStateBuffer[controllerNumber][2] = 0x0F;
	rumbleStateBuffer[controllerNumber][3] = 0xC0;
	rumbleStateBuffer[controllerNumber][4] = 0x00;
	rumbleStateBuffer[controllerNumber][5] = largeSpeed;
	rumbleStateBuffer[controllerNumber][6] = smallSpeed;
	rumbleStateBuffer[controllerNumber][7] = 0x00;
	rumbleStateBuffer[controllerNumber][8] = 0x00;
	rumbleStateBuffer[controllerNumber][9] = 0x00;
	rumbleStateBuffer[controllerNumber][10] = 0x00;
	rumbleStateBuffer[controllerNumber][11] = 0x00;

	rumbleStateRequest[controllerNumber].device = device;
	rumbleStateRequest[controllerNumber].interfaceIndex = controllerNumber<<1;
	rumbleStateRequest[controllerNumber].endpointIndex = 1;
	rumbleStateRequest[controllerNumber].transactionBuffer = rumbleStateBuffer[controllerNumber];
	rumbleStateRequest[controllerNumber].transactionLength = 12;
	rumbleStateRequest[controllerNumber].listener = this;
	rumbleStateRequest[controllerNumber].tag = RUMBLE_STATE_TAG;

	LPC2478::getHCD()->sendRequest(&rumbleStateRequest[controllerNumber]);
}

GamepadInputReport* XboxControllerDriver::getStatus(uint8_t controllerNumber) {
	return &gamepadStatus[controllerNumber];
}
