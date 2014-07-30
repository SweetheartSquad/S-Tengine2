#include "ShaderLoader.h"

ShaderLoader::ShaderLoader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	GLUtils::checkForError(true,__FILE__,__LINE__);
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	
	programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	glAttachShader(programId, fragmentShader);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	
	glLinkProgram(programId);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	
	//check for error with glLinkProgram
	GLint isLinked = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE)
	{
			GLint maxLength = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);
 
			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength+1);
			glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);
 
			//The program is useless now. So delete it.
			glDeleteProgram(programId);
 
			//Provide the infolog in whatever manor you deem best.
			for(int i = 0; i < infoLog.size(); ++i){
				std::cout << infoLog.at(i);
			}
			//Exit with failure.
	}
	GLUtils::checkForError(true,__FILE__,__LINE__);
	
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	
}

ShaderLoader::~ShaderLoader(void){}

GLuint ShaderLoader::getProgramId(){
	return programId;
}

GLuint ShaderLoader::compileShader(GLenum shaderType, const char* source){
	GLUtils::checkForError(true,__FILE__,__LINE__);
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &source, nullptr);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	glCompileShader(shaderId);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	
	GLint status = 0;
	GLUtils::checkForError(true,__FILE__,__LINE__);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	if(status == GL_FALSE){
		std::cout << "ERROR: Shader could not be compiled." << std::endl << std::endl << source << std::endl << std::endl;
		GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
 
        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength+1);
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);
 
        //Provide the infolog in whatever manner you deem best.
		std::cout << "\t";
		for(int i = 0; i < errorLog.size(); ++i){
			std::cout << errorLog.at(i);
		}
		std::cout << std::endl;
        //Exit with failure.
        glDeleteShader(shaderId); //Don't leak the shader.
        return -1;
	}else{
		std::cout << "Shader compiled succesfully." << std::endl << std::endl << source << std::endl << std::endl;
	}
	GLUtils::checkForError(true,__FILE__,__LINE__);
	
	return shaderId;
}
