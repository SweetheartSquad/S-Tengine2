#pragma once

#include "Command.h"

class NodeParent;
class SceneRoot;

class CMD_ParentSelectedNodes : public Command{
public:
	CMD_ParentSelectedNodes(SceneRoot * _sceneRoot, NodeParent * _parent);
	~CMD_ParentSelectedNodes();

	bool execute();
	bool unexecute();

	NodeParent * parent;
	SceneRoot * sceneRoot;
};