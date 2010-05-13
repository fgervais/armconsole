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
	EnemyWalkingLeft(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~EnemyWalkingLeft();
};

#endif /* ENEMYWALKINGLEFT_H_ */
