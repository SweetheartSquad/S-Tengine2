#pragma once

#include "Step.h"

Step::Step() :
	targetFrameDuration(0),
	time(0),
	deltaTime(0),
	deltaTimeCorrection(0),
	lastTimestamp(0),
	reverse(false),
	cycles(0)
{
}

Step::~Step()
{
}

void Step::setDeltaTime(double _deltaTime){
	deltaTime = _deltaTime;
	if(deltaTime >= 0){
		reverse = false;
	}else{
		reverse = true;
	}
}

double Step::getDeltaTime(){
	return deltaTime;
}

bool Step::getReverse(){
	return reverse;
}