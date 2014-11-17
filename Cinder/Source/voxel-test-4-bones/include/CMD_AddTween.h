#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"

class CMD_AddTween : public Command
{
public:
	CMD_AddTween(Animation * _animation, float _time, float _value, Easing::Type _interpolation);
	~CMD_AddTween();

	void execute();
	void unexecute();

private:

	// If keying time 0
	Animation * animation;
	float startValue;
	float oldStartValue;
	// Location of the keyframe on the timeline
	float time;
	// Value of the property at the given time
	float value;
	// Interpolation type for the tween
	Easing::Type interpolation;

	Tween * tween;

	bool executed;

	/*float deltaTime;
	float deltaValue;
	Easing::Type interpolation;*/

	int nextTweenIdx; // -1 if END
	float nextTween_oldDeltaTime;
	float nextTween_oldDeltaValue;

	float nextTween_newDeltaTime;
	float nextTween_newDeltaValue;
	
	int getNextTween(float _time);
	float getTweenEndTime(int _idx);
	float getTweenEndValue(int _idx, float _startValue);

};