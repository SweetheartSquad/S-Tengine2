#pragma once

#include "CompressedCommand.h"

CompressedCommand::CompressedCommand()
{
}

void CompressedCommand::execute(){
	for (unsigned int i = 0; i < subCommands.size(); ++i){
		subCommands.at(i)->execute();
	}
}

void CompressedCommand::unexecute(){
	for (unsigned int i = subCommands.size(); i > 0; --i){
		subCommands.at(i-1)->unexecute();
	}
}

CompressedCommand::~CompressedCommand()
{
}