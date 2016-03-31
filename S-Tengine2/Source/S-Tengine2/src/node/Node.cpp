#pragma once

#include <node/Node.h>

#ifdef _DEBUG
std::vector<Node *>Node::nodes;
bool Node::nodeCounting = false;
#endif

Node::~Node(){
#ifdef _DEBUG
	if(nodeCounting){
		for(signed long int j = nodes.size()-1; j >= 0; --j){
			if(nodes.at(j) == this){
				nodes.erase(nodes.begin() + j);
			}
		}
	}
#endif
}

NodeRenderable * Node::asNodeRenderable() const {
	return ptrNodeRenderable;
}

NodeUpdatable * Node::asNodeUpdatable() const {
	return ptrNodeUpdatable;
}

NodeChild * Node::asNodeChild() const {
	return ptrNodeChild;
}

NodeResource * Node::asNodeResource() const {
	return ptrNodeResource;
}

NodeLoadable * Node::asNodeLoadable() const {
	return ptrNodeLoadable;
}

NodeUI * Node::asNodeUI() const {
	return ptrNodeUI;
}

NodeShadable * Node::asNodeShadable() const {
	return ptrNodeShadable;
}

Transform * Node::asTransform() const {
	return ptrTransform;
}

bool Node::isNodeType(NodeType _type) const {
	return (nodeType & _type) != 0;
}

Node::Node() :
	nodeType(0),
	ptrNodeRenderable(nullptr),
	ptrNodeUpdatable(nullptr),
	ptrNodeChild(nullptr),
	ptrNodeResource(nullptr),
	ptrNodeLoadable(nullptr),
	ptrNodeUI(nullptr),
	ptrNodeShadable(nullptr),
	ptrTransform(nullptr)
{
	nodeType |= kNODE;
#ifdef _DEBUG
	name = "UNNAMED";
	if(nodeCounting){
		nodes.push_back(this);
	}
#endif
}