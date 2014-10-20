#pragma once

#include <vector>
#include "CommandProcessor.h"

class Command{
public:
	virtual void execute() = 0;
	virtual void unexecute() = 0;
	
	~Command();

	std::vector<Command *> subCommands;
};
