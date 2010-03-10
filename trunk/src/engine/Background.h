/*
 * Background.h
 *
 *  Created on: Mar 10, 2010
 *      Author: fgervais
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include <stdint.h>

class Background {
public:
	Background(char* path, uint32_t height, uint32_t width);
	virtual ~Background();

private:
	char* path;
	uint32_t height;
	uint32_t width;
};

#endif /* BACKGROUND_H_ */
