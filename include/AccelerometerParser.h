#pragma once
#include <vector>

#include "Arduino.h"

class Accelerometer;


class AccelerometerParser : public Arduino{
public:

	explicit AccelerometerParser(std::string portName);
	~AccelerometerParser();

	void addAccelerometer(Accelerometer * _accelerometer);

	virtual void update(Step * _step) override;

private:
	std::vector<Accelerometer *> accelerometers;
};