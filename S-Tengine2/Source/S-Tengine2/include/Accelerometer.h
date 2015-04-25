#pragma once

#include <string>
#include <node/NodeUpdatable.h>

class Arduino;

class Accelerometer : public NodeUpdatable{
public:
	Arduino * arduino;

	std::string buffer;

	float x, y, z;
	//Last values. Set in the accelerometer parser
	float lx, ly, lz;
	//Values without the effect of gravity and other ambient data
	float gx, gy, gz;
	float deltaX, deltaY, deltaZ;
	//gravity/ambient values
	float gravX, gravY, gravZ;

	float roll, pitch;

	float highestX, lowestX, highestY, lowestY, highestZ, lowestZ;

	explicit Accelerometer(Arduino * _arduino);
	~Accelerometer();
	
	float getRoll();
	float getPitch();
	
	// Take the values in the x,y,z and parse them into useful data
	virtual void update(Step * _step) override;
};