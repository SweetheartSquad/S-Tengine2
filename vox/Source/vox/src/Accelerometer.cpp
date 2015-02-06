#pragma once

#include "Accelerometer.h"

Accelerometer::Accelerometer(Arduino * _arduino):
	arduino(_arduino)
{
}

float Accelerometer::getRoll(){
	return 0;
}

float Accelerometer::getPitch(){
	return 0;
}

Accelerometer::~Accelerometer(){
}

