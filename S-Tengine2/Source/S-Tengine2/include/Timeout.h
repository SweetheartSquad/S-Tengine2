#pragma once

#include <node\NodeUpdatable.h>
#include <node\NodeChild.h>

#include <functional>

class Timeout : public virtual NodeChild, public virtual NodeUpdatable{
public:
	bool complete;
	bool active;
	float elapsedSeconds;
	float targetSeconds;
	std::function<void(Timeout *)> onCompleteFunction;

	Timeout(float _targetSeconds);

	// calls stop and then calls start
	void restart();
	
	// sets active to true
	virtual void start();

	// sets active to false
	void pause();

	// resets the timer to zero and sets complete and active to false
	void stop();

	// calls the onCompleteFunction, sets elapsedSeconds to targetSeconds, and complete to true
	virtual void trigger();

	virtual void update(Step * _step) override;
};