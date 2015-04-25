#pragma once

#include <node/NodeResource.h>

class Texture;
class TextureSampler : public NodeResource{
public:

	Texture * texture;

	float width;
	float height;
	float u;
	float v;
	
	bool releaseTexture;
	
	TextureSampler(Texture * _texture, float _width, float _height, float _u = 0.f, float _v = 0.f, bool _releaseTexture = true);
	TextureSampler(std::string _definitionDir, std::string _definitionName, bool _releaseTexture = true);

	~TextureSampler();

	void load() override;
	void unload() override;
};