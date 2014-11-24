#pragma once 

#include "BaseShader.h"

BaseShader::BaseShader() : Shader(buildVertexShader(), buildFragmentShader(), false){

}

BaseShader::BaseShader(std::vector<ShaderComponent*>* _components) :
	components(_components),
	Shader(buildVertexShader(), buildFragmentShader(), false)
{
}

std::string BaseShader::buildVertexShader(){
	std::string shaderString  = "#version 150" 
							    "#extension GL_ARB_explicit_attrib_location : enable"
								"layout(location = 1) in vec3 aVertexPosition;"
								"layout(location = 2) in vec4 aVertexColor;"
								"layout(location = 3) in vec3 aVertexNormals;"
								"layout(location = 4) in vec2 aVertexUVs;"

								"uniform mat4 MVP;"
								"uniform mat4 depthMVP;"

								"out vec3 fragVert;"
								"out vec3 fragNormal;"
								"out vec4 fragColor;"
								"out vec2 fragUV;";

	for(unsigned long int i = 0; i < components->size(); i++){
		shaderString += components->at(i)->getVertexVariablesString();
	}

	shaderString += "void main(){"
						"fragVert = aVertexPosition;"
						"fragNormal = aVertexNormals;"
						"fragColor = aVertexColor;"
						"fragUV = aVertexUVs;"
						"gl_Position = MVP * vec4(aVertexPosition, 1.0);";

	for(unsigned long int i = 0; i < components->size(); i++){
		shaderString += components->at(i)->getVertexBodyString();
	}

	shaderString += "}";
	return shaderString;
}

std::string BaseShader::buildFragmentShader(){
	std::string shaderString  = "#version 150"

								"in vec3 fragVert;"
								"in vec3 fragNormal;"
								"in vec4 fragColor;"
								"in vec2 fragUV;"
								"in vec4 shadowCoord;"

								"out vec4 outColor;";

	for(unsigned long int i = 0; i < components->size(); i++){
		shaderString += components->at(i)->getFragmentVariablesString();
	}

	shaderString += "void main(){"
					"fragVert = aVertexPosition;"
					"fragNormal = aVertexNormals;"
					"fragColor = aVertexColor;"
					"fragUV = aVertexUVs;"
					"gl_Position = MVP * vec4(aVertexPosition, 1.0);";

	for(unsigned long int i = 0; i < components->size(); i++){
		shaderString += components->at(i)->getFragmentBodyString();
	}

	shaderString += "outColor = vec4(1, 1, 1, 1);";

	for(unsigned long int i = 0; i < components->size(); i++){
		shaderString += components->at(i)->getOutColorMod();
	}

	shaderString += "}";
	return shaderString;
}

void BaseShader::compileShader(){
	Shader::init(buildVertexShader(), buildFragmentShader());
}

BaseShader::~BaseShader(){
}

