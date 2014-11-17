#pragma once

#include "CMD_RotateSelectedTransformable.h"
#include "UI.h"
#include "Transform.h"
#include "NodeTransformable.h"
#include "Joint.h"

CMD_RotateSelectedTransformable::CMD_RotateSelectedTransformable(glm::quat _rotation, bool _relative) :
	rotation(_rotation),
	relative(_relative)
{
}

void CMD_RotateSelectedTransformable::execute(){
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		NodeTransformable * j = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i));
		if(j != NULL){
			if(relative){
				j->transform->rotate(rotation);
			}else{
				if(orientations.size() != UI::selectedNodes.size()){
					orientations.push_back(j->transform->orientation);
				}
				j->transform->orientation = rotation;
			}
		}
	}
}

void CMD_RotateSelectedTransformable::unexecute(){
	for(unsigned long int i = UI::selectedNodes.size(); i > 0; -- i){
		NodeTransformable * j = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i-1));
		if(relative){
			j->transform->rotate(glm::inverse(rotation));
		}else{
			j->transform->orientation = orientations.at(i-1);
		}
	}
}

CMD_RotateSelectedTransformable::~CMD_RotateSelectedTransformable(void){}
