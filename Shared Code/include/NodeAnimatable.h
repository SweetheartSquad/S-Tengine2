#pragma once

#include "NodeTransformable.h"
#include "Animation.h"

class Transform;
class Step;

class NodeAnimatable abstract : public virtual NodeTransformable, public virtual NodeUpdatable{

public:
	explicit NodeAnimatable();
	~NodeAnimatable();

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

	virtual void update(Step * _step) override;
};