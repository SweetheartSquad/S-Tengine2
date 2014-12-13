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
	parent(nullptr),
	index(-1)
{
}

bool CMD_DeleteVoxel::execute(){
	if(voxel != nullptr){
		// voxel's parent
		parent = dynamic_cast<Joint *>(voxel->parent);
		if(parent != nullptr){
			for(unsigned long int i = 0; i < parent->voxels.size(); ++i){
				if(parent->voxels.at(i) == voxel){
					index = i;
					parent->voxels.erase(parent->voxels.begin() + index);
					break;
				}
			}
			if(index == (unsigned long int)(-1)){
				warn("Voxel was not a child of it's parent");
			}
		}else{
			error("Voxel's parent was not a joint");
			return false;
		}
		voxel->parent = nullptr;
	}else{
		error("Cannot delete null voxel");
		return false;
	}
}

bool CMD_DeleteVoxel::unexecute(){
	if(voxel != nullptr){
		voxel->parent = parent;

		// voxel's parent
		if(parent != nullptr){
			if(index != (unsigned long int)(-1)){
				parent->voxels.insert(parent->voxels.begin() + index, voxel);
			}else{
				warn("Voxel was not a child of it's parent");
			}
		}else{
			error("Voxel's parent was not a joint");
			return false;
		}
	}else{
		error("Cannot delete null voxel");
		return false;
	}
}

CMD_DeleteVoxel::~CMD_DeleteVoxel(void){
	if(voxel != nullptr){
		if(executed){
			if(index != (unsigned long int)(-1)){
				delete voxel;
				voxel = nullptr;
			}
		}
	}
}