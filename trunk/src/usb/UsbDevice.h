/*
 * UsbDevice.h
 *
 *  Created on: Jan 14, 2010
 *      Author: fgervais
 */

#ifndef USBDEVICE_H_
#define USBDEVICE_H_

#include <stdint.h>

class DeviceDescriptor;
class ConfigurationDescriptor;

class UsbDevice {
public:
	UsbDevice(uint8_t address);
	virtual ~UsbDevice();

	void setDeviceDescriptor(DeviceDescriptor*);
	DeviceDescriptor* getDeviceDescriptor() { return deviceDescriptor; }
	void setConfigurationDescriptor(ConfigurationDescriptor*);
	ConfigurationDescriptor* getConfigurationDescriptor() { return configurationDescriptor; }

private:
	uint8_t address;
	DeviceDescriptor* deviceDescriptor;
	ConfigurationDescriptor* configurationDescriptor;

};

#endif /* USBDEVICE_H_ */
