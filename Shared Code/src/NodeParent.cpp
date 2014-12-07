#pragma once

#include "NodeParent.h"

#include "NodeChild.h"
#include "NodeHierarchical.h"

void NodeParent::addChild(NodeChild * _child){
	// Remove the first instance of the child in the current list of children
	for (unsigned long int i = 0; i < children.size(); ++i){
		if (_child == children.at(i)){
			children.erase(children.begin() + i);
			break;
		}
	}

	// Add the child to the list of children and set it's parent to this
	children.push_back(_child);
	_child->parent = this;
}

void NodeParent::removeChildAtIndex(int _index){
	children.erase(children.begin() + _index - 1);
}


unsigned long int NodeParent::removeChild(NodeChild * _child){
	for(unsigned long int i = 0; i < children.size(); ++i){
		if(_child == children.at(i)){
			children.erase(children.begin() + i);
			return i;
		}
	}
	return -1;
}

NodeParent::~NodeParent()
{
}