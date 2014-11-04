#pragma once

#include "Keyframe.h"

Keyframe::Keyframe(float _time, float _startValue, float _value, Easing::Type _interpolation) :
	time(_time), 
	startValue(_startValue), 
	value(_value), 
	interpolation(_interpolation)
{
}

Keyframe::~Keyframe(){
}