#pragma once

#include "Command.h"

class NodeParent;
class SceneRoot;

class CMD_ParentSelectedNodes : public Command{
public:
	CMD_ParentSelectedNodes(SceneRoot * _sceneRoot, NodeParent * _parent);
	~CMD_ParentSelectedNodes();

	void execute();
	void unexecute();

	NodeParent * parent;
	SceneRoot * sceneRoot;
};