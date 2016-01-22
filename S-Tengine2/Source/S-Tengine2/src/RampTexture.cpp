#pragma once

#include <RampTexture.h>
#include <TextureUtils.h>

RampTexture::RampTexture(glm::vec3 _start, glm::vec3 _end, unsigned long _levels, bool _autoRelease):
	NodeResource(_autoRelease),
	ProgrammaticTexture(nullptr, _autoRelease, true)
{
	allocate(_levels, 1, 4);

	setRamp(_start, _end, _levels);
}


void RampTexture::setRamp(glm::vec3 _start, glm::vec3 _end, unsigned long _levels){
	start = _start;
	end = _start;
	levels = _levels;
	for(unsigned long int i = 0; i < width; ++i){
		double pStep = i/ static_cast<double>(width);
		sweet::TextureUtils::setPixel(this, i, 0, glm::uvec4(
			(_start.r * (1 - pStep ) + _end.r * (pStep)) * 255.0,
			(_start.g * (1 - pStep ) + _end.g * (pStep)) * 255.0,
			(_start.b * (1 - pStep ) + _end.b * (pStep)) * 255.0,
			255
		));
		unsigned char * pixel = sweet::TextureUtils::getPixelPointer(this, i, 0, 0);
	}
}