#pragma once

#include "CMD_RotateSelectedJoints.h"
#include "UI.h"
#include "Transform.h"
#include "NodeTransformable.h"
#include "Joint.h"

CMD_RotateSelectedJoints::CMD_RotateSelectedJoints(glm::quat _rotation, bool _relative) :
	rotation(_rotation),
	relative(_relative)
{
}

void CMD_RotateSelectedJoints::execute(){
	if(relative){
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i))->transform->rotate(rotation);
		}
	}else{
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			if(orientations.size() != UI::selectedNodes.size()){
				orientations.push_back(dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i))->transform->orientation);
			}
			dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i))->transform->orientation = rotation;
		}
	}
}

void CMD_RotateSelectedJoints::unexecute(){
	if(relative){
		for(unsigned long int i = UI::selectedNodes.size(); i > 0; -- i){
			dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i-1))->transform->rotate(glm::inverse(rotation));
		}
	}else{
		for(unsigned long int i = UI::selectedNodes.size(); i > 0; -- i){
			dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i-1))->transform->orientation = orientations.at(i-1);
		}
	}
}

CMD_RotateSelectedJoints::~CMD_RotateSelectedJoints(void){}
