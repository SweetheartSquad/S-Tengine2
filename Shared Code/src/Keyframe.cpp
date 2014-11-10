#pragma once

#include "Keyframe.h"

Keyframe::Keyframe(float _deltaTime, float _deltaValue, Easing::Type _interpolation) :
	deltaTime(_deltaTime),  
	deltaValue(_deltaValue), 
	interpolation(_interpolation)
{
}

bool Keyframe::keyframe_compare(const Keyframe k1, const Keyframe k2){
	return(k1.time < k2.time);
}

Keyframe::~Keyframe(){
}