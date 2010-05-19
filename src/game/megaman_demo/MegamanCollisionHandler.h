/*
 * MegamanCollisionHandler.h
 *
 *  Created on: May 18, 2010
 *      Author: fgervais
 */

#ifndef MEGAMANCOLLISIONHANDLER_H_
#define MEGAMANCOLLISIONHANDLER_H_

class Megaman;
class Metool;

class MegamanCollisionHandler {
public:
	MegamanCollisionHandler();
	virtual ~MegamanCollisionHandler();

	void handleCollision(Megaman* parentSprite, Metool*);
	void handleCollision(Megaman* parentSprite, Megaman*);
};

#endif /* MEGAMANCOLLISIONHANDLER_H_ */
