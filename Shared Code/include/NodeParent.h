#pragma once

#include "Node.h"

#include <vector>

class NodeChild;

class NodeParent abstract : public virtual Node{
public:
	virtual ~NodeParent() = 0;

	// Recursively delete all the children of a node, then delete the node itself
	static void deleteRecursively(NodeParent * _j);

	virtual void addChild(NodeChild * _child);
	virtual void removeChildAtIndex(int _index);


	/** List of references to child entities */
	std::vector<NodeChild *> children;


private:

};