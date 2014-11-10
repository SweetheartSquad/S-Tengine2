#pragma once

#include "Node.h"

class Step;

class NodeUpdatable : public virtual Node{
	
public:

	virtual void update(Step * step) = 0;
};