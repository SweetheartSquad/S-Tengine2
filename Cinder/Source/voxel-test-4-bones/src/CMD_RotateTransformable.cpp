#pragma once

#include "CMD_RotateTransformable.h"
#include "NodeTransformable.h"

CMD_RotateTransformable::CMD_RotateTransformable(NodeTransformable * _node, glm::quat _rotation, bool _relative, CoordinateSpace _space) :
	node(_node),
	rotation(_rotation),
	relative(_relative),
	space(_space)
{
}

void CMD_RotateTransformable::execute(){
	if(node != nullptr){
		if(firstRun){
			oldOrientation = node->transform->orientation;
		}

		if(relative){
			node->transform->rotate(rotation, space);
		}else{
			node->transform->orientation = rotation;
		}
	}else{
		// Error: no node provided
	}
}

void CMD_RotateTransformable::unexecute(){
	if(node != nullptr){
		node->transform->orientation = oldOrientation;
	}else{
		// Error: no node
	}
}

CMD_RotateTransformable::~CMD_RotateTransformable(void){}
