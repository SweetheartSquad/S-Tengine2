#pragma once

#include <typeinfo>

#include <cinder/app/AppBasic.h>

#include "CommandProcessor.h"
#include "Command.h"

CommandProcessor::CommandProcessor(void){
}

void CommandProcessor::executeCommand(Command * c){
	ci::app::console() << "executeCommand: " << typeid(*c).name() << std::endl;
	c->execute();
	undoStack.push_back(c);

	// Executing a new command will always clear the redoStack
	for(unsigned long int i = 0; i < redoStack.size(); ++i){
		delete redoStack.at(i);
	}
	redoStack.clear();
}

void CommandProcessor::undo(){
	ci::app::console() << "undo: " << typeid(*undoStack.back()).name() << std::endl;
	if (undoStack.size() > 0){
		undoStack.back()->unexecute();
		redoStack.push_back(undoStack.back());
		undoStack.pop_back();
	}
}

void CommandProcessor::redo(){
	ci::app::console() << "redo: " << typeid(*redoStack.back()).name() << std::endl;
	if (redoStack.size() > 0){
		redoStack.back()->execute();
		undoStack.push_back(redoStack.back());
		redoStack.pop_back();
	}
}

void CommandProcessor::reset(){
	for(unsigned long int i = 0; i < undoStack.size(); ++i){
		delete undoStack.at(i);
	}
	undoStack.clear();

	for(unsigned long int i = 0; i < redoStack.size(); ++i){
		delete redoStack.at(i);
	}
	redoStack.clear();
}

CommandProcessor::~CommandProcessor(void){
	reset();
}
