#pragma once

#include "node/Node.h"

#include <vector>
#include <functional>

class NodeHierarchical;
class NodeChild;
class SceneRoot;
class NodeChild;

class NodeParent abstract : public virtual Node{
public:
	virtual ~NodeParent() = 0;

	// Loops through the node's children and removes the first instance of _child
	// Pushes _child onto the node's children stack
	// Sets _child->parent = this node
	// Returns whether or not the child was successfully added
	virtual bool addChild(NodeChild * _child);

	// Inserts _child into this node's list of children at _index
	// Note: does not make any checks, so cyclical references and what not are possible
	virtual void addChildAtIndex(NodeChild * _child, int _index);

	// Erases the node at _index from the list of children
	virtual void removeChildAtIndex(int _index);
	
	// Loops through the node's children and removes the first instance of _child and returns the index
	// Does NOT change node->parent
	// Returns (unsigned long int)(-1) if _child is not a child of this node
	virtual unsigned long int removeChild(NodeChild * _child);
	
	// Returns whether _child is a direct child of this node
	// If _child = nullptr, returns false
	virtual bool hasChild(NodeChild * _child);
	// Returns whether _child is a descendant of this node (i.e. a child, a child of its children, etc)
	// If _child = nullptr, returns false
	virtual bool hasDescendant(NodeChild * _child);

	/** List of references to child entities */
	std::vector<NodeChild *> children;

	void doRecursivley(std::function<void(Node *, void * args[])> _toDo, void * _args[]);
};