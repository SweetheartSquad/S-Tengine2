#pragma once

#include "CMD_MoveSelectedJoints.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"

CMD_MoveSelectedJoints::CMD_MoveSelectedJoints(ci::Vec3d _v, bool _relative, bool _local) :
	v(_v),
	relative(_relative),
	local(_local)
{
}

void CMD_MoveSelectedJoints::execute(){
	if(local){
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
			if(j != NULL){
				if(relative){
					j->setPos(j->getPos(true) + v, false);
				}else{
					j->setPos(v);
				}
			}
		}
	}else{
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
			if(j != NULL){
				if(relative){
					j->setPos(j->getPos(false) + v, true);
				}else{
					j->setPos(v);
				}
			}
		}
	}
}

void CMD_MoveSelectedJoints::unexecute(){
	if(local){
		for(unsigned long int i = UI::selectedNodes.size(); i > 0; --i){
			Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i-1));
			if(j != NULL){
				if(relative){
					j->setPos(j->getPos(true) - v, false);
				}else{
					j->setPos(v, false);
				}
			}
		}
	}else{
		for(unsigned long int i = UI::selectedNodes.size(); i > 0; --i){
			Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i-1));
			if(j != NULL){
				if(relative){
					j->setPos(j->getPos(false) - v, true);
				}else{
					j->setPos(v, false);
				}
			}
		}
	}
}

CMD_MoveSelectedJoints::~CMD_MoveSelectedJoints(void){}
