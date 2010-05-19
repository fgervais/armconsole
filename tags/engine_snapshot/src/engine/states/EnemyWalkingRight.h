/*
 * EnemyWalkingRight.h
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#ifndef ENEMYWALKINGRIGHT_H_
#define ENEMYWALKINGRIGHT_H_

#include "EnemyState.h"

#include <stdint.h>

class EnemyWalkingRight: public EnemyState {
public:
	static EnemyState* getInstance();

	// Base class function override
	virtual void initialize(Enemy*);
	virtual void update(Enemy*);

private:
	EnemyWalkingRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~EnemyWalkingRight();

	//static EnemyState* instance;
	static Bitmap** sharedFrames;
};

#endif /* ENEMYWALKINGRIGHT_H_ */
