#pragma once

#include "CMD_DeleteJoints.h"
#include "CMD_DeleteJoint.h"
#include "CMD_SelectNodes.h"
#include "UI.h"
#include "Node.h"
#include "NodeHierarchical.h"
#include "NodeChild.h"
#include "Joint.h"

CMD_DeleteJoints::CMD_DeleteJoints(){
}

bool CMD_DeleteJoints::execute(){
	// The joints which this command is in reference to
	std::vector<Joint *> jointsForDeletion;

	// Save selected joints
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		Joint * candidate = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
		if(candidate != nullptr){
			bool add = true;
			for(unsigned long int j = 0; j < jointsForDeletion.size(); ++j){
				if(jointsForDeletion.at(j)->hasChild(candidate)){
					add = false;
					break;
				}else if(candidate->hasChild(jointsForDeletion.at(j))){
					jointsForDeletion.erase(jointsForDeletion.begin() + j);
				}
			}
			if(add){
				jointsForDeletion.push_back(candidate);
			}
		}
	}

	if(firstRun){
		for(unsigned long int i = 0; i < jointsForDeletion.size(); ++i){
			subCmdProc.executeCommand(new CMD_DeleteJoint(jointsForDeletion.at(i)));
		}
		// Unselect joints
		subCmdProc.executeCommand(new CMD_SelectNodes(nullptr));
		/*for(unsigned long int i = 0; i < jointsForDeletion.size(); ++i){
			subCmdProc.executeCommand(new CMD_SelectNodes(jointsForDeletion.at(i), true, false));
		}*/
	}else{
		subCmdProc.redoAll();
	}
	return true;
}

bool CMD_DeleteJoints::unexecute(){
	// Re-select old selection
	subCmdProc.undoAll();
	return true;
}

CMD_DeleteJoints::~CMD_DeleteJoints(void){
}