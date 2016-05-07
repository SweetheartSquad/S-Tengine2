#pragma once

#include "MeshInterface.h"
#include "RenderOptions.h"
#include "Texture.h"
#include "Material.h"
#include "MatrixStack.h"
#include "VoxRenderOptions.h"
#include "Transform.h"
#include <Log.h>

#include <algorithm>

MeshInterface::MeshInterface(GLenum polygonalDrawMode, GLenum drawMode) :
	NodeResource(true),
	dirty(true),
	drawMode(drawMode),
	polygonalDrawMode(polygonalDrawMode),
	uvEdgeMode(GL_REPEAT),
	scaleModeMag(sweet::config.scaleModeMagDefault),
	scaleModeMin(sweet::config.scaleModeMinDefault)
{
	load();
	clean();
}

MeshInterface::~MeshInterface(){
	checkForGlError(false);
	if(vaoId != 0 && glIsVertexArray(vaoId) != GL_TRUE){
		Log::warn("Mesh VAO is invalid");
		return;
	}
	if(vboId != 0 && glIsBuffer(vboId) != GL_TRUE){
		Log::warn("Mesh VBO is invalid");
		return;
	}
	if(iboId != 0 && glIsBuffer(iboId) != GL_TRUE){
		Log::warn("Mesh IBO is invalid");
		return;
	}
	checkForGlError(false);
	glDeleteVertexArrays(1, &vaoId);
	checkForGlError(false);
	glDeleteBuffers(1, &vboId);
	checkForGlError(false);
	glDeleteBuffers(1, &iboId);
	checkForGlError(false);
	for(Texture * t : textures){
		t->decrementAndDelete();
	}
	for(Material * m : materials){
		m->decrementAndDelete();
	}
	vaoId = 0;
	vboId = 0;
	iboId = 0;
}

GLsizei MeshInterface::getStride(){
	return sizeof(Vertex);
}

GLsizei MeshInterface::getVertCount(){
	return vertices.size();
}

void MeshInterface::load(){
	if(!loaded){
		checkForGlError(false);
		
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

		checkForGlError(false);

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
		checkForGlError(false);

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
		checkForGlError(false);
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
		checkForGlError(false);
	}
}

void MeshInterface::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption){
	// don't bother doing any work if we aren't rendering anyway
	if(!isVisible()){
		return;
	}

	load();
	clean();
	
	if(glIsVertexArray(vaoId) != GL_TRUE){
		Log::warn("Mesh VAO is invalid");
		return;
	}
	if(glIsBuffer(vboId) != GL_TRUE){
		Log::warn("Mesh VBO is invalid");
		return;
	}
	if(glIsBuffer(iboId) != GL_TRUE){
		Log::warn("Mesh IBO is invalid");
		return;
	}
	if(_renderOption->shader == nullptr){
		Log::warn("Mesh has no shader");
		return;
	}
	//if(_renderOption->currentVao != vaoId){
		GLint prev = -1;
		_renderOption->currentVao = vaoId;
		// Bind VAO
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev);
		glBindVertexArray(vaoId);
		checkForGlError(false);
		//glBindBuffer(GL_ARRAY_BUFFER, vboId);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	//}
	//This should help performance but there's a bit of a problem with it at the moment so I'll comment it out
	//if(_renderOption->shader->getProgramId() != _renderOption->currentlyBoundShaderId) {
		_renderOption->currentlyBoundShaderId = _renderOption->shader->getProgramId();
		glUseProgram(_renderOption->shader->getProgramId());
	//}
	_renderOption->shader->clean(_matrixStack, _renderOption, this);	
	checkForGlError(false);
	// Texture repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uvEdgeMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, uvEdgeMode);
	
	// Texture scaling mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleModeMag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleModeMin);
	//}


	// Draw (note that the last argument is expecting a pointer to the indices, but since we have an ibo, it's actually interpreted as an offset)
	glDrawRangeElements(polygonalDrawMode, 0, indices.size(), indices.size(), GL_UNSIGNED_INT, 0);
	checkForGlError(false);

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
	indices.push_back(vertices.size());
	vertices.push_back(_vertex);
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

void MeshInterface::clearTextures(){
	while(textures.size() > 0){
		popTexture2D();
	}
}

void MeshInterface::replaceTextures(Texture * _newTexture){
	clearTextures();
	pushTexture2D(_newTexture);
}

