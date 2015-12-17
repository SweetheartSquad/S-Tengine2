#pragma once

#include <node\NodeUpdatable.h>
#include <node\NodeChild.h>

#include <functional>
#include <EventManager.h>

class Timeout : public virtual NodeChild, public virtual NodeUpdatable{
public:
	bool complete;
	bool active;
	float elapsedSeconds;
	float targetSeconds;
	// events for the timeout object: complete, progress, start
	sweet::EventManager * eventManager;

	Timeout(float _targetSeconds, std::function<void (sweet::Event * )> _onComplete);

	// calls stop and then calls start
	void restart();
	
	// sets active to true (triggers "start" event)
	virtual void start();

	// sets active to false
	void pause();

	// resets the timer to zero and sets complete and active to false
	void stop();

	// calls the onCompleteFunction, sets elapsedSeconds to targetSeconds, and complete to true
	// triggers "complete" event
	virtual void trigger();

	virtual void update(Step * _step) override;
};