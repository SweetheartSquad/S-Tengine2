#pragma once 

#include <node\Node.h>
#include <vector>
#include <GL\glew.h>
#include <StandardFrameBuffer.h>

class Shader;
class Light;

class RenderOptions : public Node{
public:

	RenderOptions(Shader * _shader, std::vector<Light *> * _lights, Shader * _overrideShader = nullptr);

	~RenderOptions();

	std::vector<Light*> * lights;
	
	GLuint currentVao;
	GLuint currentlyBoundShaderId;

	Shader * shader;
	Shader * overrideShader;

	GLuint  depthBufferId;  
	GLuint  normalBufferId;  

	bool alphaSorting;
	bool alphaEnabled;
	bool depthEnabled;
};