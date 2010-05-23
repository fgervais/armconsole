/*
 * TimerConfiguration.cpp
 *
 *  Created on: Nov 6, 2009
 *      Author: fgervais
 */

#include "TimerConfiguration.h"

TimerConfiguration::TimerConfiguration() {
	mode = Timer::None;
	prescaler = 0;
	compare = 0;
}

TimerConfiguration::~TimerConfiguration() {

}
