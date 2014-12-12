#pragma once

#include <typeinfo>

#include <cinder/app/AppBasic.h>

#include "CommandProcessor.h"
#include "Command.h"
#include "CompressedCommand.h"
#include "ConsoleGUI.h"

CommandProcessor::CommandProcessor(void) :
	currentCompressedCommand(nullptr)
{
}

void CommandProcessor::executeCommand(Command * c){
	if(currentCompressedCommand != nullptr){
		currentCompressedCommand->subCmdProc.executeCommand(c);
		currentCompressedCommand->firstRun = true;
	}else{
		redoStack.push_back(c);
		redo();
		c->firstRun = false;
	}

	// Executing a new command will always clear the redoStack
	while(redoStack.size() > 0){
		delete redoStack.back();
		redoStack.pop_back();
	}
}

void CommandProcessor::startCompressing(){
	if(currentCompressedCommand == nullptr){
		CompressedCommand * c = new CompressedCommand();
		currentCompressedCommand = c;
	}else{
		// Error: already compressing
	}
}

void CommandProcessor::endCompressing(){
	if(currentCompressedCommand != nullptr){
		if(currentCompressedCommand->firstRun){
			undoStack.push_back(currentCompressedCommand);
			redoStack.clear();
		}else{
			delete currentCompressedCommand;
		}
		currentCompressedCommand = nullptr;
	}
}

void CommandProcessor::undo(){
	if (undoStack.size() != 0){
		Command * c = undoStack.back();
		if(c->unexecute()){
			c->executed = false;
			redoStack.push_back(c);
		}
		undoStack.pop_back();
		// Log command's entries
		consoleEntries.insert(consoleEntries.end(), c->consoleEntries.begin(), c->consoleEntries.end());
	}
}

void CommandProcessor::redo(){
	if (redoStack.size() != 0){
		Command * c = redoStack.back();
		if(c->execute()){
			c->executed = true;
			undoStack.push_back(c);
		}
		redoStack.pop_back();
		// Log command's entries
		consoleEntries.insert(consoleEntries.end(), c->consoleEntries.begin(), c->consoleEntries.end());
	}
}

void CommandProcessor::undoAll(){
	while (undoStack.size() != 0){
		undo();
	}
}

void CommandProcessor::redoAll(){
	while (redoStack.size() != 0){
		redo();
	}
}

void CommandProcessor::reset(){
	while(undoStack.size() > 0){
		delete undoStack.back();
		undoStack.pop_back();
	}
	
	while(redoStack.size() > 0){
		delete redoStack.back();
		redoStack.pop_back();
	}
}


CommandProcessor::~CommandProcessor(void){
	reset();
}


void CommandProcessor::log(std::string _message){
	consoleEntries.push_back(ConsoleEntry(_message, ConsoleEntry::Type::kLOG));
}

void CommandProcessor::warn(std::string _message){
	consoleEntries.push_back(ConsoleEntry(_message, ConsoleEntry::Type::kWARNING));
}

void CommandProcessor::error(std::string _message){
	consoleEntries.push_back(ConsoleEntry(_message, ConsoleEntry::Type::kERROR));
}