#pragma once

#include "Entity.h"

class VoxelJoint : public Entity {

public:
	explicit VoxelJoint(unsigned long int _id, MeshInterface * _mesh = nullptr, Transform * _transform = nullptr, Shader * _shader = nullptr);
	~VoxelJoint();

	unsigned long int id;
	//std::vector<Voxel>;
}; 