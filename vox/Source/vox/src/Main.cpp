#pragma once

#include "PuppetGame.h"
#include "GameJamGame.h"
#include "Vox.h"

Game * game;

int main(void){
	vox::initialize("Vox");
	
	game = new PuppetGame(true);
	//game = new GameJamGame(true);

	while (game->isRunning){
		game->performGameLoop();
	}

	vox::destruct();
}