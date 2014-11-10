#pragma once

#include "Easing.h"

class Tween
{
public:
	Tween(float deltaTime, float deltaValue, Easing::Type _interpolation);
	~Tween();

	float deltaTime;
	float deltaValue;

	Easing::Type interpolation;


private:

};