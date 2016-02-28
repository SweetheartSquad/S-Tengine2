#pragma once

#include "CompressedCommand.h"

CompressedCommand::CompressedCommand(){
	firstRun = false;
}

bool CompressedCommand::execute(){
	subCmdProc.redoAll();
	return true;
}

bool CompressedCommand::unexecute(){
	subCmdProc.undoAll();
	return true;
}

CompressedCommand::~CompressedCommand(){
}