#pragma once

#include "shader/Shader.h"
#include "shader/ShaderVariables.h"

void Shader::init(std::string _vertexShader, std::string _fragmentShader){
	vert = _vertexShader;
	frag = _fragmentShader;
	hasGeometryShader = false;
	//load();
	isCompiled = false;
	dirty = true;
}

void Shader::init(std::string _vertexShader, std::string _fragmentShader, std::string _geometryShader){
	vert = _vertexShader;
	frag = _fragmentShader;
	geom = _geometryShader;
	hasGeometryShader = true;
	//load();
	isCompiled = false;
	dirty = true;
}

Shader::Shader(bool _autoRelease) :
	NodeResource(_autoRelease),
	hasGeometryShader(false),
	numLightsUniformLocation(-1)
{
}

Shader::Shader(std::string _shaderSource, bool _hasGeometryShader, bool _autoRelease) :
	NodeResource(_autoRelease),
	hasGeometryShader(_hasGeometryShader),
	numLightsUniformLocation(-1)
{
	if (!hasGeometryShader){
		loadFromFile(_shaderSource + ".vert", _shaderSource + ".frag");
	}else{
		loadFromFile(_shaderSource + ".vert", _shaderSource + ".frag", _shaderSource + ".geom");
	}
}

Shader::Shader(std::string _vertexShaderSource, std::string _fragmentShaderSource, bool _autoRelease) :
	NodeResource(_autoRelease),
	hasGeometryShader(false)
{
	loadFromFile(_vertexShaderSource, _fragmentShaderSource);
}

Shader::Shader(std::string _vertexShaderSource, std::string _fragmentShaderSource, std::string _geometryShaderSource, bool _autoRelease) :
	NodeResource(_autoRelease),
	hasGeometryShader(true)
{
	loadFromFile(_vertexShaderSource, _fragmentShaderSource, _geometryShaderSource);
}

Shader::~Shader(void){
}

void Shader::load(){
	if(!loaded){
		//vert
		char * v = new char[vert.size() + 1];
		int vl = vert.length();
		memcpy(v, vert.c_str(), vl + 1);
		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, v, vl);
		checkForGlError(0,__FILE__,__LINE__);
		delete v;

		//frag
		char * f = new char[frag.size() + 1];
		int fl = frag.length();
		memcpy(f, frag.c_str(), fl + 1);
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, f, fl);
		checkForGlError(0,__FILE__,__LINE__);
		delete f;

		GLuint geometryShader = 0;
		if (hasGeometryShader){
			char * g = new char[geom.size() + 1];
			int gl = geom.length();
			memcpy(g, geom.c_str(), gl + 1);
			geometryShader = compileShader(GL_GEOMETRY_SHADER, g, gl);
			checkForGlError(0,__FILE__,__LINE__);
			delete g;
		}

		checkForGlError(0,__FILE__,__LINE__);
		programId = glCreateProgram();
		glAttachShader(programId, vertexShader);
		checkForGlError(0,__FILE__,__LINE__);
		glAttachShader(programId, fragmentShader);
		checkForGlError(0,__FILE__,__LINE__);
		if (hasGeometryShader){
			glAttachShader(programId, geometryShader);
		}
		glLinkProgram(programId);
		checkForGlError(0,__FILE__,__LINE__);

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

			// Provide the infolog in whatever manner you deem best.
			for(unsigned long int i = 0; i < infoLog.size(); ++i){
				std::cout << infoLog.at(i);
			}
			// Exit with failure.
		}
		checkForGlError(0,__FILE__,__LINE__);

		// Once the program is created, we don't need the actual shaders, so detach and delete them
		glDetachShader(programId, vertexShader);
		glDetachShader(programId, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (hasGeometryShader){
			glDeleteShader(geometryShader);
		}
		checkForGlError(0,__FILE__,__LINE__);

		// What happens if these aren't in the shader?
		aVertexPosition		= glGetAttribLocation(programId, GL_ATTRIBUTE_ID_VERTEX_POSITION.c_str());
		aVertexColor		= glGetAttribLocation(programId, GL_ATTRIBUTE_ID_VERTEX_COLOR.c_str());
		aVertexNormals		= glGetAttribLocation(programId, GL_ATTRIBUTE_ID_VERTEX_NORMALS.c_str());
		aVertexUVs			= glGetAttribLocation(programId, GL_ATTRIBUTE_ID_VERTEX_UVS.c_str());
		numLightsUniformLocation = glGetUniformLocation(programId, GL_UNIFORM_ID_NUM_LIGHTS.c_str());
	}
	
	NodeLoadable::load();
}

void Shader::unload(){
	if(loaded){
		if(glIsProgram(programId) == GL_TRUE){
			glDeleteProgram(programId);
			checkForGlError(0,__FILE__,__LINE__);
			programId = 0;
			aVertexPosition = -1;
			aVertexColor = -1;
			aVertexNormals = -1;
			aVertexUVs = -1;
			numLightsUniformLocation = -1;
			dirty = true;
			isCompiled = false;
		}else{
			std::cout << "not actually a shader?" << std::endl << std::endl;
		}
	}
	
	NodeLoadable::unload();
}

GLuint Shader::compileShader(GLenum _shaderType, char const* _source, int _length){
	GLuint shaderId = glCreateShader(_shaderType);
	glShaderSource(shaderId, 1, &_source, &_length);
	checkForGlError(0,__FILE__,__LINE__);
	glCompileShader(shaderId);
	checkForGlError(0,__FILE__,__LINE__);

	GLint status = 0;
	checkForGlError(0,__FILE__,__LINE__);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	checkForGlError(0,__FILE__,__LINE__);
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
		//std::cout << "Shader compiled succesfully." << std::endl << std::endl << _source << std::endl << std::endl;
	}
	checkForGlError(0,__FILE__,__LINE__);
	
	//isCompiled = true;
	return shaderId;
}

void Shader::loadFromFile(std::string _vertexShaderFile, std::string _fragmentShaderFile){
	vertSource = _vertexShaderFile;
	fragSource = _fragmentShaderFile;
	init(FileUtils::readFile(_vertexShaderFile), FileUtils::readFile(_fragmentShaderFile));
}

void Shader::loadFromFile(std::string _vertexShaderFile, std::string _fragmentShaderFile, std::string _geometryShaderFile){
	vertSource = _vertexShaderFile;
	fragSource = _fragmentShaderFile;
	geomSource = _geometryShaderFile;
	init(FileUtils::readFile(_vertexShaderFile), FileUtils::readFile(_fragmentShaderFile), FileUtils::readFile(_geometryShaderFile));
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

void Shader::printShader(){
	std::cout << 
		"_________VERTEX SHADER_________" << vertSource << ENDL << ENDL
		<< vert << ENDL << ENDL
		<< 
		"_________GEOMETRY SHADER_________" << geomSource << ENDL << ENDL
		<< geom << ENDL << ENDL 
		<< 
		"_________FRAGMENT SHADER_________" << fragSource << ENDL << ENDL
		<< frag << ENDL << ENDL;

}

bool Shader::isDirty() {
	return dirty;
}

void Shader::configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	//leave unimplemented
}

void Shader::clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable){
	if(dirty){
		configureUniforms(_matrixStack, _renderOption, _nodeRenderable);
		dirty = false;
	}
}

void Shader::makeDirty(){
	dirty = true;
}