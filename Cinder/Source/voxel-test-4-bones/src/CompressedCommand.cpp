#pragma once

#include "CompressedCommand.h"

CompressedCommand::CompressedCommand(){
	firstRun = false;
}

void CompressedCommand::execute(){
	subCmdProc.redoAll();
}

void CompressedCommand::unexecute(){
	subCmdProc.undoAll();
}

CompressedCommand::~CompressedCommand(){
}