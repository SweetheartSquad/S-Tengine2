#pragma once

#include "ShaderLoader.h"
#include "FileUtils.h"
#include <iostream>

class ShaderInterface
{
private:
	ShaderLoader *shader;
	GLint aPositionVertex;
	GLint uColour;

	char *vertexShaderString;
	char *fragmentShaderString;

public:
	ShaderInterface(const char *vertexShaderShource, const char *fragmentShaderSource);
	~ShaderInterface(void);

	GLuint getProgramId();
	GLint get_aPositionVertex();
	GLint get_uColourShader();
};

