#pragma once

#include <vector>

#include "Command.h"

class Joint;

class CMD_Parent : public Command{
public:
	CMD_Parent(std::vector<Joint *> * joints);
	~CMD_Parent(void);

	void execute();
	void unexecute();

private:
	std::vector<Joint *> * joints;
};