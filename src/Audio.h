/*
 * Audio.h
 *
 *  Created on: May 24, 2010
 *      Author: fgervais
 */

#ifndef AUDIO_H_
#define AUDIO_H_

class Wave;
class AudioHelper;

class Audio {
public:
	Audio();
	virtual ~Audio();

	static void playAsync(Wave* wave);

private:
	static AudioHelper* audioHelper;
};

#endif /* AUDIO_H_ */
