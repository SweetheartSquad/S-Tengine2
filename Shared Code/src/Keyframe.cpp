#pragma once

#include "Keyframe.h"

Keyframe::Keyframe(float _time, float _startValue, float _value, Easing::Type _interpolation) :
	time(_time), 
	startValue(_startValue), 
	value(_value), 
	interpolation(_interpolation)
{
}

bool Keyframe::keyframe_less_than(const Keyframe k1, const Keyframe k2){
	return(k1.time < k2.time);
}

Keyframe::~Keyframe(){
}