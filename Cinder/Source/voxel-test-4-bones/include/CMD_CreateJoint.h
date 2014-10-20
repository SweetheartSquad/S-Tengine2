#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"

class Node;
class Joint;

class CMD_CreateJoint :
	public Command
{
public:
	CMD_CreateJoint(std::vector<Joint*> * _joints, ci::Vec3d _pos, Joint * _parent);
	~CMD_CreateJoint(void);

	void execute();
	void unexecute();

private:
	std::vector<Joint*> * joints;
	ci::Vec3d pos;
	Joint * parent;
	Joint * createdJoint;

	// False on creation, true after call to execute, false after call to unexecute
	bool executed;
};

