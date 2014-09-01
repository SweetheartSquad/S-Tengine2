#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLUtils.h"

class VertexInterface{
public:
	GLuint iboId;
	GLuint vaoId;
	GLuint vboId;
	GLenum drawMode;
	GLsizei vertCount;
	GLsizei stride;
	GLenum polygonalDrawMode;

public:
	VertexInterface(const GLvoid* vertexData, const GLvoid* indexData, GLsizeiptr vertexDataSize, GLsizeiptr indexDataSize, GLenum polygonalDrawMode, GLenum drawMode, GLsizei vertCount, GLsizei stride);
	~VertexInterface(void);

	GLuint getVertexInterfaceId();
	void renderVertexInterface();
	void configureVertexAttributes(GLint vertexHandle, unsigned long int _arity, int bufferOffset);
};

