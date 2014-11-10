#pragma once

#include "Step.h"

Step::Step() :
	targetFrameDuration(0),
	time(0),
	deltaTime(0),
	deltaTimeCorrection(0),
	lastTimestamp(0),
	reverse(false)
{
}

Step::~Step()
{
}