#pragma once

#include <ProgrammaticTexture.h>
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
	genTextures();
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

	width = _width;
	height = _height;
	channels = _channels;
	numPixels = width * height;
	numBytes = numPixels * channels;

	data = static_cast<unsigned char *>(malloc(sizeof(unsigned char) * numBytes));
}