#pragma once

#include "NodeParent.h"

#include "NodeChild.h"

void NodeParent::addChild(NodeChild * _child){
	for (unsigned long int i = 0; i < children.size(); ++i){
		if (_child == children.at(i)){
			children.erase(children.begin() + i);
			break;
		}
	}

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