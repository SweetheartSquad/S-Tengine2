#pragma once

#include "Texture.h"

class ProgrammaticTexture : public Texture {
public:
	ProgrammaticTexture(unsigned char * _data = nullptr, bool _autoRelease = true, bool _useMipmaps = true);
	~ProgrammaticTexture();

	virtual void load() override;
	virtual void setTextureData(unsigned char * _data);


	void allocate(unsigned long int _width, unsigned long int _height, unsigned long int _channels = 4);
};
