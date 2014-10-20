#pragma once

#include <vector>

#include "Command.h"

class Joint;

class CMD_SetParent : public Command{
public:
	CMD_SetParent(/*std::vector<Joint *> * joints*/);
	~CMD_SetParent(void);

	void execute();
	void unexecute();

private:
	//std::vector<Joint *> * joints;
};