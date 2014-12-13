#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"

class CMD_AddTweenAfter : public Command
{
public:
	/**
	* Adds a tween to the specified animation
	*
	* @param _animation		The animation object to modify
	* @param _currentTime	The current time in the scene
	* @param _targetTime	The target time at the end of the added tween
	* @param _value			The target value at the end of the added tween
	* @param _interpolation	The interpolation type for the tween
	*/
	CMD_AddTweenAfter(Animation * _animation, float _deltaTimeline, float _targetValue, Easing::Type _interpolation, float _sumTime);
	~CMD_AddTweenAfter();

	bool execute();
	bool unexecute();

private:
	// Used in getTweenEndValue
	Animation * animation;

	// Time between the current time on the timeline and the target end-time for the tween
	float deltaTimeline;
	// Value of the property at the given time
	float targetValue;
	// Interpolation type for the tween
	Easing::Type interpolation;

	Tween * tween;

	float sumTime;
};