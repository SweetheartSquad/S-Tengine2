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

bool CMD_SetTime::execute(){
	if(timelineTime != nullptr){
		if(firstRun){
			oldValue = *timelineTime;
		}
		if (relative){
			*timelineTime += newValue;
		}else{
			*timelineTime = newValue;
		}
	}else{
		error("null timeline");
		return false;
	}
	return true;
}

bool CMD_SetTime::unexecute(){
	if(timelineTime != nullptr){
		*timelineTime = oldValue;
		ci::app::console() << "restored timelineTime: " << *timelineTime << std::endl;
	}else{
		error("null timeline");
		return false;
	}
	return true;
}

CMD_SetTime::~CMD_SetTime(){
}