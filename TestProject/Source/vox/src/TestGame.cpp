#pragma once

#include <TestGame.h>
#include <TestScene.h>

TestGame::TestGame() :
	Game(true)
{
	currentScene = new TestScene(this);
}

TestGame::~TestGame(){

}