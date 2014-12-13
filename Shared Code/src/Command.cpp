#pragma once

#include "Command.h"
#include <sstream>

Command::Command() :
	executed(false),
	firstRun(true)
{
}

Command::~Command(){
}

void Command::log(std::string _message){
	subCmdProc.log(_message);
}

void Command::warn(std::string _message){
	subCmdProc.warn(_message);
}

void Command::error(std::string _message){
	subCmdProc.error(_message);
}