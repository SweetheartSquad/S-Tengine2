#pragma once 

#include "MeshInterface.h"

class VoxelMesh : public MeshInterface{

public:
	float resolution;

	explicit VoxelMesh(GLenum _drawMode);
	explicit VoxelMesh(MeshInterface *_mesh);
	~VoxelMesh();
	
	void pushPoint(GLint _vertex);
};