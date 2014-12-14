#pragma once

#include <vector>

#include "Command.h"

class Joint;

class CMD_DeleteJoints : public Command{
public:
	CMD_DeleteJoints();
	~CMD_DeleteJoints(void);

	bool execute();
	bool unexecute();
};