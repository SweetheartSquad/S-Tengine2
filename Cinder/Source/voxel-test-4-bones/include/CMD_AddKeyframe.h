#pragma once

#include "Command.h"
#include <vector>
#include "Keyframe.h"

class CMD_AddKeyframe : public Command
{
public:
	CMD_AddKeyframe(std::vector<Keyframe> * _keyframes, float _time, float _value, Easing::Type _interpolation);
	~CMD_AddKeyframe();

	void execute();
	void unexecute();

private:

	Keyframe keyframe;
	std::vector<Keyframe> * keyframes;

	std::vector<Keyframe>::iterator previousKeyframe;
};