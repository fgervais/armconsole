/*
 * Engine.h
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#ifndef ENGINE_H_
#define ENGINE_H_

class GamepadInputReport;

class Engine {
public:
	Engine();
	virtual ~Engine();

	void start(GamepadInputReport*, GamepadInputReport*);
};

#endif /* ENGINE_H_ */
