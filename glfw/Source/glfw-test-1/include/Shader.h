#pragma once

#include <iostream>

#include "GLUtils.h"
#include "FileUtils.h"
#include "ResourceNode.h"

class Shader : public ResourceNode{
private:
	GLint aVertexPosition;
	GLint aVertexColor;
	GLint aVertexNormals;
	GLint aVertexUVs;
	GLuint programId;

public:
	std::string vertName;
	std::string fragName;

	//Creates shader using "shaderFile.vert" and "shaderFile.frag"
	explicit Shader(std::string _shaderFile, bool _autoRelease);
	//Creates shader using "vertexShaderFile" and "fragmentShaderFile"
	Shader(std::string _vertexShaderFile, std::string _fragmentShaderFile, bool _autoRelease);
	~Shader(void);

	void init(std::string _vertexShaderFile, std::string _fragmentShaderFile);

	GLuint compileShader(GLenum _shaderType, const char* _source, int _length);

	GLuint getProgramId();
	GLint get_aVertexPosition();
	GLint get_aVertexColor();
	GLint get_aVertexNormals();
	GLint get_aVertexUVs();

	void load() override;
	void unload() override;
};
