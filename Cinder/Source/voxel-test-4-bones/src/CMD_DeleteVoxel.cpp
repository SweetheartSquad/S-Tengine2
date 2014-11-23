#pragma once

#include "CMD_DeleteVoxel.h"
#include "CMD_SelectNodes.h"
#include "UI.h"
#include "Node.h"
#include "NodeHierarchical.h"
#include "Joint.h"

CMD_DeleteVoxel::CMD_DeleteVoxel(Voxel * _v) :
	voxel(_v),
	executed(false)
{
}

void CMD_DeleteVoxel::execute(){
	
	// voxel's parent
	Joint * parent = dynamic_cast<Joint *>(voxel->parent);
	
	for(unsigned long int i = 0; i < parent->voxels.size(); ++i){
		if(parent->voxels.at(i) == voxel){
			index = i;
			parent->voxels.erase(parent->voxels.begin() + i);
			break;
		}
	}

	executed = true;
}

void CMD_DeleteVoxel::unexecute(){
	// voxel's parent
	Joint * parent = dynamic_cast<Joint *>(voxel->parent);
	parent->voxels.insert(parent->voxels.begin() + index, voxel);

	executed = false;
}

CMD_DeleteVoxel::~CMD_DeleteVoxel(void){
	if(executed){
		delete voxel;
	}
}