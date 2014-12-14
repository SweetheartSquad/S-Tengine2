#pragma once 

#include <GL\glew.h>

#include "RenderOptions.h"

class Shader;
class Light;

class GLFWRenderOptions : public RenderOptions{
	
public:

	GLFWRenderOptions(Shader * _shader, std::vector<Light *> * _lights, Shader * _overrideShader = nullptr);

	~GLFWRenderOptions();

	GLuint shadowMapTextureId;

	bool kc_active;
};