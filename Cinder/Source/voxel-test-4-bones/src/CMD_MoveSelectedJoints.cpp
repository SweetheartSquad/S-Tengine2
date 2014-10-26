#pragma once

#include "CMD_MoveSelectedJoints.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"

CMD_MoveSelectedJoints::CMD_MoveSelectedJoints(ci::Vec3d _v, bool _relative) :
	v(_v),
	relative(_relative)
{
}

void CMD_MoveSelectedJoints::execute(){
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		if(relative){
			dynamic_cast<Joint *>(UI::selectedNodes.at(i))->setPos(dynamic_cast<Joint *>(UI::selectedNodes.at(i))->getPos(true) + v, false);
		}else{
			dynamic_cast<Joint *>(UI::selectedNodes.at(i))->setPos(v);
		}
	}
}

void CMD_MoveSelectedJoints::unexecute(){
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		if(relative){
			dynamic_cast<Joint *>(UI::selectedNodes.at(i))->setPos(dynamic_cast<Joint *>(UI::selectedNodes.at(i))->getPos(true) - v, false);
		}else{
			dynamic_cast<Joint *>(UI::selectedNodes.at(i))->setPos(v, false);
		}
	}
}

CMD_MoveSelectedJoints::~CMD_MoveSelectedJoints(void){}
