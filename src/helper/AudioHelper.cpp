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
		wave->load();
	}

	uint32_t waveLength = (wave->getDataHeader()->size * 8) /  wave->getWAVEHeader()->bitPerSample;
	uint32_t usDelay = (1.0/(float)wave->getWAVEHeader()->sampleRate) / (1.0/(float)LPC2478::getFrequency());

	stopped = 0;
	for(uint32_t sampleCount=0; sampleCount<waveLength; sampleCount++) {
		dac->setValue(wave->getData()[sampleCount]);
		LPC2478::delay(usDelay);
	}
	stopped = 1;
}

void AudioHelper::stop() {
	stopped = 1;
}
