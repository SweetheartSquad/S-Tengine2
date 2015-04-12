#pragma once

#include "MeshInterface.h"
#include "RenderOptions.h"
#include "Texture.h"
#include "Material.h"
#include "MatrixStack.h"
#include "VoxRenderOptions.h"
#include "Transform.h"

MeshInterface::MeshInterface(GLenum polygonalDrawMode, GLenum drawMode) :
	NodeRenderable(),
	dirty(true),
	texturesDirty(true),
	drawMode(drawMode),
	polygonalDrawMode(polygonalDrawMode),
	uvEdgeMode(GL_CLAMP_TO_EDGE)
{
	load();
	clean();
}

MeshInterface::~MeshInterface(){
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &iboId);
	for(Texture * t : textures){
		t->decrementAndDelete();
	}
	for(Material * m : materials){
		m->decrementAndDelete();
	}
	vaoId = 0;
	vboId = 0;
	iboId = 0;
	checkForGlError(0,__FILE__,__LINE__);
}

GLsizei MeshInterface::getStride(){
	return sizeof(Vertex);
}

GLsizei MeshInterface::getVertCount(){
	return vertices.size();
}

void MeshInterface::load(){
	if(!loaded){
		checkForGlError(0,__FILE__,__LINE__);

		glBindVertexArray(0);

		// Vertex Array Object (VAO)
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		// Vertex Auffer Object (VBO)
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), drawMode);

		// Index Buffer Object (IBO)
		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), drawMode);
		
		checkForGlError(0,__FILE__,__LINE__);

		// Initialize textures
		for (Texture * texture : textures){
			texture->load();
		}
		// Disable VAO
		glBindVertexArray(0);
		checkForGlError(0,__FILE__,__LINE__);
	}
	
	NodeLoadable::load();
}

void MeshInterface::unload(){
	if(loaded){
		glDeleteBuffers(1, &iboId);
		glDeleteBuffers(1, &vboId);
		glDeleteVertexArrays(1, &vaoId);
		for (Texture * texture : textures){
			texture->unload();
		}
		iboId = 0;
		vboId = 0;
		vaoId = 0;

		dirty = true;
		checkForGlError(0,__FILE__,__LINE__);
	}
	
	NodeLoadable::unload();
}

void MeshInterface::clean(){
	if(dirty){
		// Vertex Buffer Object (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (vertices.size()), vertices.data(), GL_DYNAMIC_DRAW);
		checkForGlError(0,__FILE__,__LINE__);

		// Index Buffer Object (IBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * (indices.size()), indices.data(), GL_DYNAMIC_DRAW);
		checkForGlError(0,__FILE__,__LINE__);
		dirty = false;
	}
}

void MeshInterface::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption){
	if(glIsVertexArray(vaoId) == GL_TRUE){
		if(glIsBuffer(vboId) == GL_TRUE){
			if(glIsBuffer(iboId) == GL_TRUE){

				if(_renderOption->shader != nullptr){		
					if(_renderOption->currentVao != vaoId){
						_renderOption->currentVao = vaoId;	
						// Bind VAO
						glBindVertexArray(vaoId);
						checkForGlError(0,__FILE__, __LINE__);
					}

					//This should help performance but there's a bit of a problem with it at the moment so I'll comment it out
					//if(_renderOption->shader->getProgramId() != _renderOption->currentlyBoundShaderId) {
						_renderOption->currentlyBoundShaderId = _renderOption->shader->getProgramId();
						glUseProgram(_renderOption->shader->getProgramId());
						checkForGlError(0,__FILE__,__LINE__);
					//}

					_renderOption->shader->clean(_matrixStack, _renderOption, this);	

					//Alpha blending
					// Should these be here or only once in the main render loop?
					glEnable (GL_BLEND);
					glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					//Texture repeat
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uvEdgeMode);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, uvEdgeMode);

					//}

					// Draw (note that the last argument is expecting a pointer to the indices, but since we have an ibo, it's actually interpreted as an offset)
					glDrawRangeElements(polygonalDrawMode, 0, indices.size(), indices.size(), GL_UNSIGNED_INT, 0);
					//glDrawRangeElements(polygonalDrawMode, 0, indices.size(), indices.size(), GL_UNSIGNED_INT, 0);
					//glDrawElements(drawMode, vertices.size(), GL_UNSIGNED_BYTE, 0);
					checkForGlError(0,__FILE__,__LINE__);

					// Disable VAO
					//glBindVertexArray(0);
				}else{
					std::cout << "no shader" << std::endl << std::endl;	
				}
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


void MeshInterface::configureDefaultVertexAttributes(Shader *_shader){
	GLUtils::configureVertexAttributes(_shader->get_aVertexPosition(), 3, 0, vaoId, getStride());
	GLUtils::configureVertexAttributes(_shader->get_aVertexColor(), 4, sizeof(float) * 3, vaoId, getStride());
	GLUtils::configureVertexAttributes(_shader->get_aVertexNormals(), 3, sizeof(float) * 7, vaoId, getStride());
	GLUtils::configureVertexAttributes(_shader->get_aVertexUVs(), 2, sizeof(float) * 10, vaoId, getStride());
}

void MeshInterface::pushVert(Vertex _vertex){
	vertices.push_back(_vertex);
	indices.push_back(vertices.size() - 1);
	dirty = true;
}

void MeshInterface::popTexture2D(){
	textures.pop_back();
}

void MeshInterface::removeTextureAt(int _idx){
	textures.erase(textures.begin() + _idx);
}

void MeshInterface::pushTexture2D(Texture* _texture){
	++_texture->referenceCount;
	textures.push_back(_texture);
	texturesDirty = true;
}

unsigned long int MeshInterface::textureCount(){
	return textures.size();
}
 
Texture * MeshInterface::getTexture(int _idx){
	return textures.at(_idx);
}

void MeshInterface::pushMaterial(Material * _material){
	++_material->referenceCount;
	materials.push_back(_material);
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

	dirty = true;
}
void TriMesh::pushTri(GLuint _v0, GLuint _v1, GLuint _v2){
	indices.push_back(_v0);
	indices.push_back(_v1);
	indices.push_back(_v2);

	dirty = true;
}
void QuadMesh::pushQuad(GLuint _v0, GLuint _v1, GLuint _v2, GLuint _v3){
	indices.push_back(_v0);
	indices.push_back(_v1);
	indices.push_back(_v2);
	indices.push_back(_v3);

	dirty = true;
}