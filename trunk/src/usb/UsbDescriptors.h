/*
 * UsbDescriptors.h
 *
 *  Created on: Jun 17, 2010
 *      Author: fgervais
 */

#ifndef USBDESCRIPTORS_H_
#define USBDESCRIPTORS_H_

// Device descriptor
#define DEVICE_DESCRIPTOR_INDEX		0x0100
#define DEVICE_DESCRIPTOR_LENGTH	0x12

struct DeviceDescriptor {
	uint16_t bcdUsb;
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;
};


#endif /* USBDESCRIPTORS_H_ */
