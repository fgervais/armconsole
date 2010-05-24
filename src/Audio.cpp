/*
 * Audio.cpp
 *
 *  Created on: May 24, 2010
 *      Author: fgervais
 */

#include "Audio.h"
#include "AudioHelper.h"
#include "Wave.h"
#include "LPC2478.h"

AudioHelper* Audio::audioHelper = 0;

Audio::Audio() {
	audioHelper = new AudioHelper(LPC2478::getDAC(), LPC2478::getTimer1());
}

Audio::~Audio() {

}

void Audio::playAsync(Wave* wave) {
	//TODO: This is a hack since it should be called from the constructor
	if(audioHelper == 0) {
		audioHelper = new AudioHelper(LPC2478::getDAC(), LPC2478::getTimer1());
	}

	audioHelper->playAsync(wave);
}
