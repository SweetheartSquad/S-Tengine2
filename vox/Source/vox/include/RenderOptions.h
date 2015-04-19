#pragma once 

#include <node\Node.h>
#include <vector>
#include <glew\glew.h>

class Shader;
class Light;

class RenderOptions : public Node{
public:

	RenderOptions(Shader * _shader, std::vector<Light *> * _lights, Shader * _overrideShader = nullptr);

	~RenderOptions();

	std::vector<Light*> * lights;
	unsigned long int lastNumLights;
	
	GLuint currentVao;
	GLuint currentlyBoundShaderId;

	Shader * shader;
	Shader * overrideShader;

	bool alphaSorting;
};