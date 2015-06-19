#pragma once

#include <node/Node.h>

class NodeMeasurable abstract : public Node{
protected:
	float measuredWidth, measuredHeight;
	
	virtual float getMeasuredWidth();
	virtual float getMeasuredHeight();
	virtual void setMeasuredWidth(float _width);
	virtual void setMeasuredHeight(float _height);
};