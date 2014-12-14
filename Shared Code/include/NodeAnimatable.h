#pragma once

#include "NodeTransformable.h"
#include "NodeUpdatable.h"
#include <glm\glm.hpp>

class Transform;
class Step;

template<typename T>
class Animation;

class NodeAnimatable abstract : public virtual NodeTransformable, public virtual NodeUpdatable{

public:
	explicit NodeAnimatable();
	~NodeAnimatable();

	Animation<float> * translateX;
	Animation<float> * translateY;
	Animation<float> * translateZ;
	Animation<glm::quat> * rotate;
	Animation<float> * scaleX;
	Animation<float> * scaleY;
	Animation<float> * scaleZ;

	void update(Step * _step) override;
};