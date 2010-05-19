/*
 * Gpio.h
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "Peripheral.h"
#include "LPC23xx.h"

#include <stdint.h>

class GpioConfiguration;
class GpioPin;

/**
 * General purpose IO port.
 */
class Gpio: public Peripheral {
public:
	static const uint8_t INPUT = 0;		/** Input mode */
	static const uint8_t OUTPUT = 1;	/** Output mode */

	Gpio(FIO_TypeDef *gpioRegisters, uint8_t portLength, uint8_t id);
	virtual ~Gpio();

	void configure(GpioConfiguration config);

	GpioPin* getPin(uint8_t number);

	uint32_t getData();
	void setData(uint32_t data);
	void setMask(uint32_t mask);

	uint8_t getPortLength() { return portLength; }

private:
	FIO_TypeDef *gpioRegisters;
	GpioPin **gpioPin;
	uint8_t portLength;
};

#endif /* GPIO_H_ */
