/*
 * Renderable.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

class Renderable {
public:
	Renderable();
	virtual ~Renderable();

	virtual void render() = 0;
};

#endif /* RENDERABLE_H_ */
