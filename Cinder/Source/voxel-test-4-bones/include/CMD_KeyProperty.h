#pragma once

#include "Command.h"
#include "Easing.h"
#include <vector>

class Tween;
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

	int findKeyframe(std::vector<Tween *> * _tweens);

	float getStartValue(int _idx);
	float getEndValue(int _idx);
};