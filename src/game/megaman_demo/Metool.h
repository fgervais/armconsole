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

	// Base class override
	virtual void update();
	virtual void collideWith(Collider*);
private:
	MetoolState* state;
};

#endif /* METOOL_H_ */
