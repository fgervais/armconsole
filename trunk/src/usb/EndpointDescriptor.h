/*
 * EndpointDescriptor.h
 *
 *  Created on: Nov 5, 2009
 *      Author: fgervais
 */

#ifndef ENDPOINTDESCRIPTOR_H_
#define ENDPOINTDESCRIPTOR_H_

#include "Descriptor.h"

#include <stdint.h>

class EndpointDescriptor: public Descriptor {
public:
	static const uint16_t TYPE = 0x05;
	static const uint16_t LENGTH = 0x07;

	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;

	EndpointDescriptor(uint8_t* rawDescriptor);
	virtual ~EndpointDescriptor();
};

#endif /* ENDPOINTDESCRIPTOR_H_ */
