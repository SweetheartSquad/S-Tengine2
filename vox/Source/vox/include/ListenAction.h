#pragma once

#include "DialogAction.h"

class GameJamCharacter;

class ListenAction : public DialogAction{
public:
	ListenAction(GameJamCharacter * _speaker);
	~ListenAction();

	DialogActionType getActionType() override;
};