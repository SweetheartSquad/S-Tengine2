#pragma once

#include <SpotLight.h>

SpotLight::SpotLight(glm::vec3 _coneDirection, glm::vec3 _intensities,  float _coneAngle, float _ambientCoefficient, float _attenuation, float _cutoff) :
	Light(kSPOT_LIGHT, _intensities, _ambientCoefficient, _attenuation, _cutoff, _coneAngle, _coneDirection) {	
} 

void SpotLight::setConeAngle(float _angle) {
	data.coneAngle = _angle;
	lightDirty = true;
}

void SpotLight::setConeDirection(glm::vec3 _direction) {
	data.coneDirection = _direction;
	lightDirty = true;
}

float SpotLight::getConeAngle() {
	return data.coneAngle;
}

glm::vec3 SpotLight::getConeDirection() {
	return data.coneDirection;

}