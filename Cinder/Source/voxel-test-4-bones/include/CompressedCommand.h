#pragma once

#include "Command.h"

class CompressedCommand : public Command{
public:
	CompressedCommand();
	~CompressedCommand();

	void execute();
	void unexecute();
};