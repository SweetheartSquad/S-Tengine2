#pragma once

#include "Command.h"
#include "Easing.h"
#include "UI.h"
#include <vector>

class NodeAnimatable;

class CMD_KeyAll : public Command
{
public:
	CMD_KeyAll(float _time);
	~CMD_KeyAll();

	void execute();
	void unexecute();

private:
	float time;
	Easing::Type interpolation;
	
};