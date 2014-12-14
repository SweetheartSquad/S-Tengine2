#pragma once

#include "NodeParent.h"

#include "NodeChild.h"
#include "NodeHierarchical.h"

bool NodeParent::addChild(NodeChild * _child){
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
	return true;
}

void NodeParent::addChildAtIndex(NodeChild * _child, int _index){
	children.insert(children.begin() + _index, _child);
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

bool NodeParent::hasChild(NodeChild * _child){
	if(_child == nullptr){
		return false;
	}
	for (unsigned long int i = 0; i < children.size(); ++i){
		if (_child == children.at(i)){
			return true;
			break;
		}
	}
	return false;
}

bool NodeParent::hasDescendant(NodeChild *_child) {
	if(_child == nullptr){
		return false;
	}
	for (unsigned long int i = 0; i < children.size(); ++i){
		if (_child == children.at(i)){
			return true;
			break;
		}else{
			NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(children.at(i));
			if(nh != nullptr){
				if(nh->hasDescendant(_child));
			}
		}
	}
	return false;
}

void NodeParent::doRecursivley(std::function<void(Node *, void * args[])> _toDo, void * _args[]){
	_toDo(this, _args);
	for(unsigned long int i = 0; i < children.size(); i++){
		NodeParent * np = dynamic_cast<NodeParent *>(children.at(i));
		if(np != nullptr){
			np->doRecursivley(_toDo, _args);
		}else{
			_toDo(children.at(i), _args);
		}
	}
}

NodeParent::~NodeParent()
{
}