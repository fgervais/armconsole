/*
 * AudioHelper.h
 *
 *  Created on: Apr 3, 2010
 *      Author: fgervais
 */

#ifndef AUDIOHELPER_H_
#define AUDIOHELPER_H_

#include "ff.h"
#include <stdint.h>

class DAC;
class Wave;

class AudioHelper {
public:
	AudioHelper(DAC* dac);
	virtual ~AudioHelper();

	void play(Wave* wave);
	void stop();

private:
	DAC* dac;

	uint8_t stopped;
};

#endif /* AUDIOHELPER_H_ */
