#pragma once

#include "VoxelJoint.h"
#include "VoxelMesh.h"

VoxelJoint::VoxelJoint(unsigned long int _id, VoxelMesh * _mesh, Transform * _transform, Shader * _shader):
	NodeTransformable(_transform),
	MeshEntity(_mesh, _transform, _shader),
	NodeChild(nullptr),
	NodeRenderable(),
	id(_id)
{
	if(mesh != nullptr && getShader() != nullptr){
		load();
	}
}

VoxelJoint::~VoxelJoint(){
}