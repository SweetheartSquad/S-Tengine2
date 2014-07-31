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
	GLenum polygonalDrawMode;

public:
	VertexBuffer(const GLvoid *data, GLsizeiptr size, GLenum polygonalDrawMode, GLenum drawMode, GLsizei vertCount, GLsizei dataSize);
	~VertexBuffer(void);

	GLuint getVertexBufferId();
	void renderVertexBuffer();
	void configureVertexAttributes(GLint vertexHandle);
};

