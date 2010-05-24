/*
 * AudioHelper.h
 *
 *  Created on: Apr 3, 2010
 *      Author: fgervais
 */

#ifndef AUDIOHELPER_H_
#define AUDIOHELPER_H_

#include "TimerOverflowListener.h"

#include "ff.h"
#include <stdint.h>

class DAC;
class Wave;
class Timer;

class AudioHelper : public TimerOverflowListener {
public:
	AudioHelper(DAC* dac);
	AudioHelper(DAC* dac, Timer* timer);
	virtual ~AudioHelper();

	// TimerOverflowListener implementation
	virtual void timerOverflowed(Timer* timer);

	void play(Wave* wave);
	void playAsync(Wave* wave);
	void stop();

private:
	DAC* dac;
	Timer* timer;

	// Flags
	uint8_t stopped;
	uint8_t asyncEnabled;

	// Used in asynchronous mode
	Wave* wavePlaying;
	uint32_t waveSampleCount;
	uint32_t waveLength;
};

#endif /* AUDIOHELPER_H_ */
