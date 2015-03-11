#include "GameJamGame.h"
#include "RaidTheCastle.h"

#include "GameJamCharacter.h"
#include "GameJamSceneIndoor.h"
#include "GameJamSceneOutdoor.h"

#include <cstdlib>
#include <ctime>
 

GameJamGame::GameJamGame(bool _running):
	Game(_running)
{
	std::srand((unsigned long int)std::time(0));
	GameJamCharacter::init();
	scenes.insert(std::make_pair("indoors", new GameJamSceneIndoor(this)));
	scenes.insert(std::make_pair("outdoors", new GameJamSceneOutdoor(this)));
	currentScene = scenes.at("indoors");
}

GameJamGame::~GameJamGame(){
}

void GameJamGame::update(){
	Game::update();
}

void GameJamGame::draw(){
	Game::draw();
}