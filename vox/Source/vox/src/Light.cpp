#include "Light.h"
#include "Transform.h"

LightData::LightData(LightType _type, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation) :
	type(_type),
	intensities(_intensities),
	ambientCoefficient(_ambientCoefficient),
	attenuation(_attenuation)
{
}

Light::Light(LightType _type, glm::vec3 _position, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, Transform * _transform):
	NodeTransformable(_transform == nullptr ? new Transform() : _transform),
	NodeChild(nullptr),
	data(_type, _intensities, _ambientCoefficient, _attenuation)
{
	_transform->translate(_position, false);
}

Light::~Light() {
	delete transform;
	transform = nullptr;
}