#pragma once

#include <Colour.h>
#include <NumberUtils.h>

Colour::Colour(unsigned char _r, unsigned char _g, unsigned char _b) :
	glm::ivec3(_r, _g, _b)
{
}

Colour::Colour(glm::ivec3 _rgb) :
	glm::ivec3(_rgb)
{
}

Colour::Colour() :
	glm::ivec3(255, 255, 255)
{
}

Colour Colour::brighter(){
	return Colour(*this + glm::ivec3(10));
}
Colour Colour::darker(){
	return Colour(*this - glm::ivec3(10));
}

Colour Colour::hsvMod(glm::ivec3 _hsvDelta){
	glm::ivec3 hsv = rgbToHsv(*this) + _hsvDelta;
	hsv.x = glm::mod((float)hsv.x, 360.f);
	hsv.y = glm::min(100, glm::max(0, hsv.y));
	hsv.z = glm::min(100, glm::max(0, hsv.z));
	return Colour(hsvToRgb(hsv));
}
	
glm::ivec3 Colour::rgbToHsv(glm::ivec3 _rgb){
	glm::vec3 n = glm::vec3(_rgb) / 255.f;
	float cMax = glm::max(n.r, glm::max(n.g, n.b));
	float cMin = glm::min(n.r, glm::min(n.g, n.b));
	float delta = cMax - cMin;
	glm::ivec3 hsv;

	// hue calculation
	if(delta < FLT_EPSILON){
		hsv.x = 0;
	}
	else if (glm::abs(cMax - n.r) < FLT_EPSILON){
		hsv.x = 60 * fmod((n.g - n.b) / delta, 6);
	}else if(glm::abs(cMax - n.g) < FLT_EPSILON){
		hsv.x = 60 * ((n.b - n.r) / delta + 2);
	}else if(glm::abs(cMax - n.b) < FLT_EPSILON){
		hsv.x = 60 * ((n.r - n.g) / delta + 4);
	}

	// saturation
	hsv.y = cMax < FLT_EPSILON ? 0 : (delta/cMax)*100;

	// value
	hsv.z = cMax*100;

	return hsv;
}
glm::ivec3 Colour::hsvToRgb(glm::ivec3 _hsv){
	float c = (_hsv.z * _hsv.y)/10000.f;
	float x = c * (1 - glm::abs(fmod(_hsv.x / 60.f, 2) - 1));
	float m = _hsv.z/100.f - c;

	
	glm::vec3 n;
	if(_hsv.x < 60){
		n = glm::vec3(c, x, 0);
	}else if (_hsv.x < 120){
		n = glm::vec3(x, c, 0);
	}else if (_hsv.x < 180){
		n = glm::vec3(0, c, x);
	}else if (_hsv.x < 240){
		n = glm::vec3(0, x, c);
	}else if (_hsv.x < 300){
		n = glm::vec3(x, 0, c);
	}else if(_hsv.x < 360){
		n = glm::vec3(c, 0, x);
	}
	
	return glm::ivec3((n + glm::vec3(m)) * 255.f);
}

Colour Colour::getRandomFromRgbRange(glm::ivec3 _min, glm::ivec3 _max){
	return Colour(sweet::NumberUtils::randomInt(_min.r, _max.r), sweet::NumberUtils::randomInt(_min.g, _max.g), sweet::NumberUtils::randomInt(_min.b, _max.b));
}

Colour Colour::getRandomFromRgbMean(glm::ivec3 _mean, glm::ivec3 _halfRange){
	return Colour(_mean + glm::ivec3(sweet::NumberUtils::randomInt(-_halfRange.r, _halfRange.r), sweet::NumberUtils::randomInt(-_halfRange.g, _halfRange.g), sweet::NumberUtils::randomInt(-_halfRange.b, _halfRange.b)));
}

Colour Colour::getRandomFromHsvRange(glm::ivec3 _min, glm::ivec3 _max){
	return hsvToRgb(glm::ivec3(sweet::NumberUtils::randomInt(_min.r, _max.r), sweet::NumberUtils::randomInt(_min.g, _max.g), sweet::NumberUtils::randomInt(_min.b, _max.b)));
}

Colour Colour::getRandomFromHsvMean(glm::ivec3 _mean, glm::ivec3 _halfRange){
	return hsvToRgb(glm::ivec3(_mean + glm::ivec3(sweet::NumberUtils::randomInt(-_halfRange.r, _halfRange.r), sweet::NumberUtils::randomInt(-_halfRange.g, _halfRange.g), sweet::NumberUtils::randomInt(-_halfRange.b, _halfRange.b))));
}