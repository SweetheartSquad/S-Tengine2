#pragma once

#include "Command.h"

class Node;

class CMD_SelectNode : public Command{
public:
	Node * previousSelectedNode;
	Node * nodeForSelection;

	CMD_SelectNode(Node * _node);
	~CMD_SelectNode();
	
	void execute();
	void unexecute();
};