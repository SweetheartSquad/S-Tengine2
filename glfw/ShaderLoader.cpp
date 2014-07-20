#include "ShaderLoader.h"

ShaderLoader::ShaderLoader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	programId = glCreateProgram();
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);

	glLinkProgram(programId);

	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);
}

ShaderLoader::~ShaderLoader(void){}

GLuint ShaderLoader::getProgramId(){
	return programId;
}

GLuint ShaderLoader::compileShader(GLenum shader, const char *source){
	GLuint shaderId = glCreateShader(shader);
	glShaderSource(shaderId, 1, &source, nullptr);
	glCompileShader(shaderId);
	return shaderId;
}
