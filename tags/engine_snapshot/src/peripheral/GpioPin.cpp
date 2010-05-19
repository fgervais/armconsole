/*
 * GpioPin.cpp
 *
 *  Created on: Jan 4, 2010
 *      Author: fgervais
 */

#include "GpioPin.h"
#include "GpioPinConfiguration.h"
#include "GpioPinEventListener.h"
#include "Gpio.h"

GpioPin::GpioPin(FIO_TypeDef *gpioRegisters, uint8_t pinNumber, uint8_t portNumber) {
	this->gpioRegisters = gpioRegisters;
	this->pinNumber = pinNumber;
	this->portNumber = portNumber;

	// Initialization
	extiConfigured = 0;
	listener = 0;
}

GpioPin::~GpioPin() {

}

/**
 * High level external interrupt handler.
 */
void GpioPin::extInterrupt() {
	// Browse through every listeners and tell them that
	// this object have an event pending
	/*for(int32_t i=0; i<listeners.size(); i++) {
		if(listeners.getElement(i) != 0) {
			listeners.getElement(i)->stateChanged(this);
		}
	}*/

	if(listener != 0) {
		listener->stateChanged(this);
	}
}

/**
 * Configure this pin (Direction, Speed..).
 * @param config Configuration if the pin.
 */
void GpioPin::configure(GpioPinConfiguration config) {
	uint32_t fiodir;

	fiodir = gpioRegisters->FIODIR;
	// Clear the pin configuration
	fiodir &= ~(1 << pinNumber);
	// Configure the pin direction
	fiodir |= (config.pin << pinNumber);
	// Apply changes
	gpioRegisters->FIODIR = fiodir;
}

/**
 * Check if the pin is at the high level.
 * @return	0 if false, true otherwise.
 */
uint32_t GpioPin::isHigh() {
	return gpioRegisters->FIOPIN & ~(1 << pinNumber);
}

/**
 * Set the pin to the high level.
 */
void GpioPin::setHigh() {
	gpioRegisters->FIOSET = (1 << pinNumber);
}

/**
 * Set the pin to the low level.
 */
void GpioPin::setLow() {
	gpioRegisters->FIOCLR = (1 << pinNumber);
}

/**
 * Add an event listener to this pin.
 *
 * Adding a listener will configure the external interrupt
 * if not done already. For now, it gets notified on rising
 * edge only. It may be configurable in the future.
 *
 * @param listener Listener to be added
 */
void GpioPin::addEventListener(GpioPinEventListener* listener) {
	if(!extiConfigured) {
		configureInterrupt();
		extiConfigured = 1;
	}
	//listeners.addElement(listener);
	this->listener = listener;
}

/**
 * Helper method used to configure external interrupt.
 */
void GpioPin::configureInterrupt() {

}
