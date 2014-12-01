#pragma once

#include "Node.h"
#include "NodeParent.h"

class NodeChild abstract : public virtual Node{
public:

	explicit NodeChild(NodeParent * _parent);


	/** Reference to this node's parent */
	NodeParent * parent;

private:

};