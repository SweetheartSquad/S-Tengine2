#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ShaderLoader
{
private:
	GLuint programId;
	GLuint compileShader(GLenum target, const char *source);

public:
	ShaderLoader(const char* vertexShaderSource, const char* fragmentShaderSource);
	~ShaderLoader(void);

	GLuint getProgramId();
};

