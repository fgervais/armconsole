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
#include "Enemy.h"
#include "Hero.h"
#include "HeroState.h"
#include "HeroStandingRight.h"
#include "HeroStandingLeft.h"
#include "HeroRunningLeft.h"
#include "HeroRunningRight.h"
#include "HeroJumpingLeft.h"
#include "HeroJumpingRight.h"
#include "EnemyWalkingLeft.h"
#include "EnemyWalkingRight.h"

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
	HeroStandingLeft::getInstance();
	HeroStandingRight::getInstance();
	HeroRunningLeft::getInstance();
	HeroRunningRight::getInstance();
	HeroJumpingLeft::getInstance();
	HeroJumpingRight::getInstance();

	Hero* hero = new Hero(HeroStandingLeft::getInstance(), this);
	//set(hero, 240, 189);
	//set(hero, 240, 50);
	set(hero, 720, 50);
	Debug::writeLine("Done Loading hero");

	Debug::writeLine("Loading sprites");
	Enemy** enemy = new Enemy*[32];
	for(uint8_t i=0; i<32; i++) {
		if(i<22) {
			enemy[i] = new Enemy(EnemyWalkingRight::getInstance(), this);
			//add(enemy, 500, 204);
		}
		else {
			enemy[i] = new Enemy(EnemyWalkingLeft::getInstance(), this);
		}
		add(enemy[i], (i*25), 100);
	}
}
