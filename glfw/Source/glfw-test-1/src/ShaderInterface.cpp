#include "ShaderInterface.h"

void ShaderInterface::init(std::string _vertexShaderFile, std::string _fragmentShaderFile){
	vertName = _vertexShaderFile;
	fragName = _fragmentShaderFile;

	//read source files into strings
	std::string vertexShaderString		= FileUtils::voxReadFile(_vertexShaderFile);
	std::string fragmentShaderString	= FileUtils::voxReadFile(_fragmentShaderFile);

	shader = new ShaderLoader(vertexShaderString, fragmentShaderString);
	GLUtils::checkForError(true,__FILE__,__LINE__);

	aVertexPosition		= glGetAttribLocation(getProgramId(), "aVertexPosition");
	aVertexColor		= glGetAttribLocation(getProgramId(), "aVertexColor");
	aVertexNormals		= glGetAttribLocation(getProgramId(), "aVertexNormals");
	GLUtils::checkForError(true,__FILE__,__LINE__);
}

ShaderInterface::ShaderInterface(std::string _shaderSource){
	init(_shaderSource+".vert", _shaderSource+".frag");
}

ShaderInterface::ShaderInterface(std::string _vertexShaderSource, std::string _fragmentShaderSource){
	init(_vertexShaderSource, _fragmentShaderSource);
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
