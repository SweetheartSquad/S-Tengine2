#include "Light.h"
#include "Transform.h"

LightData::LightData(LightType _type, glm::vec3 _position, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation) :
	type(_type),
	position(_position),
	intensities(_intensities),
	ambientCoefficient(_ambientCoefficient),
	attenuation(_attenuation)
{
}

Light::Light(LightType _type, glm::vec3 _position, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, Transform * _transform):
	NodeTransformable(_transform == nullptr ? new Transform() : _transform),
	Node(),
	data(_type, _position, _intensities, _ambientCoefficient, _attenuation)
{
}

Light::~Light() {
	delete transform;
	transform = nullptr;
}

void Light::update(){
	//Copy the transform to the data struct
	data.position = transform->translationVector;
}