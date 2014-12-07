#pragma once

#include "Command.h"

Command::Command() :
	executed(false),
	firstRun(true)
{
}

Command::~Command(){
}