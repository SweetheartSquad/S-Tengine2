#pragma once

#include "SceneRoot.h"
#include "NodeHierarchical.h"

SceneRoot::~SceneRoot(){
	while(children.size() > 0){
		NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(children.back());
		if(nh != nullptr){
			NodeHierarchical::deleteRecursively(nh);
		}else{
			delete children.back();
		}
		children.pop_back();
	}
}