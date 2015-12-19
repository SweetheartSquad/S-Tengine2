#pragma once

#include <GL\glew.h>
#include <Log.h>

#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#ifdef _DEBUG
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define checkForGlError(_printSuccess) do{\
	GLenum err = glGetError();\
	std::string errString;\
	do {\
		if((_printSuccess) || err != GL_NO_ERROR){\
			switch(err) {\
				case GL_NO_ERROR:						errString="NO_ERROR"; break;\
				case GL_INVALID_OPERATION:				errString="INVALID_OPERATION"; break;\
				case GL_INVALID_ENUM:					errString="INVALID_ENUM"; break;\
				case GL_INVALID_VALUE:					errString="INVALID_VALUE"; break;\
				case GL_OUT_OF_MEMORY:					errString="OUT_OF_MEMORY"; break;\
				case GL_INVALID_FRAMEBUFFER_OPERATION:  errString="INVALID_FRAMEBUFFER_OPERATION"; break;\
				default: errString = "dunno, but something went wrong?";	break;\
			}\
			std::stringstream ss;\
			ss << "OpenGL Error:\tFile: " << TOSTRING(__FILE__) <<", Line: "<< TOSTRING(__LINE__) << ", Log: " << errString;\
			Log::error(ss.str());\
		}\
	}while ((err = glGetError()) != GL_NO_ERROR);\
}while(false)
#else
#define checkForGlError(ps)
#endif

class GLUtils{
public:
	//static void checkForError(bool _printSuccess, std::string _file, int _line);

	/**
	* Takes a string representing the array variable, and the index to be reference
	* For example passing in lights[].position, 2 would return lights.position[2]
	* This can then be passed into openGL to find the attribute location for lights[2].position
	*/
	static std::string buildGLArrayReferenceString(std::string _value, unsigned long int _index);

	static void configureVertexAttributes(GLint _vertexHandle, unsigned long int _arity, int _bufferOffset, GLuint _vaoId, GLuint _vboId, GLsizei _stride);
};