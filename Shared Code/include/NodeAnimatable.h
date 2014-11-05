#pragma once

#include "NodeTransformable.h"
#include "Animation.h"

class Transform;

class NodeAnimatable : public NodeTransformable{

public:

	explicit NodeAnimatable(Transform * _transform);
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