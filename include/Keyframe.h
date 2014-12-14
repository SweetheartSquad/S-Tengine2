#pragma once

#include "Easing.h"

class Keyframe{
public:
	Keyframe(float deltaTime, float deltaValue, Easing::Type _interpolation);
	~Keyframe();

	float deltaTime;
	float deltaValue;

	float time;
	float startValue;
	float value;
	Easing::Type interpolation;

	static bool keyframe_compare(const Keyframe k1, const Keyframe k2);

private:

};