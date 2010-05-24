/*
 * Timer.h
 *
 *  Created on: 2009-09-29
 *      Author: oex
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "Peripheral.h"
#include "Vector.h"

#include "LPC23xx.h"

class TimerConfiguration;
class TimerOverflowListener;

class Timer: public Peripheral {
public:
	enum Mode { None=0, Counter };

	Timer(TIMER_Typedef* timerRegisters, uint8_t id);
	virtual ~Timer();

	void overflowInterrupt();
	void configure(TimerConfiguration config);
	uint8_t addEventListener(TimerOverflowListener* listener);
	void enable();
	void disable();
private:
	TIMER_Typedef* timerRegisters;
	Mode mode;
	Vector<TimerOverflowListener*> overflowListeners;
	TimerOverflowListener* listener;
	uint8_t overflowiConfigured;

	void counterMode();
	void configureOverflowInterrupt();
};

#endif /* TIMER_H_ */
