#pragma once

#include <glm/glm.hpp>

#include "node/ShiftKiddie.h"

enum LightType {
	kPOINT_LIGHT = 0,
	kDIRECTIONAL_LIGHT = 1
};

struct LightData {
	LightType type;
	glm::vec3 intensities;
	float ambientCoefficient;
	float attenuation;
	
	LightData(LightType _type, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation);
	
	bool operator==(const LightData &other) const;
	bool operator!=(const LightData &other) const;
};

/**************************************************************************************************
*
* A basic light node. 
* Stores a reference to a transform and a struct containing data used by lighting shaders, including
* position, color, ambient coefficient, and attentuation. 
*
****************************************************************************************************/
class Light abstract : public ShiftKiddie{
public:

	explicit Light(LightType _type, glm::vec3 _position, glm::vec3 intensities, float ambientCoefficient, float attenuation, Transform * _transform = nullptr);
	~Light();

	/**
	* The data struct which is passed to the shader
	* This should be copied in the shader
	*/
	LightData data;
	// used to check if the light needs to be updated on the GPU
	LightData lastData;
	glm::vec3 lastPos;

	bool dirty;
};