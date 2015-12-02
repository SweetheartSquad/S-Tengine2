#pragma once

#include <Timeout.h>

#include <Step.h>

Timeout::Timeout(float _targetSeconds, std::function<void (sweet::Event * )> _onComplete) :
	eventManager(new sweet::EventManager()),
	targetSeconds(_targetSeconds),
	elapsedSeconds(0),
	complete(false),
	active(false)
{
	eventManager->addEventListener("complete", _onComplete);
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
	eventManager->update(_step);
	if(!complete && active){
		elapsedSeconds += _step->deltaTime;

		// trigger a progress event
		sweet::Event * progressEvent = new sweet::Event("progress");
		progressEvent->setFloatData("progress", std::min(1.f, elapsedSeconds/targetSeconds));
		eventManager->triggerEvent(progressEvent);

		// trigger a complete event
		if(elapsedSeconds >= targetSeconds){
			trigger();
		}
	}
}

void Timeout::trigger(){
	complete = true;
	elapsedSeconds = targetSeconds;
	eventManager->triggerEvent("complete");
	// maybe set active to false here too?
}