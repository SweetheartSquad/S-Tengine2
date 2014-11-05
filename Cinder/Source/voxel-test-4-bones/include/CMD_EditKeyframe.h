#pragma once

#include "Command.h"
#include "Easing.h"

class Keyframe;

class CMD_EditKeyframe : public Command
{
public:
	CMD_EditKeyframe(Keyframe * _keyframe, float _value, Easing::Type _interpolation);
	~CMD_EditKeyframe();

	void execute();
	void unexecute();

private:
	Keyframe * keyframe;

	float oldValue;
	Easing::Type oldInterpolation;

	float value;
	Easing::Type interpolation;

};