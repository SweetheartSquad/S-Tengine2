#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "GLUtils.h"

class ShaderLoader{
private:
	GLuint programId;
	GLuint compileShader(GLenum _shaderType, const char *_source, int _length);

public:
	ShaderLoader(std::string _vertexShaderSource, std::string _fragmentShaderSource);
	~ShaderLoader(void);

	GLuint getProgramId();
};
