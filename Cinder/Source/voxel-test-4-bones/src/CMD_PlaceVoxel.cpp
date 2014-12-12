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

void CMD_PlaceVoxel::execute(){
	if(firstRun){
		voxel = new Voxel(v, parent, true);
	}
	if(parent != nullptr){
		parent->voxels.push_back(voxel);
	}
}

void CMD_PlaceVoxel::unexecute(){
	if(parent != nullptr){
		parent->voxels.pop_back();
	}
	voxel->parent = nullptr;
	
}

CMD_PlaceVoxel::~CMD_PlaceVoxel(void){
	if(!executed){
		delete voxel;
		voxel = nullptr;
	}
}
