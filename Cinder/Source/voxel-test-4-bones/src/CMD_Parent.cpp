#pragma once

#include "CMD_Parent.h"
#include "UI.h"
#include "Node.h"

#include "NodeParent.h"
#include "NodeChild.h"

CMD_Parent::CMD_Parent(NodeChild * _node, NodeParent * _parent) :
	node(_node),
	newParent(_parent),
	oldParent(nullptr)
{
}

void CMD_Parent::execute(){
	if(node != nullptr){
		oldParent = node->parent;
		node->parent = newParent;

		if(newParent != nullptr){
			// check that it isn't already a child of the new parent
			bool isChild = false;
			for (unsigned long int i = 0; i < newParent->children.size(); ++i){
				if (node == newParent->children.at(i)){
					isChild = true;
					break;
				}
			}
			if(!isChild){
				newParent->children.push_back(node);
			}
		}
		if (oldParent != nullptr){
			for(unsigned long int i = 0; i < oldParent->children.size(); ++i){
				if(node == oldParent->children.at(i)){
					index = i;
					oldParent->children.erase(oldParent->children.begin() + index);
					break;
				}
			}
		}
	}
}

void CMD_Parent::unexecute(){
	if(oldParent != nullptr){
		oldParent->children.insert(oldParent->children.begin() + index, node);
	}
	if(newParent != nullptr){
		newParent->children.pop_back();
	}
	if(node != nullptr){
		node->parent = oldParent;
	}
}

CMD_Parent::~CMD_Parent(void){
}