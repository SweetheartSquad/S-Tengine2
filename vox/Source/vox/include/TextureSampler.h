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
	
	TextureSampler(Texture * _texture, float _width, float _height, float _u = 0.f, float _v = 0.f);
	TextureSampler(std::string _definitionDir, std::string _definitionName);

	~TextureSampler();

	void load() override;
	void unload() override;
};