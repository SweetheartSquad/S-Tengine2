#pragma once

#include <NumberUtils.h>
#include <random>

float vox::NumberUtils::randomFloat(float _min, float _max){
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float diff = _max - _min;
    float r = random * diff;
    return _min + r;
}

int vox::NumberUtils::randomInt(int _min, int _max){
	return _min + (rand() % (_max - _min + 1));
}