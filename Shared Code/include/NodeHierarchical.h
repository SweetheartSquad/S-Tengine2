#pragma once


#include "Node.h"
#include "NodeParent.h"
#include "NodeChild.h"

/** Abstract node with a parent and list of children */
class NodeHierarchical abstract : public virtual NodeParent, public virtual NodeChild{
public:
	NodeHierarchical();

	// Calls deleteRecursively on all of _node's children, and then deletes _node
	static void deleteRecursively(NodeParent * _node);
	//explicit NodeHierarchical(NodeParent * _parent) : NodeChild(_parent){};

	// Loops through the node's children and removes the first instance of _child
	// Pushes _child onto the node's children stack
	// Sets _child->parent = this node
	// Note: does nothing if this node is a descendant of _child
	// Returns whether or not the child was successfully added
	virtual bool addChild(NodeChild * _child);

	// Returns the number of Nodes between this node and the top of its hierarchy
	virtual unsigned long int calculateDepth();
};