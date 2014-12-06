#pragma once

#include <string>

/********************************************************
*
* An abstract class used to define a shader component
* These components are used with BaseComponentShader
*
* Each component should #define its type so that conditional
* shader compilation can be done depending on whcih components
* are present in BaseComponentShader's component vector
*
*********************************************************/
class ShaderComponent abstract{
public:
	ShaderComponent();
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
};