#include "PointLight.h"
#include "Transform.h"

PointLight::PointLight(glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, float _cutoff):
	Light(LightType::kPOINT_LIGHT, _intensities, _ambientCoefficient, _attenuation, _cutoff)
{
}

PointLight::~PointLight() {
}