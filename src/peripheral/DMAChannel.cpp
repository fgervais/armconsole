/*
 * DMAChannel.cpp
 *
 *  Created on: Mar 13, 2010
 *      Author: fgervais
 */

#include "DMAChannel.h"
#include "DMAConfiguration.h"

DMAChannel::DMAChannel(uint8_t id, GPDMA_CH_Typedef* dmaChRegisters) : Peripheral(id) {
	this->dmaChRegisters = dmaChRegisters;
}

DMAChannel::~DMAChannel() {

}

void DMAChannel::configure(DMAConfiguration config) {
	GPDMA->GPDMA_INT_TCCLR = 0x01;
	GPDMA->GPDMA_INT_ERR_CLR = 0x01;

	dmaChRegisters->GPDMA_CH_SRC = config.lli->DMACCxSrcAddr;
	dmaChRegisters->GPDMA_CH_DEST = config.lli->DMACCxDestAddr;
	dmaChRegisters->GPDMA_CH_LLI = (uint32_t)config.lli->DMACCxLLI;
	dmaChRegisters->GPDMA_CH_CTRL = config.lli->DMACCxControl;
}

void DMAChannel::enable() {
	dmaChRegisters->GPDMA_CH_CFG |= 1;
}

void DMAChannel::disable() {
	dmaChRegisters->GPDMA_CH_CFG &= ~1;
}

uint8_t DMAChannel::isEnabled() {
	return dmaChRegisters->GPDMA_CH_CFG & 0x00000001;
}
