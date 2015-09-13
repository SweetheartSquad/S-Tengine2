#pragma once

#include <Light.h>
#include <Transform.h>

LightData::LightData(LightType _type, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, float _cutoff, float _coneAngle, glm::vec3 _coneDirection) :
	type(_type),
	intensities(_intensities),
	ambientCoefficient(_ambientCoefficient),
	attenuation(_attenuation),
	cutoff(_cutoff),
	coneAngle(_coneAngle),
	coneDirection(_coneDirection)
{
}

bool LightData::operator==(const LightData &other) const{
	return (type == other.type &&
		intensities == other.intensities &&
		ambientCoefficient == other.ambientCoefficient &&
		attenuation == other.attenuation &&
		cutoff == other.cutoff);
}
bool LightData::operator!=(const LightData &other) const{
	return !(*this == other);
}

Light::Light(LightType _type, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, float _cutoff, float _coneAngle, glm::vec3 _lightDirection):
	data(_type, _intensities, _ambientCoefficient, _attenuation, _cutoff, _coneAngle, _lightDirection),
	lightDirty(true)
{
	lastPos = getWorldPos();
}

Light::~Light(){
}

void Light::update(Step * _step){
	glm::vec3 curPos = getWorldPos();
	if(lastPos != curPos){
		lastPos = curPos;
		lightDirty = true;
	}
	Entity::update(_step);
}

void Light::unload(){
	lightDirty = true;
}

glm::vec3 Light::getIntensities() {
	return data.intensities;
}

void Light::setIntensities(const glm::vec3 _intensities) {
	data.intensities = _intensities;
	lightDirty = true;
}

float Light::getAmbientCoefficient() {
	return data.ambientCoefficient;
}

void Light::setAmbientCoefficient(float _ambientCoefficient) {
	data.ambientCoefficient = _ambientCoefficient;
	lightDirty = true;
}

float Light::getAttenuation() {
	return data.attenuation;
}

void Light::setAttenuation(float _attenuation) {
	data.attenuation = _attenuation;
	lightDirty = true;
}

float Light::getCutoff(){
	return data.cutoff;
}

void Light::setCutoff(float _cutoff) {
	data.cutoff = _cutoff;
	lightDirty = true;
}

LightType Light::getType() {
	return data.type;
}