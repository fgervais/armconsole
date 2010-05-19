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

class ConfigurationDescriptor: public Descriptor {
public:
	uint16_t wTotalLength;
	uint8_t bNumInterface;
	uint8_t bConfigurationValue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t bMaxPower;

	ConfigurationDescriptor(uint8_t* rawDescriptor);
	virtual ~ConfigurationDescriptor();
};

#endif /* CONFIGURATIONDESCRIPTOR_H_ */
