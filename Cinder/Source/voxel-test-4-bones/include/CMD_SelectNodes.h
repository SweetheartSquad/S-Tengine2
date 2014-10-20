#pragma once

#include "Command.h"

class Node;

class CMD_SelectNodes : public Command{
public:
	std::vector<Node *> previousSelectedNodes;
	std::vector<Node *> nodesForSelection;
	
	CMD_SelectNodes(std::vector<Node *> _nodes);
	// Select an individual node (if _node == nullptr, deselects everything)
	CMD_SelectNodes(Node * _node = nullptr);
	~CMD_SelectNodes();
	
	void execute();
	void unexecute();
};