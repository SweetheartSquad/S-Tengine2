#pragma once

#include "Command.h"
#include "Easing.h"

class Tween;
class Animation;

class CMD_EditKeyframe : public Command
{
public:
	CMD_EditKeyframe(Animation * _animation, float _value, Easing::Type _interpolation, int _idx);
	~CMD_EditKeyframe();

	void execute();
	void unexecute();

private:

	Tween * tween;
	std::vector<Tween> * tweens;
	
	// ??? float oldDeltaTime;
	float oldDeltaValue;
	Easing::Type oldInterpolation;

	// ??? float deltaTime;
	float deltaValue;
	Easing::Type interpolation;

	int nextTweenIdx; // -1 if none
	//float nextTween_oldDeltaTime;
	float nextTween_oldDeltaValue;

	//float nextTween_newDeltaTime;
	float nextTween_newDeltaValue;
	
	int getNextTween(int _idx);
	float getTweenEndTime(int _idx);
	float getTweenEndValue(int _idx, float _startValue);
	
};