#include "ShaderInterface.h"

ShaderInterface::ShaderInterface(const char *vertexShaderSource, const char *fragmentShaderSource)
{
	GLUtils::checkForError(true,__FILE__,__LINE__);

	std::string vertexShaderString = FileUtils::voxReadFile(vertexShaderSource);
	std::string fragmentShaderString = FileUtils::voxReadFile(fragmentShaderSource);
	

	GLUtils::checkForError(true,__FILE__,__LINE__);
	shader = new ShaderLoader(vertexShaderString, fragmentShaderString);

	GLUtils::checkForError(true,__FILE__,__LINE__);

	aPositionVertex = glGetAttribLocation(getProgramId(), "aPositionVertex");
	aFragColor = glGetAttribLocation(getProgramId(), "aFragColor");
	aVertexNormals = glGetAttribLocation(getProgramId(), "aVertexNormals");

	/*aPositionVertex = 0;
	aFragColor = 1;
	aVertexNormals = 2;
	glBindAttribLocation(getProgramId(), aPositionVertex, "aPositionVertex");
	glBindAttribLocation(getProgramId(), aFragColor, "aFragColor");
	glBindAttribLocation(getProgramId(), aVertexNormals, "aVertexNormals");*/
	GLUtils::checkForError(true,__FILE__,__LINE__);
}

ShaderInterface::~ShaderInterface(void){
	delete shader;
}

GLuint ShaderInterface::getProgramId(){
	return shader->getProgramId();
}

GLint ShaderInterface::get_aPositionVertex(){
	return aPositionVertex;
}

GLint ShaderInterface::get_aFragColor(){
	return aFragColor;
}

GLint ShaderInterface::get_aVertexNormals(){
	return aVertexNormals;
}
