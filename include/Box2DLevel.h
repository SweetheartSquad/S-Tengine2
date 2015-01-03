#pragma once

#include <vector>

#include "node/NodeUpdatable.h"
#include "Box2DWorld.h"

class Box2DLevel : public virtual NodeUpdatable{
public:
	std::vector<Box2DSprite *> sprites;
	Box2DWorld world;

	Box2DLevel();
	~Box2DLevel();

	virtual void update(Step * _step) override;
};