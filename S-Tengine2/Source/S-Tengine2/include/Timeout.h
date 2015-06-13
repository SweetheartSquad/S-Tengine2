#pragma once

#include <node\NodeUpdatable.h>
#include <node\NodeChild.h>

class Timeout : public virtual NodeChild, public virtual NodeUpdatable{
public:
	bool complete;
	float elapsedSeconds;
	float targetSeconds;
	void * userData;
	void (*onCompleteFunction)(void * _userData, Timeout * _this);

	Timeout(float _targetSeconds, void * _userData);

	// resets the timer to zero and sets complete to false, allowing the onCompleteFunction to trigger again
	void restart();

	void update(Step * _step) override;
};