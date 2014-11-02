#pragma once

#include "Easing.h"

class Keyframe{
public:
	Keyframe();
	~Keyframe();

	float time;
	float startValue;
	float value;
	Easing::Type interpolation;

private:

};