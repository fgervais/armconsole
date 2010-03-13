/*
 * DMAChannel.h
 *
 *  Created on: Mar 13, 2010
 *      Author: fgervais
 */

#ifndef DMACHANNEL_H_
#define DMACHANNEL_H_

#include "Peripheral.h"
#include <stdint.h>

class DMAChannel: public Peripheral {
public:
	DMAChannel(uint8_t id);
	virtual ~DMAChannel();
};

#endif /* DMACHANNEL_H_ */
