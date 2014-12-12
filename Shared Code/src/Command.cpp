#pragma once

#include "Command.h"

Command::Command() :
	executed(false),
	firstRun(true)
{
}

Command::~Command(){
}

void Command::log(std::string _message){
	consoleEntries.push_back(ConsoleEntry(_message, ConsoleEntry::Type::kLOG));
}

void Command::warn(std::string _message){
	consoleEntries.push_back(ConsoleEntry(_message, ConsoleEntry::Type::kWARNING));
}

void Command::error(std::string _message){
	consoleEntries.push_back(ConsoleEntry(_message, ConsoleEntry::Type::kERROR));
}