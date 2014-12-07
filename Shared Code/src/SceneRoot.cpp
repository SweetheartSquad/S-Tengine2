#pragma once

#include "SceneRoot.h"
#include "NodeHierarchical.h"

SceneRoot::~SceneRoot(){
	for(unsigned long int i = 0; i < children.size(); ++i){
		NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(children.at(i));
		if(nh != nullptr){
			NodeHierarchical::deleteRecursively(nh);
		}else{
			delete children.at(i);
		}
	}
	children.clear();
}