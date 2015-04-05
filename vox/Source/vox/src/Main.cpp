#pragma once

#include "PuppetGame.h"
#include "GameJamGame.h"
#include <Vox.h>
#include <node\Node.h>
#include <typeinfo>

int main(void){
	vox::initialize("Vox");
	
	Game * game = new PuppetGame(true);
	//game = new GameJamGame(true);

	while (game->isRunning){
		game->performGameLoop();
	}
	
	delete game;
	game = nullptr;
	ResourceManager::destruct();
	std::cout << "Final node count: " << Node::count << std::endl;

	for(auto n : Node::nodes){
		std::cout << typeid(*n).name() << std::endl;
	}

	vox::destruct();
}