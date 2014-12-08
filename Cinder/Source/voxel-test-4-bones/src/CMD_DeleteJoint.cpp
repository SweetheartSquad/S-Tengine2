#pragma once

#include "CMD_DeleteJoint.h"
#include "Joint.h"

CMD_DeleteJoint::CMD_DeleteJoint(Joint * _jointForDeletion) :
	jointForDeletion(_jointForDeletion),
	index(-1)
{
}

void CMD_DeleteJoint::execute(){
	if(jointForDeletion != nullptr){
		if(jointForDeletion->children.size() > 0){
			children = jointForDeletion->children;
			jointForDeletion->children.clear();
		}else{
			index = jointForDeletion->parent->removeChild(jointForDeletion);
		}
	}else{
		// Error: no joint selected
	}
}

void CMD_DeleteJoint::unexecute(){
	if(index != (unsigned long int)(-1)){
		jointForDeletion->parent->addChildAtIndex(jointForDeletion, index);
	}else{		
		jointForDeletion->children = children;
	}
}

CMD_DeleteJoint::~CMD_DeleteJoint(void){
	if(executed){
		if(index == (unsigned long int)(-1)){
			// Delete children for good if they were actually deleted
			while(children.size() > 0){
				delete children.back();
				children.pop_back();
			}
		}else{
			// Delete node for good if it was actually deleted
			delete jointForDeletion;
			jointForDeletion = nullptr;
		}
	}
}