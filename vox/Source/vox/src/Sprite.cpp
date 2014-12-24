#pragma once 

#include "Sprite.h"


Sprite::Sprite(Shader * _shader, Transform * _transform):
	MeshEntity(new SpriteMesh(GL_STATIC_DRAW)),
	NodeTransformable(_transform),
	NodeChild(nullptr)
{
}

Sprite::~Sprite(){
}

SpriteMesh::SpriteMesh(GLenum _drawMode): 
	MeshInterface(GL_QUADS, _drawMode){
	pushVert(Vertex(-1.f, 1.f, 1.f));
	pushVert(Vertex(1.f, 1.f, 1.f));
	pushVert(Vertex(1.f, -1.f, 1.f));
	pushVert(Vertex(-1.f, -1.f, 1.f));
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	setNormal(0, 0.0, 0.0, 1.0);
	setNormal(1, 0.0, 0.0, 1.0);
	setNormal(2, 0.0, 0.0, 1.0);
	setNormal(3, 0.0, 0.0, 1.0);
	setUV(0, 1.0, 0.0);
	setUV(1, 1.0, 1.0);
	setUV(2, 0.0, 1.0);
	setUV(3, 0.0, 0.0);
}

Vertex * Sprite::getTopLeft(){
	return &mesh->vertices.at(0);
}

Vertex * Sprite::getTopRight(){
	return &mesh->vertices.at(1);
}

Vertex * Sprite::getBottomLeft(){
	return &mesh->vertices.at(3);
}

Vertex * Sprite::getBottomRight(){
	return &mesh->vertices.at(2);
}

void Sprite::setUvs(float _topLeftU, float _topLeftV, float _topRightU, float _topRightV, 
					float _bottomLeftU, float _bottomLeftV, float _bottomRightU, float _bottomRightV){
	getBottomLeft()->u    = _bottomLeftU;
	getBottomLeft()->v    = _bottomLeftV;
	getTopLeft()->u       = _topLeftU;
	getTopLeft()->v       = _topLeftV;
	getTopRight()->u      = _topRightU;
	getTopRight()->v      = _topRightV;
	getBottomRight()->u   = _bottomRightU;
	getBottomRight()->v   = _bottomRightV;
}

SpriteMesh::~SpriteMesh(){
}