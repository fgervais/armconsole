/*
 * Megaman.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef MEGAMAN_H_
#define MEGAMAN_H_

#include "Sprite.h"

#include <stdint.h>

class Environment;
class MegamanState;
class VideoMemory;
class Collider;
class Wave;

class Megaman : public Sprite {
public:
	Megaman(MegamanState* initialState, Environment* environment);
	virtual ~Megaman();

	void setState(MegamanState* state);

	// Base class override
	virtual void update();
	virtual void collideWith(Collider*);
	virtual void collideWith(Megaman*);
	virtual void collideWith(Metool*);

	// Accessors
	Wave* getJumpSoundFX() { return jumpSoundFX; }
	Wave* getLandSoundFX() { return landSoundFX; }

	// Action functions
	void jump();
	void runLeft();
	void runRight();
	void stopRunning();
	void stopJumping();
private:
	// Current state
	MegamanState* state;

	// Every possible Megaman state


	// SoundFX
	static Wave* jumpSoundFX;
	static Wave* landSoundFX;
};

#endif /* MEGAMAN_H_ */
