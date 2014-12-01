#pragma once

#include <vector>

#include "Command.h"

class Joint;
class NodeChild;
class NodeParent;

class CMD_Parent : public Command{
public:
	CMD_Parent(NodeChild * _node, NodeParent * _parent);
	~CMD_Parent(void);

	void execute();
	void unexecute();

private:
	NodeChild * node;

	NodeParent * oldParent;
	NodeParent * newParent;
};