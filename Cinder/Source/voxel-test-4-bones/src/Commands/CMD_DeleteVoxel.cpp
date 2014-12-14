#pragma once

#include "Commands/CMD_DeleteVoxel.h"
#include "Commands/CMD_SelectNodes.h"
#include "UI.h"
#include "node/Node.h"
#include "node/NodeHierarchical.h"
#include "Joint.h"

#include "cinder/app/App.h"

CMD_DeleteVoxel::CMD_DeleteVoxel(Voxel * _v) :
	voxel(_v),
	parent(nullptr),
	index(-1)
{
}

bool CMD_DeleteVoxel::execute(){
	if(voxel != nullptr){
		ci::app::console() << "deleteVoxel.execute: " << voxel << std::endl;
		// voxel's parent
		parent = voxel->parent;
		if(parent != nullptr){
			index = parent->removeChild(voxel);
			if(index == (unsigned long int)(-1)){
				warn("Voxel was not a child of it's parent");
			}
		}else{
			warn("Voxel did not have a parent");
		}
		voxel->parent = nullptr;
	}else{
		warn("Cannot delete null voxel");
	}
	return true;
}

bool CMD_DeleteVoxel::unexecute(){
	if(voxel != nullptr){
		ci::app::console() << "deleteVoxel.unexecute: " << voxel << std::endl;
		voxel->parent = parent;

		// voxel's parent
		if(parent != nullptr){
			if(index != (unsigned long int)(-1)){
				parent->addChildAtIndex(voxel, index);
			}else{
				warn("Voxel was not a child of it's parent");
			}
		}else{
			error("Voxel did not have a parent");
		}
	}else{
		warn("Cannot delete null voxel");
	}
	return true;
}

CMD_DeleteVoxel::~CMD_DeleteVoxel(void){
	if(voxel != nullptr){
		if(executed){
			if(index != (unsigned long int)(-1)){
				ci::app::console() << "~deleteVoxel: " << voxel << std::endl;
				delete voxel;
				voxel = nullptr;
			}
		}
	}
}