/*
 * Level1.h
 *
 *  Created on: Mar 11, 2010
 *      Author: fgervais
 */

#ifndef LEVEL1_H_
#define LEVEL1_H_

#include "Environment.h"

class Level1: public Environment {
public:
	Level1();
	virtual ~Level1();

	virtual void build();
};

#endif /* LEVEL1_H_ */
