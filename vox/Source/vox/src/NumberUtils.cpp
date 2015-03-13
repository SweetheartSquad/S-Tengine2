#pragma once

#include "NumberUtils.h"s
#include <random>

float vox::NumberUtils::randomFloat(float _min, float _max){
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float diff = _max - _min;
    float r = random * diff;
    return _min + r;
}

float vox::NumberUtils::randomInt(int _min, int _max){
	return _min + (rand() % static_cast<int>(_max - _min + 1));
}