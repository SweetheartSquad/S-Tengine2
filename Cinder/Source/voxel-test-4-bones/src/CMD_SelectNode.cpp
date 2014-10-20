#pragma once

#include "CMD_SelectNode.h"
#include "UI.h"

CMD_SelectNode::CMD_SelectNode(Node * _node) :
	previousSelectedNode(nullptr),
	nodeForSelection(_node)
{
}

CMD_SelectNode::~CMD_SelectNode(){
	previousSelectedNode = nullptr;
	nodeForSelection = nullptr;
}

void CMD_SelectNode::execute(){
	previousSelectedNode = UI::selectedNode;
	UI::selectedNode = nodeForSelection;
}

void CMD_SelectNode::unexecute(){
	UI::selectedNode = previousSelectedNode;
}