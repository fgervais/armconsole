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
	//dacRegisters->DACR = (value & 0x0000FFFF);
	dacRegisters->DACR = (value & 0x0000FFC0);
}
