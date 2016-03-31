#pragma once

#include <node/Node.h>

class Step;

class NodeUpdatable abstract : public virtual Node{
public:
	NodeUpdatable();
	virtual void update(Step * _step) = 0;
};