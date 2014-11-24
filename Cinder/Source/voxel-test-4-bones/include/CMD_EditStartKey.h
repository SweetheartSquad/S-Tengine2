#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"

class CMD_EditStartKey : public Command
{
public:
	/**
	* Edits the start of the specified animation
	*
	* @param _animation		The animation object to modify
	* @param _value			The target value at the start of the animation
	*/
	CMD_EditStartKey(Animation * _animation, float _targetValue);
	~CMD_EditStartKey();

	void execute();
	void unexecute();

private:
	Animation * animation;
	float targetValue;

	float oldStartValue;
};