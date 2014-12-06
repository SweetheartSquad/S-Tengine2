#pragma once

#include "CMD_Parent.h"
#include "UI.h"
#include "Node.h"

#include "NodeParent.h"
#include "NodeChild.h"

CMD_Parent::CMD_Parent(NodeChild * _node, NodeParent * _parent) :
	node(_node),
	newParent(_parent)
{
}

void CMD_Parent::execute(){
	if(node != nullptr){
		oldParent = node->parent;
		node->parent = newParent;

		newParent->children.push_back(node);
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
	oldParent->children.insert(oldParent->children.begin() + index, node);
	newParent->children.pop_back();
	node->parent = oldParent;
}

CMD_Parent::~CMD_Parent(void){
}