#include "MeshInterface.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

MeshInterface::MeshInterface(GLenum polygonalDrawMode, GLenum drawMode):
	drawMode(drawMode),
	polygonalDrawMode(polygonalDrawMode),
	loaded(false),
	dirty(true)
{
	load();
	clean();
}

MeshInterface::~MeshInterface(void){
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &iboId);
	vaoId = 0;
	vboId = 0;
	iboId = 0;
	GLUtils::checkForError(0,__FILE__,__LINE__);
}

GLsizei MeshInterface::getStride(){
	return sizeof(Vertex);
}
GLsizei MeshInterface::getVertCount(){
	return vertices.size();
}

void MeshInterface::load(){
	if(!loaded){
		//vertex array object (VAO)
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		//vertex buffer object (VBO)
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), drawMode);
	
		//index buffer object (IBO)
		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*indices.size(), indices.data(), drawMode);

		//disable VAO
		glBindVertexArray(0);
		GLUtils::checkForError(true,__FILE__,__LINE__);
		loaded = true;
	}
}
void MeshInterface::clean(){
	if(dirty){
		//vertex buffer object (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*(vertices.size()), vertices.data(), GL_STATIC_DRAW);
		GLUtils::checkForError(0,__FILE__,__LINE__);

		//index buffer object (IBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*(indices.size()), indices.data(), GL_STATIC_DRAW);
		GLUtils::checkForError(0,__FILE__,__LINE__);
		dirty = false;
	}
}
void MeshInterface::render(ShaderInterface *shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
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

	//draw (note that the last argument is expecting a pointer to the indices, but since we have an ibo, it's actually interpreted as an offset)
	glDrawRangeElements(polygonalDrawMode, 0, vertices.size(), indices.size(), GL_UNSIGNED_BYTE, 0);
	GLUtils::checkForError(0,__FILE__,__LINE__);

	//disable VAO
	glBindVertexArray(0);
}

void MeshInterface::configureVertexAttributes(GLint vertexHandle, unsigned long int _arity, int bufferOffset){
	std::cout << "vertexHandle: " << vertexHandle << std::endl;
	if (vertexHandle != -1){
		glBindVertexArray(vaoId);

		glEnableVertexAttribArray(vertexHandle);
		glVertexAttribPointer(vertexHandle, _arity, GL_FLOAT, GL_FALSE, getStride(), BUFFER_OFFSET(bufferOffset));
		
		glBindVertexArray(0);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	}
}

void MeshInterface::pushVert(Vertex vertex){
	vertices.push_back(vertex);

	dirty = true;
}
void MeshInterface::setNormal(unsigned long int _vertId, float _x, float _y, float _z){
	vertices.at(_vertId).nx = _x;
	vertices.at(_vertId).ny = _y;
	vertices.at(_vertId).nz = _z;

	dirty = true;
}
void TriMesh::pushTri(GLubyte v0, GLubyte v1, GLubyte v2){
	indices.push_back(v0);
	indices.push_back(v1);
	indices.push_back(v2);

	dirty = true;
}
void QuadMesh::pushQuad(GLubyte v0, GLubyte v1, GLubyte v2, GLubyte v3){
	indices.push_back(v0);
	indices.push_back(v1);
	indices.push_back(v2);
	indices.push_back(v3);

	dirty = true;
}