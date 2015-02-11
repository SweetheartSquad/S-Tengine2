#pragma once

#include <string>
#include <node/NodeUpdatable.h>

class Arduino;

class Accelerometer : public NodeUpdatable{
public:
	Arduino * arduino;

	std::string buffer;

	float x, y, z;
	float deltaX, deltaY, deltaZ;

	float roll, pitch;

	float highestX, lowestX, highestY, lowestY, highestZ, lowestZ;

	explicit Accelerometer(Arduino * _arduino);
	~Accelerometer();
	
	float getRoll();
	float getPitch();
	
	// Take the values in the x,y,z and parse them into useful data
	virtual void update(Step * _step) override;
};