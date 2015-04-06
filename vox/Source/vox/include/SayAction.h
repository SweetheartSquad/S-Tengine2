#pragma once

#include "DialogAction.h"

#include <string>

class SayAction : public DialogAction{
public:
	std::string text;
	float duration;

	float timePassed;
	bool setText;
	bool setBlank;

	SayAction(GameJamCharacter * _speaker, std::string _text, float _duration);
	~SayAction();

	DialogActionType getActionType() override;
	virtual void update(Step* _step) override;
};