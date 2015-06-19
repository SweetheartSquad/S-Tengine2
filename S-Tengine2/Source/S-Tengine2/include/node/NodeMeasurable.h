#pragma once

#include <node/Node.h>

class NodeMeasurable abstract : public Node{
public:
	float measuredWidth, measuredHeight;
	
	virtual float getWidth() = 0;
	virtual float getHeight() = 0;
};