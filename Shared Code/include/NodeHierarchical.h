#pragma once

#include <vector>

#include "Node.h"

/** Abstract node with a parent and list of children */
class NodeHierarchical abstract : public virtual Node{
public:
	explicit NodeHierarchical(NodeHierarchical * _parent) : parent(_parent){};
	
	// Recursively delete all the children of a node, then delete the node itself
	static void deleteRecursively(NodeHierarchical * _j);

	virtual void addChild(NodeHierarchical * _child);
	virtual void removeChildAtIndex(int _index);

	/** Reference to this node's parent */
	NodeHierarchical * parent;
	/** List of references to child entities */
	std::vector<NodeHierarchical *> children;
};