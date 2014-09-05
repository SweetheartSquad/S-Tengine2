#include "ShaderLoader.h"

ShaderLoader::ShaderLoader(std::string vertexShaderSource, std::string fragmentShaderSource){
	//vert
	char * v = new char[vertexShaderSource.size() + 1];
	int vl = vertexShaderSource.length();
    memcpy(v, vertexShaderSource.c_str(), vertexShaderSource.size() + 1);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, v, vl);
	delete v;
	
	//frag
	char * f = new char[fragmentShaderSource.size() + 1];
	int fl = fragmentShaderSource.length();
    memcpy(f, fragmentShaderSource.c_str(), fragmentShaderSource.size() + 1);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, f, fl);
	delete f;
	
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
	GLUtils::checkForError(true,__FILE__,__LINE__);
	
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	
}

ShaderLoader::~ShaderLoader(void){}

GLuint ShaderLoader::getProgramId(){
	return programId;
}

GLuint ShaderLoader::compileShader(GLenum shaderType, const char* source, int length){
	GLUtils::checkForError(true,__FILE__,__LINE__);
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &source, &length);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	glCompileShader(shaderId);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	
	GLint status = 0;
	GLUtils::checkForError(true,__FILE__,__LINE__);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	if(status == GL_FALSE){
		std::cout << "\tERROR: Shader could not be compiled." << std::endl << std::endl << source << std::endl << std::endl;
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
