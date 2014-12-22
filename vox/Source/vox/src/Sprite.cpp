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
	setUV(0, 0.0, 0.0);
	setUV(1, 0.0, 1.0);
	setUV(2, 1.0, 1.0);
	setUV(3, 1.0, 0.0);
}

SpriteMesh::~SpriteMesh(){
}