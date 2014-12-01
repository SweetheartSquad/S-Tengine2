#pragma once

#include "VoxelJoint.h"
#include "VoxelMesh.h"

VoxelJoint::VoxelJoint(unsigned long int _id, VoxelMesh * _mesh, Transform * _transform, Shader * _shader):
	Entity(_mesh, _transform, _shader),
	id(_id)
{
}

VoxelJoint::~VoxelJoint(){
}