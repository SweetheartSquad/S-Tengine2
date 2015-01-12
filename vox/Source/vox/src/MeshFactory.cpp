#pragma once

#include "MeshFactory.h"
#include "Vertex.h"
#include "MeshInterface.h"

MeshInterface* MeshFactory::getCubeMesh(GLenum _polygonalDrawMode, GLenum _drawMode){	
	QuadMesh * m = new QuadMesh(_polygonalDrawMode, _drawMode);
	//Top
	m->pushVert(Vertex(-1.f, 1.f, 1.f));
	m->pushVert(Vertex(-1.f, 1.f, -1.f));
	m->pushVert(Vertex(1.f, 1.f, -1.f));
	m->pushVert(Vertex(1.f, 1.f, 1.f));
	m->pushQuad(0, 1, 2, 3);
	m->setNormal(0, 0.0, 1.0, 0.0);
	m->setNormal(1, 0.0, 1.0, 0.0);
	m->setNormal(2, 0.0, 1.0, 0.0);
	m->setNormal(3, 0.0, 1.0, 0.0);
	m->setUV(0, 0.0, 0.0);
	m->setUV(1, 0.0, 1.0);
	m->setUV(2, 1.0, 1.0);
	m->setUV(3, 1.0, 0.0);

	//Bottom
	m->pushVert(Vertex(-1.f, -1.f, 1.f));
	m->pushVert(Vertex(1.f, -1.f, 1.f));
	m->pushVert(Vertex(1.f, -1.f, -1.f));
	m->pushVert(Vertex(-1.f, -1.f, -1.f));
	m->pushQuad(4, 5, 6, 7);
	m->setNormal(4, 0.0, -1.0, 0.0);
	m->setNormal(5, 0.0, -1.0, 0.0);
	m->setNormal(6, 0.0, -1.0, 0.0);
	m->setNormal(7, 0.0, -1.0, 0.0);
	m->setUV(4, 0.0, 0.0);
	m->setUV(5, 0.0, 1.0);
	m->setUV(6, 1.0, 1.0);
	m->setUV(7, 1.0, 0.0);

	//Front
	m->pushVert(Vertex(-1.f, 1.f, 1.f));
	m->pushVert(Vertex(1.f, 1.f, 1.f));
	m->pushVert(Vertex(1.f, -1.f, 1.f));
	m->pushVert(Vertex(-1.f, -1.f, 1.f));
	m->pushQuad(8, 9, 10, 11);
	m->setNormal(8, 0.0, 0.0, 1.0);
	m->setNormal(9, 0.0, 0.0, 1.0);
	m->setNormal(10, 0.0, 0.0, 1.0);
	m->setNormal(11, 0.0, 0.0, 1.0);
	m->setUV(8, 0.0, 0.0);
	m->setUV(9, 0.0, 1.0);
	m->setUV(10, 1.0, 1.0);
	m->setUV(11, 1.0, 0.0);

	//Back
	m->pushVert(Vertex(1.f, 1.f, -1.f));
	m->pushVert(Vertex(-1.f, 1.f, -1.f));
	m->pushVert(Vertex(-1.f, -1.f, -1.f));
	m->pushVert(Vertex(1.f, -1.f, -1.f));
	m->pushQuad(12,13,14,15);
	m->setNormal(12, 0.0, 0.0, -1.0);
	m->setNormal(13, 0.0, 0.0, -1.0);
	m->setNormal(14, 0.0, 0.0, -1.0);
	m->setNormal(15, 0.0, 0.0, -1.0);
	m->setUV(12, 0.0, 0.0);
	m->setUV(13, 0.0, 1.0);
	m->setUV(14, 1.0, 1.0);
	m->setUV(15, 1.0, 0.0);

	//Left
	m->pushVert(Vertex(-1.f, 1.f, -1.f));
	m->pushVert(Vertex(-1.f, 1.f, 1.f));
	m->pushVert(Vertex(-1.f, -1.f, 1.f));
	m->pushVert(Vertex(-1.f, -1.f, -1.f));
	m->pushQuad(16,17,18,19);
	m->setNormal(16, -1.0, 0.0, 0.0);
	m->setNormal(17, -1.0, 0.0, 0.0);
	m->setNormal(18, -1.0, 0.0, 0.0);
	m->setNormal(19, -1.0, 0.0, 0.0);
	m->setUV(16, 0.0, 0.0);
	m->setUV(17, 0.0, 1.0);
	m->setUV(18, 1.0, 1.0);
	m->setUV(19, 1.0, 0.0);

	//Right
	m->pushVert(Vertex(1.f, 1.f, 1.f));
	m->pushVert(Vertex(1.f, 1.f, -1.f));
	m->pushVert(Vertex(1.f, -1.f, -1.f));
	m->pushVert(Vertex(1.f, -1.f, 1.f));
	m->pushQuad(20, 21, 22, 23);
	m->setNormal(20, 1.0, 0.0, 0.0);
	m->setNormal(21, 1.0, 0.0, 0.0);
	m->setNormal(22, 1.0, 0.0, 0.0);
	m->setNormal(23, 1.0, 0.0, 0.0);
	m->setUV(20, 0.0, 0.0);
	m->setUV(21, 0.0, 1.0);
	m->setUV(22, 1.0, 1.0);
	m->setUV(23, 1.0, 0.0);
	return m;
}

MeshInterface* MeshFactory::getPlaneMesh(GLenum _polygonalDrawMode, GLenum _drawMode){
	QuadMesh * m = new QuadMesh(_polygonalDrawMode, _drawMode);
	m->pushVert(Vertex(-1.f, 1.f, 1.f));
	m->pushVert(Vertex(1.f, 1.f, 1.f));
	m->pushVert(Vertex(1.f, -1.f, 1.f));
	m->pushVert(Vertex(-1.f, -1.f, 1.f));
	m->pushQuad(0, 1, 2, 3);
	m->setNormal(0, 0.0, 0.0, 1.0);
	m->setNormal(1, 0.0, 0.0, 1.0);
	m->setNormal(2, 0.0, 0.0, 1.0);
	m->setNormal(3, 0.0, 0.0, 1.0);
	m->setUV(0, 0.0, 0.0);
	m->setUV(1, 0.0, 1.0);
	m->setUV(2, 1.0, 1.0);
	m->setUV(3, 1.0, 0.0);
	return m;
}