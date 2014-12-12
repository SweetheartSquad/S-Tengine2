#pragma once

#include "Command.h"
#include "Easing.h"
#include "UI.h"
#include <vector>

class NodeAnimatable;

class CMD_KeyAll : public Command
{
public:
	CMD_KeyAll(NodeAnimatable * _node, float _time);
	~CMD_KeyAll();

	bool execute();
	bool unexecute();

private:
	NodeAnimatable * node;
	float time;
	Easing::Type interpolation;
	
};