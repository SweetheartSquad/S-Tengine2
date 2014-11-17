#pragma once

class Step{
public:
	Step();
	~Step();

	double targetFrameDuration;
	double time;
	double deltaTimeCorrection;
	double lastTimestamp;


	void setDeltaTime(double _deltaTime);
	double getDeltaTime();
	bool getReverse();
private:
	double deltaTime;
	bool reverse;
};
