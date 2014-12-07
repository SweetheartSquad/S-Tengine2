#pragma once

#include <vector>

#include <glm\glm.hpp>

#include "Command.h"

class Joint;
class NodeParent;
class NodeChild;

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

	unsigned long int index;

	glm::vec3 oldPos;
};