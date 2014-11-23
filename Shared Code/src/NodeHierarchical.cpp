#pragma once

#include "NodeHierarchical.h"

void NodeHierarchical::deleteRecursively(NodeHierarchical * _j){
	for(unsigned long int i = 0; i < _j->children.size(); ++i){
		deleteRecursively(_j->children.at(i));
	}
	delete _j;
}

void NodeHierarchical::addChild(NodeHierarchical* _child){
	if(parent != nullptr){
		for(unsigned long int i = 0; i < parent->children.size(); ++i){
			if(_child == parent->children.at(i)){
				parent->children.erase(parent->children.begin() + i);
				break;
			}
		}
	}
	children.push_back(_child);
	_child->parent = this;
}

void NodeHierarchical::removeChildAtIndex(int _index){
	children.erase(children.begin() + _index - 1);
}