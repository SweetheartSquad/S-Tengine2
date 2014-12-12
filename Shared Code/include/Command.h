#pragma once

#include <vector>
#include "CommandProcessor.h"
#include "ConsoleEntry.h"
#include "Node.h"

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

	

	std::vector<ConsoleEntry> consoleEntries;

	void log(std::string _message);
	void warn(std::string _message);
	void error(std::string _message);
};
