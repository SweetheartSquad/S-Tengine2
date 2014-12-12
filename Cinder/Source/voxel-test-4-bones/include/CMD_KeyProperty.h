#pragma once

#include "Command.h"
#include "Easing.h"
#include <vector>

class Tween;
class Animation;

class CMD_KeyProperty : public Command
{
public:
	/**
	* Adds or edits a keyframe for the specified property
	*
	* @param _animation		The animation object to modify
	* @param _currentTime	The current time in the scene
	* @param _targetTime	The target time to key
	* @param _value			The target value to key
	* @param _interpolation	The interpolation type to key
	*/
	CMD_KeyProperty(Animation * _animation, float _currentTime, float _targetTime, float _value, Easing::Type _interpolation);
	~CMD_KeyProperty();

	bool execute();
	bool unexecute();

private:

	Animation * animation;
	float currentTime;
	float targetTime;
	float value;
	Easing::Type interpolation;

	float oldStartValue;
	bool oldHasStart;

	int findKeyframe(std::vector<Tween *> * _tweens);

	float getStartValue(unsigned long int _idx);
	float getEndValue(unsigned long int _idx);
};