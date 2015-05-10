#pragma once

#include <glm/glm.hpp>

#include "Light.h"

/**************************************************************************************************
*
* A point light node. 
* Takes in a position, light colour, ambient coefficient, and attentuation value.
*
****************************************************************************************************/
class PointLight : public Light{
public:
	PointLight(glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, float _cutoff);
	~PointLight();
};