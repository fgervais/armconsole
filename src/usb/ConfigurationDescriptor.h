/*
 * ConfigurationDescriptor.h
 *
 *  Created on: Nov 4, 2009
 *      Author: fgervais
 */

#ifndef CONFIGURATIONDESCRIPTOR_H_
#define CONFIGURATIONDESCRIPTOR_H_

#include "Descriptor.h"
#include <stdint.h>

class InterfaceDescriptor;

class ConfigurationDescriptor: public Descriptor {
public:
	static const uint16_t TYPE = 0x02;
	static const uint16_t LENGTH = 0x09;

	uint16_t wTotalLength;
	uint8_t bNumInterfaces;
	uint8_t bConfigurationValue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t bMaxPower;

	InterfaceDescriptor** interfaceDescriptor;

	ConfigurationDescriptor(uint8_t* rawDescriptor);
	virtual ~ConfigurationDescriptor();

	InterfaceDescriptor* getInterfaceDescriptor(uint32_t number) { return interfaceDescriptor[number]; }
};

#endif /* CONFIGURATIONDESCRIPTOR_H_ */
