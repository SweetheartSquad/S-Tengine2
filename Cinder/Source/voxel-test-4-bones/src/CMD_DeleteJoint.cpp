#pragma once

#include "CMD_DeleteJoint.h"
#include "CMD_SelectNodes.h"
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
	jointsForDeletion = UI::selectedNodes;
	index.resize(jointsForDeletion.size());
	children.resize(jointsForDeletion.size());

	for(unsigned long int j = 0; j < jointsForDeletion.size(); ++j){
		Joint * jointForDeletion = (Joint *)jointsForDeletion.at(j);
		if(jointForDeletion->children.size() == 0){
			// If the joint has no children, delete it
			if(jointForDeletion->parent != nullptr){
				// Remove created joint from list of children on parent
				for(unsigned long int i = 0; i < jointForDeletion->parent->children.size(); ++i){
					if(jointForDeletion->parent->children.at(i) == jointForDeletion){
						jointForDeletion->parent->children.erase(jointForDeletion->parent->children.begin() + i);
						index.at(j) = i;
						break;
					}
				}
			}else{
				// Remove created joint from joint list
				for(unsigned long int i = 0; i < joints->size(); ++i){
					if(joints->at(i) == jointForDeletion){
						joints->erase(joints->begin() + i);
						index.at(j) = i;
						break;
					}
				}
			}
			// Joint was deleted, so unselect it
			if(subCommands.size() == 0){
				subCommands.push_back(new CMD_SelectNodes(nullptr));
			}
			subCommands.at(0)->execute();
		}else{
			// If the joint has children, delete it's children (but not the joint itself)
			children.at(j) = jointForDeletion->children;
			jointForDeletion->children.clear();
		}
	}
}

void CMD_DeleteJoint::unexecute(){
	// Should this go in reverse order?
	for(unsigned long int j = 0; j < jointsForDeletion.size(); ++j){
		Joint * jointForDeletion = (Joint *)jointsForDeletion.at(j);
		if(children.at(j).size() == 0){
			// If the joint had no children it was deleted, so restore it
			if(jointForDeletion->parent != nullptr){
				// If the joint had no parent, it goes back into the joints vector
				jointForDeletion->parent->children.insert(jointForDeletion->parent->children.begin() + index.at(j), jointForDeletion);
			}else{
				// If the joint had a parent, it goes back into the parent's children
				joints->insert(joints->begin() + index.at(j), jointForDeletion);
			}
		}else{
			// If the joint had children, add the children back to the joint
			jointForDeletion->children = children.at(j);
		}

		if(subCommands.size() != 0){
			// Re-select old selection
			subCommands.at(0)->unexecute();
		}
	}
}

CMD_DeleteJoint::~CMD_DeleteJoint(void){
}