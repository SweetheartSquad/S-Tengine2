#pragma once

#include <node\Node.h>

class Step : public Node{
public:
	Step();
	~Step();

	double targetFrameDuration;
	double time;
	double deltaTimeCorrection;
	double lastTimestamp;
	double deltaTime;
	unsigned long long cycles;

	void setDeltaTime(double _deltaTime);
	double getDeltaTime();
	bool getReverse();
private:
	bool reverse;
};
