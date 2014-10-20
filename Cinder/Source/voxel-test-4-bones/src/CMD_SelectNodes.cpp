#pragma once

#include "CMD_SelectNodes.h"
#include "UI.h"

CMD_SelectNodes::CMD_SelectNodes(std::vector<Node *> _nodes) :
	nodesForSelection(_nodes)
{
}

CMD_SelectNodes::CMD_SelectNodes(Node * _node){
	if(_node != nullptr){
		nodesForSelection.push_back(_node);
	}
}

CMD_SelectNodes::~CMD_SelectNodes(){
	previousSelectedNodes.clear();
	nodesForSelection.clear();
}

void CMD_SelectNodes::execute(){
	previousSelectedNodes = UI::selectedNodes;
	UI::selectedNodes = nodesForSelection;
}

void CMD_SelectNodes::unexecute(){
	UI::selectedNodes = previousSelectedNodes;
}