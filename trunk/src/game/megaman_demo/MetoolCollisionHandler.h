/*
 * MetoolCollisionHandler.h
 *
 *  Created on: May 18, 2010
 *      Author: fgervais
 */

#ifndef METOOLCOLLISIONHANDLER_H_
#define METOOLCOLLISIONHANDLER_H_

class Metool;
class Megaman;

class MetoolCollisionHandler {
public:
	MetoolCollisionHandler();
	virtual ~MetoolCollisionHandler();

	void handleCollision(Metool* parentSprite, Metool*) {}
	void handleCollision(Metool* parentSprite, Megaman*) {}
};

#endif /* METOOLCOLLISIONHANDLER_H_ */
