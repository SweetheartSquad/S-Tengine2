#pragma once

#include <iostream>

#include "ShaderLoader.h"
#include "FileUtils.h"
#include "GLUtils.h"

class ShaderInterface{
private:
	ShaderLoader * shader;
	GLint aVertexPosition;
	GLint aVertexColor;
	GLint aVertexNormals;
	GLint aVertexUVs;

	// Reads the files at the given location/name and compiles them as a vertex shader and a fragment shader
	void init(std::string _vertexShaderFile, std::string _fragmentShaderFile);
public:
	std::string vertName;	// Filename of vertex shader
	std::string fragName;	// Filename of fragment shader

	// Creates shader using "_shaderFile.vert" and "_shaderFile.frag"
	explicit ShaderInterface(std::string _shaderFile);
	// Creates shader using "_vertexShaderFile" and "_fragmentShaderFile"
	ShaderInterface(std::string _vertexShaderFile, std::string _fragmentShaderFile);
	~ShaderInterface(void);

	GLuint getProgramId();
	GLint get_aVertexPosition();
	GLint get_aVertexColor();
	GLint get_aVertexNormals();
	GLint get_aVertexUVs();
};
