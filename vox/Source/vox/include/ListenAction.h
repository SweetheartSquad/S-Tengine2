#pragma once

#include "DialogAction.h"

class Character;

class ListenAction : public DialogAction{
public:
	ListenAction(Character * _speaker);
	~ListenAction();

	DialogActionType getActionType() override;
};