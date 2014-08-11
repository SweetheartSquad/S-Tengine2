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
	GLint aFragColor;

	//const char* vertexShaderString;
	//const char* fragmentShaderString;

public:
	ShaderInterface(const char *vertexShaderShource, const char *fragmentShaderSource);
	~ShaderInterface(void);

	GLuint getProgramId();
	GLint get_aPositionVertex();
	GLint get_aFragColor();
};

