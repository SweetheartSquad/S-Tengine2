#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"

class Joint;

class CreateJointCommand :
	public Command
{
public:
	CreateJointCommand(std::vector<Joint*> &joints, ci::Vec3d pos, Joint * parent);
	~CreateJointCommand(void);

	void execute();
	void unexecute();

private:
	std::vector<Joint*> * joints;
	ci::Vec3d pos;
	Joint * parent;
};

