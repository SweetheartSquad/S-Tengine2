#include "MeshInterface.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <algorithm>

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
		glBindVertexArray(0);

		//_vertex array object (VAO)
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		//_vertex buffer object (VBO)
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), drawMode);

		//index buffer object (IBO)
		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*indices.size(), indices.data(), drawMode);

		//Initialize textures
		for (Texture texture : textures){
			texture.init();
		}

		//disable VAO
		glBindVertexArray(0);
		GLUtils::checkForError(true,__FILE__,__LINE__);
		loaded = true;
	}
}

void MeshInterface::unload(){
	if(loaded){
		glDeleteBuffers(1, &iboId);
		glDeleteBuffers(1, &vboId);
		glDeleteVertexArrays(1, &vaoId);
		for (Texture texture : textures){
			texture.unload();
		}
		iboId = 0;
		vboId = 0;
		vaoId = 0;

		loaded = false;
		dirty = true;
	}
}

void MeshInterface::clean(){
	if(dirty){
		//_vertex buffer object (VBO)
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

void MeshInterface::render(ShaderInterface * shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	if(glIsVertexArray(vaoId) == GL_TRUE){
		if(glIsBuffer(vboId) == GL_TRUE){
			if(glIsBuffer(iboId) == GL_TRUE){
				GLUtils::checkForError(0,__FILE__,__LINE__);
				//bind VAO
				glBindVertexArray(vaoId);
				GLUtils::checkForError(0,__FILE__,__LINE__);

				//specify shader attributes
				glUseProgram(shader->getProgramId());

				//Pass the shader the number of textures
				glUniform1i(glGetUniformLocation(shader->getProgramId(), "numTextures"), textures.size());

				//Bind each texture to the texture sampler array in the frag shader
				for(int i = 0; i < textures.size(); i++){
					glUniform1i(glGetUniformLocation(shader->getProgramId(), "textureSampler"), i);
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, textures.at(i).textureId);
				}

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
			}else{
				std::cout << "ibo bad" << std::endl << std::endl;
			}
		}else{
			std::cout << "vbo bad" << std::endl << std::endl;
		}
	}else{
		std::cout << "vao bad" << std::endl << std::endl;
	}
}

void MeshInterface::configureVertexAttributes(GLint _vertexHandle, unsigned long int _arity, int _bufferOffset){
	if (_vertexHandle != -1){
		glBindVertexArray(vaoId);

		glEnableVertexAttribArray(_vertexHandle);
		glVertexAttribPointer(_vertexHandle, _arity, GL_FLOAT, GL_FALSE, getStride(), BUFFER_OFFSET(_bufferOffset));

		glBindVertexArray(0);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	}
}

void MeshInterface::configureDefaultVertexAttributes(ShaderInterface *_shader){
	configureVertexAttributes(_shader->get_aVertexPosition(), 3, 0);
	configureVertexAttributes(_shader->get_aVertexColor(), 4, sizeof(float)*3);
	configureVertexAttributes(_shader->get_aVertexNormals(), 3, sizeof(float)*7);
	configureVertexAttributes(_shader->get_aVertexUVs(), 2, sizeof(float)*10);
}

void MeshInterface::pushVert(Vertex _vertex){
	vertices.push_back(_vertex);
	dirty = true;
}

void MeshInterface::pushTextrue2D(const char* _src, int _width, int _height){
	textures.push_back(Texture(_src, _width, _height, true));
}

void MeshInterface::setNormal(unsigned long int _vertId, float _x, float _y, float _z){
	vertices.at(_vertId).nx = _x;
	vertices.at(_vertId).ny = _y;
	vertices.at(_vertId).nz = _z;

	dirty = true;
}

void MeshInterface::setUV(unsigned long _vertId, float _u, float _v){
	vertices.at(_vertId).u = _u;
	vertices.at(_vertId).v = _v;
}
void TriMesh::pushTri(GLubyte _v0, GLubyte _v1, GLubyte _v2){
	indices.push_back(_v0);
	indices.push_back(_v1);
	indices.push_back(_v2);

	dirty = true;
}
void QuadMesh::pushQuad(GLubyte _v0, GLubyte _v1, GLubyte _v2, GLubyte _v3){
	indices.push_back(_v0);
	indices.push_back(_v1);
	indices.push_back(_v2);
	indices.push_back(_v3);

	dirty = true;
}