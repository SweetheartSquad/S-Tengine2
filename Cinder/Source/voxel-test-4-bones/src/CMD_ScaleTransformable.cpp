#pragma once

#include "CMD_ScaleTransformable.h"

#include "NodeTransformable.h"

CMD_ScaleTransformable::CMD_ScaleTransformable(NodeTransformable * _node, ci::Vec3f _scale, bool _relative, CoordinateSpace _space) :
	node(_node),
	scale(_scale),
	relative(_relative),
	space(_space)
{
}

bool CMD_ScaleTransformable::execute(){
	if(space == kWORLD){
		warn("World-space rotation not implemented; command aborted");
		return false;
	}

	if(node != nullptr){
		if(firstRun){
			oldScale = node->transform->scaleVector;
		}
		
		if(relative){
			glm::vec4 v2(scale.x, scale.y, scale.z, 0);
			NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(node);
			switch(space){
			case kWORLD:

				break;

			case kOBJECT:
				node->transform->scale(scale.x, scale.y, scale.z);
				break;
			}
		}else{
			node->transform->scaleVector = glm::vec3(scale.x, scale.y, scale.z);
		}
	}else{
		error("Node is null");
		return false;
	}
	return true;
}

bool CMD_ScaleTransformable::unexecute(){
	if(node != nullptr){
		node->transform->scaleVector = oldScale;
	}else{
		error("Node is null");
		return false;
	}
	return true;
}

CMD_ScaleTransformable::~CMD_ScaleTransformable(void){}
