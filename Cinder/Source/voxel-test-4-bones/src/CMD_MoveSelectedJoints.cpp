#pragma once

#include "CMD_MoveSelectedJoints.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"

CMD_MoveSelectedJoints::CMD_MoveSelectedJoints(ci::Vec3d _v, bool _relative, CoordinateSpace _space) :
	v(_v),
	relative(_relative),
	space(_space)
{
}

void CMD_MoveSelectedJoints::execute(){
	switch(space){
	case WORLD:
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
			if(j != NULL){
				if(relative){
					j->setPos(j->getPos(false) + glm::vec3(v.x, v.y, v.z), true);
				}else{
					j->setPos(glm::vec3(v.x, v.y, v.z));
				}
			}
		}
		break;
	case OBJECT:
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
			if(j != NULL){
				if(relative){
					j->setPos(j->getPos(true) + glm::vec3(v.x, v.y, v.z), false);
				}else{
					j->setPos(glm::vec3(v.x, v.y, v.z));
				}
			}
		}
		break;
	}
}

void CMD_MoveSelectedJoints::unexecute(){
	switch(space){
	case WORLD:
		for(unsigned long int i = UI::selectedNodes.size(); i > 0; --i){
			Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i-1));
			if(j != NULL){
				if(relative){
					j->setPos(j->getPos(false) - glm::vec3(v.x, v.y, v.z), true);
				}else{
					j->setPos(glm::vec3(v.x, v.y, v.z), false);
				}
			}
		}
		break;
	case OBJECT:
		for(unsigned long int i = UI::selectedNodes.size(); i > 0; --i){
			Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i-1));
			if(j != NULL){
				if(relative){
					j->setPos(j->getPos(true) - glm::vec3(v.x, v.y, v.z), false);
				}else{
					j->setPos(glm::vec3(v.x, v.y, v.z), false);
				}
			}
		}
		break;
	}
}

CMD_MoveSelectedJoints::~CMD_MoveSelectedJoints(void){}
