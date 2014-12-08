#pragma once

#include <typeinfo>

#include <cinder/app/AppBasic.h>

#include "CommandProcessor.h"
#include "Command.h"
#include "CompressedCommand.h"

CommandProcessor::CommandProcessor(void) :
	currentCompressedCommand(nullptr)
{
}

void CommandProcessor::executeCommand(Command * c){
	if(currentCompressedCommand != nullptr){
		currentCompressedCommand->subCmdProc.executeCommand(c);
		currentCompressedCommand->firstRun = true;
	}else{
		c->execute();
		c->executed = true;
		c->firstRun = false;
		undoStack.push_back(c);
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
		//ci::app::console() << "undo: " << typeid(*c).name() << std::endl;
		Command * c = undoStack.back();
		c->unexecute();
		c->executed = false;
		redoStack.push_back(c);
		undoStack.pop_back();
	}
}

void CommandProcessor::undoAll(){
	while (undoStack.size() != 0){
		//ci::app::console() << "undo: " << typeid(*c).name() << std::endl;
		Command * c = undoStack.back();
		c->unexecute();
		c->executed = false;
		redoStack.push_back(c);
		undoStack.pop_back();
	}
}

void CommandProcessor::redo(){
	if (redoStack.size() != 0){
		//ci::app::console() << "redo: " << typeid(*c).name() << std::endl;
		Command * c = redoStack.back();
		c->execute();
		c->executed = true;
		undoStack.push_back(c);
		redoStack.pop_back();
	}
}
void CommandProcessor::redoAll(){
	while (redoStack.size() != 0){
		//ci::app::console() << "redo: " << typeid(*c).name() << std::endl;
		Command * c = redoStack.back();
		c->execute();
		c->executed = true;
		undoStack.push_back(c);
		redoStack.pop_back();
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
