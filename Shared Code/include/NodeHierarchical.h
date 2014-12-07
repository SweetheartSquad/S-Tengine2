#pragma once


#include "Node.h"
#include "NodeParent.h"
#include "NodeChild.h"

/** Abstract node with a parent and list of children */
class NodeHierarchical abstract : public virtual NodeParent, public virtual NodeChild{
public:
	NodeHierarchical();
	static void deleteRecursively(NodeHierarchical * _j);
	//explicit NodeHierarchical(NodeParent * _parent) : NodeChild(_parent){};

	// Loops through the node's children and removes the first instance of _child
	// Pushes _child onto the node's children stack
	// Sets _child->parent = this node
	// Note: does nothing if this node is a descendant of _child
	virtual void addChild(NodeChild * _child);

	// Returns the number of Nodes between this node and the top of its hierarchy
	virtual unsigned long int calculateDepth();
};