#pragma once

#include "CMD_EditKeyframe.h"
#include "Keyframe.h"
#include <algorithm>

CMD_EditKeyframe::CMD_EditKeyframe(std::vector<Keyframe> * _keyframes, Keyframe * _keyframe, float _startValue, float _value, Easing::Type _interpolation) :
	keyframes(_keyframes),
	keyframe(_keyframe),
	startValue(_startValue),
	value(_value),
	interpolation(_interpolation)
{
	// Create command to edit next keyframe's startValue, if this one's is changing
	if(value != oldValue){
		std::vector<Keyframe>::iterator followingKeyframe = std::upper_bound(keyframes->begin(),keyframes->end(),*keyframe,Keyframe::keyframe_compare);
		if(followingKeyframe != keyframes->end()){
			subCommands.push_back(new CMD_EditKeyframe(keyframes,&(*followingKeyframe),value,followingKeyframe->value,followingKeyframe->interpolation));
		}
	}
}

void CMD_EditKeyframe::execute(){
	keyframe->startValue = startValue;
	keyframe->value = value;
	keyframe->interpolation = interpolation;

	if(subCommands.size() != 0){
		// Change next keyframe's startvalue if value was changed
		subCommands.at(0)->execute();
	}
}

void CMD_EditKeyframe::unexecute(){
	keyframe->startValue = oldStartValue;

	if(subCommands.size() != 0){
		// Restore next keyframe's startvalue if value was changed
		subCommands.at(0)->unexecute();
	}
	keyframe->value = oldValue;
	keyframe->interpolation = oldInterpolation;
}

CMD_EditKeyframe::~CMD_EditKeyframe()
{
}