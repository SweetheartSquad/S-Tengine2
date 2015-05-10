#pragma once

#include "MeshEntity.h"

class VoxelMesh;

class VoxelJoint : public MeshEntity {

public:
	explicit VoxelJoint(unsigned long int _id, VoxelMesh * _mesh = nullptr, Shader * _shader = nullptr);
	~VoxelJoint();

	unsigned long int id;
	//std::vector<Voxel>;
}; 