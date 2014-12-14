#pragma once

#include <GL\glew.h>
#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class GLUtils{
public:
	static void checkForError(bool _printSuccess, std::string _file, int _line);

	/**
	* Takes a string representing the array variable, and the index to be reference
	* For example passing in lights[].position, 2 would return lights.position[2]
	* This can then be passed into openGL to find the attribute location for lights[2].position
	*/
	static std::string buildGLArrayReferenceString(std::string _value, unsigned long int _index);

	static void configureVertexAttributes(GLint _vertexHandle, unsigned long int _arity, int _bufferOffset, GLuint _vaoId, GLsizei _stride);
};