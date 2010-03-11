/*
 * Engine.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: fgervais
 */

#include "Engine.h"
#include "Environment.h"
#include "Level1.h"
#include "VideoMemory.h"

Engine::Engine() {


}

Engine::~Engine() {

}

void Engine::start() {
	// Create the instance of a level
	Environment* environment = new Level1();

	VideoMemory* videoMemory = new VideoMemory((uint32_t*)0xA0000000,480,272);

	// Start tick timer

	// Infinite game loop
	while(1) {
		environment->update();
		environment->render(videoMemory);

		// 1/25s synchronization
	}
}
