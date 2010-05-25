/*
 * MetoolState.h
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#ifndef METOOLSTATE_H_
#define METOOLSTATE_H_

#include "State.h"

#include <stdint.h>

class Metool;

class MetoolState : public State {
public:
	MetoolState(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame, Bitmap** animationMasks);
	virtual ~MetoolState();

	virtual void update(Metool*) {}
	virtual void initialize(Metool*) {}
};

#endif /* METOOLSTATE_H_ */
