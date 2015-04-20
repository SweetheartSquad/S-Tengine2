#include "DirectionalLight.h"
#include "Transform.h"

DirectionalLight::DirectionalLight(glm::vec3 _direction, glm::vec3 _intensities, float _ambientCoefficient):
	Light(LightType::kDIRECTIONAL_LIGHT, _direction, _intensities, _ambientCoefficient, 1.f, 0.f, transform),
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	isKeyLight(false)
{
}

DirectionalLight::~DirectionalLight() {
	//empty
}