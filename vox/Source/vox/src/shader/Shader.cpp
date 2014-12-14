#pragma once

#include "shader/Shader.h"
#include "shader/ShaderVariables.h"

void Shader::init(std::string _vertexShaderSource, std::string _fragmentShaderSource){
	vertSource = _vertexShaderSource;
	fragSource = _fragmentShaderSource;
	hasGeometryShader = false;
	load();
}

void Shader::init(std::string _vertexShaderSource, std::string _fragmentShaderSource, std::string _geometryShaderSource){
	vertSource = _vertexShaderSource;
	fragSource = _fragmentShaderSource;
	geomSource = _geometryShaderSource;
	hasGeometryShader = true;
	load();
}

Shader::Shader(bool _autoRelease) : NodeResource(_autoRelease){
}

Shader::Shader(std::string _shaderSource, bool _hasGeometryShader, bool _autoRelease) :
	NodeResource(_autoRelease),
	hasGeometryShader(_hasGeometryShader)
{
	std::string _vertexShaderSource = FileUtils::voxReadFile(_shaderSource + ".vert");
	std::string _fragmentShaderSource = FileUtils::voxReadFile(_shaderSource + ".frag");
	
	if (hasGeometryShader){
		std::string _geometryShaderSource = FileUtils::voxReadFile(_shaderSource + ".geom");
		init(_vertexShaderSource, _fragmentShaderSource, _geometryShaderSource);
	}else{
		init(_vertexShaderSource, _fragmentShaderSource);
	}
}

Shader::Shader(std::string _vertexShaderSource, std::string _fragmentShaderSource, bool _autoRelease) :
	NodeResource(_autoRelease),
	hasGeometryShader(false)
{
	init(_vertexShaderSource, _fragmentShaderSource);
}

Shader::Shader(std::string _vertexShaderSource, std::string _fragmentShaderSource, std::string _geometryShaderSource, bool _autoRelease) :
	NodeResource(_autoRelease),
	hasGeometryShader(true){
		init(_vertexShaderSource, _fragmentShaderSource, _geometryShaderSource);
}

Shader::~Shader(void){
}

void Shader::load(){
	if(!loaded){
		//vert
		char * v = new char[vertSource.size() + 1];
		int vl = vertSource.length();
		memcpy(v, vertSource.c_str(), vl + 1);
		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, v, vl);
		GLUtils::checkForError(true,__FILE__,__LINE__);
		delete v;

		//frag
		char * f = new char[fragSource.size() + 1];
		int fl = fragSource.length();
		memcpy(f, fragSource.c_str(), fl + 1);
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, f, fl);
		GLUtils::checkForError(true,__FILE__,__LINE__);
		delete f;

		GLuint geometryShader = 0;
		if (hasGeometryShader){
			char * g = new char[geomSource.size() + 1];
			int gl = geomSource.length();
			memcpy(g, geomSource.c_str(), gl + 1);
			geometryShader = compileShader(GL_GEOMETRY_SHADER, g, gl);
			GLUtils::checkForError(true,__FILE__,__LINE__);
			delete g;
		}

		GLUtils::checkForError(true,__FILE__,__LINE__);
		programId = glCreateProgram();
		glAttachShader(programId, vertexShader);
		GLUtils::checkForError(true,__FILE__,__LINE__);
		glAttachShader(programId, fragmentShader);
		GLUtils::checkForError(true,__FILE__,__LINE__);
		if (hasGeometryShader){
			glAttachShader(programId, geometryShader);
		}
		glLinkProgram(programId);
		GLUtils::checkForError(true,__FILE__,__LINE__);

		// Check for error with glLinkProgram
		GLint isLinked = 0;
		glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
		if(isLinked == GL_FALSE){
			GLint maxLength = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength+1);
			glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);

			// The program is useless now. So delete it.
			glDeleteProgram(programId);

			// Provide the infolog in whatever manor you deem best.
			for(unsigned long int i = 0; i < infoLog.size(); ++i){
				std::cout << infoLog.at(i);
			}
			// Exit with failure.
		}
		GLUtils::checkForError(true,__FILE__,__LINE__);

		// Once the program is created, we don't need the actual shaders, so detach and delete them
		glDetachShader(programId, vertexShader);
		glDetachShader(programId, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (hasGeometryShader){
			glDeleteShader(geometryShader);
		}
		GLUtils::checkForError(true,__FILE__,__LINE__);

		// What happens if these aren't in the shader?
		aVertexPosition		= glGetAttribLocation(programId, GL_ATTRIBUTE_ID_VERTEX_POSITION.c_str());
		aVertexColor		= glGetAttribLocation(programId, GL_ATTRIBUTE_ID_VERTEX_COLOR.c_str());
		aVertexNormals		= glGetAttribLocation(programId, GL_ATTRIBUTE_ID_VERTEX_NORMALS.c_str());
		aVertexUVs			= glGetAttribLocation(programId, GL_ATTRIBUTE_ID_VERTEX_UVS.c_str());

		loaded = true;
	}
}

void Shader::unload(){
	if(loaded){
		loaded = false;
		glDeleteProgram(programId);
		programId = 0;
		aVertexPosition = -1;
		aVertexColor = -1;
		aVertexNormals = -1;
		aVertexUVs = -1;
	}
}

GLuint Shader::compileShader(GLenum _shaderType, char const* _source, int _length){
	GLUtils::checkForError(true,__FILE__,__LINE__);
	GLuint shaderId = glCreateShader(_shaderType);
	glShaderSource(shaderId, 1, &_source, &_length);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	glCompileShader(shaderId);
	GLUtils::checkForError(true,__FILE__,__LINE__);

	GLint status = 0;
	GLUtils::checkForError(true,__FILE__,__LINE__);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	GLUtils::checkForError(true,__FILE__,__LINE__);
	if(status == GL_FALSE){
		std::cout << "\tERROR: Shader could not be compiled." << std::endl << std::endl << _source << std::endl << std::endl;
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength+1);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

		//Provide the infolog in whatever manner you deem best.
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
	GLUtils::checkForError(true,__FILE__,__LINE__);

	return shaderId;
}

GLuint Shader::getProgramId(){
	return programId;
}

GLint Shader::get_aVertexPosition(){
	return aVertexPosition;
}

GLint Shader::get_aVertexColor(){
	return aVertexColor;
}

GLint Shader::get_aVertexNormals(){
	return aVertexNormals;
}


GLint Shader::get_aVertexUVs(){
	return aVertexUVs;
}

void Shader::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	//leave unimplemented
}
