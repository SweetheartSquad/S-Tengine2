#pragma once

#include <vector>
#include "CommandProcessor.h"
#include "node/Node.h"

class Command : public Node{
public:
	virtual bool execute() = 0;
	virtual bool unexecute() = 0;
	
	Command();
	virtual ~Command();

	CommandProcessor subCmdProc;

	// False on creation, true after call to execute, false after call to unexecute
	bool executed;
	// True from creation up until the end of the first call to execute, false otherwise
	bool firstRun;
};
