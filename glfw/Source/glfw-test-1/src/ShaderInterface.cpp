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
	aVertexUVs			= glGetAttribLocation(getProgramId(), "aVertexUVs");

	GLUtils::checkForError(true,__FILE__,__LINE__);
}

ShaderInterface::ShaderInterface(std::string _shaderSource, bool _autoRelease) : ReferenceManager(_autoRelease){
	init(_shaderSource+".vert", _shaderSource+".frag");
}

ShaderInterface::ShaderInterface(std::string _vertexShaderSource, std::string _fragmentShaderSource, bool _autoRelease) : ReferenceManager(_autoRelease){
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

GLint ShaderInterface::get_aVertexUVs(){
	return aVertexUVs;
}