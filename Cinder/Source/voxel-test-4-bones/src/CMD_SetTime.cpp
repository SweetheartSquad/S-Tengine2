#pragma once

#include "CMD_SetTime.h"
#include "Step.h"

CMD_SetTime::CMD_SetTime(float * _timelineTime, float _value, bool _relative) :
	timelineTime(_timelineTime),
	value(_value),
	relative(_relative)
{
}

void CMD_SetTime::execute(){
	Step s;
	s.setDeltaTime(relative ? value : (value - * timelineTime));

	//previousTime = UI::time;
}

void CMD_SetTime::unexecute(){

}

CMD_SetTime::~CMD_SetTime()
{
}