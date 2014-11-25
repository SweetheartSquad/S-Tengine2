#pragma once 

#include "BaseShader.h"

BaseShader::BaseShader() :
	Shader(buildVertexShader(), buildFragmentShader(), false){

}

BaseShader::BaseShader(std::vector<ShaderComponent*> _components) :
	components(_components),
	Shader(buildVertexShader(), buildFragmentShader(), false)
{
}

std::string BaseShader::buildVertexShader(){
	std::string shaderString  = "#version 150\n" 
							    "#extension GL_ARB_explicit_attrib_location : enable\n"
								"layout(location = 1) in vec3 aVertexPosition;\n"
								"layout(location = 2) in vec4 aVertexColor;\n"
								"layout(location = 3) in vec3 aVertexNormals;\n"
								"layout(location = 4) in vec2 aVertexUVs;\n"

								"uniform mat4 MVP;\n"
								"uniform mat4 depthMVP;\n"

								"out vec3 fragVert;\n"
								"out vec3 fragNormal;\n"
								"out vec4 fragColor;\n"
								"out vec2 fragUV;\n";

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getVertexVariablesString();
	}

	shaderString += "void main(){\n"
						"fragVert = aVertexPosition;\n"
						"fragNormal = aVertexNormals;\n"
						"fragColor = aVertexColor;\n"
						"fragUV = aVertexUVs;\n"
						"gl_Position = MVP * vec4(aVertexPosition, 1.0);\n";

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getVertexBodyString();
	}

	shaderString += "}";
	return shaderString;
}

std::string BaseShader::buildFragmentShader(){
	std::string shaderString  = "#version 150\n"

								"in vec3 fragVert;\n"
								"in vec3 fragNormal;\n"
								"in vec4 fragColor;\n"
								"in vec2 fragUV;\n"
								"in vec4 shadowCoord;\n"

								"out vec4 outColor;\n";

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getFragmentVariablesString();
	}

	shaderString += "void main(){\n";

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getFragmentBodyString();
	}

	shaderString += "outColor = fragColor;\n";

	for(unsigned long int i = 0; i < components.size(); i++){
		shaderString += components.at(i)->getOutColorMod();
	}

	shaderString += "}";
	return shaderString;
}

void BaseShader::compileShader(){
	Shader::init(buildVertexShader(), buildFragmentShader());
}

BaseShader::~BaseShader(){
}

