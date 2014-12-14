#pragma once

#include "Command.h"
#include "Easing.h"
#include "UI.h"
#include <vector>

class NodeAnimatable;

class CMD_KeyAllProperties : public Command
{
public:
	CMD_KeyAllProperties(NodeAnimatable * _node, float _time);
	~CMD_KeyAllProperties();

	bool execute();
	bool unexecute();

private:
	NodeAnimatable * node;
	float time;
	Easing::Type interpolation;
	
};