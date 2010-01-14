/*
 * InterfaceDescriptor.h
 *
 *  Created on: Nov 4, 2009
 *      Author: fgervais
 */

#ifndef INTERFACEDESCRIPTOR_H_
#define INTERFACEDESCRIPTOR_H_

#include "Descriptor.h"

#include <stdint.h>

class InterfaceDescriptor: public Descriptor {
public:
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndPoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;

	InterfaceDescriptor(uint8_t* rawDescriptor);
	virtual ~InterfaceDescriptor();
};

#endif /* INTERFACEDESCRIPTOR_H_ */
