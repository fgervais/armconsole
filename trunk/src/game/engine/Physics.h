/*
 * Physics.h
 *
 *  Created on: Mar 10, 2010
 *      Author: fgervais
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <stdint.h>

class Physics {
public:
	Physics(int32_t gravitation);
	virtual ~Physics();

	int32_t getGravitation() { return gravitation; }

private:
	int32_t gravitation;
};

#endif /* PHYSICS_H_ */
