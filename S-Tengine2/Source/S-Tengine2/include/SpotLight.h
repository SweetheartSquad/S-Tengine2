#pragma once

#include<Light.h>

class SpotLight : public Light {
public:
	SpotLight(glm::vec3 _coneDirection, glm::vec3 _intensities,  float _coneAngle, float _ambientCoefficient, float _attenuation, float _cutoff);

	void setConeAngle(float _angle);
	void setConeDirection(glm::vec3 _direction);

	float getConeAngle();
	glm::vec3 getConeDirection();
};