#pragma once

#include <gl/glew.h>

class MeshInterface;

class MeshFactory{
public:
	static MeshInterface * getCubeMesh(float _halfSize = 0.5f, GLenum _polygonalDrawMode = GL_QUADS, GLenum _drawMode = GL_STATIC_DRAW);
	static MeshInterface * getPlaneMesh(float _halfSize = 0.5f, GLenum _polygonalDrawMode = GL_QUADS, GLenum _drawMode = GL_STATIC_DRAW);
};