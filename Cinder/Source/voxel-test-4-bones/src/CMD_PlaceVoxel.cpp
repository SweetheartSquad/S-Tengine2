#pragma once

#include "CMD_PlaceVoxel.h"

#include <vector>

#include <glm\glm.hpp>

#include "UI.h"
#include "Joint.h"
#include "Transform.h"
#include "Voxel.h"

CMD_PlaceVoxel::CMD_PlaceVoxel(ci::Vec3f _v) :
	v(_v),
	voxel(nullptr),
	executed(false)
{
}

void CMD_PlaceVoxel::execute(){
	Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(0));

	glm::vec4 newPos(v.x, v.y, v.z, 1);
	NodeHierarchical * _parent = j;
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

	if(voxel == nullptr){
		voxel = new Voxel(Vec3f(newPos.x, newPos.y, newPos.z));
	}
	voxel->parent = j;
	j->voxels.push_back(voxel);
	executed = true;
}

void CMD_PlaceVoxel::unexecute(){
	Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(0));
	j->voxels.pop_back();
	voxel->parent = nullptr;
	executed = false;
}

CMD_PlaceVoxel::~CMD_PlaceVoxel(void){
	if(!executed){
		delete voxel;
	}
}
