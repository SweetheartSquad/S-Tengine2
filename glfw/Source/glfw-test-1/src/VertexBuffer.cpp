#include "VertexBuffer.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

VertexBuffer::VertexBuffer(const GLvoid* data, GLsizeiptr size, GLenum polygonalDrawMode, GLenum drawMode, GLsizei vertCount, GLsizei stride)
{
	this->drawMode = drawMode;
	this->vertCount = vertCount;
	this->stride = stride;
	this->polygonalDrawMode = polygonalDrawMode;

	GLUtils::checkForError(true,__FILE__,__LINE__);
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, size, data, drawMode);
	
	GLUtils::checkForError(true,__FILE__,__LINE__);
}

VertexBuffer::~VertexBuffer(void)
{
	glDeleteBuffers(1, &vertexBufferId);
	vertexBufferId = 0;
}

GLuint VertexBuffer::getVertexBufferId()
{
	return vertexBufferId;
}

void VertexBuffer::renderVertexBuffer()
{
	glDrawArrays(polygonalDrawMode, 0, vertCount);
}

void VertexBuffer::configureVertexAttributes(GLint vertexHandle, unsigned long int _arity, int bufferOffset){
	std::cout << "vertexHandle: " << vertexHandle << std::endl;
	if (vertexHandle != -1){
		glEnableVertexAttribArray(vertexHandle);
		glVertexAttribPointer(vertexHandle, _arity, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(bufferOffset));
	}
}