#pragma once

#include "CMD_SetTime.h"
#include "Step.h"

#include "cinder/app/AppBasic.h"

CMD_SetTime::CMD_SetTime(float * _timelineTime, float _value, bool _relative) :
	timelineTime(_timelineTime),
	newValue(_value),
	relative(_relative)
{
}

void CMD_SetTime::execute(){
	if(firstRun){
		oldValue = *timelineTime;
	}
	if (relative){
		* timelineTime += newValue;
	}else{
		* timelineTime = newValue;
	}
}

void CMD_SetTime::unexecute(){
	*timelineTime = oldValue;
	ci::app::console() << "restored timelineTime: " << *timelineTime << std::endl;
}

CMD_SetTime::~CMD_SetTime()
{
}