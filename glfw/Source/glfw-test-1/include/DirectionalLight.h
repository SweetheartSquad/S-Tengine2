#pragma once

#include <glm/glm.hpp>

#include "Light.h"

/**************************************************************************************************
*
* A directional light node. 
* Takes in a light colour and an ambient coefficient.
*
****************************************************************************************************/
class DirectionalLight : public Light{
public:

	DirectionalLight(glm::vec3 position, glm::vec3 intensities, float ambientCoefficient);
	~DirectionalLight();
};