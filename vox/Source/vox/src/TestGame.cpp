#include "TestGame.h"
#include "BaseScene.h"
#include <string>
#include "TestScene2D.h"

TestGame::TestGame(bool _running):
	Game(_running)
{
	//scenes.insert(std::make_pair("base_scene", new BaseScene(this)));
	scenes.insert(std::make_pair("scene_2d", new TestScene2D(this)));
	currentScene = scenes.at("scene_2d");
}

TestGame::~TestGame(){
}

void TestGame::update(){
	Game::update();
}

void TestGame::draw(){
	Game::draw();
}