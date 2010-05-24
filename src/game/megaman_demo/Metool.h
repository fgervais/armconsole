/*
 * Enemy.h
 *
 *  Created on: May 12, 2010
 *      Author: fgervais
 */

#ifndef METOOL_H_
#define METOOL_H_

#include "Sprite.h"

#include <stdint.h>

class Collider;
class MetoolState;
class Environment;
class State;

class Metool: public Sprite {
public:
	Metool(MetoolState* initialState, Environment* environment);
	virtual ~Metool();

	void setState(MetoolState* state);

	// State accessor
	MetoolState* getWalkingLeftState() { return metoolWalkingLeft; }
	MetoolState* getWalkingRightState() { return metoolWalkingRight; }

	// Base class override
	virtual void update();
	virtual void collideWith(Collider*);
	virtual void collideWith(Megaman*);
	virtual void collideWith(Metool*);
private:
	// Current state
	MetoolState* state;

	// Every possible Metool state
	MetoolState* metoolWalkingLeft;
	MetoolState* metoolWalkingRight;
};

#endif /* METOOL_H_ */
