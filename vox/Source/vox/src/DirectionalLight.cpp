#include "DirectionalLight.h"
#include "Transform.h"

DirectionalLight::DirectionalLight(glm::vec3 _position, glm::vec3 _intensities, float _ambientCoefficient):
	Light(LightType::kDIRECTIONAL_LIGHT, _position, _intensities, _ambientCoefficient, 1.f, transform),
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	isKeyLight(false)
{
}

DirectionalLight::~DirectionalLight() {
	//empty
}