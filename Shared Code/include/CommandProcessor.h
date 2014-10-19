#pragma once

#include "Command.h"
#include <vector>

using namespace std;

class CommandProcessor{
public:
	CommandProcessor(void);
	~CommandProcessor(void);

	void executeCommand(Command * c);
	void undo();
	void redo();
private:
	vector<Command *> undoStack;
	vector<Command *> redoStack;
};

