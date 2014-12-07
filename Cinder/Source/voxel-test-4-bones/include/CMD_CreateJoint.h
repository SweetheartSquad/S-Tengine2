#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"
#include "SceneRoot.h"

class Node;
class Joint;

class CMD_CreateJoint : public Command{
public:
	CMD_CreateJoint(SceneRoot * _sceneRoot, ci::Vec3d _pos, Joint * _parent);
	~CMD_CreateJoint(void);

	void execute();
	void unexecute();

private:
	ci::Vec3d pos;
	Joint * parent;
	Joint * createdJoint;
	SceneRoot * sceneRoot;

	// False on creation, true after call to execute, false after call to unexecute
	bool executed;
};

