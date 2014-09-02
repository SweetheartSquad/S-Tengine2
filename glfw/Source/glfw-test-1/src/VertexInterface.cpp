#include "VertexInterface.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

VertexInterface::VertexInterface(GLenum polygonalDrawMode, GLenum drawMode){
	this->drawMode = drawMode;
	this->polygonalDrawMode = polygonalDrawMode;
	
	vertices = new std::vector<Vertex>();
	indices = new std::vector<GLubyte>();

	loaded = false;
	dirty = true;

	load();
	clean();

}

VertexInterface::~VertexInterface(void){
	delete vertices;
	delete indices;

	vertices = nullptr;
	indices = nullptr;

	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &iboId);
	vaoId = 0;
	vboId = 0;
	iboId = 0;
	GLUtils::checkForError(0,__FILE__,__LINE__);
}

GLsizei VertexInterface::getStride(){
	return sizeof(Vertex);
}
GLsizei VertexInterface::getVertCount(){
	return vertices->size();
}

void VertexInterface::load(){
	if(!loaded){
		//vertex array object (VAO)
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		//vertex buffer object (VBO)
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices->size(), vertices->data(), drawMode);
	
		//index buffer object (IBO)
		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*indices->size(), indices->data(), drawMode);

		//disable VAO
		glBindVertexArray(0);
		GLUtils::checkForError(true,__FILE__,__LINE__);
		loaded = true;
	}
}
void VertexInterface::clean(){
	if(dirty){
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*(vertices->size()), vertices->data(), GL_STATIC_DRAW);
		GLUtils::checkForError(0,__FILE__,__LINE__);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*(indices->size()), indices->data(), GL_STATIC_DRAW);
		GLUtils::checkForError(0,__FILE__,__LINE__);
		dirty = false;
	}
}
void VertexInterface::render(ShaderInterface *shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	//bind VAO
	glBindVertexArray(vaoId);
	GLUtils::checkForError(0,__FILE__,__LINE__);

	//specify shader attributes
	glUseProgram(shader->getProgramId());
	GLUtils::checkForError(0,__FILE__,__LINE__);
	glm::mat4 mvp = projectionMatrix * viewMatrix * vox::currentModelMatrix;  	
	GLuint mvpUniformLocation = glGetUniformLocation(shader->getProgramId(), "MVP");
	glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	GLUtils::checkForError(0,__FILE__,__LINE__);

	//draw
	glDrawRangeElements(polygonalDrawMode, 0, vertices->size(), indices->size(), GL_UNSIGNED_BYTE, 0);
	GLUtils::checkForError(0,__FILE__,__LINE__);

	//disable VAO
	glBindVertexArray(0);
}

void VertexInterface::configureVertexAttributes(GLint vertexHandle, unsigned long int _arity, int bufferOffset){
	std::cout << "vertexHandle: " << vertexHandle << std::endl;
	if (vertexHandle != -1){
		glBindVertexArray(vaoId);//bind VAO, VBO, IBO

		glEnableVertexAttribArray(vertexHandle);
		glVertexAttribPointer(vertexHandle, _arity, GL_FLOAT, GL_FALSE, getStride(), BUFFER_OFFSET(bufferOffset));
		
		glBindVertexArray(0);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	}
}