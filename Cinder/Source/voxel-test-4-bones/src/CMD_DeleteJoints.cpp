#pragma once

#include "CMD_DeleteJoints.h"
#include "CMD_SelectNodes.h"
#include "UI.h"
#include "Node.h"
#include "NodeHierarchical.h"
#include "NodeChild.h"
#include "Joint.h"

CMD_DeleteJoints::CMD_DeleteJoints(std::vector<NodeHierarchical *> * _joints) :
	joints(_joints)
{
}

void CMD_DeleteJoints::execute(){
	// Save selected joints
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
		if(j != nullptr){
			jointsForDeletion.push_back(j);
		}
	}
	index.resize(jointsForDeletion.size());
	children.resize(jointsForDeletion.size());

	for(unsigned long int j = 0; j < jointsForDeletion.size(); ++j){
		NodeHierarchical * jointForDeletion = dynamic_cast<NodeHierarchical *>(jointsForDeletion.at(j));
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
			
			// Add the joint to the list of deleted children
			deletedJoints.push_back(jointForDeletion);
		}else{
			// If the joint has children, delete it's children (but not the joint itself)
			children.at(j) = jointForDeletion->children;
			jointForDeletion->children.clear();

			// Add the children to the list of deleted joints
			for(unsigned long int child = 0; child < children.at(j).size(); ++child){
				deletedJoints.push_back(dynamic_cast<NodeHierarchical *>(children.at(j).at(child)));
			}
		}
	}

	// Unselect joints
	if(firstRun){
		subCmdProc.executeCommand(new CMD_SelectNodes(nullptr));
	}else{
		subCmdProc.redo();
	}
}

void CMD_DeleteJoints::unexecute(){
	// Should this go in reverse order? I think it does already
	for(unsigned long int j = jointsForDeletion.size(); j > 0; --j){
		NodeHierarchical * jointForDeletion = dynamic_cast<NodeHierarchical *>(jointsForDeletion.at(j-1));
		if(children.at(j-1).size() == 0){
			// If the joint had no children it was deleted, so restore it
			if(jointForDeletion->parent != nullptr){
				// If the joint had no parent, it goes back into the joints vector
				jointForDeletion->parent->children.insert(jointForDeletion->parent->children.begin() + index.at(j-1), jointForDeletion);
			}else{
				// If the joint had a parent, it goes back into the parent's children
				joints->insert(joints->begin() + index.at(j-1), jointForDeletion);
			}
		}else{
			// If the joint had children, add the children back to the joint
			jointForDeletion->children = children.at(j-1);
		}
	}
	// Remove the deleted joints from the list of deleted joints
	deletedJoints.clear();

	// Re-select old selection
	subCmdProc.undo();
}

CMD_DeleteJoints::~CMD_DeleteJoints(void){
	joints = nullptr;
	// The deleted joints need to be deleted for real
	for(unsigned long int i = 0; i < deletedJoints.size(); ++i){
		NodeHierarchical::deleteRecursively(dynamic_cast<NodeHierarchical *>(deletedJoints.at(i)));
	}
	deletedJoints.clear();
	jointsForDeletion.clear();
	index.clear();
	children.clear();
}