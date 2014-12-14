#pragma once

#include "Node.h"

class Step;

class NodeUpdatable abstract : public virtual Node{
	
public:

	virtual void update(Step * _step) = 0;
};