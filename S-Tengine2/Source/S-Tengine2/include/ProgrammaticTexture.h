#pragma once

#include "Texture.h"
#include <glm\glm.hpp>

class ProgrammaticTexture : public Texture {
public:
	ProgrammaticTexture(unsigned char * _data = nullptr, bool _autoRelease = true, bool _useMipmaps = true);
	~ProgrammaticTexture();

	virtual void load() override;
	virtual void setTextureData(unsigned char * _data);


	void allocate(unsigned long int _width, unsigned long int _height, unsigned long int _channels = 4);
};

class NoiseTexture : public ProgrammaticTexture{
public:
	// minimum and maximum possible values for the generated texture data
	unsigned char minVal, maxVal;
	NoiseTexture(unsigned long int _width, unsigned long int _height, bool _autoRelease = true, bool _useMipmaps = true);

	void setNoise();
};

class PerlinNoiseTexture : public ProgrammaticTexture{
public:
	// coordinates are multiplied by frequency before calling perlin noise function
	// low frequency = larger, smoother waves; high frequency = short, spikier waves
	glm::vec3 frequency;
	// result of perlin noise function is multiplied by amplitude
	unsigned char amplitude;
	// offset is added to result of perlin noise
	unsigned char offset;
	PerlinNoiseTexture(unsigned long int _width, unsigned long int _height, bool _autoRelease = true, bool _useMipmaps = true);

	// _time is the z coordinate of the perlin noise function
	void setNoise(float _time);
};