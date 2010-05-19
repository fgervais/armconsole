/*
 * TimerOverflowListener.h
 *
 *  Created on: Nov 6, 2009
 *      Author: fgervais
 */

#ifndef TIMEROVERFLOWLISTENER_H_
#define TIMEROVERFLOWLISTENER_H_

class Timer;

class TimerOverflowListener {
public:
	TimerOverflowListener();
	virtual ~TimerOverflowListener();

	virtual void timerOverflowed(Timer* timer) = 0;
};

#endif /* TIMEROVERFLOWLISTENER_H_ */
