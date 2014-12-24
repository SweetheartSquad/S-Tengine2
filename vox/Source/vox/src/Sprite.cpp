#pragma once 

#include "Sprite.h"
#include "Rectangle.h"
#include "Point.h"

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
	//Top left
	pushVert(Vertex(-1.f, 1.f, 1.f));
	//top right
	pushVert(Vertex(1.f, 1.f, 1.f));
	//bottom right
	pushVert(Vertex(1.f, -1.f, 1.f));
	//bottom left
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
	setUV(1, 0.0, 0.0);
	setUV(2, 0.0, 1.0);
	setUV(3, 1.0, 1.0);
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

void Sprite::setUvs(Rectangle _rect){
	mesh->vertices.at(3).u  = _rect.getBottomLeft().x;
	mesh->vertices.at(3).v  = _rect.getBottomLeft().y;
	mesh->vertices.at(0).u  = _rect.getTopLeft().x;
	mesh->vertices.at(0).v  = _rect.getTopLeft().y;
	mesh->vertices.at(1).u  = _rect.getTopRight().x;
	mesh->vertices.at(1).v  = _rect.getTopRight().y;
	mesh->vertices.at(2).u  = _rect.getBottomRight().x;
	mesh->vertices.at(2).v  = _rect.getBottomRight().y;
}

SpriteMesh::~SpriteMesh(){
}