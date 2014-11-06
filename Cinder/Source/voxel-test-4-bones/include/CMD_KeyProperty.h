#pragma once

#include "Command.h"
#include "Easing.h"
#include <vector>

class Keyframe;
class Animation;

class CMD_KeyProperty : public Command
{
public:
	CMD_KeyProperty(Animation * _animation, float _time, float _value, Easing::Type _interpolation);
	~CMD_KeyProperty();

	void execute();
	void unexecute();

private:

	Animation * animation;
	float time;
	float value;
	Easing::Type interpolation;

	Keyframe * findKeyframe(std::vector<Keyframe> * _keyframes);
};