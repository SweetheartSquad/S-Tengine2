#pragma once

#include "VoxelJoint.h"
#include "VoxelMesh.h"

VoxelJoint::VoxelJoint(unsigned long int _id, VoxelMesh * _mesh, Shader * _shader):
	MeshEntity(_mesh, _shader),
	NodeRenderable(),
	id(_id)
{
	if(mesh != nullptr && getShader() != nullptr){
		load();
	}
}

VoxelJoint::~VoxelJoint(){
}