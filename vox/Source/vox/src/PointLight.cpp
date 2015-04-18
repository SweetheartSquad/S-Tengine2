#include "PointLight.h"
#include "Transform.h"

PointLight::PointLight(glm::vec3 _position, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation):
	Light(LightType::kPOINT_LIGHT, _position, _intensities, _ambientCoefficient, _attenuation, transform),
	NodeChild(nullptr),
	NodeTransformable(new Transform())
{
}

PointLight::~PointLight() {
	//empty
}