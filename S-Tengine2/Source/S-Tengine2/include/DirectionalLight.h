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

	DirectionalLight(glm::vec3 _direction, glm::vec3 _intensities, float _ambientCoefficient);
	~DirectionalLight();

	bool isKeyLight;
};