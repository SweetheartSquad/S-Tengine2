#pragma once

#include <iostream>

#include "GLUtils.h"
#include "FileUtils.h"
#include "NodeResource.h"

class NodeRenderable;

namespace vox{
	class MatrixStack;
}

class RenderOptions;

class Shader : public NodeResource{

protected:
	/** Reads the files at the given location/name and compiles them as a vertex shader and a fragment shader */
	void init(std::string _vertexSource, std::string _fragmentSource);
	/** Reads the files at the given location/name and compiles them as a vertex shader and a fragment shader */
	void init(std::string _vertexShaderSource, std::string _fragmentShaderSource, std::string _geometryShaderSource);

private:

	/** The attribute location of the vertex position in the shader */
	GLint aVertexPosition;
	/** The attribute location of the color position in the shader */
	GLint aVertexColor;
	/** The attribute location of the normal position in the shader */
	GLint aVertexNormals;
	/** The attribute location of the UV position in the shader */
	GLint aVertexUVs;
	GLuint programId;

	bool hasGeometryShader;

public:

	/** Filename of vertex shader */
	std::string vertSource;
	/** Filename of fragment shader */
	std::string fragSource;
	/** Filename of geometry shader */
	std::string geomSource;

	explicit Shader(bool _autoRelease);
	/** Creates shader using "_shaderFile.vert", "_shaderFile.frag" and "_shaderFile.geom" */
	explicit Shader(std::string _shaderSource, bool _hasGeometryShader, bool _autoRelease);
	/**Creates shader using "vertexShaderFile" and "_fragmentShaderFile" */
	Shader(std::string _vertexShaderFile, std::string _fragmentShaderFile, bool _autoRelease);
	/**Creates shader using "vertexShaderFile", "_fragmentShaderFile" and "_geometryShaderFile" */
	Shader(std::string _vertexShaderFile, std::string _fragmentShaderFile, std::string _geometryShaderFile, bool _autoRelease);
	~Shader(void);
	GLuint compileShader(GLenum _shaderType, const char* _source, int _length);

	/** Returns the shader's program ID */
	GLuint getProgramId();
	/** Returns the attribute location of the vertex position in the shader */
	GLint get_aVertexPosition();
	/** Returns the attribute location of the vertex color in the shader */
	GLint get_aVertexColor();
	/** Returns the attribute location of the vertex normal in the shader */
	GLint get_aVertexNormals();
	/** Returns the attribute location of the vertex UV in the shader */
	GLint get_aVertexUVs();

	virtual void configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable);

	void load() override;
	void unload() override;
};
