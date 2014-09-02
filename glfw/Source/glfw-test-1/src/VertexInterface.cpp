#include "VertexInterface.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

VertexInterface::VertexInterface(const GLvoid* vertexData, const GLvoid* indexData, GLsizeiptr vertexDataSize, GLsizeiptr indexDataSize, GLenum polygonalDrawMode, GLenum drawMode, GLsizei vertCount, GLsizei stride){
	this->drawMode = drawMode;
	this->vertCount = vertCount;
	this->stride = stride;
	this->polygonalDrawMode = polygonalDrawMode;

	//vertex array object (VAO)
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	//vertex buffer object (VBO)
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	
	//index buffer object (IBO)
	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexData, drawMode);

	//fill out VBO
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, drawMode);

	//disable VAO
	glBindVertexArray(0);
	GLUtils::checkForError(true,__FILE__,__LINE__);
}

VertexInterface::~VertexInterface(void){
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &iboId);
	vaoId = 0;
	vboId = 0;
	iboId = 0;
}

void VertexInterface::renderVertexInterface(){
	glDrawArrays(polygonalDrawMode, 0, vertCount);
}

void VertexInterface::configureVertexAttributes(GLint vertexHandle, unsigned long int _arity, int bufferOffset){
	std::cout << "vertexHandle: " << vertexHandle << std::endl;
	if (vertexHandle != -1){
		glBindVertexArray(vaoId);//bind VAO, VBO, IBO
		GLUtils::checkForError(0,__FILE__,__LINE__);
		/*glBindBuffer(GL_ARRAY_BUFFER, vboId);
		GLUtils::checkForError(0,__FILE__,__LINE__);*/

		glEnableVertexAttribArray(vertexHandle);
		glVertexAttribPointer(vertexHandle, _arity, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(bufferOffset));
		
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		GLUtils::checkForError(0,__FILE__,__LINE__);
		glBindVertexArray(0);
	}
}