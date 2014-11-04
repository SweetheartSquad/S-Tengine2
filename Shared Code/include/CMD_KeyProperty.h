#pragma once

#include "Command.h"
#include "Easing.h"
#include <vector>

#include "Keyframe.h"
#include "Animation.h"

class CMD_KeyProperty : public Command
{
public:
	CMD_KeyProperty(Animation * _animation, float _time, float _startValue, float _value, Easing::Type _interpolation);
	~CMD_KeyProperty();

	void execute();
	void unexecute();

private:
	Keyframe keyframe;
	Animation * animation;
	
};