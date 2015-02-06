#pragma once

#include <string> 

#include <glm/glm.hpp>

class Arduino;

class Accelerometer{
	
	Arduino * arduino;

	std::string buffer;

	explicit Accelerometer(Arduino * _arduino);
	~Accelerometer();
	
	float getRoll();
	float getPitch();
};