/*
 * DAC.cpp
 *
 *  Created on: Apr 3, 2010
 *      Author: fgervais
 */

#include "DAC.h"

DAC::DAC(DAC_Typedef* dacRegisters) {
	this->dacRegisters = dacRegisters;
}

DAC::~DAC() {

}

void DAC::setValue(uint16_t value) {
	dacRegisters->DACR = (value & 1023) << (16 + 6);
}
