/*
 * Gpio.cpp
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#include "Gpio.h"

#include "GpioPin.h"
#include "GpioConfiguration.h"

Gpio::Gpio(FIO_TypeDef *gpioRegisters, uint8_t portLength, uint8_t id) : Peripheral(id) {
	this->gpioRegisters = gpioRegisters;
	this->portLength = portLength;

	gpioPins = new GpioPin*[portLength];

	for(uint8_t i=0; i<portLength; i++) {
		// Create a new pin with it's corresponding pin number.
		gpioPins[i] = new GpioPin(gpioRegisters, i, id);
	}

}

Gpio::~Gpio() {
	/* Free memory */
	for(uint8_t i=0; i<portLength; i++) {
		// Create a new pin with it's corresponding pin number.
		delete[] gpioPins[i];
	}
	delete[] gpioPins;
}
