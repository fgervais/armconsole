/*
 * AudioHelper.cpp
 *
 *  Created on: Apr 3, 2010
 *      Author: fgervais
 */

#include "AudioHelper.h"
#include "DAC.h"
#include "Wave.h"
#include "LPC2478.h"

AudioHelper::AudioHelper(DAC* dac) {
	this->dac = dac;

	// init flags
	stopped = 1;
}

AudioHelper::~AudioHelper() {

}

void AudioHelper::play(Wave* wave) {
	if(!wave->isLoaded()) {
		// Check if file is loaded successfully
		if(wave->load() == 1) {
			return;
		}
	}

	uint32_t waveLength = (wave->getDataHeader()->size * 8) /  wave->getWAVEHeader()->bitPerSample;
	uint32_t usDelay = (1.0/(float)wave->getWAVEHeader()->sampleRate) / (1.0/0.000001);

	stopped = 0;
	for(uint32_t sampleCount=0; sampleCount<waveLength; sampleCount++) {
		// Sample are stored as a 16 bits signed value
		int32_t sample = wave->getData()[sampleCount];
		sample = (sample + 32768) >> 1;
		dac->setValue(sample);
		// TODO: Fixme
		//LPC2478::delay(usDelay);
		LPC2478::delay(24);
	}
	stopped = 1;
}

void AudioHelper::stop() {
	stopped = 1;
}
