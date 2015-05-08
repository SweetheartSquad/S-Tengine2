#pragma once

#define _USE_MATH_DEFINES

#include "Accelerometer.h"
#include <math.h>
#include <iostream>

Accelerometer::Accelerometer(Arduino * _arduino):
	arduino(_arduino),
	x(0.f),
	y(0.f), 
	z(0.f),
	deltaX(0.f),
	deltaY(0.f),
	deltaZ(0.f),
	roll(0.f),
	pitch(0.f),
	highestX(530.f),
	lowestX(130.f),
	highestY(530.f),
	lowestY(130.f),
	highestZ(530.f),
	lowestZ(130.f),
	gravX(0.0f),
	gravY(0.0f),
	gravZ(0.0f),
	lx(0.0f),
	ly(0.0f),
	lz(0.0f)
{
}

float Accelerometer::getRoll(){
	return roll;
}

float Accelerometer::getPitch(){
	return pitch;
}

void Accelerometer::update(Step* _step){
	gravX = 0.9f * gravX + 0.1f * x;
	gravY = 0.9f * gravY + 0.1f * y;
	gravZ = 0.9f * gravZ + 0.1f * z;

	gx -= gravX;
	gy -= gravY;
	gz -= gravZ;

	float diffX = ((x - highestX) / (lowestX - highestX))* 2.f - 1.f - deltaX;
	float diffY = ((y - highestY) / (lowestY - highestY))* 2.f - 1.f - deltaY;
	float diffZ = ((z - highestZ) / (lowestZ - highestZ))* 2.f - 1.f - deltaZ;

    deltaX = deltaX + 0.25f*(diffX);
    deltaY = deltaY + 0.25f*(diffY);
    deltaZ = deltaZ + 0.25f*(diffZ);

	pitch = static_cast<float>(atan2(deltaZ, 1 + abs(deltaY)))/0.26f * -M_PI/2.f;
	//There seems to be a problem with the roll. It always goes to far to the left
	//this offset fixes it but this could possibly vary based on the accelerometer
	//We may have to find a better solution or just dial in values for each accelerometer
	roll = 0.35f + static_cast<float>(atan2(deltaX, 1.f + abs(deltaY)))/0.26f * M_PI/2.f;
}

Accelerometer::~Accelerometer(){
}

