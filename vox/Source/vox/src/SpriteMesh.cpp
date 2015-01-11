#pragma once

#include "SpriteMesh.h"
#include "SpriteSheetAnimation.h"

SpriteMesh::SpriteMesh(GLenum _drawMode) :
	MeshInterface(GL_QUADS, _drawMode),
	NodeRenderable(),
	animatedTexture(nullptr)
{
	//Top left
	pushVert(Vertex(-1.f, 1.f, 0.f));
	//top right
	pushVert(Vertex(1.f, 1.f, 0.f));
	//bottom right
	pushVert(Vertex(1.f, -1.f, 0.f));
	//bottom left
	pushVert(Vertex(-1.f, -1.f, 0.f));
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

SpriteMesh::~SpriteMesh(){
}