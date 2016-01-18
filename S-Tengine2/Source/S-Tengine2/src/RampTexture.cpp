#pragma once

#include <RampTexture.h>
#include <TextureUtils.h>

RampTexture::RampTexture(glm::vec3 _start, glm::vec3 _end, unsigned long _levels, bool _autoRelease):
	NodeResource(_autoRelease),
	ProgrammaticTexture(nullptr, _autoRelease)
{
	channels = 4;
	width    = _levels;
	height   = 1;
	numPixels = width * height;
	numBytes = numPixels * channels;
	data = static_cast<unsigned char *>(malloc(sizeof(unsigned char) * numBytes));

	for(unsigned long int i = 0; i < width; ++i){
		
		unsigned char * pixel = sweet::TextureUtils::getPixelPointer(this, i, 0, 0);
		
		double pStep = i/ static_cast<double>(width);
		
		*(pixel+0) = (_start.r * (1 - pStep ) + _end.r * (pStep)) * 255.0;
		*(pixel+1) = (_start.g * (1 - pStep ) + _end.g * (pStep)) * 255.0;
		*(pixel+2) = (_start.b * (1 - pStep ) + _end.b * (pStep)) * 255.0;
		*(pixel+3) = 255;
	}
}
