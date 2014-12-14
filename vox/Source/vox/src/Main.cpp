#pragma once

#include "TestGame.h"
#include "Vox.h"

TestGame * game;

int main(void){

	vox::initialize("Vox");

	game = new TestGame(true);

	while (game->isRunning){
		game->performGameLoop();
	}

	vox::destruct();
}