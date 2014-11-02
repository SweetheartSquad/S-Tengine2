#pragma once

#include "Easing.h"

class Animation
{
public:
	Animation(float * _prop);
	~Animation();

	Easing::Type interpolation;
	float start;
	float duration;
	float * prop;

	void update();


private:

};