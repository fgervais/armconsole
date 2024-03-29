/*
 * Level1.cpp
 *
 *  Created on: Mar 11, 2010
 *      Author: fgervais
 */

#include "Level1.h"
#include "Tile.h"
#include "Bitmap.h"
#include "VisibleArea.h"
#include "Debug.h"
#include "Background.h"
#include "Physics.h"
#include "Sprite.h"
#include "Metool.h"
#include "Megaman.h"
#include "MegamanState.h"
#include "MegamanStandingRight.h"
#include "MegamanStandingLeft.h"
#include "MegamanRunningLeft.h"
#include "MegamanRunningRight.h"
#include "MegamanJumpingLeft.h"
#include "MegamanJumpingRight.h"
#include "MetoolWalkingLeft.h"
#include "MetoolWalkingRight.h"

#include "Wave.h"
#include "Audio.h"

Level1::Level1()
	: Environment(960, 272, 56, 32) {

}

Level1::~Level1() {

}

void Level1::build() {
	Debug::writeLine("Entering Level1 build function");

	//Bitmap** black_square = new Bitmap*[2];
	//black_square[0] = new Bitmap("0:black.bmp");
	//black_square[1] = new Bitmap("0:gray.bmp");

	Bitmap** snow = new Bitmap*[1];
	snow[0] = new Bitmap("0:snow1.bmp");

	Tile* tile1 = new Tile(56, 32, snow, 1, this);

	Bitmap** brick = new Bitmap*[1];
	brick[0] = new Bitmap("0:brick.bmp");

	Tile* tile2 = new Tile(56, 32, brick, 1, this);

	// Send the tiles to the environment (Base class)
	//for(uint8_t i=0; i<18; i++) {
	for(uint8_t i=0; i<15; i++) {
		add(tile1, i, 7);
	}

	//for(uint8_t i=0; i<18; i++) {
	for(uint8_t i=0; i<15; i++) {
		add(tile2, i, 8);
	}

	// A little hill
	add(tile1, 13, 6);

	// Create a new visible area for the entire screen
	VisibleArea* visibleArea = new VisibleArea();
	visibleArea->x = 0;
	visibleArea->y = 0;
	visibleArea->width = 480;
	visibleArea->height = 272;

	// Send it to the environment (Base class)
	set(visibleArea);

	Bitmap* background_bitmap = new Bitmap("0:back4.bmp");
	Background* background = new Background(background_bitmap, 768, 272, this);

	set(background);

	// Start with a power of 2 gravitation acceleration since it's easy to play with
	// ad 8 is close to earth acceleration anyway (which is ~9.81)
	Physics* physics = new Physics(1);
	set(physics);

	// Hero section
	Debug::writeLine("Loading hero");

	// Preload every states. Maybe something better could be used here.
	MegamanStandingLeft::getInstance();
	MegamanStandingRight::getInstance();
	MegamanRunningLeft::getInstance();
	MegamanRunningRight::getInstance();
	MegamanJumpingLeft::getInstance();
	MegamanJumpingRight::getInstance();

	Megaman* hero = new Megaman(MegamanStandingLeft::getInstance(), this);
	hero->enableCollisionCheck();
	hero->setPosition(720, 50);
	//set(hero, 240, 189);
	//set(hero, 240, 50);
	set(hero);
	Debug::writeLine("Done Loading hero");

	Debug::writeLine("Loading sprites");
	Metool** enemy = new Metool*[32];
	for(uint8_t i=0; i<32; i++) {
		if(i<22) {
			enemy[i] = new Metool(MetoolWalkingRight::getInstance(), this);
			//add(enemy, 500, 204);
		}
		else {
			enemy[i] = new Metool(MetoolWalkingLeft::getInstance(), this);
		}
		startSpawning(enemy[i], (i*25), 100);
	}

	// Try a sprite activated directly without being spawned by the environment
	Metool* lonely_metool = new Metool(MetoolWalkingRight::getInstance(), this);
	lonely_metool->setPosition(600, 50);
	activate(lonely_metool);

	Debug::writeLine("Loading background sound");
	Wave* intro = new Wave("0:sound/intro.wav");
	intro->load();
	Audio::playAsync(intro);
}
