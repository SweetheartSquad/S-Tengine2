#pragma once

#include <vector>

#include "Command.h"

class Joint;

class CMD_DeleteJoints : public Command{
public:
	CMD_DeleteJoints(std::vector<Joint *> * joints);
	~CMD_DeleteJoints(void);

	void execute();
	void unexecute();

private:
	std::vector<Joint *> * joints;
	
};