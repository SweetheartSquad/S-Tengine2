#pragma once

#include <vector>

#include "Command.h"

class Joint;
class NodeHierarchical;

class CMD_Parent : public Command{
public:
	CMD_Parent(NodeHierarchical * node, NodeHierarchical * _parent);
	~CMD_Parent(void);

	void execute();
	void unexecute();

private:
	NodeHierarchical * node;

	NodeHierarchical * oldParent;
	NodeHierarchical * newParent;
};