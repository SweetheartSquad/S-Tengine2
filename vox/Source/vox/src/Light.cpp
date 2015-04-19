#include "Light.h"
#include "Transform.h"

LightData::LightData(LightType _type, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation) :
	type(_type),
	intensities(_intensities),
	ambientCoefficient(_ambientCoefficient),
	attenuation(_attenuation)
{
}

bool LightData::operator==(const LightData &other) const{
	return (type == other.type &&
		intensities == other.intensities &&
		ambientCoefficient == other.ambientCoefficient &&
		attenuation == other.attenuation);
}
bool LightData::operator!=(const LightData &other) const{
	return !(*this == other);
}

Light::Light(LightType _type, glm::vec3 _position, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, Transform * _transform):
	NodeTransformable(_transform == nullptr ? new Transform() : _transform),
	NodeChild(nullptr),
	data(_type, _intensities, _ambientCoefficient, _attenuation),
	lastData(_type, _intensities, _ambientCoefficient, _attenuation),
	dirty(true)
{
	_transform->translate(_position, false);
	lastPos = getPos(false);
}

Light::~Light() {
	delete transform;
	transform = nullptr;
}