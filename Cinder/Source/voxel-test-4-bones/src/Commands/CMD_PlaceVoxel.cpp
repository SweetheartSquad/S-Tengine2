#pragma once

#include "Commands/CMD_PlaceVoxel.h"

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
	if(parent == nullptr){
		error("No parent");
		return false;
	}
	if(firstRun){
		voxel = new Voxel(v, parent, true);
	}
	parent->addChild(voxel);
	return true;
}

bool CMD_PlaceVoxel::unexecute(){
	if(parent != nullptr){
		parent->removeChild(voxel);
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
