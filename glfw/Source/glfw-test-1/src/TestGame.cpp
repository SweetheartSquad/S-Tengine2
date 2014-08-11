#include "TestGame.h"
#include "MainScene.h"

TestGame::TestGame(bool running):Game(running)
{
	scenes->insert(std::make_pair("main_scene", new MainScene()));
	currentScene = scenes->at("main_scene");
}

TestGame::~TestGame()
{
}

void TestGame::update()
{
	Game::update();
}

void TestGame::draw()
{
	Game::draw();
}