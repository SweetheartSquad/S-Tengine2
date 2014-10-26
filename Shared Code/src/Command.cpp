#pragma once

#include "Command.h"

Command::~Command(){
	for(unsigned long int i = 0; i < subCommands.size(); ++i){
		delete subCommands.at(i);
		subCommands.at(i) = nullptr;
	}
	subCommands.clear();
}