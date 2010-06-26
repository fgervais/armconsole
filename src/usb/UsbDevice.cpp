/*
 * UsbDevice.cpp
 *
 *  Created on: Jan 14, 2010
 *      Author: fgervais
 */

#include "UsbDevice.h"
#include "DeviceDescriptor.h"
#include "ConfigurationDescriptor.h"
#include "Debug.h"

UsbDevice::UsbDevice(uint8_t address, Speed speed) {
	this->address = address;
	this->speed = speed;

	endpoint = 0;
}

UsbDevice::~UsbDevice() {
	/* Free memory */
	delete deviceDescriptor;
	delete configurationDescriptor;

	if(endpoint != 0) {
		for(uint8_t interfaceNumber=0; interfaceNumber<getConfigurationDescriptor()->bNumInterfaces; interfaceNumber++) {
			delete[] endpoint[interfaceNumber];
		}
		delete[] endpoint;
	}
}

void UsbDevice::setDeviceDescriptor(DeviceDescriptor* deviceDescriptor) {
	this->deviceDescriptor = deviceDescriptor;
}

void UsbDevice::setConfigurationDescriptor(ConfigurationDescriptor* configurationDescriptor) {
	this->configurationDescriptor = configurationDescriptor;
}
