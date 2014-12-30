#pragma once

#include "node/NodeUpdatable.h"

#include <Box2D/Box2D.h>

class Box2DSprite;

class Box2DWorld : public virtual NodeUpdatable{
public:

	b2World * world;

	int velocityIterations;
	int positionIterations;

	explicit Box2DWorld(b2Vec2 _gravityVector);
	~Box2DWorld();

	void update(Step* _step) override;

	void addToWorld(Box2DSprite * _sprite);
};
