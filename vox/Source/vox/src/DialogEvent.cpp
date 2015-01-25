#pragma once

#include "DialogEvent.h"

DialogEvent::DialogEvent():
	NodeUpdatable(),
	running(false),
	currentAction(0)
{
}

DialogEvent::~DialogEvent(){
}

void DialogEvent::update(Step* _step){
	if(running){
		DialogAction * curAc = actions.at(currentAction);
		curAc->update(_step);
		if(curAc->isComplete()){
			if(currentAction + 1 < actions.size()){
				currentAction ++;	
			}
		}
	}
}

void DialogEvent::addAction(DialogAction * _action){
	actions.push_back(_action);
}