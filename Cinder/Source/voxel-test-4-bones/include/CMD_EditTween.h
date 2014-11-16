#pragma once

#include "Command.h"
#include "Easing.h"

class Tween;
class Animation;

class CMD_EditTween : public Command
{
public:
	CMD_EditTween(Animation * _animation, float _value, Easing::Type _interpolation, int _idx);
	~CMD_EditTween();

	void execute();
	void unexecute();

private:
	// If keying time 0
	Animation * animation;
	float startValue;
	float oldStartValue;

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