#pragma once

#include <iostream>

#include "ShaderLoader.h"
#include "FileUtils.h"
#include "GLUtils.h"

class ShaderInterface{
private:
	ShaderLoader *shader;
	GLint aVertexPosition;
	GLint aVertexColor;
	GLint aVertexNormals;

	void init(std::string _vertexShaderFile, std::string _fragmentShaderFile);
public:
	std::string vertName;
	std::string fragName;

	//Creates shader using "shaderFile.vert" and "shaderFile.frag"
	ShaderInterface(std::string _shaderFile);
	//Creates shader using "vertexShaderFile" and "fragmentShaderFile"
	ShaderInterface(std::string _vertexShaderFile, std::string _fragmentShaderFile);
	~ShaderInterface(void);

	GLuint getProgramId();
	GLint get_aVertexPosition();
	GLint get_aVertexColor();
	GLint get_aVertexNormals();
};
