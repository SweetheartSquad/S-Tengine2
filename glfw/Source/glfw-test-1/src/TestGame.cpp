#include "TestGame.h"
#include "MainScene.h"
#include "BaseScene.h"

TestGame::TestGame(bool _running):
	Game(_running)
{
	scenes.insert(std::make_pair("main_scene", new MainScene(this)));
	//scenes.insert(std::make_pair("base_scene", new BaseScene(this)));
	currentScene = scenes.at("main_scene");
}

TestGame::~TestGame(){
}

void TestGame::update(){
	Game::update();
}

void TestGame::draw(){
	Game::draw();
}