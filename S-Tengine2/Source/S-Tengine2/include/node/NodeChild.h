#pragma once

#include "node/Node.h"
#include "node/NodeParent.h"

class NodeChild abstract : public virtual Node{
public:

	explicit NodeChild(NodeParent * _parent = nullptr);

	// Returns whether or not _parent is an ancestor of this node (i.e. is its parent, is its parent's parent, etc.)
	// If _parent = nullptr, returns false
	virtual bool hasAncestor(NodeParent * _parent);

	/** Reference to this node's parent */
	NodeParent * parent;

private:

};