#pragma once

#include "Command.h"

class Node;

class CMD_SelectNodes : public Command{
public:
	bool additive;
	bool subtractive;

	std::vector<Node *> previousSelectedNodes;
	std::vector<Node *> nodesForSelection;
	
	CMD_SelectNodes(std::vector<Node *> _nodes, bool _additive = false, bool _subtractive = false);
	// Select an individual node (if _node == nullptr, deselects everything)
	CMD_SelectNodes(Node * _node = nullptr, bool _additive = false, bool _subtractive = false);
	~CMD_SelectNodes();
	
	bool execute();
	bool unexecute();
};