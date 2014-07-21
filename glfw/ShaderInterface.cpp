#include "ShaderInterface.h"

ShaderInterface::ShaderInterface(const char *vertexShaderSource, const char *fragmentShaderSource)
{
	GLUtils::checkForError(true,__FILE__,__LINE__);
	vertexShaderString = FileUtils::voxReadFile(vertexShaderSource);
	fragmentShaderString =  FileUtils::voxReadFile(fragmentShaderSource);
	
	GLUtils::checkForError(true,__FILE__,__LINE__);
	shader = new ShaderLoader(vertexShaderString, fragmentShaderString);

	//std::free(vertexShaderString);
	//std::free(fragmentShaderString);

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
