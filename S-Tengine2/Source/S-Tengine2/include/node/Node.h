#pragma once

#include <vector>
#include <ostream>

class NodeShadable;
class NodeUI;
class NodePhysicsBody;
class NodeLoadable;
class NodeResource;
class NodeChild;
class NodeUpdatable;
class NodeRenderable;

typedef enum {
	kNODE				= (1 << 0),
	kNODE_RENDERABLE	= (1 << 1),
	kNODE_UPDATABLE		= (1 << 2),
	kNODE_CHILD			= (1 << 3),
	kNODE_RESOURCE		= (1 << 4),
	kNODE_LOADABLE		= (1 << 5),
	kNODE_UI			= (1 << 6),
	kNODE_SHADABLE		= (1 << 7)
} NodeType;

/** Abstract node */
class Node abstract{
public:
#ifdef _DEBUG
	static bool nodeCounting;
	static std::vector<Node *> nodes;
#endif

	unsigned int nodeType;

	std::string name;

	Node();
	virtual ~Node();

	bool isNodeType(NodeType _type) const;

	friend std::ostream& operator<<(std::ostream& os, const Node& obj){
		return os;
	}

	NodeRenderable  * asNodeRenderable() const;
	NodeUpdatable   * asNodeUpdatable() const;
	NodeChild	    * asNodeChild() const;
	NodeResource    * asNodeResource() const;
	NodeLoadable    * asNodeLoadable() const;
	NodeUI			* asNodeUI() const;
	NodeShadable    * asNodeShadable() const;

protected:
	NodeRenderable  * ptrNodeRenderable;
	NodeUpdatable   * ptrNodeUpdatable;
	NodeChild	    * ptrNodeChild;
	NodeResource    * ptrNodeResource;
	NodeLoadable    * ptrNodeLoadable;
	NodeUI			* ptrNodeUI;
	NodeShadable    * ptrNodeShadable;
};