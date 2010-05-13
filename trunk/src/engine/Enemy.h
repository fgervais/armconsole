/*
 * Enemy.h
 *
 *  Created on: May 12, 2010
 *      Author: fgervais
 */

#ifndef ENEMY_H_
#define ENEMY_H_

#include "Sprite.h"

#include <stdint.h>

class Environment;
class EnemyState;
class VideoMemory;

class Enemy: public Sprite {
public:
	Enemy(EnemyState* initialState, Environment* environment);
	virtual ~Enemy();

	// Accessors
	virtual uint32_t getWidth();
	virtual uint32_t getHeight();

	void setState(EnemyState* state);

	virtual void update();
	virtual void render(VideoMemory*);

private:
	EnemyState* state;
};

#endif /* ENEMY_H_ */
