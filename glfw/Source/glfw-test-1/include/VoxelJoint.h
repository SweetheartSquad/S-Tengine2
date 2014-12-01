#pragma once

#include "Entity.h"

class VoxelMesh;

class VoxelJoint : public Entity {

public:
	explicit VoxelJoint(unsigned long int _id, VoxelMesh * _mesh = nullptr, Transform * _transform = nullptr, Shader * _shader = nullptr);
	~VoxelJoint();

	unsigned long int id;
	//std::vector<Voxel>;
}; 