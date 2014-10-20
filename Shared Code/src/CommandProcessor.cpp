#pragma once

#include "CommandProcessor.h"
#include <cinder/app/AppBasic.h>

using namespace ci;
CommandProcessor::CommandProcessor(void){
}

void CommandProcessor::executeCommand(Command * c){
	app::console() << "executeCommand" << endl;
	c->execute();
	undoStack.push_back(c);
	redoStack.clear();
}

void CommandProcessor::undo(){
	app::console() << "undo" << endl;
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
