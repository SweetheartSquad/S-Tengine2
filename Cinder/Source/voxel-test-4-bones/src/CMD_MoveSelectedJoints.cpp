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
					j->setPos(j->getPos(false) + v, true);
				}else{
					j->setPos(v);
				}
			}
		}
		break;
	case OBJECT:
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
					j->setPos(j->getPos(false) - v, true);
				}else{
					j->setPos(v, false);
				}
			}
		}
		break;
	case OBJECT:
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
		break;
	
	}
}

CMD_MoveSelectedJoints::~CMD_MoveSelectedJoints(void){}
