#pragma once

#include "Command.h"

class NodeParent;

class CMD_ParentSelectedNodes : public Command{
public:
	CMD_ParentSelectedNodes(NodeParent * _parent);
	~CMD_ParentSelectedNodes();

	void execute();
	void unexecute();

	NodeParent * parent;
};