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
};