#include "ShaderInterface.h"

ShaderInterface::ShaderInterface(const char *vertexShaderSource, const char *fragmentShaderSource)
{
	GLUtils::checkForError(true,__FILE__,__LINE__);

	std::string vertexShaderString = FileUtils::voxReadFile(vertexShaderSource);
	std::string fragmentShaderString = FileUtils::voxReadFile(fragmentShaderSource);

	//vertexShaderString = FileUtils::voxReadFile(vertexShaderSource);
	//fragmentShaderString = FileUtils::voxReadFile(fragmentShaderSource);
	
	char * v = new char[vertexShaderString.size() + 1];
    memcpy(v, vertexShaderString.c_str(), vertexShaderString.size() + 1);
	char * f = new char[fragmentShaderString.size() + 1];
    memcpy(f, fragmentShaderString.c_str(), fragmentShaderString.size() + 1);

	GLUtils::checkForError(true,__FILE__,__LINE__);
	shader = new ShaderLoader(v, f);

	delete v;
	delete f;

	GLUtils::checkForError(true,__FILE__,__LINE__);
	aPositionVertex = glGetAttribLocation(getProgramId(), "aPositionVertex");
	uColour = glGetUniformLocation(getProgramId(), "uColour");
	GLUtils::checkForError(true,__FILE__,__LINE__);
}

ShaderInterface::~ShaderInterface(void)
{
	delete shader;
}

GLuint ShaderInterface::getProgramId()
{
	return shader->getProgramId();
}

GLint ShaderInterface::get_aPositionVertex()
{
	return aPositionVertex;
}

GLint ShaderInterface::get_uColourShader()
{
	return uColour;
}
