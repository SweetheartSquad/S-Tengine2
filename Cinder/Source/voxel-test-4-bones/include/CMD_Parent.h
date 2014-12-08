#pragma once

#include <vector>

#include <glm\glm.hpp>

#include "Command.h"

class NodeChild;
class NodeParent;

class SceneRoot;

class CMD_Parent : public Command{
public:
	CMD_Parent(SceneRoot * _sceneRoot, NodeChild * _node, NodeParent * _parent);
	~CMD_Parent(void);

	void execute();
	void unexecute();

private:
	NodeChild * node;

	SceneRoot * sceneRoot;
	NodeParent * oldParent;
	NodeParent * newParent;

	unsigned long int index;

	glm::vec3 oldPos;
	bool error;
};