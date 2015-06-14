#pragma once

#include <Timeout.h>

#include <Step.h>

Timeout::Timeout(float _targetSeconds) :
	onCompleteFunction(nullptr),
	targetSeconds(_targetSeconds),
	elapsedSeconds(0),
	complete(false)
{
}

void Timeout::restart(){
	elapsedSeconds = 0;
	complete = false;
}

void Timeout::update(Step * _step){
	if(!complete){
		elapsedSeconds += _step->deltaTime;
		if(elapsedSeconds > targetSeconds){
			complete = true;
			if(onCompleteFunction != nullptr){
				onCompleteFunction(this);
			}
		}
	}
}