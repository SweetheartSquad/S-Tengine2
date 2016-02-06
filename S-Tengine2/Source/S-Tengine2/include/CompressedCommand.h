#pragma once

#include "Command.h"

class CompressedCommand : public Command{
public:
	CompressedCommand();
	~CompressedCommand();

	bool execute();
	bool unexecute();
};