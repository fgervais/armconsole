/*
 * Timer.cpp
 *
 *  Created on: 2009-09-29
 *      Author: oex
 */

#include "Timer.h"
#include "TimerConfiguration.h"
#include "TimerOverflowListener.h"
#include "irq.h"

Timer::Timer(TIMER_Typedef* timerRegisters, uint8_t id) : Peripheral(id) {
	this->timerRegisters = timerRegisters;

	// Initialization
	mode = None;
	overflowiConfigured = 0;
}

Timer::~Timer() {

}

/**
 * @brief Function called by the overflow interrupt.
 */
void Timer::overflowInterrupt() {
	// Browse through every listeners and tell them that
	// this object have an event pending
	for(int32_t i=0; i<overflowListeners.size(); i++) {
		if(overflowListeners.getElement(i) != 0) {
			overflowListeners.getElement(i)->timerOverflowed(this);
		}
	}
}

/**
 * @brief Configure the timer in a certain mode.
 *
 * The timer needs to be in the right mode before adding
 * an event listener.
 *
 * @param config Configuration structure
 */
void Timer::configure(TimerConfiguration config) {
	disable();

	switch(config.mode) {
	case Timer::Counter:
		timerRegisters->PR = config.prescaler;
		timerRegisters->MR0 = config.compare;
		counterMode();
		break;
	default:
		break;
	}
}

/**
 * Add a timer overflow listener.
 * @param listener Overflow listener.
 * @return 0 if successful, 1 if the timer is not in the right mode.
 */
uint8_t Timer::addEventListener(TimerOverflowListener* listener) {
	// Timer should be configured in the right mode first
	if(mode == Counter) {
		if(!overflowiConfigured) {
			configureOverflowInterrupt();
			overflowiConfigured = 1;
		}
		overflowListeners.addElement(listener);
		return 0;
	}
	return 1;
}

/**
 * @brief Enable the timer.
 */
void Timer::enable() {
	timerRegisters->TCR = 1;
}

/**
 * @brief Disable the timer.
 */
void Timer::disable() {
	timerRegisters->TCR = 0;
}

/**
 * @brief Set the timer in counter mode.
 */
void Timer::counterMode() {
	timerRegisters->CTCR = 0;		// Timer mode
	timerRegisters->MCR |= (1<<1);	// Reset Timer on MR0 match

	mode = Counter;
}

/**
 * @brief Configure the overflow interrupt.
 */
void Timer::configureOverflowInterrupt() {
	// Get the IRQn according to the timer id
	// Better way to do this anyone?
	uint8_t irqNumber = 0;
	switch(getId()) {
	case 1:
		// Timer 1 is an advanced control timer and is not supported
		break;
	case 2:
		irqNumber = TIMER0_INT;
		break;
	case 3:
		irqNumber = TIMER1_INT;
		break;
	case 4:
		irqNumber = TIMER2_INT;
		break;
	case 5:
		irqNumber = TIMER3_INT;
		break;
	default:
		break;
	}

	timerRegisters->MCR |= (1<<0);	// Interrupt on MR0 match

	VICIntSelect &= ~(1 << irqNumber); // IRQ Category (Not FIQ)
	VICIntEnable |= (1 << irqNumber);
}
