#pragma once

#include "ListenAction.h"
#include "Character.h"

ListenAction::ListenAction(Character * _speaker):
	DialogAction(_speaker)
{
}

ListenAction::~ListenAction(){
}

DialogActionType ListenAction::getActionType(){
	return LISTEN;
}