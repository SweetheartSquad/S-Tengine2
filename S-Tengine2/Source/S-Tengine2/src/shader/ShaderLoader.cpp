#include "shader/ShaderLoader.h"

ShaderLoader::ShaderLoader(std::string _vertexShaderSource, std::string _fragmentShaderSource){
	//vertex shader
	char * v = new char[_vertexShaderSource.size() + 1];
	int vl = _vertexShaderSource.length();
	memcpy(v, _vertexShaderSource.c_str(), _vertexShaderSource.size() + 1);
	checkForGlError(true,__FILE__,__LINE__);
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, v, vl);
	delete v;

	//fragment shader
	char * f = new char[_fragmentShaderSource.size() + 1];
	int fl = _fragmentShaderSource.length();
	memcpy(f, _fragmentShaderSource.c_str(), _fragmentShaderSource.size() + 1);
	checkForGlError(true,__FILE__,__LINE__);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, f, fl);
	delete f;

	checkForGlError(true,__FILE__,__LINE__);

	programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	checkForGlError(true,__FILE__,__LINE__);
	glAttachShader(programId, fragmentShader);
	checkForGlError(true,__FILE__,__LINE__);
	glLinkProgram(programId);
	checkForGlError(true,__FILE__,__LINE__);

	//check for error with glLinkProgram
	GLint isLinked = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE){
		GLint maxLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength+1);
		glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);

		//The program is useless now. So delete it.
		glDeleteProgram(programId);

		//Provide the infolog in whatever manor you deem best.
		for(unsigned long int i = 0; i < infoLog.size(); ++i){
			std::cout << infoLog.at(i);
		}
		//Exit with failure.
	}
	checkForGlError(true,__FILE__,__LINE__);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	checkForGlError(true,__FILE__,__LINE__);
}

ShaderLoader::~ShaderLoader(void){}

GLuint ShaderLoader::getProgramId(){
	return programId;
}

GLuint ShaderLoader::compileShader(GLenum _shaderType, const char* _source, int _length){
	checkForGlError(true,__FILE__,__LINE__);
	GLuint shaderId = glCreateShader(_shaderType);
	glShaderSource(shaderId, 1, &_source, &_length);
	checkForGlError(true,__FILE__,__LINE__);
	glCompileShader(shaderId);
	checkForGlError(true,__FILE__,__LINE__);

	GLint status = 0;
	checkForGlError(true,__FILE__,__LINE__);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	checkForGlError(true,__FILE__,__LINE__);
	if(status == GL_FALSE){
		std::cout << "\tERROR: Shader could not be compiled." << std::endl << std::endl << _source << std::endl << std::endl;
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength+1);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

		//Print the error log
		std::cout << "\t";
		for(unsigned long int i = 0; i < errorLog.size(); ++i){
			std::cout << errorLog.at(i);
		}
		std::cout << std::endl;
		//Exit with failure.
		glDeleteShader(shaderId); //Don't leak the shader.
		return -1;
	}else{
		std::cout << "Shader compiled succesfully." << std::endl << std::endl << _source << std::endl << std::endl;
	}
	checkForGlError(true,__FILE__,__LINE__);

	return shaderId;
}