#pragma once

#include <iostream>

#include "ShaderLoader.h"
#include "FileUtils.h"
#include "GLUtils.h"

class ShaderInterface
{
private:
	ShaderLoader *shader;
	GLint aPositionVertex;
	GLint uColour;

	//const char* vertexShaderString;
	//const char* fragmentShaderString;

public:
	ShaderInterface(const char *vertexShaderShource, const char *fragmentShaderSource);
	~ShaderInterface(void);

	GLuint getProgramId();
	GLint get_aPositionVertex();
	GLint get_uColourShader();
};

