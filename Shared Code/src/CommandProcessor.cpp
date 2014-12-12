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

bool CommandProcessor::executeCommand(Command * c){
	if(currentCompressedCommand != nullptr){
		currentCompressedCommand->subCmdProc.executeCommand(c);
		currentCompressedCommand->firstRun = true;
	}else{
		redoStack.push_back(c);
		if(redo()){
			c->firstRun = false;
		}else{
			return false;
		}
	}

	// Executing a new command will always clear the redoStack
	while(redoStack.size() > 0){
		delete redoStack.back();
		redoStack.pop_back();
	}
	return true;
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

bool CommandProcessor::undo(){
	//log("undo");
	if (undoStack.size() != 0){
		Command * c = undoStack.back();
		bool success = c->unexecute();
		undoStack.pop_back();
		// Log command's entries
		for(unsigned long int i = 0; i < c->subCmdProc.consoleEntries.size(); ++i){
			consoleEntries.push_back(c->subCmdProc.consoleEntries.at(i));
		}
		c->subCmdProc.consoleEntries.clear();
		//consoleEntries.insert(consoleEntries.end(), c->subCmdProc.consoleEntries.begin(), c->subCmdProc.consoleEntries.end());
		if(success){
			c->executed = true;
			redoStack.push_back(c);
		}else{
			delete c;
		}
		return success;
	}
}

bool CommandProcessor::redo(){
	//log("redo");
	if (redoStack.size() != 0){
		Command * c = redoStack.back();
		bool success = c->execute();
		redoStack.pop_back();
		// Log command's entries
		for(unsigned long int i = 0; i < c->subCmdProc.consoleEntries.size(); ++i){
			consoleEntries.push_back(c->subCmdProc.consoleEntries.at(i));
		}
		c->subCmdProc.consoleEntries.clear();
		//consoleEntries.insert(consoleEntries.end(), c->subCmdProc.consoleEntries.begin(), c->subCmdProc.consoleEntries.end());
		if(success){
			c->executed = true;
			undoStack.push_back(c);
		}else{
			delete c;
		}
		return success;
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
	while(consoleEntries.size() > 0){
		delete consoleEntries.back();
		consoleEntries.pop_back();
	}
}

void CommandProcessor::log(std::string _message){
	std::stringstream t;
	t << "Log: ";
	t << _message;
	consoleEntries.push_back(new ConsoleEntry(t.str(), ConsoleEntry::Type::kLOG));
}

void CommandProcessor::warn(std::string _message){
	std::stringstream t;
	t << "Warning: ";
	t << _message;
	consoleEntries.push_back(new ConsoleEntry(t.str(), ConsoleEntry::Type::kWARNING));
}

void CommandProcessor::error(std::string _message){
	std::stringstream t;
	t << "Error: ";
	t << _message;
	consoleEntries.push_back(new ConsoleEntry(t.str(), ConsoleEntry::Type::kERROR));
}