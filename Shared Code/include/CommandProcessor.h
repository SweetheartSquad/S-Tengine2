#pragma once

#include <vector>

class Command;
class CompressedCommand;

class CommandProcessor{
public:
	CommandProcessor(void);
	~CommandProcessor(void);

	void executeCommand(Command * c);
	// Runs "unexecute()" on the last command in the undoStack, pops it off the stack, and pushes it onto the redoStack
	void undo();
	// Runs "execute()" on the last command in the redoStack, pops it off the stack, and pushes it onto the undoStack
	void redo();
	// Deletes and clears the contents of the undo and redo stacks
	void reset();
	
	void startCompressing();
	void endCompressing();
private:
	std::vector<Command *> undoStack;
	std::vector<Command *> redoStack;
	
	CompressedCommand * currentCompressedCommand;
};
