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
	//ci::app::console() << "executeCommand: " << typeid(*c).name() << std::endl;
	c->execute();
	if(currentCompressedCommand != NULL){
		currentCompressedCommand->subCommands.push_back(c);
	}else{
		undoStack.push_back(c);
	}

	// Executing a new command will always clear the redoStack
	for(unsigned long int i = redoStack.size(); i > 0; --i){
		delete redoStack.at(i-1);
		redoStack.at(i-1) = nullptr;
	}
	redoStack.clear();
}

void CommandProcessor::undo(){
	if (undoStack.size() != 0){
		//ci::app::console() << "undo: " << typeid(*c).name() << std::endl;
		Command * c = undoStack.back();
		c->unexecute();
		redoStack.push_back(c);
		undoStack.pop_back();
	}
}

void CommandProcessor::redo(){
	if (redoStack.size() != 0){
		//ci::app::console() << "redo: " << typeid(*c).name() << std::endl;
		Command * c = redoStack.back();
		c->execute();
		undoStack.push_back(c);
		redoStack.pop_back();
	}
}

void CommandProcessor::reset(){
	for(unsigned long int i = 0; i < undoStack.size(); ++i){
		delete undoStack.at(i);
		undoStack.at(i) = nullptr;
	}
	undoStack.clear();

	for(unsigned long int i = redoStack.size(); i > 0; --i){
		delete redoStack.at(i-1);
		redoStack.at(i-1) = nullptr;
	}
	redoStack.clear();
}

void CommandProcessor::startCompressing(){

	CompressedCommand * c = new CompressedCommand();
	currentCompressedCommand = c;
}

void CommandProcessor::endCompressing(){
	if(currentCompressedCommand != nullptr){
		if(currentCompressedCommand->subCommands.size() > 0){
			undoStack.push_back(currentCompressedCommand);
			redoStack.clear();
		}else{
			delete currentCompressedCommand;
		}
		currentCompressedCommand = nullptr;
	}
}

CommandProcessor::~CommandProcessor(void){
	reset();
}
