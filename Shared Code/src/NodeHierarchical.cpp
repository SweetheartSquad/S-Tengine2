#pragma once

#include "NodeHierarchical.h"

NodeHierarchical::NodeHierarchical() : NodeChild(nullptr){}

unsigned long int NodeHierarchical::calculateDepth(){
	unsigned long int depth = 0;
	NodeParent * np = parent;
	while(np != nullptr){
		NodeHierarchical * t = dynamic_cast<NodeHierarchical *>(np);
		if (t != NULL){
			np = t->parent;
			depth += 1;
		}else{
			break;
		}
	}
	return depth;
}

void NodeHierarchical::deleteRecursively(NodeHierarchical * _j){
	for (unsigned long int i = 0; i < _j->children.size(); ++i){
		deleteRecursively(dynamic_cast<NodeHierarchical *>(_j->children.at(i)));
	}
	delete _j;
}

void NodeHierarchical::addChild(NodeChild * _child){
	// Check to see if the child is one of the ancestors of this node
	bool error = false;
	NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(this->parent);
	while(nh != nullptr){
		if(nh == _child){
			error = true;
			break;
		}
		nh = dynamic_cast<NodeHierarchical *>(nh->parent);
	}

	if(!error){
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
	}else{
		// Error Message: Cannot parent a node to one of its descendants
		children.push_back(nullptr);
	}
}