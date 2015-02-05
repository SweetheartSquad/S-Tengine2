#include "TestGame.h"
#include "PuppetTestScene.h"

#include "GameJamCharacter.h"

#include <cstdlib>
#include <ctime>
 

TestGame::TestGame(bool _running):
	Game(_running)
{
	std::srand(std::time(0));
	GameJamCharacter::init();
	scenes.insert(std::make_pair("Raid the Castle", new PuppetTestScene(this)));
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