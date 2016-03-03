#pragma once

#include <Timeout.h>

#include <Step.h>

Timeout::Timeout(double _targetSeconds, std::function<void (sweet::Event * )> _onComplete) :
	eventManager(new sweet::EventManager()),
	targetSeconds(_targetSeconds),
	elapsedSeconds(0),
	complete(false),
	active(false)
{
	eventManager->addEventListener("complete", _onComplete);
}

Timeout::~Timeout(){
	delete eventManager;
}

void Timeout::start(){
	if(!active){
		eventManager->triggerEvent("start");
		active = true;
	}
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
		// if there are no listeners/parents, skip over this
		if(eventManager->listeners["progress"].size() > 0 || eventManager->hasParentManagers()){
			sweet::Event * progressEvent = new sweet::Event("progress");
			progressEvent->setFloatData("progress", std::min(1.f, (float)(elapsedSeconds/targetSeconds)));
			eventManager->triggerEvent(progressEvent);
		}

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