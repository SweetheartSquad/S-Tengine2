#include "DirectionalLight.h"
#include "Transform.h"

DirectionalLight::DirectionalLight(glm::vec3 _intensities, float _ambientCoefficient):
	Light(LightType::kDIRECTIONAL_LIGHT, _intensities, _ambientCoefficient, 1.f, -1.f, 0.f, glm::vec3()),
	isKeyLight(false)
{
}

DirectionalLight::~DirectionalLight() {
}