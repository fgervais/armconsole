/*
 * Synchronized.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef SYNCHRONIZED_H_
#define SYNCHRONIZED_H_

class Synchronized {
public:
	Synchronized();
	virtual ~Synchronized();

	virtual void update() = 0;
};

#endif /* SYNCHRONIZED_H_ */
