#include "PuppetGame.h"
#include "RaidTheCastle.h"

#include "GameJamCharacter.h"
#include "GameJamSceneIndoor.h"
#include "GameJamSceneOutdoor.h"

#include <cstdlib>
#include <ctime>
 

PuppetGame::PuppetGame(bool _running):
	Game(_running)
{
	std::srand((unsigned long int)std::time(0));
	GameJamCharacter::init();
	//scenes.insert(std::make_pair("indoors", new GameJamSceneIndoor(this)));
	//scenes.insert(std::make_pair("outdoors", new GameJamSceneOutdoor(this)));
	scenes.insert(std::make_pair("Raid the Castle", new RaidTheCastle(this)));
	//((GameJamSceneIndoor *)scenes.at("Raid the Castle"))->debugDraw = true;
	//currentScene = scenes.at("indoors");
	currentScene = scenes.at("Raid the Castle");
}

PuppetGame::~PuppetGame(){
}

void PuppetGame::update(){
	Game::update();
}

void PuppetGame::draw(){
	Game::draw();
}