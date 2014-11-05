#pragma once

#include "CMD_AddKeyframe.h"

#include <algorithm>

CMD_AddKeyframe::CMD_AddKeyframe(std::vector<Keyframe> * _keyframes, float _time, float _value, Easing::Type _interpolation) :
	keyframes(_keyframes),
	keyframe(Keyframe(_time,0,_value,_interpolation))
{
	//find insert iterator and set startValue of keyframe
	previousKeyframe = std::lower_bound(keyframes->begin(),keyframes->end(),keyframe,Keyframe::keyframe_less_than);
	if(keyframes->size() != 0){
		keyframe.startValue = previousKeyframe->value;
	}else{
		keyframe.startValue = _value;
	}
}

void CMD_AddKeyframe::execute(){
	keyframes->insert(previousKeyframe,keyframes->begin(),keyframes->end());
}

void CMD_AddKeyframe::unexecute(){
	// Remove created joint from joint list
	for(unsigned long int i = 0; i < keyframes->size(); ++i){
		if(keyframes->at(i).time == keyframe.time){
			keyframes->erase(keyframes->begin() + i);
			break;
		}
	}
}

CMD_AddKeyframe::~CMD_AddKeyframe()
{
}