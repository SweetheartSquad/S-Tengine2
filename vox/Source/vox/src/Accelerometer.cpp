#pragma once

#define _USE_MATH_DEFINES

#include "Accelerometer.h"
#include <math.h>
#include <iostream>

Accelerometer::Accelerometer(Arduino * _arduino):
	arduino(_arduino)
{
}

float Accelerometer::getRoll(){
	return roll;
}

float Accelerometer::getPitch(){
	return pitch;
}

void Accelerometer::update(Step* _step){

	float diffX = ((x - 530.0f) / (130.0f - 530.0f))* 2.f - 1.f - deltaX;
	float diffY = ((y - 530.0f) / (130.0f - 530.0f))* 2.f - 1.f - deltaY;
	float diffZ = ((z - 530.0f) / (130.0f - 530.0f))* 2.f - 1.f - deltaZ;

    deltaX = deltaX + 0.25f*(diffX);
    deltaY = deltaY + 0.25f*(diffY);
    deltaZ = deltaZ + 0.25f*(diffZ);

	pitch =  atan2(deltaZ, 1 + abs(deltaY))/0.26f * -M_PI/2.f;
	roll  =  atan2(deltaX, 1 + abs(deltaY))/0.26f *  M_PI/2.f;

	std::cout<<roll << "    " << pitch;
}

Accelerometer::~Accelerometer(){
}

