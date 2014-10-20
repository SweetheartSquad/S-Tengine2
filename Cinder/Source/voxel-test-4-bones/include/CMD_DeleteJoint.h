#pragma once

#include <vector>

#include "Command.h"

class Joint;

class CMD_DeleteJoint : public Command{
public:
	CMD_DeleteJoint(std::vector<Joint *> * joints);
	~CMD_DeleteJoint(void);

	void execute();
	void unexecute();

private:
	std::vector<Joint *> * joints;
	Joint * jointForDeletion;
	// Location in jointForDeletion's parent's list of children (or the joint list if it was a root) at which jointForDeletion exists
	unsigned long int index;

	std::vector<Joint *> children;
};