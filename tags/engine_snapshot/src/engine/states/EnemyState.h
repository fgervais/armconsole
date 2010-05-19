/*
 * EnemyState.h
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#ifndef ENEMYSTATE_H_
#define ENEMYSTATE_H_

#include <stdint.h>

class Sprite;
class Bitmap;
class VideoMemory;
class Enemy;

class EnemyState {
public:
	EnemyState(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~EnemyState();

	virtual void initialize(Enemy*) {};

	// Accessors
	uint32_t getWidth();
	uint32_t getHeight();

	Bitmap* getCurrentFrame() { return frames[currentFrame]; }

	virtual void reset();
	virtual void update(Enemy*) {};
	virtual void render(Enemy*, VideoMemory*);

protected:
	// Subclass may need to update the current frame manually
	uint32_t currentFrame;
	uint32_t numberOfFrame;

private:
	uint32_t height;
	uint32_t width;
	Bitmap** frames;
};

#endif /* ENEMYSTATE_H_ */
