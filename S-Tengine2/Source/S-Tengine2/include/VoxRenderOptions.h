#pragma once 

#include <GL\glew.h>

#include "RenderOptions.h"

class Shader;
class Light;

// necessary for the old shadow shader component
class VoxRenderOptions : public RenderOptions{
public:
	VoxRenderOptions(Shader * _shader, std::vector<Light *> * _lights, Shader * _overrideShader = nullptr);
	~VoxRenderOptions();

	GLuint shadowMapTextureId;
};