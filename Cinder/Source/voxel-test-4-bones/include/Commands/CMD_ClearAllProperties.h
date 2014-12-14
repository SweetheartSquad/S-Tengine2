#pragma once

#include "Command.h"
#include "Easing.h"
#include "UI.h"
#include <vector>

class NodeAnimatable;

class CMD_ClearAllProperties : public Command
{
public:
	CMD_ClearAllProperties(NodeAnimatable * _node, float _time);
	~CMD_ClearAllProperties();

	bool execute();
	bool unexecute();

private:
	NodeAnimatable * node;
	float time;
	
};