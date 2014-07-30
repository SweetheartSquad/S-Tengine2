#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const GLvoid* data, GLsizeiptr size, GLenum drawMode, GLsizei vertCount, GLsizei dataSize)
{
	this->drawMode = drawMode;
	this->vertCount = vertCount;
	this->dataSize = dataSize;
	
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
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}

void VertexBuffer::configureVertexAttributes(GLint vertexHandle)
{
	if (vertexHandle != -1)
	{
		glEnableVertexAttribArray(vertexHandle);
		glVertexAttribPointer(vertexHandle, 3, GL_FLOAT, GL_FALSE, dataSize, nullptr);
	}
}