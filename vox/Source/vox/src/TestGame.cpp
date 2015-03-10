#include "TestGame.h"
#include "RaidTheCastle.h"

#include "GameJamCharacter.h"
#include "GameJamSceneIndoor.h"

#include <cstdlib>
#include <ctime>
#include <BaseScene.h>


TestGame::TestGame(bool _running):
	Game(_running)
{
	std::srand((unsigned long int)std::time(0));
	GameJamCharacter::init();
	scenes.insert(std::make_pair("Raid the Castle", new BaseScene(this)));
	//((GameJamSceneIndoor *)scenes.at("Raid the Castle"))->debugDraw = true;
	currentScene = scenes.at("Raid the Castle");
}

TestGame::~TestGame(){
}

void TestGame::update(){
	Game::update();
}

void TestGame::draw(){
	Game::draw();
}