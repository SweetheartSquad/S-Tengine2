#include "ShaderInterface.h"

void ShaderInterface::init(std::string vertexShaderFile, std::string fragmentShaderFile){

	//read source files into strings
	std::string vertexShaderString		= FileUtils::voxReadFile(vertexShaderFile);
	std::string fragmentShaderString	= FileUtils::voxReadFile(fragmentShaderFile);

	shader = new ShaderLoader(vertexShaderString, fragmentShaderString);
	GLUtils::checkForError(true,__FILE__,__LINE__);

	aVertexPosition		= glGetAttribLocation(getProgramId(), "aVertexPosition");
	aVertexColor		= glGetAttribLocation(getProgramId(), "aVertexColor");
	aVertexNormals		= glGetAttribLocation(getProgramId(), "aVertexNormals");
	GLUtils::checkForError(true,__FILE__,__LINE__);
}

ShaderInterface::ShaderInterface(std::string shaderSource){
	init(shaderSource+".vert", shaderSource+".frag");
}

ShaderInterface::ShaderInterface(std::string vertexShaderSource, std::string fragmentShaderSource){
	init(vertexShaderSource, fragmentShaderSource);
}

ShaderInterface::~ShaderInterface(void){
	delete shader;
	shader = nullptr;
}

GLuint ShaderInterface::getProgramId(){
	return shader->getProgramId();
}

GLint ShaderInterface::get_aVertexPosition(){
	return aVertexPosition;
}

GLint ShaderInterface::get_aVertexColor(){
	return aVertexColor;
}

GLint ShaderInterface::get_aVertexNormals(){
	return aVertexNormals;
}
