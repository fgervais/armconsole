/*
 * DAC.h
 *
 *  Created on: Apr 3, 2010
 *      Author: fgervais
 */

#ifndef DAC_H_
#define DAC_H_

#include "Peripheral.h"
#include "LPC23xx.h"

/**
 * Digital to Analog Converter handling class.
 */
class DAC: public Peripheral {
public:
	DAC(DAC_Typedef* dacRegisters);
	virtual ~DAC();

	void setValue(uint16_t value);

private:
	DAC_Typedef* dacRegisters;
};

#endif /* DAC_H_ */
