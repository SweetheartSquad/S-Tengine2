#pragma once

#include <iostream>

#include "ShaderLoader.h"
#include "FileUtils.h"
#include "GLUtils.h"

class ShaderInterface{
private:
	ShaderLoader * shader;	// Reference to the shader loader (why is the loader separate from the interface?)
	GLint aVertexPosition;	// The attribute location of the vertex position in the shader
	GLint aVertexColor;		// The attribute location of the color position in the shader
	GLint aVertexNormals;	// The attribute location of the normal position in the shader
	GLint aVertexUVs;		// The attribute location of the UV position in the shader

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

	// Returns the shader's program ID
	GLuint getProgramId();
	// Returns the attribute location of the vertex position in the shader
	GLint get_aVertexPosition();
	// Returns the attribute location of the vertex color in the shader
	GLint get_aVertexColor();
	// Returns the attribute location of the vertex normal in the shader
	GLint get_aVertexNormals();
	// Returns the attribute location of the vertex UV in the shader
	GLint get_aVertexUVs();
};
