#pragma once

#include <iostream>

#include "GLUtils.h"
#include "FileUtils.h"
#include "node/NodeResource.h"

class NodeRenderable;

namespace sweet{
	class MatrixStack;
}

class RenderOptions;

class Shader : public NodeResource{
	friend class SharedComponentShaderMethods;

protected:
	/** Compiles the given strings as source code for a shader (note that the actual shader code should be used, not a filename) */
	void init(std::string _vertexSource, std::string _fragmentSource);
	/** Compiles the given strings as source code for a shader (note that the actual shader code should be used, not a filename) */
	void init(std::string _vertexShaderSource, std::string _fragmentShaderSource, std::string _geometryShaderSource);
	
	GLint numLightsUniformLocation;
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
	bool dirty;

public:
	
	/** Filename of vertex shader */
	std::string vertSource;
	/** Filename of fragment shader */
	std::string fragSource;
	/** Filename of geometry shader */
	std::string geomSource;
	
	/** contents of vertex shader */
	std::string vert;
	/** contents of fragment shader */
	std::string frag;
	/** contents of geometry shader */
	std::string geom;

	// Whether the shader has been compiled
	bool isCompiled;

	explicit Shader(bool _autoRelease);
	/** Creates shader using "_shaderFile.vert", "_shaderFile.frag" and "_shaderFile.geom" */
	explicit Shader(std::string _shaderSource, bool _hasGeometryShader, bool _autoRelease);
	/**Creates shader using "vertexShaderFile" and "_fragmentShaderFile" */
	Shader(std::string _vertexShaderFile, std::string _fragmentShaderFile, bool _autoRelease);
	/**Creates shader using "vertexShaderFile", "_fragmentShaderFile" and "_geometryShaderFile" */
	Shader(std::string _vertexShaderFile, std::string _fragmentShaderFile, std::string _geometryShaderFile, bool _autoRelease);
	virtual ~Shader(void);
	GLuint compileShader(GLenum _shaderType, const char* _source, int _length);
	
	void loadFromFile(std::string _vertexShaderFile, std::string _fragmentShaderFile);
	void loadFromFile(std::string _vertexShaderFile, std::string _fragmentShaderFile, std::string _geometryShaderFile);


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
	/** Prints the shader to the console */
	void printShader();
	/** Returns whether the shader is dirty or not */
	bool isDirty();

	virtual void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable);
	virtual void clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable);
	virtual void makeDirty();

	void load() override;
	void unload() override;
};
