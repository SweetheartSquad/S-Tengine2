#pragma once

#include "CMD_ScaleSelectedTransformable.h"

#include "UI.h"
#include "Node.h"
#include "NodeTransformable.h"
#include "Transform.h"

CMD_ScaleSelectedTransformable::CMD_ScaleSelectedTransformable(ci::Vec3d _v) :
	v(_v)
{
}

void CMD_ScaleSelectedTransformable::execute(){
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		NodeTransformable * j = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i));
		if(j != NULL){
			j->transform->scale(v.x, v.y, v.z);
		}
	}
}

void CMD_ScaleSelectedTransformable::unexecute(){
	for(unsigned long int i = UI::selectedNodes.size(); i > 0; -- i){
		NodeTransformable * j = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i-1));
		if(j != NULL){
			j->transform->scale(1/v.x, 1/v.y, 1/v.z);
		}
	}
}

CMD_ScaleSelectedTransformable::~CMD_ScaleSelectedTransformable(void){}
