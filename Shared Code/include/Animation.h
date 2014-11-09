#pragma once

#include "Easing.h"
#include "Keyframe.h"
#include <vector>

class Animation
{
public:
	explicit Animation(float * _prop);
	~Animation();

	float * prop;

	std::vector<Keyframe> keyframes;

	void update();


private:

};