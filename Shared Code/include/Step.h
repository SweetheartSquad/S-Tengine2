#pragma once

class Step{
public:
	Step();
	~Step();

	double targetFrameDuration;
	double time;
	double deltaTime;
	double deltaTimeCorrection;
	double lastTimestamp;

	bool reverse;
};
