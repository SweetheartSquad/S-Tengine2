#pragma once

#include <vector>

class Command;

class CommandProcessor{
public:
	CommandProcessor(void);
	~CommandProcessor(void);

	void executeCommand(Command * c);
	void undo();
	void redo();
private:
	std::vector<Command *> undoStack;
	std::vector<Command *> redoStack;
};
