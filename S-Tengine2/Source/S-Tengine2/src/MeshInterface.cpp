#pragma once

#include "MeshInterface.h"
#include "RenderOptions.h"
#include "Texture.h"
#include "Material.h"
#include "MatrixStack.h"
#include "VoxRenderOptions.h"
#include "Transform.h"
#include <algorithm>

MeshInterface::MeshInterface(GLenum polygonalDrawMode, GLenum drawMode) :
	NodeResource(true),
	dirty(true),
	texturesDirty(true),
	drawMode(drawMode),
	polygonalDrawMode(polygonalDrawMode),
	uvEdgeMode(GL_REPEAT),
	scaleModeMag(GL_LINEAR),
	scaleModeMin(GL_LINEAR_MIPMAP_LINEAR)
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
		
		GLint prev;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev);
		GLint prev2;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prev2);
		GLint prev3;
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &prev3);

		glBindVertexArray(0);

		// Vertex Array Object (VAO)
		glGenVertexArrays(1, &vaoId);
		// Vertex Buffer Object (VBO)
		glGenBuffers(1, &vboId);
		// Index Buffer Object (IBO)
		glGenBuffers(1, &iboId);
		
		checkForGlError(0,__FILE__,__LINE__);

		// Initialize textures
		for (Texture * texture : textures){
			texture->load();
		}
		glBindVertexArray(vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		// Disable VAO
		glBindVertexArray(prev);
		glBindBuffer(GL_ARRAY_BUFFER, prev2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prev3);
		checkForGlError(0,__FILE__,__LINE__);

		clean();
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
		GLint prev;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev);
		glBindVertexArray(0);
		// Vertex Buffer Object (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (vertices.size()), vertices.data(), drawMode);

		// Index Buffer Object (IBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * (indices.size()), indices.data(), drawMode);
		dirty = false;
		glBindVertexArray(prev);
		checkForGlError(0,__FILE__,__LINE__);
	}
}

void MeshInterface::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption){
	load();
	clean();
	
	if(glIsVertexArray(vaoId) != GL_TRUE){
		std::cout << "vao bad" << std::endl << std::endl;
		return;
	}
	if(glIsBuffer(vboId) != GL_TRUE){
		std::cout << "vbo bad" << std::endl << std::endl;
		return;
	}
	if(glIsBuffer(iboId) != GL_TRUE){
		std::cout << "ibo bad" << std::endl << std::endl;
		return;
	}
	if(_renderOption->shader == nullptr){
		std::cout << "no shader" << std::endl << std::endl;	
		return;
	}
	//if(_renderOption->currentVao != vaoId){
		GLint prev = -1;
		_renderOption->currentVao = vaoId;
		// Bind VAO
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev);
		glBindVertexArray(vaoId);
		checkForGlError(0,__FILE__,__LINE__);
		//glBindBuffer(GL_ARRAY_BUFFER, vboId);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	//}
	//This should help performance but there's a bit of a problem with it at the moment so I'll comment it out
	//if(_renderOption->shader->getProgramId() != _renderOption->currentlyBoundShaderId) {
		_renderOption->currentlyBoundShaderId = _renderOption->shader->getProgramId();
		glUseProgram(_renderOption->shader->getProgramId());
	//}
	_renderOption->shader->clean(_matrixStack, _renderOption, this);	

	// Alpha blending
	// Should these be here or only once in the main render loop?
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Texture repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uvEdgeMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, uvEdgeMode);
	
	// Texture scaling mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleModeMag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleModeMin);
	//}


	// Draw (note that the last argument is expecting a pointer to the indices, but since we have an ibo, it's actually interpreted as an offset)
	glDrawRangeElements(polygonalDrawMode, 0, indices.size(), indices.size(), GL_UNSIGNED_INT, 0);
	checkForGlError(0,__FILE__,__LINE__);

	//if(prev != -1){
		// Disable VAO
		glBindVertexArray(prev);
	//}
}


void MeshInterface::configureDefaultVertexAttributes(Shader *_shader){
	GLUtils::configureVertexAttributes(_shader->get_aVertexPosition(), 3, 0, vaoId, vboId, getStride());
	GLUtils::configureVertexAttributes(_shader->get_aVertexColor(), 4, sizeof(float) * 3, vaoId, vboId, getStride());
	GLUtils::configureVertexAttributes(_shader->get_aVertexNormals(), 3, sizeof(float) * 7, vaoId, vboId, getStride());
	GLUtils::configureVertexAttributes(_shader->get_aVertexUVs(), 2, sizeof(float) * 10, vaoId, vboId, getStride());
}

void MeshInterface::pushVert(Vertex _vertex){
	vertices.push_back(_vertex);
	indices.push_back(vertices.size() - 1);
	dirty = true;
}

Texture * MeshInterface::popTexture2D(){
	Texture * t = textures.back();
	if(t->decrementAndDelete()){
		t = nullptr;
	}
	textures.pop_back();
	return t;
}

void MeshInterface::removeTextureAt(int _idx){
	textures.at(_idx)->decrementAndDelete();
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

vox::Box MeshInterface::calcBoundingBox(){
	float minX = 99999, minY= 99999, minZ = 99999,
		maxX = -99999, maxY = -99999, maxZ = -99999;

	for(auto i : vertices){
		maxX = std::max(i.x, maxX);
		maxY = std::max(i.y, maxY);
		maxZ = std::max(i.z, maxZ);

		minX = std::min(i.x, minX);
		minY = std::min(i.y, minY);
		minZ = std::min(i.z, minZ);
	}

	return vox::Box(minX, minY, minZ, maxX - minX, maxY - minY, maxZ - minZ);
}

std::ostream& operator<<(std::ostream& os, const MeshInterface& obj){
		os
			<< static_cast<const NodeRenderable&>(obj)<< std::endl
			<< ' ' << static_cast<const NodeLoadable&>(obj)<< std::endl
			<< ' ' << static_cast<const NodeResource&>(obj)<< std::endl
			<< " dirty: " << obj.dirty<< std::endl
			<< " texturesDirty: " << obj.texturesDirty<< std::endl
			<< " uvEdgeMode: " << obj.uvEdgeMode<< std::endl
			<< " scaleModeMag: " << obj.scaleModeMag<< std::endl
			<< " scaleModeMin: " << obj.scaleModeMin<< std::endl
			<< " vaoId: " << obj.vaoId<< std::endl
			<< " vboId: " << obj.vboId<< std::endl
			<< " iboId: " << obj.iboId<< std::endl
			<< " drawMode: " << obj.drawMode<< std::endl
			<< " polygonalDrawMode: " << obj.polygonalDrawMode << std::endl
			<< " Vertices: " << std::endl;
		for(auto v : obj.vertices){
			os<<v<< std::endl;
		}

		os << "Textures: " << std::endl;
		for(auto t : obj.textures){
			os<<t<< std::endl;
		}

		os << "Materials: " << std::endl;
		for(auto m : obj.materials){
			os<<m << std::endl;
		}
		return os;
}