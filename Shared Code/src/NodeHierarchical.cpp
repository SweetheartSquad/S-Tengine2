#pragma once

#include "NodeHierarchical.h"

void NodeHierarchical::deleteRecursively(NodeHierarchical * _j){
	for(unsigned long int i = 0; i < _j->children.size(); ++i){
		deleteRecursively(_j->children.at(i));
	}
	delete _j;
}