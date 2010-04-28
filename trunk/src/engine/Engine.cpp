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
#include "Debug.h"
#include "LPC2478.h"
#include "LCDControllerDriver.h"

Engine::Engine() {


}

Engine::~Engine() {

}

void Engine::start() {
	Debug::writeLine("Entering Engine start function");
	// Create the instance of a level
	Environment* environment = new Level1();
	environment->build();

	VideoMemory** videoPage = new VideoMemory*[2];

	videoPage[0] = new VideoMemory((uint32_t*)0xA0000000,480,272);
	//videoPage[1] = new VideoMemory((uint32_t*)0xA007F800,480,272);
	videoPage[1] = new VideoMemory((uint32_t*)0xA0080000,480,272);

	uint8_t currentPage = 0;

	LPC2478::getLCD()->setBaseAddress((uint32_t)(videoPage[0]->getPointer()));
	LPC2478::getLCD()->setBackground(0x00DEC3BD);
	LPC2478::getLCD()->setBaseAddress((uint32_t)(videoPage[1]->getPointer()));
	LPC2478::getLCD()->setBackground(0x00DEC3BD);

	// Start tick timer

	// Infinite game loop
	Debug::writeLine("Starting the update and render loop");
	while(1) {
		// Switch to the other video page
		currentPage ^= 1;

		environment->update();
		environment->render(videoPage[currentPage]);

		// Display the newly rendered page
		LPC2478::getLCD()->setBaseAddress((uint32_t)(videoPage[currentPage]->getPointer()));

		// 1/25s synchronization
		//LPC2478::delay(500000);
	}
}
