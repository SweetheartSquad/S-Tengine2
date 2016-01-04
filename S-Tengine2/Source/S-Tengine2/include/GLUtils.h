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
	GLenum opengl_error_code = glGetError();\
	std::string opengl_error_string;\
	do {\
		if((_printSuccess) || opengl_error_code != GL_NO_ERROR){\
			switch(opengl_error_code) {\
				case GL_NO_ERROR:						opengl_error_string = "NO_ERROR"; break;\
				case GL_INVALID_OPERATION:				opengl_error_string = "INVALID_OPERATION"; break;\
				case GL_INVALID_ENUM:					opengl_error_string = "INVALID_ENUM"; break;\
				case GL_INVALID_VALUE:					opengl_error_string = "INVALID_VALUE"; break;\
				case GL_OUT_OF_MEMORY:					opengl_error_string = "OUT_OF_MEMORY"; break;\
				case GL_INVALID_FRAMEBUFFER_OPERATION:  opengl_error_string = "INVALID_FRAMEBUFFER_OPERATION"; break;\
				default: opengl_error_string = "dunno, but something went wro ng?";	break;\
			}\
			std::stringstream opengl_error_stream;\
			opengl_error_stream << "OpenGL Error:\tFile: " << TOSTRING(__FILE__) <<", Line: "<< TOSTRING(__LINE__) << ", Log: " << opengl_error_string;\
			Log::error(opengl_error_stream.str());\
		}\
	}while ((opengl_error_code = glGetError()) != GL_NO_ERROR);\
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