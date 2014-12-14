#pragma once

#include "Commands/CMD_DeleteJoint.h"
#include "Joint.h"

CMD_DeleteJoint::CMD_DeleteJoint(Joint * _jointForDeletion) :
	jointForDeletion(_jointForDeletion),
	index(-1)
{
}

bool CMD_DeleteJoint::execute(){
	if(jointForDeletion != nullptr){
		if(jointForDeletion->children.size() > 0){
			children = jointForDeletion->children;
			jointForDeletion->children.clear();
		}else{
			index = jointForDeletion->parent->removeChild(jointForDeletion);
		}
	}else{
		error("Null joint cannot be deleted");
		return false;
	}
	return true;
}

bool CMD_DeleteJoint::unexecute(){
	if(jointForDeletion != nullptr){
		if(index != (unsigned long int)(-1)){
			jointForDeletion->parent->addChildAtIndex(jointForDeletion, index);
		}else{		
			jointForDeletion->children = children;
		}
		error("Null joint cannot be deleted");
		return false;
	}
	return true;
}

CMD_DeleteJoint::~CMD_DeleteJoint(void){
	if(executed){
		if(index == (unsigned long int)(-1)){
			// Delete children for good if they were actually deleted
			while(children.size() > 0){
				NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(children.back());
				if(nh){
					NodeHierarchical::deleteRecursively(nh);
				}else{
					delete children.back();
				}children.pop_back();
			}
		}else{
			// Delete node for good if it was actually deleted
			NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(children.back());
			if(nh){
				NodeHierarchical::deleteRecursively(nh);
			}else{
				delete jointForDeletion;
			}
			jointForDeletion = nullptr;
		}
	}
}