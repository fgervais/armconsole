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

	gpioPin = new GpioPin*[portLength];

	for(uint8_t i=0; i<portLength; i++) {
		// Create a new pin with it's corresponding pin number.
		gpioPin[i] = new GpioPin(gpioRegisters, i, id);
	}

}

Gpio::~Gpio() {
	/* Free memory */
	for(uint8_t i=0; i<portLength; i++) {
		delete[] gpioPin[i];
	}
	delete[] gpioPin;
}

/**
 * Configure the GPIO port according to the configuration provided.
 *
 * @param config Configuration of the port.
 */
void Gpio::configure(GpioConfiguration config) {
	uint32_t fiodir;

	fiodir = 0;
	for(uint8_t pinNumber=0; pinNumber<portLength; pinNumber++) {
		fiodir |= (config.pin[pinNumber] << pinNumber);
	}
	gpioRegisters->FIODIR = fiodir;
}

/**
 * Get a handle on the specified GPIO pin.
 *
 * @param number Number of the pin required
 * @return Handle to the pin if the pin number is valid, 0 otherwise.
 */
GpioPin* Gpio::getPin(uint8_t number) {
	if(number < portLength) {
		return gpioPin[number];
	}
	return 0;
}

/**
 * Get data of the whole GPIO port.
 *
 * Note that the port mask should be all 0s or you won't get the
 * whole port value. The mask default to 0x00 on reset so if
 * you don't know what this is about, it's probably fine.
 *
 * @return Value of the port
 */
uint32_t Gpio::getData() {
	return gpioRegisters->FIOPIN;
}

/**
 * Set data of the whole GPIO port.
 *
 * Note that the port mask should be all 0s or you won't set the
 * whole port value. The mask default to 0x00 on reset so if
 * you don't know what this is about, it's probably fine.
 *
 * @param data Value of the port to set
 */
void Gpio::setData(uint32_t data) {
	gpioRegisters->FIOPIN = data;
}

/**
 * Set the port of the GPIO port.
 *
 * @see LPC2478 user manual for more informations.
 * @param mask Mask to set
 */
void Gpio::setMask(uint32_t mask) {
	gpioRegisters->FIOMASK = mask;
}
