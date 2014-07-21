#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "GLUtils.h"

class ShaderLoader
{
private:
	GLuint programId;
	GLuint compileShader(GLenum shaderType, const char *source);

public:
	ShaderLoader(const char* vertexShaderSource, const char* fragmentShaderSource);
	~ShaderLoader(void);

	GLuint getProgramId();
};

