#include "DirectionalLight.h"
#include "Transform.h"

DirectionalLight::DirectionalLight(glm::vec3 _position, glm::vec3 _intensities, float _ambientCoefficient):
	Light(LightType::DIRECTIONAL, _position, _intensities, _ambientCoefficient, 1.f)
{
}

DirectionalLight::~DirectionalLight() {
	//empty
}