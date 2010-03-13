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

/**
 * This is the structure used for scatter/gatter DMA tranfer.
 *
 * The naming convention is the one used in the LPC24XX
 * user manual.
 */
struct DMACCxLLI {
	uint32_t DMACCxSrcAddr;
	uint32_t DMACCxDestAddr;
	DMACCxLLI* DMACCxLLI;
	uint32_t DMACCxControl;
};

class DMAConfiguration;

class DMAChannel: public Peripheral {
public:
	DMAChannel(uint8_t id);
	virtual ~DMAChannel();

	void configure(DMAConfiguration config);
};

#endif /* DMACHANNEL_H_ */
