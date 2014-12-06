#pragma once

#include <vector>

#include "Command.h"

class Joint;
class NodeParent;
class NodeChild;

class CMD_Parent : public Command{
public:
	CMD_Parent(NodeChild * node, NodeParent * _parent);
	~CMD_Parent(void);

	void execute();
	void unexecute();

private:
	NodeChild * node;

	NodeParent * oldParent;
	NodeParent * newParent;
};