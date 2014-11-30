#pragma once

#include "CMD_Parent.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"
#include "NodeHierarchical.h"

CMD_Parent::CMD_Parent(NodeHierarchical * _node, NodeHierarchical * _parent) :
	node(_node),
	oldParent(dynamic_cast<NodeHierarchical *>(_node)->parent),
	newParent(_parent)
{
}

void CMD_Parent::execute(){
	node->parent = newParent;

	newParent->children.push_back(node);
	if (oldParent != nullptr){
		//std::vector<NodeHierarchical *>::iterator = node;
		//oldParent->children.erase(oldParent->children.begin(), oldParent->children.end(), it);
//		oldParent->children.erase(std::find_if(oldParent->children.begin(), oldParent->children.end(), address_equals(node)));
	}
}

void CMD_Parent::unexecute(){
	node->parent = oldParent;
}

CMD_Parent::~CMD_Parent(void){
}