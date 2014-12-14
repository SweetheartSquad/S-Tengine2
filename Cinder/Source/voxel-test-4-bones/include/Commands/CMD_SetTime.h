#pragma once

#include "Command.h"

class CMD_SetTime : public Command{
public:
	CMD_SetTime(float * _timelineTime, float _value, bool _relative);
	~CMD_SetTime();

	bool execute();
	bool unexecute();

private:
	float * timelineTime;
	float newValue;
	float oldValue;
	bool relative;
};