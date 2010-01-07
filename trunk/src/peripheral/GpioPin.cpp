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
	uint32_t configRegister;


}

/**
 * @brief	Check if the pin is at the high level.
 * @return	0 if false, true otherwise.
 */
uint32_t GpioPin::isHigh() {

}

/**
 * @brief	Set the pin to the high level.
 */
void GpioPin::setHigh() {

}

/**
 * @brief	Set the pin to the low level.
 */
void GpioPin::setLow() {

}

void GpioPin::addEventListener(GpioPinEventListener* listener) {
	if(!extiConfigured) {
		configureInterrupt();
		extiConfigured = 1;
	}
	//listeners.addElement(listener);
	this->listener = listener;
}


void GpioPin::configureInterrupt() {

}
