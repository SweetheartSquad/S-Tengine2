#pragma once

#include <ProgrammaticTexture.h>

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
