#pragma once

#include "CMD_KeyProperty.h"
#include "CMD_AddKeyframe.h"
#include "CMD_EditKeyframe.h"
#include "Keyframe.h"
#include "Animation.h"
#include <algorithm>

CMD_KeyProperty::CMD_KeyProperty(Animation * _animation, float _time, float _value, Easing::Type _interpolation) :
	animation(_animation),
	time(_time),
	value(_value),
	interpolation(_interpolation)
{
	Keyframe * k = findKeyframe(&animation->keyframes);

	if(k != nullptr){
		subCommands.push_back(new CMD_AddKeyframe(&animation->keyframes, time, value, interpolation));
	}else{
		subCommands.push_back(new CMD_EditKeyframe(&animation->keyframes, k, k->startValue, value, interpolation));
	}
}

void CMD_KeyProperty::execute(){
	subCommands.at(0)->execute();
}

void CMD_KeyProperty::unexecute(){
	subCommands.at(0)->unexecute();
}

Keyframe * CMD_KeyProperty::findKeyframe(std::vector<Keyframe> * _keyframes){
	Keyframe * k = nullptr;
	for(unsigned long int i = 0; i < _keyframes->size(); ++i){
		if(_keyframes->at(i).time == time){
			k = &_keyframes->at(i);
		}
	}
	return k;
}

CMD_KeyProperty::~CMD_KeyProperty(){
}