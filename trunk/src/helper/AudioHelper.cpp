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
#include "Timer.h"
#include "TimerConfiguration.h"
#include "Debug.h"

AudioHelper::AudioHelper(DAC* dac) {
	this->dac = dac;

	// init flags
	stopped = 1;
	asyncEnabled = 0;

	for(uint8_t i=0; i<MIXER_SIZE; i++) {
		wavePlaying[i] = 0;
	}
	timer = 0;
}

AudioHelper::AudioHelper(DAC* dac, Timer* timer) {
	this->dac = dac;
	this->timer = timer;

	timer->disable();
	// Pre configure timer
	TimerConfiguration timerConfig;
	timerConfig.mode = Timer::Counter;
	timerConfig.prescaler = 0;
	timerConfig.compare = 0;
	timer->configure(timerConfig);
	timer->addEventListener(this);

	// init flags
	stopped = 1;
	asyncEnabled = 1;

	for(uint8_t i=0; i<MIXER_SIZE; i++) {
		wavePlaying[i] = 0;
	}
}

AudioHelper::~AudioHelper() {

}

void AudioHelper::timerOverflowed(Timer* timer) {
	int32_t mixedSample = 0;
	for(uint8_t i=0; i<MIXER_SIZE; i++) {
		if(wavePlaying[i] != 0) {
			if(waveSampleCount[i]<waveLength[i]) {
				// Sample are stored as a 16 bits signed value
				int32_t sample = wavePlaying[i]->getData()[waveSampleCount[i]];
				//sample = (sample + 32768) >> 1;
				mixedSample += sample;
				waveSampleCount[i]++;
			}
			else {
				wavePlaying[i] = 0;
			}
		}
	}
	/*if(mixedSample == 0) {
		timer->disable();
		stopped = 1;
	}
	else {
		dac->setValue(mixedSample);
	}*/
	if(mixedSample != 0) {
		dac->setValue((mixedSample+ 32768) >> 1);
	}
}

void AudioHelper::play(Wave* wave) {
	if(!wave->isLoaded()) {
		// Check if file is loaded successfully
		if(wave->load() == 0) {
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

void AudioHelper::playAsync(Wave* wave) {
	if(asyncEnabled) {

		if(!wave->isLoaded()) {
			// Check if file is loaded successfully
			if(wave->load() == 0) {
				return;
			}
		}

		uint8_t mixerPosition;
		for(mixerPosition=0; mixerPosition<MIXER_SIZE; mixerPosition++) {
			if(wavePlaying[mixerPosition] == 0) {
				break;
			}
		}
		if(mixerPosition == MIXER_SIZE) {
			return;	// No free spot available
		}

		waveLength[mixerPosition] = (wave->getDataHeader()->size * 8) /  wave->getWAVEHeader()->bitPerSample;
		//uint32_t usDelay = (1.0/(float)wave->getWAVEHeader()->sampleRate) / (1.0/0.000001);

		waveSampleCount[mixerPosition] = 0;
		wavePlaying[mixerPosition] = wave;

		if(stopped) {
			// Configure Timer
			TimerConfiguration timerConfig;
			timerConfig.mode = Timer::Counter;
			timerConfig.prescaler = 0;
			//debug - Hardcoded
			timerConfig.compare = 2250;		// 32 kHz

			timer->configure(timerConfig);

			timer->enable();

			int32_t sample = wave->getData()[waveSampleCount[mixerPosition]];
			sample = (sample + 32768) >> 1;
			dac->setValue(sample);
			waveSampleCount[mixerPosition]++;

			stopped = 0;
		}
	}
}

void AudioHelper::stop() {
	stopped = 1;
}
