#pragma once

#include <iostream>

#include "ShaderLoader.h"
#include "FileUtils.h"
#include "GLUtils.h"
#include "ReferenceManager.h"

class ShaderInterface : public ReferenceManager{
private:
	ShaderLoader *shader;
	GLint aVertexPosition;
	GLint aVertexColor;
	GLint aVertexNormals;
	GLint aVertexUVs;

public:
	std::string vertName;
	std::string fragName;

	//Creates shader using "shaderFile.vert" and "shaderFile.frag"
	explicit ShaderInterface(std::string _shaderFile, bool _autoRelease);
	//Creates shader using "vertexShaderFile" and "fragmentShaderFile"
	ShaderInterface(std::string _vertexShaderFile, std::string _fragmentShaderFile, bool _autoRelease);
	~ShaderInterface(void);

	void init(std::string _vertexShaderFile, std::string _fragmentShaderFile);

	GLuint getProgramId();
	GLint get_aVertexPosition();
	GLint get_aVertexColor();
	GLint get_aVertexNormals();
	GLint get_aVertexUVs();
};
