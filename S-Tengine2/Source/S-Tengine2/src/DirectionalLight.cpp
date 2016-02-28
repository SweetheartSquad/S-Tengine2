#pragma once

#include "DirectionalLight.h"
#include "Transform.h"

DirectionalLight::DirectionalLight(glm::vec3 _intensities, float _ambientCoefficient):
	Light(LightType::kDIRECTIONAL_LIGHT, _intensities, _ambientCoefficient, 1.f, -1.f, 360.f, glm::vec3(0.f)),
	isKeyLight(false)
{
}

DirectionalLight::~DirectionalLight() {
}