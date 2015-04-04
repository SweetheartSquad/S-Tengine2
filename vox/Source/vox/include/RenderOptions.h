#pragma once 

#include <vector>
#include <glew\glew.h>

class Shader;
class Light;

class RenderOptions{
public:

	RenderOptions(Shader * _shader, std::vector<Light *> * _lights, Shader * _overrideShader = nullptr);

	~RenderOptions();

	std::vector<Light*> * lights;
	
	GLuint currentVao;
	GLuint currentlyBoundShaderId;

	Shader * shader;
	Shader * overrideShader;

	bool alphaSorting;
};