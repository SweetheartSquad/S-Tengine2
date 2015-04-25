#pragma once
#include <vector>

#include "Arduino.h"

#define LINE_SIZE 37

class Accelerometer;

class AccelerometerParser : public Arduino{
public:
	bool forced;
	bool firstPing;

	explicit AccelerometerParser(std::string portName);
	~AccelerometerParser();

	Accelerometer *  addAccelerometer();

	virtual void update(Step * _step) override;

private:
	std::string accumlator;

	std::vector<Accelerometer *> accelerometers;
	std::string AccelerometerParser::getLatestData(std::string _acc);
};