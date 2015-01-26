#pragma once

#include "DialogHandler.h"
#include "DialogEvent.h"
#include "Transform.h"
#include "BitmapFont.h"

DialogHandler::DialogHandler(std::vector<Character *> _sceneCharacters):
	NodeUpdatable(),
	sceneCharacters(_sceneCharacters),
	dialogEvent(new DialogEvent())
{
	
}

DialogHandler::~DialogHandler(){
}

void DialogHandler::makeDialog(){
	int charVecSize = sceneCharacters.size();
	int randDialog = 0;
	for(unsigned long int i = 0; i < 50; i++) {
		for(unsigned long int i = 0; i < charVecSize; i++){
			randDialog = rand() % 29;
			dialogEvent->addAction(new SayAction(sceneCharacters[i], dialogBits[randDialog], dialogBits[randDialog].length()/4));
		}
	}
	dialogEvent->running = true;
}

void DialogHandler::update(Step* _step){
	dialogEvent->update(_step);
}