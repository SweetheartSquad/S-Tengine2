#pragma once

#include "NodeHierarchical.h"

NodeHierarchical::NodeHierarchical() : NodeChild(nullptr){}

void NodeHierarchical::deleteRecursively(NodeHierarchical * _j){
	for (unsigned long int i = 0; i < _j->children.size(); ++i){
		deleteRecursively(dynamic_cast<NodeHierarchical *>(_j->children.at(i)));
	}
	delete _j;
}
