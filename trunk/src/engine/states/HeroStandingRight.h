/*
 * HeroStandingRight.h
 *
 *  Created on: May 3, 2010
 *      Author: fgervais
 */

#ifndef HEROSTANDINGRIGHT_H_
#define HEROSTANDINGRIGHT_H_

#include "HeroState.h"

class HeroStandingRight : public HeroState {
public:
	static HeroState* getInstance();

private:
	HeroStandingRight(uint32_t width, uint32_t height, Bitmap** frames, uint32_t numberOfFrame);
	virtual ~HeroStandingRight();

	static HeroState* instance;
};

#endif /* HEROSTANDINGRIGHT_H_ */