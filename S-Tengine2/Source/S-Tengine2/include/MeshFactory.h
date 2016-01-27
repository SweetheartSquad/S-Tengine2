#pragma once

#include <gl/glew.h>

class QuadMesh;

class MeshFactory{
public:
	static QuadMesh * getCubeMesh(float _halfSize = 0.5f, bool _autorelease = true);
	static QuadMesh * getPlaneMesh(float _halfSize = 0.5f, bool _autorelease = true);
	static QuadMesh * getPlaneMesh(float _halfWidth, float _halfHeight, bool _autorelease = true);
};