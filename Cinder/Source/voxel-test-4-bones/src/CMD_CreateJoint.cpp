#pragma once

#include "CMD_CreateJoint.h"
#include "CMD_SelectNodes.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"
#include "SceneRoot.h"

CMD_CreateJoint::CMD_CreateJoint(SceneRoot * _sceneRoot, ci::Vec3d _pos, Joint * _parent) :
	sceneRoot(_sceneRoot),
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
		sceneRoot->children.push_back(createdJoint);
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
		sceneRoot->children.pop_back();
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
