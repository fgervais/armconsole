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
