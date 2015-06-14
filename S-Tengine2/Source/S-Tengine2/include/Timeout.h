#pragma once

#include <node\NodeUpdatable.h>
#include <node\NodeChild.h>

#include <functional>

class Timeout : public virtual NodeChild, public virtual NodeUpdatable{
public:
	bool complete;
	float elapsedSeconds;
	float targetSeconds;
	std::function<void(Timeout *)> onCompleteFunction;

	Timeout(float _targetSeconds);

	// resets the timer to zero and sets complete to false, allowing the onCompleteFunction to trigger again
	void restart();

	void update(Step * _step) override;
};