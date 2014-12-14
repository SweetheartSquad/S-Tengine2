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

	PointLight(glm::vec3 position, glm::vec3 intensities, float ambientCoefficient, float attenuation);
	~PointLight();
};