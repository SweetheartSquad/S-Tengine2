#pragma once

#include "Animation.h"
#include "Transform.h"

class NodeAnimatable
{
public:
	NodeAnimatable(Transform * _transform);
	~NodeAnimatable();

	Animation translateX;
	Animation translateY;
	Animation translateZ;
	Animation rotateX;
	Animation rotateY;
	Animation rotateZ;
	Animation rotateW;
	Animation scaleX;
	Animation scaleY;
	Animation scaleZ;
private:

};