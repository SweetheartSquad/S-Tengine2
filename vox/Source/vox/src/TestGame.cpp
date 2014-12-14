#include "TestGame.h"
#include "BaseScene.h"
#include <string>

TestGame::TestGame(bool _running):
	Game(_running)
{
	scenes.insert(std::make_pair("base_scene", new BaseScene(this)));
	currentScene = scenes.at("base_scene");
}

TestGame::~TestGame(){
}

void TestGame::update(){
	Game::update();
}

void TestGame::draw(){
	Game::draw();
}