#pragma once

#include "ListenAction.h"
#include "Character.h"

ListenAction::ListenAction(GameJamCharacter * _speaker):
	DialogAction(_speaker)
{
}

ListenAction::~ListenAction(){
}

DialogActionType ListenAction::getActionType(){
	return LISTEN;
}