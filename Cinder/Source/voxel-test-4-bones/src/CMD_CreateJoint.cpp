#pragma once

#include "CMD_CreateJoint.h"
#include "CMD_SelectNodes.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"

CMD_CreateJoint::CMD_CreateJoint(std::vector<Joint *> * _joints, ci::Vec3d _pos, Joint * _parent) :
	joints(_joints),
	pos(_pos),
	parent(_parent),
	createdJoint(nullptr),
	executed(false)
{
}

void CMD_CreateJoint::execute(){
	if(createdJoint == nullptr){
		createdJoint = new Joint(parent);
	}

	if(parent != nullptr){
		parent->children.push_back(createdJoint);
	}else{
		joints->push_back(createdJoint);
	}

	createdJoint->setPos(glm::vec3(pos.x, pos.y, pos.z));

	// Select newly created joint
	if(subCommands.size() == 0){
		subCommands.push_back(new CMD_SelectNodes(createdJoint));
	}
	subCommands.at(0)->execute();

	executed = true;
}

void CMD_CreateJoint::unexecute(){
	if(createdJoint->parent != nullptr){
		// Remove created joint from list of children on parent
		createdJoint->parent->children.pop_back();
	}else{
		// Remove created joint from joint list
		joints->pop_back();
	}

	// Re-select old selection
	subCommands.at(0)->unexecute();

	executed = false;
}

CMD_CreateJoint::~CMD_CreateJoint(void){
	joints = nullptr;
	if(!executed){
		delete createdJoint;
	}
	createdJoint = nullptr;
	joints = nullptr;
	parent = nullptr;
}
