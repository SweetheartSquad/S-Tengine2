#pragma once

#include "CMD_EditKeyframe.h"
#include "Keyframe.h"

CMD_EditKeyframe::CMD_EditKeyframe(Keyframe * _keyframe, float _value, Easing::Type _interpolation) :
	keyframe(_keyframe),
	value(_value),
	interpolation(_interpolation)
{
}

void CMD_EditKeyframe::execute(){
	keyframe->value = value;
	keyframe->interpolation = interpolation;
}

void CMD_EditKeyframe::unexecute(){
	keyframe->value = oldValue;
	keyframe->interpolation = oldInterpolation;
}

CMD_EditKeyframe::~CMD_EditKeyframe()
{
}