#pragma once

#include <cinder/app/AppBasic.h>

#include "CommandProcessor.h"
#include "Command.h"

CommandProcessor::CommandProcessor(void){
}

void CommandProcessor::executeCommand(Command * c){
	ci::app::console() << "executeCommand" << std::endl;
	c->execute();
	undoStack.push_back(c);
	redoStack.clear();
}

void CommandProcessor::undo(){
	ci::app::console() << "undo" << std::endl;
	if (undoStack.size() > 0){
		undoStack.back()->unexecute();
		redoStack.push_back(undoStack.back());
		undoStack.pop_back();
	}
}

void CommandProcessor::redo(){
	if (redoStack.size() > 0){
		redoStack.back()->execute();
		undoStack.push_back(redoStack.back());
		redoStack.pop_back();
	}
}

CommandProcessor::~CommandProcessor(void){
	for(unsigned long int i = 0; i < undoStack.size(); ++i){
		delete undoStack.at(i);
	}
	undoStack.clear();

	for(unsigned long int i = 0; i < redoStack.size(); ++i){
		delete redoStack.at(i);
	}
	redoStack.clear();
}
