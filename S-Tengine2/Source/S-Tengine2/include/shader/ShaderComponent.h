#pragma once

#include <node/NodeLoadable.h>

#include <string>

#include <GL\glew.h> // imported here so we don't have to do it in every derived class with uniforms

class NodeRenderable;

namespace sweet{
	class MatrixStack;
}

class RenderOptions;
class Shader;
class ComponentShaderBase;

/********************************************************
*
* An abstract class used to define a shader component
* These components are used with ComponentShaderBase
*
* Each component should #define its type so that conditional
* shader compilation can be done depending on whcih components
* are present in ComponentShaderBase's component vector
*
*********************************************************/
class ShaderComponent abstract : public NodeLoadable{
public:
	ComponentShaderBase * shader;

	ShaderComponent(ComponentShaderBase * _shader);
	virtual ~ShaderComponent();

	/**
	* @returns The string containing vertex shader variables
	*/
	virtual std::string getVertexVariablesString() = 0;
	
	/**
	* @returns The String containing the fragment shader variables
	*/
	virtual std::string getFragmentVariablesString() = 0;
	
	/**
	* @returns The String containing the vertex shader body
	*/
	virtual std::string getVertexBodyString() = 0;
	
	/**
	* @returns The String containing the fragment shader body
	*/
	virtual std::string getFragmentBodyString() = 0;
	
	/**
	* @returns The String containing the modification to the 
	* fragment shaders out color. This is GL_OUT_OUT_COLOR in ShaderVariables.h
	*/
	virtual std::string getOutColorMod() = 0;

	virtual void configureUniforms(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable) = 0;
	virtual void clean(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable);
	void makeDirty();
private:
	bool dirty;
};