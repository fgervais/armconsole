/*
 * Descriptor.h
 *
 *  Created on: Nov 4, 2009
 *      Author: fgervais
 */

#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_

#include <stdint.h>

class Descriptor {
public:
	uint8_t bLength;
	uint8_t bDescriptorType;

	Descriptor();
	virtual ~Descriptor();
};

#endif /* DESCRIPTOR_H_ */
