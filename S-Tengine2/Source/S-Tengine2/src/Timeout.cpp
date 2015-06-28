#pragma once

#include <Timeout.h>

#include <Step.h>

Timeout::Timeout(float _targetSeconds) :
	onCompleteFunction(nullptr),
	targetSeconds(_targetSeconds),
	elapsedSeconds(0),
	complete(false),
	active(false)
{
}

void Timeout::start(){
	active = true;
}

void Timeout::stop(){
	active = false;
	elapsedSeconds = 0;
	complete = false;
}

void Timeout::pause(){
	active = false;
}

void Timeout::restart(){
	stop();
	start();
}

void Timeout::update(Step * _step){
	if(!complete && active){
		elapsedSeconds += _step->deltaTime;
		if(elapsedSeconds >= targetSeconds){
			trigger();
		}
	}
}

void Timeout::trigger(){
	complete = true;
	elapsedSeconds = targetSeconds;
	if(onCompleteFunction != nullptr){
		onCompleteFunction(this);
	}
	// maybe set active to false here too?
}