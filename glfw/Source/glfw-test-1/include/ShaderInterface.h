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
	GLint aVertexNormals;

	//const char* vertexShaderString;
	//const char* fragmentShaderString;
	void init(const char *vertexShaderShource, const char *fragmentShaderSource);
public:
	ShaderInterface(const char *shaderShource);
	ShaderInterface(const char *vertexShaderShource, const char *fragmentShaderSource);
	~ShaderInterface(void);

	GLuint getProgramId();
	GLint get_aPositionVertex();
	GLint get_aFragColor();
	GLint get_aVertexNormals();
};

