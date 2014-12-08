#pragma once

#include "NodeChild.h"
#include "NodeHierarchical.h"

NodeChild::NodeChild(NodeParent * _parent):
	parent(_parent)
{
}

bool NodeChild::hasAncestor(NodeParent * _parent){
	if(_parent == nullptr){
		return false;
	}
	if(_parent == parent){
		return true;
	}
	NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(this->parent);
	while(nh != nullptr){
		if(nh == _parent){
			return true;
		}
		nh = dynamic_cast<NodeHierarchical *>(nh->parent);
	}

	return false;
}