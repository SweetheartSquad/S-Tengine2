#pragma once

#include "CMD_AddKeyframe.h"

#include "CMD_EditKeyframe.h"
#include <algorithm>

CMD_AddKeyframe::CMD_AddKeyframe(std::vector<Keyframe> * _keyframes, float _time, float _value, Easing::Type _interpolation) :
	keyframes(_keyframes),
	keyframe(Keyframe(_time,0,_value,_interpolation))
{
	// Find insert iterator and set startValue of new keyframe
	previousKeyframe = std::lower_bound(keyframes->begin(),keyframes->end(),keyframe,Keyframe::keyframe_compare);
	if(keyframes->size() != 0){
		keyframe.startValue = previousKeyframe->value;
	}else{
		keyframe.startValue = _value;
	}

	// Create command to edit next keyframe's startValue
	std::vector<Keyframe>::iterator followingKeyframe = std::upper_bound(keyframes->begin(),keyframes->end(),keyframe,Keyframe::keyframe_compare);
	if(followingKeyframe != keyframes->end()){
		subCommands.push_back(new CMD_EditKeyframe(keyframes,&(*followingKeyframe),keyframe.value,followingKeyframe->value,followingKeyframe->interpolation));
	}
}

void CMD_AddKeyframe::execute(){
	// Add keyframe
	keyframes->insert(previousKeyframe,keyframes->begin(),keyframes->end());

	if(subCommands.size() != 0){
		// Change next keyframe's startvalue
		subCommands.at(0)->execute();
	}
}

void CMD_AddKeyframe::unexecute(){
	// Remove keyframe
	for(unsigned long int i = 0; i < keyframes->size(); ++i){
		if(keyframes->at(i).time == keyframe.time){
			keyframes->erase(keyframes->begin() + i);
			break;
		}
	}

	if(subCommands.size() != 0){
		// Restore next keyframe's startvalue
		subCommands.at(0)->unexecute();
	}
}

CMD_AddKeyframe::~CMD_AddKeyframe()
{
}