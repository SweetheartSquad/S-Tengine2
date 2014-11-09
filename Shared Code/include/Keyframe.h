#pragma once

#include "Easing.h"

class Keyframe{
public:
	Keyframe(float _time, float _startValue, float _value, Easing::Type _interpolation);
	~Keyframe();

	float time;
	float startValue;
	float value;
	Easing::Type interpolation;

	static bool keyframe_compare(const Keyframe k1, const Keyframe k2);

private:

};