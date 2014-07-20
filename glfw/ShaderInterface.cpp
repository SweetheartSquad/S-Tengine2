#include "ShaderInterface.h"

ShaderInterface::ShaderInterface(const char *vertexShaderSource, const char *fragmentShaderSource)
{
	vertexShaderString = FileUtils::voxReadFile(vertexShaderSource);
	fragmentShaderString =  FileUtils::voxReadFile(fragmentShaderSource);
	
	shader = new ShaderLoader(vertexShaderString, fragmentShaderString);

	//std::free(vertexShaderString);
	//std::free(fragmentShaderString);

	aPositionVertex = glGetAttribLocation(getProgramId(), "aPositionVertex");
	uColour = glGetUniformLocation(getProgramId(), "uColour");
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
