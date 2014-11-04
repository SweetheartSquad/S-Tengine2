#pragma once

#include "CMD_KeyProperty.h"

CMD_KeyProperty::CMD_KeyProperty(Animation * _animation, float _time, float _startValue, float _value, Easing::Type _interpolation) :
	animation(_animation),
	keyframe(Keyframe(_time,_startValue,_value,_interpolation))
{
}

void CMD_KeyProperty::execute(){
	animation->keyframes.push_back(keyframe);
}

void CMD_KeyProperty::unexecute(){
	animation->keyframes.pop_back();
}

CMD_KeyProperty::~CMD_KeyProperty(){
}