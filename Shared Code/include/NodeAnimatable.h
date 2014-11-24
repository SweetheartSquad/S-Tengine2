#pragma once

#include "NodeTransformable.h"
#include "Animation.h"

class Transform;
class Step;

class NodeAnimatable abstract : public NodeTransformable, NodeUpdatable{

public:
	explicit NodeAnimatable(Transform * _transform);

	Animation * translateX;
	Animation * translateY;
	Animation * translateZ;
	Animation * rotateX;
	Animation * rotateY;
	Animation * rotateZ;
	Animation * rotateW;
	Animation * scaleX;
	Animation * scaleY;
	Animation * scaleZ;

	void update(Step * _step) override;
};