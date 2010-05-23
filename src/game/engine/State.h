/*
 * State.h
 *
 *  Created on: May 18, 2010
 *      Author: fgervais
 */

#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>

class Bitmap;
class Sprite;
class VideoMemory;

class State {
public:
	State(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks);
	virtual ~State();

	// Accessors
	uint32_t getAnimationWidth();
	uint32_t getAnimationHeight();

	Bitmap* getCurrentFrame() { return animationFrames[currentFrame]; }

	virtual void reset();

	virtual void update(Sprite*) {}
	virtual void render(Sprite*, VideoMemory*);
	virtual void initialize(Sprite*) {}

protected:
	// Subclass may need to update the current frame manually
	uint32_t currentFrame;
	uint32_t numberOfFrame;

private:
	uint32_t animationHeight;
	uint32_t animationWidth;
	Bitmap** animationFrames;
	Bitmap** animationMasks;
};

#endif /* STATE_H_ */
