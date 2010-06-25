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

UsbDevice::UsbDevice(uint8_t address) {
	this->address = address;

}

UsbDevice::~UsbDevice() {
	/* Free memory */
	delete deviceDescriptor;
}

void UsbDevice::setDeviceDescriptor(DeviceDescriptor* deviceDescriptor) {
	this->deviceDescriptor = deviceDescriptor;
}

void UsbDevice::setConfigurationDescriptor(ConfigurationDescriptor* configurationDescriptor) {
	this->configurationDescriptor = configurationDescriptor;
}
