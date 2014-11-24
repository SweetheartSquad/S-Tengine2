#pragma once

#include "Command.h"
#include "Easing.h"

class Tween;
class Animation;

class CMD_EditTween : public Command
{
public:
	CMD_EditTween(Animation * _animation, float _targetValue, Easing::Type _interpolation, int _idx);
	~CMD_EditTween();

	void execute();
	void unexecute();

private:
	// Used in getTweenEndValue function
	Animation * animation;

	// New value of the property at the given time, used to calculate new deltaValue
	float targetValue;
	// New interpolation type for the tween
	Easing::Type interpolation;

	// New deltaValue to be set in the tween
	float newDeltaValue;
	// Old deltaValue set in the tween
	float oldDeltaValue;
	// Old interpolation type of the tween
	Easing::Type oldInterpolation;

	// The index of the tween
	int idx;
	Tween * tween;

	// Tween after this tween in animation, will be updated if this tween's deltaValue changes
	unsigned long int nextTweenIndex; // -1 if none
	float nextTween_oldDeltaValue;
	float nextTween_newDeltaValue;

	bool executed;
};