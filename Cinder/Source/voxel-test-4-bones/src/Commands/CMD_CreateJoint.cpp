#pragma once

#include "Commands/CMD_CreateJoint.h"
#include "Commands/CMD_SelectNodes.h"
#include "UI.h"
#include "node/Node.h"
#include "Joint.h"
#include "SceneRoot.h"

CMD_CreateJoint::CMD_CreateJoint(SceneRoot * _sceneRoot, ci::Vec3d _pos, Joint * _parent) :
	sceneRoot(_sceneRoot),
	pos(_pos),
	parent(_parent),
	createdJoint(nullptr)
{
}

bool CMD_CreateJoint::execute(){
	if(createdJoint == nullptr){
		createdJoint = new Joint();
	}

	if(parent != nullptr){
		parent->addChild(createdJoint);
		//log("Created joint as a child of joint");
	}else if(sceneRoot != nullptr){
		sceneRoot->addChild(createdJoint);
		//log("Created joint as a child of scene");
	}else{
		// Error: no appropriate parent for created joint found
		error("No appropriate parent for created joint found");
		return false;
	}

	createdJoint->setPos(glm::vec3(pos.x, pos.y, pos.z));

	// Select newly created joint
	if(firstRun){
		subCmdProc.executeCommand(new CMD_SelectNodes(createdJoint));
	}else{
		subCmdProc.redo();
	}
	return true;
}

bool CMD_CreateJoint::unexecute(){
	if(createdJoint->parent != nullptr){
		// Remove created joint from list of children on parent
		createdJoint->parent->children.pop_back();
	}else{
		error("Created joint has no parent");
		return false;
	}

	// Re-select old selection
	subCmdProc.undo();
	return true;
}

CMD_CreateJoint::~CMD_CreateJoint(void){
	if(!executed){
		delete createdJoint;
	}
	createdJoint = nullptr;
	parent = nullptr;
}
