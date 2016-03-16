#pragma once

#include <ProgrammaticTexture.h>
#include <NumberUtils.h>
#include <TextureUtils.h>

#include <assert.h>

ProgrammaticTexture::ProgrammaticTexture(unsigned char * _data, bool _autoRelease, bool _useMipmaps) :
	Texture("", true, _autoRelease, _useMipmaps),
	NodeResource(_autoRelease)
{
	data = _data;
}

ProgrammaticTexture::~ProgrammaticTexture() {
}

void ProgrammaticTexture::load() {
	if(!loaded){
		genTextures();
	}
	NodeLoadable::load();
}

void ProgrammaticTexture::setTextureData(unsigned char* _data) {
	data = _data;
	unload();
	load();
}

void ProgrammaticTexture::allocate(unsigned long int _width, unsigned long int _height, unsigned long int _channels){
	// make sure that we aren't allocating on top of an existing texture, which would cause a memory leak
	assert(data == nullptr);

	resize(_width, _height, _channels);

	data = static_cast<unsigned char *>(malloc(sizeof(unsigned char) * numBytes));
}


NoiseTexture::NoiseTexture(unsigned long int _width, unsigned long int _height, bool _autoRelease, bool _useMipmaps) :
	ProgrammaticTexture(nullptr, _autoRelease, _useMipmaps),
	NodeResource(_autoRelease),
	maxVal(255),
	minVal(0)
{
	allocate(_width, _height, 1);
	setNoise();
}

void NoiseTexture::setNoise(){
	for(unsigned long int i = 0; i < numBytes; ++i){
		data[i] = sweet::NumberUtils::randomInt(minVal, maxVal);
	}
}




PerlinNoiseTexture::PerlinNoiseTexture(unsigned long int _width, unsigned long int _height, bool _autoRelease, bool _useMipmaps) :
	ProgrammaticTexture(nullptr, _autoRelease, _useMipmaps),
	NodeResource(_autoRelease),
	amplitude(255),
	offset(0),
	frequency(1.f)
{
	allocate(_width, _height, 1);
	setNoise(0);
}

void PerlinNoiseTexture::setNoise(float _time){
	for(unsigned long int y = 0; y < height; ++y){
	for(unsigned long int x = 0; x < width; ++x){
		sweet::TextureUtils::setPixel(this, x, y, sweet::NumberUtils::pNoise( ((float)x)/(width) * frequency.x, ((float)y)/(height) * frequency.y, _time * frequency.z )*255.f + offset);
	}
	}
}