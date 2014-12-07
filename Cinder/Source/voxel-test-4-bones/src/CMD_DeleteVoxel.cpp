#pragma once

#include "CMD_DeleteVoxel.h"
#include "CMD_SelectNodes.h"
#include "UI.h"
#include "Node.h"
#include "NodeHierarchical.h"
#include "Joint.h"

#include "cinder/app/App.h"

CMD_DeleteVoxel::CMD_DeleteVoxel(Voxel * _v) :
	voxel(_v),
	executed(false),
	index(-1)
{
}

void CMD_DeleteVoxel::execute(){
	
	// voxel's parent
	Joint * parent = dynamic_cast<Joint *>(voxel->parent);
	if(parent != nullptr){
		for(unsigned long int i = 0; i < parent->voxels.size(); ++i){
			if(parent->voxels.at(i) == voxel){
				index = i;
				parent->voxels.erase(parent->voxels.begin() + i);
				break;
			}
		}
	}else{
		// Error: Parent of voxel is not a joint
	}

	executed = true;
}

void CMD_DeleteVoxel::unexecute(){
	// voxel's parent
	Joint * parent = dynamic_cast<Joint *>(voxel->parent);
	if(parent != nullptr){
		if(index != (unsigned long int)(-1)){
			parent->voxels.insert(parent->voxels.begin() + index, voxel);
		}
	}else{
		// Error: Parent of voxel is not a joint
	}

	executed = false;
}

CMD_DeleteVoxel::~CMD_DeleteVoxel(void){
	if(executed){
		if(index != (unsigned long int)(-1)){
			delete voxel;
		}
	}
}