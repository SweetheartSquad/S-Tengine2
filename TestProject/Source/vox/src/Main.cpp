#pragma once

#include <Vox.h>
#include <node\Node.h>
#include <typeinfo>
#include <Texture.h>
#include <Game.h>
#include <LD32_ResourceManager.h>

#include <LD32_Game.h>

int main(void){
	vox::initialize("Vox");
	LD32_ResourceManager::init();
	LD32_ResourceManager::load();
	LD32_Game * game = new LD32_Game();

	while (game->isRunning){
		game->performGameLoop();
	}
	
	delete game;
	game = nullptr;
	LD32_ResourceManager::destruct();
#ifdef _DEBUG
	std::cout << "Final node count: " << Node::nodes.size() << std::endl;

	for(auto n : Node::nodes){
		std::cout << typeid(*n).name() << std::endl;
	}
#endif

	vox::destruct();
}