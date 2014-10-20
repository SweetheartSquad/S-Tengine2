#pragma once

#include "CMD_CreateJoint.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"

CMD_CreateJoint::CMD_CreateJoint(std::vector<Joint *> * _joints, ci::Vec3d _pos, Joint * _parent) :
	joints(_joints),
	pos(_pos),
	parent(_parent),
	createdJoint(nullptr)
{
}

void CMD_CreateJoint::execute(){
	oldSelection = UI::selectedNode;
	if(createdJoint == nullptr){
		createdJoint = new Joint(parent);
	}

	if(parent != nullptr){
		parent->children.push_back(createdJoint);
	}else{
		joints->push_back(createdJoint);
	}

	createdJoint->setPos(pos);
	UI::selectedNode = createdJoint;
	
}

void CMD_CreateJoint::unexecute(){
	if(createdJoint->parent != nullptr){
		// Remove created joint from list of children on parent
		for(unsigned long int i = 0; i < createdJoint->parent->children.size(); ++i){
			if(createdJoint->parent->children.at(i) == createdJoint){
				createdJoint->parent->children.erase(createdJoint->parent->children.begin() + i);
				break;
			}
		}
	}else{
		// Remove created joint from joint list
		joints->pop_back();
	}

	// Re-select old selection
	UI::selectedNode = oldSelection;
}

CMD_CreateJoint::~CMD_CreateJoint(void){
	delete createdJoint;
	joints = nullptr;
	parent = nullptr;
}
