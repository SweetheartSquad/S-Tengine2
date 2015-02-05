#pragma once

#include "DialogAction.h"

DialogAction::DialogAction(GameJamCharacter * speaker):
	speaker(speaker),
	complete(false)
{
}

DialogAction::~DialogAction(){
}

DialogActionType DialogAction::getActionType(){
	return NONE;
}

void DialogAction::update(Step* _step){
}

bool DialogAction::isComplete(){
	return complete;
}