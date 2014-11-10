#pragma once

#include "Command.h"
#include "Easing.h"

class Keyframe;

class CMD_EditKeyframe : public Command
{
public:
	CMD_EditKeyframe(std::vector<Keyframe> * _keyframes, Keyframe * _keyframe, float _startValue, float _value, Easing::Type _interpolation);
	~CMD_EditKeyframe();

	void execute();
	void unexecute();

private:
	Keyframe * keyframe;
	std::vector<Keyframe> * keyframes;

	float oldStartValue;
	float oldValue;
	Easing::Type oldInterpolation;

	float startValue;
	float value;
	Easing::Type interpolation;

	float followingStartValue;
	float nextDeltaTime;
	float nextDeltaValue;
	
};