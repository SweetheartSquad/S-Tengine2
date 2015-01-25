#pragma once

#include "DialogAction.h"

DialogAction::DialogAction(Character * spreaker):
	spreaker(spreaker),
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