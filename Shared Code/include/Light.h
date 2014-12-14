#pragma once

#include <glm/glm.hpp>

#include "node/NodeTransformable.h"

enum class LightType {
	POINT = 0,
	DIRECTIONAL = 1
};

struct LightData {
	LightType type;
	glm::vec3 intensities;
	float ambientCoefficient;
	float attenuation;

	LightData(LightType _type, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation);
};

/**************************************************************************************************
*
* A basic light node. 
* Stores a reference to a transform and a struct containing data used by lighting shaders, including
* position, color, ambient coefficient, and attentuation. 
*
****************************************************************************************************/
class Light abstract : public NodeTransformable{
public:

	explicit Light(LightType _type, glm::vec3 _position, glm::vec3 intensities, float ambientCoefficient, float attenuation, Transform * _transform = nullptr);
	~Light();

	/**
	* The data struct which is passed to the shader
	* This should be copied in the shader
	*/
	LightData data;
};