/*
 * MetoolWalkingLeft.h
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#ifndef METOOLWALKINGLEFT_H_
#define METOOLWALKINGLEFT_H_

#include "MetoolState.h"

#include <stdint.h>

class Metool;
class Sprite;

class MetoolWalkingLeft: public MetoolState {
public:
	virtual ~MetoolWalkingLeft();

	static MetoolState* createInstance();

	// Base class function override
	virtual void initialize(Metool*);
	virtual void update(Metool*);

private:
	MetoolWalkingLeft(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks);

	//static MetoolState* instance;
	static Bitmap** sharedFrames;
	static Bitmap** sharedMasks;
};

#endif /* METOOLWALKINGLEFT_H_ */
