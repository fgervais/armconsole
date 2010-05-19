/*
 * EnemyWalkingLeft.h
 *
 *  Created on: May 13, 2010
 *      Author: fgervais
 */

#ifndef ENEMYWALKINGLEFT_H_
#define ENEMYWALKINGLEFT_H_

#include "EnemyState.h"

#include <stdint.h>

class EnemyWalkingLeft: public EnemyState {
public:
	static EnemyState* getInstance();

	// Base class function override
	virtual void initialize(Enemy*);
	virtual void update(Enemy*);

private:
	EnemyWalkingLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~EnemyWalkingLeft();

	//static EnemyState* instance;
	static Bitmap** sharedFrames;
};

#endif /* ENEMYWALKINGLEFT_H_ */