void MeshInterface::removeTextureAt(int _idx){
	textures.at(_idx)->decrementAndDelete();
	textures.erase(textures.begin() + _idx);
}

void MeshInterface::pushTexture2D(Texture* _texture){
	_texture->incrementReferenceCount();
	textures.push_back(_texture);
}

unsigned long int MeshInterface::textureCount() const{
	return textures.size();
}
 
Texture * MeshInterface::getTexture(int _idx) const{
	return textures.at(_idx);
}

void MeshInterface::pushMaterial(Material * _material){
	_material->incrementReferenceCount();
	materials.push_back(_material);
}

void MeshInterface::setScaleMode(GLenum _both){
	scaleModeMag = scaleModeMin = _both;
}

void MeshInterface::setNormal(unsigned long int _vertId, float _x, float _y, float _z){
	vertices.at(_vertId).nx = _x;
	vertices.at(_vertId).ny = _y;
	vertices.at(_vertId).nz = _z;

	dirty = true;
}

glm::vec3 MeshInterface::calcNormal(unsigned long int _v1, unsigned long int _v2, unsigned long int _v3) const{
	const Vertex & v1 = vertices.at(_v1);
	const Vertex & v2 = vertices.at(_v2);
	const Vertex & v3 = vertices.at(_v3);
	glm::vec3 a(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	glm::vec3 b(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

	return glm::normalize(glm::cross(b, a));
}

glm::vec3 MeshInterface::setAutoNormal(unsigned long int _v1, unsigned long int _v2, unsigned long int _v3){
	glm::vec3 n(calcNormal(_v1, _v2, _v3));
	setNormal(_v1, n.x, n.y, n.z);
	setNormal(_v2, n.x, n.y, n.z);
	setNormal(_v3, n.x, n.y, n.z);
	return n;
}

void MeshInterface::setUV(unsigned long _vertId, float _u, float _v){
	vertices.at(_vertId).u = _u;
	vertices.at(_vertId).v = _v;

	dirty = true;
}

TriMesh::TriMesh(const QuadMesh * const _mesh, bool _autoRelease) :
	NodeResource(_autoRelease),
	MeshInterface(GL_TRIANGLES, GL_STATIC_DRAW)
{
	for(unsigned long int i = 0; i < _mesh->vertices.size(); i += 4){
		pushVert(_mesh->vertices.at(i));
		pushVert(_mesh->vertices.at(i+1));
		pushVert(_mesh->vertices.at(i+2));

		pushVert(_mesh->vertices.at(i+2));
		pushVert(_mesh->vertices.at(i+3));
		pushVert(_mesh->vertices.at(i));
	}
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

sweet::Box MeshInterface::calcBoundingBox() const{
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

	return sweet::Box(minX, minY, minZ, maxX - minX, maxY - minY, maxZ - minZ);
}

void MeshInterface::applyTransformation(Transform * _transform){
	const glm::mat4x4 m = _transform->getModelMatrix();
	for(Vertex & i : vertices){
		glm::vec3 v(m * glm::vec4(i.x, i.y, i.z, 1));
		i.x = v.x;
		i.y = v.y;
		i.z = v.z;
	}
	dirty = true;
}

void MeshInterface::insertVertices(const MeshInterface & _mesh){
	// save the number of vertices and indices so we can offset them later
	unsigned long int
		vertOffset = vertices.size(),
		indOffset = indices.size(),
		indNumTotal = indOffset + _mesh.indices.size();

	// copy the verts from the temporary mesh into this one
	vertices.insert(vertices.end(), _mesh.vertices.begin(), _mesh.vertices.end());
	indices.insert(indices.end(), _mesh.indices.begin(), _mesh.indices.end());
				
	// offset the inserted indices
	for(unsigned long int i = indOffset; i < indNumTotal; ++i) {
		indices.at(i) += vertOffset;
	}

	dirty = true;
}

std::ostream& operator<<(std::ostream& os, const MeshInterface& obj){
		os
			<< static_cast<const NodeRenderable&>(obj)<< std::endl
			<< ' ' << static_cast<const NodeLoadable&>(obj)<< std::endl
			<< ' ' << static_cast<const NodeResource&>(obj)<< std::endl
			<< " dirty: " << obj.dirty<< std::endl
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