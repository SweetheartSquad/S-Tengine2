#pragma once

#include "CMD_ScaleSelectedTransformable.h"

#include "UI.h"
#include "Node.h"
#include "NodeTransformable.h"

CMD_ScaleSelectedTransformable::CMD_ScaleSelectedTransformable(ci::Vec3f _v, CoordinateSpace _space) :
	v(_v),
	space(_space)
{
}

void CMD_ScaleSelectedTransformable::execute(){
	switch(space){
	case WORLD:

		break;
	case OBJECT:
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeTransformable * j = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i));
			if(j != NULL){
				j->transform->scale(v.x, v.y, v.z);
			}
		}
		break;
	}
}

void CMD_ScaleSelectedTransformable::unexecute(){
	switch(space){
	case WORLD:

		break;
	case OBJECT:
		for(unsigned long int i = UI::selectedNodes.size(); i > 0; -- i){
			NodeTransformable * j = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i-1));
			if(j != NULL){
				j->transform->scale(1.f/v.x, 1.f/v.y, 1.f/v.z);
			}
		}
		break;
	}
}

CMD_ScaleSelectedTransformable::~CMD_ScaleSelectedTransformable(void){}
