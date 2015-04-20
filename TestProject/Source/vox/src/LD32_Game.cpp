#pragma once

#include <LD32_Game.h>
#include <LD32_StartupScene.h>
#include <LD32_Scene.h>

LD32_Game::LD32_Game() :
	Game(true)
{
	currentSceneKey = "1";
	currentScene = new LD32_StartupScene(this);
	scenes.insert(std::pair<std::string, Scene * >(currentSceneKey, currentScene));
}

LD32_Game::~LD32_Game(){

}