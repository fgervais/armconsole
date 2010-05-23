/*
 * TimerConfiguration.h
 *
 *  Created on: Nov 6, 2009
 *      Author: fgervais
 */

#ifndef TIMERCONFIGURATION_H_
#define TIMERCONFIGURATION_H_

#include <stdint.h>

#include "Timer.h"

class TimerConfiguration {
public:
	Timer::Mode mode;
	uint32_t prescaler;
	uint32_t compare;

	TimerConfiguration();
	virtual ~TimerConfiguration();
};

#endif /* TIMERCONFIGURATION_H_ */
