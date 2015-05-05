#pragma once

#include <Light.h>
#include <Transform.h>

LightData::LightData(LightType _type, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, float _cutoff) :
	type(_type),
	intensities(_intensities),
	ambientCoefficient(_ambientCoefficient),
	attenuation(_attenuation),
	cutoff(_cutoff)
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

Light::Light(LightType _type, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, float _cutoff):
	data(_type, _intensities, _ambientCoefficient, _attenuation, _cutoff),
	lastData(_type, _intensities, _ambientCoefficient, _attenuation, _cutoff),
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
	}if(lastData != data){
		lastData = data;
		lightDirty = true;
	}
}

void Light::unload(){
	lightDirty = true;
}