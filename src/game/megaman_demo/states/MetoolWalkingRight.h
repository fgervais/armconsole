/*
 * MetoolWalkingRight.h
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#ifndef METOOLWALKINGRIGHT_H_
#define METOOLWALKINGRIGHT_H_

#include "MetoolState.h"

#include <stdint.h>

class Metool;

class MetoolWalkingRight: public MetoolState {
public:
	static MetoolState* getInstance();

	// Base class function override
	virtual void initialize(Metool*);
	virtual void update(Metool*);

private:
	MetoolWalkingRight(uint32_t animationWidth, uint32_t animationHeight, Bitmap** animationFrames, uint32_t numberOfFrame);
	virtual ~MetoolWalkingRight();

	//static MetoolState* instance;
	static Bitmap** sharedFrames;
};

#endif /* METOOLWALKINGRIGHT_H_ */
