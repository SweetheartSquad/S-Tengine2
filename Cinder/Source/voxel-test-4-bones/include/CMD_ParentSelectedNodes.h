#pragma once

#include "Command.h"

class CMD_ParentSelectedNodes : public Command{
public:
	CMD_ParentSelectedNodes();
	~CMD_ParentSelectedNodes();

	void execute();
	void unexecute();

	bool executed = false;
};