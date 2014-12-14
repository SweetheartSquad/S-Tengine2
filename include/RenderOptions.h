#pragma once 

#include <vector>

class Shader;
class Light;

class RenderOptions{
public:

	RenderOptions(Shader * _shader, std::vector<Light *> * _lights, Shader * _overrideShader = nullptr);

	~RenderOptions();

	std::vector<Light*> * lights;
	
	Shader * shader;
	Shader * overrideShader;
};