#pragma once

#include <LD32_Game.h>
#include <LD32_Scene.h>

LD32_Game::LD32_Game() :
	Game(true)
{
	currentScene = new LD32_Scene(this);
}

LD32_Game::~LD32_Game(){

}