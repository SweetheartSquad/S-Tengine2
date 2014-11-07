#pragma once 

#include "Light.h"
#include "Shader.h"
#include <vector>

class RenderStack{
	
public:

	RenderStack();
	RenderStack(Shader * _shader, std::vector<Light *> * _lights);

	~RenderStack();

	std::vector<Light*> * lights;
	Shader * shader;
};