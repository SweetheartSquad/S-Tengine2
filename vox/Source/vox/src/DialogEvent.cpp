#pragma once

#include "DialogEvent.h"
#include "SoundManager.h"

DialogEvent::DialogEvent():
	NodeUpdatable(),
	running(false),
	currentAction(0),
	soundManager(new SoundManager(-1))
{
	soundManager->addNewSound("1", "../assets/Voice1.ogg");
	soundManager->addNewSound("2", "../assets/Voice2.ogg");
	soundManager->addNewSound("3", "../assets/Voice3.ogg");
	soundManager->addNewSound("4", "../assets/Voice4.ogg");
	soundManager->addNewSound("5", "../assets/Voice5.ogg");
	soundManager->addNewSound("6", "../assets/Voice6.ogg");
}

DialogEvent::~DialogEvent(){
}

void DialogEvent::update(Step* _step){
	if(running){
		DialogAction * curAc = actions.at(currentAction);
		curAc->update(_step);
		if(curAc->isComplete()){
			if(currentAction + 1 < actions.size()){
				currentAction++;	
				int output = 1 + (rand() % (int)(6 - 1 + 1));
				soundManager->play(std::to_string(output));
			}
		}
	}
}

void DialogEvent::addAction(DialogAction * _action){
	actions.push_back(_action);
}