/*
 * UsbDevice.h
 *
 *  Created on: Jan 14, 2010
 *      Author: fgervais
 */

#ifndef USBDEVICE_H_
#define USBDEVICE_H_

#include "HostControllerDriver.h"

#include <stdint.h>

class DeviceDescriptor;
class ConfigurationDescriptor;

class UsbDevice {
public:
	enum Speed {
		LowSpeed,
		FullSpeed
	};

	UsbDevice(uint8_t address, Speed);
	virtual ~UsbDevice();

	void setDeviceDescriptor(DeviceDescriptor*);
	DeviceDescriptor* getDeviceDescriptor() { return deviceDescriptor; }
	void setConfigurationDescriptor(ConfigurationDescriptor*);
	ConfigurationDescriptor* getConfigurationDescriptor() { return configurationDescriptor; }

	uint8_t getAddress() { return address; }
	Speed getSpeed() { return speed; }
	HcEd*** getEndpoints() { return endpoint; }

	void setEndpoints(HcEd*** endpoint) { this->endpoint = endpoint; }
private:
	uint8_t address;
	Speed speed;
	DeviceDescriptor* deviceDescriptor;
	ConfigurationDescriptor* configurationDescriptor;
	HcEd*** endpoint;
};

#endif /* USBDEVICE_H_ */
