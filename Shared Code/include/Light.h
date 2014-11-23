#pragma once

#include <glm/glm.hpp>

#include "NodeTransformable.h"

struct LightData {
	glm::vec3 position;
	glm::vec3 intensities;
	float ambientCoefficient;
	float attenuation;
};

/**************************************************************************************************
*
* A basic light node. 
* Stores a reference to a transform and a struct containing the position and color of the light 
*
****************************************************************************************************/
class Light : public NodeTransformable{
public:

	Light();
	explicit Light(Transform * _transform);
	~Light();

	/**
	* Updates the light data.position with its transforms
	* translation vector
	*/
	void update();

	/**
	* The data struct which is passed to the shader
	* This should be copied in the shader
	*/
	LightData data;
};