#pragma once

#include <gl/glew.h>

class MeshInterface;

class MeshFactory{
public:
	static MeshInterface * getCubeMesh (GLenum _polygonalDrawMode = GL_QUADS, GLenum _drawMode = GL_STATIC_DRAW);
	static MeshInterface * getPlaneMesh(GLenum _polygonalDrawMode = GL_QUADS, GLenum _drawMode = GL_STATIC_DRAW);
};