#pragma once

#include <gl/glew.h>

class QuadMesh;

class MeshFactory{
public:
	// creates a cube
	// origin is center of mesh
	static QuadMesh * getCubeMesh(float _halfSize = 0.5f, bool _autorelease = true);

	// creates a plane in the XY plane, facing down Z
	// origin is center of mesh
	static QuadMesh * getPlaneMesh(float _halfSize = 0.5f, bool _autorelease = true);
	// creates a plane in the XY plane, facing down Z
	// origin is center of mesh
	static QuadMesh * getPlaneMesh(float _halfWidth, float _halfHeight, bool _autorelease = true);
};