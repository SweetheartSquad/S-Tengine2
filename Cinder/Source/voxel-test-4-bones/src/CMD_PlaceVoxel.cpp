#pragma once

#include "CMD_PlaceVoxel.h"

#include <vector>

#include <glm\glm.hpp>

#include "UI.h"
#include "Joint.h"
#include "Transform.h"
#include "Voxel.h"

CMD_PlaceVoxel::CMD_PlaceVoxel(ci::Vec3f _v, Joint * _parent) :
	v(_v),
	parent(_parent),
	voxel(nullptr)
{
}

bool CMD_PlaceVoxel::execute(){
	glm::vec4 newPos(v.x, v.y, v.z, 1);
	NodeHierarchical * _parent = parent;
	std::vector<glm::mat4> modelMatrixStack;
	while(_parent != nullptr){
		modelMatrixStack.push_back(dynamic_cast<NodeTransformable *>(_parent)->transform->getModelMatrix());
		_parent = dynamic_cast<NodeHierarchical *>(_parent->parent);
	}

	glm::mat4 modelMatrix(1);
	for(unsigned long int i = modelMatrixStack.size(); i > 0; --i){
		modelMatrix = modelMatrix * modelMatrixStack.at(i-1);
	}
	newPos = glm::inverse(modelMatrix) * newPos;

	if(firstRun){
		voxel = new Voxel(v, parent, true);
	}
	if(parent != nullptr){
		parent->voxels.push_back(voxel);
	}
	return true;
}

bool CMD_PlaceVoxel::unexecute(){
	if(parent != nullptr){
		parent->voxels.pop_back();
	}
	voxel->parent = nullptr;
	
	return true;
}

CMD_PlaceVoxel::~CMD_PlaceVoxel(void){
	if(!executed){
		delete voxel;
		voxel = nullptr;
	}
}
