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