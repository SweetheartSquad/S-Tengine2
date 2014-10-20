#pragma once

#include "CMD_DeleteJoint.h"
#include "CMD_SelectNode.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"

CMD_DeleteJoint::CMD_DeleteJoint(std::vector<Joint *> * _joints) :
	joints(_joints),
	index(0)
{
}

void CMD_DeleteJoint::execute(){
	// Save selected joint
	jointForDeletion = (Joint *)UI::selectedNode;

	if(jointForDeletion->children.size() == 0){
		// If the joint has no children, delete it
		if(jointForDeletion->parent != nullptr){
			// Remove created joint from list of children on parent
			for(unsigned long int i = 0; i < jointForDeletion->parent->children.size(); ++i){
				if(jointForDeletion->parent->children.at(i) == jointForDeletion){
					jointForDeletion->parent->children.erase(jointForDeletion->parent->children.begin() + i);
					index = i;
					break;
				}
			}
		}else{
			// Remove created joint from joint list
			for(unsigned long int i = 0; i < joints->size(); ++i){
				if(joints->at(i) == jointForDeletion){
					joints->erase(joints->begin() + i);
					index = i;
					break;
				}
			}
		}
		// Joint was deleted, so unselect it
		if(subCommands.size() == 0){
			subCommands.push_back(new CMD_SelectNode(nullptr));
		}
		subCommands.at(0)->execute();
	}else{
		// If the joint has children, delete it's children (but not the joint itself)
		children = jointForDeletion->children;
		jointForDeletion->children.clear();
	}
}

void CMD_DeleteJoint::unexecute(){
	if(children.size() == 0){
		// If the joint had no children it was deleted, so restore it
		if(jointForDeletion->parent != nullptr){
			// If the joint had no parent, it goes back into the joints vector
			jointForDeletion->parent->children.insert(jointForDeletion->parent->children.begin() + index, jointForDeletion);
		}else{
			// If the joint had a parent, it goes back into the parent's children
			joints->insert(joints->begin() + index, jointForDeletion);
		}
	}else{
		// If the joint had children, add the children back to the joint
		jointForDeletion->children = children;
	}

	if(subCommands.size() != 0){
		// Re-select old selection
		subCommands.at(0)->unexecute();
	}
}

CMD_DeleteJoint::~CMD_DeleteJoint(void){
}