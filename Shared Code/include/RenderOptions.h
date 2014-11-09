#pragma once 

#include "Light.h"
#include "Shader.h"
#include <vector>

class RenderOptions{
	
public:

	RenderOptions();
	RenderOptions(Shader * _shader, std::vector<Light *> * _lights);

	~RenderOptions();

	std::vector<Light*> * lights;
	Shader * shader;
};