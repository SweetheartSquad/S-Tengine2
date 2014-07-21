#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLUtils.h"

class VertexBuffer
{
private:

	GLuint vertexBufferId;
	GLenum drawMode;
	GLsizei vertCount;
	GLsizei dataSize;

public:
	VertexBuffer(const GLvoid *data, GLsizeiptr size, GLenum drawMode, GLsizei vertCount, GLsizei dataSize);
	~VertexBuffer(void);

	GLuint getVertexBufferId();
	void renderVertexBuffer();
	void configureVertexAttributes(GLint vertexHandle);
};

