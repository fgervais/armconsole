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
	Gpio(FIO_TypeDef *gpioRegisters, uint8_t portLength, uint8_t id);
	virtual ~Gpio();

	void configure(GpioConfiguration config);

	GpioPin* getPin(uint8_t number);

	uint32_t getData();
	void setData(uint32_t);

	uint8_t getPortLength() { return portLength; }

private:
	FIO_TypeDef *gpioRegisters;
	GpioPin **gpioPins;
	uint8_t portLength;
};

#endif /* GPIO_H_ */
